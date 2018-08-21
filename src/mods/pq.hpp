#ifndef __MENTOC_MODS_PQ_HEADER__
#define __MENTOC_MODS_PQ_HEADER__

#include "lmdb.hpp"

namespace pq {
	inline void transaction(void){}

#if 0
#include <time.h>
#include <netdb.h>
#include <sys/socket.h>
#include <cstdio>
typedef int socket_t;
#include "../structs.h"
#include "../utils.h"
#include "../conf.h"
#include "../sysdep.h"
#include <array>
#include "../mods/util.hpp"
#include "../globals.hpp"
#include <optional>

#define txn() mods::pq::transaction(*mods::globals::pq_con);

namespace mods::pq {
	using transaction = pqxx::work;
	using result = pqxx::result;
	pqxx::result exec(transaction& txn,std::string_view query);
	int as_int(result& r,int index, int index_y);
	void commit(transaction&);
	int count(std::string_view query);
};
#endif

#endif
