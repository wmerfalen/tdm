#include "rand.hpp"
#include <random>
#include <iostream>

extern void log(const char* format,...);

/* creates a random number in interval [from;to] */
int rand_number(int from, int to) {
	static std::map<std::tuple<int,int>,std::uniform_int_distribution<int>> distributions;
	std::tuple<int,int> f(from,to);
	if(distributions.find(f) == distributions.end()){
		std::cerr << "distribution inserting..\n";
		distributions[f] = std::uniform_int_distribution<int>(from,to);
	}
	/* error checking in case people call this incorrectly */
	if(from > to) {
		int tmp = from;
		from = to;
		to = tmp;
		log("SYSERR: rand_number() should be called with lowest, then highest. (%d, %d), not (%d, %d).", from, to, to, from);
	}
	return distributions[f](mods::rand::lax_generator);
}

/* simulates dice roll */
int dice(int num, int size) {
	if(size <= 0 || num <= 0) {
		return (-1);
	}

	return mods::rand::roll(num,size);
}
