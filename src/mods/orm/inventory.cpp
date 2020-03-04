#include "inventory.hpp"
#include <vector>
#include <map>
#include <string>
#include "../weapons/shotgun-sasg12.hpp"
#include "../weapons/smg-mp5.hpp"
#include "../weapons/pistol-czp10.hpp"

extern obj_ptr_t read_object_ptr(obj_vnum nr, int type);
extern obj_ptr_t blank_object();
extern void obj_ptr_to_char(obj_ptr_t  object, player_ptr_t player);
namespace mods::orm::inventory {

	obj_data_ptr_t rifle_fetch(int id){
		try{
			auto select_transaction = txn();
			sql_compositor comp("object_rifle",&select_transaction);
			auto player_sql = comp.select("rifle_file")
				.from("object_rifle")
				.where("rifle_id","=",std::to_string(id))
				.sql();
			auto player_record = mods::pq::exec(select_transaction,player_sql);
			std::cerr << "rifle.size: " << player_record.size() << "\n";
				for(auto && row : player_record){
					auto file = row["rifle_file"].as<std::string>();
					std::cerr << "[rifle_fetch] rifle_file: '" << file << "'\n";
					
					if(file.compare("mp5.yml") == 0){
						return mods::weapons::smg::mp5::feed_by_file(file);
					}else if(file.compare("sasg12.yml") == 0){
						return mods::weapons::shotgun::sasg12::feed_by_file(file);
					}else if(file.compare("czp10.yml") == 0){
						return mods::weapons::pistol::czp10::feed_by_file(file);
					}
					break;
				}
			mods::pq::commit(select_transaction);
			std::cerr << "[mods::pq::commit] done...\n";
		}catch(std::exception& e){
			std::cerr << __FILE__ << ": " << __LINE__ << ": error loading character preferences by pkid: '" << e.what() << "'\n";
		}
		return nullptr;
	}
	int16_t feed_player(player_ptr_t & player){
		{
			try{
				// ----------------------------+-----------+----------+----------------------------------------------
				//  po_id        | integer                     |           | not null | nextval('player_object_po_id_seq'::regclass)
				//  po_player_id | integer                     |           | not null |
				//  po_type      | player_object_type_t        |           | not null | 'object'::player_object_type_t
				//po_wear_position INTEGER NOT NULL,
				//po_in_inventory INTEGER NOT NULL DEFAULT 0,
				//po_type_vnum
				//po_type_id
				//po_type_load 'id' or 'vnum'
				//  created_at   | timestamp without time zone |           | not null | CURRENT_TIMESTAMP
				//  updated_at   | timestamp without time zone |           | not null | CURRENT_TIMESTAMP

				auto select_transaction = txn();
				sql_compositor comp("player_object",&select_transaction);
				auto player_sql = comp.select("*")
					.from("player_object")
					.where("po_player_id","=",std::to_string(player->db_id()))
					.sql();
				auto player_record = mods::pq::exec(select_transaction,player_sql);
				obj_ptr_t obj = nullptr;
				mods::pq::commit(select_transaction);
				for(auto && row : player_record){
					obj = nullptr;
					//'rifle','explosive','attachment','gadget','drone', 'armor', 'consumable','object';
					if(row["po_type"].as<std::string>().compare("rifle") == 0){
						std::cerr << "mods::orm::feed_player we got a rifle...\n" ;
						obj = rifle_fetch(row["po_type_id"].as<int>());
					} else {
						log("Warning: unhandled po_type from player_object table: '%s'... skipping...",row["po_type"].c_str());
						continue;
					}
					if(!obj){
						log("Failed to load user object: player[%s] po_id[%d]", player->name().c_str(),row["po_id"].as<int>());
						continue;
					}
					if(row["po_in_inventory"].as<int>()){
						std::cerr << "[debug][feed_player]: " << player->name().c_str() << " carrying object po_id[" << row["po_id"].c_str() << "]\n";
						obj_ptr_to_char(obj,player);
						continue;
					}
					if(row["po_wear_position"].as<int>()){
						std::cerr << "[debug][feed_player]: " << player->name().c_str() << " wearing object po_id[" << row["po_id"].c_str() << "]\n";
						player->equip(obj,row["po_wear_position"].as<int>());
						continue;
					}
				}
				log("SYSERR: couldn't grab player's pkid: '%s'",player->name().c_str());
				return -1;
			}catch(std::exception& e){
				std::cerr << __FILE__ << ": " << __LINE__ << ": error loading character by pkid: '" << e.what() << "'\n";
				return -2;
			}
		}
	}//end feed_player

	int16_t flush_player(player_ptr_t & player){
		if(player->db_id() == 0){
			log("SYSERR: couldn't grab player's pkid: '%s' -- not able to flush player's inventory to db", player->name().c_str());
			return -1;
		}
		{
			try{
				auto del_txn = txn();
				sql_compositor comp("player_object",&del_txn);
				auto up_sql = comp
					.del()
					.from("player_object")
					.where("po_player_id","=",std::to_string(player->db_id()))
					.sql();
				mods::pq::exec(del_txn,up_sql);
				mods::pq::commit(del_txn);
			}catch(std::exception& e){
				std::cerr << __FILE__ << ": " << __LINE__ << ": error deleting player's existing player_object row: '" << e.what() << "'\n";
			}
		}

		std::map<std::string,std::string> mapped_values;
		for(unsigned i=0;i < NUM_WEARS;++i){
			auto eq = player->equipment(i);
			if(!eq){
				continue;
			}
//---+-----------------------------+-----------+----------+----------------------------------------------
// po_id            | integer                     |           | not null | nextval('player_object_po_id_seq'::regclass)
// po_player_id     | integer                     |           | not null |
// po_type          | player_object_type_t        |           | not null | 'object'::player_object_type_t
// po_type_vnum     | integer                     |           |          |
// po_type_id       | integer                     |           |          |
// po_type_load     | character varying(4)        |           | not null | 'id'::character varying
// po_wear_position | integer                     |           | not null |
// po_in_inventory  | integer                     |           | not null | 0
// created_at       | timestamp without time zone |           | not null | CURRENT_TIMESTAMP
// updated_at       | timestamp without time zone |           | not null | CURRENT_TIMESTAMP
			mapped_values.clear();
			mapped_values["po_player_id"] = std::to_string(player->db_id());
			mapped_values["po_type"] = eq->str_type;
			if(eq->str_type.compare("object") == 0){
				mapped_values["po_type_id"] = mapped_values["po_type_vnum"] = std::to_string(eq->item_number);
			}else{
				mapped_values["po_type_id"] = mapped_values["po_type_vnum"] = std::to_string(eq->extended_item_vnum);
			}
			mapped_values["po_type_load"] = "id";
			mapped_values["po_wear_position"] = std::to_string(i);
			mapped_values["po_in_inventory"] = "0";
			try{
				auto insert_transaction = txn();
				sql_compositor comp("player_object",&insert_transaction);
				auto up_sql = comp
					.insert()
					.into("player_object")
					.values(mapped_values)
					.sql();
				mods::pq::exec(insert_transaction,up_sql);
				mods::pq::commit(insert_transaction);
			}catch(std::exception& e){
				std::cerr << __FILE__ << ": " << __LINE__ << ": error inserting player_object row: '" << e.what() << "'\n";
			}
		}
		for(auto &eq : player->real_carrying()){
//---+-----------------------------+-----------+----------+----------------------------------------------
// po_id            | integer                     |           | not null | nextval('player_object_po_id_seq'::regclass)
// po_player_id     | integer                     |           | not null |
// po_type          | player_object_type_t        |           | not null | 'object'::player_object_type_t
// po_type_vnum     | integer                     |           |          |
// po_type_id       | integer                     |           |          |
// po_type_load     | character varying(4)        |           | not null | 'id'::character varying
// po_wear_position | integer                     |           | not null |
// po_in_inventory  | integer                     |           | not null | 0
// created_at       | timestamp without time zone |           | not null | CURRENT_TIMESTAMP
// updated_at       | timestamp without time zone |           | not null | CURRENT_TIMESTAMP
			mapped_values.clear();
			mapped_values["po_player_id"] = std::to_string(player->db_id());
			mapped_values["po_type"] = eq->str_type;
			if(eq->str_type.compare("object") == 0){
				mapped_values["po_type_id"] = mapped_values["po_type_vnum"] = std::to_string(eq->item_number);
			}else{
				mapped_values["po_type_id"] = mapped_values["po_type_vnum"] = std::to_string(eq->extended_item_vnum);
			}
			mapped_values["po_type_load"] = "id";
			mapped_values["po_wear_position"] = "0";
			mapped_values["po_in_inventory"] = "1";
			try{
				auto insert_transaction = txn();
				sql_compositor comp("player_object",&insert_transaction);
				auto up_sql = comp
					.insert()
					.into("player_object")
					.values(mapped_values)
					.sql();
				mods::pq::exec(insert_transaction,up_sql);
				mods::pq::commit(insert_transaction);
			}catch(std::exception& e){
				std::cerr << __FILE__ << ": " << __LINE__ << ": error inserting player_object row: '" << e.what() << "'\n";
				continue;
			}
		}
		return 0;
	}

};

