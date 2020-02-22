#include "chargen.hpp"
#include "classes/sentinel.hpp"
#include "weapon.hpp"
#include "../db.h"
#include <tuple>
#include <string>

//namespace db {
//	extern int16_t save_new_char(player_ptr_t player);
//	extern int16_t load_char_pkid(player_ptr_t player);
//};
//extern bool parse_sql_player(player_ptr_t player_ptr);
namespace mods::chargen {
	std::string_view parse_primary_choice(char choice,int class_type){
		switch(class_type){
			case CLASS_SENTINEL:
				if(choice == 'm' || choice == 'M'){
					return "MP5";
				}
				if(choice == 's' || choice == 'S'){
					return "SASG12";
				}
				break;
			default: return "IGNORE";
		}
		return "INVALID";
	}
	std::string_view primary_weapon_menu(player_class_t class_type){
		switch(class_type){
			case CLASS_SENTINEL:
				return "Primary weapon selection\r\n"
					"[m] mp5 - A good close combat sub machine gun\r\n"
					"[s] sasg12 - Shotgun\r\n"
					;
			default: return "";
		}
	}

	std::tuple<bool,std::string> make_char(player_ptr_t player){
		REMOVE_BIT(MOB_FLAGS(player->cd()), MOB_ISNPC);
		player->set_db_id(0);
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
	void handle_primary_choice(player_ptr_t p,char arg_choice,player_class_t class_type){
		if(class_type == CLASS_SENTINEL){
			auto choice = mods::chargen::parse_primary_choice(arg_choice,class_type);
			if(choice.compare("MP5") == 0){
				p->set_sentinel(mods::classes::create_sentinel(p));
				auto sresult = p->cl_sentinel()->new_player(p,"MP5");
				if(sresult < 0){
					write_to_output(p->desc(), "\r\nIt appears something went wrong. Contact an admin. Error code 600.");
					p->set_state(CON_CLOSE);
					return;
				}
				p->set_state(CON_CHARGEN_FINALIZE);
				return;
			}
			if(choice.compare("SASG12") == 0){
				p->set_sentinel(mods::classes::create_sentinel(p));
				auto sresult = p->cl_sentinel()->new_player(p,"SASG12");
				if(sresult < 0){
					write_to_output(p->desc(), "\r\nIt appears something went wrong. Contact an admin. Error code 600.");
					p->set_state(CON_CLOSE);
					return;
				}
				p->set_state(CON_CHARGEN_FINALIZE);
				return;
			}
			write_to_output(p->desc(), "%s\r\nSelect primary weapon: ", primary_weapon_menu(class_type).data());
			p->set_state(CON_CHARGEN_PRIMARY_CHOICE);
			return;
		}
		p->set_state(CON_CHARGEN_FINALIZE);
	}
};
