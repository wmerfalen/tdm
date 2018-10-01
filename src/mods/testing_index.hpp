#ifndef __MENTOC_MODS_TESTING_INDEX_HEADER__
#define __MENTOC_MODS_TESTING_INDEX_HEADER__

#define DESCRIBE_TESTING_SUITE(n_space,cls_name)\
	namespace mods::testing::n_space { \
		struct cls_name { \
			cls_name(int,char**);\
		};\
	};

DESCRIBE_TESTING_SUITE(lmdb,db)
DESCRIBE_TESTING_SUITE(lmdb,player_syncing)

#endif
