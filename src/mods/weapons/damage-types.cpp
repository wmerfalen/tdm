#include "damage-types.hpp"
#include "../../spells.h"
#include "../object-utils.hpp"
#include "../injure.hpp"
#define dty_debug(a) std::cerr << "[mods::weapons::damage_types][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
extern int ok_damage_shopkeeper(char_data *ch, char_data *victim);
extern void die(char_data* killer,char_data *victim);
extern void die(char_data *victim);
extern void set_fighting(char_data *ch, char_data *vict);
extern void appear(char_data *ch);
extern int pk_allowed;
extern void check_killer(char_data *ch, char_data *vict);
extern int skill_message(int dam, char_data *ch, char_data *vict, int attacktype);
extern void dam_message(int dam, char_data *ch, char_data *victim, int w_type);
extern ACMD(do_flee);
extern void group_gain(char_data *ch, char_data *victim);
extern void solo_gain(char_data *ch, char_data *victim);
extern void forget(char_data *ch,char_data *victim);
#ifndef IS_WEAPON
	#define IS_WEAPON(type) (((type) >= TYPE_HIT) && ((type) < TYPE_SUFFERING))
#endif
namespace mods::weapons::damage_types {
	using vpd = mods::scan::vec_player_data;

	
	namespace legacy {
		int step_one(char_data *ch, char_data *victim, int dam, int attacktype) {
			/*TODO: Modify this code to allow sniping */
			if(GET_POS(victim) <= POS_DEAD) {
				/* This is "normal"-ish now with delayed extraction. -gg 3/15/2001 */
				if(PLR_FLAGGED(victim, PLR_NOTDEADYET) || MOB_FLAGGED(victim, MOB_NOTDEADYET)) {
					return (-1);
				}

				if(ch){
					die(ch,victim);
				}else{
					die(victim);
				}
				return (-1);			/* -je, 7/7/92 */
			}
			/* peaceful rooms */
			if(ch && ch != victim && ROOM_FLAGGED(IN_ROOM(ch), ROOM_PEACEFUL)) {
				send_to_char(ch, "This room just has such a peaceful, easy feeling...\r\n");
				return (0);
			}

			/* shopkeeper protection */
			if(ch && !ok_damage_shopkeeper(ch, victim)) {
				return (0);
			}
			return 1;
		}
		int step_two(char_data *ch, char_data *victim, int dam, int attacktype) {
			/* You can't damage an immortal! */
			if(!IS_NPC(victim) && (GET_LEVEL(victim) >= LVL_IMMORT)) {
				dam = 0;
			}
			if(victim != ch) {
				/* Start the attacker fighting the victim */
				if(GET_POS(ch) > POS_STUNNED && (FIGHTING(ch) == NULL)) {
					set_fighting(ch, victim);
				}

				/* Start the victim fighting the attacker */
				if(GET_POS(victim) > POS_STUNNED && (FIGHTING(victim) == NULL)) {
					set_fighting(victim, ch);

					if(MOB_FLAGGED(victim, MOB_MEMORY) && !IS_NPC(ch)) {
						remember(victim, ch);
					}
				}
				/* If you attack a pet, it hates your guts */
				if(victim->master == ch) {
					stop_follower(victim);
				}

				/* If the attacker is invisible, he becomes visible */
				if(AFF_FLAGGED(ch, AFF_INVISIBLE | AFF_HIDE)) {
					appear(ch);
				}
				return 1;
			}
			return -1;
		}
		int get_damage(char_data *ch, char_data *victim, int dam, int attacktype) {
			/* Cut damage in half if victim has sanct, to a minimum 1 */
			if(AFF_FLAGGED(victim, AFF_SANCTUARY) && dam >= 2) {
				dam /= 2;
			}

			/* Check for PK if this is not a PK MUD */
			if(!pk_allowed) {
				check_killer(ch, victim);

				if(PLR_FLAGGED(ch, PLR_KILLER) && (ch != victim)) {
					dam = 0;
				}
			}
			return dam;
		}
		void deal_damage(char_data *ch, char_data *victim, int dam, int attacktype) {
			/* Set the maximum damage per round and subtract the hit points */
			dam = MAX(MIN(dam, 100), 0);
			GET_HIT(victim) -= dam;
		}

		void gain_experience(char_data *ch, char_data *victim, int dam, int attacktype) {
			/* Gain exp for the hit */
			if(ch != victim) {
				gain_exp(ch, GET_LEVEL(victim) * dam);
			}
		}

		void update_position(char_data* ch){
			update_pos(ch);
		}

		void send_combat_messages(char_data *ch, char_data *victim, int dam, int attacktype) {
			MENTOC_PREAMBLE();
			/*
			 * skill_message sends a message from the messages file in lib/misc.
			 * dam_message just sends a generic "You hit $n extremely hard.".
			 * skill_message is preferable to dam_message because it is more
			 * descriptive.
			 *
			 * If we are _not_ attacking with a weapon (i.e. a spell), always use
			 * skill_message. If we are attacking with a weapon: If this is a miss or a
			 * death blow, send a skill_message if one exists; if not, default to a
			 * dam_message. Otherwise, always send a dam_message.
			 */
			if(ch){
				if(!IS_WEAPON(attacktype)) {
					skill_message(dam, ch, victim, attacktype);
				} else {
					if(GET_POS(victim) == POS_DEAD || dam == 0) {
						if(!skill_message(dam, ch, victim, attacktype)) {
							dam_message(dam, ch, victim, attacktype);
						}
					} else {
						dam_message(dam, ch, victim, attacktype);
					}
				}
			}
			/* Use send_to_char -- act() doesn't send message if you are DEAD. */
			switch(GET_POS(victim)) {
				case POS_MORTALLYW:
					act("$n is mortally wounded, and will die soon, if not aided.", TRUE, victim, 0, 0, TO_ROOM);
					send_to_char(victim, "You are mortally wounded, and will die soon, if not aided.\r\n");
					break;

				case POS_INCAP:
					act("$n is incapacitated and will slowly die, if not aided.", TRUE, victim, 0, 0, TO_ROOM);
					send_to_char(victim, "You are incapacitated an will slowly die, if not aided.\r\n");
					break;

				case POS_STUNNED:
					act("$n is stunned, but will probably regain consciousness again.", TRUE, victim, 0, 0, TO_ROOM);
					send_to_char(victim, "You're stunned, but will probably regain consciousness again.\r\n");
					break;

				case POS_DEAD:
					act("$n is dead!  R.I.P.", FALSE, victim, 0, 0, TO_ROOM);
					send_to_char(victim, "You are dead!  Sorry...\r\n");
					break;

				default:			/* >= POSITION SLEEPING */
					if(dam > (GET_MAX_HIT(victim) / 4)) {
						send_to_char(victim, "That really did HURT!\r\n");
					}

					if(GET_HIT(victim) < (GET_MAX_HIT(victim) / 4)) {
						send_to_char(victim, "{red}You wish that your wounds would stop BLEEDING so much!{/red}\r\n");

						if(ch != victim && MOB_FLAGGED(victim, MOB_WIMPY)) {
							do_flee(victim, NULL, 0, 0,player);
						}
					}

					if(!IS_NPC(victim) && GET_WIMP_LEV(victim) && (victim != ch) &&
							GET_HIT(victim) < GET_WIMP_LEV(victim) && GET_HIT(victim) > 0) {
						send_to_char(victim, "You wimp out, and attempt to flee!\r\n");
						do_flee(victim, NULL, 0, 0,player);
					}

					break;
			}

		}
		void help_linkless(char_data *ch, char_data *victim, int dam, int attacktype) {
			MENTOC_PREAMBLE();
			/* Help out poor linkless people who are attacked */
			if(!IS_NPC(victim) && !(victim->has_desc) && GET_POS(victim) > POS_STUNNED) {
				do_flee(victim, NULL, 0, 0,player);

				if(!FIGHTING(victim)) {
					act("$n is rescued by divine forces.", FALSE, victim, 0, 0, TO_ROOM);
					GET_WAS_IN(victim) = IN_ROOM(victim);
					char_from_room(victim);
					char_to_room(victim, 0);
				}
			}
		}
		int perform_damage_cleanup(char_data *ch, char_data *victim, int dam, int attacktype) {
			/* stop someone from fighting if they're stunned or worse */
			if(ch && GET_POS(victim) <= POS_STUNNED && FIGHTING(victim) != NULL) {
				stop_fighting(victim);
			}

			/* Uh oh.  Victim died. */
			if(GET_POS(victim) == POS_DEAD) {
				if(ch && ch != victim && (IS_NPC(victim) || victim->has_desc)) {
					if(AFF_FLAGGED(ch, AFF_GROUP)) {
						group_gain(ch, victim);
					} else {
						solo_gain(ch, victim);
					}
				}

				if(!IS_NPC(victim)) {
					if(ch){
						mudlog(BRF, LVL_IMMORT, TRUE, "%s killed by %s at %s", GET_NAME(victim).c_str(), GET_NAME(ch).c_str(), world[IN_ROOM(victim)].name.c_str());
					}else{
						mudlog(BRF, LVL_IMMORT, TRUE, "%s killed at %s", GET_NAME(victim).c_str(), world[IN_ROOM(victim)].name.c_str());
					}

				}

				if(ch){
					die(ch,victim);
				}else{
					die(victim);
				}
				return (-1);
			}
			return dam;
		}
		int damage(char_data *ch, char_data *victim, int dam, int attacktype) {
			if(step_one(ch,victim,dam,attacktype) <= 0){
				return -1;
			}
			if(step_two(ch,victim,dam,attacktype) <= 0){
				return -1;
			}
			dam = get_damage(ch,victim,dam,attacktype);
			deal_damage(ch,victim,dam,attacktype);
			gain_experience(ch,victim,dam,attacktype);
			update_position(victim);
			send_combat_messages(ch,victim,dam,attacktype);
			help_linkless(ch,victim,dam,attacktype);
			return perform_damage_cleanup(ch,victim,dam,attacktype);
		}
	};//end namespace legacy

	void deal_hp_damage(player_ptr_t& player, uint16_t damage){
		auto & hp = player->hp();
		int hp_after = hp;
		hp_after -= damage;
		if(hp_after < 0){
			hp_after = 0;
		}
		auto victim = player->cd();
		if(legacy::step_one(nullptr,victim,damage,0) <= 0){
			return;
		}

		legacy::deal_damage(nullptr,victim,damage,0);
		legacy::update_position(victim);
		legacy::send_combat_messages(nullptr,victim,damage,0);
	}

	bool attack_injures(obj_ptr_t& weapon) {
		/** TODO: calculate other buffs/nerfs here */
		return mods::injure::do_injure_roll(weapon->rifle()->attributes->chance_to_injure);
	}

	namespace elemental {
		namespace fire {
			void damage(player_ptr_t& player, uint16_t damage){
				player->send(mods::values::MSG_FIRE_DAMAGE().c_str(),damage); 
				deal_hp_damage(player,damage);
			}
			void smoke_damage(player_ptr_t& player, uint16_t damage){
				player->send(mods::values::MSG_SMOKE_DAMAGE().c_str(),damage); 
				deal_hp_damage(player,damage);
			}
		};
	};

	void fire_damage(player_ptr_t& player, uint16_t damage){
		elemental::fire::damage(player,damage);
	}
	void smoke_damage(player_ptr_t& player, uint16_t damage){
		elemental::fire::smoke_damage(player,damage);
	}
	
	/**
	 * @brief This is needed because we feed the damage and the attacktype to damage()
	 * Unfortunately, the damage() function does so much and is such a core part of legacy cm
	 * that I do not want to reinvent it... 
	 * @param weapon
	 *
	 * @return 
	 */
	int get_legacy_attack_type(obj_ptr_t& weapon){
		switch((mw_rifle)weapon->rifle()->attributes->type){
			case mw_rifle::SNIPER:
				return TYPE_SNIPE;
			case mw_rifle::SHOTGUN:
				return TYPE_SHOTGUN;
			case mw_rifle::SUB_MACHINE_GUN:
				return TYPE_SUB_MACHINE_GUN;
			case mw_rifle::LIGHT_MACHINE_GUN:
				return TYPE_LIGHT_MACHINE_GUN;
			default:
				log("SYSERR: invalid rifle type");
				return TYPE_SNIPE;
		}
	}

	/**
	 * @brief sprays using the primary weapon
	 *
	 * @param player
	 * @param direction
	 */
	void spray_direction(player_ptr_t& player,int direction){
		auto weapon = player->primary();
		if(!weapon || !weapon->has_rifle()){
			player->sendln("You aren't wielding any weapon.");
			return;
		}
		if(!player->weapon_cooldown_expired(0)){
			player->sendln("Weapon on cooldown.");
			return;
		}
		/* Check ammo */
		if(mods::object_utils::get_ammo(weapon) == 0) {
			player->sendln("{gld}*CLICK*{/gld} Your weapon is out of ammo!");
			return;
		}

		/** FIXME : grab weapon's accuracy and apply accurace modifications */

		int dam = 0;
		int damage_dice = weapon->rifle()->attributes->damage_dice_count;
		int damage_sides = weapon->rifle()->attributes->damage_dice_sides;
		int crit_range = weapon->rifle()->attributes->critical_range;
		int crit_chance = weapon->rifle()->attributes->critical_chance;
		int critical_bonus = 0;

		vpd scan;
		mods::scan::los_scan_direction(player->cd(),weapon->rifle()->attributes->max_range,&scan,direction);

		player->set_fight_timestamp();
		decrease_spray_shot_ammo(weapon);
		for(auto && scanned_target : scan) {
			dam = 0;
			auto victim = ptr(scanned_target.ch);
			if(!victim){
				continue;
			}
			if(dice(1,(100 * scanned_target.distance)) <= mods::values::SPRAY_CHANCE()){
				player->send(MSG_HIT().c_str());
				dam += weapon->rifle()->attributes->base_stat_list->at(scanned_target.distance).damage;
			}
			/** calculate headshot */
			if(dice(1,(100 * scanned_target.distance)) <= mods::values::SPRAY_HEADSHOT_CHANCE()){
				player->send(MSG_HEADSHOT().c_str());
				dam = victim->hp();
			}
			if(scanned_target.distance == crit_range){
				if(dice(1,(100 * scanned_target.distance)) <= crit_chance){
					player->send(MSG_CRITICAL().c_str());
					critical_bonus = dice(
							damage_dice,
							damage_sides
					);
					critical_bonus /= mods::values::SPRAY_CRITICAL_REDUCTION_DIVISOR();
					dam += critical_bonus;
				}
			}
			if(dam && victim->position() > POS_DEAD) {
				if(victim->is_npc()){
					damage(player->cd(),victim->cd(),dam,get_legacy_attack_type(weapon));
					remember(victim->cd(),player->cd());
					return;
				}
			}else{
				stop_fighting(player->cd());
				stop_fighting(victim->cd());
			}
		}

		return;

	}



	/**
	 * @brief if you know your target's name and direction, then use this to initiate the attack
	 *
	 * @param player
	 * @param victim_name
	 * @param direction
	 */
	void rifle_attack_by_name(player_ptr_t& player,std::string_view victim_name,int direction){
		auto weapon = player->primary();
		if(!weapon || !weapon->has_rifle()){
			player->sendln("You aren't wielding any weapon.");
			return;
		}
		vpd scan;
		/** TODO: add onto max range if has buff */
		mods::scan::los_scan_direction(player->cd(),weapon->rifle()->attributes->max_range,&scan,direction);
		player_ptr_t victim = nullptr;

		for(auto && scanned_target : scan) {
			if(mods::util::fuzzy_match(victim_name.data(),scanned_target.ch->player.name.c_str())) {
				/** FIXME needs to be weapon's max range not global max range */
				if(scanned_target.distance > weapon->rifle()->attributes->max_range){
					player->sendln("That target is out of range!");
					return;
				}
				victim = ptr(scanned_target.ch);
				rifle_attack(player,weapon,victim,scanned_target.distance);
				return;
			}
		}
	}

	/**
	 * @brief reduces the amount of ammo used from spraying
	 *
	 * @param weapon
	 */
	void decrease_spray_shot_ammo(obj_ptr_t& weapon) {
		/** TODO: if weapon has a bullet printer mod, calculate ammo */
		if(weapon->rifle_instance->ammo == 0){
			return;
		}
		switch((mw_rifle)weapon->rifle()->attributes->type){
			case mw_rifle::LIGHT_MACHINE_GUN:
				weapon->rifle_instance->ammo -= mods::values::SPRAY_SHOT_LIGHT_MACHINE_GUN();
				break;

			case mw_rifle::SUB_MACHINE_GUN:
				weapon->rifle_instance->ammo -= mods::values::SPRAY_SHOT_SUB_MACHINE_GUN();
				break;

			case mw_rifle::SHOTGUN:
				weapon->rifle_instance->ammo -= mods::values::SPRAY_SHOT_SHOTGUN();
				break;

			case mw_rifle::SNIPER:
				weapon->rifle_instance->ammo -= mods::values::SPRAY_SHOT_SNIPER();
				break;

			case mw_rifle::ASSAULT_RIFLE:
				weapon->rifle_instance->ammo -= mods::values::SPRAY_SHOT_ASSAULT_RIFLE();
				break;
			case mw_rifle::HANDGUN:
			case mw_rifle::PISTOL:
				weapon->rifle_instance->ammo -= mods::values::SPRAY_SHOT_HANDGUN();
				break;
			case mw_rifle::MACHINE_PISTOL:
				weapon->rifle_instance->ammo -= mods::values::SPRAY_SHOT_MACHINE_PISTOL();
				break;
			default:
				weapon->rifle_instance->ammo = 1;
				log("SYSERR: warning, no rifle type given for decrease_spray_shot_ammo, default to 1");
				break;
		}
	}
	/**
	 * @brief subtracts 1 from weapon ammo
	 *
	 * @param weapon
	 */
	void decrease_single_shot_ammo(obj_ptr_t& weapon) {
		/** TODO: if weapon has a bullet printer mod, calculate ammo */
		if(weapon->rifle_instance->ammo == 0){
			return;
		}
		switch((mw_rifle)weapon->rifle()->attributes->type){
			case mw_rifle::LIGHT_MACHINE_GUN:
				weapon->rifle_instance->ammo -= mods::values::SINGLE_SHOT_LIGHT_MACHINE_GUN();
				break;

			case mw_rifle::SUB_MACHINE_GUN:
				weapon->rifle_instance->ammo -= mods::values::SINGLE_SHOT_SUB_MACHINE_GUN();
				break;

			case mw_rifle::SHOTGUN:
				weapon->rifle_instance->ammo -= mods::values::SINGLE_SHOT_SHOTGUN();
				break;

			case mw_rifle::SNIPER:
				weapon->rifle_instance->ammo -= mods::values::SINGLE_SHOT_SNIPER();
				break;

			case mw_rifle::ASSAULT_RIFLE:
				weapon->rifle_instance->ammo -= mods::values::SINGLE_SHOT_ASSAULT_RIFLE();
				break;
			case mw_rifle::HANDGUN:
			case mw_rifle::PISTOL:
				weapon->rifle_instance->ammo -= mods::values::SINGLE_SHOT_HANDGUN();
				break;
			case mw_rifle::MACHINE_PISTOL:
				weapon->rifle_instance->ammo -= mods::values::SINGLE_SHOT_MACHINE_PISTOL();
				break;
			default:
				weapon->rifle_instance->ammo = 1;
				log("SYSERR: warning, no rifle type given for decrease_single_shot_ammo, default to 1");
				break;
		}
	}


	/**
	 * @brief attack using any obj_data structure that has a rifle_data_t pointer
	 *
	 * @param player
	 * @param weapon
	 * @param victim
	 * @param distance
	 */
	void rifle_attack(
			player_ptr_t& player,
			obj_ptr_t weapon,
			player_ptr_t victim,
			uint16_t distance
		){
		/** TODO: if primary is out of ammo, and player_pref.auto_switch is on, use secondary */
		if(!weapon || !weapon->has_rifle()){
			player->sendln("You aren't wielding any weapon.");
			return;
		}
		if(!player->weapon_cooldown_expired(0)){
			player->sendln("Weapon on cooldown.");
			return;
		}
		/* Check ammo */
		if(mods::weapon::has_clip(player->rifle()) && player->ammo() <= 0) {
			player->sendln("{gld}*CLICK*{/gld} Your weapon is out of ammo!");
			return;
		}

		if(!victim){
			player->sendln("You can't find your target!");
			return;
		}
		/** FIXME : grab weapon's accuracy and apply accurace modifications */

		int dam = weapon->rifle()->attributes->base_stat_list->at(distance).damage;
		int damage_dice = weapon->rifle()->attributes->damage_dice_count;
		int damage_sides = weapon->rifle()->attributes->damage_dice_sides;
		int crit_range = weapon->rifle()->attributes->critical_range;
		int crit_chance = weapon->rifle()->attributes->critical_chance;
		int critical_bonus = 0;

		/** calculate headshot */
		if(dice(1,100) >= 95){
			player->send(MSG_HEADSHOT().c_str());
			dam = victim->hp();
		}
		if(distance == crit_range){
			if(dice(1,100) <= crit_chance){
				player->send(MSG_CRITICAL().c_str());
				critical_bonus = dice(
						damage_dice,
						damage_sides
						);
			}
		}
		dam += critical_bonus;

		auto dice_roll = dice(
				damage_dice,
				damage_sides
				);
		dam += dice_roll;

#ifdef __MENTOC_SHOW_SNIPE_HIT_STATS__
		player->send(
				"dice roll[%d]\r\n"
				"damage: [%d]\r\n"
				"critical_bonus: [%d]\r\n"
				"damage_dice [%d]\r\n"
				"damage_slides [%d]\r\n"
				"crit_range [%d]\r\n"
				"crit_chance [%d]\r\n",
				dice_roll,
				dam,
				critical_bonus,
				damage_dice,
				damage_sides,
				crit_range,
				crit_chance
				);
#endif

		if(victim->position() > POS_DEAD) {
			if(victim->is_npc()){
				if(dam == 0){
					player->sendln(MSG_MISSED_TARGET());
				}else if(dam > 0){
					damage(player->cd(),victim->cd(),dam,get_legacy_attack_type(weapon));
					if(MOB_FLAGGED(victim->cd(),MOB_SENTINEL)){
						dty_debug("Mob is a sentinel. Setting sentinel_snipe_tracking on mob's behaviour tree");
						victim->cd()->mob_specials.set_behaviour_tree("sentinel_snipe_tracking");
						victim->cd()->mob_specials.snipe_tracking = player->uuid();
					}else{
						dty_debug("Mob is normal mob. Setting snipe_tracking on mob's behaviour tree");
						victim->cd()->mob_specials.set_behaviour_tree("snipe_tracking");
						victim->cd()->mob_specials.snipe_tracking = player->uuid();
					}
					if(attack_injures(weapon)){
						mods::injure::injure_player(victim);
					}
				}
				remember(victim->cd(),player->cd());
			}
		}else{
			player->sendln(MSG_TARGET_DEAD());
			stop_fighting(player->cd());
			stop_fighting(victim->cd());
		}

		decrease_single_shot_ammo(weapon);
		player->set_fight_timestamp();
		return;

	}


};

