#include "attachment-underbarrel.hpp"
/** db.h for read_object() */
#include "../../db.h"
#include "../../globals.hpp"

#define __MENTOC_SHOW_ATTACHMENT_UNDERBARREL_DEBUG_OUTPUT__
#ifdef __MENTOC_SHOW_ATTACHMENT_UNDERBARREL_DEBUG_OUTPUT__
#define m_debug(a) std::cerr << green_str("[attachment-underbarrel]:") << a << "\n";
#else
#define m_debug(a)
#endif
namespace mods::weapons {
	void attachment_underbarrel::init() {
		m_attached = false;
		m_obj = nullptr;
		m_yaml_file.clear();
	}
	attachment_underbarrel::~attachment_underbarrel() {
		m_attached = false;
		m_obj = nullptr;
		m_yaml_file.clear();
	}

	attachment_underbarrel::attachment_underbarrel() {
		init();
	}

	void attachment_underbarrel::set_yaml_file(std::string_view y) {
		m_yaml_file = y;
	}
	std::string_view attachment_underbarrel::get_yaml_file() {
		return m_yaml_file;
	}
	uint16_t attachment_underbarrel::ammo() {
		return m_obj ? m_obj->obj_flags.ammo : 0;
	}
	bool attachment_underbarrel::is_attached() const {
		return m_attached;
	}

	void attachment_underbarrel::consume_ammo() {
		if(m_attached) {
			m_debug("obj_flags.ammo->" << m_obj->obj_flags.ammo);
			if(m_obj->obj_flags.ammo == 0) {
				m_attached = false;
				return;
			}
			--m_obj->obj_flags.ammo;
			if(m_obj->obj_flags.ammo == 0) {
				m_attached = false;
			}
		}
	}
	obj_ptr_t attachment_underbarrel::obj() {
		if(m_attached) {
			return m_obj;
		}
		return nullptr;
	}

};
#undef m_debug
