#include "flee.hpp"
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

#define __MENTOC_SHOW_MODS_flee_DEBUG_OUTPUT__
#ifdef __MENTOC_SHOW_MODS_flee_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::flee::debug]")  << MSG << "\n";
#define m_error(MSG) mentoc_prefix_debug(red_str("[mods::flee::ERROR]"))  << MSG << "\n";
#else
#define m_debug(MSG) ;;
#define m_error(MSG) ;;
#endif


namespace mods::flee {
	/**
	 * Feature set fulfills:
	 * ALPHA GOALS:
	 *	- [ ] (DEXTERITY) affects successful chance of fleeing melee combat
	 */
	int get_flee_buff(player_ptr_t& player) {
		return player->dexterity() / 10;
	}

	ACMD(do_flee) {
		int i, attempt;
		if(GET_POS(ch) < POS_FIGHTING) {
			send_to_char(ch, "You are in pretty bad shape, unable to flee!");
			return;
		}

		int flee_buff = get_flee_buff(player);

		for(i = 0; i < 6; i++) {
			attempt = rand_number(0, NUM_OF_DIRS - 1);	/* Select a random direction */

			if(CAN_GO(ch, attempt) &&
			        !ROOM_FLAGGED(EXIT(ch, attempt)->to_room, ROOM_DEATH)) {
				if(rand_number(0,flee_buff) > flee_buff / 2) {
					act("$n's quick movement helps him find another way out!",1,ch,0,0,TO_ROOM);
					player->sendln("{grn}You get lucky!{/grn}");
					continue;
				}
				act("$n panics, and attempts to flee!", TRUE, ch, 0, 0, TO_ROOM);

				if(do_simple_move(ch, attempt, TRUE)) {
					send_to_char(ch, "You flee head over heels.");
				} else {
					act("$n tries to flee, but can't!", TRUE, ch, 0, 0, TO_ROOM);
				}

				return;
			}
		}

		send_to_char(ch, "PANIC!  You couldn't escape!");
	}

	void init() {
		ADD_COMMAND("flee", do_flee);

	}

#undef m_error
#undef m_debug
};
