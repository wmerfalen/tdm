#include "reload.hpp"
#include "../interpreter-include.hpp"
#include "damage-calculator.hpp"

#define dty_debug(a) std::cerr << "[mods::weapons::damage_types][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#ifndef TO_ROOM
#define TO_ROOM		1
#endif
namespace mods::weapons::reload {
	void reload_weapon(player_ptr_t& player,obj_ptr_t weapon) {
		uint16_t ticks = 6;
		switch((mw_rifle)weapon->rifle()->attributes->type) {
			case mw_rifle::LIGHT_MACHINE_GUN:
				ticks = mods::values::RELOAD_TICKS_LIGHT_MACHINE_GUN();
				break;

			case mw_rifle::SUB_MACHINE_GUN:
				ticks = mods::values::RELOAD_TICKS_SUB_MACHINE_GUN();
				break;

			case mw_rifle::SHOTGUN:
				ticks = mods::values::RELOAD_TICKS_SHOTGUN();
				break;

			case mw_rifle::SNIPER:
				ticks = mods::values::RELOAD_TICKS_SNIPER();
				break;

			case mw_rifle::ASSAULT_RIFLE:
				ticks = mods::values::RELOAD_TICKS_ASSAULT_RIFLE();
				break;
			case mw_rifle::HANDGUN:
			case mw_rifle::PISTOL:
				ticks = mods::values::RELOAD_TICKS_HANDGUN();
				break;
			case mw_rifle::MACHINE_PISTOL:
				ticks = mods::values::RELOAD_TICKS_MACHINE_PISTOL();
				break;
			default:
				log("SYSERR: warning, no rifle type given for reload. default to %d", ticks);
				break;
		}
		/** TODO: honor the ticks and block the player for the specified time */
		auto obj = player->get_ammo_for(weapon);
		if(!obj) {
			player->send("You don't seem to have any ammunition for that weapon anywhere in your inventory!");
			return;
		}
		auto ammo_amount = mods::weapons::damage_calculator::ammunition_amount(player,weapon);
		/** TODO: buff to clip size using attachments */
		if(ammo_amount < weapon->rifle()->attributes->clip_size) {
			weapon->rifle_instance->ammo = ammo_amount;
			player->send("You partially reload your %s with %d ammo.", weapon->name.c_str(), ammo_amount);
			player->consume_from_carrying(obj);
			return;
		}
		if(!player->carrying_ammo_of_type(weapon->rifle()->attributes->type)) {
			player->sendln("You don't have any ammo for that weapon!");
			return;
		}
		weapon->rifle_instance->ammo = weapon->rifle()->attributes->clip_size;
		player->send("You reload your %s with %d ammo.", weapon->name.c_str(), ammo_amount);
		player->consume_from_carrying(obj);

	}
	/* TODO: Implement weapon tags in the obj_data data structure */
	ACMD(do_reload) {
		auto vec_args = PARSE_ARGS();
		bool primary = true, secondary = false;
		if(vec_args.size() > 0) {
			if(mods::util::is_lower_match(vec_args[0], "primary")) {
				primary = true;
				secondary = false;
			}
			if(mods::util::is_lower_match(vec_args[0], "secondary")) {
				secondary = true;
				primary = false;
			}
		}

		auto weapon = player->primary();
		if(secondary) {
			weapon = player->secondary();
		}

		if(!weapon || !weapon->has_rifle()) {
			player->send("You aren't wielding a reloadable weapon in your [%s] slot.\r\n", primary ? "primary" : "secondary");
			return;
		}
		reload_weapon(player,weapon);
	}
	void init() {
		mods::interpreter::add_command("reload", POS_RESTING, do_reload, 0,0);
	}
};
