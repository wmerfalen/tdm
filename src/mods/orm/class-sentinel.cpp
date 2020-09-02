#include "class-sentinel.hpp"
#include "../weapons/smg-mp5.hpp"
#include "../weapons/shotgun-sasg12.hpp"
#include "../weapons/pistol-czp10.hpp"

namespace mods::orm {
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;

	/** Handy little script to restore a sentinel's primary and secondary weapons:
	 * You can pass it space separated primary keys
	 * run ./sql/restore-sentinel.sh $USER_PK_ID [...$USER_PK_ID_N]
	 * Then run sql-copy, then \i sql/restore.sql
	 */
	/**
	 * Primary_choice will be either "MP5" or "SASG12"
	 */
	/**
	 * @brief this should be called when you create a sentinel player for the first time
	 *
	 * @param player
	 * @param primary_choice
	 *
	 * @return 
	 */
	uint64_t sentinel::initialize_row(player_ptr_t &player, primary_choice_t primary_choice) {
		this->init();
		sentinel_secondary_type = "czp10";
		if(primary_choice == primary_choice_t::MP5){
			auto mp5 = mods::weapons::smg::mp5::make();
			sentinel_primary_weapon_id = mp5->rifle()->attributes->flush_to_db();
			sentinel_primary_type = MP5;
		}
		if(primary_choice == primary_choice_t::SASG12){
			auto sasg12 = mods::weapons::shotgun::sasg12::make();
			sentinel_primary_weapon_id = sasg12->rifle()->attributes->flush_to_db();
			sentinel_primary_type = SASG12;
		}
		auto czp10 = mods::weapons::pistol::czp10::make();
		sentinel_secondary_weapon_id = czp10->rifle()->attributes->flush_to_db();
		sentinel_player_id = player->db_id();
		auto status = this->create<sentinel>(this);
		if(ORM_SUCCESS(status)){
			updated_at = created_at = time(nullptr);
			loaded = 1;
			id = sentinel_id = std::get<2>(status);
		}
		return id;
	}
	sql_save_map_t sentinel::export_class() {
		sql_save_map_t values;
		values["sentinel_player_id"] = std::to_string(this->sentinel_player_id);
		values["sentinel_primary_type"] = this->sentinel_primary_type;
		values["sentinel_primary_weapon_id"] = std::to_string( this->sentinel_primary_weapon_id);
		values["sentinel_secondary_type"] = "czp10";
		values["sentinel_secondary_weapon_id"] = std::to_string(this->sentinel_secondary_weapon_id);
		values["sentinel_heal_level"] = std::to_string(this->sentinel_heal_level);
		values["sentinel_intimidate_level"] = std::to_string(this->sentinel_intimidate_level);
		values["sentinel_human_shield_level"] = std::to_string(this->sentinel_human_shield_level);
		values["sentinel_deny_entry_level"] = std::to_string(this->sentinel_deny_entry_level);
		values["sentinel_gadget_shield_level"] = std::to_string(this->sentinel_gadget_shield_level);
		values["sentinel_oblique_kick_level"] = std::to_string(this->sentinel_oblique_kick_level);
		return std::move(values);
	}
	int16_t sentinel::load_by_player(uint64_t player_id){
		loaded = 0;
		created_at = updated_at = 0;
		id = sentinel_id = 0;
		sentinel_player_id = 0;
		sentinel_secondary_type = sentinel_primary_type ="NONE";
		sentinel_primary_weapon_id = sentinel_secondary_weapon_id = 0;
		return std::get<0>(this->read<sentinel>(this,"sentinel_player_id",std::to_string(player_id)));
	}
	long pg_timestamp_to_long(std::string timestamp){
		struct tm time;
		memset(&time,0,sizeof(tm));
		strptime(timestamp.c_str(), "%Y-%m-%d %H:%M:%S",&time);
		return mktime(&time);
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
		this->created_at = pg_timestamp_to_long(row["created_at"].c_str());
		this->updated_at = pg_timestamp_to_long(row["updated_at"].c_str());
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

