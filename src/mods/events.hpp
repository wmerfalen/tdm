#ifndef __MENTOC_MODS_EVENTS_HEADER__
#define __MENTOC_MODS_EVENTS_HEADER__

#include <iostream>
#include "../structs.h"
#include <functional>
#include <deque>
#include <tuple>
#include <unordered_map>

namespace mods::events {
	enum event_enum {
		NONE = 0,
		CREATE_OBJECT,
		DESTROY_OBJECT,
		CREATE_PLAYER,
		DESTROY_PLAYER,
		OBJECT_FROM_ROOM,
		OBJECT_TO_ROOM
	};
	namespace publish {
		namespace objects {
			void create(uuid_t);
			void destroy(uuid_t);
		};
		namespace players {
			void create(uuid_t);
			void destroy(uuid_t);
		};
		namespace rooms {
			void object_from_room(uuid_t,room_rnum);
			void object_to_room(uuid_t,room_rnum);
		};
	};
	using event_t = event_enum;
	using callback_t = std::function<int(uuid_t,mods::events::event_t,void*,int)>;
	using listener_collection_t = std::unordered_map<mods::events::event_t,std::deque<callback_t>>;
	void register_callback(mods::events::event_t,callback_t);
	void publish_event(mods::events::event_t,const uuid_t&,void*,int);
};
#endif

