#include "replenish.hpp"
#include "super-users.hpp"
#include "interpreter.hpp"
#include "db.hpp"
#include "util.hpp"
#include "object-utils.hpp"
#include "builder/object-placement.hpp"
#include "rifle-attachments.hpp"
#include "query-objects.hpp"

#ifdef __MENTOC_MODS_INTEGRAL_OBJECTS_DEBUG__
#define mo_debug(A) std::cerr << "[mods::replenish][debug]:" << A <<"\n";
#else
#define mo_debug(A)
#endif

extern std::string sanitize_key(std::string key);
extern void obj_to_obj(obj_ptr_t from_object, obj_ptr_t to_object);
namespace mods::replenish {
	using ghost_ptr_t = std::shared_ptr<mods::classes::ghost>;
	static std::vector<uuid_t> ghost_list;
	static std::map<uuid_t,ghost_ptr_t> ghost_ptrs;

	void register_ghost(uuid_t ghost){
		ghost_list.emplace_back(ghost);
		ghost_ptrs[ghost] = ptr_by_uuid(ghost)->ghost();
	}
	void destroy_player(uuid_t player_uuid){
		ghost_list.erase(std::find(ghost_list.begin(),ghost_list.end(),player_uuid));
		ghost_ptrs.erase(player_uuid);
	}

	/** Depending on PULSE_REPLENISH_CHARACTERS value */
	void run(){
		/** will be called every 5 IRL seconds */
		static uint16_t call_count = 0;
		++call_count;
		if((call_count % GHOST_REPLENISH_PULSE()) == 0){
			for(auto & pair : ghost_ptrs){
				pair.second->replenish();
			}
		}
	}
};
