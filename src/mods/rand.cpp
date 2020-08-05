#include "rand.hpp"
#include <random>
#include <iostream>

extern void log(const char* format,...);

#ifdef  __MENTOC_MODS_RAND_SHOW_DEBUG_OUTPUT__
#define rand_debug(a) mentoc_prefix_debug("mods::rand") << a << "\n";
#else
#define rand_debug(a) ;;
#endif
namespace mods::rand {
	int roll(int number, int size) {
		static std::random_device rd;
		static std::mt19937 serious_generator(rd());
		static std::map<int,std::uniform_int_distribution<int>> popular_distributions;
		if(number < 0){
			rand_debug("SYSERR: roll() should not receive a negative number for count dice");
			return -1;
		}
		auto search = popular_distributions.find(size);
		if(search == popular_distributions.end()){
			rand_debug("[static initialization of distribution] number/size:" << number << "/" << size);
			popular_distributions[size] = std::uniform_int_distribution<int>(1,size);
		}
		int result = 0;
		int rolls = number;
		while(rolls-- > 0){
			result += popular_distributions[size](serious_generator);
		}
		rand_debug("(mods::rand::roll): num,size:" << number << "," << size << "| roll:" << result);
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
		rand_debug("distribution inserting..");
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
	rand_debug("(dice) dice: num,size:" << num << "," << size << "| roll:" << roll);
	return roll;
}
