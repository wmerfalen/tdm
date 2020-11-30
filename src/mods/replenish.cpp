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
	using sniper_ptr_t = std::shared_ptr<mods::classes::sniper>;
	using breacher_ptr_t = std::shared_ptr<mods::classes::breacher>;
	using marine_ptr_t = std::shared_ptr<mods::classes::marine>;
	using engineer_ptr_t = std::shared_ptr<mods::classes::engineer>;
	using ghost_ptr_t = std::shared_ptr<mods::classes::ghost>;
	using medic_ptr_t = std::shared_ptr<mods::classes::medic>;
	using support_ptr_t = std::shared_ptr<mods::classes::support>;
	static std::vector<uuid_t> sniper_list;
	static std::vector<uuid_t> marine_list;
	static std::vector<uuid_t> breacher_list;
	static std::vector<uuid_t> engineer_list;
	static std::vector<uuid_t> medic_list;
	static std::vector<uuid_t> ghost_list;
	static std::vector<uuid_t> support_list;

	static std::map<uuid_t,sniper_ptr_t> sniper_ptrs;
	static std::map<uuid_t,marine_ptr_t> marine_ptrs;
	static std::map<uuid_t,breacher_ptr_t> breacher_ptrs;
	static std::map<uuid_t,engineer_ptr_t> engineer_ptrs;
	static std::map<uuid_t,medic_ptr_t> medic_ptrs;
	static std::map<uuid_t,ghost_ptr_t> ghost_ptrs;
	static std::map<uuid_t,support_ptr_t> support_ptrs;

	void register_sniper(uuid_t sniper){
		sniper_list.emplace_back(sniper);
		sniper_ptrs[sniper] = ptr_by_uuid(sniper)->sniper();
	}
	void register_marine(uuid_t marine){
		marine_list.emplace_back(marine);
		marine_ptrs[marine] = ptr_by_uuid(marine)->marine();
	}
	void register_breacher(uuid_t breacher){
		breacher_list.emplace_back(breacher);
		breacher_ptrs[breacher] = ptr_by_uuid(breacher)->breacher();
	}
	void register_engineer(uuid_t engineer){
		engineer_list.emplace_back(engineer);
		engineer_ptrs[engineer] = ptr_by_uuid(engineer)->engineer();
	}
	void register_ghost(uuid_t ghost){
		ghost_list.emplace_back(ghost);
		ghost_ptrs[ghost] = ptr_by_uuid(ghost)->ghost();
	}
	void register_medic(uuid_t medic){
		medic_list.emplace_back(medic);
		medic_ptrs[medic] = ptr_by_uuid(medic)->medic();
	}
	void register_support(uuid_t support){
		support_list.emplace_back(support);
		support_ptrs[support] = ptr_by_uuid(support)->support();
	}
	void destroy_player(uuid_t player_uuid){
		if(std::find(sniper_list.begin(),sniper_list.end(),player_uuid) != sniper_list.end()){
			sniper_list.erase(std::find(sniper_list.begin(),sniper_list.end(),player_uuid));
			sniper_ptrs.erase(player_uuid);
		}
		if(std::find(marine_list.begin(),marine_list.end(),player_uuid) != marine_list.end()){
			marine_list.erase(std::find(marine_list.begin(),marine_list.end(),player_uuid));
			marine_ptrs.erase(player_uuid);
		}
		if(std::find(breacher_list.begin(),breacher_list.end(),player_uuid) != breacher_list.end()){
			breacher_list.erase(std::find(breacher_list.begin(),breacher_list.end(),player_uuid));
			breacher_ptrs.erase(player_uuid);
		}
		if(std::find(engineer_list.begin(),engineer_list.end(),player_uuid) != engineer_list.end()){
			engineer_list.erase(std::find(engineer_list.begin(),engineer_list.end(),player_uuid));
			engineer_ptrs.erase(player_uuid);
		}
		if(std::find(ghost_list.begin(),ghost_list.end(),player_uuid) != ghost_list.end()){
			ghost_list.erase(std::find(ghost_list.begin(),ghost_list.end(),player_uuid));
			ghost_ptrs.erase(player_uuid);
		}
		if(std::find(medic_list.begin(),medic_list.end(),player_uuid) != medic_list.end()){
			medic_list.erase(std::find(medic_list.begin(),medic_list.end(),player_uuid));
			medic_ptrs.erase(player_uuid);
		}
		if(std::find(support_list.begin(),support_list.end(),player_uuid) != support_list.end()){
			support_list.erase(std::find(support_list.begin(),support_list.end(),player_uuid));
			support_ptrs.erase(player_uuid);
		}
	}

	/** Depending on PULSE_REPLENISH_CHARACTERS value */
	void run(){
		/** will be called every 5 IRL seconds */
		static uint16_t call_count = 0;
		++call_count;
		for(auto & pair : sniper_ptrs){
			pair.second->replenish();
		}
		for(auto & pair : marine_ptrs){
			pair.second->replenish();
		}
		for(auto & pair : breacher_ptrs){
			pair.second->replenish();
		}
		for(auto & pair : engineer_ptrs){
			pair.second->replenish();
		}
		for(auto & pair : ghost_ptrs){
			pair.second->replenish();
		}
		for(auto & pair : medic_ptrs){
			pair.second->replenish();
		}
		for(auto & pair : support_ptrs){
			pair.second->replenish();
		}
	}
};
