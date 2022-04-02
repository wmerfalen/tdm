#include "blind.hpp"
#include "affects.hpp"
#include "skills.hpp"
#include "armor.hpp"

#include "rooms.hpp"
#include "injure.hpp"
#include "doors.hpp"
#include "super-users.hpp"
#include "interpreter.hpp"
#include "weapons/damage-types.hpp"
#include "rand.hpp"
#include "weapons/damage-calculator.hpp"
#include "weapons/elemental.hpp"

#ifdef __MENTOC_SHOW_MODS_BLIND_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::blind::debug]")  << MSG << "\n";
#define m_error(MSG) mentoc_prefix_debug(red_str("[mods::blind::ERROR]"))  << MSG << "\n";
#else
#define m_debug(MSG) ;;
#define m_error(MSG) ;;
#endif


namespace mods::blind {
	using affect_vector_t = mods::affects::affect_vector_t;
	using affect_t = mods::affects::affect_t;
	using texture_type_t = room_data::texture_type_t;
	static uint64_t blind_id = 0;
	struct blind_damage_t {
		uint64_t id;
		uuid_t target;
		uint64_t stop_tick;
		bool cleanup;
		inline bool operator==(const blind_damage_t& lhs) {
			if(lhs.id == id) {
				return true;
			}
			return false;
		}
		std::string report() {
			return CAT("id:",id,"\ntarget:",target,"\nstop_tick:",stop_tick);
		}
		blind_damage_t() = delete;
		blind_damage_t(player_ptr_t victim,
		               uint64_t ticks) : id(++blind_id),
			target(victim->uuid()),stop_tick(ticks + CURRENT_TICK()),cleanup(false) {
			victim->affect(AFF_BLIND);
		}
		blind_damage_t(const blind_damage_t& other) {
			m_debug("[blind_damage_t COPY OTHER:" << other.id << "]");
			id = other.id;
			target = other.target;
			stop_tick = other.stop_tick;
			cleanup = other.cleanup;
		}
		~blind_damage_t() {
			m_debug("[~blind_damage_t]");
		}

	};
	using blind_list_t = std::vector<blind_damage_t>;

	static blind_list_t blind_player_list;

	void blind_for(player_ptr_t& victim, uint16_t ticks) {
		m_debug("blind_for cast on: " << victim->name() << ", for " << ticks << " ticks");

		auto goggles = victim->equipment(WEAR_GOGGLES);
		float tick_reduce = 0.0;
		int blind_ticks = ticks;
		if(goggles && goggles->has_armor()) {
			if(goggles->armor()->attributes->classification.compare("BASIC") == 0) {
				tick_reduce = 0.05;
			}
			if(goggles->armor()->attributes->classification.compare("ADVANCED") == 0) {
				tick_reduce = 0.75;
			}
			if(goggles->armor()->attributes->classification.compare("ELITE") == 0) {
				tick_reduce = 0.98;
			}
			switch((mw_armor)goggles->armor()->attributes->type) {
				case mw_armor::EYEWEAR:
				case mw_armor::GOGGLES:
					tick_reduce += 0.95;
					break;
				default:
					break;
			}
		}
		blind_ticks = ticks - (tick_reduce * ticks);
		if(blind_ticks > 0) {
			blind_player_list.emplace_back(victim,blind_ticks);
		}
	}
	void process_players() {
		m_debug("[BLIND][process_players]...");
		if(blind_player_list.size() == 0) {
			return;
		}
		std::vector<uint64_t> player_removals;
		for(const auto& entry : blind_player_list) {
			m_debug("[BLIND](for loop)[1]");
			auto victim = ptr_by_uuid(entry.target);
			if(!victim || victim->position() == POS_DEAD) {
				player_removals.emplace_back(entry.id);
				continue;
			}
			bool cleanup = entry.cleanup;
			if(entry.stop_tick <= CURRENT_TICK()) {
				cleanup = true;
				victim->remove_affect(AFF_BLIND);
			}
			if(cleanup) {
				m_debug("cleanup requested for entry: " << entry.id);
				player_removals.emplace_back(entry.id);
				continue;
			}
			m_debug("victim: " << entry.target);
			/**TODO got lucky? undo blindness now!
			 * uto damage = mods::rand::roll(entry.dice_count,entry.dice_sides);
			 */
		}
		m_debug("[player_removals.size() check]");
		if(player_removals.size()) {
			m_debug("[BLIND: REMOVE_IF]");
			blind_player_list.erase(std::remove_if(
			                            blind_player_list.begin(),
			                            blind_player_list.end(),
			[&](auto entry) -> bool {
				return std::find(
				    player_removals.cbegin(),
				    player_removals.cend(),
				    entry.id) != player_removals.cend();
			})
			                       );
			m_debug("[BLIND: REMOVE_IF]-- DONE --");
		}
	}
	ACMD(do_blind_me_for) {
		if(argshave()->size_gt(0)->int_at(0)->passed()) {
			player->sendln(CAT("intat(0):",intat(0)));
			blind_for(player,intat(0));
		}
	}
	void init() {
		mods::interpreter::add_command("blind_me_for", POS_RESTING, do_blind_me_for, 0, 0);

	}

#undef m_error

#undef m_error
#undef m_debug
};
