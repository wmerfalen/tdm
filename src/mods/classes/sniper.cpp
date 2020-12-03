#include "sniper.hpp"
#include "../orm/inventory.hpp"
#include "../weapons/damage-types.hpp"
#include "../interpreter.hpp"

namespace mods::classes {
	sniper::sniper(){
		m_player = nullptr;
	}
	sniper::sniper(player_ptr_t p){
		m_player = nullptr;
		load_by_player(p);
	}
	player_ptr_t 	sniper::player(){
		return m_player;
	}
	void sniper::replenish(){

	}
	int16_t sniper::new_player(player_ptr_t &player){
		auto db_id = m_orm.initialize_row(player);
		if(db_id == 0){
			report(CAT("unable to initialize row for player (sniper::new_player) ",(player->db_id()),".. player's database id is zero!"));
			return -2;
		}
		player->set_db_id(db_id);
		load_by_player(player);
		return 0;
	}
	int16_t sniper::load_by_player(player_ptr_t & player){
		m_player = player;
		auto result = m_orm.load_by_player(player->db_id());
		if(result < 0){
			report(CAT("unable to load sniper class by player id: " , player->db_id() , ".. return status: " , result));
			return -100 - result;
		}
		obj_ptr_t primary = nullptr;
		primary = create_object(ITEM_RIFLE,mods::weapon::yaml_file(m_orm.primary_type()));
		player->equip(primary,WEAR_PRIMARY);
		player->equip(create_object(ITEM_RIFLE,"czp10.yml"),WEAR_SECONDARY);
		return result;
	}
	std::vector<uuid_t> sniper::get_targets_scanned_by_drone(){
		return m_scanned;
	}
	int16_t sniper::save() {
		mods::orm::inventory::flush_player(m_player);
		return m_orm.save();
	}
	void sniper::drone_scan(room_rnum room){
		m_scanned.clear();
		for(const auto & player_in_room : mods::globals::room_list[room]){
			if(player_in_room->is_npc()){
				m_scanned.emplace_back(player_in_room->uuid());
			}
		}
	}
	/*
	- Ability: X-Ray Shot (can shoot through multiple layers of walls/doors)
		- Can snipe an enemy within a building
		- Can snipe an enemy through walls or doors
		- If enemy not behind cover, it causes 150% damage to target
		*/
	std::tuple<bool,std::string> sniper::xray_shot(){
		return {1,"stub"};
	}

	std::shared_ptr<sniper> create_sniper(player_ptr_t &in_player){
		return std::move(std::make_shared<sniper>(in_player));
	}

ACMD(do_xray_shot){
	PLAYER_CAN("sniper.xray_shot");
	DO_HELP("xray_shot");
	auto weapon = player->primary();
	if(!weapon || weapon->rifle()->attributes->type != mw_rifle::SNIPER){
		player->sendln("You must be wielding a sniper rifle!");
		return;
	}
	int distance = weapon->rifle()->attributes->critical_range;
	int direction = NORTH;
	for(auto scanned : player->sniper()->get_targets_scanned_by_drone()){
		auto victim = ptr_by_uuid(scanned);
		if(!victim){
			continue;
		}
		auto feedback = mods::weapons::damage_types::rifle_attack_with_feedback(
			player,
			weapon,
			victim,
			distance,
			direction
		);
	}
}
};
namespace mods::class_abilities::sniper {
	void init(){
		//mods::interpreter::add_command("start_drone", POS_RESTING, mods::classes::do_start_drone, 0,0);
	}
};
