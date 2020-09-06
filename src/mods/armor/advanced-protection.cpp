#include "advanced-protection.hpp"

#define __MENTOC_SHOW_MODS_ARMOR_ADVANCED_PROTECTION_DEBUG_OUTPUT__
#ifdef __MENTOC_SHOW_MODS_ARMOR_ADVANCED_PROTECTION_DEBUG_OUTPUT__
#define m_debug(a) std::cerr << "[mods::armor::advanced_protection][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#else
#define m_debug(a)
#define m_crit(a)
#endif

namespace mods::armor {

#define m_crit(a) std::cerr << red_str("[CRITICAL]") << "mods::armor::advanced_protection][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";

		void advanced_protection::init(){
			m_destroyed_pieces.clear();
			m_armor_ptrs.clear();
		}
		void advanced_protection::clear_obj_ptrs(){
			m_armor_ptrs.clear();
		}
		advanced_protection::advanced_protection(uuid_t player_uuid){
			init();
			this->sync_equipment(player_uuid);
		}

		/** this class is a rule set for how to calculate how much 
		 * damage is done when wielding specific pieces of armor
		 * */
		float advanced_protection::calculate_limb_damage(feedback_t& damage,int where){
			float calculated = 1.0;
			m_cached_limb_damage[where] = calculated;
			return 1;
		}
		float advanced_protection::calculate_headshot_damage(feedback_t& damage){
			float calculated = 1.0;
			m_cached_headshot_damage = calculated;
			return 1;
		}
		float advanced_protection::calculate_critical_damage(feedback_t& damage){
			float calculated = 1.0;
			m_cached_critical_damage = calculated;
			return 1;
		}
		vec_t<uuid_t>& advanced_protection::destroyed_pieces_of_armor(){
			return m_destroyed_pieces;
		}
		void advanced_protection::sync_equipment(uuid_t player_uuid){
			auto ptr = ptr_by_uuid(player_uuid);
			clear_obj_ptrs();
			for(unsigned i = 0; i < NUM_WEARS; i++){
				auto eq = ptr->equipment(i);
				if(!eq){
					continue;
				}
				m_armor_ptrs[i] = eq;
			}
		}
		void advanced_protection::unequip(int where){
			m_armor_ptrs.erase(where);
		}
		void advanced_protection::equip(int where,uuid_t obj){
			auto ptr = optr_by_uuid(obj);
			m_armor_ptrs[where] = ptr;
		}
#undef m_crit
};

