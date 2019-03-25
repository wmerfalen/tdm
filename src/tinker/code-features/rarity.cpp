// uniform_int_distribution
#include <iostream>
#include <random>

enum Rarity {
	COMMON = 0,
	UNCOMMON,
	RARE,
	ULTRA_RARE,
	LEGENDARY,
	ULTRA_LEGENDARY,
	EXTREME,
	ULTRA_EXTREME,
	ELITE,
	ELITE_EXTREME,
	FIRST = COMMON,
	LAST = ELITE_EXTREME
};
	

int main()
{
  const int nrolls = 10000; // number of experiments
  const int nstars = 95;    // maximum number of stars to distribute

  std::default_random_engine generator;
  //std::uniform_int_distribution<int> distribution(
	//		static_cast<int>(Rarity::FIRST),
	//		static_cast<int>(Rarity::LAST)
	//);
  std::geometric_distribution<int> distribution(
		0.3);
			//static_cast<int>(Rarity::FIRST),
			//static_cast<int>(Rarity::LAST)
	//);

  int p[10]={};

  for (int i=0; i<nrolls; ++i) {
    int number = distribution(generator);
#define PR_ENUM(a){ case a: std::cout << #a << "\n"; break; }
		switch(static_cast<Rarity>(number)){
	PR_ENUM(COMMON);
	PR_ENUM(UNCOMMON);
	PR_ENUM(RARE);
	PR_ENUM(ULTRA_RARE);
	PR_ENUM(LEGENDARY);
	PR_ENUM(ULTRA_LEGENDARY);
	PR_ENUM(EXTREME);
	PR_ENUM(ULTRA_EXTREME);
	PR_ENUM(ELITE);
			default: std::cerr << "[ignored]: " << number << "\n";break;
		}
  }

		/*
  std::cout << "uniform_int_distribution (0,9):" << std::endl;
  for (int i=0; i<10; ++i)
    std::cout << i << ": " << std::string(p[i]*nstars/nrolls,'*') << std::endl;
		*/

  return 0;
}
