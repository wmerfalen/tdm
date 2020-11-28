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
	using pyrexia_ptr_t = std::shared_ptr<mods::classes::pyrexia>;
	using forge_ptr_t = std::shared_ptr<mods::classes::forge>;
	using syndrome_ptr_t = std::shared_ptr<mods::classes::syndrome>;
	static std::vector<uuid_t> ghost_list;
	static std::vector<uuid_t> pyrexia_list;
	static std::vector<uuid_t> forge_list;
	static std::vector<uuid_t> syndrome_list;
	static std::map<uuid_t,ghost_ptr_t> ghost_ptrs;
	static std::map<uuid_t,pyrexia_ptr_t> pyrexia_ptrs;
	static std::map<uuid_t,forge_ptr_t> forge_ptrs;
	static std::map<uuid_t,syndrome_ptr_t> syndrome_ptrs;

	void register_ghost(uuid_t ghost){
		ghost_list.emplace_back(ghost);
		ghost_ptrs[ghost] = ptr_by_uuid(ghost)->ghost();
	}
	void register_forge(uuid_t forge){
		forge_list.emplace_back(forge);
		forge_ptrs[forge] = ptr_by_uuid(forge)->forge();
	}
	void register_pyrexia(uuid_t pyrexia){
		pyrexia_list.emplace_back(pyrexia);
		pyrexia_ptrs[pyrexia] = ptr_by_uuid(pyrexia)->pyrexia();
	}
	void register_syndrome(uuid_t syndrome){
		syndrome_list.emplace_back(syndrome);
		syndrome_ptrs[syndrome] = ptr_by_uuid(syndrome)->syndrome();
	}
	void destroy_player(uuid_t player_uuid){
		if(std::find(ghost_list.begin(),ghost_list.end(),player_uuid) != ghost_list.end()){
			ghost_list.erase(std::find(ghost_list.begin(),ghost_list.end(),player_uuid));
			ghost_ptrs.erase(player_uuid);
		}
		if(std::find(forge_list.begin(),forge_list.end(),player_uuid) != forge_list.end()){
			forge_list.erase(std::find(forge_list.begin(),forge_list.end(),player_uuid));
			forge_ptrs.erase(player_uuid);
		}
		if(std::find(pyrexia_list.begin(),pyrexia_list.end(),player_uuid) != pyrexia_list.end()){
			pyrexia_list.erase(std::find(pyrexia_list.begin(),pyrexia_list.end(),player_uuid));
			pyrexia_ptrs.erase(player_uuid);
		}
		if(std::find(syndrome_list.begin(),syndrome_list.end(),player_uuid) != syndrome_list.end()){
			syndrome_list.erase(std::find(syndrome_list.begin(),syndrome_list.end(),player_uuid));
			syndrome_ptrs.erase(player_uuid);
		}
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
		if((call_count % PYREXIA_REPLENISH_PULSE()) == 0){
			for(auto & pair : pyrexia_ptrs){
				pair.second->replenish();
			}
		}
		if((call_count % FORGE_REPLENISH_PULSE()) == 0){
			for(auto & pair : forge_ptrs){
				pair.second->replenish();
			}
		}
		if((call_count % SYNDROME_REPLENISH_PULSE()) == 0){
			for(auto & pair : syndrome_ptrs){
				pair.second->replenish();
			}
		}
	}
};
