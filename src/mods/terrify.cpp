#include "terrify.hpp"
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

#ifdef __MENTOC_SHOW_MODS_TERRIFY_DEBUG_OUTPUT__
	#define m_debug(MSG) mentoc_prefix_debug("[mods::terrify::debug]")  << MSG << "\n";
	#define m_error(MSG) mentoc_prefix_debug(red_str("[mods::terrify::ERROR]"))  << MSG << "\n";
#else
	#define m_debug(MSG) ;;
	#define m_error(MSG) ;;
#endif


namespace mods::terrify {
	using affect_vector_t = mods::affects::affect_vector_t;
	using affect_t = mods::affects::affect_t;
	using texture_type_t = room_data::texture_type_t;
	static uint64_t terrify_id = 0;
	struct terrify_damage_t {
		uint64_t id;
		uuid_t target;
		uint64_t stop_tick;
		bool cleanup;
		inline bool operator==(const terrify_damage_t& lhs) {
			if(lhs.id == id) {
				return true;
			}
			return false;
		}
		std::string report() {
			return CAT("id:",id,"\ntarget:",target,"\nstop_tick:",stop_tick);
		}
		terrify_damage_t() = delete;
		terrify_damage_t(player_ptr_t victim,
		    uint64_t ticks) : id(++terrify_id),
			target(victim->uuid()),stop_tick((9 * ticks) + CURRENT_TICK()),cleanup(false) {
			victim->affect(AFF_TERRIFY);
			m_debug("[[[[ stop at: " << stop_tick << " which is " << stop_tick - CURRENT_TICK() << " in the future");
		}
		terrify_damage_t(const terrify_damage_t& other) {
			m_debug("[terrify_damage_t COPY OTHER:" << other.id << "]");
			id = other.id;
			target = other.target;
			stop_tick = other.stop_tick;
			cleanup = other.cleanup;
		}
		~terrify_damage_t() {
			m_debug("[~terrify_damage_t]");
		}

	};
	using terrify_list_t = std::vector<terrify_damage_t>;

	static terrify_list_t terrify_player_list;

	void terrify_for(player_ptr_t victim, uint32_t ticks) {
		m_debug("terrify_for cast on: " << victim->name() << ", for " << ticks << " ticks");
		terrify_player_list.emplace_back(victim,ticks);
		return;
	}
	void process_players() {
		m_debug("[TERRIFY][process_players]...");
		if(terrify_player_list.size() == 0) {
			return;
		}
		std::vector<uint64_t> player_removals;
		for(const auto& entry : terrify_player_list) {
			m_debug("[TERRIFY](for loop)[1]");
			auto victim = ptr_by_uuid(entry.target);
			if(!victim || victim->position() == POS_DEAD) {
				player_removals.emplace_back(entry.id);
				continue;
			}
			bool cleanup = entry.cleanup;
			if(entry.stop_tick <= CURRENT_TICK()) {
				cleanup = true;
				victim->remove_affect(AFF_TERRIFY);
			}
			if(cleanup) {
				m_debug("cleanup requested for entry: " << entry.id);
				player_removals.emplace_back(entry.id);
				continue;
			}
			m_debug("victim: " << entry.target);
			/**TODO got lucky? undo terrifyness now!
			 * uto damage = mods::rand::roll(entry.dice_count,entry.dice_sides);
			 */
		}
		m_debug("[player_removals.size() check]");
		if(player_removals.size()) {
			m_debug("[TERRIFY: REMOVE_IF]");
			terrify_player_list.erase(std::remove_if(
			        terrify_player_list.begin(),
			        terrify_player_list.end(),
			[&](auto entry) -> bool {
				return std::find(
				    player_removals.cbegin(),
				    player_removals.cend(),
				    entry.id) != player_removals.cend();
			})
			);
			m_debug("[TERRIFY: REMOVE_IF]-- DONE --");
		}
	}
	ACMD(do_terrify_me_for) {
		if(argshave()->size_gt(0)->int_at(0)->passed()) {
			player->sendln(CAT("intat(0):",intat(0)));
			terrify_for(player,intat(0));
		}
	}
	void init() {
		mods::interpreter::add_command("terrify_me_for", POS_RESTING, do_terrify_me_for, 0, 0);

	}

#undef m_error

#undef m_error
#undef m_debug
};
