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
#include "orm/loot-payload.hpp"
#include "orm/loot-ammo.hpp"
#include "orm/static-loot.hpp"

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
	static constexpr std::size_t STATIC_LOOT_MINUTES = 5;
	bool should_run_replenish_static_loot();
	obj_ptr_t reward_player(player_ptr_t& player,mob_vnum victim);
	void replenish_static_loot();
	void new_room(room_data* room);
	struct crate_t {
		enum type_t : uint8_t {
			NONE = 0,
			PAYLOAD_CRATE,
			AMMO_CRATE,
			STATIC_LOOT_CRATE,
		};
		room_vnum room;
		room_rnum r_room;
		std::vector<obj_ptr_t> contents;
		uint16_t sacrifice_count;
		obj_ptr_t sacrifice;
		obj_ptr_t container;
		obj_ptr_t create_container(std::string_view yaml);
		void create_into_container(std::string_view yaml);
		crate_t(room_data* room,const std::shared_ptr<mods::orm::loot_payload> loot_payloads);
		crate_t(room_data* room,const std::shared_ptr<mods::orm::static_loot> static_loots);
		crate_t(room_data* room,const std::shared_ptr<mods::orm::loot_ammo> ammo_payloads);
		void* orm_object;
	};
	using crate_index_t = std::vector<crate_t>;
	void load_crates();

	const std::vector<obj_ptr_t>& payload_items(const obj_ptr_t& obj);
	const std::vector<obj_ptr_t>& ammo_items(const obj_ptr_t& obj);

	bool mixup_contents_of_crate(const obj_ptr_t& crate);
};

namespace mods::loot::events {
	void player_killed_npc(player_ptr_t& player,player_ptr_t& npc);
};
#endif
