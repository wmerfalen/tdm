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

std::vector<std::string> blind_friendly_class_menu_list = {
    " Here are a list of Sniper sub-classes.\r\n",
		" The GHOST class. This class is a stealth operator. To select this class type 1G. That's the number one immediately followed by the letter G as in Good.\r\n",
		" The Marksman class. This class is a weapons expert. To select this class type 1M. That's the number one immediately followed by the letter M as in Military.\r\n",
		" The Bandit class. This class is a thief class. To select this class type 1B. That's the number one immediately followed by the letter B as in Bravo.\r\n",
		" That's all of the Sniper sub-classes.\r\n",
		"\r\n",
		" Here are a list of Marine sub-classes.r\n",
		" The Butcher class. This class is a bladed weapons specialist. To select this class type 2B. That's the number two immediately followed by the letter B as in Bravo.\r\n",
		" The Striker class. This class is a martial arts expert. To select this class type 2S. That's the number two immediately followed by the letter S as in Sahara.\r\n",
		" The Obstructor class. This class is a submissions grappling expert. To select this class type 2M. That's the number two immediately followed by the letter  M as in Military.\r\n",
		" That's all of the Marine sub-classes.\r\n",
		"\r\n",
		" Here are a list of Chemist sub-classes.\r\n",
		" The Malady class. This class is a poison weapons specialist. To select this class type 3M. That's the number three immediately followed by the letter M as in Military.\r\n",
		" The Pyrexia class. This is a incendiary pyro class. To select this class type 3P. That's the number three immediately followed by the letter P as in Proximity.\r\n",
		" The Dealer class. This is a chemical performance enhancment specialist. To select this class type 3D. That's the number three immediately followed by the letter D as in Delta.\r\n",
		" That's all of the Chemist sub-classes.\r\n",
		"\r\n",
		"Here are a list of Engineer sub-classes.\r\n",
		"The Forge class. This is a weapons crafting expert. To select this class type 4F. That's the number four immediately followed by the letter F as in Fighter.\r\n",
		"The Syndrome class. This is an autonomous drone expert class. To select this class type 4S. That's the number four immediately followed by the letter S as in Sahara.\r\n",
		"The Machinist class. This is technology expert class. To select this class type 4M. That's the number four immediately followed by the letter M as in Military.\r\n",
		"\r\n",
		/* nice to have
		"If you would like these classes listed line by line, type 'line please'. That's the word line followed by a space followed by the word please.\r\n",
		"Otherwise, choose your class below.\r\n",
		"\r\n",
		*/
		"To see the stats of a class, type the phrase stats followed by a space and the class identifier.\r\n"
		"For example: {grn}stats 4F{/grn}. That's the word 'stats' followed by a space, then the number four followed by the letter F as in Fighter.\r\n"
		"Type the class of your choice: "
	};

	void show_blind_friendly_class_menu(player_ptr_t& player){
			for(auto line :  blind_friendly_class_menu_list){
				write_to_output(player->desc(), "%s", line.c_str());
			}
	}
	void show_class_menu(player_ptr_t& player){
		if(player->needs_ada()){
			show_blind_friendly_class_menu(player);
			return;
		}
		write_to_output(player->desc(), "%s", class_menu);
	}
	void show_triads(player_ptr_t& player, player_class_t p_class){
		switch(p_class){
			case player_class_t::CLASS_GHOST:player->sendln(CHARGEN_GHOST_CLASS_TRIADS()); break;
		 	case player_class_t::CLASS_MARKSMAN:player->sendln(CHARGEN_MARKSMAN_CLASS_TRIADS()); break;
		 	case player_class_t::CLASS_BANDIT:player->sendln(CHARGEN_BANDIT_CLASS_TRIADS()); break;
		 	case player_class_t::CLASS_BUTCHER:player->sendln(CHARGEN_BUTCHER_CLASS_TRIADS()); break;
		 	case player_class_t::CLASS_STRIKER:player->sendln(CHARGEN_STRIKER_CLASS_TRIADS()); break;
		 	case player_class_t::CLASS_OBSTRUCTOR:player->sendln(CHARGEN_OBSTRUCTOR_CLASS_TRIADS()); break;
		 	case player_class_t::CLASS_MALADY:player->sendln(CHARGEN_MALADY_CLASS_TRIADS()); break;
		 	case player_class_t::CLASS_PYREXIA:player->sendln(CHARGEN_PYREXIA_CLASS_TRIADS()); break;
		 	case player_class_t::CLASS_DEALER:player->sendln(CHARGEN_DEALER_CLASS_TRIADS()); break;
		 	case player_class_t::CLASS_FORGE:player->sendln(CHARGEN_FORGE_CLASS_TRIADS()); break;
		 	case player_class_t::CLASS_SYNDROME:player->sendln(CHARGEN_SYNDROME_CLASS_TRIADS()); break;
		 	case player_class_t::CLASS_MACHINIST:player->sendln(CHARGEN_MACHINIST_CLASS_TRIADS()); break;
			default:
			case CLASS_UNDEFINED: player->sendln(CHARGEN_UNDEFINED_CLASS_TRIADS()); break;
		}

	}
	std::string get_class_description(player_class_t p_class){
		switch(p_class){
			case player_class_t::CLASS_GHOST: return CHARGEN_GHOST_CLASS_DESCRIPTION();
		 	case player_class_t::CLASS_MARKSMAN: return CHARGEN_MARKSMAN_CLASS_DESCRIPTION();
		 	case player_class_t::CLASS_BANDIT: return CHARGEN_BANDIT_CLASS_DESCRIPTION();
		 	case player_class_t::CLASS_BUTCHER: return CHARGEN_BUTCHER_CLASS_DESCRIPTION();
		 	case player_class_t::CLASS_STRIKER: return CHARGEN_STRIKER_CLASS_DESCRIPTION();
		 	case player_class_t::CLASS_OBSTRUCTOR: return CHARGEN_OBSTRUCTOR_CLASS_DESCRIPTION();
		 	case player_class_t::CLASS_MALADY: return CHARGEN_MALADY_CLASS_DESCRIPTION();
		 	case player_class_t::CLASS_PYREXIA: return CHARGEN_PYREXIA_CLASS_DESCRIPTION();
		 	case player_class_t::CLASS_DEALER: return CHARGEN_DEALER_CLASS_DESCRIPTION();
		 	case player_class_t::CLASS_FORGE: return CHARGEN_FORGE_CLASS_DESCRIPTION();
		 	case player_class_t::CLASS_SYNDROME: return CHARGEN_SYNDROME_CLASS_DESCRIPTION();
		 	case player_class_t::CLASS_MACHINIST: return CHARGEN_MACHINIST_CLASS_DESCRIPTION();
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
