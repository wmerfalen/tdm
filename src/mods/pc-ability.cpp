#include "pc-ability.hpp"
#include <map>
#include <string>

namespace mods {
	std::forward_list<page> marine_help;
	std::forward_list<page> contagion_help;
	std::forward_list<page> breacher_help;
	std::forward_list<page> ghost_help;

	void init() {
#if 0
		for(const auto& ability : mods::classes::marine_help()){
	std::pair<bool,std::string> pair = register_class_ability(
			"MARINE",
	    ability.title,
	    ability._ability,
	    ability.usage,
	    ability.description,
	    ability.examples,
	    ability.cssv_keywords);
		}
#endif
	}
	pc_ability::pc_ability(PC_CLASS _class,
	    std::string_view _title,
	    std::string_view _ability,
	    std::string_view _usage,
	    std::string_view _desc,
	    std::forward_list<std::string>&& _ex,
	    std::forward_list<std::string>&& _key) :
		player_class(_class),
		title(_title),
		ability(_ability),
		usage(_usage),
		description(_desc),
		examples(std::move(_ex)),
		keywords(std::move(_key))

	{
	}
	PC_CLASS to_pc(std::string_view c) {
		if(c.compare("MARINE") == 0) {
			return PC_CLASS::PC_MARINE;
		}
		if(c.compare("GHOST") == 0) {
			return PC_CLASS::PC_GHOST;
		}
		if(c.compare("CONTAGION") == 0) {
			return PC_CLASS::PC_CONTAGION;
		}
		if(c.compare("BREACHER") == 0) {
			return PC_CLASS::PC_BREACHER;
		}
		return PC_CLASS::PC_UNKNOWN;
	}
	std::forward_list<std::string> csv_to_list(std::string_view str) {
		std::forward_list<std::string> list;
		std::string current;
		for(const auto& ch : str) {
			if(ch == ',' && current.length()) {
				list.push_front(current);
				current.clear();
				continue;
			}
			current += ch;
		}
		if(current.length()) {
			list.push_front(current);
		}
		return list;
	}
	std::pair<bool,std::string> register_class_ability(
	    std::string_view player_class,
	    std::string_view title,
	    std::string_view _ability,
	    std::string_view usage,
	    std::string_view description,
	    std::forward_list<std::string>&& examples,
	    std::string_view csv_keywords) {
		if(to_pc(player_class) == PC_UNKNOWN) {
			return {false,"Invalid class"};
		}
		pc_ability ability(to_pc(player_class),title,_ability,usage,description,std::move(examples),csv_to_list(csv_keywords));
		switch(ability.player_class) {
			case PC_CONTAGION:
				contagion_help.push_front(std::move(ability.make()));
				break;
			case PC_MARINE:
				marine_help.push_front(std::move(ability.make()));
				break;
			case PC_BREACHER:
				breacher_help.push_front(std::move(ability.make()));
				break;
			case PC_GHOST:
				ghost_help.push_front(std::move(ability.make()));
				break;
			default:
				// This will never happen since we're doign that with to_pc() above
				break;
		}
		return {true,"created"};
	}
	page pc_ability::make() {
		/**
		   creates this part:
		    "_____________________________\r\n"
		    " MARINE CLASS WEAPON         |\r\n"
		    "_____________________________|\r\n"
		*/
		std::string title_wrapper;
		for(uint16_t i=0; i < title.length() + 4; i++) {
			title_wrapper += "_";
		}
		title_wrapper += "\r\n";
		created_page = title_wrapper;
		created_page += " ";
		created_page += title + " |\r\n";
		created_page += title_wrapper;

		/**
		 * creates this part:
		    " Ability:     Spawn your class weapon\r\n"
		    " Usage:       marine:giveme_m16\r\n"
		    "{hr}"
		*/
		created_page += " Ability:      ";
		created_page += ability + "\r\n";
		created_page += " Usage:        ";
		created_page += usage + "\r\n{hr}";
		/**
		 * creates this part:
		    " Description: This command will spawn a M16A4 Assault Rifle to your primary  \r\n"
		    "              weapon slot.\r\n"
		    "\r\n"
		 */
		created_page += " Description: ";
		/**
		 * TODO: FIXME: need word wrap
		 */
		created_page += description + "\r\n";
		/**
		 * Creates this part:
		    "EXAMPLE:\r\n"
		    "{hr}"
		    "$> marine:giveme_m16\r\n"
		    "You equip your M16A into your primary weapon slot.\r\n"
		    "\r\n";
		 */
		for(const auto& example : examples) {
			created_page += "EXAMPLE:\r\n{hr}";
			created_page += example;
			created_page += "\r\n";
		}
		page p;
		p.keywords = keywords;
		p.contents = created_page;
		return p;
	}
};
