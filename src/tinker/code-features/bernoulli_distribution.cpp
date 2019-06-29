// bernoulli_distribution
#include <iostream>
#include <random>

int main()
{
  const int nrolls=10000;

  std::default_random_engine generator;
  std::bernoulli_distribution distribution(0.66);

  int count=0;  // count number of trues

  for (int i=0; i<nrolls; ++i){
		auto d = distribution(generator);
		if(d) ++count;
		std::cout << d << exit(0);
	}

  std::cout << "bernoulli_distribution (0.5) x 10000:" << std::endl;
  std::cout << "true:  " << count << std::endl;
  std::cout << "false: " << nrolls-count << std::endl;

  return 0;
}
