#include "player-registration.hpp"
#include "help.hpp"
#include "super-users.hpp"

namespace mods::player_registration {
	static bool registration_allowed = 1;
	void allow(bool on_off){
		registration_allowed = on_off;
	}
	bool allowed() {
		return registration_allowed;
	}

};

ACMD(do_enable_registration){
	DO_HELP("enable_registration");
	ADMIN_REJECT();
	mods::player_registration::allow(true);
	ADMIN_DONE();
}
ACMD(do_disable_registration){
	DO_HELP("disable_registration");
	ADMIN_REJECT();
	mods::player_registration::allow(false);
	ADMIN_DONE();
}

ACMD(do_registration_status){
	DO_HELP("registration_status");
	player->sendln(
			mods::player_registration::allowed() ?
			green_str("[+] Allowed.") 
			: red_str("[-] Not Allowed.")
	);
}
