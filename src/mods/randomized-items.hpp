#ifndef __MENTOC_MODS_RANDOMIZED_ITEM_DROPS_HEADER__
#define  __MENTOC_MODS_RANDOMIZED_ITEM_DROPS_HEADER__

#include <iostream>
#include "../structs.h"

namespace mods {
enum rarity_enum_t {
	COMMON = 0,
	UNCOMMON,
	RARE,
	LEGENDARY,
	ELITE,
	FIRST = COMMON,
	LAST = ELITE
};
	class randomized_items{
		constexpr static float COMMON_DROP_RATE = 85.0;
		constexpr static float UNCOMMON_DROP_RATE = 15.0;
		constexpr static float RARE_DROP_RATE = 6.0;
		constexpr static float LEGENDARY_DROP_RATE = 3.0;
		constexpr static float ELITE_DROP_RATE = 0.033;
		public:
			randomized_items();
			
			~randomized_items() = default;
		protected:
	};
};

#endif
