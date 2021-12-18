#include "reload.hpp"
#include "../interpreter-include.hpp"
#include "damage-calculator.hpp"
#include "../query-objects.hpp"
#include "../object-utils.hpp"
#include "../orm/inventory.hpp"

#define m_debug(a) std::cerr << "[mods::weapons::reload][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#define m_error(a) std::cerr << "[mods::weapons::reload][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#ifndef TO_ROOM
#define TO_ROOM		1
#endif
namespace mods::weapons::reload {
	static std::vector<std::pair<mw_rifle,std::string>> ammo_map = {
		{mw_rifle::ASSAULT_RIFLE,"sg3-ar-ammunition.yml"},
		{mw_rifle::ASSAULT_RIFLE,"opsix-incendiary-ar-clip.yml"},
		{mw_rifle::LIGHT_MACHINE_GUN,"sg3-lmg-ammunition.yml"},
		{mw_rifle::MACHINE_PISTOL,"sg3-mp-ammunition.yml"},
		{mw_rifle::MACHINE_PISTOL,"sg3-pistol-ammunition.yml"},
		{mw_rifle::PISTOL,"sg3-pistol-ammunition.yml"},
		{mw_rifle::SHOTGUN,"sg3-shotgun-ammunition.yml"},
		{mw_rifle::SUB_MACHINE_GUN,"sg3-smg-ammunition.yml"},
		{mw_rifle::SNIPER,"sg3-sniper-ammunition.yml"},
	};
	std::pair<obj_ptr_t,obj_ptr_t> reload_weapon(player_ptr_t& player,obj_ptr_t weapon) {
		m_debug("reloading weapon...");
		uint16_t ticks = 6;
		switch((mw_rifle)weapon->rifle()->type) {
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
		obj_ptr_t obj = nullptr;
		for(const auto& pair : ammo_map) {
			if(pair.first == weapon->rifle()->type) {
				auto yaml_file = pair.second;
				m_debug("looking for yaml file in players inventory: '" << yaml_file << "'");
				std::vector<uuid_t> items = mods::query_objects::query_inventory_by_yaml(player, yaml_file);
				for(const auto& item_uuid : items) {
					obj = optr_by_uuid(item_uuid);
					if(!obj) {
						continue;
					}
					m_debug("Found: " << obj->name.c_str());
					break;
				}
			}
			if(obj) {
				break;
			}
		}
		if(!obj) {
			player->send("You don't seem to have any ammunition for that weapon anywhere in your inventory!");
			return {nullptr,nullptr};
		}

		auto clip_size = weapon->rifle()->attributes->clip_size;
		auto& capacity = obj->consumable()->attributes->capacity;
		if(capacity < clip_size) {
			clip_size = capacity;
		}

		weapon->rifle_instance->ammo = clip_size;
		player->send("You reload your %s with %d ammo.", weapon->name.c_str(), clip_size);
		if(player->marine() && mods::object_utils::is_assault_rifle(weapon)) {
			clip_size -= (clip_size * MARINE_AR_PASSIVE_EXTRA_AMMO_BONUS() * 0.01);
		}
		if((obj->consumable()->attributes->capacity - clip_size) < 0) {
			obj->consumable()->attributes->capacity = 0;
		} else {
			obj->consumable()->attributes->capacity -= clip_size;
		}
		player->uncarry(obj);
		if(obj->consumable()->attributes->capacity > 0) {
			/** Since we don't store the player_object.id field, this is a nasty little
			 * hack that gets it done with a bit more overhead. The uncarry will remove
			 * it from the inventory, but recarrying it will flush it to the db
			 * properly. This is a minor inconvenience when in reality, it's probably
			 * better to keep track of the player_object.id primary key.
			 * so that we simply run an update query.
			 * This is the most bug-free way to update that item in the inventory.
			 */
			player->carry(obj);
		}
		return {weapon,obj};
	}
	void reload_primary(player_ptr_t& player) {
		auto pair = reload_weapon(player,player->primary());
		auto weapon = pair.first;
		auto ammo = pair.second;
		{
			std::tuple<bool,std::string> res = mods::orm::inventory::user_reloaded_primary(player);
			if(!std::get<0>(res)) {
				m_error("WARNING. Unable to flush DB changes for primary weapon reload! :'" << std::get<1>(res));
			} else {
				m_debug("Saved primary weapon reload data okay.");
			}
		}
	}
	void reload_secondary(player_ptr_t& player) {
		auto pair = reload_weapon(player,player->secondary());
		auto weapon = pair.first;
		auto ammo = pair.second;
		{
			std::tuple<bool,std::string> res = mods::orm::inventory::user_reloaded_secondary(player);
			if(!std::get<0>(res)) {
				m_error("WARNING. Unable to flush DB changes for secondary weapon reload! :'" << std::get<1>(res));
			} else {
				m_debug("Saved secondary weapon reload data okay.");
			}
		}
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
		if(secondary) {
			reload_secondary(player);
		} else {
			reload_primary(player);
		}
	}
	void init() {
		mods::interpreter::add_command("reload", POS_RESTING, do_reload, 0,0);
	}
};
#undef m_debug
#undef m_error
