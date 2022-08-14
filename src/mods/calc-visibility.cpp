#include "calc-visibility.hpp"
#include "../globals.hpp"
#include "rand.hpp"
#include "classes/ghost.hpp"
#include "rooms.hpp"
#include "player-utils.hpp"
#include "object-utils.hpp"

//#define __MENTOC_SHOW_CALC_VISIBILITY_DUMP_OUTPUT__
#ifdef __MENTOC_SHOW_CALC_VISIBILITY_DUMP_OUTPUT__
#define FOR_WHOM "far"
#define mcv_debug(MSG) if(std::string(observer->name().c_str()).compare(FOR_WHOM) == 0){ mentoc_prefix_debug("[mods::calc_visibility]")  << info << "|" << MSG << "\n" \
			<< "dump: " << dump << "\n"; }
#else
#define mcv_debug(MSG)
#endif


namespace mods::calc_visibility {
	std::tuple<bool,std::string> can_see_object(player_ptr_t& observer,obj_ptr_t& target) {
		const bool invisible = false; /**TODO: target->visibility() == 0;*/
		const bool observer_has_thermals = observer->has_thermal_vision();
		const bool observer_has_night_vision = observer->has_night_vision();
		const bool same_room = observer->viewing_room() == target->in_room;
		const bool smoked_room = mods::rooms::is_smoked(target->in_room);
		const bool observer_disoriented = observer->has_affect(AFF_DISORIENT);
		const bool room_on_fire = mods::rooms::is_on_fire(target->in_room);
		const bool room_is_dark = mods::rooms::is_dark(target->in_room);
		const bool observer_is_affected_by_emp = mods::rooms::has_emp(observer->viewing_room()) || mods::rooms::has_emp(observer->room());
		auto camera = observer->get_camera();
		//auto room = observer->viewing_room();
		//auto fire_status = mods::rooms::get_fire_status(room);
		//bool camera_is_thermal = camera != nullptr ? mods::object_utils::is_thermal_camera(camera->object_uuid()) : false;
		//bool camera_is_night_vision = camera != nullptr ? mods::object_utils::is_night_vision_camera(camera->object_uuid()) : false;
		//bool can_see_through_fire = mods::rooms::can_see_through_fire(room);

		std::string info = CAT("Observer(",observer->name(),"|",observer->uuid(),")::Target(",target->name.c_str(),"|",target->uuid,")");
#define __MENTOC_SHOW_MODS_CALC_VISIBILITY_DEBUG_OUTPUT__
#ifdef __MENTOC_SHOW_MODS_CALC_VISIBILITY_DEBUG_OUTPUT__
		std::string dump = CAT("invisible: ",invisible,"\n",
		                       "observer_has_thermals: ",observer_has_thermals,"\n",
		                       "observer_has_night_vision: ",observer_has_night_vision,"\n",
		                       "same_room: ",same_room,"\n",
		                       "smoked_room: ",smoked_room,"\n",
		                       "observer_disoriented: ",observer_disoriented,"\n",
		                       "room_on_fire: ",room_on_fire,"\n",
		                       "room_is_dark: ",room_is_dark,"\n",
		                       "observer_is_affected_by_emp: ",observer_is_affected_by_emp,"\n"
		                      );
#endif

		if(observer->has_affect(AFF_BLIND)) {
			mcv_debug("observer is blind");
			return {false,"observer is blind"};
		}
		/** TODO scan objects
		if(observer->engineer() && target_is_scanned) {
			mcv_debug("Observer is an engineer and target was scanned - returning true");
			return {true, "Observer is an engineer and target was scanned - returning true"};
		}
		*/
		/** TODO: track objects
		if(target_is_tracked) {
			return {true,"Target is tracked"};
		}
		*/
		if(invisible) {
			mcv_debug("target dissipated/invisible");
			return {false,"target dissipated/invisible"};
		}
		if(observer_is_affected_by_emp && (observer_has_thermals || observer_has_night_vision) &&
		        (room_is_dark || smoked_room || room_on_fire)) {
			mcv_debug("observer affected by emp + special vision + dark/smoked/fire");
			return {false,"observer affected by emp + special vision + dark/smoked/fire"};
		}
		if((observer_has_thermals || observer_has_night_vision) && room_on_fire) {
			mcv_debug("Observer has special vision and room is on fire");
			return {false,"Observer has special vision and room is on fire"};
		}
		if(observer_has_thermals && (room_is_dark || smoked_room) && !same_room) {
			mcv_debug("Observer has special vision + dark/smoked + not in same room");
			return {false,"Observer has special vision + dark/smoked + not in same room"};
		}
		if(observer_has_night_vision && (room_is_dark || smoked_room) && !same_room) {
			mcv_debug("Observer has night vision + dark/smoked + not in same room");
			return {false,"Observer has night vision + dark/smoked + not in same room"};
		}
		/** TODO: visibility for objects
		if(observer_disoriented && mods::rand::chance(LUCKY_DISORIENTED_VISION_CHANCE()) &&
		        (target->visibility() <= 250)) {
			mcv_debug("observer got very lucky");
			return {false, "observer got very lucky"};
		}
		*/
		mcv_debug("Target is visible");
		return {true,"target is visible"};
	}


	std::tuple<bool,std::string> is_visible_with_reason(player_ptr_t& observer,player_ptr_t& target, uint8_t distance=0) {
		if(!target) {
			return {1,"object dead"};
		}
		const bool is_dissipated = target->ghost() && target->ghost()->is_dissipated();
		const bool invisible = target->visibility() == 0;
		const bool observer_has_thermals = observer->has_thermal_vision();
		const bool observer_has_night_vision = observer->has_night_vision();
		const bool same_room = observer->viewing_room() == target->room();
		const bool smoked_room = mods::rooms::is_smoked(target->room());
		const bool observer_disoriented = observer->has_affect(AFF_DISORIENT);
		const bool room_on_fire = mods::rooms::is_on_fire(target->room());
		const bool room_is_dark = mods::rooms::is_dark(target->room());
		const bool target_is_sneaking = target->has_affect(AFF_SNEAK);
		const bool observer_is_affected_by_emp = mods::rooms::has_emp(observer->viewing_room()) || mods::rooms::has_emp(observer->room());
		const bool target_is_scanned = mods::player_utils::is_scanned(target);
		const bool target_is_tracked = target->has_affect(AFF_TRACKED);
		auto camera = observer->get_camera();
		const auto& thermal_range = observer->thermal_range();
		const auto& night_vision_range = observer->night_vision_range();
		//auto room = observer->viewing_room();
		//auto fire_status = mods::rooms::get_fire_status(room);
		//bool camera_is_thermal = camera != nullptr ? mods::object_utils::is_thermal_camera(camera->object_uuid()) : false;
		//bool camera_is_night_vision = camera != nullptr ? mods::object_utils::is_night_vision_camera(camera->object_uuid()) : false;
		//bool can_see_through_fire = mods::rooms::can_see_through_fire(room);

		std::string info = CAT("Observer(",observer->name(),"|",observer->uuid(),")::Target(",target->name(),"|",target->uuid(),")");
#define __MENTOC_SHOW_MODS_CALC_VISIBILITY_DEBUG_OUTPUT__
#ifdef __MENTOC_SHOW_MODS_CALC_VISIBILITY_DEBUG_OUTPUT__
		std::string dump = CAT("is_dissipated: ",is_dissipated,"\n",
		                       "invisible: ",invisible,"\n",
		                       "observer_has_thermals: ",observer_has_thermals,"\n",
		                       "observer_has_night_vision: ",observer_has_night_vision,"\n",
		                       "same_room: ",same_room,"\n",
		                       "smoked_room: ",smoked_room,"\n",
		                       "observer_disoriented: ",observer_disoriented,"\n",
		                       "room_on_fire: ",room_on_fire,"\n",
		                       "room_is_dark: ",room_is_dark,"\n",
		                       "target_is_sneaking: ",target_is_sneaking,"\n",
		                       "observer_is_affected_by_emp: ",observer_is_affected_by_emp,"\n",
		                       "observer_thermal_range:", thermal_range, "\n",
		                       "observer_night_vision_range:", night_vision_range,"\n",
		                       "distance:", distance,"\n",
		                       "target_is_scanned: ",target_is_scanned,"\n",
		                       "target_is_tracked: ",target_is_tracked,"\n");
#endif

		if(observer->has_affect(AFF_BLIND)) {
			mcv_debug("observer is blind");
			return {false,"observer is blind"};
		}
		//if(observer->engineer() && target_is_scanned) {
		//	mcv_debug("Observer is an engineer and target was scanned - returning true");
		//	return {true, "Observer is an engineer and target was scanned - returning true"};
		//}
		if(target_is_tracked) {
			return {true,"Target is tracked"};
		}
		if(observer_is_affected_by_emp && (observer_has_thermals || observer_has_night_vision) &&
		        (room_is_dark || smoked_room || room_on_fire)) {
			mcv_debug("observer affected by emp + special vision + dark/smoked/fire");
			return {false,"observer affected by emp + special vision + dark/smoked/fire"};
		}
		if((observer_has_thermals || observer_has_night_vision) && room_on_fire) {
			mcv_debug("Observer has special vision and room is on fire");
			return {false,"Observer has special vision and room is on fire"};
		}
		/** TODO: fix this crap */
		//if((room_is_dark || smoked_room) && thermal_range >= distance) {
		//	mcv_debug("Observer has special vision + dark/smoked + thermal range is sufficient");
		//	return {true,"Observer has special vision + dark/smoked + thermal range is sufficient"};
		//}
		//if((room_is_dark || !smoked_room) && (0 != night_vision_range) && night_vision_range >= distance) {
		//	mcv_debug("vision + dark + !smoked + night vision range is sufficient");
		//	return {true,"Observer has night vision + dark + !smoked + night vision range is sufficient"};
		//}
		if((is_dissipated || invisible) && (thermal_range <= 0 || thermal_range <= distance)) {
			mcv_debug("target dissipated/invisible");
			return {false,"target dissipated/invisible"};
		}
		if(observer_has_thermals && (room_is_dark || smoked_room) && !same_room) {
			mcv_debug("Observer has special vision + dark/smoked + not in same room");
			return {false,"Observer has special vision + dark/smoked + not in same room"};
		}
		if(observer_has_night_vision && (room_is_dark || smoked_room) && !same_room) {
			mcv_debug("Observer has night vision + dark/smoked + not in same room");
			return {false,"Observer has night vision + dark/smoked + not in same room"};
		}
		if(target_is_sneaking && !observer_has_thermals && !observer_has_night_vision) {
			mcv_debug("target is sneaking + observer has normal vision");
			return {false,"target is sneaking + observer has normal vision"};
		}
		if(observer_disoriented && mods::rand::chance(LUCKY_DISORIENTED_VISION_CHANCE()) &&
		        (target_is_sneaking || target->visibility() <= 250)) {
			mcv_debug("observer got very lucky");
			return {false, "observer got very lucky"};
		}
		mcv_debug("Target is visible");
		return {true,"target is visible"};
	}



	bool roll_victim_spots_attacker(player_ptr_t& victim,player_ptr_t& attacker,const feedback_t& feedback) {
		auto s = is_visible_with_reason(victim,attacker);
		if(std::get<0>(s)) {
			/** 1) allow attacker to use passive abilities to avoid being spotted TODO */
			return true;
		} else {
			/** initial scan says we can't see our target... */
			/** 1) allow victim to use passive abilities to see attacker */
			return false;
		}
	}
	bool is_visible(player_ptr_t& observer,player_ptr_t& target,uint8_t distance) {
		return std::get<0>(is_visible_with_reason(observer,target,distance));
	}
	bool is_visible(uuid_t observer_uuid,uuid_t target_uuid,uint8_t distance) {
		auto target = ptr_by_uuid(target_uuid);
		auto observer = ptr_by_uuid(observer_uuid);
		if(!target) {
#ifndef __MENTOC_DONT_REPORT_IS_VISIBLE_NULLPTR__
			std::cerr << "[mods::calc_visibility::is_visible]: WARNING: target is nullptr\n";
#endif
			return false;
		}
		if(!observer) {
#ifndef __MENTOC_DONT_REPORT_IS_VISIBLE_NULLPTR__
			std::cerr << "[mods::calc_visibility::is_visible]: WARNING: observer is nullptr\n";
#endif
			return false;
		}
		return is_visible(observer,target,distance);
	}
	bool player_can_see_obj(player_ptr_t& player,obj_ptr_t& obj) {
		return true; /* FIXME */
	}
	bool obj_can_see_player(obj_ptr_t& obj,player_ptr_t& target) {
		return true; /** FIXME */
	}
	bool sense_player(obj_ptr_t& device, player_ptr_t& target) {
		return true; /** FIXME */
	}
};

#undef mcv_debug

