#include "sentinel.hpp"
#if 0
#include "sniper.hpp"
#include "medic.hpp"
#include "engineer.hpp"
#include "contagion.hpp"
#include "psyop.hpp"
#include "support.hpp"
#endif

#include "../weapon.hpp"
#include "../affects.hpp"
#include "ghost.hpp"

namespace mods::classes {
#if 0
void base::sendln(std::string_view msg){ m_player->sendln(msg); }
void base::psendln(std::string_view msg){ m_player->psendln(msg); }
#endif

		void unblock_event(uint32_t unblock_event,uuid_t player_uuid){
			auto player = ptr_by_uuid(player_uuid);
			switch(unblock_event){
				case mods::deferred::EVENT_PLAYER_FINISHES_FEIGN_DEATH:
					player->ghost()->feign_death_done();
					break;
				default:
					break;
			}
		}
};
