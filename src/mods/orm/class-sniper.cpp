#include "base.hpp"
//#include "class-sniper.hpp"
#include <vector>
#include <map>
#include <string>
#include "../weapons/sniper-rifle-psg1.hpp"
#include "../weapons/sniper-rifle-l96aw.hpp"
#include "../weapons/pistol-czp10.hpp"
#include <time.h>

/**
 * This class has the right idea, however it is unusable due to the fact
 * that we're errnoeously saving the primary and secondary weapons
 * to the user. In a mud, you must allow the user to customize their loadout.
 * This class violates that. Instead of saving the primary and secondary,
 * we should be using the mods::orm::inventory::feed/flush player functions
 * to update the player's carrying and euipment.
 */
namespace mods::orm {
	int16_t sniper::save(){
		try{
			auto up_txn = txn();
			sql_compositor comp(sniper::table_name,&up_txn);
			auto up_sql = comp
				.update(sniper::table_name)
				.set(export_class())
				.where("sniper_id","=",std::to_string(this->id))
				.sql();
			mods::pq::exec(up_txn,up_sql);
			mods::pq::commit(up_txn);
			return 0;
		}catch(std::exception& e){
			std::cerr << __FILE__ << ": " << __LINE__ << ": error updating sniper by pkid: '" << e.what() << "'\n";
			return -1;
		}
	}
	/**
	 * @brief this should be called when you create a sniper player for the first time
	 *
	 * @param player
	 * @param primary_choice
	 *
	 * @return 
	 */
	uint64_t sniper::initialize_row(player_ptr_t &player, primary_choice_t primary_choice) {
		init();
		sniper_secondary_type = "czp10";
		if(primary_choice == primary_choice_t::PSG1){
			auto psg1 = mods::weapons::sniper_rifle::psg1::make();
			sniper_primary_weapon_id = psg1->rifle()->attributes->flush_to_db();
			sniper_primary_type = "PSG1";
		}
		if(primary_choice == primary_choice_t::L96AW){
			auto l96aw = mods::weapons::sniper_rifle::l96aw::make();
			sniper_primary_weapon_id = l96aw->rifle()->attributes->flush_to_db();
			sniper_primary_type = "L96AW";
		}
		auto czp10 = mods::weapons::pistol::czp10::make();
		sniper_secondary_weapon_id = czp10->rifle()->attributes->flush_to_db();
		try{
			auto insert_transaction = txn();
			sql_compositor comp("class_sniper",&insert_transaction);
			sniper_player_id = player->db_id();
			auto up_sql = comp
				.insert()
				.into("class_sniper")
				.values(export_class())
				.returning("sniper_id")
				.sql();
			auto record = mods::pq::exec(insert_transaction,up_sql);
			mods::pq::commit(insert_transaction);
			for(auto && row : record){
				updated_at = created_at = time(nullptr);
				loaded = 1;
				id = sniper_id = row["sniper_id"].as<uint64_t>();
				return id;
			}
		}catch(std::exception& e){
			std::cerr << __FILE__ << ": " << __LINE__ << ": error initializing sniper class row: '" << e.what() << "'\n";
		}
		return 0;
	}
	strmap_t sniper::export_class() {
		strmap_t values;
		values["sniper_player_id"] = std::to_string(sniper_player_id);
		values["sniper_primary_type"] = sniper_primary_type;
		values["sniper_primary_weapon_id"] = std::to_string( sniper_primary_weapon_id);
		values["sniper_secondary_type"] = "czp10";
		values["sniper_secondary_weapon_id"] = std::to_string(sniper_secondary_weapon_id);
		return std::move(values);
	}
	int16_t sniper::load_by_player(uint64_t player_id){
		try{
			auto select_transaction = txn();
			sql_compositor comp(table_name,&select_transaction);
			auto player_sql = comp.select("*")
				.from(table_name)
				.where("sniper_player_id","=",std::to_string(player_id))
				.sql();
			auto player_record = mods::pq::exec(select_transaction,player_sql);
				for(auto && row : player_record){
					feed(row);
					return 0;
				}
				return -1;
		}catch(std::exception& e){
			std::cerr << __FILE__ << ": " << __LINE__ << ": error loading character by pkid: '" << e.what() << "'\n";
			return -2;
		}
	}
	int16_t sniper::feed(const pqxx::result::reference & row){
		init();
		loaded = 0;
		id = row["sniper_id"].as<uint64_t>();
		sniper_id = row["sniper_id"].as<uint64_t>();
		sniper_player_id = row["sniper_player_id"].as<uint64_t>();
		sniper_primary_type = row["sniper_primary_type"].c_str();
		sniper_primary_weapon_id = row["sniper_primary_weapon_id"].as<uint64_t>();
		sniper_secondary_type = row["sniper_secondary_type"].c_str();
		sniper_secondary_weapon_id = row["sniper_secondary_weapon_id"].as<uint64_t>();
		created_at = mods::util::pg_timestamp_to_long(row["created_at"].c_str());
		updated_at = mods::util::pg_timestamp_to_long(row["updated_at"].c_str());
		loaded = 1;
		return 0;
	}
	void sniper::init(){
		id = 0;
		sniper_id = 0;
		sniper_player_id = 0;
		sniper_primary_type.clear();
		sniper_primary_weapon_id = 0;
		sniper_secondary_type.clear();
		sniper_secondary_weapon_id = 0;
		created_at = 0;
		updated_at = 0;
		loaded = 0;
	}

};

