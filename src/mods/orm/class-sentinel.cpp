#include "class-sentinel.hpp"
#include <vector>
#include <map>
#include <string>
#include "../weapons/smg-mp5.hpp"
#include "../weapons/shotgun-sasg12.hpp"
#include "../weapons/pistol-czp10.hpp"

namespace mods::orm {
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	/**
	 * Primary_choice will be either "MP5" or "SASG12"
	 */
	uint64_t sentinel::initialize_row(player_ptr_t &player, primary_choice_t primary_choice) {
		this->init();
		sentinel_primary_type = mods::weapon::sentinel::to_string(primary_choice).data();
		sentinel_secondary_type = "czp10";
		if(primary_choice == primary_choice_t::MP5){
			auto mp5 = mods::weapons::smg::mp5::make();
			sentinel_primary_weapon_id = mp5->rifle()->attributes->flush_to_db();
		}
		if(primary_choice == primary_choice_t::SASG12){
			auto sasg12 = mods::weapons::shotgun::sasg12::make();
			sentinel_primary_weapon_id = sasg12->rifle()->attributes->flush_to_db();
		}
		auto czp10 = mods::weapons::pistol::czp10::make();
		sentinel_secondary_weapon_id = czp10->rifle()->attributes->flush_to_db();
			try{
				auto insert_transaction = txn();
				sql_compositor comp("class_sentinel",&insert_transaction);
				sentinel_player_id = player->db_id();
				auto up_sql = comp
					.insert()
					.into("class_sentinel")
					.values(export_class())
					.returning("sentinel_id")
					.sql();
				auto record = mods::pq::exec(insert_transaction,up_sql);
				mods::pq::commit(insert_transaction);
				for(auto && row : record){
					updated_at = created_at = time(nullptr);
					loaded = 1;
					this->id = this->sentinel_id = row["sentinel_id"].as<uint64_t>();
					return this->id;
				}
			}catch(std::exception& e){
				std::cerr << __FILE__ << ": " << __LINE__ << ": error initializing sentinel class row: '" << e.what() << "'\n";
			}
			return 0;
	}
	sql_save_map_t sentinel::export_class() {
		sql_save_map_t values;
		values["sentinel_player_id"] = this->sentinel_player_id;
		values["sentinel_primary_type"] = this->sentinel_primary_type;
		values["sentinel_primary_weapon_id"] =  this->sentinel_primary_weapon_id;
		values["sentinel_secondary_type"] =  "czp10";
		values["sentinel_secondary_weapon_id"] = this->sentinel_secondary_weapon_id;
		values["sentinel_heal_level"] = this->sentinel_heal_level;
		values["sentinel_intimidate_level"] = this->sentinel_intimidate_level;
		values["sentinel_human_shield_level"] = this->sentinel_human_shield_level;
		values["sentinel_deny_entry_level"] = this->sentinel_deny_entry_level;
		values["sentinel_gadget_shield_level"] = this->sentinel_gadget_shield_level;
		values["sentinel_oblique_kick_level"] = this->sentinel_oblique_kick_level;
		return std::move(values);
	}
	int16_t sentinel::load_by_player(uint64_t player_id){
		try{
			auto select_transaction = txn();
			sql_compositor comp(table_name,&select_transaction);
			auto player_sql = comp.select("*")
				.from(table_name)
				.where("sentinel_player_id","=",std::to_string(player_id))
				.sql();
			auto player_record = mods::pq::exec(select_transaction,player_sql);
				for(auto && row : player_record){
					this->feed(row);
					return 0;
				}
				return -1;
		}catch(std::exception& e){
			std::cerr << __FILE__ << ": " << __LINE__ << ": error loading character by pkid: '" << e.what() << "'\n";
			return -2;
		}
	}

	int16_t sentinel::feed(const pqxx::result::reference & row){
		this->init();
		this->loaded = 0;
		this->id = row["sentinel_id"].as<uint64_t>();
		this->sentinel_id = row["sentinel_id"].as<uint64_t>();
		this->sentinel_player_id = row["sentinel_player_id"].as<uint64_t>();
		this->sentinel_primary_type = row["sentinel_primary_type"].c_str();
		this->sentinel_primary_weapon_id = row["sentinel_primary_weapon_id"].as<uint64_t>();
		this->sentinel_secondary_type = row["sentinel_secondary_type"].c_str();
		this->sentinel_secondary_weapon_id = row["sentinel_secondary_weapon_id"].as<uint64_t>();
		this->sentinel_heal_level = row["sentinel_heal_level"].as<uint64_t>();
		this->sentinel_intimidate_level = row["sentinel_intimidate_level"].as<uint64_t>();
		this->sentinel_human_shield_level = row["sentinel_human_shield_level"].as<uint64_t>();
		this->sentinel_deny_entry_level = row["sentinel_deny_entry_level"].as<uint64_t>();
		this->sentinel_gadget_shield_level = row["sentinel_gadget_shield_level"].as<uint64_t>();
		this->sentinel_oblique_kick_level = row["sentinel_oblique_kick_level"].as<uint64_t>();
		this->created_at = row["created_at"].as<long>();
		this->updated_at = row["updated_at"].as<long>();
		this->loaded = 1;
		return 0;
	}
	void sentinel::init(){
		id = 0;
		sentinel_id = 0;
		sentinel_player_id = 0;
		sentinel_primary_type.clear();
		sentinel_primary_weapon_id = 0;
		sentinel_secondary_type.clear();
		sentinel_secondary_weapon_id = 0;
		sentinel_heal_level = 0;
		sentinel_intimidate_level = 0;
		sentinel_human_shield_level =0;
		sentinel_deny_entry_level = 0;
		sentinel_gadget_shield_level = 0;
		sentinel_oblique_kick_level = 0;
		created_at = 0;
		updated_at = 0;
		loaded = 0;
	}

};

