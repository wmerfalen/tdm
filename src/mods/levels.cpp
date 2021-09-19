#include "levels.hpp"
#include "help.hpp"
#include "super-users.hpp"
#include "interpreter.hpp"
#include "screen-searcher.hpp"
#include "players/util.hpp"
#include "classes/advance-levels.hpp"
#include "rand.hpp"
#include "players/db-load.hpp"

#ifdef __MENTOC_MODS_LEVELS_DEBUG__
#define m_debug(A) std::cerr << "[mods::levels][debug]:" << A <<"\n";
#else
#define m_debug(A)
#endif

extern player_ptr_t new_player();
extern std::string sanitize_key(std::string key);
extern void obj_to_obj(obj_ptr_t from_object, obj_ptr_t to_object);
extern int immort_level_ok;
namespace mods::levels {
	static constexpr uint32_t EXP_MAX = 10000000;
	int level_exp(int level) {
		if(level > LVL_IMPL || level < 0) {
			log("SYSERR: Requesting exp for invalid level %d!", level);
			return 0;
		}

		/*
		 * Gods have exp close to EXP_MAX.  This statement should never have to
		 * changed, regardless of how many mortal or immortal levels exist.
		 */
		if(level > LVL_IMMORT) {
			return EXP_MAX - ((LVL_IMPL - level) * 1000);
		}

		switch(level) {
			case  0:
				return 0;

			case  1:
				return 1;

			case  2:
				return 250;

			case  3:
				return 500;

			case  4:
				return 1000;

			case  5:
				return 2000;

			case  6:
				return 4000;

			case  7:
				return 6000;

			case  8:
				return 9000;

			case  9:
				return 13500;

			case 10:
				return 25000;

			case 11:
				return 37500;

			case 12:
				return 75000;

			case 13:
				return 112500;

			case 14:
				return 150000;

			case 15:
				return 187500;

			case 16:
				return 225000;

			case 17:
				return 262500;

			case 18:
				return 300000;

			case 19:
				return 337500;

			case 20:
				return 375000;

			case 21:
				return 400000;

			case 22:
				return 430000;

			case 23:
				return 460000;

			case 24:
				return 490000;

			case 25:
				return 520000;

			case 26:
				return 550000;

			case 27:
				return 595000;

			case 28:
				return 640000;

			case 29:
				return 685000;

			case 30:
				return 740000;

			/* add new levels here */
			default:
			case LVL_IMMORT:
				return 8000000;
		}


		/*
		 * This statement should never be reached if the exp tables in this function
		 * are set up properly.  If you see exp of 123456 then the tables above are
		 * incomplete -- so, complete them!
		 */
		log("SYSERR: XP tables not set up correctly in %s:%d",__FILE__,__LINE__);
		return -1;
	}
	int gain_exp(player_ptr_t& player,int gain) {
		if(IS_NPC(player->cd())) {
			return -1;
		}
		player->exp() += gain;
		bool is_altered = 0;
		int num_levels = 0;
		if(player->level() == 0) {
			player->level() = 1;
			advance_level(player);
		}
		while(player->level() < LVL_IMMORT - immort_level_ok &&
		        player->exp() >= mods::levels::level_exp(player->level() + 1)) {
			player->level() += 1;
			num_levels++;
			advance_level(player);
			is_altered = TRUE;
			if(is_altered) {
				mudlog(BRF, MAX(LVL_IMMORT, GET_INVIS_LEV(player->cd())), TRUE, "%s advanced %d level%s to level %d.",
				       player->name().c_str(), num_levels, num_levels == 1 ? "" : "s", player->level());
				if(num_levels == 1) {
					player->sendln(ADVANCE_LEVEL_MESSAGE());
				} else {
					player->send(ADVANCE_MULTIPLE_LEVELS_MESSAGE().c_str(),num_levels);
				}
				//set_title(ch, NULL);
			}
		}//end while
		m_debug(green_str("gain_exp has your level as: ") << player->level());
		mods::players::db_load::save_from(player,mods::players::db_load::save_from_t::GAIN_EXP);
		return num_levels;
	}
	void gain_mp(player_ptr_t& player,int gain) {
		player->mp() += gain;
	}
	int gain_mp_from_killing(player_ptr_t& attacker,player_ptr_t& victim) {
		if(attacker->is_npc()) {
			return 0;
		}
		if(victim->is_npc()) {
			gain_mp(attacker,victim->cd()->mob_specials.mp);
			return victim->cd()->mob_specials.mp;
		}
		int32_t level_difference = attacker->level() - victim->level();
		int gain = 0;
		switch(level_difference) {
			case 1:
				gain = victim->level() + victim->level() * GAIN_MP_LEVEL_DIFFERENCE_POSITIVE_ONE();
				break;
			case 0:
				gain = victim->level() + victim->level() * GAIN_MP_LEVEL_DIFFERENCE_ZERO();
				break;
			case -1:
				gain = victim->level() + victim->level() * GAIN_MP_LEVEL_DIFFERENCE_NEGATIVE_ONE();
				break;
			case -2:
				gain = victim->level() + victim->level() * GAIN_MP_LEVEL_DIFFERENCE_NEGATIVE_TWO();
				break;
			default:
				break;
		}
		if(level_difference < -2 && level_difference >= -4) {
			gain = victim->level() + victim->level() * GAIN_MP_LEVEL_DIFFERENCE_BETWEEN_NEG_TWO_NEG_FOUR();
		}
		if(level_difference < -5 && level_difference >= -10) {
			gain = victim->level() + victim->level() * GAIN_MP_LEVEL_DIFFERENCE_BETWEEN_NEG_FIVE_NEG_TEN();
		}
		if(level_difference < -10) {
			gain = victim->level() + victim->level() * GAIN_MP_LEVEL_DIFFERENCE_LESS_THAN_NEG_TEN();
		}
		if(level_difference > 1) {
			gain = victim->level() + victim->level() * GAIN_MP_LEVEL_GREATER_THAN_ONE();
		}
		if(gain != 0) {
			gain_mp(attacker,gain);
		}
		return gain;
	}

	int gain_exp_from_killing(player_ptr_t& attacker,player_ptr_t& victim) {
		if(attacker->is_npc()) {
			return 0;
		}
		if(victim->is_npc()) {
			gain_exp(attacker,victim->cd()->mob_specials.experience);
			return victim->cd()->mob_specials.experience;
		}
		int32_t level_difference = attacker->level() - victim->level();
		int gain = 0;
		switch(level_difference) {
			case 1:
				gain = victim->level() + victim->level() * GAIN_EXP_LEVEL_DIFFERENCE_POSITIVE_ONE();
				break;
			case 0:
				gain = victim->level() + victim->level() * GAIN_EXP_LEVEL_DIFFERENCE_ZERO();
				break;
			case -1:
				gain = victim->level() + victim->level() * GAIN_EXP_LEVEL_DIFFERENCE_NEGATIVE_ONE();
				break;
			case -2:
				gain = victim->level() + victim->level() * GAIN_EXP_LEVEL_DIFFERENCE_NEGATIVE_TWO();
				break;
			default:
				break;
		}
		if(level_difference < -2 && level_difference >= -4) {
			gain = victim->level() + victim->level() * GAIN_EXP_LEVEL_DIFFERENCE_BETWEEN_NEG_TWO_NEG_FOUR();
		}
		if(level_difference < -5 && level_difference >= -10) {
			gain = victim->level() + victim->level() * GAIN_EXP_LEVEL_DIFFERENCE_BETWEEN_NEG_FIVE_NEG_TEN();
		}
		if(level_difference < -10) {
			gain = victim->level() + victim->level() * GAIN_EXP_LEVEL_DIFFERENCE_LESS_THAN_NEG_TEN();
		}
		if(level_difference > 1) {
			gain = victim->level() + victim->level() * GAIN_EXP_LEVEL_GREATER_THAN_ONE();
		}
		if(gain != 0) {
			gain_exp(attacker,gain);
		}
		return gain;
	}

	int reduce_exp_from_dying(player_ptr_t& player) {
		/** TODO: implement this for later tiers... */
		return 0;
#if 0
		if(player->is_npc()) {
			return 0;
		}
		if(player->level() >= 1 && player->level() <= NEWBIES_KEEP_EXP_LEVEL()) {
			return 0;
		}
		switch(player_tier(player->level())) {
			case 2:
				return gain_exp(player,player->level() * TIER_TWO_DEATH_EXP_LOSS());
			case 3:
				return gain_exp(player,player->level() * TIER_THREE_DEATH_EXP_LOSS());
			default:
				return gain_exp(player,player->level() * TIER_ELITE_DEATH_EXP_LOSS());
		}
#endif
	}

	/**
	 * extra rewards for advancing
	 */
	void reward_player_for_advancing_levels(player_ptr_t& player) {
		player->sendln(ADVANCE_LEVEL_CONGRATS());
	}


	void report_advances(std::array<uint16_t,STAT_INDEXES_SIZE> stats) {
		std::cerr <<
		          "hp:" << stats[STAT_HP] << "\n"\
		          << "strength:		" << stats[STAT_STRENGTH] << "\n"\
		          << "constitution:		" << stats[STAT_CONSTITUTION] << "\n"\
		          << "dexterity:		" << stats[STAT_DEXTERITY] << "\n"\
		          << "intelligence:		" << stats[STAT_INTELLIGENCE]<< "\n"\
		          << "wisdom:		" << stats[STAT_WISDOM] << "\n"\
		          << "electronics:		" << stats[STAT_ELECTRONICS] << "\n"\
		          << "chemistry:		" << stats[STAT_CHEMISTRY] << "\n"\
		          << "strategy:		" << stats[STAT_STRATEGY]<< "\n"\
		          << "marksmanship:		" << stats[STAT_MARKSMANSHIP] << "\n"\
		          << "sniping:		" << stats[STAT_SNIPING] << "\n"\
		          << "weapon_handling:		" << stats[STAT_WEAPON_HANDLING] << "\n"\
		          << "demolitions:		" << stats[STAT_DEMOLITIONS] << "\n"\
		          << "armor:		" << stats[STAT_ARMOR] << "\n"\
		          << "medical:		" << stats[STAT_MEDICAL] << "\n"\
		          << "charisma:		" << stats[STAT_CHARISMA] << "\n";
	}

	/*
	 * Example Triad.
	 * Triad rules:
	 * ------------
	 *  - Each field (i.e. Melee) must have atleast one point
	 *  - six points are allocated betwen M/W/I
	 *  - no two fields can have 3 points each at the same time
	 *  - speed and armor are 4 points allocated between each field
	 *  	- both fields must have atleast one point
	 *
	 *				- Military Triad
	 *						- [Melee]   [x] [ ] [ ]
	 *						- [Weapons] [x] [x] [x]
	 *						- [Intel]   [x] [x] [ ]
	 *					- Speed/Armor tradeoff MAX 4 points
	 *						- [Speed]   [x] [ ] [ ]
	 *						- [Armor]   [x] [x] [x]
	*/
	std::array<uint8_t,5> get_triads_by_class(player_class_t c) {
		std::array<uint8_t,5> m_triads;
		switch(c) {
			case SNIPER:
				/**
				 * --[
				 *  -- expert at sniper rifles and ranged combat
				[DONE]  	-- Extra 10.5% damage per tier (each tier is ten levels)
				 *  -- can attach underbarrels to sniper rifles
				 *  	-- shotgun underbarrel
				 *  	-- frag launcher
				 *  -- has access to emp/chaff grenades (replenishes)
				 *  	-- EMP causes damage to a 10x10x1 radius
				 *  		-- devices under the emp umbrella:
				 *  			-- Thermal vision does not work
				 *  			-- Night vision does not work
				 *  		-- TODO: flesh out this ability
				 *  -- can inject arenaline shot
				 *  	-- 100 movement points are added to your current movement points
				 *  	-- 25% chance of hitting first when forced into melee encounter
				 *  -- has xray shot
				 *  	-- targets don't need to be in line of sight
				 *  		-- tier 1: does damage
				 *  		-- tier 2: target becomes TRACKED and SCANNED
				 *  		-- tier 3: shot explodes causing damage to anyone in room
				 *  -- has access to corrosive/shrapnel/explosive claymores (replenishes)
				 *  	-- corrossive damage is dealt in addition to explosive damage
				 *  	-- shrapnel damage is dealt in addition to explosive damage
				 *  -- can mark and track an enemy
				 *  	-- tracked enemies take more damage
				 *  -- can target limb
				 *  	-- attacking right arm activates nerfs on target related to primary
				 *  	-- attacking left arm activates nerfs on target related to primary and secondary
				 *  -- can snipe objects (such as doors, cars, etc)
				 *  	-- cars will explode and light the room on fire
				 *  -- can fire "neutron" shell
				 *  	-- targets hit by shell cause radioactive and explosive damage to all room inhabitants
				 *  -- fleeing from melee is 25% more effective
				 *  -- Tactial Foam
				 *  	-- Causes target to not be able to move for 20 ticks per tier
				 *  --[
				 */
				return {
					1, // MELEE
					3, // WEAPONS
					2, // INTEL
					3, // SPEED
					1, // ARMOR
				};
			case CONTAGION:
				/**
				 * --[
				 *  -- life tap
				 *  	-- damage done to target adds life and mana to self
				 *
				 *  -- corpse explosion
				 *  	-- can detonate target corpse by firing a shell into it
				 *  	-- [ level 10 ] --
				 *  		-- unlocks shrapnel corpse explosion
				 *  			-- does 25% shrapnel damage
				 *  	-- [ level 15 ] --
				 *  		-- unlocks corrosive corpse explosion
				 *  			-- does 30% corrosive damage
				 *  	-- [ level 23 ] --
				 *  		-- unlocks hellfire corpse explosion
				 *  			-- does incendiary, radioactive, and anti-matter damage
				 *  				-- each 120% damage
				 *
				 *
				 * -- attract
				 *		-- draw enemies into a specific room
				 *
				 * -- confuse
				 * 		-- attacker loses concentration and becomes temporarily blinded
				 *
				 * -- hellfire incantation
				 *  	-- target becomes ingulfed in fire, continuously dealing incendiary damage
				 *
				 *	-- detonate limb
				 *		-- detonate target limb causing
				 *			-- 150% explosive damage to surrounding enemies
				 *		-- follows semantics of targetting limbs (left vs right, etc)
				 *
				 *	-- "Suffering Shot"
				 *  	-- fire special ammunition which sticks to the target and deals damage continuously
				 *  		for 50 ticks
				 *  -- [
				 */
				return {
					1, // MELEE
					2, // WEAPONS
					3, // INTEL
					3, // SPEED
					1, // ARMOR
				};
			case GHOST:
				/**
				 * --[
				 *  -- uses stealth and intel to deal extra damage
				 *  -- can dissipate
				 *  	-- when dissipated, enemies cannot see or detect you
				 *  -- time to install claymore mines are reduced by 50%
				 *  -- ranged combat has a 75% chance of the target not knowing
				 *  	which direction the shot came from
				 *  -- SMG's are silenced
				 *  	-- first attack with SMG's cause the target to not know who
				 *  		the attack is
				 *  -- SMG's have "initiative"
				 *  	-- when an enemy would attack you, you attack first
				 *  	-- when they attack, you immediately attack again
				 *  -- have access to smoke and flashbang grenades (replenishes)
				 *  -- "misdirection"
				 *  	-- spawn a holographic decoy that your enemies will attack instead of you
				 *	-- "Suffering Shot"
				 *  	-- fire special ammunition which sticks to the target and deals damage continuously
				 *  		for 50 ticks
				 *  -- [
				 */
				return {
					1, // MELEE
					3, // WEAPONS
					2, // INTEL
					3, // SPEED
					1, // ARMOR
				};
			case MARINE:
				/**
				 * --[
				 *  -- most well-balanced character
				 *  -- all wielded assault rifles get:
				 *  	-- +10% critical chance
				 *  	-- +15% free ammunition
				 *  	-- underbarrel frag launcher (ammo replenishes)
				 *  -- received shrapnel and incendiary damage is reduced by 25%
				 * 	-- "second wind" (passive)
				 * 		-- if a hit would cause death, you have a 23% chance of
				 * 			surviving with 20hp for 20 ticks. During this time
				 * 			you can move around but cannot attack. When over,
				 * 			your primary weapon does 10% more damage and gains
				 * 			10% explosive damage.
				 * 	-- received disorient affects are reduced in duration by 50%
				 * 	-- Shotguns deal same room damage at 2-3 rooms away
				 * 	-- "disarm"
				 * 		-- melee anatagonists get their primary dropped on the ground
				 * 		-- tier 3: melee antagonists get their primary weapon removed and destroyed
				 * 	-- assault rifles have a 10% chance of dealing incendiary damage
				 * --[
				 */
				return {
					2, // MELEE
					2, // WEAPONS
					2, // INTEL
					2, // SPEED
					2  // ARMOR
				};
			case BREACHER:
				/**
				 * -- [
				 *  -- expert with demolitions
				 *  -- breach charges replenish automatically
				 *  -- can retrofit shotguns with explosive breach underbarrel
				 *  -- all wielded weapons get +10% explosive damage
				 *  -- can remotely seal a room for the purpose of amplifying detonations
				 *  -- can walk in a direction and automatically breach through
				 *  	-- can sometimes make new rooms
				 *  -- breaching doors causes room inhabitants to take damage from explosion
				 *  -- "breach and clear"
				 *  	-- breach a door and immediatel throw a flash bang into the room
				 *  	-- inhabitants get disoriented and take more damage
				 *  -- "knockdown"
				 *  	-- fire a shot that knocks the enemy to the ground
				 *  	-- knocked down enemies take more damage and are vulnerable to "detain"
				 *  -- "detain"
				 *  	-- disarm and zip-tie an enemy
				 *  -- "entry denial"
				 *  	-- prevent any enemy from entering your room by guarding a specific entrance
				 *  	-- friendlies can snipe around you
				 *  	-- you can use your secondary to deal damage to NPCs trying to enter
				 *  -- Shotguns deal 10% extra damage
				 *  -- SMG's deal 10% extra damage
				 *  -- SMG's have corrossive damage
				 *  	-- corrossive damage from SMG's cause enemy armor to be less effective
				 *  -- SMG's in same-room engagements have a chance of dealing shotgun damage
				 *  -- received shrapnel and explosive damage reduced by 25%
				 *  -- [
				 */
				return {
					3, // MELEE
					2, // WEAPONS
					1, // INTEL
					1, // SPEED
					3 // ARMOR
				};
			case MEDIC:
				/**
				 * -- [
				 *  -- built like a tank, but not very fast
				 *  -- can heal self and teammates
				 *  -- has parasitic ammunition
				 *  -- can craft PED's
				 *  -- can deal extra damage with SMG's and Shotguns
				 *  -- can dual wield shotguns
				 *  -- has access to ballistic shield
				 *  -- [
				 */
				return {
					1, // MELEE
					2, // WEAPONS
					3, // INTEL
					1, // SPEED
					3, // ARMOR
				};
			case ENGINEER:
				/**
				 * -- [
				 *  -- decent at weaponry
				 *  -- famliarity with gadgets/demolitions/attachments
				 *  -- can setup entry denial devices to keep targets at ideal sniping distance
				 *  -- can construct in-field turret
				 *  	-- shrapnel upgrade
				 *  	-- corrossive upgrade
				 *  	-- radioactive upgrade
				 *  -- can attach items to rifles
				 *  -- can repair melee items
				 *  -- sensor grenades have a wider area of affect
				 *  -- emp grenades do not affect engineer
				 *  -- can request UAV scan
				 *  	-- scanned enemies become TRACKED and take more damage
				 *
				 *  -- [
				 */
				return {
					1, // MELEE
					2, // WEAPONS
					3, // INTEL
					2, // SPEED
					2, // ARMOR
				};
			case SUPPORT:
				/**
				 * -- [
				 * - Quick moving, expert at weaponry and can carry tons of weight.
				 * - not much armor due to quickness to bring ammo/weapons/items to teammates
				 * - can call in loadout drops
				 * - can pilot drone that drops items near squads that need it
				 * -- [
				 */
				return {
					2, // MELEE
					3, // WEAPONS
					1, // INTEL
					3, // SPEED
					1, // ARMOR
				};
			default:
				return {0,0,0,0,0};
		}

		return m_triads;
	}

	uint8_t player_tier(const uint8_t& level) {
		return (level / 10) + 1;
	}
	uint8_t player_tier(player_ptr_t& player) {
		return player->level() / 10 + 1;
	}

	std::array<uint16_t,STAT_INDEXES_SIZE> calculate_based_on_triads(std::array<uint8_t,5> triads,int level) {
		std::array<uint16_t,STAT_INDEXES_SIZE> s = {0};
		uint16_t
		/** melee and armor related */
		hp = 0,	/** also speed related */
		mana = 0,
		move = 0,

		/** melee related */
		strength = 0,
		constitution = 0,	/** also speed related */
		dexterity = 0,		/** also speed related */

		/** intel related */
		intelligence =0,
		wisdom = 0,
		electronics = 0,
		chemistry = 0,
		strategy = 0,

		/** weapons related */
		marksmanship = 0,
		sniping = 0,
		weapon_handling = 0,	/** also speed related */

		/** weapons and intel related */
		demolitions = 0,	/** armor triad value reduces explosive damage */

		/** armor related */
		armor = 0,

		/** armor and intel related */
		medical = level * 3,

		/** unclassified */
		charisma = 0
		           ;
		switch(triads[MELEE]) {
			case 1:
				hp += level * (LOW_TRIAD);
				strength += level * (LOW_TRIAD);
				dexterity += level * (LOW_TRIAD);
				constitution += level * (LOW_TRIAD);
				armor += level * (LOW_TRIAD);
				move += level * (LOW_TRIAD);
				break;
			case 2:
				hp += level * (MID_TRIAD);
				strength += level * (MID_TRIAD);
				dexterity += level * (MID_TRIAD);
				constitution += level * (MID_TRIAD);
				armor += level * (MID_TRIAD);
				move += level * (MID_TRIAD);

				break;
			case 3:
				hp += level * (HIGH_TRIAD);
				strength += level * (HIGH_TRIAD);
				dexterity += level * (HIGH_TRIAD);
				constitution += level * (HIGH_TRIAD);
				armor += level * (HIGH_TRIAD);
				move += level * (HIGH_TRIAD);

				break;
			default:
				std::cerr << red_str("generic_advance_hp encountered odd MELEE triad:") << triads[MELEE] << "\n";
				break;
		}
		switch(triads[WEAPONS]) {
			case 1:
				weapon_handling += level * (LOW_TRIAD);
				sniping += level * (LOW_TRIAD);
				marksmanship += level * (LOW_TRIAD);
				demolitions += level * (LOW_TRIAD);
				break;
			case 2:
				weapon_handling += level * (MID_TRIAD);
				sniping += level * (MID_TRIAD);
				marksmanship += level * (MID_TRIAD);
				demolitions += level * (MID_TRIAD);
				break;
			case 3:
				weapon_handling += level * (HIGH_TRIAD);
				sniping += level * (HIGH_TRIAD);
				marksmanship += level * (HIGH_TRIAD);
				demolitions += level * (HIGH_TRIAD);
				break;
			default:
				std::cerr << red_str("generic_advance_hp encountered odd WEAPONS triad:") << triads[WEAPONS] << "\n";
				break;
		}
		switch(triads[INTEL]) {
			case 1:
				intelligence += level * (LOW_TRIAD);
				electronics += level * (LOW_TRIAD);
				chemistry += level * (LOW_TRIAD);
				strategy += level * (LOW_TRIAD);
				charisma += level * (LOW_TRIAD);
				wisdom += level * (LOW_TRIAD);
				demolitions += level * (LOW_TRIAD);
				mana += level * (LOW_TRIAD);
				break;
			case 2:
				intelligence += level * (MID_TRIAD);
				electronics += level * (MID_TRIAD);
				chemistry += level * (MID_TRIAD);
				strategy += level * (MID_TRIAD);
				charisma += level * (MID_TRIAD);
				wisdom += level * (MID_TRIAD);
				demolitions += level * (MID_TRIAD);
				mana += level * (MID_TRIAD);
				break;
			case 3:
				intelligence += level * (HIGH_TRIAD);
				electronics += level * (HIGH_TRIAD);
				chemistry += level * (HIGH_TRIAD);
				strategy += level * (HIGH_TRIAD);
				charisma += level * (HIGH_TRIAD);
				wisdom += level * (HIGH_TRIAD);
				demolitions += level * (HIGH_TRIAD);
				mana += level * (HIGH_TRIAD);
				break;
			default:
				std::cerr << red_str("generic_advance_hp encountered odd INTEL triad:") << triads[INTEL] << "\n";
				break;
		}
		switch(triads[SPEED]) {
			case 1:
				constitution += level * (LOW_TRIAD);
				dexterity += level * (LOW_TRIAD);
				move += level * (LOW_TRIAD);
				mana += level * LOW_TRIAD;//always
				break;
			case 2:
				constitution += level * (MID_TRIAD);
				dexterity += level * (MID_TRIAD);
				move += level * (MID_TRIAD);
				mana += level * LOW_TRIAD;//always
				break;
			case 3:
				constitution += level * (HIGH_TRIAD);
				dexterity += level * (HIGH_TRIAD);
				move += level * (HIGH_TRIAD);
				mana += level * LOW_TRIAD;//always
				break;
			default:
				std::cerr << red_str("generic_advance_hp encountered odd SPEED triad:") << triads[SPEED] << "\n";
				break;
		}
		switch(triads[ARMOR]) {
			case 1:
				hp += level * (LOW_TRIAD);
				constitution += level * (LOW_TRIAD);
				armor += level * (LOW_TRIAD);
				move += level * HIGH_TRIAD;// inverse
				break;
			case 2:
				hp += level * (MID_TRIAD);
				constitution += level * (MID_TRIAD);
				armor += level * (MID_TRIAD);
				move += level * MID_TRIAD;//inverse
				break;
			case 3:
				hp += level * (HIGH_TRIAD);
				constitution += level * (HIGH_TRIAD);
				armor += level * (HIGH_TRIAD);
				move += level * LOW_TRIAD;//inverse
				break;
			default:
				std::cerr << red_str("generic_advance_hp encountered odd ARMOR triad:") << triads[ARMOR] << "\n";
				break;
		}
		s[STAT_HP] = (hp * (32.5)) + (level * 15.53);
		s[STAT_MANA] = (mana * (32.5)) + (level * 15.53);
		s[STAT_MOVE] = (move * (32.5)) + (level * 15.53);
		s[STAT_STRENGTH] = strength;
		s[STAT_CONSTITUTION] = constitution;
		s[STAT_DEXTERITY] = dexterity;
		s[STAT_INTELLIGENCE] = intelligence;
		s[STAT_WISDOM] = wisdom;
		s[STAT_ELECTRONICS] = electronics;
		s[STAT_CHEMISTRY] = chemistry;
		s[STAT_STRATEGY] = strategy;
		s[STAT_MARKSMANSHIP] = marksmanship;
		s[STAT_SNIPING] = sniping;
		s[STAT_WEAPON_HANDLING] = weapon_handling;
		s[STAT_DEMOLITIONS] = demolitions;
		s[STAT_ARMOR] = armor;
		s[STAT_MEDICAL] = medical;
		s[STAT_CHARISMA] = charisma;
		return s;
	}

	std::array<uint16_t,STAT_INDEXES_SIZE> calculate_based_on_player(player_ptr_t& player) {
		auto stats = calculate_based_on_triads(player->triads(),player->level());
		switch(player->get_class()) {
			case player_class_t::GHOST:  {
					/**
					 * ghosts
					 * -------
					 *  Get +2 sniping
					 *  Get +2 marksmanship
					 *  Get +1 chemistry
					 */
					stats[STAT_SNIPING] += 2;
					stats[STAT_MARKSMANSHIP] += 2;
					stats[STAT_CHEMISTRY] += 1;
					//stats[STAT_MANA] /= 2.3;
					//stats[STAT_MOVE] /= 2.3;
					break;
				}
			case player_class_t::MARINE: {
					/**
					 * marines
					 * --------
					 *  Get +2 Constitution
					 *  Get +2 weapon handling
					 *  Get +1 medical
					 */
					stats[STAT_CONSTITUTION] += 2;
					stats[STAT_WEAPON_HANDLING] += 2;
					stats[STAT_MEDICAL] += 1;
					/**
					 * Nerfs
					 */
					//stats[STAT_MANA] /= 3;
					//stats[STAT_MOVE] /= 2.1;
					break;
				}
			case player_class_t::BREACHER: {
					/**
					 * Breachers
					 * ----------
					 *  Get +2 Armor
					 *  Get +2 demolitions
					 *  Get +1 strength
					 */
					stats[STAT_ARMOR] += 2;
					stats[STAT_DEMOLITIONS] += 2;
					stats[STAT_STRENGTH] += 1;
					/**
					 * Nerfs
					 */
					//stats[STAT_MANA] /= 3.5;
					//stats[STAT_MOVE] /= 3;
					break;
				}
			default: {
					std::cerr << "warning: unknown class passed to calculate_based_on_player!\n";
					break;
				}
		}
		return stats;
	}

	void generic_advance(player_ptr_t& player) {
		auto s = calculate_based_on_player(player);
		report_advances(s);
		player->hp() = s[STAT_HP];
		player->max_hp() = s[STAT_HP];
		player->mana() = s[STAT_MANA];
		player->max_mana() = player->mana();
		player->move() = s[STAT_MOVE];
		player->max_move() = player->move();
		player->strength() = s[STAT_STRENGTH];
		player->constitution() = s[STAT_CONSTITUTION];
		player->dexterity() = s[STAT_DEXTERITY];
		player->intelligence() = s[STAT_INTELLIGENCE];
		player->wisdom() = s[STAT_WISDOM];
		player->electronics() = s[STAT_ELECTRONICS];
		player->chemistry() = s[STAT_CHEMISTRY];
		player->strategy() = s[STAT_STRATEGY];
		player->marksmanship() = s[STAT_MARKSMANSHIP];
		player->sniping() = s[STAT_SNIPING];
		player->weapon_handling() = s[STAT_WEAPON_HANDLING];
		player->demolitions() = s[STAT_DEMOLITIONS];
		player->real_armor() = s[STAT_ARMOR];
		player->medical() = s[STAT_MEDICAL];
		player->charisma() = s[STAT_CHARISMA];
		player->practice_sessions() += PRACTICE_SESSIONS_PER_LEVEL_UP();
	}


	/**
	 * This is the ideal function we want to use as it makes use of
	 * the player's specific class pointer. i.e.: player->ghost()
	 * The hope is that once all classes have been fully implemented,
	 * we can phase out the generic_* functions completely.
	 */
	void advance_level(player_ptr_t& player) {
		generic_advance(player);
		mods::players::db_load::save_from(player,mods::players::db_load::save_from_t::GAIN_EXP);
		switch(player->get_class()) {
			case GHOST:
				mods::classes::ghost_advance_level(player);
				break;
			default:
				break;
		}
	}
	int csv_export_report() {
		FILE* fp = fopen("levels.csv", "w+");
		if(!fp) {
			return -1;
		}
		std::string header = "player_class,level,HP,Mana,Move,Strength,con,dex,intel,wis,electronics,chemistry,strategy,marksmanship,sniping,weapon_handling,demolitions,armor,medical,charisma\n";
		fwrite(header.c_str(),sizeof(char),header.length(),fp);
		for(const auto& player_class : {
		            player_class_t::GHOST,
		            player_class_t::MARINE,
		            player_class_t::BREACHER
		        }) {
			for(unsigned player_level = 1; player_level <= LVL_IMMORT; ++player_level) {
				std::string str_class = mods::util::player_class_to_string(player_class);
				auto player = new_player();
				player->set_class(player_class);
				player->level() = player_level;

				auto stats = mods::levels::calculate_based_on_player(player);
				std::string s;
				s = str_class + "," + std::to_string(player_level) + ",";
				fwrite(s.c_str(),sizeof(char),s.length(),fp);
				for(int i=0; i < stats.size(); ++i) {
					s = std::to_string((int)stats[i]);
					if(stats.size() > i+1) {
						s += ",";
					}
					fwrite(s.c_str(), sizeof(char),s.length(),fp);
				}
				fwrite("\n",1,1,fp);
			}
		}
		fclose(fp);
		return 0;
	}

	/**
	 * admin debug command
	 */
	SUPERCMD(do_award_exp_by_name) {
		ADMIN_REJECT();
		DO_HELP_WITH_ZERO("award_exp_by_name");
		/** code here */
		auto vec_args = PARSE_ARGS();
		static const char* usage = "usage: award_exp_by_name <player> <exp>\r\n";
		if(vec_args.size() < 2) {
			player->errorln(usage);
			SEND_HELP("award_exp_by_name");
			return;
		}

		auto found = mods::players::util::find_player_by_name(vec_args[0]);
		if(!found.has_value()) {
			player->sendln("Couldn't find a player by that name.");
			player->errorln(usage);
			return;
		}
		auto found_player = ptr_by_uuid(found.value());
		player->sendln("Setting...");
		auto opt = mods::util::stoi_optional<int>(vec_args[1]);
		if(!opt.has_value()) {
			player->errorln("Invalid integer encountered. Please specify a valid amount of experience points.");
			player->errorln(usage);
			return;
		}
		int used_to = found_player->exp();
		mods::levels::gain_exp(found_player,opt.value());
		player->send("Player '%s' used to have %d experience points and now has %d\r\n",found_player->name().c_str(),used_to,found_player->exp());
		player->sendln("Done.");
		ADMIN_DONE();
	}
	SUPERCMD(do_csv_export_levels) {
		ADMIN_REJECT();
		DO_HELP_WITH_ZERO("csv_export_levels");
		auto status = csv_export_report();
		if(status == -1) {
			player->errorln("Unable to write to levels.csv");
			return;
		}
		player->admin_success("Successfully wrote to levels.csv.");
		ADMIN_DONE();
	}
	ACMD(do_exp) {
		player->send("{grn}You have %d experience points.{/grn}\r\n",player->exp());
	}


	void init() {
		mods::interpreter::add_command("award_exp_by_name", POS_RESTING, do_award_exp_by_name, LVL_BUILDER,0);
		mods::interpreter::add_command("csv_export_levels", POS_RESTING, do_csv_export_levels, LVL_BUILDER,0);
		mods::interpreter::add_command("exp", POS_RESTING, do_exp, 0,0);
	}
};
#undef m_debug
