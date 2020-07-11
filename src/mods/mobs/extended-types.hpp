#ifndef __MENTOC_MODS_MOBS_EXTENDED_TYPES_HEADER__
#define  __MENTOC_MODS_MOBS_EXTENDED_TYPES_HEADER__

#include <iostream>
#include <map>
#include <string>
#include <optional>
#include <vector>

namespace mods::mobs {
	enum extended_types_t {
		NONE =0,
		MINI_GUNNER,
		INNOCENT,
		SENTINEL,
		AGGRESSIVE_MELEE,
		AGGRESSIVE_RIFLER,
		LUNATIC,
		GRUNT
	};
	//vim sorcery: s/\t\t\([A-Z_]\+\),/\t\t{\1,"\1"},/g
	static std::map<extended_types_t,std::string> ext_map = {
		{NONE,"NONE"},
		{MINI_GUNNER,"MINI_GUNNER"},
		{INNOCENT,"INNOCENT"},
		{SENTINEL,"SENTINEL"},
		{AGGRESSIVE_MELEE,"AGGRESSIVE_MELEE"},
		{AGGRESSIVE_RIFLER,"AGGRESSIVE_RIFLER"},
		{LUNATIC,"LUNATIC"},
		{GRUNT,"GRUNT"},
	};

	namespace desc {
	static const std::string mini_gunner = std::string("The mob will have a backpack and minigun which he/she will use as the main weapon.") +
			("Mini gunner mobs are usually very slow and take a long time to adjust which direction ") +
			("they are attempting to spray toward. Once they have found a target, they will spray in that ") +
			("direction. There is one caveat with the mini-gunner and that is HEAT. Their mini-gun overheats ") +
			("frequently and requires their constant spraying to cease every once in awhile.");
	static const std::string innocent = std::string("Innocent mobs that wouldn't harm a fly (even when provoked). These mobs fulfill the ") +
			("victim archetype, often being in the middle of unfortunate situations despite their willingness ") +
			("to actively avoid any conflict. An innocent mob will do anything as long as it means they can ") +
			("avoid losing their life.");
	};
	static std::map<extended_types_t,std::string> description_strings = {
		{NONE,"NONE"},
		{MINI_GUNNER,desc::mini_gunner},
		{INNOCENT,desc::innocent},
		{SENTINEL,"SENTINEL"},
		{AGGRESSIVE_MELEE,"AGGRESSIVE_MELEE"},
		{AGGRESSIVE_RIFLER,"AGGRESSIVE_RIFLER"},
		{LUNATIC,"LUNATIC"},
		{GRUNT,"GRUNT"},
	};
	namespace extended_types {
		std::vector<std::string> strings();
		std::optional<extended_types_t> from_string(std::string);
		std::string to_string(extended_types_t t);
		std::string description(std::string str);
		std::string description(extended_types_t type);
	};
};
#endif
