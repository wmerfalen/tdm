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
namespace mods::chargen {
	std::string get_class_description(player_class_t p_class){
		switch(p_class){
			case player_class_t::CLASS_SNIPER: return CHARGEN_SNIPER_CLASS_DESCRIPTION();
			case player_class_t::CLASS_MARINE: return CHARGEN_MARINE_CLASS_DESCRIPTION();
			case player_class_t::CLASS_SENTINEL: return CHARGEN_SENTINEL_CLASS_DESCRIPTION();
			case player_class_t::CLASS_CONTAGION: return CHARGEN_CONTAGION_CLASS_DESCRIPTION();
			case player_class_t::CLASS_ENGINEER: return CHARGEN_ENGINEER_CLASS_DESCRIPTION();
			case player_class_t::CLASS_MEDIC: return CHARGEN_MEDIC_CLASS_DESCRIPTION();
			case player_class_t::CLASS_PSYOP: return CHARGEN_PSYOP_CLASS_DESCRIPTION();
			case player_class_t::CLASS_SUPPORT: return CHARGEN_SUPPORT_CLASS_DESCRIPTION();
			default:
			case CLASS_UNDEFINED: return CHARGEN_UNDEFINED_CLASS_DESCRIPTION();
		}
	}
	int parse_primary_choice(char in_choice,int class_type){
		char choice = std::tolower(in_choice);
		switch((player_class_t)class_type){
			case player_class_t::CLASS_SENTINEL:
				if(choice == 'm'){
					return mods::classes::sentinel::primary_choice_t::MP5;
				}
				if(choice == 's'){
					return mods::classes::sentinel::primary_choice_t::SASG12;
				}
				break;
			case player_class_t::CLASS_SNIPER:
				if(choice == 'p'){
					return mods::classes::sniper::primary_choice_t::PSG1;
				}
				if(choice == 'a'){
					return mods::classes::sniper::primary_choice_t::L96AW;
				}
				break;
			case player_class_t::CLASS_MARINE:
				if(choice == 'm'){
					return mods::classes::marine::primary_choice_t::M16A4;
				}
				if(choice == 'c'){
					return mods::classes::marine::primary_choice_t::M4;
				}
				break;
			case player_class_t::CLASS_CONTAGION:
				if(choice == 'm'){
					return mods::classes::contagion::primary_choice_t::M3;
				}
				if(choice == 'f'){
					return mods::classes::contagion::primary_choice_t::FAMAS;
				}
				break;
			case player_class_t::CLASS_ENGINEER:
				if(choice == 'f'){
					return mods::classes::engineer::primary_choice_t::FMG9;
				}
				if(choice == 'p'){
					return mods::classes::engineer::primary_choice_t::P90;
				}
				break;
			case player_class_t::CLASS_MEDIC:
				if(choice == 'a'){
					return mods::classes::medic::primary_choice_t::AUGPARA;
				}
				if(choice == 't'){
					return mods::classes::medic::primary_choice_t::TAR21;
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
	}
	std::string primary_weapon_menu(player_class_t class_type){
		std::string r = "Primary weapon selection\r\n";
		switch(class_type){
			case CLASS_SNIPER:
				return r +
					"[p] PSG-1 - " + PRIMARY_CHOICE_SCREEN_PSG1_DESCRIPTION() +
					"[a] L96 Arctic Warfare - " + PRIMARY_CHOICE_SCREEN_L96AW_DESCRIPTION()
					;
			case CLASS_MARINE:
				return r +
					"[m] M16A4 - " + PRIMARY_CHOICE_SCREEN_M16A4_DESCRIPTION() +
					"[c] M4 Carbine - " + PRIMARY_CHOICE_SCREEN_M4_DESCRIPTION()
					;
			case CLASS_SENTINEL:
				return r +
					"[m] MP5 - " + PRIMARY_CHOICE_SCREEN_MP5_DESCRIPTION() +
					"[s] SASG-12 - " + PRIMARY_CHOICE_SCREEN_SASG12_DESCRIPTION()
					;
			case CLASS_CONTAGION:
				return r +
					"[m] M3 - " + PRIMARY_CHOICE_SCREEN_M3_DESCRIPTION() + 
					"[f] FAMAS - " + PRIMARY_CHOICE_SCREEN_FAMAS_DESCRIPTION()
					;
			case CLASS_ENGINEER:
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
	}

	void undo_make_char(player_ptr_t player){
		int ret = ::db::delete_char(player);
		if(ret < 0){
			mudlog(NRM, LVL_IMMORT, TRUE, CAT({player->name()," [",player->host(),"] player creation rollback ",
						red_str("failed")," for user: S.Q.L. id: '",tostr(player->pk_id()),"'"}).c_str());
			return;
		}
		mudlog(NRM, LVL_IMMORT, TRUE, CAT({player->name()," [",player->host(),"] player creation rollback ",
					green_str("success")}).c_str());
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
		int choice = mods::chargen::parse_primary_choice(arg_choice,class_type);
		int sresult = -1;
		if(choice < 0){
			write_to_output(p->desc(), "\r\nI don't get it? Please read the menu and try again...\r\n%s\r\nSelect primary weapon: ", primary_weapon_menu(class_type).data());
			p->set_state(CON_CHARGEN_PRIMARY_CHOICE);
			return;
		}
		switch(class_type){
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
		return;
	}
};
