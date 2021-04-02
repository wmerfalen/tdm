#include "player.hpp"
#include "demolitions.hpp"
#include "projectile.hpp"
#include "object-utils.hpp"

namespace mods::movement {
	bool char_move_to(player_ptr_t& player,const room_rnum& room) {
		player->moving_to_room() = true;
		bool move = true;
		auto& current = player->room();
		auto list = mods::demolitions::claymores_in(room);
		/** current room */
		int8_t exit = -1;
		int8_t heading = -1;
		for(const auto& dir : world[current].directions()) {
			if(world[current].dir_option[dir] && world[current].dir_option[dir]->to_room == room) {
				exit = OPPOSITE_DIR(dir);
				heading = dir;
				break;
			}
		}
		/** check the current room. if the exit the user is trying has
		 * a claymore. blow it up
		 */
		for(const auto& obj_uuid : mods::demolitions::claymores_in(current)) {
			auto obj = optr_by_uuid(obj_uuid);
			if(obj->get_owner() != player->uuid() && mods::object_utils::claymore_installed_at(obj) == heading) {
				mods::projectile::explode(current,obj->uuid,player->uuid());
				player->moving_to_room() = false;
				return false;
			}
		}
		for(const auto& obj_uuid : list) {
			auto obj = optr_by_uuid(obj_uuid);
			if(obj->get_owner() != player->uuid() && mods::object_utils::claymore_installed_at(obj) == exit) {
				mods::projectile::explode(room,obj->uuid,player->uuid());
				move = false;
			}
		}
		player->moving_to_room() = move;
		return move;
	}
};
