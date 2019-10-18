#ifndef __MENTOC_MODS_deferred_HEADER__
#define  __MENTOC_MODS_deferred_HEADER__

#include <iostream>
#include <functional>
#include <map>
#include <sys/socket.h>
#include <netdb.h>
#include "../conf.h"
#include "../sysdep.h"
#include "../structs.h"
#include "../types.hpp"

namespace mods {
	class deferred {
		public:
			constexpr static uint64_t TICK_RESOLUTION = 20;
			using seconds = uint16_t;
			deferred() = delete;
			deferred(uint64_t tick_resolution) : m_tres(tick_resolution),
				m_tick(0), m_iterations(0) {

			}
			~deferred() = default;
			/* TODO: Operator << for sending to the character */
			deferred& operator<<(const char* m) {
				return *this;
			}
			deferred& operator<<(const std::string m) {
				return *this;
			}
			deferred& operator<<(int m) {
				return *this;
			}
			void push(uint64_t ticks_in_future,std::function<void()> lambda);
			void push_secs(seconds secs,std::function<void()> lambda);
			void iteration();
		protected:
			std::multimap<uint64_t,std::function<void()>> m_q;
			std::multimap<seconds,std::function<void()>> m_secs;
			uint64_t m_tres;
			uint64_t m_tick;
			uint64_t m_iterations;
	};
};

#endif
