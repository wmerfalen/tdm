#ifndef __MENTOC_MODS_PC_ABILITY_HEADER__
#define __MENTOC_MODS_PC_ABILITY_HEADER__

#include "item-types.hpp"
#include "../globals.hpp"
#include <istream>

namespace mods {
	enum PC_CLASS : uint16_t {
		PC_UNKNOWN,
		PC_MARINE,
		PC_CONTAGION,
		PC_GHOST,
		PC_BREACHER,
	};
	struct page {
		std::forward_list<std::string> keywords;
		std::string contents;
	};
	struct str_ability {
		std::string title;
		std::string ability;
		std::string usage;
		std::string description;
		std::forward_list<std::string> examples;
		std::string csv_keywords;
	};

	struct pc_ability {
		pc_ability() = delete;
		pc_ability(PC_CLASS _class,
		    std::string_view _title,
		    std::string_view _ability,
		    std::string_view _usage,
		    std::string_view _desc,
		    std::forward_list<std::string>&& _ex,
		    std::forward_list<std::string>&& _key);
		PC_CLASS player_class;
		std::string title;
		std::string ability;
		std::string usage;
		std::string description;
		std::forward_list<std::string> examples;
		std::forward_list<std::string> keywords;
		std::string created_page;
		page make();
	};
	void register_class_ability(pc_ability& ability);
};
#endif

