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
		uint64_t primary_choice_weapon_id = 0;
		uint64_t secondary_choice_weapon_id = 0;
		if(primary_choice == primary_choice_t::MP5){
			auto mp5 = mods::weapons::smg::mp5::make();
			primary_choice_weapon_id = mp5->rifle()->attributes->flush_to_db();
		}
		if(primary_choice == primary_choice_t::SASG12){
			auto sasg12 = mods::weapons::shotgun::sasg12::make();
			primary_choice_weapon_id = sasg12->rifle()->attributes->flush_to_db();
		}
		auto czp10 = mods::weapons::pistol::czp10::make();
		secondary_choice_weapon_id = czp10->rifle()->attributes->flush_to_db();
		{
			try{
				auto insert_transaction = txn();
				sql_compositor comp("class_sentinel",&insert_transaction);
				std::map<std::string,std::string> values;
				values["sentinel_player_id"] = player->db_id();
				values["sentinel_primary_type"] =  mods::weapon::sentinel::to_string(primary_choice).data();
				values["sentinel_primary_weapon_id"] =  primary_choice_weapon_id;
				values["sentinel_secondary_type"] =  "czp10";
				values["sentinel_secondary_weapon_id"] =  secondary_choice_weapon_id;
				values["sentinel_heal_level"] = "0";
				values["sentinel_intimidate_level"] = "0";
				auto up_sql = comp
					.insert()
					.into("class_sentinel")
					.values(values)
					.returning("sentinel_id")
					.sql();
				auto record = mods::pq::exec(insert_transaction,up_sql);
				mods::pq::commit(insert_transaction);
				for(auto && row : record){
					return row["sentinel_id"].as<uint64_t>();
				}
			}catch(std::exception& e){
				std::cerr << __FILE__ << ": " << __LINE__ << ": error initializing sentinel class row: '" << e.what() << "'\n";
				return 0;
			}
			return 0;
		}
	}
	int16_t sentinel::feed(const pqxx::result::reference & row){
		this->init();
		this->loaded = 0;
		this->id = row["sentinel_id"].as<uint64_t>();
		this->sentinel_id = row["sentinel_id"].as<uint64_t>(); // SERIAL,
		this->sentinel_player_id = row["sentinel_player_id"].as<uint64_t>(); // INTEGER NOT NULL,
		this->sentinel_primary_type = row["sentinel_primary_type"].c_str();// VARCHAR(16) NOT NULL DEFAULT 'mp5',
		this->sentinel_primary_weapon_id = row["sentinel_primary_weapon_id"].as<uint64_t>();// INTEGER NOT NULL DEFAULT 0,
		this->sentinel_secondary_type = row["sentinel_secondary_type"].c_str();// VARCHAR(16) NOT NULL DEFAULT 'czp10',
		this->sentinel_secondary_weapon_id = row["sentinel_secondary_weapon_id"].as<uint64_t>();// INTEGER NOT NULL DEFAULT 0,
		this->sentinel_heal_level = row["sentinel_heal_level"].as<uint64_t>();// INTEGER NOT NULL DEFAULT 0,
		this->sentinel_intimidate_level = row["sentinel_intimidate_level"].as<uint64_t>();// INTEGER NOT NULL DEFAULT 0,
		this->created_at = row["created_at"].as<long>();
		this->updated_at = row["updated_at"].as<long>();
		this->loaded = 1;
		return 0;
	}
	void sentinel::init(){
		id = 0;
		sentinel_id = 0; // SERIAL,
		sentinel_player_id = 0; // INTEGER NOT NULL,
		sentinel_primary_type.clear();// VARCHAR(16) NOT NULL DEFAULT 'mp5',
		sentinel_primary_weapon_id = 0;// INTEGER NOT NULL DEFAULT 0,
		sentinel_secondary_type.clear();// VARCHAR(16) NOT NULL DEFAULT 'czp10',
		sentinel_secondary_weapon_id = 0;// INTEGER NOT NULL DEFAULT 0,
		sentinel_heal_level = 0;// INTEGER NOT NULL DEFAULT 0,
		sentinel_intimidate_level = 0;// INTEGER NOT NULL DEFAULT 0,
		created_at = 0;// TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
		updated_at = 0;// TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
		loaded = 0;
	}

};

