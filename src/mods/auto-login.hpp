#ifndef __MENTOC_MODS_AUTO_LOGIN_HEADER__
#define  __MENTOC_MODS_AUTO_LOGIN_HEADER__

#include <iostream>
#include <string>

namespace mods::auto_login {
	static std::string user;
	static std::string password;
	void set_user(std::string_view user);
	const std::string& get_user();
	void set_password(std::string_view pass);
	const std::string& get_password();
};

#endif
