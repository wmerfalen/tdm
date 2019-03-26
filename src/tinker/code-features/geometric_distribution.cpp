#include <iostream>
#include <random>

int main() {
  constexpr int nrolls = 10000; // number of experiments
  constexpr int nstars = 100;   // maximum number of stars to distribute

  std::default_random_engine generator;
  std::geometric_distribution<int> distribution(0.3);

  int p[10]={};

  for (int i=0; i<nrolls; ++i) {
    int number = distribution(generator);
    if (number<10) ++p[number];
  }

  std::cout << "geometric_distribution (0.3):" << std::endl;
  for (int i=0; i<10; ++i)
    std::cout << i << ": " << std::string(p[i]*nstars/nrolls,'*') << std::endl;

  return 0;
}
