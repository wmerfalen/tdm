#include "login.hpp"

namespace login {
	bool is_valid_name(std::string_view name){
		if(name.length() < 2 || name.length() > MAX_NAME_LENGTH){
			return false;
		}
		for(auto c : name){
			if(isalpha(c) || isdigit(c)){
				continue;
			}else{
				return false;
			}
		}
		return true;
	}
};
