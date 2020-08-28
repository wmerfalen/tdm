#include "chargen.hpp"
#include "classes/sniper.hpp"
#include "classes/marine.hpp"
#include "classes/sentinel.hpp"
#include "classes/contagion.hpp"
#include "classes/engineer.hpp"
#include "classes/medic.hpp"
#include "classes/psyop.hpp"
#include "classes/support.hpp"
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
		static std::map<player_class_t,std::string> d;
		static bool initialized = false;
		static std::string ed = green_str("\r\n####- = [ end description\r\n");
		static std::string desc = yellow_str("[description]: ");
		if(!initialized){
			d[CLASS_SNIPER] = green_str(
						   "####- = [ The Sniper ] = -####\r\n"
							 "==============================\r\n") +
							 desc + " The sniper is a specialty class that \r\n"
							 "focusses on the SNIPER RIFLE weapon type. While any\r\n"
							 "class can wield and use a sniper rifle, the sniper has\r\n"
							 "extra proficiencies and perks associated with the use\r\n"
							 "of ranged weaponry.\r\n"
							 + ed
					;
			d[CLASS_MARINE] =
				green_str(
						   "####- = [ The Marine ] = -####\r\n"
							 "==============================\r\n") +
							 desc + "A well-rounded soldier built for many\r\n"
							 "tasks. The marine utilizes every aspect of warfare\r\n"
							 "and composes destructive technology for use against\r\n"
							 "insurgents. The ideal candidate is always itching\r\n"
							 "to get back into the battlefield. Marines have their\r\n"
							 "own specialized martial arts training program which\r\n"
							 "unlocks special melee and lethal weapon abilities.\r\n"
							 + ed
					;
			d[CLASS_SENTINEL] = green_str(
						   "####- = [ The Sentinel ] = -####\r\n"
							 "================================\r\n") +
							 desc + "The sentinel is your ideal security \r\n"
							 "task force. The longer you secure an area, the \r\n"
							 "more perks you unlock. This occurs by strategically \r\n"
							 "utilizing technology to impose your will upon the enemy.\r\n"
							 + ed
					;
			d[CLASS_CONTAGION] = green_str(
							 "####- = [ The Contagion ] = -####\r\n"
							 "=================================\r\n") +
							 desc + "Highly trained in the art of chemical \r\n"
							 "warfare, the Contagion takes chemistry to unprecedented\r\n"
							 "levels in order to support and clear out forces behind \r\n"
							 "enemy lines. A strategically placed detonation can be\r\n"
							 "the determining factor in wiping out resistance in the \r\n"
							 "local vicinity.\r\n"
							 + ed
					;
			d[CLASS_ENGINEER] = green_str(
						   "####- = [ The Engineer ] = -####\r\n"
							 "================================\r\n") +
							 desc + "Skilled in advanced methods of offensive \r\n"
							 "electronics, the Engineer supplies his squad with\r\n"
							 "intelligence and electronic subversion. A carefully \r\n"
							 "placed Engineer can subvert radio communications and\r\n"
							 "cause the opposing force to disrupt or even harm one \r\n"
							 "another by leveraging advanced hacking techniques.\r\n"
							 "The Engineer can employ offensive air, land, or water \r\n"
							 "autonomous drones to discover, steal, and disrupt the\r\n"
							 "opposing force's well-guarded secrets.\r\n"
							 + ed
					;
			d[CLASS_MEDIC] = green_str(
						   "####- = [ The Medic ] = -####\r\n"
							 "=============================\r\n") +
							 desc + "The only class with the ability to \r\n"
							 "revive downed teammates from a few rooms away.\r\n"
							 "The Medic can bandage and repair broken limbs. Squads \r\n"
							 "with a Medic in their team gain a passive HP regen\r\n"
							 "bonus and will take less damage.\r\n"
							 + ed
					;
			d[CLASS_PSYOP] = green_str(
						   "####- = [ Psychological Warfare ] = -####\r\n"
							 "====================================================\r\n") +
							 desc + "PSYOP specialists utilize techniques\r\n"
							 "of advanced warfare that simply can't be explained. \r\n"
							 "The techniques employed by a PSYOP involve a combination\r\n"
							 "of occult practices and off-world technology.\r\n" +
							 red_str("This is an advanced class.\r\n")
							 + ed
					;
			d[CLASS_SUPPORT] = green_str(
						   "####- = [ The Support Team ] = -####\r\n"
							 "=============================\r\n") +
							 desc + "Operatives in the Supporting role can\r\n"
							 "carry an obscene amount of equipment long distances. \r\n"
							 "They can revitalize wounded soldiers with medkits,\r\n"
							 "or supply ammo to fellow squad members. A support \r\n"
							 "specialist has a natural affinity for the light machine gun\r\n"
							 "weapon type, and can surpress an enemy while team mates advance.\r\n"
							 + ed
					;
			initialized = true;
			d[CLASS_UNDEFINED] = green_str("class not found");
		}
		return d[p_class];
	}
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
			case CLASS_SNIPER:
				if(choice == 'p' || choice == 'P'){
					return "PSG-1";
				}
				if(choice == 'a' || choice == 'A'){
					return "L96-Arctic-Warfare";
				}
				break;
			case CLASS_MARINE:
				if(choice == 'm' || choice == 'M'){
					return "M16A4";
				}
				if(choice == 'c' || choice == 'C'){
					return "M4";
				}
				break;
			case CLASS_CONTAGION:
				if(choice == 'm' || choice == 'M'){
					return "M3";
				}
				if(choice == 'f' || choice == 'F'){
					return "FAMAS";
				}
				break;
			case CLASS_ENGINEER:
				if(choice == 'f' || choice == 'F'){
					return "FMG-9";
				}
				if(choice == 'p' || choice == 'P'){
					return "P90";
				}
				break;
			case CLASS_MEDIC:
				if(choice == 'a' || choice == 'A'){
					return "AUG-PARA";
				}
				if(choice == 't' || choice == 'T'){
					return "TAR-21";
				}
				break;
			case CLASS_PSYOP:
				if(choice == 's' || choice == 'S'){
					return "SCAR-H";
				}
				if(choice == 'u' || choice == 'U'){
					return "UMP45";
				}
				break;
			case CLASS_SUPPORT:
				if(choice == 'm' || choice == 'M'){
					return "MK46";
				}
				if(choice == 'h' || choice == 'H'){
					return "HK21";
				}
				break;
			default: return "IGNORE";
		}
		return "INVALID";
	}
	std::string_view primary_weapon_menu(player_class_t class_type){
		switch(class_type){
			case CLASS_SNIPER:
				return "Primary weapon selection\r\n"
					"[p] PSG-1 - sniper rifle \r\n"
					"[a] L96 Arctic Warfare - sniper rifle \r\n"
					;
			case CLASS_MARINE:
				return "Primary weapon selection\r\n"
					"[m] M16A4 - Standard issue marine corps automatic rifle.\r\n"
					"[c] M4 Carbine - Lighter and shorter variant of the M16A2.\r\n"
					;
			case CLASS_SENTINEL:
				return "Primary weapon selection\r\n"
					"[m] MP5 - A good close combat sub machine gun.\r\n"
					"[s] SASG-12 - Shotgun\r\n"
					;
			case CLASS_CONTAGION:
				return "Primary weapon selection\r\n"
					"[m] M3 - shotgun \r\n"
					"[f] FAMAS - assault rifle \r\n"
					;
			case CLASS_ENGINEER:
				return "Primary weapon selection\r\n"
					"[f] FMG-9 - foldable machine gun\r\n" 
					"[p] P90 - sub machine gun with large magazine\r\n"
					;
			case CLASS_MEDIC:
				return "Primary weapon selection\r\n"
					"[a] AUG-PARA - assault rifle\r\n"
					"[t] TAR-21 - assault rifle\r\n"
					;
			case CLASS_PSYOP:
				return "Primary weapon selection\r\n"
					"[s] SCAR-H - assault rifle\r\n"
					"[u] UMP45 - sub machine gun\r\n"
					;
			case CLASS_SUPPORT:
				return "Primary weapon selection\r\n"
					"[m] MK46 - light machine gun\r\n"
					"[h] HK21 - light machine gun\r\n"
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
	void show_finalized_chargen_screen(player_ptr_t p){
		p->set_state(CON_RMOTD);
		write_to_output(p->desc(), "%s\r\n*** PRESS RETURN: ", motd.c_str());
	}
	void handle_primary_choice(player_ptr_t p,char arg_choice,player_class_t class_type){
		if(class_type == CLASS_SNIPER){
			auto choice = mods::chargen::parse_primary_choice(arg_choice,class_type);
			if(choice.compare("PSG-1") == 0){
				p->set_sniper(mods::classes::create_sniper(p));
				auto sresult = p->cl_sniper()->new_player(p,"PSG-1");
				if(sresult < 0){
					write_to_output(p->desc(), "\r\nIt appears something went wrong. Contact an admin. Error code 600.");
					p->set_state(CON_CLOSE);
					return;
				}
				show_finalized_chargen_screen(p);
				return;
			}
			if(choice.compare("L96-Arctic-Warfare") == 0){
				p->set_sniper(mods::classes::create_sniper(p));
				auto sresult = p->cl_sniper()->new_player(p,"L96-Arctic-Warfare");
				if(sresult < 0){
					write_to_output(p->desc(), "\r\nIt appears something went wrong. Contact an admin. Error code 600.");
					p->set_state(CON_CLOSE);
					return;
				}
				show_finalized_chargen_screen(p);
				return;
			}
			write_to_output(p->desc(), "\r\nI don't get it? Please read the menu and try again...\r\n%s\r\nSelect primary weapon: ", primary_weapon_menu(class_type).data());
			p->set_state(CON_CHARGEN_PRIMARY_CHOICE);
			return;
		}
		if(class_type == CLASS_MARINE){
			auto choice = mods::chargen::parse_primary_choice(arg_choice,class_type);
			if(choice.compare("M16A4") == 0){
				p->set_marine(mods::classes::create_marine(p));
				auto sresult = p->cl_marine()->new_player(p,"M16A4");
				if(sresult < 0){
					write_to_output(p->desc(), "\r\nIt appears something went wrong. Contact an admin. Error code 600.");
					p->set_state(CON_CLOSE);
					return;
				}
				show_finalized_chargen_screen(p);
				return;
			}
			if(choice.compare("M4") == 0){
				p->set_marine(mods::classes::create_marine(p));
				auto sresult = p->cl_marine()->new_player(p,"M4");
				if(sresult < 0){
					write_to_output(p->desc(), "\r\nIt appears something went wrong. Contact an admin. Error code 600.");
					p->set_state(CON_CLOSE);
					return;
				}
				show_finalized_chargen_screen(p);
				return;
			}
			write_to_output(p->desc(), "\r\nI don't get it? Please read the menu and try again...\r\n%s\r\nSelect primary weapon: ", primary_weapon_menu(class_type).data());
			p->set_state(CON_CHARGEN_PRIMARY_CHOICE);
			return;
		}
		if(class_type == CLASS_ENGINEER){
			auto choice = mods::chargen::parse_primary_choice(arg_choice,class_type);
			if(choice.compare("FMG-9") == 0){
				p->set_engineer(mods::classes::create_engineer(p));
				auto sresult = p->cl_engineer()->new_player(p,"M16A4");
				if(sresult < 0){
					write_to_output(p->desc(), "\r\nIt appears something went wrong. Contact an admin. Error code 600.");
					p->set_state(CON_CLOSE);
					return;
				}
				show_finalized_chargen_screen(p);
				return;
			}
			if(choice.compare("M4") == 0){
				p->set_engineer(mods::classes::create_engineer(p));
				auto sresult = p->cl_engineer()->new_player(p,"M4");
				if(sresult < 0){
					write_to_output(p->desc(), "\r\nIt appears something went wrong. Contact an admin. Error code 600.");
					p->set_state(CON_CLOSE);
					return;
				}
				show_finalized_chargen_screen(p);
				return;
			}
			write_to_output(p->desc(), "\r\nI don't get it? Please read the menu and try again...\r\n%s\r\nSelect primary weapon: ", primary_weapon_menu(class_type).data());
			p->set_state(CON_CHARGEN_PRIMARY_CHOICE);
			return;
		}
		if(class_type == CLASS_MEDIC){
			auto choice = mods::chargen::parse_primary_choice(arg_choice,class_type);
			if(choice.compare("AUG-PARA") == 0){
				p->set_medic(mods::classes::create_medic(p));
				auto sresult = p->cl_medic()->new_player(p,"M16A4");
				if(sresult < 0){
					write_to_output(p->desc(), "\r\nIt appears something went wrong. Contact an admin. Error code 600.");
					p->set_state(CON_CLOSE);
					return;
				}
				show_finalized_chargen_screen(p);
				return;
			}
			if(choice.compare("TAR-21") == 0){
				p->set_medic(mods::classes::create_medic(p));
				auto sresult = p->cl_medic()->new_player(p,"M4");
				if(sresult < 0){
					write_to_output(p->desc(), "\r\nIt appears something went wrong. Contact an admin. Error code 600.");
					p->set_state(CON_CLOSE);
					return;
				}
				show_finalized_chargen_screen(p);
				return;
			}
			write_to_output(p->desc(), "\r\nI don't get it? Please read the menu and try again...\r\n%s\r\nSelect primary weapon: ", primary_weapon_menu(class_type).data());
			p->set_state(CON_CHARGEN_PRIMARY_CHOICE);
			return;
		}
		if(class_type == CLASS_PSYOP){
			auto choice = mods::chargen::parse_primary_choice(arg_choice,class_type);
			if(choice.compare("SCAR-H") == 0){
				p->set_psyop(mods::classes::create_psyop(p));
				auto sresult = p->cl_psyop()->new_player(p,"M16A4");
				if(sresult < 0){
					write_to_output(p->desc(), "\r\nIt appears something went wrong. Contact an admin. Error code 600.");
					p->set_state(CON_CLOSE);
					return;
				}
				show_finalized_chargen_screen(p);
				return;
			}
			if(choice.compare("UMP45") == 0){
				p->set_psyop(mods::classes::create_psyop(p));
				auto sresult = p->cl_psyop()->new_player(p,"M4");
				if(sresult < 0){
					write_to_output(p->desc(), "\r\nIt appears something went wrong. Contact an admin. Error code 600.");
					p->set_state(CON_CLOSE);
					return;
				}
				show_finalized_chargen_screen(p);
				return;
			}
			write_to_output(p->desc(), "\r\nI don't get it? Please read the menu and try again...\r\n%s\r\nSelect primary weapon: ", primary_weapon_menu(class_type).data());
			p->set_state(CON_CHARGEN_PRIMARY_CHOICE);
			return;
		}
		if(class_type == CLASS_SUPPORT){
			auto choice = mods::chargen::parse_primary_choice(arg_choice,class_type);
			if(choice.compare("MK46") == 0){
				p->set_support(mods::classes::create_support(p));
				auto sresult = p->cl_support()->new_player(p,"M16A4");
				if(sresult < 0){
					write_to_output(p->desc(), "\r\nIt appears something went wrong. Contact an admin. Error code 600.");
					p->set_state(CON_CLOSE);
					return;
				}
				show_finalized_chargen_screen(p);
				return;
			}
			if(choice.compare("HK21") == 0){
				p->set_support(mods::classes::create_support(p));
				auto sresult = p->cl_support()->new_player(p,"M4");
				if(sresult < 0){
					write_to_output(p->desc(), "\r\nIt appears something went wrong. Contact an admin. Error code 600.");
					p->set_state(CON_CLOSE);
					return;
				}
				show_finalized_chargen_screen(p);
				return;
			}
			write_to_output(p->desc(), "\r\nI don't get it? Please read the menu and try again...\r\n%s\r\nSelect primary weapon: ", primary_weapon_menu(class_type).data());
			p->set_state(CON_CHARGEN_PRIMARY_CHOICE);
			return;
		}
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
				show_finalized_chargen_screen(p);
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
				show_finalized_chargen_screen(p);
				return;
			}
			write_to_output(p->desc(), "\r\nI don't get it? Please read the menu and try again...\r\n%s\r\nSelect primary weapon: ", primary_weapon_menu(class_type).data());
			p->set_state(CON_CHARGEN_PRIMARY_CHOICE);
			return;
		}

		show_finalized_chargen_screen(p);
	}
};
