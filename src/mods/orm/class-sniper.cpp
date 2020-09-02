//#include "base.hpp"
#include "class-sniper.hpp"
#include <vector>
#include <map>
#include <string>
#include "../weapons/sniper-rifle-psg1.hpp"
#include "../weapons/sniper-rifle-l96aw.hpp"
#include "../weapons/pistol-czp10.hpp"
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
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	int16_t sniper::save(){
		return std::get<0>(mods::orm::util::update<sniper,sql_compositor>(this));
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
#if 0
		std::tuple<int16_t,std::string,uint64_t> status = mods::orm::util::insert_returing<sniper,sql_compositor>(this,primary_key_name());
		if(std::get<0>(status)){
			updated_at = created_at = time(nullptr);
			loaded = 1;
			id = sniper_id = std::get<2>(status);
			return id;
		}
		return std::get<0>(status);
#endif
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
		loaded = 0;
		created_at = updated_at = 0;
		id = sniper_id = 0;
		sniper_player_id = 0;
		sniper_secondary_type = sniper_primary_type ="NONE";
		sniper_primary_weapon_id = sniper_secondary_weapon_id = 0;
#if 0
		std::tuple<int16_t,std::string> status = mods::orm::util::load_by_pkid<sniper,sql_compositor>(this);
		loaded = std::get<0>(status) > 0;
		return std::get<0>(status);
#endif
		return 0;
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

