#include "engineer.hpp"
#include "../orm/inventory.hpp"
#include "../help.hpp"
#include "../interpreter.hpp"
#include "../projectile.hpp"
namespace mods::projectile {
	extern room_rnum resolve_room(room_rnum source_room,int direction,std::size_t depth);
};
extern void send_to_room(room_rnum room, const char *messg, ...);
namespace mods::classes {
	static obj_ptr_t& global_engineer_drone_bomb() {
		static obj_ptr_t drone_bomb = nullptr;
		if(!drone_bomb) {
			drone_bomb = create_object(ITEM_EXPLOSIVE,"engineer-drone-bomb.yml");
		}
		return drone_bomb;
	}
	void engineer::init() {
		m_drone_bomb_charges = 0;
		m_player = nullptr;
		m_sensor_grenade_charges = 0;
	}
	engineer::engineer() {
		this->init();
	}
	void engineer::replenish() {
		static uint16_t call_count = 0;
		++call_count;
		auto tier = tier(m_player);
		if((call_count % ENGINEER_REPLENISH_PULSE()) == 0) {
			if(m_sensor_grenade_charges + 1 < ENGINEER_SENSOR_GRENADE_MAX_COUNT() * tier) {
				++m_sensor_grenade_charges;
				m_player->sendln("{grn}An engineer class sensor grenade has been regenerated.{/grn}");
			}
			if(m_drone_bomb_charges + 1 < ENGINEER_DRONE_BOMB_CHARGE_MAX_COUNT() * tier) {
				++m_drone_bomb_charges;
				m_player->sendln("{grn}An engineer drone bomb has been regenerated.{/grn}");
			}
#ifdef __MENTOC_SEND_ENGINEER_PLAYER_REPLENISH_DEBUG_MESSAGE__
			m_player->sendln("Replenish");
#endif
		}
	}

	std::tuple<bool,std::string> engineer::drone_bomb(direction_t direction,uint8_t depth) {
		if(m_drone_bomb_charges == 0) {
			return {0,"You don't have any drone bomb charges."};
		}
		--m_drone_bomb_charges;
		room_rnum room_id = mods::projectile::resolve_room(m_player->room(),direction,depth);
		send_to_room(room_id,"A highly sophisticated drone bomb explodes in your face!");
		mods::projectile::explode(room_id,global_engineer_drone_bomb()->uuid,m_player->uuid());
		return {1,"You deployed your drone bomb."};
	}
	engineer::engineer(player_ptr_t p) {
		this->init();
		load_by_player(p);
	}
	player_ptr_t 	engineer::player() {
		return m_player;
	}

	int16_t engineer::new_player(player_ptr_t& player, primary_choice_t primary_choice) {
		if(primary_choice == primary_choice_t::NONE) {
			report("engineer::new_player. got primary_choice of zero. defaulting to P90");
			primary_choice = primary_choice_t::P90;
		}
		auto db_id = m_orm.initialize_row(player);
		if(db_id == 0) {
			return -2;
		}
		load_by_player(player);
		return 0;
	}
	int16_t engineer::load_by_player(player_ptr_t& player) {
		m_player = player;
		auto result = m_orm.load_by_player(player->db_id());
		if(result < 0) {
			report(CAT("unable to load engineer class by player id: ",player->db_id(), ".. return status: ", result));
		}
		obj_ptr_t primary = nullptr;
		/** TODO: create catchy name using the deep object parser */
		primary = create_object(ITEM_RIFLE,"ump45.yml");
		player->equip(primary,WEAR_PRIMARY);
		player->equip(create_object(ITEM_RIFLE,"glock.yml"),WEAR_SECONDARY);
		auto fatal = create_object(ITEM_EXPLOSIVE,"sensor-grenade.yml");
		player->carry(fatal);
		return result;
	}
	int16_t engineer::save() {
		mods::orm::inventory::flush_player(m_player);
		return m_orm.save();
	}
	std::shared_ptr<engineer> create_engineer(player_ptr_t& in_player) {
		return std::move(std::make_shared<engineer>(in_player));
	}
};

namespace mods::class_abilities::engineer {
	static constexpr const char* drone_bomb_usage = "usage: drone_bomb <direction> <distance>";
	ACMD(do_drone_bomb) {
		PLAYER_CAN("engineer.drone_bomb");
		auto vec_args = PARSE_ARGS();
		if(vec_args.size() != 2) {
			player->errorln(drone_bomb_usage);
			return;
		}
		auto parsed = mods::util::parse_direction_count(vec_args);
		if(!std::get<0>(parsed)) {
			player->errorln("Couldn't parse your direction/count.");
			return;
		}
		auto status = player->engineer()->drone_bomb(std::get<1>(parsed),std::get<2>(parsed));
		if(!std::get<0>(status)) {
			player->errorln(std::get<1>(status));
			return;
		}
		player->sendln(std::get<1>(status));
	}
	void init() {
		mods::interpreter::add_command("drone_bomb", POS_RESTING, do_drone_bomb, 0,0);
	}
};
