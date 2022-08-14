#include "goat.hpp"
#include "../weapons/damage-types.hpp"
#include "../scan.hpp"
#include "../rooms.hpp"
#include "helpers.hpp"
#include "extended-types.hpp"
#include "roam-pattern.hpp"
#include "../scan.hpp"
#include "../mob-roam.hpp"
#include "../loops.hpp"
#include "../calc-visibility.hpp"
#include "../contract-steps.hpp"
#include "../interpreter.hpp"
#include "../weapons/reload.hpp"
#include "../combat-composer/includes.hpp"
#include "../sneak.hpp"
#include "../ensnare.hpp"

#ifdef m_debug
#undef m_debug
#endif

#define   __MENTOC_MODS_MOBS_goat_SHOW_DEBUG_OUTPUT__
#ifdef   __MENTOC_MODS_MOBS_goat_SHOW_DEBUG_OUTPUT__
#define m_debug(a) mentoc_prefix_debug("goat") << a << "\n";
#define cmem(a) mentoc_prefix_debug("[goat][memory_footprint]") << a << "\n";
#else
#define m_debug(a)
#define cmem(a)
#endif

#ifdef __MENTOC_MODS_MOBS_GOAT_VERBOSE__
#define m_verbose(a) mentoc_prefix_debug("goat") << a << "\n";
#else
#define m_verbose(a)
#endif

namespace mods::mobs {

	std::vector<std::shared_ptr<goat>> goat_list;
	/**
	 * - TODO: as soon as we spawn the goat, we need to place him in the
	 *   room where he will meet the quest taker
	 */
	void goat::create(const uuid_t& mob_uuid, std::string_view targets) {
		m_debug("goat::create on uuid:" << mob_uuid);
		auto p = ptr_by_uuid(mob_uuid);
		if(!p) {
			log("SYSERR: did not find player to populate goat with: %d",mob_uuid);
			return;
		}
		mods::mobs::goat_list.emplace_back(std::make_shared<goat> (mob_uuid,targets.data()));
		for(const auto& pat : {
		            "Butcher"
		        }) {
			mods::mobs::roam_pattern::register_roam_pattern(goat::MOB_VNUM,pat);
		}
	}


	bool goat::debug_mode_on() {
		return m_debug_mode;
	}

	void goat::mention(std::string_view msg) {
		if(this->debug_mode_on()) {
			std::cerr << green_str("GOAT REPORT:") << "============================================\n";
			std::cerr << green_str("GOAT REPORT:") << msg << "\n";
			std::cerr << green_str("GOAT REPORT:") << "============================================\n";
		}
	}
	/**
	 * TODO: include stats (hp,mana,etc) and who the Defiler is currently
	 * hunting
	 */
	str_map_t goat::report() {
		return usages();
	}


	str_map_t goat::usages() {
		str_map_t m;
		//m = base_usages();
		//std::size_t rem_size = 0, attackers = 0, hostiles = 0;
		//rem_size = std::distance(m_remembered_items.cbegin(),m_remembered_items.cend());
		//attackers = std::distance(m_attackers.cbegin(),m_attackers.cend());
		//hostiles = std::distance(m_hostiles.cbegin(),m_hostiles.cend());
		//if(rem_size) {
		//	m["remembered_items"] = std::to_string(rem_size);
		//}
		//if(attackers) {
		//	m["attackers"] = std::to_string(attackers);
		//}
		//if(hostiles) {
		//	m["hostiles"] = std::to_string(hostiles);
		//}
		//if(m_scanned_items.size()) {
		//	m["scanned_items"] = std::to_string(m_scanned_items.size());
		//}
		return m;
	}


#if 0
	/** =================================================================*/
	/** =================================================================*/
	/** === DAMAGE CALLBACKS */
	/** =================================================================*/
	/** =================================================================*/
	/**
	 * @brief damage_events registered here
	 */
	void goat::setup_damage_callbacks() {
		using de = damage_event_t;
		static const std::vector<de> pacify_events = {
			de::TARGET_DEAD_EVENT,
			de::TARGET_IN_PEACEFUL_ROOM_EVENT,
			de::YOURE_IN_PEACEFUL_ROOM,
		};
		player_ptr->register_damage_event_callback(pacify_events,[&](const feedback_t& feedback,const uuid_t& player) {
			m_debug("pacify events");
			roam(5);
		});

		static const std::vector<de> enrage_if = {
			de::ATTACKER_NARROWLY_MISSED_YOU_EVENT,
			de::HIT_BY_RIFLE_ATTACK,
			de::HIT_BY_SPRAY_ATTACK,
			de::YOU_GOT_HEADSHOT_BY_SPRAY_ATTACK,
			de::YOU_GOT_HEADSHOT_BY_RIFLE_ATTACK,
		};
		player_ptr->register_damage_event_callback(enrage_if,[&](const feedback_t& feedback,const uuid_t& player) {
			if(!ptr_by_uuid(player)) {
				std::cerr << type().data() << ":" << red_str("USE AFTER FREE") << "\n";
				return;
			}
			auto attacker = ptr_by_uuid(feedback.attacker);
			auto weapon = player_ptr->primary();

			if(player_ptr->room() != attacker->room()) {
				if(mods::calc_visibility::roll_victim_spots_attacker(player_ptr,attacker,feedback)) {
					m_debug("generic thief (as victim) spots attacker!");
					auto decision = feedback.from_direction;
					move_to(decision);
					set_heading(decision);
				} else {
					m_debug("generic thief (as victim) ***DOES NOT*** spot attacker!");
				}
			}
			if(player_ptr->room() == attacker->room()) {
				//melee_attack(attacker);
			}
		});

		static const std::vector<de> desperation_move = {
			de::YOU_ARE_INJURED_EVENT,
			de::YOU_MISSED_YOUR_TARGET_EVENT,
		};

		static const std::vector<de> taunt_if = {
			de::YOU_INFLICTED_INCENDIARY_DAMAGE,
			de::YOU_DEALT_HEADSHOT_WITH_RIFLE_ATTACK,
			de::YOU_DEALT_CRITICAL_RIFLE_ATTACK,
			de::YOU_INFLICTED_MELEE_ATTACK,
			de::YOU_INFLICTED_BLADED_MELEE_ATTACK,
			de::YOU_INJURED_SOMEONE_EVENT,
			de::YOU_INFLICTED_INCENDIARY_AMMO,
			de::YOU_DISORIENTED_SOMEONE_EVENT,
		};

		static const std::vector<de> whine_if = {
			de::YOU_GOT_HEADSHOT_BY_SPRAY_ATTACK,
			de::YOU_GOT_HEADSHOT_BY_RIFLE_ATTACK,
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

		static const std::vector<de> upkeep_if = {
			de::OUT_OF_AMMO_EVENT,
			de::NO_PRIMARY_WIELDED_EVENT,
			de::COOLDOWN_IN_EFFECT_EVENT,
			de::COULDNT_FIND_TARGET_EVENT,
		};
		player_ptr->register_damage_event_callback(upkeep_if,[&](const feedback_t& feedback,const uuid_t& player) {
			switch(feedback.damage_event) {
				case de::OUT_OF_AMMO_EVENT: //{
					//		/** TODO: I *REALLY* need this to work! */
					//		auto ammo = create_object(ITEM_CONSUMABLE,"opsix-incendiary-ar-clip.yml");
					//		player_ptr->carry(ammo);
					//	}
					//	mods::weapons::reload::reload_weapon(player_ptr,player_ptr->primary());
					m_debug("DAMN! OUT OF AMMO!");
					refill_ammo();
					break;
				case de::NO_PRIMARY_WIELDED_EVENT:
					m_debug("No primary wieldded... wtf?");
					m_weapon = player_ptr->primary();
					break;
				case de::COOLDOWN_IN_EFFECT_EVENT:
					m_debug("cooldown in effect for primary");
					break;
				case de::COULDNT_FIND_TARGET_EVENT:
					m_debug("Can't find target");
					m_debug("Resetting target");
					//this->reset_last_attacker();
					break;
				default:
					m_debug("Weird status. unknown");
					break;
			}
		});

	}

	/**
	 * TODO: if this is used, it needs to return true
	 */
	bool goat::is_rival(player_ptr_t& player) {
		return true;
	}
#endif

	///**
	// * - TODO: React according to which mode the Defiler is in
	// * 		- [ ] Ensnaring mode causes snare to be immediately set on player
	// * 			- [ ] Defiler moves away and throws daggers
	// * 		- [ ] Corpse explosion mode causes corpse to explode
	// * 		- [ ] Dagger mode causes Defiler to throw daggers at player's legs
	// * 			- [ ] Player cannot move until daggers taken out
	// * 		- [ ] Chainsaw mode causes Defiler to attack player with Chainsaw
	// */
	//void goat::door_entry_event(player_ptr_t& player) {
	//	if(player->is_npc()) {
	//		if(is_rival(player)) {
	//			btree_roam();
	//			//TODO: attack_with_melee(player);
	//			//player->sendln(CAT("I am:",uuid," and I'm Watching you"));
	//		}
	//	}
	//}

	/**
	 * - TODO: add quest taker permanently to memory
	 */
	void goat::init() {
		//this->RCT = nullptr;
		//m_watching_everywhere = false;
		//smart_mob::init();
		//m_should_do_max[SHOULD_DO_ROAM] = LOWLY_SECURITY_ROAM_TICK();
		//m_should_do_max[SHOULD_DO_RANDOM_TRIVIAL] = LOWLY_SECURITY_RANDOM_TRIVIAL_TICK();
		//m_found_item = 0;
		//m_last_attacker = nullptr;
		//m_attackers.clear();
		//m_weapon = nullptr;
		//m_attackers_last_direction = std::nullopt;
		//m_scanned_items.clear();
		//m_remembered_items.clear();
		/** TODO:
		 *
		for(const auto& msg : EXPLODE(CHAOTIC_METH_ADDICT_PSV_RANDOM_ACT(),'|')) {
			m_random_acts.emplace_back(msg);
		}
		*/
		//m_cant_find = 0;
		//cmem("m_random_acts:" << m_random_acts.size());
		m_status = goat::status_t::LOOKING_FOR_A_FIGHT;
		m_debug_mode = GOAT_DEBUG_MODE_ON();
		//m_last_attack_direction = NORTH;
	};

	/**
	 * @brief preferred constructor method
	 *
	 * @param mob_uuid
	 * @param variation
	 */
	goat::goat(const uuid_t& mob_uuid, std::string_view variation) {
		this->init();
		this->uuid = mob_uuid;
		auto p = ptr_by_uuid(mob_uuid);
		if(!p) {
			log("SYSERR: did not find player to populate goat with: %d",mob_uuid);
			this->loaded = false;
			this->error = true;
			return;
		}
		player_ptr = p;
		auto ch = p->cd();
		ch->mob_specials.extended_mob_type = mob_special_data::extended_mob_type_t::GOAT;
		this->setup_damage_callbacks();
		this->loaded = true;
		this->error = false;
		//this->set_variation(variation.data());
		bootstrap_equipment();
		//m_weapon = player()->primary();
		std::fill(m_weapons.begin(),m_weapons.end(),nullptr);
		std::size_t i= 0;
		if(player_ptr->primary()) {
			m_weapons[i++] = player_ptr->primary();
		}
		if(player_ptr->secondary()) {
			m_weapons[i++] = player_ptr->secondary();
		}
	}


	goat::weapons_list_t& goat::weapons() {
		return m_weapons;
	}



	direction_t random_room_direction(auto room_id) {
		if(world.size() < room_id) {
			return NORTH;
		}
		auto& room = world[room_id];
		return room.directions()[rand_number(0,room.directions().size()-1)];
	}


	/**====================================================*/
	/** START: SIMPLIFIED ATTACK LOOP */
	/**====================================================*/
	//void goat::start_turn() {
	//	/** This function is called every time our turn occurs
	//	 * during the event loop
	//	 */
	//	m_hostile_phase_1_attempts = 0;
	//}
	//bool goat::find_same_room_targets() {
	//	m_same_room_targets.clear();
	//	auto room = this->room();
	//	for(auto& player : room_list(room)) {
	//		if(player->is_npc()) {
	//			mention("player is an npc. skipping");
	//			continue;
	//		}
	//		m_same_room_targets.emplace_back(player);
	//	}
	//	return m_same_room_targets.size();
	//}
	void goat::roam() {
		auto chase = determine_chase_direction();
		auto room = this->room();
		if(world.size() <= room) {
			log("WARNING: goat's room is %d which doesn't exist!",room);
			return;
		}
		if(chase >= NUM_OF_DIRS) {
			log("WARNING: goat's chase is %d which doesn't exist!",chase);
			return;
		}

		if(!world[room].dir_option[chase]) {
			chase = mods::rand::shuffle_container(world[room].directions())[0];
			set_heading(chase);
			move_to(chase);
			return;
		}
		if(chase >= sizeof(world[room].dir_option)) {
			log("WARNING: goat chase is %d which doesn't exist!",chase);
			return;
		}
		if(world[room].dir_option[chase] && mods::mobs::roam_pattern::can_roam_to(goat::MOB_VNUM, world[room].dir_option[chase]->to_room)) {
			auto s = move_to(chase);
			if(!std::get<0>(s)) {
				chase = mods::rand::shuffle_container(world[room].directions())[0];
			} else {
				return;
			}
		} else {
			chase = mods::rand::shuffle_container(world[room].directions())[0];
		}
	}

	bool goat::is(goat::status_t status) {
		return m_status == status;
	}

	void goat::slapped_by(uuid_t player_uuid) {
		auto ptr = ptr_by_uuid(player_uuid);
		if(!ptr) {
			return;
		}
		if(ptr->is_npc()) {
			return;
		}
		static constexpr std::array<std::string_view,6> strings = {
			"$n says, 'Blaaaaa-aat-at-at-aaaat-aaaaat-aatt.....'",
			"$n looks at you and the floor simultaneously...",
			"$n nods in agreement.",
			"$n farts.",
			"$n begins to chew on the floor...",
			"$n temporarily goes un-cross-eyed.",
		};

		const auto& r = rand_item(strings);
		act(r.data(), TRUE, this->player_ptr->cd(), 0, 0, TO_ROOM);

	}

	void goat::run() {

	}

	std::pair<bool,std::string> goat::move_to(const direction_t& dir) {
		auto old_vis = player_ptr->visibility();
		auto room_id = player_ptr->room();
		auto opt = world[room_id].dir_option[dir];
		if(opt && opt->to_room <= world.size()) {
			mods::mobs::room_watching::unwatch_room(player_ptr);
			player_ptr->visibility() = 0;
			mods::sneak::apply_absolute_sneak(player_ptr,100);
			perform_move(player_ptr->cd(),dir,0);
			player_ptr->visibility() = old_vis;
			mods::sneak::clear_sneak(player_ptr);
			return {true,"moved"};
		}
		return {false,"stayed"};

	}

	void run_goat_pulse() {
		for(const auto& goat : goat_list) {
			goat->run();
		}
	}
	void process_goat_slap(player_ptr_t& attacker,player_ptr_t& victim) {
		if(attacker->is_npc()) {
			return;
		}
		for(auto& goat : goat_list) {
			if(goat->uuid == victim->uuid()) {
				goat->slapped_by(attacker->uuid());
				return;
			}
		}
	}
};
namespace mods::mobs::goat_init {
	ACMD(do_pray) {
		static constexpr std::string_view usage = "usage: pray <target>\r\nexample: pray meat\r\n";
		if(argshave()->size_gt(0)->passed() == false) {
			player->sendln(usage);
			return;
		}
		if(!player->has_contract()) {
			player->sendln("Scan all you want, but you're not currently under contract.");
			return;
		}
	}
	void init() {
		/**
		 * Builds resistance to shrapnel and incendiary damage
		 */
		using namespace mods::mobs::resistance;
		using ele = mods::elemental_types_t;
		using un = unit_type_t;
		std::vector<resistance_t> rezzes;
		static constexpr const resistance_t r[] = {
			{100.0,un::PERCENTAGE,ele::ELEM_SHRAPNEL},
			{100.0,un::PERCENTAGE,ele::ELEM_INCENDIARY},
		};
		for(const auto& rez : r) {
			rezzes.emplace_back(rez);
		}

		register_resistances_for_mob_vnum(
		    mods::mobs::goat::MOB_VNUM,
		    rezzes
		);
		mods::interpreter::add_user_command("pray",do_pray);
	}
};//end mods::mobs::goat_init
#undef m_debug
#undef cmem
