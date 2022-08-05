#ifndef __MENTOC_MODS_LOOT_HEADER__
#define __MENTOC_MODS_LOOT_HEADER__
#include <iostream>
#include "forge-engine/generator.hpp"
#include "lmdb.hpp"
#include "super-users.hpp"
#include "interpreter.hpp"
#include "screen-searcher.hpp"
#include "forge-engine/generator.hpp"
#include "forge-engine/generated-rifle.hpp"
#include "forge-engine/generated-armor.hpp"

/**
 * Loot is also controlled by make_corpse and mods::drops.cpp
 */

namespace mods::forge_engine {
	extern mods::forge_engine::generator item_generator;
};
namespace mods::loot {
	static constexpr std::string_view PAYLOAD_YAML_FILE = "container/loot-payload.yml";
	static constexpr std::string_view AMMO_YAML_FILE = "container/loot-ammo.yml";
	static constexpr std::string_view STATIC_LOOT_YAML_FILE = "container/static-loot.yml";
	obj_ptr_t reward_player(player_ptr_t& player,mob_vnum victim);
	void new_room(room_data* room);
	struct crate_t {
		room_vnum room;
		std::vector<obj_ptr_t> contents;
		uint16_t sacrifice_count;
		obj_ptr_t sacrifice;
	};
	using crate_index_t = std::vector<crate_t>;
	void load_crates();

	const std::vector<obj_ptr_t>& payload_items(const obj_ptr_t& obj);
	const std::vector<obj_ptr_t>& ammo_items(const obj_ptr_t& obj);
};

namespace mods::loot::events {
	void player_killed_npc(player_ptr_t& player,player_ptr_t& npc);
};
#endif
