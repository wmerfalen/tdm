#include "rand.hpp"
#include <random>
#include <iostream>
#include <chrono>
#include "interpreter.hpp"

extern void log(const char* format,...);

#ifdef  __MENTOC_MODS_RAND_SHOW_DEBUG_OUTPUT__
#define rand_debug(a) mentoc_prefix_debug("mods::rand") << a << "\n";
#else
#define rand_debug(a) ;;
#endif
namespace mods::rand {
	SUPERCMD(do_roll_dice) {
		if(argshave()->size_gt(1)->nth_has_integer({0,1})->passed() == false) {
			player->sendln("Usage: roll_dice <count> <sides>");
			return;
		}

		player->sendln(CAT("rolling ",intat(0),"d",intat(1),": ",roll(intat(0),intat(1))));
	}
	SUPERCMD(do_roll_dice_average) {
		if(argshave()->size_gt(1)->nth_has_integer({0,1})->passed() == false) {
			player->sendln("Usage: roll_dice_average <count> <sides>");
			return;
		}
		player->sendln(CAT(intat(0),"d",intat(1),": HIGH(",intat(0) * intat(1),") LOW(",intat(0),") AVERAGE(",((intat(1) + 1)/2) * intat(0),")"));
	}
	uint32_t roll(uint32_t number, uint32_t size) {
		static std::random_device rd;
		static std::mt19937 serious_generator(rd());
		static std::map<uint32_t,std::uniform_int_distribution<uint32_t>> popular_distributions;
		if(number < 0) {
			rand_debug("SYSERR: roll() should not receive a negative number for count dice");
			return -1;
		}
		auto search = popular_distributions.find(size);
		if(search == popular_distributions.end()) {
			rand_debug("[static initialization of distribution] number/size:" << number << "/" << size);
			popular_distributions[size] = std::uniform_int_distribution<uint32_t>(1,size);
		}
		uint32_t result = 0;
		uint32_t rolls = number;
		while(rolls-- > 0) {
			result += popular_distributions[size](serious_generator);
		}
		rand_debug("(mods::rand::roll): num,size:" << number << "," << size << "| roll:" << result);
		return result;
	}
	bool chance(uint8_t chance) {
		if(chance >= 100) {
			return true;
		}
		uint8_t percent = 100 - chance;
		return dice(1,100) >= percent;
	}
};

/* creates a random number in interval [from;to] */
uint32_t rand_number(uint32_t from, uint32_t to) {
	static std::random_device rd;
	static std::mt19937 serious_generator(rd());
	static std::map<std::tuple<uint32_t,uint32_t>,std::uniform_int_distribution<uint32_t>> distributions;
	std::tuple<uint32_t,uint64_t> f(from,to);
	if(distributions.find(f) == distributions.end()) {
		rand_debug("distribution inserting..");
		distributions[f] = std::uniform_int_distribution<uint32_t>(from,to);
	}
	/* error checking in case people call this incorrectly */
	if(from > to) {
		uint32_t tmp = from;
		from = to;
		to = tmp;
		log("SYSERR: rand_number() should be called with lowest, then highest. (%d, %d), not (%d, %d).", from, to, to, from);
	}
	return distributions[f](serious_generator);
}

uint32_t dice(uint32_t num,uint32_t size) {
	uint32_t roll = mods::rand::roll(num,size);
	rand_debug("(dice) dice: num,size:" << num << "," << size << "| roll:" << roll);
	return roll;
}
uint32_t dice(const uint16_t dice [2]) {
	return mods::rand::roll(dice[0],dice[1]);
}
uint32_t dice(const std::array<uint16_t,2>& dice) {
	return mods::rand::roll(dice[0],dice[1]);
}

uint32_t dice(const std::array<uint16_t,3>& dice) {
	auto roll = mods::rand::roll(dice[0],dice[1]);
	if(roll > dice[2]) {
		return roll;
	}
	return 0;
}

namespace mods::rand::xoroshiro {
	/*  Written in 2018 by David Blackman and Sebastiano Vigna (vigna@acm.org)

	To the extent possible under law, the author has dedicated all copyright
	and related and neighboring rights to this software to the public domain
	worldwide. This software is distributed without any warranty.

	See <http://creativecommons.org/publicdomain/zero/1.0/>. */

#include <stdint.h>

	/* This is xoshiro256** 1.0, one of our all-purpose, rock-solid
		 generators. It has excellent (sub-ns) speed, a state (256 bits) that is
		 large enough for any parallel application, and it passes all tests we
		 are aware of.

		 For generating just floating-point numbers, xoshiro256+ is even faster.

		 The state must be seeded so that it is not everywhere zero. If you have
		 a 64-bit seed, we suggest to seed a splitmix64 generator and use its
		 output to fill s. */

	static inline uint64_t rotl(const uint64_t x, uint32_t k) {
		return (x << k) | (x >> (64 - k));
	}


	static uint64_t s[4];

	uint64_t next(void) {
		const uint64_t result = rotl(s[1] * 5, 7) * 9;

		const uint64_t t = s[1] << 17;

		s[2] ^= s[0];
		s[3] ^= s[1];
		s[1] ^= s[2];
		s[0] ^= s[3];

		s[2] ^= t;

		s[3] = rotl(s[3], 45);

		return result;
	}


	/* This is the jump function for the generator. It is equivalent
		 to 2^128 calls to next(); it can be used to generate 2^128
		 non-overlapping subsequences for parallel computations. */

	void jump(void) {
		static const uint64_t JUMP[] = { 0x180ec6d33cfd0aba, 0xd5a61266f0c9392c, 0xa9582618e03fc9aa, 0x39abdc4529b1661c };

		uint64_t s0 = 0;
		uint64_t s1 = 0;
		uint64_t s2 = 0;
		uint64_t s3 = 0;
		for(uint32_t i = 0; i < sizeof JUMP / sizeof *JUMP; i++)
			for(uint32_t b = 0; b < 64; b++) {
				if(JUMP[i] & UINT64_C(1) << b) {
					s0 ^= s[0];
					s1 ^= s[1];
					s2 ^= s[2];
					s3 ^= s[3];
				}
				next();
			}

		s[0] = s0;
		s[1] = s1;
		s[2] = s2;
		s[3] = s3;
	}



	/* This is the long-jump function for the generator. It is equivalent to
		 2^192 calls to next(); it can be used to generate 2^64 starting points,
		 from each of which jump() will generate 2^64 non-overlapping
		 subsequences for parallel distributed computations. */

	void long_jump(void) {
		static const uint64_t LONG_JUMP[] = { 0x76e15d3efefdcbbf, 0xc5004e441c522fb3, 0x77710069854ee241, 0x39109bb02acbe635 };

		uint64_t s0 = 0;
		uint64_t s1 = 0;
		uint64_t s2 = 0;
		uint64_t s3 = 0;
		for(uint32_t i = 0; i < sizeof LONG_JUMP / sizeof *LONG_JUMP; i++)
			for(uint32_t b = 0; b < 64; b++) {
				if(LONG_JUMP[i] & UINT64_C(1) << b) {
					s0 ^= s[0];
					s1 ^= s[1];
					s2 ^= s[2];
					s3 ^= s[3];
				}
				next();
			}

		s[0] = s0;
		s[1] = s1;
		s[2] = s2;
		s[3] = s3;
	}
};//end namespace mods::rand::xoroshiro

uint64_t rand_xoroshiro() {
	return mods::rand::xoroshiro::next();
}

namespace mods::rand {
	void init() {
		using namespace std::chrono;
		for(uint8_t i = 0; i < 4; i++) {
			mods::rand::xoroshiro::s[i] = duration_cast< milliseconds >(system_clock::now().time_since_epoch()).count();
		}
		ADD_BUILDER_COMMAND("roll_dice",do_roll_dice);
		ADD_BUILDER_COMMAND("roll_dice_average",do_roll_dice_average);
		ADD_BUILDER_COMMAND("rda",do_roll_dice_average);
	}
};
