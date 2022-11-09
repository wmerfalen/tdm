#ifndef __MENTOC_BUILDER_FLAGS_HEADER__
#define __MENTOC_BUILDER_FLAGS_HEADER__
#include <string>
#include <array>
#include <map>
#include <tuple>
#include "weapon.hpp"
#include "../structs.h"
#include "../db.h"

namespace mods::builder {
	extern std::array<std::pair<int,std::string>,13> weapon_type_flags;
	extern std::array<std::pair<int,std::string>,3> sex_flags;
	extern std::array<std::pair<int,std::string>,20> mob_act_flags;
	extern std::array<std::pair<int,std::string>,22> aff_flags;
	extern std::array<std::pair<int,std::string>,9> position_flags;
	extern std::array<std::pair<int,std::string>,3> type2_flags;
	extern std::array<std::pair<int,std::string>,24> type_flags;
	extern std::array<std::pair<int,std::string>,17> extra_flags;
	extern std::array<std::pair<int,std::string>,38> affected_flags;
	extern std::array<std::pair<int,std::string>,15> wear_flags;

};
#endif
