#ifndef __MENTOC_MODS_RAND_HEADER__
#define __MENTOC_MODS_RAND_HEADER__
#include <random>	//C++ library
#include <iostream>
#include <string>
#include <map>

namespace mods::rand {
	static bool initialized = false;
	static std::random_device rd;
	static std::mt19937 serious_generator(rd());

	static std::default_random_engine lax_generator;
	static constexpr uint8_t dice_sides_index[] = {
		4,
		6,
		8,
		10,
		50,
		80,
		0
	};
	static std::map<int,std::uniform_int_distribution<int>> popular_distributions;
	static void init() {
		for(unsigned i=0; dice_sides_index[i]; i++){
			popular_distributions[dice_sides_index[i]] = std::uniform_int_distribution<int>(1,dice_sides_index[i]);
		}
		initialized = 1;
	}

	static inline int roll(int number, int size) {
		if(number < 0){
			std::cerr << "SYSERR: roll() should not receive a negative number for count dice\n";
			return -1;
		}
		if(!mods::rand::initialized){
			mods::rand::init();
		}
		auto search = popular_distributions.find(size);
		if(search == popular_distributions.end()){
			popular_distributions[size] = std::uniform_int_distribution<int>(1,size);
		}
		auto& dist = popular_distributions[size];
		int result = 0;
		while(number-- > 0){
			result += dist(mods::rand::lax_generator);
		}
		return result;
	}
};
/* creates a random number in interval [from;to] */
int rand_number(int from, int to);
/* simulates dice roll */
int dice(int num, int size);

#endif
