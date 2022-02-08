#include "demolitions.hpp"
#include "help.hpp"
#include "interpreter.hpp"
#include "screen-searcher.hpp"
#include "util.hpp"
#include "object-utils.hpp"
#include "classes/ghost.hpp"

#ifdef __MENTOC_MODS_DEMOLITIONS_DEBUG__
#define mo_debug(A) std::cerr << "[mods::demolitions][debug]:" << A <<"\n";
#else
#define mo_debug(A)
#endif

extern std::string sanitize_key(std::string key);
extern void obj_to_obj(obj_ptr_t from_object, obj_ptr_t to_object);
namespace mods::demolitions {
	void plant_claymore(player_ptr_t& player, int direction, obj_ptr_t& obj) {
		player->sendln(CAT("You begin installing a ",obj->name.c_str()," at the foot of the ",mods::globals::dir_to_str(direction, true).c_str(), " entrance..."));
		mods::object_utils::set_is_installing(obj,player,direction);
		int ticks = CLAYMORE_INSTALLATION_TICS();
		if(player->ghost()) {
			mo_debug("ghost ticks. normal(" << ticks << ")");
			ticks -= ticks * (GHOST_CLAYMORE_INSTALLATION_REDUCTION_PERCENT() * 0.01);
			mo_debug("ghost ticks after nerf: (" << ticks << ")");
		}
		if(ticks < 0) {
			ticks = 0;
		}
		player->block_for(ticks, mods::deferred::EVENT_PLAYER_UNBLOCK_INSTALLATION, obj->uuid);
	}
	void uninstall_claymore(player_ptr_t& player, int direction, obj_ptr_t& obj) {
		player->sendln(CAT("You begin {red}uninstalling{/red} a ", obj->name.c_str(), " at the foot of the ", mods::globals::dir_to_str(direction, true).c_str()," entrance..."));
		mods::object_utils::set_is_uninstalling(obj,player,direction);
		int ticks = CLAYMORE_INSTALLATION_TICS();
		if(player->ghost()) {
			mo_debug("ghost ticks. normal(" << ticks << ")");
			ticks -= ticks * (GHOST_CLAYMORE_INSTALLATION_REDUCTION_PERCENT() * 0.01);
			mo_debug("ghost ticks after nerf: (" << ticks << ")");
		}
		if(ticks < 0) {
			ticks = 0;
		}
		player->block_for(ticks, mods::deferred::EVENT_PLAYER_UNBLOCK_INSTALLATION, obj->uuid);
	}
	std::vector<uuid_t> claymores_in(const room_rnum& room) {
		if(room >= world_size()) {
			return {};
		}
		std::vector<uuid_t> v;
		for(auto i = world[room].contents; i; i = i->next_content) {
			if(i->has_explosive() && i->explosive()->attributes->type == mw_explosive::CLAYMORE_MINE) {
				v.emplace_back(i->uuid);
			}
		}
		return v;
	}

	void set_done_installing(uuid_t object_uuid,uuid_t player_uuid) {
		auto player = ptr_by_uuid(player_uuid);
		if(player->ghost()) {
			player->ghost()->use_claymore(object_uuid);
		}
	}

};

#undef mo_debug
