#include "../../globals.hpp"
#include "player-syncing.hpp"

namespace mods::testing::lmdb {
	player_syncing::player_syncing(int argc,char** argv){
		using namespace mods::db;
		auto player = mods::globals::player_list.emplace_back(
				std::make_shared<mods::player>());
		lmdb_save_char("far",player->cd(),mods::globals::db.get());
		mods::globals::db->close();
	}
};
