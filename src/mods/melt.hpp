#ifndef __MENTOC_MODS_melt_HEADER__
#define __MENTOC_MODS_melt_HEADER__
#include "../globals.hpp"

namespace mods::melt {
	static uint32_t melt_id = 0;
	struct melt_damage_t {
		uint32_t id;
		uuid_t attacker;
		uuid_t target;
		int16_t damage;
		bool cleanup;

		std::array<uint16_t,2> dice;
		inline bool operator==(const melt_damage_t& lhs);
		std::string report();
		melt_damage_t() = delete;
		melt_damage_t(const uuid_t& atk_uuid,
		              const uuid_t& target_uuid,
		              const uint16_t in_dice[2],
		              const uint16_t damage
		             )
			: id(++melt_id),
			  attacker(atk_uuid), target(target_uuid),damage(damage),cleanup(false),dice({in_dice[0],in_dice[1]}) {
		}
	};
	void melt_damage(melt_damage_t damage);
	void queue_melt_player(const uuid_t& attacker,const uuid_t& target_uuid,const uint16_t in_dice[2],const int16_t& damage);
	void process_melt();
	void unqueue_player(const uuid_t& player_uuid);
	constexpr uint8_t tick_resolution() {
		return 20;
	}
};
#endif
