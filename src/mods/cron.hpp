#ifndef __MENTOC_MODS_cron_HEADER__
#define  __MENTOC_MODS_cron_HEADER__

#include <iostream>
#include <functional>
#include <queue>
#include <sys/socket.h>
#include <netdb.h>
#include "../conf.h"
#include "../sysdep.h"
#include "../structs.h"
#include "../types.hpp"

namespace mods {
	class cron {
		public:
			cron() = delete;
			cron(uint64_t tick_resolution) : m_tres(tick_resolution) {

			}
			~cron() = default;
			/* TODO: Operator << for sending to the character */
			cron& operator<<(const char* m) {
				return *this;
			}
			cron& operator<<(const std::string m) {
				return *this;
			}
			cron& operator<<(int m) {
				return *this;
			}
		protected:
			std::queue<std::function<void()>> m_q;
			uint64_t m_tres;
	};
};

#endif
