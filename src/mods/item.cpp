#include "item.hpp"
#include "../globals.hpp"

namespace mods::item {
		bool is_capable_of_single(player_ptr_t& player, obj_ptr_t& object, capability_t action){
			return false;
		}
		bool is_capable_of_all(player_ptr_t& player, obj_ptr_t& object, cap_list_t& actions){
			return false;
		}
		bool is_capable_of_any(player_ptr_t& player, obj_ptr_t& object, cap_list_t& actions){
			return false;
		}
};
