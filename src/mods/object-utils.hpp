#ifndef __MENTOC_MODS_OBJECT_UTILS_HEADER__
#define __MENTOC_MODS_OBJECT_UTILS_HEADER__
#include "../globals.hpp"
extern obj_ptr_t optr_by_uuid(uuid_t);

namespace mods::object_utils {
	constexpr static bitvector_t STATUS_INSTALLING = (1 << 0);
	constexpr static bitvector_t STATUS_BREACHING = (1 << 1);
	constexpr static obj_data::location_data_t INSTALL_MASK = 16;
	constexpr static obj_data::location_data_t BREACH_MASK = 32;
	constexpr static uint8_t INSTALL_TICKS_DURATION = 30;
	constexpr static uint8_t BREACH_TICKS_DURATION = 15;
	constexpr static uint8_t THERMITE_BREACH_TICKS_DURATION = 35;
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
	static inline mw_explosive get_explosive_type(T& obj){
		return static_cast<mw_explosive>(obj->explosive()->attributes->type);
	}
	template <>
	inline mw_explosive get_explosive_type<uuid_t>(uuid_t& obj_uuid){
		return static_cast<mw_explosive>(optr_by_uuid(obj_uuid)->explosive()->attributes->type);
	}
	template <typename T>
	static inline bool is_breach_charge(T& obj){
		if(obj->has_explosive()){
			switch(obj->explosive()->attributes->type){
				case mw_explosive::BREACH_CHARGE:
				case mw_explosive::THERMITE_CHARGE:
					return true;
				default: break;
			}
		}
		return false;
	}
	template <typename T>
		bool is_installed(T& obj){
			if(is_claymore(obj) || is_camera(obj)){
				return obj->location_data() & INSTALL_MASK;
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
		void set_is_breaching(T& obj, P& player,int direction){
			obj->obj_flags.bitvector |= STATUS_BREACHING;
			obj->in_room = player->room();
			obj->set_owner(player->uuid());
			obj->set_location_data(direction + BREACH_MASK);
		}
	template <typename T,typename P>
		void set_is_installing(T& obj, P& player,int direction){
			obj->obj_flags.bitvector |= STATUS_INSTALLING;
			obj->in_room = player->room();
			obj->set_owner(player->uuid());
			obj->set_location_data(direction + INSTALL_MASK);
		}
	template <typename T>
		void set_done_installing(T& obj){
			obj->obj_flags.bitvector ^= STATUS_INSTALLING;
		}
	template <typename T>
		void set_done_breaching(T& obj){
			obj->obj_flags.bitvector ^= STATUS_INSTALLING;
		}
	template <typename T,typename PlayerT>
		bool is_owner(T& obj, PlayerT& player){
			return player->uuid() == obj->get_owner();
		}
};//End namespace

#endif
