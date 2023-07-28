#include "replenish.hpp"
#include "super-users.hpp"
#include "interpreter.hpp"
#include "db.hpp"
#include "util.hpp"
#include "object-utils.hpp"
#include "builder/object-placement.hpp"
#include "rifle-attachments.hpp"
#include "query-objects.hpp"
#include "classes/includes.hpp"

#ifdef __MENTOC_MODS_INTEGRAL_OBJECTS_DEBUG__
	#define mo_debug(A) std::cerr << "[mods::replenish][debug]:" << A <<"\n";
#else
	#define mo_debug(A)
#endif

extern std::string sanitize_key(std::string key);
extern void obj_to_obj(obj_ptr_t from_object, obj_ptr_t to_object);
namespace mods::replenish {
	using breacher_ptr_t = std::shared_ptr<mods::classes::breacher>;
	using marine_ptr_t = std::shared_ptr<mods::classes::marine>;
	using ghost_ptr_t = std::shared_ptr<mods::classes::ghost>;
	using contagion_ptr_t = std::shared_ptr<mods::classes::contagion>;
	static std::vector<uuid_t> marine_list;
	static std::vector<uuid_t> breacher_list;
	static std::vector<uuid_t> ghost_list;
	static std::vector<uuid_t> contagion_list;

	static std::map<uuid_t,marine_ptr_t> marine_ptrs;
	static std::map<uuid_t,breacher_ptr_t> breacher_ptrs;
	static std::map<uuid_t,ghost_ptr_t> ghost_ptrs;
	static std::map<uuid_t,contagion_ptr_t> contagion_ptrs;

	void register_marine(uuid_t marine) {
		marine_list.emplace_back(marine);
		marine_ptrs[marine] = ptr_by_uuid(marine)->marine();
	}
	void register_breacher(uuid_t breacher) {
		breacher_list.emplace_back(breacher);
		breacher_ptrs[breacher] = ptr_by_uuid(breacher)->breacher();
	}
	void register_ghost(uuid_t ghost) {
		ghost_list.emplace_back(ghost);
		ghost_ptrs[ghost] = ptr_by_uuid(ghost)->ghost();
	}
	void register_contagion(uuid_t contagion) {
#ifdef CONTAGIONS_CAN_REPLENISH
		contagion_list.emplace_back(contagion);
		contagion_ptrs[contagion] = ptr_by_uuid(contagion)->contagion();
#endif
	}
	void destroy_player(uuid_t player_uuid) {
		if(std::find(marine_list.begin(),marine_list.end(),player_uuid) != marine_list.end()) {
			marine_list.erase(std::find(marine_list.begin(),marine_list.end(),player_uuid));
			marine_ptrs.erase(player_uuid);
		}
		if(std::find(breacher_list.begin(),breacher_list.end(),player_uuid) != breacher_list.end()) {
			breacher_list.erase(std::find(breacher_list.begin(),breacher_list.end(),player_uuid));
			breacher_ptrs.erase(player_uuid);
		}
		if(std::find(ghost_list.begin(),ghost_list.end(),player_uuid) != ghost_list.end()) {
			ghost_list.erase(std::find(ghost_list.begin(),ghost_list.end(),player_uuid));
			ghost_ptrs.erase(player_uuid);
		}
#ifdef CONTAGIONS_CAN_REPLENISH
		if(std::find(contagion_list.begin(),contagion_list.end(),player_uuid) != contagion_list.end()) {
			contagion_list.erase(std::find(contagion_list.begin(),contagion_list.end(),player_uuid));
			contagion_ptrs.erase(player_uuid);
		}
#endif
	}

	/** Depending on PULSE_REPLENISH_CHARACTERS value */
	void run() {
		/** will be called every 5 IRL seconds */
		static uint16_t call_count = 0;
		++call_count;
#ifdef CONTAGIONS_CAN_REPLENISH
		for(auto& pair : contagion_ptrs) {
			if(pair.second->get_player_ptr()->state() != CON_PLAYING) {
				continue;
			}
			pair.second->replenish();
		}
#endif
		for(auto& pair : marine_ptrs) {
			if(pair.second->get_player_ptr()->state() != CON_PLAYING) {
				continue;
			}
			pair.second->replenish();
		}
		for(auto& pair : breacher_ptrs) {
			if(pair.second->get_player_ptr()->state() != CON_PLAYING) {
				continue;
			}
			pair.second->replenish();
		}
		for(auto& pair : ghost_ptrs) {
			if(pair.second->get_player_ptr()->state() != CON_PLAYING) {
				continue;
			}
			pair.second->replenish();
		}
	}
};
