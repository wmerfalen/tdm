#ifndef __MENTOC_MODS_CAMERA_HEADER__
#define  __MENTOC_MODS_CAMERA_HEADER__

#include <iostream>
#include "deferred.hpp"

namespace mods {
	class camera {
		public:
			camera() = delete;
			camera(uuid_t player, uuid_t obj);
			camera(uuid_t player, uuid_t obj, uint16_t time);
			uuid_t object_uuid() const { return m_object_uuid; }
			uuid_t owner_uuid() const { return m_player; }
			uuid_t player_uuid() const { return m_player; }
			int room();
			void queue_object_destruct();
			void destruct_now();
			~camera() = default;
		protected:
			uuid_t m_player;
			uuid_t m_object_uuid;
			uint16_t m_time;
			int m_in_room;
	};
};

#endif
