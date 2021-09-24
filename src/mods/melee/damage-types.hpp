#ifndef __MENTOC_MODS_MELEE_DAMAGE_TYPES_HEADER__
#define __MENTOC_MODS_MELEE_DAMAGE_TYPES_HEADER__

#include "../item-types.hpp"
#include "../player.hpp"
#include "../weapon.hpp"
#include "../pqxx-types.hpp"
#include "../../globals.hpp"
#include "../scan.hpp"
#include "../damage-event.hpp"
#include "../forge-engine/elemental-enum.hpp"

namespace mods::melee::damage_types {
	using vpd = mods::scan::vec_player_data;
	feedback_t cqc_damage_with_feedback(
	    player_ptr_t& player,
	    player_ptr_t victim,
	    uint16_t damage
	);
};
#endif

