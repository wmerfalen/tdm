#include "defiler.hpp"
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

//#define  __MENTOC_MODS_MOBS_defiler_SHOW_DEBUG_OUTPUT__
#ifdef  __MENTOC_MODS_MOBS_defiler_SHOW_DEBUG_OUTPUT__
#define m_debug(a) mentoc_prefix_debug("defiler") << a << "\n";
#define cmem(a) mentoc_prefix_debug("[defiler][memory_footprint]") << a << "\n";
#else
#define m_debug(a)
#define cmem(a)
#endif

#ifdef __MENTOC_MODS_MOBS_DEFILER_VERBOSE__
#define m_verbose(a) mentoc_prefix_debug("defiler") << a << "\n";
#else
#define m_verbose(a)
#endif

namespace mods::mobs {

	/**
	 * - TODO: as soon as we spawn the defiler, we need to place him in the
	 *   room where he will meet the quest taker
	 */
	void defiler::create(const uuid_t& mob_uuid, std::string_view targets) {
		m_debug("defiler create on uuid:" << mob_uuid);
		auto p = ptr_by_uuid(mob_uuid);
		if(!p) {
			log("SYSERR: did not find player to populate defiler with: %d",mob_uuid);
			return;
		}
		auto g = std::make_shared<defiler>(mob_uuid,targets.data());
		mods::mobs::defiler_list().push_front(g);
		for(const auto& pat : {
		            "Butcher"
		        }) {
			mods::mobs::roam_pattern::register_roam_pattern(defiler::MOB_VNUM,pat);
		}
	}


	bool defiler::debug_mode_on() {
		return m_debug_mode;
	}

	void defiler::mention(std::string_view msg) {
		if(this->debug_mode_on()) {
			std::cerr << green_str("DEFILER REPORT:") << "============================================\n";
			std::cerr << green_str("DEFILER REPORT:") << msg << "\n";
			std::cerr << green_str("DEFILER REPORT:") << "============================================\n";
		}
	}
	/**
	 * TODO: include stats (hp,mana,etc) and who the Defiler is currently
	 * hunting
	 */
	str_map_t defiler::report() {
		return usages();
	}


	str_map_t defiler::usages() {
		str_map_t m;
		m = base_usages();
		std::size_t rem_size = 0, attackers = 0, hostiles = 0;
		rem_size = std::distance(m_remembered_items.cbegin(),m_remembered_items.cend());
		attackers = std::distance(m_attackers.cbegin(),m_attackers.cend());
		hostiles = std::distance(m_hostiles.cbegin(),m_hostiles.cend());
		if(rem_size) {
			m["remembered_items"] = std::to_string(rem_size);
		}
		if(attackers) {
			m["attackers"] = std::to_string(attackers);
		}
		if(hostiles) {
			m["hostiles"] = std::to_string(hostiles);
		}
		if(m_scanned_items.size()) {
			m["scanned_items"] = std::to_string(m_scanned_items.size());
		}
		return m;
	}


	/** =================================================================*/
	/** =================================================================*/
	/** === DAMAGE CALLBACKS */
	/** =================================================================*/
	/** =================================================================*/
	/**
	 * @brief damage_events registered here
	 */
	void defiler::setup_damage_callbacks() {
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
	bool defiler::is_rival(player_ptr_t& player) {
		return true;
	}

	///**
	// * - TODO: React according to which mode the Defiler is in
	// * 		- [ ] Ensnaring mode causes snare to be immediately set on player
	// * 			- [ ] Defiler moves away and throws daggers
	// * 		- [ ] Corpse explosion mode causes corpse to explode
	// * 		- [ ] Dagger mode causes Defiler to throw daggers at player's legs
	// * 			- [ ] Player cannot move until daggers taken out
	// * 		- [ ] Chainsaw mode causes Defiler to attack player with Chainsaw
	// */
	//void defiler::door_entry_event(player_ptr_t& player) {
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
	void defiler::init() {
		this->RCT = nullptr;
		m_watching_everywhere = false;
		smart_mob::init();
		m_should_do_max[SHOULD_DO_ROAM] = LOWLY_SECURITY_ROAM_TICK();
		m_should_do_max[SHOULD_DO_RANDOM_TRIVIAL] = LOWLY_SECURITY_RANDOM_TRIVIAL_TICK();
		m_found_item = 0;
		m_last_attacker = nullptr;
		m_attackers.clear();
		m_weapon = nullptr;
		m_attackers_last_direction = std::nullopt;
		m_scanned_items.clear();
		m_remembered_items.clear();
		for(const auto& msg : EXPLODE(CHAOTIC_METH_ADDICT_PSV_RANDOM_ACT(),'|')) {
			m_random_acts.emplace_back(msg);
		}
		m_cant_find = 0;
		cmem("m_random_acts:" << m_random_acts.size());
		m_status = defiler::status_t::LOOKING_FOR_A_FIGHT;
		m_debug_mode = DEFILER_DEBUG_MODE_ON();
	};

	/**
	 * @brief preferred constructor method
	 *
	 * @param mob_uuid
	 * @param variation
	 */
	defiler::defiler(const uuid_t& mob_uuid, std::string_view variation) {
		this->init();
		this->uuid = mob_uuid;
		auto p = ptr_by_uuid(mob_uuid);
		if(!p) {
			log("SYSERR: did not find player to populate defiler with: %d",mob_uuid);
			this->loaded = false;
			this->error = true;
			return;
		}
		player_ptr = p;
		auto ch = p->cd();
		ch->mob_specials.extended_mob_type = mob_special_data::extended_mob_type_t::DEFILER;
		this->setup_damage_callbacks();
		this->loaded = true;
		this->error = false;
		//this->set_variation(variation.data());
		bootstrap_equipment();
		m_weapon = player()->primary();
		std::fill(m_weapons.begin(),m_weapons.end(),nullptr);
		std::size_t i= 0;
		if(player_ptr->primary()) {
			m_weapons[i++] = player_ptr->primary();
		}
		if(player_ptr->secondary()) {
			m_weapons[i++] = player_ptr->secondary();
		}
	}


	defiler::weapons_list_t& defiler::weapons() {
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
	void defiler::start_turn() {
		/** This function is called every time our turn occurs
		 * during the event loop
		 */
		m_hostile_phase_1_attempts = 0;
	}
	bool defiler::find_same_room_targets() {
		m_same_room_targets.clear();
		for(auto& player : room_list(room())) {
			if(player->is_npc()) {
				mention("player is an npc. skipping");
				continue;
			}
			m_same_room_targets.emplace_back(player);
		}
		return m_same_room_targets.size();
	}
	void defiler::hostile_phase_1() {
#ifdef DEFILER_SIMPLIFIED_BTREE_DEBUG
		m_debug(green_str("-----------------------------"));
		m_debug(green_str("defiler simplified btree"));
		m_debug(green_str("-----------------------------"));
#endif

		/**
		 * TODO: if MELEE fighting someone...
		 */

		/**
		 * TODO: if already in ranged combat...
		 */

		/**
		 * if i need a target...
		 */
		auto target = acquire_target();
		if(target.has_value()) {
			rifle_attack(target.value());
			mention("I found an existing target");
			return;
		}
		mention("Looking for same room targets");
		if(find_same_room_targets()) {
			mention("Found same room target");
			auto target = mods::rand::shuffle_container(m_same_room_targets)[0];
			auto weapon = primary();
			m_engaged_target = nullptr;
			if(mods::combat_composer::engage_target(player_ptr,target,weapon)) {
				mention("Engaged target");
				m_engaged_target = target;
			} else {
				mention("Could not engage target");
			}
			return;
		}
		m_targets.clear();
		mention("Looking for someone to attack");
		if(find_someone_to_attack()) {
			auto found = mods::rand::shuffle_container(m_targets)[0];
			auto victim = ptr_by_uuid(found.uuid);
			if(!victim) {
				return;
			}
			rifle_attack(found);
			/** it is imperative that this be called AFTER get ranged combat totals */
			if(!m_watching_everywhere) {
				mods::mobs::helpers::watch_multiple(world[room()].directions(),cd(),RCT->max_range);
				m_watching_everywhere = true;
			}
			return;
		}
		mention("I didn't find anyone to attack...");
		roam(5);
	}
	void defiler::rifle_attack(const target_t& target) {
		auto victim = ptr_by_uuid(target.uuid);
		if(!victim) {
			return ;
		}
		auto weapon = primary();
		mods::combat_composer::snipe_target(player_ptr,victim,target.direction,(uint8_t)target.distance,weapon);
		save_attack_direction(target.direction);
		return ;
	}
	void defiler::roam(uint8_t times) {
		if(times == 0) {
			return;
		}
		auto chase = determine_chase_direction();
		if(!world[room()].dir_option[chase]) {
			chase = mods::rand::shuffle_container(world[room()].directions())[0];
			set_heading(chase);
			move_to(chase);
			return;
		}
		if(world[room()].dir_option[chase] && mods::mobs::roam_pattern::can_roam_to(defiler::MOB_VNUM, world[room()].dir_option[chase]->to_room)) {
			auto s = move_to(chase);
			if(!std::get<0>(s)) {
				chase = mods::rand::shuffle_container(world[room()].directions())[0];
			} else {
				return;
			}
		} else {
			chase = mods::rand::shuffle_container(world[room()].directions())[0];
		}
	}
	void defiler::rct_upkeep() {
		if(!this->RCT) {
			this->RCT = player_ptr->get_ranged_combat_totals();
		}
	}
	std::optional<target_t> defiler::acquire_target() {
		std::vector<target_t> found;
		rct_upkeep();
		int depth = RCT->max_range;
		mods::scan::vec_player_data vpd;
		mods::scan::los_scan_for_players(this->cd(),depth,&vpd);
		std::map<uint8_t,int> scores;
		for(auto&& v : vpd) {
			auto victim = ptr_by_uuid(v.uuid);
			if(!victim) {
				continue;
			}
			if(mods::rooms::is_peaceful(v.room_rnum)) {
				continue;
			}
			if(IS_NPC(victim->cd())) {
				continue;
			}
			found.emplace_back(target_t{v.uuid,v.direction,v.distance});
		}
		if(found.size() == 0) {
			return std::nullopt;
		}
		if(found.size() == 1) {
			return found[0];
		}
		return mods::rand::shuffle_container(found)[0];
	}
	bool defiler::find_someone_to_attack() {
		int depth = RCT->max_range;
		mods::scan::vec_player_data vpd;
		mods::scan::los_scan_for_players(this->cd(),depth,&vpd);
		std::map<uint8_t,int> scores;
		for(auto&& v : vpd) {
			auto victim = ptr_by_uuid(v.uuid);
			if(!victim) {
				continue;
			}
			if(mods::rooms::is_peaceful(v.room_rnum)) {
				continue;
			}
			if(IS_NPC(victim->cd())) {
				continue;
			}
			if(m_targets.cend() == std::find_if(
			            m_targets.cbegin(),
			            m_targets.cend(),
			[&v](const auto& t) -> bool {
			return t.uuid == v.uuid;
		})
		  ) {
				m_targets.emplace_back(target_t{v.uuid,v.direction,v.distance});
			}
		}
		return m_targets.size() > 0;
	}
	/**====================================================*/
	/** END: SIMPLIFIED ATTACK LOOP */
	/**====================================================*/



	bool defiler::is(defiler::status_t status) {
		return m_status == status;
	}

	void defiler::telegraph(player_ptr_t& victim,std::string_view saying) {
		victim->sendln(saying);
	}

	void defiler::shout(std::string_view message) {

	}

	void defiler::ensnare(player_ptr_t& victim) {
		auto barbed_wire = create_object(ITEM_GADGET,"demonic-snares.yml");
		mods::ensnare::ensnare_damage(
		    player_ptr,
		    victim,
		    barbed_wire,
		    barbed_wire->gadget()->attributes->damage_points);

	}

	/**
	 * - TODO: Agony scream and disappearance
	 */
	void defiler::attacked(const feedback_t& feedback) {
		auto p = ptr_by_uuid(feedback.attacker);
		if(p) {
			m_last_attacker = p;
			m_attackers.emplace_front(p);
			//cmem("{m_attackers.size}:" << std::distance(m_attackers.cbegin(),m_attackers.cend()));
		}
	}

	/**
	 * - TODO: for each mode, determines if the following will happen
	 *   - [ ] Ensnaring, throw dagger
	 *   - [ ] Dagger, snipe
	 *   - [ ] Corpse Explosion, snipe/engage
	 *   - [ ] Chainsaw,
	 */
	void defiler::extra_attack() {
		m_debug("extra attack roll success");
		auto attacker = player()->fighting();
		if(m_weapon && attacker->room() == this->room()) {
			m_debug("i have a weapon and i'm using it against who i'm fighting");
		}
	}

	std::pair<bool,std::string> defiler::move_to(const direction_t& dir) {
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

	void defiler::move_closer_to_target() {
		uint8_t loops = 1;
		if(mods::rand::chance(CAR_THIEF_EXTRA_LOOP_CHANCE())) {
			++loops;
		}
		for(int i =0; i < loops; ++i) {
			auto results = mods::scan::los_find(player_ptr,m_last_attacker);
			if(results.found && results.distance > 0) {
				move_to(m_attackers_last_direction.value());
			}
		}
	}
	std::forward_list<std::shared_ptr<defiler>>& defiler_list() {
		static std::forward_list<std::shared_ptr<defiler>> s;
		return s;
	}
	uint8_t defiler::scan_depth() const {
		return 8;
	}

	void defiler::door_entry_event(player_ptr_t& victim,const room_rnum room_id) {
		std::tuple<bool,direction_t,int> r = mods::scan::los_find_player_with_depth(this->player(),victim->uuid(),RCT->max_range);
		if(std::get<0>(r)) {
			auto weapon = this->player()->primary();
			mods::combat_composer::snipe_target(
			    this->player(),
			    victim->name().c_str(),
			    std::get<1>(r),
			    0,
			    weapon
			);
		}
	}
	namespace defiler_callbacks {
		bool dispatch_watcher(const uuid_t& defiler_uuid,player_ptr_t& player, const room_rnum& room_id) {
			uint32_t ctr =0;
			for(auto& defiler : defiler_list()) {
				if(defiler->uuid == defiler_uuid) {
					defiler->door_entry_event(player,room_id);
					++ctr;
				}
			}
			return ctr;
		}
	};
};
namespace mods::mobs::defiler_init {
	static constexpr std::array<obj_vnum,2> has_human_remains  = {
		197, /** 12 ounce steak */
		198, /** frozen liver */
	};

	ACMD(do_bioscan) {
		static constexpr std::string_view usage = "usage: bioscan <target>\r\nexample: bioscan meat\r\n";
		if(argshave()->size_gt(0)->passed() == false) {
			player->sendln(usage);
			return;
		}
		if(!player->has_contract()) {
			player->sendln("Scan all you want, but you're not currently under contract.");
			return;
		}
		if(argshave()->size_gt(0)->passed()) {
			auto target = argat(0);
			auto obj = OBJFIND(target,player);
			if(!obj) {
				player->sendln("Couldn't find anything!");
				return;
			} else {
				if(std::find(has_human_remains.cbegin(),has_human_remains.cend(),obj->item_number) != has_human_remains.cend()) {
					player->contract_custom_event(mods::contracts::custom_events_t::CEV_HUMAN_REMAINS_FOUND,obj->uuid);
					player->sendln("{grn}[+] Positive read: {red}HUMAN REMAINS{/red}");
				} else {
					player->sendln("{red}[-] Negative read{/red}");
				}
			}
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
		    mods::mobs::defiler::MOB_VNUM,
		    rezzes
		);
		mods::interpreter::add_user_command("bioscan",do_bioscan);
	}
};//end mods::mobs::defiler_init
#undef m_debug
#undef cmem
