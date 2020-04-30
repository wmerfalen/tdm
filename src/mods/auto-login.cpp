#include "auto-login.hpp"
namespace mods::auto_login {
	void set_user(std::string_view user){
		mods::auto_login::user = user.data();
	}
	const std::string& get_user() { return mods::auto_login::user; }
	void set_password(std::string_view pass){
		mods::auto_login::password = pass.data();
	}
	const std::string& get_password() { return mods::auto_login::password; }
	bool auto_login_enabled(){ return mods::auto_login::get_user().length(); }
};
