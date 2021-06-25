#ifndef __MENTOC_MODS_RAND_HEADER__
#define __MENTOC_MODS_RAND_HEADER__
#include <random>
#include <iostream>
#include <string>
#include <map>
#include <algorithm>

/* creates a random number in interval [from;to] */
int rand_number(int from, int to);
/* simulates dice roll */
int dice(int num, int size);
namespace mods::rand {
	int roll(int num,int size);
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
