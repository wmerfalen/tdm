#include "../util.hpp"

int main(int argc,char** argv){
	if(mods::util::fuzzy_match(argv[1],argv[2])){
		std::cout << "1\n";
	}
	return 0;
}
