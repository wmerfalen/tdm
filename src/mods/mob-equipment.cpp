#include "mob-equipment.hpp"
#include "npc.hpp"
#include "super-users.hpp"
#include "interpreter.hpp"
#include "screen-searcher.hpp"

#ifdef  __MENTOC_MODS_MOBS_HELPERS_SHOW_DEBUG_OUTPUT__
#define m_debug(a) mentoc_prefix_debug("mods::mob_equipment") << a << "\n";
#define m_error(a) mentoc_prefix_debug("mods::mob_equipment") << a << "\n";
#else
#define m_debug(a) ;;
#define m_error(a)
#endif

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
	void decorate(const uuid_t& mob_uuid) {
		m_debug("decorate entrance");
		auto npc = npc_by_uuid(mob_uuid);
		if(!npc) {
			m_error("Couldn't find npc by uuid");
			std::cerr << red_str("mods::mob_equipment::decorate ERROR: failed to find mob by uuid:'") << mob_uuid << "'\n";
			return;
		}
		m_debug("Fetching list by mob vnum:" << npc->vnum());
		const auto mapping = fetch_list_by_mob_vnum(npc->vnum());
		for(unsigned i = 0; i < NUM_WEARS; i++) {
			if(mapping[i].length() == 0) {
				continue;
			}
			m_debug("Found slot at[" << i << "]:'" << green_str(mapping[i]) << "'\n");
			std::tuple<int,std::string> s = mods::util::extract_yaml_info_from_path(mapping[i]);
			m_debug("Yaml info: " << std::get<0>(s) << ", '" << std::get<1>(s) << "'\n");
			auto obj = create_object(std::get<0>(s),std::get<1>(s));
			npc->equip(obj,i);
		}
		m_debug("Done equipping mob");
	}
	SUPERCMD(do_npc_report) {
		ADMIN_REJECT();
		for(const auto& player : mods::globals::get_room_list(player->room())) {
			player->report();
		}
		ADMIN_DONE();
	}
	void init() {
		mods::interpreter::add_command("npc_report", POS_RESTING, do_npc_report, LVL_BUILDER,0);
	}
};
#undef m_debug
#undef m_error
