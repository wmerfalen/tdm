#include "mob-equipment.hpp"

namespace mods::mob_equipment {
	std::map<mob_vnum,std::array<std::string,NUM_WEARS>>& meq_mappings() {
		static std::map<mob_vnum,std::array<std::string,NUM_WEARS>> list;
		return list;
	}


	void refresh_mappings() {
		meq_mappings().clear();
		for(const auto& mapping : mods::orm::mob_equipment_map_list()) {
			for(const auto& eq_list : mods::orm::mob_equipment_list()) {
				if(eq_list->meq_vnum == mapping->mmap_mob_equipment_vnum) {
					meq_mappings()[mapping->mmap_mob_vnum] = eq_list->convert_to_array();
					break;
				}
			}
		}
	}

	std::array<std::string,NUM_WEARS> fetch_list_by_mob_vnum(mob_vnum mobvnum) {
		return meq_mappings()[mobvnum];
	}
};
