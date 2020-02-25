#include "inventory.hpp"
#include <vector>
#include <map>
#include <string>

extern obj_ptr_t read_object_ptr(obj_vnum nr, int type);
extern obj_ptr_t blank_object();
extern void obj_to_char(obj_ptr_t  object, player_ptr_t player);
namespace mods::orm::inventory {

	void rifle_fetch(obj_ptr_t& obj,int id){
		try{
			auto select_transaction = txn();
			sql_compositor comp("object_rifle",&select_transaction);
			auto player_sql = comp.select("rifle_file")
				.from("object_rifle")
				.where("rifle_id","=",std::to_string(id))
				.sql();
			auto player_record = mods::pq::exec(select_transaction,player_sql);
			std::cerr << "rifle.size: " << player_record.size() << "\n";
			mods::pq::commit(select_transaction);
				for(auto && row : player_record){
					std::cerr << "blank obj\n";
					obj = blank_object();
					obj->rifle(row["rifle_file"].as<std::string>());
					break;
				}
		}catch(std::exception& e){
			std::cerr << __FILE__ << ": " << __LINE__ << ": error loading character preferences by pkid: '" << e.what() << "'\n";
		}
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
						rifle_fetch(obj,row["po_type_id"].as<int>());
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
						obj_to_char(obj,player);
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
		return 0;
//		{
//			try{
//				auto insert_transaction = txn();
//				sql_compositor comp("player_races",&insert_transaction);
//				auto up_sql = comp
//					.insert()
//					.into("player_races")
//					.values(mapped_values)
//					.sql();
//				mods::pq::exec(insert_transaction,up_sql);
//				mods::pq::commit(insert_transaction);
//			}catch(std::exception& e){
//				std::cerr << __FILE__ << ": " << __LINE__ << ": error seeding player_classes: '" << e.what() << "'\n";
//			}
//		}
//		{
//			try{
//				auto insert_transaction = txn();
//				sql_compositor comp("player_races",&insert_transaction);
//				auto up_sql = comp
//					.insert()
//					.into("player_races")
//					.values(mapped_values)
//					.sql();
//				mods::pq::exec(insert_transaction,up_sql);
//				mods::pq::commit(insert_transaction);
//			}catch(std::exception& e){
//				std::cerr << __FILE__ << ": " << __LINE__ << ": error seeding player_classes: '" << e.what() << "'\n";
//			}
//		}
	}

};

