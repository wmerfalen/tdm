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
		auto obj = blank_object();
		try{
			auto select_transaction = txn();
			sql_compositor comp("object_rifle",&select_transaction);
			auto player_sql = comp.select("rifle_file")
				.from("object_rifle")
				.where("rifle_id","=",std::to_string(id))
				.sql();
			auto player_record = mods::pq::exec(select_transaction,player_sql);
				for(auto && row : player_record){
					auto file = row["rifle_file"].as<std::string>();
					obj->rifle(file);
					obj->obj_flags.ammo = obj->rifle()->attributes->ammo_max;
					obj->obj_flags.ammo_max = obj->rifle()->attributes->ammo_max;
					obj->obj_flags.weapon_flags = obj->rifle()->type;
					mods::weapon::feed_caps(obj, obj->rifle()->type);
					obj->obj_flags.clip_size =obj->rifle()->attributes->clip_size;
					obj->name.assign(obj->rifle()->attributes->name);
					obj->description.assign(obj->rifle()->attributes->description);
					obj->short_description.assign(obj->rifle()->attributes->short_description);
					obj->action_description.assign(obj->rifle()->attributes->action_description);
					obj->ex_description.emplace_back(obj->name.c_str(),obj->description.c_str());
					obj->extended_item_vnum = obj->rifle()->attributes->vnum;
					break;
				}
				mods::pq::commit(select_transaction);
		}catch(std::exception& e){
			std::cerr << __FILE__ << ": " << __LINE__ << ": error loading character preferences by pkid: '" << e.what() << "'\n";
		}
		return std::move(obj);
	}
	namespace sql {
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
							player->carry(obj);
							continue;
						}
						if(row["po_wear_position"].as<int>()){
							player->equip(obj,row["po_wear_position"].as<int>());
							continue;
						}
					}
					//log("SYSERR: couldn't grab player's pkid: '%s'",player->name().c_str());
					std::cerr << "[sql][feed_player] size: " << player_record.size() << "\n";
					return player_record.size();
				}catch(std::exception& e){
					std::cerr << __FILE__ << ": " << __LINE__ << ": error loading character by pkid: '" << e.what() << "'\n";
					return -2;
				}
			}
		}//end feed_player

		void blank_wear_row(player_ptr_t & player, std::size_t i){
			std::map<std::string,std::string> mapped_values;
			mapped_values["po_player_id"] = std::to_string(player->db_id());
			mapped_values["po_type"] = "object";
			mapped_values["po_type_id"] = mapped_values["po_type_vnum"] = "0";
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

		int16_t flush_player(player_ptr_t & player){
			if(player->db_id() == 0){
				log("SYSERR: couldn't grab player's pkid: '%s' -- not able to flush player's inventory to db", player->name().c_str());
				return -1;
			}
			{
				try{
					d("Deleting player object rows..");
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
				d("done");
			}

			std::map<std::string,std::string> mapped_values;
			for(int i=0;i < NUM_WEARS;++i){
				d("Grabbing equipment");
				auto eq = player->equipment(i);
				if(!eq){
					d("No equipment for position: " << i);
					blank_wear_row(player, i);
					continue;
				}
				d("Found equipment at position: " << i);
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
				/*
					 if(eq->str_type.compare("object") == 0){
					 mapped_values["po_type_id"] = mapped_values["po_type_vnum"] = std::to_string(eq->item_number);
					 }else{
					 mapped_values["po_type_id"] = mapped_values["po_type_vnum"] = std::to_string(eq->extended_item_vnum);
					 }
					 */
				mapped_values["po_type_id"] = std::to_string(eq->db_id());
				mapped_values["po_type_vnum"] = std::to_string(eq->item_number);
				mapped_values["po_type_load"] = "id";
				mapped_values["po_wear_position"] = std::to_string(i);
				mapped_values["po_in_inventory"] = "0";
				try{
					d("Saving position: " << i << " [id:" << eq->db_id() << "][type:'" << eq->str_type << "']");
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
					d("ERROR - pq exception");
					std::cerr << __FILE__ << ": " << __LINE__ << ": error inserting player_object row: '" << e.what() << "'\n";
				}
				d("Done [position:" << i << "]");
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
				/*
					 if(eq->str_type.compare("object") == 0){
					 mapped_values["po_type_id"] = mapped_values["po_type_vnum"] = std::to_string(eq->item_number);
					 }else{
					 mapped_values["po_type_id"] = mapped_values["po_type_vnum"] = std::to_string(eq->extended_item_vnum);
					 }
					 */
				mapped_values["po_type_id"] = std::to_string(eq->db_id());
				mapped_values["po_type_vnum"] = std::to_string(eq->item_number);
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
	};//end namespace sql

	namespace lmdb {
		struct player_wear_t {
			uint64_t object_db_id;
			uint8_t type;
		};
		struct player_wear_key_t {
			char type[4];
			uint64_t player_db_id;
			uint8_t position;
		};
		struct player_inventory_data {
			char type[4];
			uint64_t player_db_id;
			uint64_t value_db_id;
		};
		int16_t flush_player(player_ptr_t & player){
			std::vector<uint64_t> wearing;
			wearing.resize(NUM_WEARS);
			for(std::size_t i = 0; i < NUM_WEARS;i++){
				auto eq = player->equipment(i);
				wearing.push_back(eq ? eq->db_id() : 0);
			}
			std::vector<uint64_t> carrying;
			for(auto &w : player->cd()->m_carrying){
				carrying.push_back(w->db_id());
			}
			auto wkey = "player|" + std::string(player->name().c_str()) + "|wearing";
			auto ckey ="player|" + std::string(player->name().c_str()) + "|carrying";

			std::size_t wkey_size_value = wearing.size();
			std::size_t ckey_size_value = carrying.size();

			auto wkey_size = "player|" + std::string(player->name().c_str()) + "|wearing|size";
			auto ckey_size="player|" + std::string(player->name().c_str()) + "|carrying|size";
			LMDBRENEW();
			LMDBNSET(wkey.data(),wkey.length()+1, &wearing[0],wearing.size() * sizeof(uint64_t));
			LMDBNSET(ckey.data(),ckey.length()+1, &carrying[0],carrying.size() * sizeof(uint64_t));
			LMDBNSET(wkey_size.data(),wkey_size.length()+1, (void*)&wkey_size_value,sizeof(wkey_size_value));
			LMDBNSET(ckey_size.data(),ckey_size.length()+1, (void*)&ckey_size_value,sizeof(ckey_size_value));
			LMDBCOMMIT();
			return 0;
		}
		int16_t feed_player(player_ptr_t & player){
			std::vector<uint64_t> wearing,carrying;
			auto wkey = "player|" + std::string(player->name().c_str()) + "|wearing";
			auto ckey = "player|" + std::string(player->name().c_str()) + "|carrying";
			auto wkey_size = "player|" + std::string(player->name().c_str()) + "|wearing|size";
			auto ckey_size = "player|" + std::string(player->name().c_str()) + "|carrying|size";

			std::size_t ckey_size_value,wkey_size_value;
			LMDBRENEW();
			LMDBVGET(ckey_size.data(),ckey_size.length()+1,(void*)&ckey_size_value);
			LMDBVGET(wkey_size.data(),wkey_size.length()+1,(void*)&wkey_size_value);
			std::cerr << "[wearing.resize]: " << wkey_size << "\n";
			wearing.resize(wkey_size_value);
			LMDBVGET(wkey.data(),wkey.length()+1,(void*)&wearing[0]);
			std::cerr << "[carrying.resize]: " << ckey_size << "\n";
			carrying.resize(ckey_size_value);
			LMDBVGET(ckey.data(),ckey.length()+1,(void*)&carrying[0]);
			LMDBCOMMIT();
			for(auto && carried_item : carrying){
				std::cerr << "[carrying stub]: " << carried_item << "\n";
			}
			return 0;
		}
		void add_player_wear(uint64_t player_db_id, uint64_t object_db_id, uint8_t object_type_id, uint8_t position){
			player_wear_t w;
			w.object_db_id = object_db_id;
			w.type = object_type_id;

			player_wear_key_t id;
			bcopy("wear",(void*)&id.type,sizeof(id.type));
			id.player_db_id = player_db_id;
			id.position = position;
			LMDBNSET((void*)&id,sizeof(id),(void*)&w,sizeof(w));
			auto uf_key = std::string("wear|") + std::to_string(player_db_id) + "|" + std::to_string(position);
			auto uf_val = std::to_string(object_db_id) + "|" + std::to_string(object_type_id);
			std::cerr << "[add_player_wear][uf_key:'" << uf_key << "'][uf_val:'" << uf_val << "']\n";
			LMDBSET(uf_key,uf_val);
		}
		void remove_player_wear(uint64_t player_db_id, uint8_t position){
			player_wear_key_t id;
			bcopy("wear",(void*)&id.type,sizeof(id.type));
			id.player_db_id = player_db_id;
			id.position = position;
			LMDBNDEL((void*)&id,sizeof(id));
			auto uf = std::string("wear|") + std::to_string(player_db_id) + "|" + std::to_string(position);
			std::cerr << "[remove_player_wear][uf_key:'" << uf << "']\n";

			LMDBDEL(uf);
		}
		void add_player_inventory(uint64_t player_db_id, uint64_t object_db_id, uint16_t obj_type){
			player_inventory_data key_data;
			bcopy("inv|",(void*)&key_data.type,sizeof(key_data.type));
			key_data.player_db_id = player_db_id;
			key_data.value_db_id = object_db_id;
			LMDBNSET((void*)&key_data,sizeof(key_data),(void*)&obj_type,sizeof(obj_type));
			auto uf_key = std::string("inv|") + std::to_string(player_db_id) + "|" + std::to_string(object_db_id);
			auto uf_val = std::to_string(obj_type);
			std::cerr << "[add_player_inventory][uf_key:'" << uf_key << "'][uf_val:'" << uf_val << "']\n";
			LMDBSET(uf_key,uf_val);
		}
		void remove_player_inventory(uint64_t player_db_id, uint64_t object_db_id){
			LMDBRENEW();
			player_inventory_data key_data;
			bcopy("inv|",(void*)&key_data.type,sizeof(key_data.type));
			key_data.player_db_id = player_db_id;
			key_data.value_db_id = object_db_id;
			LMDBNDEL((void*)&key_data,sizeof(key_data));

			auto uf_key = std::string("inv|") + std::to_string(player_db_id) + "|" + std::to_string(object_db_id);
			std::cerr << "[remove_player_inventory][uf_key:'" << uf_key << "']\n";
			LMDBDEL(uf_key);
		}
	};

	int16_t flush_player(player_ptr_t & player){
		return sql::flush_player(player);
	}
	int16_t feed_player(player_ptr_t & player){
		return sql::feed_player(player);
	}
};

