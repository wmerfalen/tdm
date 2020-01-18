#include <iostream>
#include <functional>
#include <queue>
#include <sys/socket.h>
#include <netdb.h>
#include <chrono>
#include <sys/time.h>
#include "../conf.h"
#include "../sysdep.h"
#include "../types.hpp"
#include "../globals.hpp"
#include "../utils.h"

#define MOD_SNIPE_AGGRESSION 333
#define MOD_SNIPE_THRESHOLD 666
#define MOD_AGGRESSION_MODIFIER 3.33
#define MOD_AGGRESSION_CHANCE 0.33
#define MOD_WANDER_SAY_DIVISOR 6
#define MOD_WANDER_SAY_TOP 20
#define NO_ATTACK_TYPE -1
EXTERN_ACT(do_say);
EXTERN_ACT(do_tell);
extern void perform_tell(char_data *ch, char_data *vict, char *arg);
extern size_t send_to_char(char_data *ch, const char *messg, ...);
extern void hit(char_data *ch, char_data *victim, int type);
namespace mods {
	void seed() {
		/** deprecated */
	}
	uint32_t ai_state::dispatch(char_data* ch) {
		return 0;
	}
	uint32_t ai_state::event(char_data* ch,event_type_t ev) {
		static int divisor = 6;

		switch(ev) {
			case AI_EVENT_SNIPED:
				break;

			case AI_EVENT_ATTACKER_FOUND:
				break;

			default:
			case AI_WITNESS_ATTACK:
				break;
		}

		if(divisor == 12) {
			divisor = 3;
		}

		return 0;
	}
};

