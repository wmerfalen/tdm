#ifndef __MENTOC_MODS_RAND_HEADER__
#define __MENTOC_MODS_RAND_HEADER__
#include <random>
#include <iostream>
#include <string>
#include <map>
#include <algorithm>

/* creates a random number in interval [from;to] */
uint32_t rand_number(uint32_t from, uint32_t to);
/* simulates dice roll */
uint32_t dice(uint32_t num, uint32_t size);
template <typename T>
static inline uint32_t roll(const T& yaml) {
	return dice(yaml->attributes->damage_dice_count,yaml->attributes->damage_dice_sides);
}
namespace mods::rand {
	uint32_t roll(uint32_t num,uint32_t size);
	bool chance(uint8_t chance);

	template <typename T>
	static inline T shuffle_container(const T& v) {
		std::random_device rd;
		std::mt19937 g(rd());
		auto f = v;
		std::shuffle(f.begin(), f.end(), g);
		return f;
	}
};
uint64_t rand_xoroshiro();
#endif
