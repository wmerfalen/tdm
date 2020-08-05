#ifndef __MENTOC_MODS_DOORS_HEADER__
#define __MENTOC_MODS_DOORS_HEADER__

#include "../globals.hpp"
#include "loops.hpp"
#include "object-utils.hpp"

extern obj_ptr_t read_object_ptr(obj_vnum nr, int type);
namespace mods::doors  {
	enum exit_info_masks_t {
		EIM_ISDOOR = EX_ISDOOR,
		EIM_CLOSED = EX_CLOSED,
		EIM_LOCKED = EX_LOCKED,
		EIM_PICKPROOF = EX_PICKPROOF,
		EIM_REINFORCED = EX_REINFORCED,
		EIM_BREACHED = EX_BREACHED,
		EIM_BREACHABLE = EX_BREACHABLE,
		EIM_QUEST_LOCKED = EX_QUEST_LOCKED,
		EIM_HIDDEN = EX_HIDDEN,
		EIM_ELECTRIFIED = EX_ELECTRIFIED,
		__LAST = EIM_ELECTRIFIED << 1
	};
	static constexpr int BREACHABLE_DOOR = exit_info_masks_t::EIM_ISDOOR + exit_info_masks_t::EIM_BREACHABLE;

	static inline std::string directional_dirstr(const int & direction){
		switch(direction){
			case NORTH:
				return "northern";
			case SOUTH:
				return "southern";
			case EAST:
				return "eastern";
			case WEST:
				return "western";
			case UP:
				return "above";
			case DOWN:
				return "below";
		}
		return "??";
	}

	static inline std::string dirstr(const int & direction){
		switch(direction){
			case NORTH:
				return "north";
			case SOUTH:
				return "south";
			case EAST:
				return "east";
			case WEST:
				return "west";
			case UP:
				return "above";
			case DOWN:
				return "below";
		}
		return "??";
	}

	static inline std::string to_dirstr(const int& direction){
		switch(direction){
			case NORTH:
				return "to the {gld}north{/gld}";
			case SOUTH:
				return "to the {gld}south{/gld}";
			case EAST:
				return "to the {gld}east{/gld}";
			case WEST:
				return "to the {gld}west{/gld}";
			case UP:
				return "{gld}above you{/gld}";
			case DOWN:
				return "{gld}below you{/gld}";
		}
		return "??";
	}

	static inline std::string from_dirstr(const int& direction){
		switch(direction){
			case NORTH:
				return "from the {gld}north{/gld}";
			case SOUTH:
				return "from the {gld}south{/gld}";
			case EAST:
				return "from the {gld}east{/gld}";
			case WEST:
				return "from the {gld}west{/gld}";
			case UP:
				return "from {gld}above you{/gld}";
			case DOWN:
				return "from {gld}below you{/gld}";
		}
		return "??";
	}
	static inline void dispose_breach(uuid_t obj_uuid){
		mods::globals::dispose_object(obj_uuid);
	}
	static inline void knockback_room(const room_rnum room,int door){
		send_to_room(room,"\r\n{gld}*** BOOM!!! ***{/gld}\r\nYou are knocked back by a breach charge explosion %s!\r\n", from_dirstr(door).c_str());
		auto opposite_room = world[room].dir_option[door]->to_room;
		send_to_room(opposite_room,"You are knocked back by a breach charge explosion %s!\r\n\r\n", from_dirstr(OPPOSITE_DIR(door)).c_str());
	}
	static inline void breached(const room_rnum room,int direction){
		send_to_room(room, "\r\n{gld}*** BOOM!!! ***{/gld}\r\nThe door %s explodes from a breach charge explosion!\r\n", from_dirstr(direction).c_str());
	}
	static inline void thermite_breached(const room_rnum room,int direction){
		send_to_room(room,"\r\n{gld}*** BOOM!!! ***{/gld}\r\nThe {red}REINFORCED{/red} door %s gets destroyed by a {gld}THERMITE{/gld} breach charge!", from_dirstr(direction).c_str());
	}
	static inline void warn_thermite_room(const room_rnum room,int direction){
		send_to_room(room,"\r\n{gld}*** HISSSSS ***{/gld}\r\nThe {red}REINFORCED{/red} door %s starts to melt in a square pattern as a {gld}THERMITE{/gld} breach charge cuts through it...\r\n", from_dirstr(direction).c_str());
		send_to_room(world[room].dir_option[direction]->to_room,"\r\n{gld}*** HISSSSS ***{/gld}\r\nThe {red}REINFORCED{/red} door %s starts to melt in a square pattern as a {gld}THERMITE{/gld} breach charge cuts through it...\r\n", from_dirstr(OPPOSITE_DIR(direction)).c_str());
	}

	static inline void pave_open_direction(room_rnum room, int direction){
		world[room].dir_option[direction]->exit_info = EX_ISDOOR | EX_BREACHED;
		world[world[room].dir_option[direction]->to_room].dir_option[OPPOSITE_DIR(direction)]->exit_info = EX_ISDOOR | EX_BREACHED;
	}

	static inline void door_explosion_event(uuid_t player_uuid,uuid_t obj_uuid,int direction){
				auto player = ptr_by_uuid(player_uuid);
				auto room = player->room();
				switch(mods::object_utils::get_explosive_type(obj_uuid)){
					case mw_explosive::THERMITE_CHARGE:
						mods::doors::thermite_breached(room,direction);
						break;
					case mw_explosive::BREACH_CHARGE:
						mods::doors::breached(room,direction);
						break;
					default:
						std::cerr << "[WARNING] unhandled type! " << __FILE__ << ":" << __LINE__ << "\n";
						break;
				}
				mods::doors::dispose_breach(obj_uuid);
				mods::doors::pave_open_direction(room,direction);
				mods::doors::knockback_room(room,direction);
	}
	static inline void perform_breach(uuid_t obj_uuid,uuid_t player_uuid,int direction){
		auto holding = optr_by_uuid(obj_uuid);
		auto player = ptr_by_uuid(player_uuid);
		auto dir_str = directional_dirstr(direction).c_str();
		auto room = player->room();
		player->send("You carefully place a %s on the %s door...\r\n",holding->name.c_str(),dir_str);
		send_to_room_except(room,{player},"%s carefully places a %s on the %s door...",player->name().c_str(),holding->name.c_str(),dir_str);
		player->unequip(WEAR_HOLD);
		mods::object_utils::set_is_breaching(holding,player,direction);
		player->block_for(mods::object_utils::BREACH_TICKS_DURATION, mods::deferred::EVENT_PLAYER_UNBLOCK_BREACH, obj_uuid);
		mods::globals::defer_queue->push(mods::object_utils::BREACH_TICKS_DURATION / 2,[obj_uuid,player_uuid]() {
			auto holding = optr_by_uuid(obj_uuid);
			auto player = ptr_by_uuid(player_uuid);
			auto room = player->room();
			player->send("\r\n{gld}*** CLICK ***{/gld}\r\nYou activate the trigger on the %s...\r\n\r\n", holding->name.c_str());
			send_to_room_except(room,{player},"\r\n{gld}*** CLICK ***{/gld}\r\n%s activates the trigger on %s...\r\n",player->name().c_str(),holding->name.c_str());
		});

		mods::globals::defer_queue->push(mods::object_utils::BREACH_TICKS_DURATION,[direction,obj_uuid,player_uuid]() {
				mods::doors::door_explosion_event(player_uuid,obj_uuid,direction);
		});
	}

	static inline void perform_thermite_breach(uuid_t obj_uuid,uuid_t player_uuid,int direction){
		auto holding = optr_by_uuid(obj_uuid);
		auto player = ptr_by_uuid(player_uuid);
		auto room = player->room();
		auto dir_str = directional_dirstr(direction).c_str();
		player->send("You carefully place a %s on the %s door...\r\n",holding->name.c_str(),dir_str);
		player->unequip(WEAR_HOLD);
		mods::object_utils::set_is_breaching(holding,player,direction);
		send_to_room_except(room,{player},"%s carefully places a %s on the %s door...",player->name().c_str(),holding->name.c_str(),dir_str);
		player->block_for(mods::object_utils::THERMITE_BREACH_TICKS_DURATION, mods::deferred::EVENT_PLAYER_UNBLOCK_BREACH, obj_uuid);
		mods::globals::defer_queue->push(mods::object_utils::THERMITE_BREACH_TICKS_DURATION / 3,[obj_uuid,player_uuid,direction]() {
			auto holding = optr_by_uuid(obj_uuid);
			auto player = ptr_by_uuid(player_uuid);
			auto room = player->room();
			player->send("\r\n{gld}*** CLICK ***{/gld}\r\nYou activate the trigger on the %s...\r\n", holding->name.c_str());
			send_to_room_except(room,{player},"\r\n{gld}*** CLICK ***{/gld}\r\n%s activates the trigger on %s...\r\n",player->name().c_str(),holding->name.c_str());
		});
		mods::globals::defer_queue->push(mods::object_utils::THERMITE_BREACH_TICKS_DURATION / 2,[player_uuid,direction]() {
			auto player = ptr_by_uuid(player_uuid);
			warn_thermite_room(player->room(),direction);
		});

		mods::globals::defer_queue->push(mods::object_utils::THERMITE_BREACH_TICKS_DURATION,[obj_uuid,player_uuid,direction]() {
			mods::doors::door_explosion_event(player_uuid,obj_uuid,direction);
		});
	}

	static inline bool is_open(const room_rnum  room, int direction){
		auto d_opt = world[room].dir_option[direction];
		if(!d_opt){
			return false;
		}
		bool is_door = d_opt->exit_info & EX_ISDOOR;
		bool is_closed = d_opt->exit_info & EX_CLOSED;
		bool is_breached = d_opt->exit_info & EX_BREACHED;
		bool is_reinforced = d_opt->exit_info & EX_REINFORCED;
		return !is_door ||
			(is_door && !is_closed) ||
			(is_door && is_breached) ||
			(is_door && is_reinforced && !is_breached);
	}

	static inline bool is_dir_electrified(const room_rnum room, int direction){
		auto d_opt = world[room].dir_option[direction];
		if(!d_opt){
			return false;
		}
		return ((d_opt->exit_info & EIM_ELECTRIFIED) == EIM_ELECTRIFIED);
	}

	static inline bool is_dir_breachable(const room_rnum  room, int direction){
		auto d_opt = world[room].dir_option[direction];
		if(!d_opt){
			return false;
		}
		return !is_open(room,direction) && !is_dir_electrified(room,direction);
	}

	static inline bool is_dir_thermite_breachable(const room_rnum  room, int direction){
		auto d_opt = world[room].dir_option[direction];
		if(!d_opt){
			return false;
		}
		return !is_open(room,direction) && 
			((d_opt->exit_info & EIM_REINFORCED) == EIM_REINFORCED) &&
			!is_dir_electrified(room,direction);
	}

	static inline std::string mask_to_string(int i){
#define MENTOC_MS_COMP(a) case BOOST_PP_CAT(EIM_,a): return #a;
		switch(static_cast<exit_info_masks_t>(i)){
			MENTOC_MS_COMP(ISDOOR)
				MENTOC_MS_COMP(CLOSED)
				MENTOC_MS_COMP(LOCKED)
				MENTOC_MS_COMP(PICKPROOF)
				MENTOC_MS_COMP(REINFORCED)
				MENTOC_MS_COMP(BREACHED)
				MENTOC_MS_COMP(BREACHABLE)
				MENTOC_MS_COMP(QUEST_LOCKED)
				MENTOC_MS_COMP(HIDDEN)
				MENTOC_MS_COMP(ELECTRIFIED)
			default: return "UNKNOWN";
		}
#undef MENTOC_MS_COMP
	}
	static inline int from_string(std::string_view i){
#define MENTOC_LAZY_COMP(a) if(!i.compare(#a)){ return BOOST_PP_CAT(EIM_,a); }
		MENTOC_LAZY_COMP(ISDOOR)
			MENTOC_LAZY_COMP(CLOSED)
			MENTOC_LAZY_COMP(LOCKED)
			MENTOC_LAZY_COMP(PICKPROOF)
			MENTOC_LAZY_COMP(REINFORCED)
			MENTOC_LAZY_COMP(BREACHED)
			MENTOC_LAZY_COMP(BREACHABLE)
			MENTOC_LAZY_COMP(QUEST_LOCKED)
			MENTOC_LAZY_COMP(HIDDEN)
			MENTOC_LAZY_COMP(ELECTRIFIED)
#undef MENTOC_LAZY_COMP
			return -1;
	}

	static inline std::vector<std::string> all_string_flags(int exit_info){
		std::vector<std::string> masks;
		for(int i=EIM_ISDOOR; i != exit_info_masks_t::__LAST; i <<= 1){
			if(exit_info & i){
				masks.emplace_back(mask_to_string(i));
			}
		}
		return masks;
	}

	static inline std::vector<exit_info_masks_t> all_integral_flags(int exit_info){
		std::vector<exit_info_masks_t> masks;
		for(int i=EIM_ISDOOR; i != exit_info_masks_t::__LAST; i <<= 1){
			if(exit_info & i){
				masks.emplace_back(i);
			}
		}
		return masks;
	}

};
#endif
