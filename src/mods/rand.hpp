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
uint32_t dice(const uint16_t dice[2]);
uint32_t dice(const std::array<uint16_t,2>& dice);
uint32_t dice(const std::array<uint16_t,3>& dice);
template <typename T>
static inline uint32_t roll(const T& yaml) {
	return dice(yaml->attributes->damage_dice_count,yaml->attributes->damage_dice_sides);
}
template <typename Twidth>
struct dice_passer_t {
		dice_passer_t() = delete;
		dice_passer_t(const std::array<Twidth,2>& dice, Twidth pass) :
			m_dice(dice), m_pass(pass) {}
		bool test() {
			return dice(m_dice) >= m_pass;
		}
	private:
		std::array<Twidth,2> m_dice;
		Twidth m_pass;
};
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
static inline bool chance(uint8_t chance) {
	return mods::rand::chance(chance);
}
static inline uint32_t roll(uint32_t num,uint32_t size) {
	return mods::rand::roll(num,size);
}
static inline bool roll_bool() {
	return (rand_xoroshiro() % 2) == 0;
}
#endif
