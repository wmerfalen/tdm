#include <iostream>
#include <functional>
#include <queue>
#include <sys/socket.h>
#include <netdb.h>
#include "../conf.h"
#include "../sysdep.h"
#include "../types.hpp"
#include "../globals.hpp"

#define MOD_SNIPE_AGGRESSION 333
#define MOD_SNIPE_THRESHOLD 666
#define MOD_AGGRESSION_MODIFIER 3.33
#define MOD_AGGRESSION_CHANCE 0.33
EXTERN_ACT(do_say);
EXTERN_ACT(do_tell);
extern void perform_tell(struct char_data *ch, struct char_data *vict, char *arg);
extern size_t send_to_char(struct char_data *ch, const char *messg, ...);
extern void hit(struct char_data *ch, struct char_data *victim, int type);
namespace mods {
	uint32_t ai_state::dispatch(struct char_data* ch){
		if(m_state == WANDER){
			do_say(m_char_data,"I've got my eye on you,fucker",0,0);
		}
		return 0;
	}
	uint32_t ai_state::event(struct char_data* ch,event_type_t ev){
		switch(ev){
			case AI_EVENT_SNIPED:
				//perform_tell(m_char_data,ch,"{red}What the fuck!?{/red}");
				mpub_aggro += MOD_SNIPE_AGGRESSION;
				if(mpub_aggro >= MOD_SNIPE_THRESHOLD){
					m_state = FIGHT;
				}
				break;
			case AI_EVENT_ATTACKER_FOUND:
				break;
			default:
			break;	
		}
		return 0;
	}
};

