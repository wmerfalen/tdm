#include "rooms.hpp"
#include "weapon-types.hpp"

namespace mods::rooms {
	namespace affects {
		void add_fire(room_rnum room){
			start_fire_dissolver(room);
		}
		void remove_fire(room_rnum room){
			stop_fire_dissolver(room);
		}
		void process(){
			for(auto it = needs_dissolve.begin();it != needs_dissolve.end();++it){
				auto ticked = it->second.tick();
				auto fire_amount = it->second.get_affects()[(affect_t)room_data::texture_type_t::ON_FIRE];
				if(ticked == 0 && fire_amount == 0){
					mra_debug("Reached zero on: " << it->first);
					needs_dissolve.erase(it);
					mods::rooms::affects::process();
					return;
				}
			}
		}
	};
};//End namespace
