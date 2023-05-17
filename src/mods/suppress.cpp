#include "suppress.hpp"
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
#include "examine.hpp"

#define SUPPRESS_SHOW_CLEANUPS
//#define __MENTOC_SHOW_MODS_SUPPRESS_DEBUG_OUTPUT__
#ifdef __MENTOC_SHOW_MODS_SUPPRESS_DEBUG_OUTPUT__
	#define m_debug(MSG) mentoc_prefix_debug("[mods::suppress::debug]")  << MSG << "\n";
	#define m_error(MSG) mentoc_prefix_debug(red_str("[mods::suppress::ERROR]"))  << MSG << "\n";
#else
	#define m_debug(MSG) ;;
	#define m_error(MSG) ;;
#endif


namespace mods::suppress {
	using affect_vector_t = mods::affects::affect_vector_t;
	using affect_t = mods::affects::affect_t;
	using texture_type_t = room_data::texture_type_t;
	static uint64_t suppress_id = 0;
	struct suppress_damage_t {
		uint64_t id;
		uuid_t target;
		uint64_t stop_tick;
		bool cleanup;
		inline bool operator==(const suppress_damage_t& lhs) {
			return lhs.id == id;
		}
		std::string report() {
			return CAT("suppression: id:",id,"\ntarget:",target,"\nstop_tick:",stop_tick);
		}
		suppress_damage_t() = delete;
		suppress_damage_t(const uuid_t& _victim_uuid, const uint64_t& ticks) : id(++suppress_id),
			target(_victim_uuid),stop_tick(ticks + CURRENT_TICK()),cleanup(false) {
			auto victim = ptr_by_uuid(_victim_uuid);
			if(victim) {
				victim->affect(AFF_SUPPRESS);
			}
			m_debug("[[[[ stop at: " << stop_tick << " which is " << stop_tick - CURRENT_TICK() << " in the future");
		}
		suppress_damage_t(const suppress_damage_t& other) {
			m_debug("[suppress_damage_t COPY OTHER:" << other.id << "]");
			id = other.id;
			target = other.target;
			stop_tick = other.stop_tick;
			cleanup = other.cleanup;
		}
		~suppress_damage_t() = default;

	};
	using suppress_list_t = std::vector<suppress_damage_t>;

	static suppress_list_t suppress_player_list;

	void suppress_for(const uuid_t& victim, const uint64_t& ticks) {
		suppress_player_list.emplace_back(victim,ticks);
		return;
#if 0

		auto goggles = victim->equipment(WEAR_GOGGLES);
		float tick_reduce = 0.0;
		int suppress_ticks = ticks;
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
		suppress_ticks = ticks - (tick_reduce * ticks);
		if(suppress_ticks > 0) {
			suppress_player_list.emplace_back(victim,suppress_ticks);
		}
#endif
	}
	void process_players() {
		m_debug("[SUPPRESS][process_players]...");
		m_debug("CURRENT_TICK: " << CURRENT_TICK());
		if(suppress_player_list.size() == 0) {
			return;
		}
		std::vector<uint64_t> player_removals;
		for(const auto& entry : suppress_player_list) {
			m_debug("[SUPPRESS](for loop)[1]");
			auto victim_opt = ptr_opt(entry.target);
			auto victim = victim_opt.value_or(nullptr);
			if(!victim || victim->position() == POS_DEAD) {
				player_removals.emplace_back(entry.id);
				continue;
			}
			m_debug("victim: " << entry.target);
			bool cleanup = entry.cleanup;
			if(entry.stop_tick <= CURRENT_TICK()) {
#ifdef SUPPRESS_SHOW_CLEANUPS
				std::cerr << "suppress[process_players]::CLEANUP " << entry.target << "\n";
#endif
				m_debug("CLEANUP" << entry.target << " id:" << entry.id);
				cleanup = true;
				victim->remove_affect(AFF_SUPPRESS);
			}
			if(cleanup) {
				m_debug("cleanup requested for entry: " << entry.id);
				player_removals.emplace_back(entry.id);
				continue;
			}
		}
		m_debug("[player_removals.size() check]");
		if(player_removals.size()) {
			m_debug("[SUPPRESS: REMOVE_IF]");
			suppress_player_list.erase(std::remove_if(
			        suppress_player_list.begin(),
			        suppress_player_list.end(),
			[&](auto entry) -> bool {
				return std::find(
				    player_removals.cbegin(),
				    player_removals.cend(),
				    entry.id) != player_removals.cend();
			})
			);
			m_debug("[SUPPRESS: REMOVE_IF]-- DONE --");
		}
	}
	ACMD(do_suppress_me_for) {
		if(argshave()->size_gt(0)->int_at(0)->passed()) {
			player->sendln(CAT("intat(0):",intat(0)));
			suppress_for(player->uuid(),intat(0));
			return;
		}
		player->errorln("Usage: admin:suppress_me_for <ticks>");
	}
	ACMD(do_suppress_target_for) {
		if(argshave()->size_gt(1)->int_at(1)->passed()) {
			suppress_for(mods::examine::find_player_by_name(player,argat(0)),intat(1));
		} else {
			player->sendln("Usage: admin:suppress_target_for <player> <ticks>");
		}
	}
	bool is_suppressed(const uuid_t& victim_uuid) {
		return std::find_if(suppress_player_list.cbegin(),suppress_player_list.cend(),[&](const auto& sd) -> bool {
			return victim_uuid == sd.target;
		}) != suppress_player_list.cend();
	}
	void init() {
		ADD_ADMIN_COMMAND("admin:suppress_me_for", do_suppress_me_for);
		ADD_ADMIN_COMMAND("admin:suppress_target_for", do_suppress_target_for);

	}

#undef m_error

#undef m_error
#undef m_debug
};
#ifdef SUPPRESS_SHOW_CLEANUPS
	#undef SUPPRESS_SHOW_CLEANUPS
#endif
