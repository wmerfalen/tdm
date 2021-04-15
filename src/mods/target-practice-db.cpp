#include "integral-objects-db.hpp"
#include "help.hpp"
#include "super-users.hpp"
#include "interpreter.hpp"
#include "orm/integral-object.hpp"
#include "screen-searcher.hpp"
#include "db.hpp"
#include "util.hpp"
#include "object-utils.hpp"
#include "zone.hpp"
#include "builder/object-placement.hpp"
#include "rifle-attachments.hpp"

#ifdef __MENTOC_MODS_TARGET_PRACTICE_DEBUG__
#define mtp_debug(A) std::cerr << "[mods::target_practice][debug]:" << A <<"\n";
#else
#define mtp_debug(A)
#endif

extern std::string sanitize_key(std::string key);
extern void obj_to_obj(obj_ptr_t from_object, obj_ptr_t to_object);
namespace mods::target_practice_db {
	static std::vector<room_vnum> dummy_queue;
	void save_dummy(player_ptr_t& player, std::vector<std::string>& args) {
		mtp_debug("saving practice dummy");
		save_item_to_db(player, "dummy", args);
	}
	void remove_dummy(player_ptr_t& player, std::vector<std::string>& args) {
		using namespace mods::db;
		auto status = delete_section_vector("dummy",std::to_string(world[player->room()].number));
		player->send("delete status: %d\r\n",status);
	}
	void queue_dummy_on_room(room_vnum room) {
		dummy_queue.emplace_back(room);
	}
};

#undef mtp_debug
