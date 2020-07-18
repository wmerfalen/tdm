#include "rand.hpp"
#include <random>
#include <iostream>

extern void log(const char* format,...);

namespace mods::rand {
	int roll(int number, int size) {
		static std::random_device rd;
		static std::mt19937 serious_generator(rd());
		static std::map<int,std::uniform_int_distribution<int>> popular_distributions;
		if(number < 0){
			std::cerr << "SYSERR: roll() should not receive a negative number for count dice\n";
			return -1;
		}
		auto search = popular_distributions.find(size);
		if(search == popular_distributions.end()){
			std::cerr << "[static initialization of distribution] number/size:" << number << "/" << size << "\n";
			popular_distributions[size] = std::uniform_int_distribution<int>(1,size);
		}
		int result = 0;
		int rolls = number;
		while(rolls-- > 0){
			result += popular_distributions[size](serious_generator);
		}
		std::cerr << "(mods::rand::roll): num,size:" << number << "," << size << "| roll:" << result << "\n";
		return result;
	}
};

/* creates a random number in interval [from;to] */
int rand_number(int from, int to) {
	static std::random_device rd;
	static std::mt19937 serious_generator(rd());
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
	return distributions[f](serious_generator);
}

int dice(int num,int size){
	int roll = mods::rand::roll(num,size);
	std::cerr << "(dice) dice: num,size:" << num << "," << size << "| roll:" << roll << "\n";
	return roll;
}
