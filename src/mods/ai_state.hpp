#ifndef __MENTOC_MODS_ai_state_HEADER__
#define  __MENTOC_MODS_ai_state_HEADER__

#include <iostream>
#include <functional>
#include <queue>
#include <sys/socket.h>
#include <netdb.h>
#include "../conf.h"
#include "../sysdep.h"

namespace mods {
	class ai_state;
};
#include "../structs.h"
#include "../types.hpp"

typedef struct char_data char_data;
namespace mods {
    class ai_state {
		public:
			enum state { 
				WANDER, SUSPICIOUS, FIGHT,SUSPICIOUS2
			};
			enum event_type_t {
				AI_EVENT_SNIPED,AI_EVENT_ATTACKER_FOUND
			};
			ai_state() = delete;
			ai_state(char_data* ch,uint8_t st,uint32_t aggr) : m_char_data(ch), m_state(st), mpub_aggro(aggr){
				mpub_intelligence = mpub_stunned = mpub_dead = 0;
			}
			~ai_state() = default;
			/* TODO: Operator << for sending to the character */
			ai_state& operator<<(const state & m){
				m_state = m;
				return *this;
			}

			/* Spot or unspot certain mobs/player */
			uint32_t dispatch(struct char_data*);
			uint32_t event(struct char_data*,event_type_t);
			
			/* Affects how fast it goes from wander to fight */
			uint32_t mpub_aggro;

			/* Affects if WANDER goes directly to FIGHT. Lower intelligence
			means more likely to stay in SUSPICIOUS state than to go to
			fight */
			uint32_t mpub_intelligence;

			/* Boolean values */
			uint32_t mpub_stunned;
			uint32_t mpub_dead;
		protected:
			struct char_data* m_char_data;
			uint8_t m_state;
    };
};

#endif
