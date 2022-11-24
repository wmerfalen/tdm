#define __MENTOC_SHOW_SQL_DEBUG_OUTPUT__
#include "inventory.hpp"
#include <vector>
#include <map>
#include <string>
#include "../weapons/shotgun-sasg12.hpp"
#include "../weapons/smg-mp5.hpp"
#include "../weapons/pistol-czp10.hpp"
#include "../../globals.hpp"
#include "rifle-attachment.hpp"

//#define __MENTOC_SHOW_INVENTORY_DEBUG_OUTPUT__
#ifdef __MENTOC_SHOW_INVENTORY_DEBUG_OUTPUT__
	#define m_debug(a) std::cerr << "[mods::orm::inventory[file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
	#define m_error(a) std::cerr << "[mods::orm::inventory[file:" << __FILE__ << "][line:" << __LINE__ << "][ERROR]->" << a << "\n";
#else
	#define m_debug(a)
	#define m_error(a)
#endif

extern obj_ptr_t read_object_ptr(obj_vnum nr, int type);
extern obj_ptr_t blank_object();
extern void obj_ptr_to_char(obj_ptr_t  object, player_ptr_t player);
extern obj_ptr_t create_object_from_index(std::size_t proto_index);
namespace mods::orm::inventory {

	static constexpr const char* PLACEHOLDER = "0";
	static constexpr const char* FORGED = "1";
	static constexpr const char* YAML = "2";
	static constexpr const char* RIFLE_ATTACHMENT = "3";
	static constexpr uint8_t I_PLACEHOLDER = 0;
	static constexpr uint8_t I_FORGED = 1;
	static constexpr uint8_t I_YAML= 2;
	static constexpr uint8_t I_RIFLE_ATTACHMENT = 3;
	bool is_ammo(obj_ptr_t& eq) {
		return eq->type == ITEM_CONSUMABLE && eq->has_consumable() && eq->consumable()->attributes->ammo_type.compare("NONE") != 0;
	}
	bool is_rifle(obj_ptr_t& eq) {
		return eq->type == ITEM_RIFLE && eq->has_rifle();
	}
	std::map<std::string,std::string> export_object(obj_ptr_t eq,const uint64_t& db_id,const std::size_t& wear_position,bool inventory) {
		std::map<std::string,std::string> mapped_values;
		mapped_values["po_player_id"] = std::to_string(db_id);
		mapped_values["po_type"] = std::to_string(eq->type);
		m_debug("export object called");
		if(is_ammo(eq)) {
			m_debug("found ammo consumable. saving capacity...");
			mapped_values["po_ammunition"] = std::to_string(eq->consumable()->attributes->capacity);
		}
		if(eq->type == ITEM_RIFLE) {
			m_debug("Found ITEM RIFLE type. saving rifle instance ammo..." << eq->rifle_instance->ammo);
			mapped_values["po_ammunition"] = std::to_string(eq->rifle_instance->ammo);
		}
		if(eq->forged) {
			mapped_values["po_type_id"] = std::to_string(eq->db_id());
			mapped_values["po_load_type"] = FORGED;
		} else {
			mapped_values["po_load_type"] = YAML;
			auto rifle_attachment = mods::rifle_attachments::by_uuid(eq->uuid);
			if(rifle_attachment) {
				mapped_values["po_yaml"] = rifle_attachment->export_objects();
				mapped_values["po_load_type"] = RIFLE_ATTACHMENT;
			} else {
				mapped_values["po_yaml"] = eq->feed_file();
			}
		}
		if(inventory) {
			mapped_values["po_in_inventory"] = "1";
			mapped_values["po_wear_position"] = "";
		} else {
			mapped_values["po_in_inventory"] = "0";
			mapped_values["po_wear_position"] = std::to_string(wear_position);
		}
		return mapped_values;
	}

	obj_data_ptr_t dynamic_fetch(mentoc_pqxx_result_t row,player_ptr_t& player) {
		m_debug("dynamic fetch entry");
		switch(row["po_load_type"].as<int>()) {
			case I_FORGED: {
					m_debug("dynamic fetching FORGED: " << row["id"].as<int>());
					auto obj = create_object(ITEM_RIFLE,CAT("rifle|pkid:",row["po_type_id"].as<int>()));
					obj->forged = true;
					if(!row["po_ammunition"].is_null()) {
						m_debug("po_ammunition set for item: " << row["id"].as<int>());
						obj->rifle_instance->ammo = row["po_ammunition"].as<int>();
					}
					return std::move(obj);
				}
			case I_RIFLE_ATTACHMENT: {
					auto obj = mods::rifle_attachments::make(row["po_yaml"].c_str());
					obj->set_owner_uuid(player->uuid());
					return obj->base_object;
				}
			case I_YAML: {
					if(row["po_yaml"].is_null()) {
						return nullptr;
					}
					auto obj = create_object(row["po_type"].as<int>(),row["po_yaml"].c_str());
					if(!row["po_ammunition"].is_null() && is_ammo(obj)) {
						m_debug("po_ammunition set for item: " << row["id"].as<int>());
						obj->consumable()->attributes->capacity = row["po_ammunition"].as<int>();
						m_debug("Set ammo successfully from database for ammunition");
					}
					if(!row["po_ammunition"].is_null() && is_rifle(obj)) {
						m_debug("po_ammunition set for rifle: " << row["id"].as<int>());
						obj->rifle_instance->ammo = row["po_ammunition"].as<int>();
					}
					return std::move(obj);
				}
			default:
				return nullptr;
		}
		return nullptr;
	}



	std::tuple<bool,std::string> unequip(const uint64_t& player_db_id, obj_ptr_t& object,const std::size_t& position) {
		try {
			auto obj = export_object(object,player_db_id,position,false);
			auto del_txn = txn();
			sql_compositor comp("player_object",&del_txn);
			auto builder = comp
			    .del()
			    .from("player_object")
			    .where("po_player_id",player_db_id)
			    .op_and("po_wear_position",position);
			mods::pq::exec(del_txn,builder.sql());
			mods::pq::commit(del_txn);
			return {true,""};
		} catch(std::exception& e) {
			REPORT_DB_ISSUE(": error unequipping by pkid: '",e.what());
			return {false,e.what()};
		}
	}

	std::tuple<bool,std::string> equip(const uint64_t& player_db_id,const std::size_t& position,obj_ptr_t& object) {
		try {
			auto insert_transaction = txn();
			sql_compositor comp("player_object",&insert_transaction);
			auto up_sql = comp
			    .insert()
			    .into("player_object")
			    .values(export_object(object,player_db_id,position,false))
			    .returning("id")
			    .sql();
			auto record = mods::pq::exec(insert_transaction,up_sql);
			mods::pq::commit(insert_transaction);
			return {true,""};
		} catch(std::exception& e) {
			REPORT_DB_ISSUE(": error inserting player_object row: '",e.what());
			return {false,e.what()};
		}
	}
	std::tuple<bool,std::string> carry(const uint64_t& player_db_id,obj_ptr_t& object) {
		try {
			auto insert_transaction = txn();
			sql_compositor comp("player_object",&insert_transaction);
			auto up_sql = comp
			    .insert()
			    .into("player_object")
			    .values(export_object(object,player_db_id,0,true))
			    .returning("id")
			    .sql();
			auto record = mods::pq::exec(insert_transaction,up_sql);
			mods::pq::commit(insert_transaction);
			return {true,""};
		} catch(std::exception& e) {
			REPORT_DB_ISSUE(": error inserting player_object row: '",e.what());
			return {false,e.what()};
		}
	}
	std::tuple<bool,std::string> uncarry(const uint64_t& player_db_id,obj_ptr_t& object) {
		try {
			auto sel_txn = txn();
			sql_compositor comp("player_object",&sel_txn);
			std::string po_load_type = YAML;
			auto builder = comp
			    .select("id")
			    .from("player_object")
			    .where("po_player_id",std::to_string(player_db_id))
			    .op_and("po_in_inventory","1");
			if(object->forged) {
				builder.op_and("po_load_type",FORGED)
				.op_and("po_type_id",object->db_id());
			} else {
				auto rifle_attachment = mods::rifle_attachments::by_uuid(object->uuid);
				if(rifle_attachment) {
					builder
					.op_and("po_load_type",RIFLE_ATTACHMENT)
					.op_and("po_yaml",rifle_attachment->export_objects());
				} else {
					builder
					.op_and("po_load_type",YAML)
					.op_and("po_yaml",object->feed_file());
				}
			}
			builder.limit(1);
			auto record = mods::pq::exec(sel_txn,builder.sql());
			mods::pq::commit(sel_txn);
			if(record.size()) {
				auto del_txn = txn();
				sql_compositor comp("player_object",&del_txn);
				std::string po_load_type = YAML;
				auto builder = comp
				    .del()
				    .from("player_object")
				    .where("id",record[0]["id"].c_str());
				mods::pq::exec(del_txn,builder.sql());
				mods::pq::commit(del_txn);
			}
			return {true,""};
		} catch(std::exception& e) {
			REPORT_DB_ISSUE(": error deleting player's existing player_object row: '",e.what());
			return {false,e.what()};
		}
		d("done");

	}


	int16_t feed_player(player_ptr_t& player) {
		for(auto obj : player->vcarrying()) {
			player->uncarry(optr(obj),false);
		}
		for(auto i=0; i < NUM_WEARS; i++) {
			player->unequip(i,false);
		}
		try {
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
				switch(row["po_load_type"].as<int>()) {
					default:
						log("Warning: weird po_load_type encountered (%d). Ignoring...",row["po_load_type"].as<int>());
						continue;
					case I_PLACEHOLDER:
						continue;
					case I_FORGED:
					case I_YAML:
					case I_RIFLE_ATTACHMENT:
						break;
				}
				obj = nullptr;
				m_debug("dynamic fetch for: " << row["po_yaml"].c_str());
				obj = dynamic_fetch(row,player);
				if(!obj) {
					log("Failed to load user object: player[%s] po_id[%d]", player->name().c_str(),row["po_id"].as<int>());
					continue;
				}
				if(row["po_in_inventory"].as<int>() == 1) {
					player->carry(std::move(obj),false);
					continue;
				}
				if(row["po_wear_position"].is_null() == false) {
					m_debug("equippping " << row["po_yaml"].c_str() << "to " << row["po_wear_position"].as<int>());
					player->equip(std::move(obj),row["po_wear_position"].as<int>(),false);
					continue;
				}
			}
			d("[sql][feed_player] size: " << player_record.size() << "\n");
			return player_record.size();
		} catch(std::exception& e) {
			REPORT_DB_ISSUE(": error loading character by pkid: '",e.what());
			return -2;
		}
	}//end feed_player

	std::tuple<bool,std::string> user_reloaded_position(player_ptr_t& player,int position) {
		try {
			auto update_txn = txn();
			sql_compositor comp("player_object",&update_txn);
			auto weapon = player->primary();
			auto obj = export_object(weapon,player->db_id(),position,false);
			auto ammo_count = obj["po_ammunition"];
			obj.erase("po_ammunition");
			auto player_sql = comp.update("player_object")
			.set({
				{"po_ammunition", ammo_count},
			})
			.where("po_wear_position",obj["po_wear_position"])
			.op_and("po_player_id",obj["po_player_id"])
			.op_and("po_yaml",obj["po_yaml"])
			.op_and("po_type",obj["po_type"])
			.op_and("po_load_type",obj["po_load_type"])
			.sql();
			m_debug("Running sql: '" << player_sql << "'");
			auto player_record = mods::pq::exec(update_txn,player_sql);
			mods::pq::commit(update_txn);
			return {1,"saved"};
		} catch(std::exception& e) {
			REPORT_DB_ISSUE(": error saving reload: '",e.what());
			return {0,CAT("Update failed: '",e.what(),"'")};
		}
	}
	std::tuple<bool,std::string> user_reloaded_primary(player_ptr_t& player) {
		return user_reloaded_position(player,WEAR_PRIMARY);
	}

	std::tuple<bool,std::string> user_reloaded_secondary(player_ptr_t& player) {
		return user_reloaded_position(player,WEAR_SECONDARY);
	}

};

#undef m_debug
#undef m_error
