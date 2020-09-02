#include "class-sniper.hpp"

namespace mods::orm {
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
			auto psg1 = create_object(ITEM_RIFLE,"psg1.yml");
			sniper_primary_weapon_id = psg1->rifle()->attributes->flush_to_db();
			sniper_primary_type = PSG1;
		}
		if(primary_choice == primary_choice_t::L96AW){
			auto l96aw = create_object(ITEM_RIFLE,"l96aw.yml");
			sniper_primary_weapon_id = l96aw->rifle()->attributes->flush_to_db();
			sniper_primary_type = L96AW;
		}
		auto czp10 = create_object(ITEM_RIFLE,"czp10.yml");
		sniper_secondary_weapon_id = czp10->rifle()->attributes->flush_to_db();

		sniper_player_id = player->db_id();
		auto status = this->create<sniper>(this);
		if(ORM_SUCCESS(status)){
			updated_at = created_at = time(nullptr);
			loaded = 1;
			id = sniper_id = std::get<2>(status);
		}
		return id;
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
		loaded = 0;
		created_at = updated_at = 0;
		id = sniper_id = 0;
		sniper_player_id = 0;
		sniper_secondary_type = sniper_primary_type ="NONE";
		sniper_primary_weapon_id = sniper_secondary_weapon_id = 0;
		return std::get<0>(this->read<sniper>(this,"sniper_player_id",std::to_string(sniper_player_id)));
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

