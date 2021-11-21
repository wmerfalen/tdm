#include "muted.hpp"

namespace mods::orm::admin {

	void muted::load_all() {
		this->read_all<muted>(this);

	}
	void muted::load_by_room_vnum(room_vnum game_room_vnum) {
		this->read_normalized<muted>(this,"game_room_vnum",std::to_string(game_room_vnum));
	}
	strmap_t muted::export_class() {
		strmap_t m;
		m["game_id"] = std::to_string(game_id);
		m["game_type"] = game_type;
		m["game_vnum"] = std::to_string(game_vnum);
		m["game_name"] = game_name;
		m["game_action"] = game_action;
		m["game_room_vnum"] = std::to_string(game_room_vnum);
		m["game_order"] = std::to_string(game_order);
		m["created_at"] = std::to_string(created_at);
		m["updated_at"] = std::to_string(updated_at);
		return m;
	}
};
