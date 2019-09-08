#include "../../globals.hpp"
#include "player-syncing.hpp"

namespace mods::testing::lmdb {
	player_syncing::player_syncing(int argc,char** argv){
		using namespace mods::db;
		auto player = mods::globals::player_list.emplace_back();
		mods::db::save_char(player);
		mods::globals::db->close();
	}
};
