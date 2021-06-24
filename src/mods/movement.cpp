#include "player.hpp"
#include "demolitions.hpp"
#include "projectile.hpp"
#include "object-utils.hpp"
#include "rooms.hpp"

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
	bool force_move_to(player_ptr_t& player,const room_rnum& target_room) {
		if(target_room < world.size()) {
			std::string msg = "You leave the room";
			auto old_room = player->room();
			for(const auto& dir : world[old_room].directions()) {
				if(world[old_room].dir_option[dir]->to_room == target_room) {
					msg = CAT("You go ",dirstr(dir));
					break;
				}
			}
			mods::globals::rooms::char_from_room(player->cd());
			player->moving_to_room() = true;
			player->set_room(target_room);
			mods::globals::room_list[target_room].push_back(player);

			player->sendln(msg);
			return true;
		}
		return false;
	}
	std::size_t force_room_to(const room_rnum& existing_room,const room_rnum& target_room) {
		auto room = room_list(existing_room);
		auto i = 0;
		for(auto& player : room) {
			i += mods::movement::force_move_to(player,target_room);
		}
		return i;
	}
	std::size_t banish_except(const room_rnum& source,const room_rnum& destination,std::vector<uuid_t>& player_uuids) {
		auto room = room_list(source);
		auto i = 0;
		for(auto& player : room) {
			if(std::find(player_uuids.begin(),player_uuids.end(),player->uuid()) == player_uuids.end()) {
				i += mods::movement::force_move_to(player,destination);
			}
		}
		return i;
	}
};
