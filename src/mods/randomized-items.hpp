#ifndef __MENTOC_MODS_RANDOMIZED_ITEM_DROPS_HEADER__
#define  __MENTOC_MODS_RANDOMIZED_ITEM_DROPS_HEADER__

#include <iostream>
#include "../structs.h"

namespace mods {
enum Rarity {
	COMMON = 0,
	UNCOMMON,
	RARE,
	ULTRA_RARE,
	LEGENDARY,
	ULTRA_LEGENDARY,
	EXTREME,
	ULTRA_EXTREME,
	ELITE,
	ELITE_EXTREME,
	FIRST = COMMON,
	LAST = ELITE_EXTREME
};
	class randomized_items{
		constexpr static float COMMON_DROP_RATE = 85.0;
		constexpr static float UNCOMMON_DROP_RATE = 15.0;
		constexpr static float RARE_DROP_RATE = 10.0;
		constexpr static float ULTRA_RARE_DROP_RATE = 8.0;
		constexpr static float LEGENDARY_DROP_RATE = 6.0;
		constexpr static float ULTRA_LEGENDARY_DROP_RATE = 3.0;
		constexpr static float EXTREME_DROP_RATE = 1.5;
		constexpr static float ULTRA_EXTREME_DROP_RATE = 0.75;
		constexpr static float ELITE_DROP_RATE = 0.33;
		constexpr static float ELITE_EXTREME_DROP_RATE = 0.01;
		public:
			randomized_items();
			~randomized_items() = default;
		protected:
	};
};

#endif
