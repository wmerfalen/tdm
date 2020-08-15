#ifndef __MENTOC_MODS_ARMOR_BASIC_PROTECTION_HEADER__
#define  __MENTOC_MODS_ARMOR_BASIC_PROTECTION_HEADER__

#include "../../globals.hpp"
#include "../damage-event.hpp"

namespace mods::armor {
	struct basic_protection {
		basic_protection() = delete;
		~basic_protection() = default;
		void init();
		basic_protection(uuid_t player);

		/** 
		 * this class is a rule set for how to calculate how much 
		 * damage is done when wielding specific pieces of armor.
		 * Each function returns a float which will be multiplied
		 * by the actual damage calculated in the mods/damage-types class.
		 *
		 * */
		float calculate_limb_damage(feedback_t& damage,int where);
		float calculate_headshot_damage(feedback_t& damage);
		float calculate_critical_damage(feedback_t& damage);
		vec_t<uuid_t>& destroyed_pieces_of_armor();
		void clear_obj_ptrs();
		void sync_equipment(uuid_t player_uuid);
		void unequip(int where);
		void equip(int where,uuid_t obj);
		private:
			vec_t<uuid_t> m_destroyed_pieces;
			std::map<int,float> m_cached_limb_damage;
			float m_cached_headshot_damage;
			float m_cached_critical_damage;
			std::map<int,obj_ptr_t> m_armor_ptrs;
	};
};

#endif
