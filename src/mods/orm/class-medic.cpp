//#include "base.hpp"
#include "class-medic.hpp"
#include <vector>
#include <map>
#include <string>
#include <time.h>
#include "../sql.hpp"
#include "../pq.hpp"

/**
 * This class has the right idea, however it is unusable due to the fact
 * that we're errnoeously saving the primary and secondary weapons
 * to the user. In a mud, you must allow the user to customize their loadout.
 * This class violates that. Instead of saving the primary and secondary,
 * we should be using the mods::orm::inventory::feed/flush player functions
 * to update the player's carrying and euipment.
 */
namespace mods::orm {
	/**
	 * @brief this should be called when you create a medic player for the first time
	 *
	 * @param player
	 * @param primary_choice
	 *
	 * @return 
	 */
	uint64_t medic::initialize_row(player_ptr_t &player, primary_choice_t primary_choice) {
		init();
		medic_secondary_type = CZP10;/** FIXME */
		if(primary_choice == primary_choice_t::AUGPARA){
			auto primary = create_object(ITEM_RIFLE,"augpara.yml");
			medic_primary_weapon_id = primary->rifle()->attributes->flush_to_db();
			medic_primary_type = AUGPARA;
		}
		if(primary_choice == primary_choice_t::TAR21){
			auto primary = create_object(ITEM_RIFLE,"tar21.yml");
			medic_primary_weapon_id = primary->rifle()->attributes->flush_to_db();
			medic_primary_type = TAR21;
		}
		auto czp10 = create_object(ITEM_RIFLE,"czp10.yml");/** FIXME */
		medic_secondary_weapon_id = czp10->rifle()->attributes->flush_to_db();

		medic_player_id = player->db_id();
		std::tuple<int16_t,std::string,uint64_t> status = this->create<medic>(this);
		if(std::get<0>(status)){
			updated_at = created_at = time(nullptr);
			loaded = 1;
			id = medic_id = std::get<2>(status);
			return id;
		}
		return std::get<0>(status);
	}
	strmap_t medic::export_class() {
		strmap_t values;
		values["medic_player_id"] = std::to_string(medic_player_id);
		values["medic_primary_type"] = medic_primary_type;
		values["medic_primary_weapon_id"] = std::to_string( medic_primary_weapon_id);
		values["medic_secondary_type"] = "czp10";
		values["medic_secondary_weapon_id"] = std::to_string(medic_secondary_weapon_id);
		return std::move(values);
	}
	int16_t medic::load_by_player(uint64_t player_id){
		loaded = 0;
		created_at = updated_at = 0;
		id = medic_id = 0;
		medic_player_id = 0;
		medic_secondary_type = medic_primary_type ="NONE";
		medic_primary_weapon_id = medic_secondary_weapon_id = 0;
		return std::get<0>(this->read<medic>(this,"medic_player_id",std::to_string(player_id)));
	}

	int16_t medic::feed(const pqxx::result::reference & row){
		init();
		loaded = 0;
		id = row["medic_id"].as<uint64_t>();
		medic_id = row["medic_id"].as<uint64_t>();
		medic_player_id = row["medic_player_id"].as<uint64_t>();
		medic_primary_type = row["medic_primary_type"].c_str();
		medic_primary_weapon_id = row["medic_primary_weapon_id"].as<uint64_t>();
		medic_secondary_type = row["medic_secondary_type"].c_str();
		medic_secondary_weapon_id = row["medic_secondary_weapon_id"].as<uint64_t>();
		created_at = mods::util::pg_timestamp_to_long(row["created_at"].c_str());
		updated_at = mods::util::pg_timestamp_to_long(row["updated_at"].c_str());
		loaded = 1;
		return 0;
	}
	void medic::init(){
		id = 0;
		medic_id = 0;
		medic_player_id = 0;
		medic_primary_type.clear();
		medic_primary_weapon_id = 0;
		medic_secondary_type.clear();
		medic_secondary_weapon_id = 0;
		created_at = 0;
		updated_at = 0;
		loaded = 0;
	}

};

