//#include "base.hpp"
#include "class-psyop.hpp"
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
	int16_t psyop::save(){
		try{
			auto up_txn = txn();
			sql_compositor comp(psyop::table_name,&up_txn);
			auto up_sql = comp
				.update(psyop::table_name)
				.set(export_class())
				.where("psyop_id","=",std::to_string(this->id))
				.sql();
			mods::pq::exec(up_txn,up_sql);
			mods::pq::commit(up_txn);
			return 0;
		}catch(std::exception& e){
			std::cerr << __FILE__ << ": " << __LINE__ << ": error updating psyop by pkid: '" << e.what() << "'\n";
			return -1;
		}
	}
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
		psyop_secondary_type = "czp10";
		if(primary_choice == primary_choice_t::SCARH){
			auto psg1 = mods::weapons::sniper_rifle::psg1::make();
			psyop_primary_weapon_id = psg1->rifle()->attributes->flush_to_db();
			psyop_primary_type = "SCARH";
		}
		if(primary_choice == primary_choice_t::UMP45){
			auto l96aw = mods::weapons::sniper_rifle::l96aw::make();
			psyop_primary_weapon_id = l96aw->rifle()->attributes->flush_to_db();
			psyop_primary_type = "UMP45";
		}
		auto czp10 = mods::weapons::pistol::czp10::make();
		psyop_secondary_weapon_id = czp10->rifle()->attributes->flush_to_db();
		try{
			auto insert_transaction = txn();
			sql_compositor comp("class_psyop",&insert_transaction);
			psyop_player_id = player->db_id();
			auto up_sql = comp
				.insert()
				.into("class_psyop")
				.values(export_class())
				.returning("psyop_id")
				.sql();
			auto record = mods::pq::exec(insert_transaction,up_sql);
			mods::pq::commit(insert_transaction);
			for(auto && row : record){
				updated_at = created_at = time(nullptr);
				loaded = 1;
				id = psyop_id = row["psyop_id"].as<uint64_t>();
				return id;
			}
		}catch(std::exception& e){
			std::cerr << __FILE__ << ": " << __LINE__ << ": error initializing psyop class row: '" << e.what() << "'\n";
		}
		return 0;
	}
	strmap_t psyop::export_class() {
		strmap_t values;
		values["psyop_player_id"] = std::to_string(psyop_player_id);
		values["psyop_primary_type"] = psyop_primary_type;
		values["psyop_primary_weapon_id"] = std::to_string( psyop_primary_weapon_id);
		values["psyop_secondary_type"] = "czp10";
		values["psyop_secondary_weapon_id"] = std::to_string(psyop_secondary_weapon_id);
		return std::move(values);
	}
	int16_t psyop::load_by_player(uint64_t player_id){
		try{
			auto select_transaction = txn();
			sql_compositor comp(table_name,&select_transaction);
			auto player_sql = comp.select("*")
				.from(table_name)
				.where("psyop_player_id","=",std::to_string(player_id))
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

