#include "innocent.hpp"
#include "../weapons/damage-types.hpp"
#include "../combat-composer/snipe-target.hpp"
#include "../scan.hpp"
#include "../rooms.hpp"
#include "helpers.hpp"
#include "extended-types.hpp"
#include "../scan.hpp"

#ifdef  __MENTOC_MODS_MOBS_INNOCENT_SHOW_DEBUG_OUTPUT__
	#define mps_debug(a) mentoc_prefix_debug("m|m|mps") << a << "\n";
	#define m_error(a) mentoc_prefix_debug("m|m|mps[ERROR]:") << a << "\n";
#else
	#define mps_debug(a) ;;
	#define m_error(a) ;;
#endif
namespace mods::mobs {
	void innocent_shout(player_ptr_t npc) {
		if(!npc) {
			return;
		}

	}
	static inline std::vector<std::shared_ptr<innocent>>& innocent_list() {
		static std::vector<std::shared_ptr<innocent>> list;
		return list;
	}
	void innocent::create(uuid_t mob_uuid, std::string variation) {
		mps_debug("innocent create on uuid:" << mob_uuid);
		auto p = ptr_by_uuid(mob_uuid);
		if(!p) {
			log("SYSERR: did not find player to populate innocent with: %d",mob_uuid);
			return;
		}
		innocent_list().emplace_back(std::make_shared<innocent>(mob_uuid,variation));
	}

	/**
	 * @brief callback when someone spotted
	 *
	 * @param room
	 * @param player
	 */
	void innocent::set_variation(std::string v) {
		mps_debug("setting variation: '" << v << "'");
		this->variation = v;
	}
	str_map_t innocent::report() {
		return usages();
	}
	str_map_t innocent::usages() {
		return base_usages();
	}
	/**
	 * @brief damage_events registered here
	 */
	void innocent::setup_damage_callbacks() {
#ifdef __MENTOC_SHOW_INNOCENT_DAMAGE_CALLBACKS__
#define m(A) std::cerr << green_str("[innocent::setup_damage_callbacks]") << A << "\n";
#else
#define m(A)
#endif
		using de = damage_event_t;

		static const std::vector<de> triggered_fear = {
			de::ATTACKER_NARROWLY_MISSED_YOU_EVENT,
			de::HIT_BY_RIFLE_ATTACK,
			de::HIT_BY_SPRAY_ATTACK,
			de::YOU_GOT_HEADSHOT_BY_SPRAY_ATTACK,
			de::YOU_GOT_HEADSHOT_BY_RIFLE_ATTACK,
			de::COULDNT_FIND_TARGET_EVENT,
			de::HIT_BY_MELEE_ATTACK,
			de::HIT_BY_BLADED_MELEE_ATTACK,
			de::YOU_ARE_INJURED_EVENT,
			de::HIT_BY_BLUNT_MELEE_ATTACK,
			de::HIT_BY_CRITICAL_SPRAY_ATTACK,
			de::HIT_BY_CRITICAL_RIFLE_ATTACK,
			de::YOU_GOT_HIT_BY_REFLECTED_MUNITIONS_EVENT,
			de::YOU_GOT_HIT_BY_AR_SHRAPNEL,
			de::YOU_GOT_HIT_BY_INCENDIARY_AMMO,
			de::YOU_ARE_DISORIENTED_EVENT,
			de::HIT_BY_INCENDIARY_DAMAGE,
			de::HIT_BY_RADIOACTIVE_DAMAGE,
			de::HIT_BY_ANTI_MATTER_DAMAGE,
			de::HIT_BY_CORROSIVE_DAMAGE,
			de::HIT_BY_EMP_DAMAGE,
			de::HIT_BY_EXPLOSIVE_DAMAGE,
			de::HIT_BY_SHRAPNEL_DAMAGE,
			de::HIT_BY_CRYOGENIC_DAMAGE,
			de::HIT_BY_SHOCK_DAMAGE,
		};
		player_ptr->register_damage_event_callback(triggered_fear,[&](const feedback_t& feedback,const uuid_t& player) {
			switch(player_ptr->position()) {
				case POS_DEAD:
				case POS_INCAP:
				case POS_SLEEPING:
				case POS_STUNNED:
					break;
				case POS_RESTING:
				case POS_SITTING:
				case POS_MORTALLYW:
				case POS_FIGHTING:
				case POS_STANDING:
					innocent_shout(player_ptr);
					break;
				default:
					break;
			}

		});

	}
	void innocent::init() {
		smart_mob::init();
	}

	/**
	 * @brief preferred constructor method
	 *
	 * @param mob_uuid
	 * @param variation
	 */
	innocent::innocent(uuid_t mob_uuid, std::string variation) {
		this->init();
		this->uuid = mob_uuid;
		auto p = ptr_by_uuid(mob_uuid);
		if(!p) {
			log("SYSERR: did not find player to populate innocent with: %d",mob_uuid);
			this->loaded = false;
			this->error = true;
			return;
		}
		player_ptr = p;
		auto ch = p->cd();
		ch->mob_specials.extended_mob_type = mob_special_data::extended_mob_type_t::INNOCENT;
		this->set_behaviour_tree("innocent");
		this->setup_damage_callbacks();
		this->loaded = true;
		this->error = false;
		this->set_variation(variation);
	}
	std::pair<bool,std::string> innocent::move_to(const direction_t& dir) {
		auto room_id = player_ptr->room();
		auto opt = world[room_id].dir_option[dir];
		if(opt && opt->to_room <= world.size()) {
			char_from_room(player_ptr->cd());
			char_to_room(player_ptr->cd(),opt->to_room);
			return {true,"moved"};
		}
		return {false,"stayed"};
	}
};
#undef mps_debug
