#include "class-psyop.hpp"

namespace mods::orm {
	/**
	 * @brief this should be called when you create a psyop player for the first time
	 *
	 * @param player
	 * @param primary_choice
	 *
	 * @return 
	 */
	uint64_t psyop::initialize_row(player_ptr_t &player, primary_choice_t primary_choice) {
		init();
		psyop_secondary_type = CZP10;
		if(primary_choice == primary_choice_t::SCARH){
			auto scarh = create_object(ITEM_RIFLE,"scarh.yml");
			psyop_primary_weapon_id = scarh->rifle()->attributes->flush_to_db();
			psyop_primary_type = SCARH;
		}
		if(primary_choice == primary_choice_t::UMP45){
			auto ump45 = create_object(ITEM_RIFLE,"ump45.yml");
			psyop_primary_weapon_id = ump45->rifle()->attributes->flush_to_db();
			psyop_primary_type = UMP45;
		}
		auto czp10 = create_object(ITEM_RIFLE,"czp10.yml");
		psyop_secondary_weapon_id = czp10->rifle()->attributes->flush_to_db();

		psyop_player_id = player->db_id();
		auto status = this->create<psyop>(this);
		if(ORM_SUCCESS(status)){
			updated_at = created_at = time(nullptr);
			loaded = 1;
			id = psyop_id = std::get<2>(status);
		}
		return id;
	}
	strmap_t psyop::export_class() {
		strmap_t values;
		values["psyop_player_id"] = std::to_string(psyop_player_id);
		values["psyop_primary_type"] = psyop_primary_type;
		values["psyop_primary_weapon_id"] = std::to_string( psyop_primary_weapon_id);
		values["psyop_secondary_type"] = CZP10;
		values["psyop_secondary_weapon_id"] = std::to_string(psyop_secondary_weapon_id);
		return std::move(values);
	}
	int16_t psyop::load_by_player(uint64_t player_id){
		loaded = 0;
		created_at = updated_at = 0;
		id = psyop_id = 0;
		psyop_player_id = 0;
		psyop_secondary_type = psyop_primary_type ="NONE";
		psyop_primary_weapon_id = psyop_secondary_weapon_id = 0;
		return std::get<0>(this->read<psyop>(this,"psyop_player_id",std::to_string(psyop_player_id)));
	}
	int16_t psyop::feed(const pqxx::result::reference & row){
		init();
		loaded = 0;
		id = row["psyop_id"].as<uint64_t>();
		psyop_id = row["psyop_id"].as<uint64_t>();
		psyop_player_id = row["psyop_player_id"].as<uint64_t>();
		psyop_primary_type = row["psyop_primary_type"].c_str();
		psyop_primary_weapon_id = row["psyop_primary_weapon_id"].as<uint64_t>();
		psyop_secondary_type = row["psyop_secondary_type"].c_str();
		psyop_secondary_weapon_id = row["psyop_secondary_weapon_id"].as<uint64_t>();
		created_at = mods::util::pg_timestamp_to_long(row["created_at"].c_str());
		updated_at = mods::util::pg_timestamp_to_long(row["updated_at"].c_str());
		loaded = 1;
		return 0;
	}
	void psyop::init(){
		id = 0;
		psyop_id = 0;
		psyop_player_id = 0;
		psyop_primary_type.clear();
		psyop_primary_weapon_id = 0;
		psyop_secondary_type.clear();
		psyop_secondary_weapon_id = 0;
		created_at = 0;
		updated_at = 0;
		loaded = 0;
	}

};
