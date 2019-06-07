#include "debug.hpp"

namespace mods::debug {
	state::state(bool _show_tics) : m_show_tics(_show_tics) {

	}
	bool state::show_tics() { return m_show_tics; }
	void state::show_tics(bool b){ m_show_tics = b; }

	std::unique_ptr<state> debug_state;
};
