#include "projectile.hpp"

namespace mods {
	namespace projectile {
		/**
		 * FIXME: throwing nades twice into a single room north shows the fly-by message twice */
		/** FIXME: needs blast radius stuff to happen */
		/** FIXME: needs shrapnel calculation */
		std::string todirstr(const char* direction,bool prefix,bool suffix) {
			std::string pre = prefix ? " " : "";
			std::string suf = suffix ? " " : "";
			if(strncmp(direction,"north",5) == 0 || direction[0] == 'n' || direction[0] == 'N') {
				return pre + "to the north" + suf;
			}

			if(strncmp(direction,"south",5) == 0 || direction[0] == 's' || direction[0] == 'S') {
				return pre + "to the south" + suf;
			}

			if(strncmp(direction,"east",4) == 0 || direction[0] == 'e' || direction[0] == 'E') {
				return pre + "to the east" + suf;
			}

			if(strncmp(direction,"west",4) == 0 || direction[0] == 'w' || direction[0] == 'W') {
				return pre + "to the west" + suf;
			}

			if(strncmp(direction,"up",2) == 0 || direction[0] == 'u' || direction[0] == 'U') {
				return pre + "above you" + suf;
			}

			if(strncmp(direction,"down",4) == 0 || direction[0] == 'd' || direction[0] == 'D') {
				return pre + "below you" + suf;
			}
			return pre + "??" + suf;
		}

		std::string fromdirstr(int direction,bool prefix, bool suffix) {
			std::string f = std::string(prefix ? " " : "") + "from ";
			switch(direction){
				case NORTH:
					return f + "the north" + (suffix ? " " : "");
				case SOUTH:
					return f + "the south" + (suffix ? " " : "");
				case EAST:
					return f + "the east" + (suffix ? " " : "");
				case WEST:
					return f + "the west" + (suffix ? " " : "");
				case UP:
					return f + "above" + (suffix ? " " : "");
				case DOWN:
					return f + "below" + (suffix ? " " : "");
			}
			return f + "??" + (suffix ? " " : "");
		}

		void projectile_lands(room_rnum room_id,obj_data* object,int from_direction) {
			for(auto & player : mods::globals::room_list[room_id]) {
				player->stc(std::string("A ") + object->name + " tumbles to the ground" + fromdirstr(from_direction,1,0) + "! Take cover!");
			}
		}

		void projectile_flies_by(room_rnum room, obj_data* object, int from_direction, int to_direction) {
			for(auto & player : mods::globals::room_list[room]) {
				player->stc(std::string("A ") + object->name + " flies by" + fromdirstr(from_direction,1,0) + ", narrowly missing you!");
			}
		}

		void travel_to(room_rnum from, int direction, std::size_t depth, obj_data* object){
			room_rnum room_id = 0;
			room_rnum previous = 0;

			auto opposite = OPPOSITE_DIR(direction);
			for(unsigned room_depth = 1; room_depth < depth; ++room_depth) {
				room_id = resolve_room(from,direction,room_depth);

				if(previous == room_id) {
					return;
				}

				previous = room_id;
				if (room_depth + 1 == depth) {
					projectile_lands(room_id,object,opposite);
					return;
				}
				projectile_flies_by(room_id,object,opposite,direction);
			}

			return;
		}

		room_rnum cast_finite(struct char_data* source_char,room_rnum source_room,int direction,std::size_t depth,obj_data* object) {
			room_rnum room_id = 0;
			room_rnum previous = 0;

			auto opposite = OPPOSITE_DIR(direction);
			for(std::size_t room_depth = 0; room_depth < depth; ++room_depth) {
				room_id = resolve_room(source_room,direction,room_depth);
				projectile_flies_by(room_id,object,opposite,direction);

				if(previous == room_id) {
					return room_id;
				}

				previous = room_id;
				//mods::globals::room_event(room_id,mods::ai_state::GRENADE_FLIES_BY);
			}

			return room_id;
		}

		room_rnum resolve_room(room_rnum source_room,int direction,std::size_t depth) {
			auto room_dir = world[source_room].dir_option[direction];
			room_rnum room_id = source_room;

			for(std::size_t recursive_depth =0; recursive_depth <= depth; ++recursive_depth) {
				if(!room_dir) {
					std::cerr << "invalid room_dir, returning: " << room_id << "\n";
					return room_id;
				}
				if((EXIT_FLAGGED(room_dir, EX_CLOSED) || EXIT_FLAGGED(room_dir,EX_REINFORCED))
				        && !(EXIT_FLAGGED(room_dir,EX_BREACHED))) {
					std::cerr << "breaking on recursive_depth: " << recursive_depth << "\n";
					break;
				}

				room_id = room_dir->to_room;

				if(room_id == NOWHERE || !room_dir) {
					std::cerr << "nowhere or !room_dir returning room_id: " << room_id << "\n";
					return room_id;
				}
				room_dir = world[room_id].dir_option[direction];
				std::cerr << "end for, looping\n";
			}

			std::cerr << "returning room_id [final]: " << room_id << "\n";
			return room_id;
		}

		/*
		 * Alert: As of bpl14, this function returns the following codes:
		 *  < 0 Victim died.
		 *  = 0 No damage.
		 *  > 0 How much damage done.
		 */
		int grenade_damage(player_ptr_t victim, obj_data* projectile) {
			victim->stc(std::string("You are hit by a ") + projectile->explosive()->name);
			return 0;
		}




	};
};
