#ifndef __MENTOC_MODS_DEBUG_HEADER__
#define  __MENTOC_MODS_DEBUG_HEADER__

#include <iostream>
#include <memory>
namespace mods::debug {
	struct state {
		state(bool _show_tics);
		~state() = default;
		bool show_tics();
		void show_tics(bool b);
		private:
			bool m_show_tics;
	};
	extern std::unique_ptr<state> debug_state;
	static inline void init(bool tics){ debug_state = std::make_unique<state>(tics); }
};

#endif
