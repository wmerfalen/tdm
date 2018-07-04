#include <iostream>

struct foo {
	int x,y;
};

int m(foo f){
	std::cerr << f.x << "\n";
	return f.x;
}

int main(int argc,char** argv){
	return m({.x = 1111111110,.y = 1});
}

