#include "db-load.hpp"
#include "../classes/includes.hpp"
#include "../replenish.hpp"
#include "../affects.hpp"
#include "../orm/inventory.hpp"
#include "../db.hpp"
#include "../orm/player-base-ability.hpp"
#include "../pq.hpp"
#include "../sql.hpp"
#include "../levels.hpp"
#include "../orm/player-skill-points.hpp"
#include "../orm/player-skill-usage.hpp"
#include "../orm/skill-trees.hpp"
#include "../orm/rifle-attachment.hpp"

#ifdef __MENTOC_SHOW_MODS_PLAYERS_DB_LOAD_DEBUG_OUTPUT__
#define m_debug(a) std::cerr << "[mods::players::db_load][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#define m_crit(a) std::cerr << red_str("[CRITICAL]") << "mods::players::db_load][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#else
#define m_debug(a)
#define m_crit(a)
#endif

extern void write_aliases(char_data *ch);
extern void read_aliases(char_data *ch);

namespace mods::players::db_load {
	static reporter_t report_function;
	static bool reporter_function_set = false;
	void game_entry(player_ptr_t& player) {
		mods::orm::load_player_rifle_attachments(player);
		mods::orm::inventory::feed_player(player);
		/** A nasty little hack to level up the player if they somehow
		 * have a surplus of exp that hasn't been used towards
		 * leveling up
		 */
		mods::levels::gain_exp(player,0);
		player->clear_all_affected();
	}

	void sync_player_with_class_skills(const uint64_t& player_id,const std::string& player_class) {
		mods::orm::skill_trees tree;
		std::string pc = "";
		for(auto ch : player_class) {
			pc += std::tolower(ch);
		}
		auto r = tree.load_by_class(pc);
		mods::orm::player_skill_points ps;
		ps.rows.clear();
		ps.load_by_player(player_id);
		bool found = false;
		std::map<uint32_t,uint16_t> missing;
		for(const auto& row : tree.rows) {
			found = false;
			for(const auto& prow : ps.rows) {
				if(prow.skill_id == row.id) {
					m_debug(green_str("Found skill id: ") << prow.skill_id);
					found = true;
					break;
				}
			}
			if(!found) {
				m_crit(red_str("Missing player skill id for player: ") << player_id << ", skill: '" << row.skill_name << "'");
				missing[row.id] = 0;
			}
		}
		if(missing.size()) {
			ps.rows.clear();
			m_crit(red_str("Player has missing skill tree members. Populating..."));
			ps.populate(player_id,missing);
			ps.save();
			m_debug(green_str("Remedied."));
		}
		mods::orm::player_skill_usage_upkeep(player_id,player_class);
	}

	void extract_character(player_ptr_t& player_ptr) {
		mods::players::db_load::save_from(player_ptr,save_from_t::EXTRACTION);
	}
	void save_prefs(player_ptr_t& player_ptr) {
		mods::players::db_load::save_from(player_ptr,save_from_t::PREFS);
	}
	void save_from(player_ptr_t& player_ptr,save_from_t from) {
		if(from != save_from_t::EXTRACTION) {
			mods::orm::flush_player_rifle_attachments(player_ptr);
		}
		auto ch = player_ptr->cd();
		std::map<std::string,std::string> values;
		values["player_affection_plr_bitvector"] = std::to_string(player_ptr->get_affected_plr());
		values["player_affection_bitvector"] = std::to_string(player_ptr->get_affected());

		if(values["player_affection_plr_bitvector"].length() == 0) {
			values.erase("player_affection_plr_bitvector");
		}
		if(values["player_affection_bitvector"].length() == 0) {
			values.erase("player_affection_bitvector");
		}

		values["player_name"] = player_ptr->name().c_str();
		values["player_short_description"] = std::to_string(ch->player.short_descr);
		values["player_long_description"] = std::to_string(ch->player.long_descr);
		values["player_action_bitvector"] = (std::to_string(ch->char_specials.saved.act));
		values["player_ability_strength"] = (std::to_string(ch->real_abils.str));
		values["player_ability_strength_add"] = (std::to_string(ch->real_abils.str_add));
		values["player_ability_intelligence"] = (std::to_string(ch->real_abils.intel));
		values["player_ability_wisdom"] = (std::to_string(ch->real_abils.wis));
		values["player_ability_dexterity"] = (std::to_string(ch->real_abils.dex));
		values["player_ability_constitution"] = (std::to_string(ch->real_abils.con));
		values["player_ability_charisma"] = (std::to_string(ch->real_abils.cha));
		values["player_ability_alignment"] = (std::to_string(ch->char_specials.saved.alignment));
		values["player_attack_type"] = (std::to_string(ch->real_abils.con));
		values["player_ability_constitution"] = (std::to_string(ch->real_abils.con));
		values["player_max_mana"] = (std::to_string(player_ptr->max_mana()));
		values["player_max_move"] = (std::to_string(player_ptr->max_move()));
		values["player_gold"] = (std::to_string(player_ptr->gold()));
		values["player_exp"] = (std::to_string(player_ptr->exp()));
		values["player_sex"] = player_ptr->sex() == SEX_FEMALE ? std::string("F") : std::string("M");
		values["player_hitpoints"] = (std::to_string(player_ptr->hp()));
		values["player_max_hitpoints"] = (std::to_string(player_ptr->max_hp()));
		values["player_mana"] = (std::to_string(player_ptr->mana()));
		values["player_move"] = (std::to_string(player_ptr->move()));
		values["player_damroll"] = (std::to_string(player_ptr->damroll()));
		values["player_weight"] = (std::to_string(player_ptr->weight()));
		values["player_height"] = (std::to_string(player_ptr->height()));
		values["player_class"] = std::to_string(player_ptr->get_class());
		values["player_title"] = std::to_string(player_ptr->title());
		values["player_hometown"] = (std::to_string(player_ptr->hometown()));
		values["player_damnodice"] = std::string("0");
		values["player_damsizedice"] = std::string("0");
		values["player_attack_type"] = std::string("0");
		values["player_type"] = std::string("PC");
		values["player_alignment"] = std::to_string(
		                                 ch->char_specials.saved.alignment);
		values["player_level"] = std::to_string(player_ptr->level());
		values["player_hitroll"] = std::to_string(player_ptr->cd()->points.hitroll);
		values["player_armor"] = std::to_string(player_ptr->cd()->points.armor);
		values["player_preferences"] = std::to_string(player_ptr->get_prefs());
		values["player_practice_sessions"] = std::to_string(player_ptr->practice_sessions());
		try {
			auto up_txn = txn();
			mods::sql::compositor comp("player",&up_txn);
			auto up_sql = comp
			              .update("player")
			              .set(values)
			              .where("id","=",std::to_string(player_ptr->db_id()))
			              .sql();
			mods::pq::exec(up_txn,up_sql);
			mods::pq::commit(up_txn);
		} catch(std::exception& e) {
			log(CAT("SYSERR:Failed saving player!:", e.what()).c_str());
		}
		values.clear();

		switch(player_ptr->get_class()) {
			default:
				break;
			case player_class_t::GHOST:
				player_ptr->ghost()->save();
				break;
		}
		mods::orm::player_base_ability pba;
		auto status = pba.save_by_player(player_ptr);
		if(0 != status) {
			m_crit(red_str("Warning: couldn't save player_ptr's base abilities...") << "status: " << status << " for player_ptr:'" << player_ptr->name().c_str() << "'");
		}
		if(player_ptr->position() == CON_PLAYING) {
			player_ptr->sendln("Your character has been saved.");
		}
	}

	void set_reporter_lambda(reporter_t f) {
		report_function = f;
		reporter_function_set = true;
	}
	void report(int64_t code, std::string_view msg) {
		if(!reporter_function_set) {
			m_debug("[fallback reporter]: " << red_str(CAT("code:",(code))) <<
			        " message: '" << red_str(msg.data()) << "'");
			return;
		}
		report_function(code,msg);
	}
	void set_class(player_ptr_t& player, player_class_t p_class) {
		m_debug(green_str("set_class called for player uuid:") << player->uuid());
		player->set_class(p_class);
		switch(p_class) {
			case MARINE:
				player->set_marine(mods::classes::create_marine(player));
				mods::replenish::register_marine(player->uuid());
				break;
			case BREACHER:
				player->set_breacher(mods::classes::create_breacher(player));
				mods::replenish::register_breacher(player->uuid());
				break;
			case GHOST:
				player->set_ghost(mods::classes::create_ghost(player));
				mods::replenish::register_ghost(player->uuid());
				break;
			case CONTAGION:
				player->set_contagion(mods::classes::create_contagion(player));
				mods::replenish::register_contagion(player->uuid());
				break;
			default:
				report(p_class,"Unable to create class. unknown class");
				break;
		}
	}
	void load_skill_points(player_ptr_t& player) {
		//
	}
	void load_base_abilities(player_ptr_t& player) {
		mods::orm::player_base_ability pba;
		pba.feed_player(player);
	}

	int save_player_password(player_ptr_t& player,std::string_view password) {
		try {
			std::map<std::string,std::string> values;
			values["player_password"] = password.data();
			auto up_txn = txn();
			mods::sql::compositor comp("player",&up_txn);
			auto up_sql = comp
			              .update("player")
			              .set_with_password(values, "player_password")
			              .where("id","=",std::to_string(player->db_id()))
			              .sql();
			mods::pq::exec(up_txn,up_sql);
			mods::pq::commit(up_txn);
			return 0;
		} catch(std::exception& e) {
			auto msg = CAT("Player:'",player->db_id(),"/name:'",player->name().c_str(),"'.. Unable to save player password!:'",e.what(),"'");
			report(-1,msg);
			log(CAT("SYSERR: ",msg).c_str());
			return -1;
		}
	}

	int16_t save_new_char(player_ptr_t& player) {
		try {
			std::map<std::string,std::string> values;
			mods::db::lmdb_export_char(player,values);
			auto insert_transaction = txn();
			mods::sql::compositor comp("player",&insert_transaction);
			auto up_sql = comp
			              .insert()
			              .into("player")
			              .values_with_password(values, "player_password")
			              .sql();
			values.clear();
			mods::pq::exec(insert_transaction,up_sql);
			mods::pq::commit(insert_transaction);
			return 0;
		} catch(std::exception& e) {
			REPORT_DB_ISSUE("error inserting new character",e.what());
			return -1;
		}
	}

	int16_t load_char_pkid(player_ptr_t& player) {
		try {
			auto select_transaction = txn();
			mods::sql::compositor comp("player",&select_transaction);
			auto player_sql = comp.select("id")
			                  .from("player")
			                  .where("player_name","=",player->name())
			                  .sql();
			auto player_record = mods::pq::exec(select_transaction,player_sql);
			if(player_record.size()) {
				player->set_db_id(player_record[0]["id"].as<int>(0));
				return 0;
			}
			log("SYSERR: couldn't grab player's pkid: '%s'",player->name().c_str());
			return -1;
		} catch(std::exception& e) {
			REPORT_DB_ISSUE("error loading character by pkid",e.what());
			return -2;
		}
	}
	int16_t delete_char_by_name(std::string_view user) {
		try {
			std::map<std::string,std::string> values;
			auto delete_txn = txn();
			mods::sql::compositor comp("player",&delete_txn);
			auto del_sql = comp
			               .del()
			               .from("player")
			               .where("player_name","=",user.data())
			               .sql();
			mods::pq::exec(delete_txn,del_sql);
			mods::pq::commit(delete_txn);
			return 0;
		} catch(std::exception& e) {
			REPORT_DB_ISSUE("error deleting character",e.what());
			return -1;
		}
	}
	int16_t delete_char(player_ptr_t& player) {
		try {
			std::map<std::string,std::string> values;
			mods::db::lmdb_export_char(player,values);
			auto delete_txn = txn();
			mods::sql::compositor comp("player",&delete_txn);
			auto del_sql = comp
			               .del()
			               .from("player")
			               .where("player_name","=",values["player_name"])
			               .sql();
			mods::pq::exec(delete_txn,del_sql);
			mods::pq::commit(delete_txn);
			return 0;
		} catch(std::exception& e) {
			REPORT_DB_ISSUE("error deleting character",e.what());
			return -1;
		}
	}
	void load_aliases(player_ptr_t& player) {
	}
};
