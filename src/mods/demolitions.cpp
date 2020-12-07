#include "demolitions.hpp"
#include "help.hpp"
#include "interpreter.hpp"
#include "screen-searcher.hpp"
#include "util.hpp"
#include "object-utils.hpp"
#include "classes/sniper.hpp"
#include "classes/ghost.hpp"

#ifdef __MENTOC_MODS_INTEGRAL_OBJECTS_DEBUG__
#define mo_debug(A) std::cerr << "[mods::demolitions][debug]:" << A <<"\n";
#else
#define mo_debug(A)
#endif

extern std::string sanitize_key(std::string key);
extern void obj_to_obj(obj_ptr_t from_object, obj_ptr_t to_object);
namespace mods::demolitions {
	void plant_claymore(player_ptr_t& player, int direction, obj_ptr_t& obj){
		player->send("You begin installing a %s at the foot of the %s entrance...\r\n", obj->name.c_str(), mods::globals::dir_to_str(direction, true).c_str());
		mods::object_utils::set_is_installing(obj,player,direction);
		player->block_for(CLAYMORE_INSTALLATION_TICS(), mods::deferred::EVENT_PLAYER_UNBLOCK_INSTALLATION, obj->uuid);
	}

	void set_done_installing(uuid_t object_uuid,uuid_t player_uuid){
		auto player = ptr_by_uuid(player_uuid);
		if(player->sniper()){
			player->sniper()->use_claymore(object_uuid);
		}
		if(player->ghost()){
			player->ghost()->use_claymore(object_uuid);
		}
	}

};

#undef mo_debug
