#include "suicide.hpp"
#include "affects.hpp"
#include "skills.hpp"
#include "armor.hpp"

#include "rooms.hpp"
#include "injure.hpp"
#include "doors.hpp"
#include "super-users.hpp"
#include "interpreter.hpp"
#include "weapons/damage-types.hpp"
#include "rand.hpp"
#include "weapons/damage-calculator.hpp"
#include "weapons/elemental.hpp"

#define __MENTOC_SHOW_MODS_suicide_DEBUG_OUTPUT__
#ifdef __MENTOC_SHOW_MODS_suicide_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::suicide::debug]")  << MSG << "\n";
#define m_error(MSG) mentoc_prefix_debug(red_str("[mods::suicide::ERROR]"))  << MSG << "\n";
#else
#define m_debug(MSG) ;;
#define m_error(MSG) ;;
#endif


namespace mods::suicide {

	ACMD(do_suicide) {
		player->sendln("You put a gun to your temple and pull the trigger... {red}*BLAM!!!*{/red}");
		act("$n points a gun at $s temple and PULLS THE TRIGGER... {red}*BLAM!!!*{/red}!", TRUE, ch, 0, 0, TO_ROOM);
		player->hp() = 0;
		mods::weapons::damage_types::legacy::die(player->cd());
	}

	void init() {
		ADD_COMMAND("suicide", do_suicide);

	}

#undef m_error
#undef m_debug
};
