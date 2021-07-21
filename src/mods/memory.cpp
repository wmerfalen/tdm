#include "memory.hpp"
//#include "../db.hpp"
#include "../shop.h"

using shop_data_t = shop_data<mods::orm::shop,mods::orm::shop_rooms,mods::orm::shop_objects>;
using shop_ptr_t = std::shared_ptr<shop_data_t>;
extern std::deque<std::shared_ptr<obj_data>> obj_list;
extern std::deque<std::shared_ptr<mods::npc>> mob_list;
extern std::deque<std::shared_ptr<shop_data_t>> shop_list;
extern std::deque<obj_data> obj_proto;	/* prototypes for objs		 */
extern std::deque<zone_data> zone_table;	/* zone table			 */
namespace mods::memory {
	void print_footprints() {
		std::cerr << "[memory footprint]\n" <<
		          "------------------------------------------------------------------\n" <<
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
		          "------------------------------------------------------------------\n";
		std::cerr << "[obj_list]:\n" <<
		          "------------------------------------------------------------------\n";
		std::map<std::string,std::size_t> counts;
		for(const auto& obj : obj_list) {
			++counts[obj->name.str()];
		}
		for(const auto& pair : counts) {
			std::cerr << green_str("[") << pair.second << green_str("]:") << " " << yellow_str(pair.first) << "\n";
		}
	}
};

