#include <iostream>
#include <vector>

template <class ... Args>
struct lexer {
};
enum token_type {
	string_literal,number,quoted_string
};
template <typename T> 
std::function<T,(std::string_view)> expect(){
	return [](std::string_view chunk) -> T { 
		return T(chunk); 
	};
}
template<> 
std::function<std::string,(std::string_view)> expect<token_type::string_literal>{
	return [](std::string_view chunk) -> std::string {
		return std::string(chunk);
	};
};
template<>
std::function<int,(std::string_view)> expect<token_type::number>{
	return [](std::string_view chunk) -> int {
		return std::stoi(chunk.data());
	};
}
int main(int argc,char** argv){
	std::string str_input;
	std::getline(std::cin,str_input);
	for(auto &c : str_input){
		input.push_back({c});
	}
	context.begin = input.begin();
	context.end = input.end();
	lex<ctx_type,char_type>(context);
	lex_copy<ctx_type,char_type>(context);
	for(auto it = context.begin; it != context.end; ++it){
		std::cout << static_cast<char>(*it) << ":";
	}
	std::cout << "\n";
	return 0;
}
