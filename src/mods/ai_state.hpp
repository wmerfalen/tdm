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
#define MOD_LENIENT_MAX 666 
#define MOD_SUSPICIOUS_MAX 666

typedef struct char_data char_data;
namespace mods {
    class ai_state {
		public:
			enum state { 
				WANDER, SUSPICIOUS, FIGHT,SUSPICIOUS2,PROTECT,AGGRESSIVE_PROTECT,
				MANIC,CHAOTIC,RESERVED
			};
			enum event_type_t {
				AI_EVENT_SNIPED,AI_EVENT_ATTACKER_FOUND,AI_WITNESS_ATTACK,AI_EVENT_WANDER,
				BREACHED_NORTH,BREACHED_SOUTH,BREACHED_EAST,BREACHED_WEST,
				BREACHED_FROM_NORTH,BREACHED_FROM_SOUTH,BREACHED_FROM_EAST,BREACHED_FROM_WEST,
				GRENADE_FLIES_BY,GRENADE_EXPLOSION,
				ROOM_EMP,ROOM_NORMAL
			};
			ai_state() = delete;
			ai_state(char_data* ch,uint8_t st,uint32_t aggr) : m_char_data(ch), m_state(st), mpub_aggro(aggr){
				mpub_lenient = mpub_suspicious = mpub_intelligence = mpub_stunned = mpub_dead = 0;
				mpub_lenient = 333;
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
			int32_t mpub_aggro;
			int32_t mpub_lenient;
			int32_t mpub_suspicious;

			/* Affects if WANDER goes directly to FIGHT. Lower intelligence
			means more likely to stay in SUSPICIOUS state than to go to
			fight */
			/* A lot of these values were chosen as uint32_t because we didn't have a better type
			to choose from*/
			int32_t mpub_intelligence;

			/* Boolean values */
			uint32_t mpub_stunned;
			uint32_t mpub_dead;
		protected:
			struct char_data* m_char_data;
			uint8_t m_state;
    };
};

#endif
