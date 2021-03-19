#include "inventory.hpp"
#include <vector>
#include <map>
#include <string>
#include "../weapons/shotgun-sasg12.hpp"
#include "../weapons/smg-mp5.hpp"
#include "../weapons/pistol-czp10.hpp"
#include "../../globals.hpp"
#include "rifle-attachment.hpp"

extern obj_ptr_t read_object_ptr(obj_vnum nr, int type);
extern obj_ptr_t blank_object();
extern void obj_ptr_to_char(obj_ptr_t  object, player_ptr_t player);
extern obj_ptr_t create_object_from_index(std::size_t proto_index);
namespace mods::orm::inventory {

#ifdef __MENTOC_USE_SQL_FLUSH_PLAYER__
	obj_data_ptr_t dynamic_fetch(mentoc_pqxx_result_t row) { //int id, std::string_view in_type){
		std::string in_type = row["po_type"].c_str();
		int id = 0;
		std::string field_suffix = "_id";
		if(row["po_type_load"].as<std::string>().compare("vnum") == 0) {
			id = row["po_type_vnum"].as<int>();
			field_suffix = "_vnum";
		} else {
			id = row["po_type_id"].as<int>();
		}

#ifdef __MENTOC_FLUSH_PLAIN_OBJECTS__
#else
		if(in_type.compare("object") == 0) {
			return nullptr;
		}
#endif
		try {
			auto select_transaction = txn();
			std::string table = "object_";
			table += in_type.data();
			std::string file_field = in_type.data();
			file_field += "_file";
			std::string id_field = in_type.data();
			id_field += field_suffix;
			sql_compositor comp(in_type,&select_transaction);
			auto player_sql = comp.select(file_field)
			                  .from(table)
			                  .where(id_field,"=",std::to_string(id))
			                  .sql();
			auto player_record = mods::pq::exec(select_transaction,player_sql);
			if(player_record.size()) {
				/** FIXME */
				int t = 0;
				/** !!*****************!! */
				/** !!UPDATE_ITEM_TYPES!! */
				/** !!*****************!! */
				if(in_type.compare("container") == 0) {
					t = ITEM_CONTAINER;
				}
				if(in_type.compare("trap") == 0) {
					t = ITEM_TRAP;
				}
				if(in_type.compare("consumable") == 0) {
					t = ITEM_CONSUMABLE;
				}
				if(in_type.compare("drone") == 0) {
					t = ITEM_DRONE;
				}
				if(in_type.compare("armor") == 0) {
					t = ITEM_ARMOR;
				}
				if(in_type.compare("attachment") == 0) {
					t = ITEM_ATTACHMENT;
				}
				if(in_type.compare("explosive") == 0) {
					t = ITEM_EXPLOSIVE;
				}
				if(in_type.compare("rifle") == 0) {
					t = ITEM_RIFLE;
				}
				if(in_type.compare("gadget") == 0) {
					t = ITEM_GADGET;
				}
				if(in_type.compare("melee") == 0) {
					t = ITEM_MELEE;
				}
				if(in_type.compare("vehicle") == 0) {
					t = ITEM_VEHICLE;
				}
				mods::pq::commit(select_transaction);
				return std::move(create_object(t,player_record[0][file_field].c_str()));
			}
			mods::pq::commit(select_transaction);
		} catch(std::exception& e) {
			REPORT_DB_ISSUE(": error loading character preferences by pkid: '",e.what());
		}
		return nullptr;
	}
	namespace sql {
		int16_t feed_player(player_ptr_t& player) {
			//mods::orm::load_player_rifle_attachments(player);
			{
				try {
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
					for(auto&& row : player_record) {
						obj = nullptr;
#ifdef __MENTOC_FLUSH_PLAIN_OBJECTS__
#else
						if(row["po_type"].as<std::string>().compare("object") == 0) {
							continue;
						}
#endif
						obj = dynamic_fetch(row);//["po_type_id"].as<int>(), row["po_type"].as<std::string>());
						if(!obj) {
							log("Failed to load user object: player[%s] po_id[%d]", player->name().c_str(),row["po_id"].as<int>());
							continue;
						}
						if(row["po_in_inventory"].as<int>()) {
							player->carry(std::move(obj));
							continue;
						}
						if(row["po_wear_position"].as<int>()) {
							player->equip(std::move(obj),row["po_wear_position"].as<int>());
							continue;
						}
					}
					//log("SYSERR: couldn't grab player's pkid: '%s'",player->name().c_str());
					d("[sql][feed_player] size: " << player_record.size() << "\n");
					return player_record.size();
				} catch(std::exception& e) {
					REPORT_DB_ISSUE(": error loading character by pkid: '",e.what());
					return -2;
				}
			}
		}//end feed_player

		int16_t flush_player(player_ptr_t& player) {
			//mods::orm::flush_player_rifle_attachments(player);
			if(player->db_id() == 0) {
				log("SYSERR: couldn't grab player's pkid: '%s' -- not able to flush player's inventory to db", player->name().c_str());
				return -1;
			}
			{
				try {
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
				} catch(std::exception& e) {
					REPORT_DB_ISSUE(": error deleting player's existing player_object row: '",e.what());
				}
				d("done");
			}

			std::map<std::string,std::string> mapped_values;
			for(int i=0; i < NUM_WEARS; ++i) {
				d("Grabbing equipment");
				auto eq = player->equipment(i);
				if(!eq) {
					continue;
				}
#ifdef __MENTOC_FLUSH_PLAIN_OBJECTS__
#else
				if(eq->str_type.compare("object") == 0) {
					continue;
				}
#endif
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
				try {
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
				} catch(std::exception& e) {
					REPORT_DB_ISSUE(": error inserting player_object row: '",e.what());
				}
				d("Done [position:" << i << "]");
			}
			for(auto& eq : player->real_carrying()) {
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
				try {
					auto insert_transaction = txn();
					sql_compositor comp("player_object",&insert_transaction);
					auto up_sql = comp
					              .insert()
					              .into("player_object")
					              .values(mapped_values)
					              .sql();
					mods::pq::exec(insert_transaction,up_sql);
					mods::pq::commit(insert_transaction);
				} catch(std::exception& e) {
					REPORT_DB_ISSUE(": error inserting player_object row: '",e.what());
					continue;
				}
			}
			return 0;
		}
	};//end namespace sql
#endif

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
		namespace yaml {
			std::string just_yaml_file(std::string path) {
				if(path.length() == 0) {
					return "";
				}
				auto pos = path.find_last_of("/");
				if(pos == std::string::npos) {
					return path;
				}
				++pos;
				if(pos < path.length()) {
					return path.substr(pos);
				}
				return "";
			}
#ifdef __MENTOC_SHOW_ORM_INVENTORY_FLUSH_OUTPUT__
#define DBG(a) std::cerr << "[mods::orm::inventory::flush_player][LINE:" << __LINE__ << "][FILE:" << __FILE__ << "]->'" << a << "'\n";
#else
#define DBG(a) ;;
#endif
			int16_t flush_player(player_ptr_t& player) {
				//mods::orm::flush_player_rifle_attachments(player);
				DBG("entrance");
				LMDBRENEW();
				std::string wearing = "",carrying = "";
				for(std::size_t i = 0; i < NUM_WEARS; i++) {
					auto eq = player->equipment(i);
					if(!eq) {
						DBG("invalid equipment in slot: " << i);
						continue;
					}
					wearing += txt::yaml_caps_to_lower(eq->str_type);
					wearing += ":";
					wearing += just_yaml_file(eq->feed_file().data());
					wearing += ":";
					wearing += std::to_string(i);
					wearing += "|";
				}
				std::string key = "",player_key = "player|";
				player_key += player->name().c_str();
				key = player_key + "|wearing";

				DBG("key: '" << key << "' value:'" << wearing << "'");

				LMDBSET(key,wearing);
				for(auto w : player->real_carrying()) {
					if(!w) {
						DBG("Invalid carrying");
						continue;
					}
					DBG("valid carrying. str_type: '" << w->str_type << "', feed_file: '" << w->feed_file() << "'");
					carrying += txt::yaml_caps_to_lower(w->str_type);
					carrying += ":";
					carrying += just_yaml_file(w->feed_file().data());
					carrying += "|";
				}
				DBG("carrying value");
				DBG(carrying);
				key = player_key + "|carrying";
				DBG("key: '" << key << "' value:'" << carrying << "'");
				LMDBSET(key,carrying);
				LMDBCOMMIT();
				return 0;
#undef DBG
			}
			int16_t feed_player(player_ptr_t& player) {
				//mods::orm::load_player_rifle_attachments(player);
#ifdef __MENTOC_SHOW_ORM_INVENTORY_FLUSH_OUTPUT__
#define DBG(a) std::cerr << "[mods::orm::inventory::feed_player][LINE:" << __LINE__ << "][FILE:" << __FILE__ << "]->'" << a << "'\n";
#else
#define DBG(a) ;;
#endif
				DBG("entrance");
				LMDBRENEW();
				std::string player_key = std::string("player|") + player->name().c_str();
				std::string wearing = LMDBGET(player_key + "|wearing");
				std::string carrying = LMDBGET(player_key + "|carrying");
				LMDBCOMMIT();
				DBG("wearing: '" << wearing << "'");
				DBG("carrying: '" << carrying << "'");
				for(auto& yaml_file : txt::explode('|',wearing)) {
					DBG("exploding: '" << yaml_file << "'");
					auto vec = txt::explode(':',yaml_file);
					for(auto v : vec) {
						DBG("EXPLODED: v:'" << v << "'");
					}
					if(vec.size() < 3) {
						DBG("invalid size of string: '" << yaml_file << "'");
						log("SYSERR: malformed type:yaml_file combination in lmdb. Not printing for security purposes.");
						continue;
					}
					std::string file = "objects/" + vec[0] + "/" + vec[1];
					DBG("file: '" << file << "'");
					if(!txt::yaml_file_path_is_sane(file)) {
						DBG("not a sane ymal file path.. string: '" << file << "'");
						log("SYSERR: malformedk yaml file used as path: '%s'",file.c_str());
						continue;
					}
					std::cerr << "[mods::inventory::yaml::feed_player][WEARING] giving player: '" << file << "'\n";
					int i_type = txt::yaml_string_to_int(vec[0]);
					auto obj = create_object(i_type,vec[1]);
					int wear = mods::util::stoi(vec[2]).value_or(-1);
					DBG("equipping object");
					player->equip(std::move(obj),wear);
				}
				for(auto& yaml_file : txt::explode('|',carrying)) {
					DBG("exploding: '" << yaml_file << "'");
					auto vec = txt::explode(':',yaml_file);
					for(auto v : vec) {
						DBG("EXPLODED: v:'" << v << "'");
					}
					if(vec.size() < 2) {
						DBG("invalid size of string: '" << yaml_file << "'");
						log("SYSERR: malformed CARRYING type:yaml_file combination in lmdb. Not printing for security purposes.");
						continue;
					}
					std::string file = "objects/" + vec[0] + "/" + vec[1];
					if(!txt::yaml_file_path_is_sane(file)) {
						DBG("not a sane ymal file path.. string: '" << file << "'");
						log("SYSERR: malformed CARRYING yaml file used as path: '%s'",file.c_str());
						continue;
					}
					std::cerr << "[mods::inventory::yaml::feed_player][CARRYING] giving player: '" << file << "'\n";
					int i_type = txt::yaml_string_to_int(vec[0]);
					auto obj = create_object(i_type,vec[1]);
					DBG("carrying object");
					player->carry(std::move(obj));
				}
				return 0;
			}
		};
		/**
		 * @brief flush player's equipment and carrying to lmdb
		 * @param player
		 *
		 * @return
		 */
		int16_t flush_player(player_ptr_t& player) {
			//mods::orm::flush_player_rifle_attachments(player);
			std::vector<uint64_t> wearing;
			wearing.resize(NUM_WEARS);
			for(std::size_t i = 0; i < NUM_WEARS; i++) {
				auto eq = player->equipment(i);
				wearing.push_back(eq ? eq->db_id() : 0);
			}
			std::vector<uint64_t> carrying;
			for(auto& w : player->cd()->m_carrying) {
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
		/**
		 * @brief gets player's equipment and carrying from lmdb and loads player
		 *
		 * @param player
		 *
		 * @return
		 */
		int16_t feed_player(player_ptr_t& player) {
			//mods::orm::load_player_rifle_attachments(player);
			std::vector<uint64_t> wearing,carrying;
			auto wkey = "player|" + std::string(player->name().c_str()) + "|wearing";
			auto ckey = "player|" + std::string(player->name().c_str()) + "|carrying";
			auto wkey_size = "player|" + std::string(player->name().c_str()) + "|wearing|size";
			auto ckey_size = "player|" + std::string(player->name().c_str()) + "|carrying|size";

			std::size_t ckey_size_value,wkey_size_value;
			LMDBRENEW();
			LMDBVGET(ckey_size.data(),ckey_size.length()+1,(void*)&ckey_size_value);
			LMDBVGET(wkey_size.data(),wkey_size.length()+1,(void*)&wkey_size_value);
			d("[wearing.resize]: " << wkey_size << "\n");
			wearing.resize(wkey_size_value);
			LMDBVGET(wkey.data(),wkey.length()+1,(void*)&wearing[0]);
			d("[carrying.resize]: " << ckey_size << "\n");
			carrying.resize(ckey_size_value);
			LMDBVGET(ckey.data(),ckey.length()+1,(void*)&carrying[0]);
			LMDBCOMMIT();
			return 0;
		}
		/**
		 * @brief called when a player wears an item. saves to lmdb and flushes
		 *
		 * @param player_db_id
		 * @param object_db_id
		 * @param object_type_id
		 * @param position
		 */
		void add_player_wear(uint64_t player_db_id, uint64_t object_db_id, uint8_t object_type_id, uint8_t position) {
#ifndef __MENTOC_USE_DEFAULT_INVENTORY_FLUSH__
			return;
#endif
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
			d("[add_player_wear][uf_key:'" << uf_key << "'][uf_val:'" << uf_val << "']\n");
			LMDBSET(uf_key,uf_val);
		}
		/**
		 * @brief called when player removes a worn item. flushes to lmdb
		 *
		 * @param player_db_id
		 * @param position
		 */
		void remove_player_wear(uint64_t player_db_id, uint8_t position) {
#ifndef __MENTOC_USE_DEFAULT_INVENTORY_FLUSH__
			return;
#endif
			player_wear_key_t id;
			bcopy("wear",(void*)&id.type,sizeof(id.type));
			id.player_db_id = player_db_id;
			id.position = position;
			LMDBNDEL((void*)&id,sizeof(id));
			auto uf = std::string("wear|") + std::to_string(player_db_id) + "|" + std::to_string(position);
			d("[remove_player_wear][uf_key:'" << uf << "']\n");

			LMDBDEL(uf);
		}
		/**
		 * @brief called when player adds an item to inv. flushes to lmdb
		 *
		 * @param player_db_id
		 * @param object_db_id
		 * @param obj_type
		 */
		void add_player_inventory(uint64_t player_db_id, uint64_t object_db_id, uint16_t obj_type) {
#ifndef __MENTOC_USE_DEFAULT_INVENTORY_FLUSH__
			return;
#endif
			player_inventory_data key_data;
			bcopy("inv|",(void*)&key_data.type,sizeof(key_data.type));
			key_data.player_db_id = player_db_id;
			key_data.value_db_id = object_db_id;
			LMDBNSET((void*)&key_data,sizeof(key_data),(void*)&obj_type,sizeof(obj_type));
			auto uf_key = std::string("inv|") + std::to_string(player_db_id) + "|" + std::to_string(object_db_id);
			auto uf_val = std::to_string(obj_type);
			d("[add_player_inventory][uf_key:'" << uf_key << "'][uf_val:'" << uf_val << "']\n");
			LMDBSET(uf_key,uf_val);
		}
		/**
		 * @brief called when player removes item from inv. flushes to lmdb
		 *
		 * @param player_db_id
		 * @param object_db_id
		 */
		void remove_player_inventory(uint64_t player_db_id, uint64_t object_db_id) {
#ifndef __MENTOC_USE_DEFAULT_INVENTORY_FLUSH__
			return;
#endif
			LMDBRENEW();
			player_inventory_data key_data;
			bcopy("inv|",(void*)&key_data.type,sizeof(key_data.type));
			key_data.player_db_id = player_db_id;
			key_data.value_db_id = object_db_id;
			LMDBNDEL((void*)&key_data,sizeof(key_data));

			auto uf_key = std::string("inv|") + std::to_string(player_db_id) + "|" + std::to_string(object_db_id);
			d("[remove_player_inventory][uf_key:'" << uf_key << "']\n");
			LMDBDEL(uf_key);
		}
	};

	int16_t flush_player(player_ptr_t& player) {
#ifdef __MENTOC_USE_SQL_FLUSH_PLAYER__
		return sql::flush_player(player);
#else
		return lmdb::yaml::flush_player(player);
#endif
	}
	int16_t feed_player(player_ptr_t& player) {
#ifdef __MENTOC_USE_SQL_FLUSH_PLAYER__
		return sql::feed_player(player);
#else
		return lmdb::yaml::feed_player(player);
#endif
	}
	int16_t flush_player_by_uuid(uuid_t uuid) {
		auto p = ptr_by_uuid(uuid);
		return flush_player(p);
	}
	int16_t feed_player_by_uuid(uuid_t uuid) {
		auto p = ptr_by_uuid(uuid);
		return feed_player(p);
	}
};

