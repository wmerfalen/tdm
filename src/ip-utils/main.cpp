#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>


int main(int argc,char** argv) {
	std::string host = "198.199.114.15";
	uint16_t port = 25033;

	std::string cmd = "/bin/nc -p 25033 -u -q 0 198.199.114.15 25033 < ../list";
	int i=0;
	while(1) {
		std::cout << ". ";
		system(cmd.c_str());
		usleep(812800);
		++i;
		if(0 == (i % 80)) {
			std::cout << "\n";
		}
	}
	return 0;
}
