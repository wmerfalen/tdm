#include "class-support.hpp"

namespace mods::orm {
	/**
	 * @brief this should be called when you create a support player for the first time
	 *
	 * @param player
	 * @param primary_choice
	 *
	 * @return 
	 */
	uint64_t support::initialize_row(player_ptr_t &player, primary_choice_t primary_choice) {
		init();
		support_secondary_type = CZP10;/** FIXME */
		if(primary_choice == primary_choice_t::MK46){
			auto primary = create_object(ITEM_RIFLE,"mk46.yml");
			support_primary_weapon_id = primary->rifle()->attributes->flush_to_db();
			support_primary_type = MK46;
		}
		if(primary_choice == primary_choice_t::HK21){
			auto primary = create_object(ITEM_RIFLE,"hk21.yml");
			support_primary_weapon_id = primary->rifle()->attributes->flush_to_db();
			support_primary_type = HK21;
		}
		auto czp10 = create_object(ITEM_RIFLE,"czp10.yml");/** FIXME */
		support_secondary_weapon_id = czp10->rifle()->attributes->flush_to_db();

		support_player_id = player->db_id();
		auto status = this->create<support>(this);
		if(ORM_SUCCESS(status)){
			updated_at = created_at = time(nullptr);
			loaded = 1;
			id = support_id = std::get<2>(status);
		}
		return id;
	}
	strmap_t support::export_class() {
		strmap_t values;
		values["support_player_id"] = std::to_string(support_player_id);
		values["support_primary_type"] = support_primary_type;
		values["support_primary_weapon_id"] = std::to_string( support_primary_weapon_id);
		values["support_secondary_type"] = "czp10";
		values["support_secondary_weapon_id"] = std::to_string(support_secondary_weapon_id);
		return std::move(values);
	}
	int16_t support::load_by_player(uint64_t player_id){
		loaded = 0;
		created_at = updated_at = 0;
		id = support_id = 0;
		support_player_id = 0;
		support_secondary_type = support_primary_type ="NONE";
		support_primary_weapon_id = support_secondary_weapon_id = 0;
		return std::get<0>(this->read<support>(this,"support_player_id",std::to_string(player_id)));
	}

	int16_t support::feed(const pqxx::result::reference & row){
		init();
		loaded = 0;
		id = row["support_id"].as<uint64_t>();
		support_id = row["support_id"].as<uint64_t>();
		support_player_id = row["support_player_id"].as<uint64_t>();
		support_primary_type = row["support_primary_type"].c_str();
		support_primary_weapon_id = row["support_primary_weapon_id"].as<uint64_t>();
		support_secondary_type = row["support_secondary_type"].c_str();
		support_secondary_weapon_id = row["support_secondary_weapon_id"].as<uint64_t>();
		created_at = mods::util::pg_timestamp_to_long(row["created_at"].c_str());
		updated_at = mods::util::pg_timestamp_to_long(row["updated_at"].c_str());
		loaded = 1;
		return 0;
	}
	void support::init(){
		id = 0;
		support_id = 0;
		support_player_id = 0;
		support_primary_type.clear();
		support_primary_weapon_id = 0;
		support_secondary_type.clear();
		support_secondary_weapon_id = 0;
		created_at = 0;
		updated_at = 0;
		loaded = 0;
	}

};
