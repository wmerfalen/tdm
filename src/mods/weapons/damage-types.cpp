#include "damage-types.hpp"
#include "../../spells.h"
#define dty_debug(a) std::cerr << "[mods::weapons::damage_types][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
namespace mods::weapons::damage_types {
	using vpd = mods::scan::vec_player_data;
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
	 * @brief if you know your target's name and direction, then use this to initiate the attack
	 *
	 * @param player
	 * @param victim_name
	 * @param direction
	 */
	void rifle_attack_by_name(player_ptr_t& player,std::string_view victim_name,int direction){
		auto weapon = player->primary();
		if(!weapon){
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

	void rifle_attack(
			player_ptr_t& player,
			obj_ptr_t weapon,
			player_ptr_t victim,
			uint16_t distance
		){
		/** TODO: if primary is out of ammo, and player_pref.auto_switch is on, use secondary */
		if(!weapon){
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
			player->send("{red}***HEADSHOT***{/red} -- ");
			dam = victim->hp();
		}
		if(distance == crit_range){
			if(dice(1,100) <= crit_chance){
				player->send("{red}***CRITICAL***{/red} -- ");
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
			player->set_fight_timestamp();
			if(victim->is_npc()){
				//if(victim->is_dead()){
				//	victim->cd()->mob_specials.behaviour_tree = 0;
				//	return;// -1;
				//}else
				if(dam == 0){
					player->sendln("You missed your target!");
				}else if(dam > 0){
					damage(player->cd(),victim->cd(),dam,get_legacy_attack_type(weapon));
					if(MOB_FLAGGED(victim->cd(),MOB_SENTINEL)){
						dty_debug("Mob is a sentine. Setting sentinel_snipe_tracking on mob's behaviour tree");
						victim->cd()->mob_specials.set_behaviour_tree("sentinel_snipe_tracking");
						victim->cd()->mob_specials.snipe_tracking = player->uuid();
					}else{
						dty_debug("Mob is normal mob. Setting snipe_tracking on mob's behaviour tree");
						victim->cd()->mob_specials.set_behaviour_tree("snipe_tracking");
						victim->cd()->mob_specials.snipe_tracking = player->uuid();
					}
				}
				remember(victim->cd(),player->cd());
			}
		}else{
			player->stc("It appears that your target is dead\r\n");
			stop_fighting(player->cd());
			stop_fighting(victim->cd());
		}

		/** FIXME: remove however many ammunition was used */
		player->sendln("TODO: ammo adjustment");
		//TODO: player->ammo_adjustment(-1);
		player->sendln("TODO: weapon cooldown");
		//TODO: player->weapon_cooldown_start(3,0);
		return;// (dmg_dealt);

	}


};

