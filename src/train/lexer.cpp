#include <iostream>
#include <vector>

struct char_type {
	char_type(){ std::cout << "[constructed]char_type\n"; }
	char_type(char_type& copy){ std::cout << "[copy_constructor]char_type\n"; }
	char_type(char c): m_character(c) { std::cout << "[initializer_list]char_type\n"; }
	operator char(){ return m_character; }
	private:
	char m_character;
};
template <typename T>
struct ctx {
	typename T::iterator begin;
	typename T::iterator end;
};
template <typename T,typename ReturnType>
ReturnType lex(T & context){
	return std::move(*context.begin++);
}

template <typename T,typename ReturnType>
ReturnType lex_copy(T & context){
	return *context.begin++;
}
int main(int argc,char** argv){
	using container = std::vector<char_type>;
	using ctx_type = ctx<container>;
	ctx_type context;
	container input;
	std::string str_input;
	std::getline(std::cin,str_input);
	for(auto &c : str_input){
		input.push_back(std::forward<char_type>(c));
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
