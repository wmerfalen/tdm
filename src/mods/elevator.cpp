#include "elevator.hpp"
#include "help.hpp"
#include "super-users.hpp"
#include "interpreter.hpp"
#include "orm/elevator.hpp"
#include "screen-searcher.hpp"
#include "db.hpp"
#include "util.hpp"
#include "rooms.hpp"

#ifdef __MENTOC_MODS_ELEVATOR_DEBUG__
#define mo_debug(A) std::cerr << "[mods::elevator][debug]:" << A <<"\n";
#else
#define mo_debug(A)
#endif

extern std::string sanitize_key(std::string key);
extern void obj_to_obj(obj_ptr_t from_object, obj_ptr_t to_object);
namespace mods::elevator {
	void instantiate_elevator_here(room_rnum room_id) {
		mods::rooms::set_sector_type(room_id, mods::rooms::sector_type_t::INDOOR_ELEVATOR);
	}
};

SUPERCMD(do_instantiate_elevator_here) {
	ADMIN_REJECT();
	DO_HELP("instantiate_elevator_here");
	/** code here */
	auto vec_args = PARSE_ARGS();
	mods::elevator::instantiate_elevator_here(player->room());
	ADMIN_DONE();
}
namespace mods::elevator {
	void init() {
		mods::interpreter::add_command("instantiate_elevator_here", POS_RESTING, do_instantiate_elevator_here, LVL_BUILDER,0);
	}
};
#undef mo_debug
