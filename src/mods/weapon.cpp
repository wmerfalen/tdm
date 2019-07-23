#include "weapon.hpp"

namespace mods::weapon {
	base::rifle rifle(std::shared_ptr<obj_data>& object){
		return static_cast<base::rifle>(object->wpn_base);
	}
	base::explosive explosive(std::shared_ptr<obj_data>& object){
		return static_cast<base::explosive>(object->wpn_base);
	}
	base::drone drone(std::shared_ptr<obj_data>& object){
		return static_cast<base::drone>(object->wpn_base);
	}
	std::variant<base::rifle,base::explosive,base::drone> 
		type(std::shared_ptr<obj_data>& o){
			switch(o->wpn_type){
				case type_t::EXPLOSIVE:
					return explosive(o);
				case type_t::DRONE:
					return drone(o);
				default:
				case type_t::RIFLE:
					return rifle(o);
			}
	}

};
