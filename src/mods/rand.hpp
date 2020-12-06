#ifndef __MENTOC_MODS_RAND_HEADER__
#define __MENTOC_MODS_RAND_HEADER__
#include <random>	//C++ library
#include <iostream>
#include <string>
#include <map>

/* creates a random number in interval [from;to] */
int rand_number(int from, int to);
/* simulates dice roll */
int dice(int num, int size);
namespace mods::rand {
	int roll(int num,int size);
	bool chance(uint8_t chance);
};
#endif
