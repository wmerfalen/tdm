#include "zone.hpp"
#include "integral-objects.hpp"

namespace mods::zone {
static std::deque<replenish_command> replenish;

void build_weapon_locker(room_vnum room){
	auto obj = create_object(ITEM_CONTAINER, "weapon-locker.yml");
	obj_to_room(obj.get(),real_room(room));
	mods::integral_objects::feed_weapon_locker(room);
}
void build_armor_locker(room_vnum room){
	auto obj = create_object(ITEM_CONTAINER, "armor-locker.yml");
	obj_to_room(obj.get(),real_room(room));
	mods::integral_objects::feed_armor_locker(room);
}
void register_replenish(room_vnum room,std::string type){
	replenish_command c;
	c.room = room;
	c.type = type;
	replenish.emplace_back(c);
}

	void run_replenish(){
		std::vector<std::string> values;
		for(auto & command : replenish){
			values.clear();
			if(command.type.compare("weapon-locker") == 0){
				mods::integral_objects::feed_weapon_locker(command.room);
			}
			if(command.type.compare("armor-locker") == 0){
				mods::integral_objects::feed_armor_locker(command.room);
			}
		}
	}
	void zone_update(){

	}
	void new_room(room_data* room){
		if(mods::db::vector_exists("weapon-locker",std::to_string(room->number))){
			build_weapon_locker(room->number);
			register_replenish(room->number,"weapon-locker");
		}
		if(mods::db::vector_exists("armor-locker",std::to_string(room->number))){
			build_armor_locker(room->number);
			register_replenish(room->number,"armor-locker");
		}
	}
};
