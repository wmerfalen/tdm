#include "chargen.hpp"
#include "classes/includes.hpp"
#include "weapon.hpp"
#include "../db.h"
#include <tuple>
#include <string>
#include "orm/player-base-ability.hpp"
#include "players/db-load.hpp"
#include "levels.hpp"
#include "orm/enabled-classes.hpp"
namespace db {
	extern int16_t delete_char(player_ptr_t& player);
};

extern player_class_t parse_class(char arg);
extern std::string motd;
extern size_t write_to_output(mods::descriptor_data& t, const char *txt, ...) __attribute__((format(printf, 2, 3)));
extern const char* MENU;

namespace mods::chargen {
	static std::vector<std::string> classes;
	void init() {
		classes.clear();
		classes.emplace_back(
		    "MARINE:\r\n"
		    " /----------------------------\r\n"
		    "| STATS                       |\r\n"
		    "|-----------------------------|\r\n"
		    "| MELEE          | 2          |\r\n"
		    "| WEAPONS        | 2          |\r\n"
		    "| INTEL          | 2          |\r\n"
		    "| SPEED          | 2          |\r\n"
		    "| ARMOR          | 2          |\r\n"
		    " \\-----------------------------+\r\n"
		);
		classes.emplace_back(
		    "BREACHER:\r\n"
		    " /----------------------------\r\n"
		    "| STATS                       |\r\n"
		    "|-----------------------------|\r\n"
		    "| MELEE          | 3          |\r\n"
		    "| WEAPONS        | 2          |\r\n"
		    "| INTEL          | 1          |\r\n"
		    "| SPEED          | 1          |\r\n"
		    "| ARMOR          | 3          |\r\n"
		    " \\-----------------------------+\r\n"
		);
		classes.emplace_back(
		    "GHOST:\r\n"
		    " /----------------------------\r\n"
		    "| STATS                       |\r\n"
		    "|-----------------------------|\r\n"
		    "| MELEE          | 1          |\r\n"
		    "| WEAPONS        | 3          |\r\n"
		    "| INTEL          | 2          |\r\n"
		    "| SPEED          | 3          |\r\n"
		    "| ARMOR          | 1          |\r\n"
		    " \\-----------------------------+\r\n"
		);
		classes.emplace_back(
		    "CONTAGION:\r\n"
		    " /----------------------------\r\n"
		    "| STATS                       |\r\n"
		    "|-----------------------------|\r\n"
		    "| MELEE          | 1          |\r\n"
		    "| WEAPONS        | 2          |\r\n"
		    "| INTEL          | 3          |\r\n"
		    "| SPEED          | 3          |\r\n"
		    "| ARMOR          | 1          |\r\n"
		    " \\-----------------------------+\r\n"
		);
	};
	std::string chargen_triads_for(player_class_t pclass) {
		std::array<uint8_t,5> triads = mods::levels::get_triads_by_class(pclass);
		std::array<char,1024> buffer = {0};
		snprintf(&buffer[0],1024,CHARGEN_TRIADS_FORMAT().c_str(),triads[MELEE],triads[WEAPONS],triads[INTEL],triads[SPEED],triads[ARMOR]);
		return std::string(&buffer[0]);
	}
	void show_blind_stats(player_ptr_t& player) {
		auto data = player->get_ada_data();
		auto i = mods::util::stoi(data["current-class"]).value_or(0);
		auto str_class = mods::util::extract_until(classes[i],':');
		if(str_class.compare("BREACHER") == 0) {
			player->sendln(chargen_triads_for(BREACHER));
		}
		if(str_class.compare("MARINE") == 0) {
			player->sendln(chargen_triads_for(MARINE));
		}
		if(str_class.compare("GHOST") == 0) {
			player->sendln(chargen_triads_for(GHOST));
		}
		if(str_class.compare("CONTAGION") == 0) {
			player->sendln(chargen_triads_for(CONTAGION));
		}
		player->send("Press enter to go back to the class list.");
	}

	int create_char_from_registration(player_ptr_t& p,std::string argument) {
		player_class_t pclass = GHOST;//FIXME parse_class(argument);
		p->set_class(pclass);
		std::tuple<bool,std::string> make_char_status;
		make_char_status = mods::chargen::make_char(p,pclass);
		if(!std::get<0>(make_char_status)) {
			p->send("\r\nAn error occurred: %s\r\n", std::get<1>(make_char_status).data());
			p->set_state(CON_CLOSE);
			mods::chargen::undo_make_char(p);
			mods::globals::unregister_authenticated_player(p);
			p->set_authenticated(false);
			p->set_db_id(0);
			return -2;
		}
		p->sendln(MENU);
		p->set_state(CON_MENU);
		return 0;
	}

	void show_blind_friendly_class_menu(player_ptr_t& player) {
		static bool initialized = false;
		if(!initialized) {
			init();
			initialized = true;
		}
		auto data = player->get_ada_data();
		if(data.find("current-class") == data.end()) {
			player->set_ada_data("current-class","0");
			data = player->get_ada_data();
		}
		int ctr = mods::util::stoi(data["current-class"]).value_or(0);
		if(ctr >= classes.size()) {
			ctr = 0;
			player->set_ada_data("current-class","0");
			data = player->get_ada_data();
		}
		player->sendln(classes[ctr]);
		auto str_class = mods::util::extract_until(classes[ctr],':');
		player->send("To display the stats for this class, {yel}type {grn}stats{/grn}.\r\n");
		player->sendln(CAT("To display the next class, {yel}type {grn}next.{/grn}, or {grn}n{/grn} for short. If you would like to choose this class {yel}type {grn}choose ",str_class,"{/grn}"));
		player->send("Your choice: ");
		player->set_state(CON_BLIND_CHARGEN_TAKE_OVER);
		player->set_ada_data("accept","class-menu");
		return;
	}
	void blind_chargen_take_over(player_ptr_t& player,std::string_view argument) {
		auto& data = player->get_ada_data();
		if(data.find("state") == data.end()) {
			player->set_ada_data("state","show-class-menu");
			data = player->get_ada_data();
		}
		if(data["accept"].compare("class-menu") == 0) {
			if(strncmp(argument.data(),"stats",strlen("stats")) == 0) {
				show_blind_stats(player);
				player->set_state(CON_BLIND_CHARGEN_TAKE_OVER);
				player->set_ada_data("accept","stats-menu");
				return;
			}
		}
		auto class_counter = mods::util::stoi(data["current-class"]).value_or(0);
		auto str_class = mods::util::extract_until(classes[class_counter],':');
		if(data["accept"].compare("confirm-class") == 0 && mods::util::match_any_lower(argument, {"y","yes"},3)) {
			player_class_t pclass = mods::util::to_player_class(str_class);
			player->set_class(pclass);
			std::tuple<bool,std::string> make_char_status = mods::chargen::make_char(player,pclass);
			if(!std::get<0>(make_char_status)) {
				player->send("\r\n%s\r\n",std::get<1>(make_char_status).data());
				player->set_state(CON_CLOSE);
				mods::chargen::undo_make_char(player);
				mods::globals::unregister_authenticated_player(player);
				player->set_authenticated(false);
				player->set_db_id(0);
				return;
			}
			player->sendln(MENU);
			player->set_state(CON_MENU);
			parse_sql_player(player);
			return;
		}
		if(data["accept"].compare("class-menu") == 0 && mods::util::is_lower_match(argument, CAT("choose ",str_class))) {
			player->set_ada_data("current-class",std::to_string(class_counter));
			player->set_ada_data("accept", "confirm-class");
			player->send("%s", CAT("Are you sure you want to choose the ", str_class, " as your class? Yes or No: ").c_str());
			return;
		}
		if(data["accept"].compare("class-menu") == 0 && mods::util::match_any(argument, {"n","next"},4)) {
			auto i = mods::util::stoi(data["current-class"]).value_or(0);
			player->set_ada_data("current-class",std::to_string(++i));
			show_blind_friendly_class_menu(player);
			return;
		}
		if(data["state"].compare("show-class-menu") == 0) {
			show_blind_friendly_class_menu(player);
			player->set_state(CON_BLIND_CHARGEN_TAKE_OVER);
		}
	}

	void show_blind_friendly_chargen_prompt(player_ptr_t& player) {
		player->send("You are about to choose your class. Do you need us to format the menu to accomodate for screen readers? Type Yes or No:");
	}
	void show_class_menu(player_ptr_t& player) {
		//if(player->needs_ada()){
		show_blind_friendly_class_menu(player);
		//return;
		//}
		//write_to_output(player->desc(), "%s", class_menu);
	}
	void show_triads(player_ptr_t& player, player_class_t p_class) {
		switch(p_class) {
			case player_class_t::MARINE:
				player->sendln(chargen_triads_for(MARINE));
				break;
			case player_class_t::BREACHER:
				player->sendln(chargen_triads_for(BREACHER));
				break;
			case player_class_t::CONTAGION:
				player->sendln(chargen_triads_for(CONTAGION));
				break;
			case player_class_t::GHOST:
				player->sendln(chargen_triads_for(GHOST));
				break;
			default:
				player->sendln("No stats page for the class you picked. Something has gone wrong...");
				break;
		}
		player->send("Press enter to go back to the class menu.");
	}
	std::string get_class_description(player_class_t p_class) {
		switch(p_class) {
			case player_class_t::MARINE:
				return classes[1];
				break;
			case player_class_t::BREACHER:
				return classes[2];
				break;
			case player_class_t::CONTAGION:
				return classes[4];
				break;
			case player_class_t::GHOST:
				return classes[3];
				break;
			default:
				return "Unknown class selected.";
				break;
		}
	}

	void undo_make_char(player_ptr_t player) {
		int ret = mods::players::db_load::delete_char(player);
		if(ret < 0) {
			mudlog(NRM, LVL_IMMORT, TRUE, CAT(player->name()," [",player->host(),"] player creation rollback ",
			                                  red_str("failed")," for user: S.Q.L. id: '",(player->pk_id()),"'").c_str());
			return;
		}
		mudlog(NRM, LVL_IMMORT, TRUE, CAT(player->name()," [",player->host(),"] player creation rollback ",
		                                  green_str("success")).c_str());
	}
	std::tuple<bool,std::string> make_char(player_ptr_t player,player_class_t class_type) {
		REMOVE_BIT(MOB_FLAGS(player->cd()), MOB_ISNPC);
		player->set_db_id(0);
		GET_CLASS(player->cd()) = class_type;
		if(mods::players::db_load::save_new_char(player) == 0) {
			if(mods::players::db_load::load_char_pkid(player) < 0) {
				log("SYSERR: couldn't load character's pkid: '%s'",player->name().c_str());
				player->set_state(CON_CLOSE);
				return {0,"An error occurred during player creation. Please contact an admin for more help.\r\nError Code 118\r\n"};
			}
			if(parse_sql_player(player) == false) {
				log("SYSERR: after saving to the db, we couldn't parse the player's info");
				player->set_state(CON_CLOSE);
				return{0,"An error occurred during player creation. Please contact an admin for more help.\r\nError Code 218\r\n"};
			} else {
				mudlog(NRM, LVL_IMMORT, TRUE, "%s [%s] new player.", player->name().c_str(), player->host().c_str());
			}
		} else {
			log("SYSERR: save_new_char failed");
			mudlog(NRM, LVL_IMMORT, TRUE, "%s [%s] new player creation failed.", player->name().c_str(), player->host().c_str());
			return {0,"An error occurred during player creation. Please contact an admin for more help.\r\nError Code 318\r\n"};
		}
		mods::orm::player_base_ability pba;
		pba.initialize_row(player);
		switch(class_type) {
			case MARINE: {
					mods::orm::marine marine_orm;
					marine_orm.initialize_row(player);
				}
				break;
			case BREACHER: {
					mods::orm::breacher breacher_orm;
					breacher_orm.initialize_row(player);
				}
				break;
			case GHOST: {
					mods::orm::ghost ghost_orm;
					ghost_orm.initialize_row(player);
				}
				break;
			case CONTAGION: {
					mods::orm::contagion contagion_orm;
					contagion_orm.initialize_row(player);
				}
				break;
			default:
				break;
		}
		mods::levels::gain_exp(player,1);

		return {1,"success"};
	}
	void show_finalized_chargen_screen(player_ptr_t p) {
		p->set_state(CON_RMOTD);
		write_to_output(p->desc(), "%s\r\n*** PRESS RETURN: ", motd.c_str());
	}
};
