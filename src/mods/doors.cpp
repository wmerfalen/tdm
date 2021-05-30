#include "doors.hpp"

namespace mods::doors  {
	bool sane_dir(const room_rnum& room_id,const direction_t& dir) {
		if(room_id >= world.size()) {
			return false;
		}
		auto d = world[room_id].dir_option[dir];
		if(!d) {
			return false;
		}
		return true;
	}
	void unlock(room_rnum room_id,direction_t dir) {
		if(!sane_dir(room_id,dir)) {
			return;
		}
		world[room_id].dir_option[dir]->exit_info &= ~(EX_LOCKED);
	}
	void open(room_rnum room_id,direction_t dir) {
		if(!sane_dir(room_id,dir)) {
			return;
		}
		world[room_id].dir_option[dir]->exit_info &= ~(EX_CLOSED);
	}
};
