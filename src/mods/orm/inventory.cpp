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
#define __MENTOC_SHOW_ORM_INVENTORY_FLUSH_OUTPUT__
#ifdef __MENTOC_SHOW_ORM_INVENTORY_FLUSH_OUTPUT__
#define DBG(a) std::cerr << "[mods::orm::inventory::flush_player][LINE:" << __LINE__ << "][FILE:" << __FILE__ << "]->'" << a << "'\n";
#else
#define DBG(a) ;;
#endif

	obj_data_ptr_t dynamic_fetch(mentoc_pqxx_result_t row) {
		std::string in_type = row["po_load_type"].c_str();
		if(in_type.compare("forged") == 0) {
			auto obj = create_object(ITEM_RIFLE,CAT("rifle|pkid:",row["po_type_id"].as<int>()));
			obj->forged = true;
			return std::move(obj);
		}
		if(in_type.compare("yaml") == 0) {
			return std::move(create_object(mods::util::yaml_string_to_int(row["po_type"].c_str()),row["po_yaml"].c_str()));
		}
		return nullptr;
	}
	namespace sql {
		int16_t feed_player(player_ptr_t& player) {
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
						obj = dynamic_fetch(row);
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
					d("[sql][feed_player] size: " << player_record.size() << "\n");
					return player_record.size();
				} catch(std::exception& e) {
					REPORT_DB_ISSUE(": error loading character by pkid: '",e.what());
					return -2;
				}
			}
		}//end feed_player
		std::map<std::string,std::string> export_object(obj_ptr_t eq,player_ptr_t& player,int wear_position,bool inventory) {
			std::map<std::string,std::string> mapped_values;
			mapped_values["po_player_id"] = std::to_string(player->db_id());
			mapped_values["po_type"] = eq->str_type;
			if(eq->forged) {
				mapped_values["po_type_id"] = std::to_string(eq->db_id());
				mapped_values["po_load_type"] = "forged";
			} else {
				mapped_values["po_load_type"] = "yaml";
				mapped_values["po_yaml"] = eq->feed_file();
			}
			if(inventory) {
				mapped_values["po_in_inventory"] = "1";
			} else {
				mapped_values["po_in_inventory"] = "0";
				mapped_values["po_wear_position"] = std::to_string(wear_position);
			}
			return mapped_values;
		}

		/**
		 * Must:
		 * 1) store forge engine rifles
		 * 2) store forge engine armor
		 * 3) store forge engine melee weapons
		 * 4) store rifle attachments
		 * 5) store deep object parser schemas
		 */
		int16_t flush_player(player_ptr_t& player) {
			if(player->db_id() == 0) {
				log("SYSERR: couldn't grab player's pkid: '%s' -- not able to flush player's inventory to db", player->name().c_str());
				return -1;
			}
			mods::orm::flush_player_rifle_attachments(player);
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
				auto mapped_values = export_object(eq,player,i,false);
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
			for(auto& eq : player->vcarrying()) {
				auto mapped_values = export_object(optr(eq),player,0,true);
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


	int16_t flush_player(player_ptr_t& player) {
		return sql::flush_player(player);
	}
	int16_t feed_player(player_ptr_t& player) {
		return sql::feed_player(player);
	}
	int16_t flush_player_by_uuid(uuid_t uuid) {
		auto p = ptr_by_uuid(uuid);
		return sql::flush_player(p);
	}
	int16_t feed_player_by_uuid(uuid_t uuid) {
		auto p = ptr_by_uuid(uuid);
		return sql::feed_player(p);
	}
};

