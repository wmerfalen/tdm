#include "../jx.hpp"

int main(int argc,char** argv){
	mods::jx::compositor jx;
	std::cout << jx.push("foo","bar")
		.object_start("x")
			.push("a","b")
			.push("foo",10.00)
			.array_start("times")
				.push(std::string("10"))
				.push(10)
				.push(100.10)
			.array_end()
		.object_end()
	.get();
	return 0;
}
