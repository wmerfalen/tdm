#include "mini-game.hpp"

namespace mods::orm {
#if 0
	uint64_t mini_game::initialize_row() {
		init();
		auto status = this->create<mini_game>(this);
		if(ORM_SUCCESS(status)){
			updated_at = created_at = time(nullptr);
			loaded = 1;
			id = game_id = std::get<2>(status);
		}
		return id;
	}
	strmap_t mini_game::export_class() {
		strmap_t values;
		values["game_id"] = std::to_string(game_id);
		//values["game_room_vnums"] = IMPLODE(",", game_room_vnum_list);
		values["game_type"] = game_type;
		values["game_vnum"] = std::to_string(game_vnum);
		return std::move(values);
	}

	int16_t mini_game::feed(const pqxx::result::reference & row){
		init();
		loaded = 0;
		id = row["game_id"].as<uint64_t>();
		game_id = row["game_id"].as<uint64_t>();
		//game_room_vnum = row["game_room_vnum"].as<room_vnum_t>();
		game_type = row["game_type"].c_str();
		game_vnum = row["game_vnum"].as<game_vnum_t>();
		created_at = mods::util::pg_timestamp_to_long(row["created_at"].c_str());
		updated_at = mods::util::pg_timestamp_to_long(row["updated_at"].c_str());
		loaded = 1;
		return 0;
	}
	void mini_game::init(){
		id = 0;
		game_id = 0;
		//game_room_vnum = 0;
		game_type = "";
		game_vnum = 0;
		created_at = 0;
		updated_at = 0;
		loaded = 0;
	}
#endif

	void mini_game::load_by_room_vnum(room_vnum game_room_vnum){
		this->read_normalized<mini_game>(this,"game_room_vnum",std::to_string(game_room_vnum));
	}

};
