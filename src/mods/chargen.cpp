#include "chargen.hpp"
#include "classes/includes.hpp"
#include "weapon.hpp"
#include "../db.h"
#include <tuple>
#include <string>
#include "orm/player-base-ability.hpp"
namespace db {
	extern int16_t delete_char(player_ptr_t& player);
};

extern player_class_t parse_class(char arg);
extern std::string motd;
extern size_t write_to_output(mods::descriptor_data &t, const char *txt, ...) __attribute__((format(printf, 2, 3)));
extern const char* MENU;
namespace mods::chargen {
		static std::vector<std::string> classes;
		void init(){
			classes.clear();
			classes.emplace_back(
				"GHOST: {blu}The ghost class is a stealth operator that can deploy full stealth camoflauge. "
				"This class is a sniper sub-class which means it can take out enemies from long distances.{/blu}"
			);
			classes.emplace_back(
				"MARKSMAN: {blu}The Marksman class is a weapons export. This is a sniper sub-class which means it can "
				"snipe from far distances and is also very well-versed in assault rifles.{/blu}"
			);
			classes.emplace_back(
				"BANDIT: {blu}This is a thief class.{/blu}"
			);
			classes.emplace_back(
				"BUTCHER: {blu}Bladed weapons specialist.{/blu}"
			);
			classes.emplace_back(
				"STRIKER: {blu}Martial arts expert.{/blu}"
			);
			classes.emplace_back(
				"OBSTRUCTOR: {blu}Submissions grappling expert.{/blu}"
			);
			classes.emplace_back(
				"MALADY: {blu}Poison and chemical warfare specialist.{/blu}"
			);
			classes.emplace_back(
				"PYREXIA: {blu}Incendiary weapons specialist.{/blu}"
			);
			classes.emplace_back(
				"DEALER: {blu}Chemical performance enhancement specialist.{/blu}"
			);
			classes.emplace_back(
				"FORGE: {blu}Master weapons crafting specialist.{/blu}"
			);
			classes.emplace_back(
				"SYNDROME: {blu}Autonomous drone and recon expert.{/blu}"
			);
			classes.emplace_back(
				"MACHINIST: {blu}Technology expert.{/blu}"
			);
		}
	void show_blind_stats(player_ptr_t& player){
		auto data = player->get_ada_data();
		auto i = mods::util::stoi(data["current-class"]).value_or(0);
		auto str_class = mods::util::extract_until(classes[i],':');
		if(str_class.compare("GHOST") == 0){player->sendln(CHARGEN_BLIND_GHOST_CLASS_TRIADS());}
		if(str_class.compare("MARKSMAN") == 0){player->sendln(CHARGEN_BLIND_MARKSMAN_CLASS_TRIADS());}
		if(str_class.compare("BANDIT") == 0){player->sendln(CHARGEN_BLIND_BANDIT_CLASS_TRIADS());}
		if(str_class.compare("BUTCHER") == 0){player->sendln(CHARGEN_BLIND_BUTCHER_CLASS_TRIADS());}
		if(str_class.compare("STRIKER") == 0){player->sendln(CHARGEN_BLIND_STRIKER_CLASS_TRIADS());}
		if(str_class.compare("OBSTRUCTOR") == 0){player->sendln(CHARGEN_BLIND_OBSTRUCTOR_CLASS_TRIADS());}
		if(str_class.compare("MALADY") == 0){player->sendln(CHARGEN_BLIND_MALADY_CLASS_TRIADS());}
		if(str_class.compare("PYREXIA") == 0){player->sendln(CHARGEN_BLIND_PYREXIA_CLASS_TRIADS());}
		if(str_class.compare("DEALER") == 0){player->sendln(CHARGEN_BLIND_DEALER_CLASS_TRIADS());}
		if(str_class.compare("FORGE") == 0){player->sendln(CHARGEN_BLIND_FORGE_CLASS_TRIADS());}
		if(str_class.compare("SYNDROME") == 0){player->sendln(CHARGEN_BLIND_SYNDROME_CLASS_TRIADS());}
		if(str_class.compare("MACHINIST") == 0){player->sendln(CHARGEN_BLIND_MACHINIST_CLASS_TRIADS());}
		player->send("Press enter to go back to the class list.");
	}

	int create_char_from_registration(player_ptr_t& p,std::string argument){
		player_class_t pclass = CLASS_GHOST;//FIXME parse_class(argument);
		p->set_class(pclass);
		std::tuple<bool,std::string> make_char_status;
		make_char_status = mods::chargen::make_char(p,pclass);
		if(!std::get<0>(make_char_status)){
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

	void show_blind_friendly_class_menu(player_ptr_t& player){
		static bool initialized = false;
		if(!initialized){init(); initialized = true;}
		auto data = player->get_ada_data();
		if(data.find("current-class") == data.end()){
			player->set_ada_data("current-class","0");
			data = player->get_ada_data();
		}
		int ctr = mods::util::stoi(data["current-class"]).value_or(0);
		if(ctr >= classes.size()){
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
	void blind_chargen_take_over(player_ptr_t& player,std::string_view argument){
		auto & data = player->get_ada_data();
		if(data.find("state") == data.end()){
			player->set_ada_data("state","show-class-menu");
			data = player->get_ada_data();
		}
		if(data["accept"].compare("class-menu") == 0){
			if(strncmp(argument.data(),"stats",strlen("stats")) == 0){
				show_blind_stats(player);
				player->set_state(CON_BLIND_CHARGEN_TAKE_OVER);
				player->set_ada_data("accept","stats-menu");
				return;
			}
		}
		auto class_counter = mods::util::stoi(data["current-class"]).value_or(0);
		auto str_class = mods::util::extract_until(classes[class_counter],':');
		if(data["accept"].compare("confirm-class") == 0 && mods::util::match_any_lower(argument,{"y","yes"},3)){
			player_class_t pclass = mods::util::to_player_class(str_class);
			player->set_class(pclass);
			std::tuple<bool,std::string> make_char_status = mods::chargen::make_char(player,pclass);
			if(!std::get<0>(make_char_status)){
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
		if(data["accept"].compare("class-menu") == 0 && mods::util::is_lower_match(argument, CAT("choose ",str_class))){
			player->set_ada_data("current-class",std::to_string(class_counter));
			player->set_ada_data("accept", "confirm-class");
			player->send("%s", CAT("Are you sure you want to choose the ", str_class, " as your class? Yes or No: ").c_str());
			return;
		}
		if(data["accept"].compare("class-menu") == 0 && mods::util::match_any(argument,{"n","next"},4)){
			auto i = mods::util::stoi(data["current-class"]).value_or(0);
			player->set_ada_data("current-class",std::to_string(++i));
			show_blind_friendly_class_menu(player);
			return;
		}
		if(data["state"].compare("show-class-menu") == 0){
			show_blind_friendly_class_menu(player);
			player->set_state(CON_BLIND_CHARGEN_TAKE_OVER);
		}
	}

	void show_blind_friendly_chargen_prompt(player_ptr_t& player){
		player->send("You are about to choose your class. Do you need us to format the menu to accomodate for screen readers? Type Yes or No:");
	}
	void show_class_menu(player_ptr_t& player){
		//if(player->needs_ada()){
			show_blind_friendly_class_menu(player);
			//return;
		//}
		//write_to_output(player->desc(), "%s", class_menu);
	}
	void show_triads(player_ptr_t& player, player_class_t p_class){
		switch(p_class){
			case player_class_t::CLASS_GHOST:player->sendln(CHARGEN_BLIND_GHOST_CLASS_TRIADS()); break;
		 	case player_class_t::CLASS_MARKSMAN:player->sendln(CHARGEN_BLIND_MARKSMAN_CLASS_TRIADS()); break;
		 	case player_class_t::CLASS_BANDIT:player->sendln(CHARGEN_BLIND_BANDIT_CLASS_TRIADS()); break;
		 	case player_class_t::CLASS_BUTCHER:player->sendln(CHARGEN_BLIND_BUTCHER_CLASS_TRIADS()); break;
		 	case player_class_t::CLASS_STRIKER:player->sendln(CHARGEN_BLIND_STRIKER_CLASS_TRIADS()); break;
		 	case player_class_t::CLASS_OBSTRUCTOR:player->sendln(CHARGEN_BLIND_OBSTRUCTOR_CLASS_TRIADS()); break;
		 	case player_class_t::CLASS_MALADY:player->sendln(CHARGEN_BLIND_MALADY_CLASS_TRIADS()); break;
		 	case player_class_t::CLASS_PYREXIA:player->sendln(CHARGEN_BLIND_PYREXIA_CLASS_TRIADS()); break;
		 	case player_class_t::CLASS_DEALER:player->sendln(CHARGEN_BLIND_DEALER_CLASS_TRIADS()); break;
		 	case player_class_t::CLASS_FORGE:player->sendln(CHARGEN_BLIND_FORGE_CLASS_TRIADS()); break;
		 	case player_class_t::CLASS_SYNDROME:player->sendln(CHARGEN_BLIND_SYNDROME_CLASS_TRIADS()); break;
		 	case player_class_t::CLASS_MACHINIST:player->sendln(CHARGEN_BLIND_MACHINIST_CLASS_TRIADS()); break;
			default:
				player->sendln("No stats page for the class you picked. Something has gone wrong...");
				break;
		}
		player->send("Press enter to go back to the class menu.");
	}
	std::string get_class_description(player_class_t p_class){
		switch(p_class){
			case player_class_t::CLASS_GHOST: return classes[0];
		 	case player_class_t::CLASS_MARKSMAN: return classes[1];
		 	case player_class_t::CLASS_BANDIT: return classes[2];
		 	case player_class_t::CLASS_BUTCHER: return classes[3];
		 	case player_class_t::CLASS_STRIKER: return classes[4];
		 	case player_class_t::CLASS_OBSTRUCTOR: return classes[5];
		 	case player_class_t::CLASS_MALADY: return classes[6];
		 	case player_class_t::CLASS_PYREXIA: return classes[7];
		 	case player_class_t::CLASS_DEALER: return classes[8];
		 	case player_class_t::CLASS_FORGE: return classes[9];
		 	case player_class_t::CLASS_SYNDROME: return classes[10];
		 	case player_class_t::CLASS_MACHINIST: return classes[11];
			default:
			case CLASS_UNDEFINED: return "Unknown class selected.";
		}
	}

	void undo_make_char(player_ptr_t player){
		int ret = ::db::delete_char(player);
		if(ret < 0){
			mudlog(NRM, LVL_IMMORT, TRUE, CAT(player->name()," [",player->host(),"] player creation rollback ",
						red_str("failed")," for user: S.Q.L. id: '",(player->pk_id()),"'").c_str());
			return;
		}
		mudlog(NRM, LVL_IMMORT, TRUE, CAT(player->name()," [",player->host(),"] player creation rollback ",
					green_str("success")).c_str());
	}
	std::tuple<bool,std::string> make_char(player_ptr_t player,player_class_t class_type){
		REMOVE_BIT(MOB_FLAGS(player->cd()), MOB_ISNPC);
		player->set_db_id(0);
		GET_CLASS(player->cd()) = class_type;
		if(::db::save_new_char(player) == 0){
			if(::db::load_char_pkid(player) < 0){
				log("SYSERR: couldn't load character's pkid: '%s'",player->name().c_str());
				player->set_state(CON_CLOSE);
				return {0,"An error occurred during player creation. Please contact an admin for more help.\r\nError Code 118\r\n"};
			}
			if(parse_sql_player(player) == false){
				log("SYSERR: after saving to the db, we couldn't parse the player's info");
				player->set_state(CON_CLOSE);
				return{0,"An error occurred during player creation. Please contact an admin for more help.\r\nError Code 218\r\n"};
			}else{
				mudlog(NRM, LVL_IMMORT, TRUE, "%s [%s] new player.", player->name().c_str(), player->host().c_str());
			}
		}else{
			log("SYSERR: save_new_char failed");
			mudlog(NRM, LVL_IMMORT, TRUE, "%s [%s] new player creation failed.", player->name().c_str(), player->host().c_str());
			return {0,"An error occurred during player creation. Please contact an admin for more help.\r\nError Code 318\r\n"};
		}
		mods::orm::player_base_ability pba;
		pba.initialize_row(player);
		return {1,"success"};
	}
	void show_finalized_chargen_screen(player_ptr_t p){
		p->set_state(CON_RMOTD);
		write_to_output(p->desc(), "%s\r\n*** PRESS RETURN: ", motd.c_str());
	}
};
