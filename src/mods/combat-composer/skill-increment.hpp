#include "snipe-target.hpp"
#include "../rifle.hpp"
#include "../scan.hpp"
#include "../player.hpp"
#include "../ranged-combat-totals.hpp"
#include "../damage-event.hpp"
#include "../rooms.hpp"
#include "../object-utils.hpp"
#include "../player-utils.hpp"
#include "../calc-visibility.hpp"
#include "../classes/ghost.hpp"
#include "../classes/includes.hpp"
#include "../levels.hpp"

#include "../../fight.hpp"
#include "../mobs/damage-event.hpp"
#include "../weapons/elemental.hpp"
#include "../weapons/legacy-combat.hpp"
#include "../interpreter.hpp"

namespace mods::combat_composer::skill_increment {
	static inline void increment(player_ptr_t& attacker,obj_ptr_t& weapon) {
		if(weapon->has_rifle()) {
			switch(weapon->rifle()->attributes->type) {
				case mw_rifle::SNIPER:
					mods::classes::increment_technique(attacker,"wpn_sniper_rifles");
					break;
				case mw_rifle::ASSAULT_RIFLE:
					mods::classes::increment_technique(attacker,"wpn_assault_rifles");
					break;
				case mw_rifle::SHOTGUN:
					mods::classes::increment_technique(attacker,"wpn_shotguns");
					break;
				case mw_rifle::PISTOL:
					mods::classes::increment_technique(attacker,"wpn_pistols");
					break;
				case mw_rifle::MACHINE_PISTOL:
					mods::classes::increment_technique(attacker,"wpn_machine_pistols");
					break;
				case mw_rifle::LIGHT_MACHINE_GUN:
					mods::classes::increment_technique(attacker,"wpn_light_machine_guns");
					break;
				case mw_rifle::SUB_MACHINE_GUN:
					mods::classes::increment_technique(attacker,"wpn_sub_machine_guns");
					break;
				default:
					log("Invalid rifle type. not incrementing for attacker.");
					break;
			}
		}
	}
};//end namespace mods::combat_composer::skill_increment
