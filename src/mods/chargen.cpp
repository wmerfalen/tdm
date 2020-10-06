#include "chargen.hpp"
#include "classes/includes.hpp"
#include "weapon.hpp"
#include "../db.h"
#include <tuple>
#include <string>
namespace db {
	extern int16_t delete_char(player_ptr_t& player);
};

extern player_class_t parse_class(char arg);
extern std::string motd;
extern size_t write_to_output(mods::descriptor_data &t, const char *txt, ...) __attribute__((format(printf, 2, 3)));
extern const char* MENU;
namespace mods::chargen {
const char* class_menu =
		"\r\n"
		"  |=================================================|\r\n"
    "  | Sniper (Rogue Archetype)                        |\r\n"
		"  |-------------------------------------------------|\r\n"
		"  |  -> [1G] GHOST       |         stealth operator |\r\n" 
		"  |  -> [1M] Marksman    |           weapons expert |\r\n" 
		"  |  -> [1B] Bandit      |                    thief |\r\n" 
		"  |=================================================|\r\n"
		"  | Marine (Warrior Archetype)                      |\r\n"
		"  |-------------------------------------------------|\r\n"
		"  |  -> [2B] Butcher     | bladed weapon specialist |\r\n"
		"  |  -> [2S] Striker     |      martial arts expert |\r\n"
		"  |  -> [2M] Obstructor  |       submissions expert |\r\n"
		"  |=================================================|\r\n"
		"  | Chemist (Alchemist Archetype)                   |\r\n"
		"  |-------------------------------------------------|\r\n"
		"  |  -> [3M] Malady      | poison weapon specialist |\r\n"
		"  |  -> [3P] Pyrexia     |    incendiary specialist |\r\n"
		"  |  -> [3D] Dealer      |   enhancement specialist |\r\n"
		"  |=================================================|\r\n"
		"  | Engineer (Blacksmith Archetype)                 |\r\n"
		"  |-------------------------------------------------|\r\n"
		"  |  -> [4F] Forge       |          weapons crafter |\r\n"
		"  |  -> [4S] Syndrome    |  autonomous drone expert |\r\n"
		"  |  -> [4M] Machinist   |        technology expert |\r\n"
		"  |----------------------===========================|\r\n"
		"\r\n"
		"To see the stats of a class, type the phrase stats followed by a space and the class identifier.\r\n"
		"For example: {grn}stats 4F{/grn}\r\n"
		"Your choice: "
		;
const char* real_blind_friendly_prompt = 
	"Before we get started, do you need the blind-friendly version of our character creation screens? Type 'Yes' or 'No'.\r\n";

	const char* blind_friendly_prompt(){
		return real_blind_friendly_prompt;
	}

		static std::vector<std::string> classes;
		void init(){
			classes.clear();
			classes.emplace_back(
				"GHOST: The ghost class is a stealth operator that can deploy full stealth camoflauge. "
				"This class is a sniper sub-class which means it can take out enemies from long distances."
				"To display the stats for this class, type stats.\r\n"
				"To display the next class, type next. If you would like to choose this class type choose GHOST."
				"Your choice: "
			);
			classes.emplace_back(
				"MARKSMAN: The Marksman class is a weapons export. This is a sniper sub-class which means it can "
				"snipe from far distances and is also very well-versed in assault rifles."
				"To display the stats for this class, type stats.\r\n"
				"To display the next class, type next. If you would like to choose this class type choose MARKSMAN."
				"Your choice: "
			);
			classes.emplace_back(
				"BANDIT: This is a thief class.\r\n"
				"To display the stats for this class, type stats.\r\n"
				"To display the next class, type next. If you would like to choose this class type choose BANDIT."
				"Your choice: "
			);
			classes.emplace_back(
				"BUTCHER:  Bladed weapons specialist.\r\n"
				"To display the stats for this class, type stats.\r\n"
				"To display the next class, type next. If you would like to choose this class type choose BUTCHER."
				"Your choice: "
			);
			classes.emplace_back(
				"STRIKER: Martial arts expert."
				"To display the stats for this class, type stats.\r\n"
				"To display the next class, type next. If you would like to choose this class type choose STRIKER."
				"Your choice: "
			);
			classes.emplace_back(
				"OBSTRUCTOR: Submissions grappling expert."
				"To display the stats for this class, type stats.\r\n"
				"To display the next class, type next. If you would like to choose this class type choose OBSTRUCTOR."
				"Your choice: "
			);
			classes.emplace_back(
				"MALADY: Poison and chemical warfare specialist."
				"To display the stats for this class, type stats.\r\n"
				"To display the next class, type next. If you would like to choose this class type choose MALADY."
				"Your choice: "
			);
			classes.emplace_back(
				"PYREXIA: Incendiary weapons specialist."
				"To display the stats for this class, type stats.\r\n"
				"To display the next class, type next. If you would like to choose this class type choose PYREXIA."
				"Your choice: "
			);
			classes.emplace_back(
				"DEALER: Chemical performance enhancement specialist."
				"To display the stats for this class, type stats.\r\n"
				"To display the next class, type next. If you would like to choose this class type choose DEALER."
				"Your choice: "
			);
			classes.emplace_back(
				"FORGE: Master weapons crafting specialist."
				"To display the stats for this class, type stats.\r\n"
				"To display the next class, type next. If you would like to choose this class type choose FORGE."
				"Your choice: "
			);
			classes.emplace_back(
				"SYNDROME: Autonomous drone and recon expert."
				"To display the stats for this class, type stats.\r\n"
				"To display the next class, type next. If you would like to choose this class type choose SYNDROME."
				"Your choice: "
			);
			classes.emplace_back(
				"MACHINIST: Technology expert."
				"To display the stats for this class, type stats.\r\n"
				"To display the next class, type next. If you would like to choose this class type choose MACHINIST."
				"Your choice: "
			);
		}
	void show_blind_stats(player_ptr_t& player){
		auto data = player->get_ada_data();
		auto i = mods::util::stoi(data["current-class"]).value_or(0);
		auto str_class = classes[i];//std::get<0>(classes[i]);
		if(str_class.compare("GHOST") == 0){player->sendln(CHARGEN_BLIND_GHOST_CLASS_TRIADS()); return; }
		if(str_class.compare("MARKSMAN") == 0){player->sendln(CHARGEN_BLIND_MARKSMAN_CLASS_TRIADS()); return; }
		if(str_class.compare("BANDIT") == 0){player->sendln(CHARGEN_BLIND_BANDIT_CLASS_TRIADS()); return; }
		if(str_class.compare("BUTCHER") == 0){player->sendln(CHARGEN_BLIND_BUTCHER_CLASS_TRIADS()); return; }
		if(str_class.compare("STRIKER") == 0){player->sendln(CHARGEN_BLIND_STRIKER_CLASS_TRIADS()); return; }
		if(str_class.compare("OBSTRUCTOR") == 0){player->sendln(CHARGEN_BLIND_OBSTRUCTOR_CLASS_TRIADS()); return; }
		if(str_class.compare("MALADY") == 0){player->sendln(CHARGEN_BLIND_MALADY_CLASS_TRIADS()); return; }
		if(str_class.compare("PYREXIA") == 0){player->sendln(CHARGEN_BLIND_PYREXIA_CLASS_TRIADS()); return; }
		if(str_class.compare("DEALER") == 0){player->sendln(CHARGEN_BLIND_DEALER_CLASS_TRIADS()); return; }
		if(str_class.compare("FORGE") == 0){player->sendln(CHARGEN_BLIND_FORGE_CLASS_TRIADS()); return; }
		if(str_class.compare("SYNDROME") == 0){player->sendln(CHARGEN_BLIND_SYNDROME_CLASS_TRIADS()); return; }
		if(str_class.compare("MACHINIST") == 0){player->sendln(CHARGEN_BLIND_MACHINIST_CLASS_TRIADS()); return; }
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
		if(data["accept"].compare("class-menu") == 0 && strncmp(argument.data(),"next",strlen("next")) == 0){
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
			case CLASS_UNDEFINED: player->sendln(CHARGEN_UNDEFINED_CLASS_TRIADS()); break;
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
			case CLASS_UNDEFINED: return CHARGEN_UNDEFINED_CLASS_DESCRIPTION();
		}
	}
	int parse_primary_choice(char in_choice,int class_type){
#if 0
		char choice = std::tolower(in_choice);
		switch((player_class_t)class_type){
			case player_class_t::CLASS_BANDIT:
				if(choice == 'm'){
					return mods::classes::sentinel::primary_choice_t::MP5;
				}
				if(choice == 's'){
					return mods::classes::sentinel::primary_choice_t::SASG12;
				}
				break;
			case player_class_t::CLASS_MARKSMAN:
			case player_class_t::CLASS_GHOST:
				if(choice == 'p'){
					return mods::classes::ghost::primary_choice_t::PSG1;
				}
				if(choice == 'a'){
					return mods::classes::ghost::primary_choice_t::L96AW;
				}
				break;
			case player_class_t::CLASS_STRIKER:
			case player_class_t::CLASS_OBSTRUCTOR:
			case player_class_t::CLASS_BUTCHER:
				if(choice == 'm'){
					return mods::classes::marine::primary_choice_t::M16A4;
				}
				if(choice == 'c'){
					return mods::classes::marine::primary_choice_t::M4;
				}
				break;
			case player_class_t::CLASS_PYREXIA:
			case player_class_t::CLASS_MALADY:
				if(choice == 'm'){
					return mods::classes::contagion::primary_choice_t::M3;
				}
				if(choice == 'f'){
					return mods::classes::contagion::primary_choice_t::FAMAS;
				}
				break;
			case player_class_t::CLASS_FORGE:
			case player_class_t::CLASS_SYNDROME:
			case player_class_t::CLASS_MACHINIST:
				if(choice == 'f'){
					return mods::classes::engineer::primary_choice_t::FMG9;
				}
				if(choice == 'p'){
					return mods::classes::engineer::primary_choice_t::P90;
				}
				break;
			
			case player_class_t::CLASS_PSYOP:
				if(choice == 's'){
					return mods::classes::psyop::primary_choice_t::SCARH;
				}
				if(choice == 'u'){
					return mods::classes::psyop::primary_choice_t::UMP45;
				}
				break;
			case player_class_t::CLASS_SUPPORT:
				if(choice == 'm'){
					return mods::classes::support::primary_choice_t::MK46;
				}
				if(choice == 'h'){
					return mods::classes::support::primary_choice_t::HK21;
				}
				break;
			default: return -1;
		}
		return -1;
#endif
		return 0;
	}
	std::string primary_weapon_menu(player_class_t class_type){
		return "";
#if 0
		std::string r = "Primary weapon selection\r\n";
		switch(class_type){
			case CLASS_MARKSMAN:
			case CLASS_GHOST:
				return r +
					"[p] PSG-1 - " + PRIMARY_CHOICE_SCREEN_PSG1_DESCRIPTION() +
					"[a] L96 Arctic Warfare - " + PRIMARY_CHOICE_SCREEN_L96AW_DESCRIPTION()
					;
			case CLASS_STRIKER:
			case CLASS_OBSTRUCTOR:
			case CLASS_BUTCHER:
				return r +
					"[m] M16A4 - " + PRIMARY_CHOICE_SCREEN_M16A4_DESCRIPTION() +
					"[c] M4 Carbine - " + PRIMARY_CHOICE_SCREEN_M4_DESCRIPTION()
					;
			case CLASS_BANDIT:
				return r +
					"[m] MP5 - " + PRIMARY_CHOICE_SCREEN_MP5_DESCRIPTION() +
					"[s] SASG-12 - " + PRIMARY_CHOICE_SCREEN_SASG12_DESCRIPTION()
					;
			case CLASS_PYREXIA:
			case CLASS_MALADY:
				return r +
					"[m] M3 - " + PRIMARY_CHOICE_SCREEN_M3_DESCRIPTION() + 
					"[f] FAMAS - " + PRIMARY_CHOICE_SCREEN_FAMAS_DESCRIPTION()
					;
			case CLASS_FORGE:
			case CLASS_SYNDROME:
			case CLASS_MACHINIST:
				return r +
					"[f] FMG-9 - " + PRIMARY_CHOICE_SCREEN_FMG9_DESCRIPTION() +
					"[p] P90 - " + PRIMARY_CHOICE_SCREEN_P90_DESCRIPTION()
					;
			case CLASS_MEDIC:
				return r +
					"[a] AUG-PARA - " + PRIMARY_CHOICE_SCREEN_AUGPARA_DESCRIPTION() +
					"[t] TAR-21 - " + PRIMARY_CHOICE_SCREEN_TAR21_DESCRIPTION()
					;
			case CLASS_PSYOP:
				return r +
					"[s] SCAR-H - " + PRIMARY_CHOICE_SCREEN_SCARH_DESCRIPTION() +
					"[u] UMP45 - " + PRIMARY_CHOICE_SCREEN_UMP45_DESCRIPTION()
					;
			case CLASS_SUPPORT:
				return r +
					"[m] MK46 - " + PRIMARY_CHOICE_SCREEN_MK46_DESCRIPTION() +
					"[h] HK21 - " + PRIMARY_CHOICE_SCREEN_HK21_DESCRIPTION()
					;
			default: return "";
		}
#endif
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
		return {1,"success"};
	}
	void show_finalized_chargen_screen(player_ptr_t p){
		p->set_state(CON_RMOTD);
		write_to_output(p->desc(), "%s\r\n*** PRESS RETURN: ", motd.c_str());
	}
	void handle_primary_choice(player_ptr_t p,char arg_choice,player_class_t class_type){
#if 0
		int choice = mods::chargen::parse_primary_choice(arg_choice,class_type);
		int sresult = -1;
		if(choice < 0){
			write_to_output(p->desc(), "\r\nI don't get it? Please read the menu and try again...\r\n%s\r\nSelect primary weapon: ", primary_weapon_menu(class_type).data());
			p->set_state(CON_CHARGEN_PRIMARY_CHOICE);
			return;
		}
		switch(class_type){
			case CLASS_STRIKER:
			case CLASS_OBSTRUCTOR:
			case CLASS_BUTCHER:
			case CLASS_BANDIT:
			case CLASS_PYREXIA:
			case CLASS_MALADY:
			case CLASS_FORGE:
			case CLASS_SYNDROME:
			case CLASS_MACHINIST:
			case CLASS_MARKSMAN:
				break;
			case CLASS_GHOST:
				p->set_ghost(mods::classes::create_ghost(p));
				sresult = p->cl_ghost()->new_player(p,(mods::classes::ghost::primary_choice_t)choice);
				break;
			case CLASS_SNIPER:
				p->set_sniper(mods::classes::create_sniper(p));
				sresult = p->cl_sniper()->new_player(p,(mods::classes::sniper::primary_choice_t)choice);
				break;
			case CLASS_MARINE:
				p->set_marine(mods::classes::create_marine(p));
				sresult = p->cl_marine()->new_player(p,(mods::classes::marine::primary_choice_t)choice);
				break;
			case CLASS_SENTINEL:
				p->set_sentinel(mods::classes::create_sentinel(p));
				sresult = p->cl_sentinel()->new_player(p,(mods::classes::sentinel::primary_choice_t)choice);
				break;
			case CLASS_CONTAGION:
				p->set_contagion(mods::classes::create_contagion(p));
				sresult = p->cl_contagion()->new_player(p,(mods::classes::contagion::primary_choice_t)choice);
				break;
			case CLASS_ENGINEER:
				p->set_engineer(mods::classes::create_engineer(p));
				sresult = p->cl_engineer()->new_player(p,(mods::classes::engineer::primary_choice_t)choice);
				break;
			case CLASS_MEDIC:
				p->set_medic(mods::classes::create_medic(p));
				sresult = p->cl_medic()->new_player(p,(mods::classes::medic::primary_choice_t)choice);
				break;
			case CLASS_PSYOP:
				p->set_psyop(mods::classes::create_psyop(p));
				sresult = p->cl_psyop()->new_player(p,(mods::classes::psyop::primary_choice_t)choice);
				break;
			case CLASS_SUPPORT:
				p->set_support(mods::classes::create_support(p));
				sresult = p->cl_support()->new_player(p,(mods::classes::support::primary_choice_t)choice);
				break;
			default:
				write_to_output(p->desc(), "\r\nPlease contact an admin for assistance. Error code 611.");
				p->set_state(CON_CLOSE);
				break;
		}//end switch
		if(sresult < 0){
			write_to_output(p->desc(), "\r\nIt appears something went wrong. Contact an admin. Error code 600.");
			p->set_state(CON_CLOSE);
			return;
		}
		show_finalized_chargen_screen(p);
#endif
		return;
	}
};
