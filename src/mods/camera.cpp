#ifndef __MENTOC_MODS_CAMERA_SOURCE__
#define  __MENTOC_MODS_CAMERA_SOURCE__

#include "camera.hpp"
#include "player.hpp"

namespace mods {
	camera::camera(uuid_t  owner, uuid_t  obj) :
		m_player(owner), m_object_uuid(obj), m_time(60 * 5)  {
			auto ptr = optr_by_uuid(obj);
			m_in_room = ptr->in_room;
			queue_object_destruct();
	}
	camera::camera(uuid_t  owner, uuid_t  obj, uint16_t time) :
		m_player(owner), m_object_uuid(obj), m_time(time)  {
			auto ptr = optr_by_uuid(obj);
			m_in_room = ptr->in_room;
			queue_object_destruct();
	}
	void camera::queue_object_destruct() {
		d("[stub] queing object destruct: " << m_object_uuid << " time:" << m_time);
		//mods::globals::queue_object_destruct(m_object_uuid,m_time);
	}
	void camera::destruct_now() {
		d("[stub] destructing camera object: " << m_object_uuid);
		//mods::globals::destruct_object(m_object_uuid);
	}
	int camera::room() {
		return m_in_room;
	}
};

#endif
