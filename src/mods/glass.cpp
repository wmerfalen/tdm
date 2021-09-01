#include "glass.hpp"
#include "affects.hpp"
#include "skills.hpp"

#include "rooms.hpp"
#include "injure.hpp"
#include "doors.hpp"
#include "super-users.hpp"
#include "interpreter.hpp"

#define __MENTOC_SHOW_MODS_GLASS_DEBUG_OUTPUT__
#ifdef __MENTOC_SHOW_MODS_GLASS_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::glass]")  << MSG << "\n";
#define m_error(MSG) mentoc_prefix_debug(red_str("[mods::glass::ERROR]"))  << MSG << "\n";
#else
#define m_debug(MSG) ;;
#define m_error(MSG) ;;
#endif


namespace mods::glass {
	void scatter_room(const room_rnum& room_id) {
		m_debug("scattering glass all over room: " << room_id);
		world[room_id].add_texture(room_data::texture_type_t::SHATTERED_GLASS);
	}
};

