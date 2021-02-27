#include "calc-visibility.hpp"
#include "../globals.hpp"
#include "rand.hpp"
#include "classes/ghost.hpp"
#include "rooms.hpp"
#include "player-utils.hpp"
#include "object-utils.hpp"

#ifdef __MENTOC_SHOW_MODS_CALC_VISIBILITY_DEBUG_OUTPUT__
#ifdef __MENTOC_SHOW_CALC_VISIBILITY_DUMP_OUTPUT__
#define mcv_debug(MSG) mentoc_prefix_debug("[mods::calc_visibility]")  << info << "|" << MSG << "\n" \
			<< "dump: " << dump << "\n";
#else
#define mcv_debug(MSG) mentoc_prefix_debug("[mods::calc_visibility]")  << info << "|" << MSG << "\n";
#endif
#else
#define mcv_debug(MSG) ;;
#endif


namespace mods::calc_visibility {
	bool is_visible(player_ptr_t& observer,player_ptr_t& target) {
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
		//auto room = observer->viewing_room();
		//auto fire_status = mods::rooms::get_fire_status(room);
		//bool camera_is_thermal = camera != nullptr ? mods::object_utils::is_thermal_camera(camera->object_uuid()) : false;
		//bool camera_is_night_vision = camera != nullptr ? mods::object_utils::is_night_vision_camera(camera->object_uuid()) : false;
		//bool can_see_through_fire = mods::rooms::can_see_through_fire(room);

#ifdef __MENTOC_SHOW_MODS_CALC_VISIBILITY_DEBUG_OUTPUT__
		std::string info = CAT("Observer(",observer->name(),"|",observer->uuid(),")::Target(",target->name(),"|",target->uuid(),")");
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
		                       "target_is_scanned: ",target_is_scanned,"\n",
		                       "target_is_tracked: ",target_is_tracked,"\n");
#endif

		if(observer->has_affect(AFF_BLIND)) {
			mcv_debug("observer is blind");
			return false;
		}
		if(observer->engineer() && target_is_scanned) {
			mcv_debug("Observer is an engineer and target was scanned - returning true");
			return true;
		}
		if(target_is_tracked) {
			return true;
		}
		if(is_dissipated || invisible) {
			mcv_debug("target dissipated/invisible");
			return false;
		}
		if(observer_is_affected_by_emp && (observer_has_thermals || observer_has_night_vision) &&
		        (room_is_dark || smoked_room || room_on_fire)) {
			mcv_debug("observer affected by emp + special vision + dark/smoked/fire");
			return false;
		}
		if((observer_has_thermals || observer_has_night_vision) && room_on_fire) {
			mcv_debug("Observer has special vision and room is on fire");
			return false;
		}
		if(observer_has_thermals && (room_is_dark || smoked_room) && !same_room) {
			mcv_debug("Observer has special vision + dark/smoked + not in same room");
			return false;
		}
		if(observer_has_night_vision && (room_is_dark || smoked_room) && !same_room) {
			mcv_debug("Observer has night vision + dark/smoked + not in same room");
			return false;
		}
		if(target_is_sneaking && (observer_has_thermals || observer_has_night_vision) && !same_room) {
			mcv_debug("target is sneaking + observer has special vision + not in same room");
			return false;
		}
		if(target_is_sneaking && !observer_has_thermals && !observer_has_night_vision) {
			mcv_debug("target is sneaking + observer has normal vision");
			return false;
		}
		if(observer_disoriented && mods::rand::chance(LUCKY_DISORIENTED_VISION_CHANCE()) &&
		        (target_is_sneaking || target->visibility() <= 250)) {
			mcv_debug("observer got very lucky");
			return false;
		}
		mcv_debug("Target is visible");
		return true;
	}
	bool is_visible(uuid_t observer_uuid,uuid_t target_uuid) {
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
		return is_visible(observer,target);
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

