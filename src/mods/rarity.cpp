#include "rarity.hpp"

namespace mods::rarity {
	bool is_common(const float& rarity)  {
		return rarity == COMMON;
	}
	bool is_uncommon(const float& rarity)  {
		return rarity == UNCOMMON;
	}
	bool is_rare(const float& rarity)  {
		return rarity == RARE;
	}

	bool is_legendary(const float& rarity)  {
		return rarity == LEGENDARY;
	}
	bool is_god_tier(const float& rarity)  {
		return rarity == GOD_TIER;
	}
};
