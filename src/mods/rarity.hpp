#ifndef __MENTOC_MODS_RARITY_HEADER__
#define  __MENTOC_MODS_RARITY_HEADER__

#include <iostream>

namespace mods::rarity {
	constexpr static float COMMON = 0.2510437;
	constexpr static float UNCOMMON = 0.1503172;
	constexpr static float RARE = 0.05034415;
	constexpr static float LEGENDARY = 0.0100341;
	constexpr static float GOD_TIER = 0.001311304;
	constexpr static float DEFAULT = mods::rarity::COMMON;
	bool is_common(const  float& rarity) ;
	bool is_uncommon(const  float& rarity) ;
	bool is_rare(const  float& rarity) ;
	bool is_legendary(const  float& rarity) ;
	bool is_god_tier(const  float& rarity) ;
};

#endif
