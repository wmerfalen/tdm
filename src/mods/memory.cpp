#include "memory.hpp"
//#include "../db.hpp"
#include "../shop.h"
#include "mobs/car-thief.hpp"
#include "mobs/mp-shotgunner.hpp"
#include "mobs/mini-gunner.hpp"
#include "mobs/chaotic-meth-addict.hpp"
#include "mobs/generic-thief.hpp"

using shop_data_t = shop_data<mods::orm::shop,mods::orm::shop_rooms,mods::orm::shop_objects>;
using shop_ptr_t = std::shared_ptr<shop_data_t>;
namespace mods::behaviour_tree_impl {
	extern std::vector<uuid_t>& mob_list();
};
namespace mods::weapons::corrosive_claymore_memory {
	std::string get();
};
namespace mods::weapons::shrapnel_claymore_memory {
	std::string get();
};
extern std::deque<std::shared_ptr<obj_data>> obj_list;
extern std::deque<std::shared_ptr<mods::npc>> mob_list;
extern std::deque<std::shared_ptr<shop_data_t>> shop_list;
extern std::deque<obj_data> obj_proto;	/* prototypes for objs		 */
extern std::deque<zone_data> zone_table;	/* zone table			 */
namespace mods::memory {
	std::map<std::string,str_map_t> usage_report;
	template <typename T>
	void save_footprint(T& ptr) {
		usage_report[CAT(ptr->type().data(),":",ptr->uuid)] = ptr->usages();
	}
	template <typename T>
	void save_counts(std::map<uuid_t,T>& map,std::string_view type) {
		usage_report[std::string(type.data()) + ".size()"] = {{"size",std::to_string(map.size())}};
	}
	template <typename T>
	void save_counts(std::forward_list<T>& l,std::string_view type) {
		usage_report[std::string(type.data()) + ".size()"] = {{"size",std::to_string(std::distance(l.cbegin(),l.cend()))}};
	}
	std::string get_saved_footprints() {
		std::string s;
		std::map<std::string,std::size_t> counts;
		for(const auto& pair : usage_report) {
			if(pair.second.size()) {
				for(const auto& ipair : pair.second) {
					auto type = CAT(EXPLODE(pair.first,':')[0],"|",ipair.first);
					int i = mods::util::stoi(ipair.second).value_or(0);
					counts[type] += i;
				}
			}
		}
		for(const auto& pair : counts) {
			s += CAT(pair.first,": ",pair.second,"\n");
		}
		return s;
	}
	void print_footprints() {
		for(const auto& g : mods::mobs::generic_thief_list()) {
			save_footprint(g);
		}
		save_counts(mods::mobs::generic_thief_list(),"generic_thief");
		for(const auto& g : mods::mobs::chaotic_meth_addict_list()) {
			save_footprint(g);
		}
		save_counts(mods::mobs::chaotic_meth_addict_list(),"chaotic_meth_addict");
		for(const auto& pair : mods::mobs::car_thief_map()) {
			auto& g = pair.second;
			save_footprint(g);
		}
		save_counts(mods::mobs::car_thief_map(),"car_thief");
		for(const auto& pair : mods::mobs::car_thief_map()) {
			std::cerr << "car_thief hp: " << pair.second->player()->hp() << "\n";
		}
		for(const auto& pair : mods::mobs::mpshotgunner_map()) {
			auto& g = pair.second;
			save_footprint(g);
		}
		save_counts(mods::mobs::mpshotgunner_map(),"mpshotgunner");
		for(const auto& pair : mods::mobs::mg_map()) {
			auto& g = pair.second;
			save_footprint(g);
		}
		auto cor = mods::weapons::corrosive_claymore_memory::get();
		auto shrapnel = mods::weapons::shrapnel_claymore_memory::get();
		std::size_t watchers = 0;
		for(const auto& room : world) {
			watchers += room.watchers.size();
		}
		std::cerr << "[memory footprint]\n" <<
		          "------------------------------------------------------------------\n" <<
		          "world.watchers:" << watchers << "\n" <<
		          "obj_list: " << obj_list.size() << "\n" <<
		          "obj_map: " << mods::globals::obj_map.size() << "\n" <<
		          "socket_map: " << mods::globals::socket_map.size() << "\n" <<
		          "obj_odmap: " << mods::globals::obj_odmap.size() << "\n" <<
		          "player_chmap: " << mods::globals::player_chmap.size() << "\n" <<
		          "player_name_map: " << mods::globals::player_name_map.size() << "\n" <<
		          "room_list: " << mods::globals::room_list.size() << "\n" <<
		          "player_list: " << mods::globals::player_list.size() << "\n" <<
		          "mob_list: " << mob_list.size() << "\n" <<
		          "ram_db: " << mods::globals::ram_db.size() << "\n" <<
		          "obj_stat_pages: " << mods::globals::obj_stat_pages.size() << "\n" <<
		          "current_tick: " << mods::globals::current_tick << "\n" <<
		          "player_map: " << mods::globals::player_map.size() << "\n" <<
		          "obj_proto: " << obj_proto.size() << "\n" <<
		          "zone_table: " << zone_table.size() << "\n" <<
		          "mods::behaviour_tree_impl: " << mods::behaviour_tree_impl::mob_list().size() << "\n" <<
		          get_saved_footprints() <<
		          "corrosive_claymore: " << cor << "\n" <<
		          "shrapnel_claymore: " << shrapnel << "\n" <<
		          "------------------------------------------------------------------\n";

#ifdef __MENTOC_SHOW_OBJ_LIST_MEMORY_OUTPUT__
		std::cerr << "[obj_list]:\n" <<
		          "------------------------------------------------------------------\n";
		std::map<std::string,std::size_t> counts;
		for(const auto& obj : obj_list) {
			++counts[obj->name.str()];
		}
		for(const auto& pair : counts) {
			std::cerr << green_str("[") << pair.second << green_str("]:") << " " << yellow_str(pair.first) << "\n";
		}
		std::cerr << "\n" <<
		          "------------------------------------------------------------------\n";
#endif
	}
};

