#ifndef __MENTOC_MODS_MOVEMENT_HEADER__
#define __MENTOC_MODS_MOVEMENT_HEADER__
#include "player.hpp"

namespace mods::movement {
	bool char_move_to(player_ptr_t& player,const room_rnum& room);
	bool force_move_to(player_ptr_t& player,const room_rnum& target_room);
	std::size_t force_room_to(const room_rnum& source,const room_rnum& destination);
	std::size_t banish_except(const room_rnum& source,const room_rnum& destination,std::vector<uuid_t>& player_uuids);
	//bool necromancer_move_to(player_ptr_t& player,const room_rnum& room, uint8_t necrowalk_skill_level);
};

#endif
