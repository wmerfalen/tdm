#include "no-drop.hpp"

#include "interpreter.hpp"

#define __MENTOC_SHOW_MODS_NO_DROP_DEBUG_OUTPUT__
#ifdef __MENTOC_SHOW_MODS_NO_DROP_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::no_drop::debug]")  << MSG << "\n";
#define m_error(MSG) mentoc_prefix_debug(red_str("[mods::no_drop::ERROR]"))  << MSG << "\n";
#else
#define m_debug(MSG) ;;
#define m_error(MSG) ;;
#endif

namespace mods::no_drop {
	bool npc_is_no_drop(const player_ptr_t& npc) {
		return npc && npc->char_specials().saved.act & MOB_NODROP;
	}
	bool should_drop(const player_ptr_t& npc) {
		return !npc_is_no_drop(npc);
	}


	void init() {
		//mods::interpreter::add_user_command("no_drop_me", do_no_drop_me);

	}

#undef m_error
#undef m_debug
};
