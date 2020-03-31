#ifndef __MENTOC_MODS_OBJECT_UTILS_HEADER__
#define __MENTOC_MODS_OBJECT_UTILS_HEADER__
#include "../structs.h"

namespace mods::object_utils {
	constexpr static bitvector_t STATUS_INSTALLING = (1 << 0);
	template <typename T>
	static inline bool is_claymore(T& obj){
		if(obj->has_explosive()){
			switch(obj->explosive()->attributes->type){
				case mw_explosive::CLAYMORE_MINE:
					return true;
				default: break;
			}
		}
		return false;
	}
	template <typename T>
	static inline bool is_camera(T& obj){
		if(obj->has_gadget()){
			switch(obj->gadget()->attributes->type){
				case mw_gadget::CAMERA:
				case mw_gadget::THERMAL_CAMERA:
				case mw_gadget::NIGHT_VISION_CAMERA:
					return true;
				default: break;
			}
		}
		return false;
	}
	template <typename T>
		bool is_installed(T& obj){
			if(is_claymore(obj) || is_camera(obj)){
				return obj->location_data() >= 16;
			}
			return false;
		}
	template <typename T>
		bool is_installing(T& obj){
			if(is_claymore(obj) || is_camera(obj)){
				return obj->obj_flags.bitvector & STATUS_INSTALLING;
			}
			return false;
		}
	template <typename T,typename P>
		void set_is_installing(T& obj, P& player,int direction){
			obj->obj_flags.bitvector |= STATUS_INSTALLING;
			obj->in_room = player->room();
			obj->set_owner(player->uuid());
			obj->set_location_data(direction + 16);
		}
	template <typename T>
		void set_done_installing(T& obj){
			obj->obj_flags.bitvector ^= STATUS_INSTALLING;
		}
	template <typename T,typename PlayerT>
		bool is_owner(T& obj, PlayerT& player){
			return player->uuid() == obj->get_owner();
		}
};//End namespace

#endif
