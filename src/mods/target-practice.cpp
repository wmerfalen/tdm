#include "target-practice.hpp"
#include "target-practice-db.hpp"
#include "help.hpp"
#include "super-users.hpp"
#include "interpreter.hpp"
#include "db.hpp"
#include "util.hpp"
#include "zone.hpp"

#ifdef __MENTOC_MODS_TARGET_PRACTICE_DEBUG__
#define mtp_debug(A) std::cerr << "[mods::target_practice][debug]:" << A <<"\n";
#else
#define mtp_debug(A)
#endif

namespace mods::target_practice {
	void instantiate_dummy(player_ptr_t& player,std::string_view identifier){
		//TODO:
	}
};

ACMD(do_install_dummy){
	ADMIN_REJECT();
	DO_HELP("install_dummy");
	/** code here */
	auto vec_args = PARSE_ARGS();
	mods::target_practice_db::save_dummy(player,vec_args);
	mods::zone::register_replenish(world[player->room()].number,"dummy");
	mods::zone::remove_replenish(world[player->room()].number,"dummy");
	ADMIN_DONE();
}

ACMD(do_uninstall_dummy){
	ADMIN_REJECT();
	DO_HELP("uninstall_dummy");
	/** code here */
	auto vec_args = PARSE_ARGS();
	mods::target_practice_db::remove_dummy(player,vec_args);
	ADMIN_DONE();
}

ACMD(do_instantiate_dummy){
	ADMIN_REJECT();
	DO_HELP("instantiate_dummy");
	/** code here */
	auto vec_args = PARSE_ARGS();
	static constexpr const char* usage = "usage: instantiate_dummy <identifier-with-no-spaces>";
	if(vec_args.size() < 1){
		player->errorln(usage);
		return;
	}
	mods::target_practice::instantiate_dummy(player,vec_args[0]);
	ADMIN_DONE();
}
namespace mods::target_practice {
	void init(){
		mods::interpreter::add_command("install_dummy", POS_RESTING, do_install_dummy, LVL_BUILDER,0);
		mods::interpreter::add_command("uninstall_dummy", POS_RESTING, do_uninstall_dummy, LVL_BUILDER,0);
		mods::interpreter::add_command("instantiate_dummy", POS_RESTING, do_instantiate_dummy, LVL_BUILDER,0);
	}
};
#undef mtp_debug
