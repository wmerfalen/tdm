#include "db-load.hpp"
#include "../classes/includes.hpp"
#include "../replenish.hpp"
#include "../orm/inventory.hpp"
#include "../db.hpp"
#include "../orm/player-base-ability.hpp"
#include "../pq.hpp"
#include "../sql.hpp"

#define __MENTOC_SHOW_MODS_PLAYERS_DB_LOAD_DEBUG_OUTPUT__
#ifdef __MENTOC_SHOW_MODS_PLAYERS_DB_LOAD_DEBUG_OUTPUT__
	#define m_debug(a) std::cerr << "[mods::players::db_load][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
	#define m_crit(a) std::cerr << red_str("[CRITICAL]") << "mods::players::db_load][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#else
	#define m_debug(a)
	#define m_crit(a)
#endif


namespace mods::players::db_load {
	static reporter_t report_function;
	static bool reporter_function_set = false;
	void save(player_ptr_t& player_ptr){
		auto ch = player_ptr->cd();
		std::map<std::string,std::string> values;
		values["player_affection_plr_bitvector"] = std::to_string(player_ptr->get_affected_plr());
		values["player_affection_bitvector"] = std::to_string(player_ptr->get_affected());

		if(values["player_affection_plr_bitvector"].length() == 0){
			values.erase("player_affection_plr_bitvector");
		}
		if(values["player_affection_bitvector"].length() == 0){
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
		values["player_max_hitpoints"] = (std::to_string(player_ptr->max_hp()));
		values["player_max_mana"] = (std::to_string(player_ptr->max_mana()));
		values["player_max_move"] = (std::to_string(player_ptr->max_move()));
		values["player_gold"] = (std::to_string(player_ptr->gold()));
		values["player_exp"] = (std::to_string(player_ptr->exp()));
		values["player_sex"] = player_ptr->sex() == SEX_FEMALE ? std::string("F") : std::string("M");
		values["player_hitpoints"] = (std::to_string(player_ptr->hp()));
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
		try{
			auto up_txn = txn();
			mods::sql::compositor comp("player",&up_txn);
			auto up_sql = comp
				.update("player")
				.set(values)
				.where("id","=",std::to_string(player_ptr->db_id()))
				.sql();
			mods::pq::exec(up_txn,up_sql);
			mods::pq::commit(up_txn);
		}catch(std::exception& e){
			std::cerr << red_str("Failed saving player!:") << red_str(e.what()) << "\n";
		}
		values.clear();

		mods::orm::inventory::flush_player(player_ptr);
		switch(player_ptr->get_class()){
			default:
				break;
			case player_class_t::GHOST:
				player_ptr->ghost()->save();
				break;
		}
		mods::orm::player_base_ability pba;
		auto status = pba.save_by_player(player_ptr);
		if(0 != status){
			std::cerr << red_str("Warning: couldn't save player_ptr's base abilities...") << "status: " << status << " for player_ptr:'" << player_ptr->name().c_str() << "'\n";
		}
		player_ptr->sendln("Your character has been saved.");
	}

	void set_reporter_lambda(reporter_t f){
		report_function = f;
		reporter_function_set = true;
	}
	void report(int64_t code, std::string_view msg){
		if(!reporter_function_set){
			m_debug("[fallback reporter]: " << red_str(CAT("code:",(code))) << 
				" message: '" << red_str(msg.data()) << "'");
			return;
		}
		report_function(code,msg);
	}
	void set_class(player_ptr_t& player, player_class_t p_class){
		m_debug(green_str("set_class called for player uuid:") << player->uuid());
		player->set_class(p_class);
		switch(p_class){
			case GHOST:
				player->set_ghost(mods::classes::create_ghost(player));
				mods::replenish::register_ghost(player->uuid());
				break;
			default:
				report(p_class,"Unable to create class. unknown class");
				break;
		}
	}
	void load_base_abilities(player_ptr_t& player){
		mods::orm::player_base_ability pba;
		pba.feed_player(player);
	}

	int save_player_password(player_ptr_t& player,std::string_view password){
		try{
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
		}catch(std::exception& e){
			auto msg = CAT("Player:'",player->db_id(),"/name:'",player->name().c_str(),"'.. Unable to save player password!:'",e.what(),"'");
			report(-1,msg);
			std::cerr << red_str("Failed updating player password!:") << red_str(msg) << "\n";
			return -1;
		}
	}
};
