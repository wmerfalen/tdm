#include "projectile.hpp"

extern int grenade_damage(struct char_data *ch, struct char_data *victim, int dam, int attacktype);
namespace mods {
	namespace projectile {
		room_rnum cast_finite(struct char_data* source_char,room_rnum source_room,int direction,int depth) {
			room_rnum room_id = 0;
			room_rnum previous = 0;

			for(unsigned room_depth = 0; room_depth < depth; ++room_depth) {
				room_id = resolve_room(source_room,direction,room_depth);

				if(previous == room_id) {
					return room_id;
				}

				previous = room_id;
				send_to_char(source_char,std::to_string(room_id).c_str());
				mods::globals::room_event(room_id,mods::ai_state::GRENADE_FLIES_BY);
			}

			return room_id;
		}

		room_rnum resolve_room(room_rnum source_room,int direction,int depth) {
			auto room_dir = world[source_room].dir_option[direction];//EXIT(ch,i_d);
			room_rnum room_id = 0;

			for(auto recursive_depth = depth; recursive_depth > -1; --recursive_depth) {
				if((EXIT_FLAGGED(room_dir, EX_CLOSED) || EXIT_FLAGGED(room_dir,EX_REINFORCED))
				        && !(EXIT_FLAGGED(room_dir,EX_BREACHED))) {
					break;
				}

				room_id = room_dir->to_room;
				auto next_room = room_dir->to_room;
				room_dir = world[next_room].dir_option[direction];

				if(next_room == NOWHERE || !room_dir) {
					return room_id;
				}
			}

			return room_id;
		}

		/*
		 * Alert: As of bpl14, this function returns the following codes:
		 *  < 0 Victim died.
		 *  = 0 No damage.
		 *  > 0 How much damage done.
		 */
		int grenade_damage(struct char_data *ch, struct char_data *victim, int dam, int attacktype) {
			return ::grenade_damage(ch,victim,dam,attacktype);
		}




	};
};
