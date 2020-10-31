#include "zone.hpp"
#include "integral-objects.hpp"

#define z_debug(A) std::cerr << "[mods::zone debug]" << A << "\n";
#define rr_debug(A) std::cerr << "[run_replenish]:" << A << "\n";
namespace mods::zone {
static std::deque<replenish_command> replenish;

void build_weapon_locker(room_vnum room){
	z_debug("building weapon locker");
	auto obj = create_object(ITEM_CONTAINER, "weapon-locker.yml");
	obj_to_room(obj.get(),real_room(room));
}
void build_armor_locker(room_vnum room){
	z_debug("building armor locker");
	auto obj = create_object(ITEM_CONTAINER, "armor-locker.yml");
	obj_to_room(obj.get(),real_room(room));
}
void build_camera_feed(room_vnum room){
	z_debug("building camera feed");
	auto obj = create_object(ITEM_GADGET, "camera.yml");
	obj_to_room(obj.get(),real_room(room));
}
void register_replenish(room_vnum room,std::string type){
	replenish_command c;
	c.room = room;
	c.type = type;
	mods::zone::replenish.emplace_back(c);
}
void remove_replenish(room_vnum room,std::string type){
		for(auto it = replenish.begin(); it != replenish.end(); ++it){
			if(it->room == room && it->type.compare(type) == 0){
				replenish.erase(it);
				remove_replenish(room,type);
				return;
			}
		}
}

	void run_replenish(){
		rr_debug("run replenish!");
		for(auto command : replenish){
			rr_debug("running command:" << command.type);
			if(command.type.compare("weapon-locker") == 0){
				rr_debug("weapon locker command type found. running feed");
				mods::integral_objects::feed_weapon_locker(command.room);
			}
			if(command.type.compare("armor-locker") == 0){
				rr_debug("armor locker command type found. running feed");
				mods::integral_objects::feed_armor_locker(command.room);
			}
			if(command.type.compare("camera-feed") == 0){
				rr_debug("camera feed command type found. running feed");
				mods::integral_objects::feed_camera_feed(command.room);
				mods::integral_objects::rotate_camera_feed(command.room);
			}
		}
	}
	void zone_update(){
		z_debug("zone update. running replenish");
		run_replenish();
	}
	void new_room(room_data* room_ptr){
		if(mods::db::vector_exists("weapon-locker",std::to_string(room_ptr->number))){
			z_debug("Found weapon locker in room_ptr->number: " << room_ptr->number);
			build_weapon_locker(room_ptr->number);
			register_replenish(room_ptr->number,"weapon-locker");
		}
		if(mods::db::vector_exists("armor-locker",std::to_string(room_ptr->number))){
			z_debug("Found armor locker in room_ptr->number: " << room_ptr->number);
			build_armor_locker(room_ptr->number);
			register_replenish(room_ptr->number,"armor-locker");
		}
		if(mods::db::vector_exists("camera-feed",std::to_string(room_ptr->number))){
			z_debug("Found camera feed in room_ptr->number: " << room_ptr->number);
			build_camera_feed(room_ptr->number);
			register_replenish(room_ptr->number,"camera-feed");
		}
	}
};
#undef rr_debug
#undef z_debug
