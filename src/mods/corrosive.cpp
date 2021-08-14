#include "corrosive.hpp"
#include "affects.hpp"
#include "skills.hpp"

#include "rooms.hpp"
#include "injure.hpp"
#include "doors.hpp"
#include "super-users.hpp"
#include "interpreter.hpp"
#include "weapons/damage-types.hpp"
#include "rand.hpp"

#ifdef __MENTOC_SHOW_MODS_CORROSIVE_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::corrosive::debug]")  << MSG << "\n";
#else
#define m_debug(MSG) ;;
#endif


namespace mods::corrosive {
#define m_error(MSG) mentoc_prefix_debug(red_str("[mods::corrosive::ERROR]"))  << MSG << "\n";
	using affect_vector_t = mods::affects::affect_vector_t;
	using affect_t = mods::affects::affect_t;
	using texture_type_t = room_data::texture_type_t;
	static uint32_t corrode_id = 0;
	struct corrode_damage_t {
		uint32_t id;
		uuid_t attacker;
		uuid_t target;
		int16_t dice_count;
		int16_t dice_sides;
		bool cleanup;
		inline bool operator==(const corrode_damage_t& lhs) {
			if(lhs.id == id) {
				return true;
			}
			return false;
		}
		std::string report() {
			return CAT("id:",id,"\nattacker:",attacker,"\ntarget:",target,"\ndice_count:",dice_count,"\ndice_sides:",dice_sides);
		}
		corrode_damage_t() = delete;
		corrode_damage_t(const uuid_t& atk,
		                 const uuid_t& t,
		                 const int16_t& count,
		                 const int16_t& sides) : id(++corrode_id),
			attacker(atk), target(t),dice_count(count),dice_sides(sides),cleanup(false) {}

		corrode_damage_t(const uuid_t& atk,
		                 const uuid_t& t,
		                 obj_ptr_t device) : id(++corrode_id),
			attacker(atk), target(t),cleanup(false) {
			if(device->has_explosive()) {
				dice_count = device->explosive()->attributes->chemical_damage_dice_count;
				dice_sides = device->explosive()->attributes->chemical_damage_dice_sides;
			}
			if(device->has_rifle()) {
				dice_count = device->rifle()->attributes->damage_dice_count;
				dice_sides = device->rifle()->attributes->damage_dice_sides;
			}
		}
	};
	using corrode_list_t = std::forward_list<corrode_damage_t>;

	static corrode_list_t& corrode_player_list() {
		static corrode_list_t list;
		return list;
	}
	static corrode_list_t& corrode_object_list() {
		static corrode_list_t list;
		return list;
	}

	int corrode_damage(player_ptr_t attacker,player_ptr_t victim, obj_ptr_t item) {
		/**
		 * corrosive damage against a player has the following effects:
		 * 	- melee attackers take 2 turns to move instead of 1
		 * 	- chance of melee attackers to drop their weapon
		 * 	- ranged attacks do N percent less damage
		 *  - continuous damage over N seconds
		 *  - blindness over N seconds
		 */
		return 0;
	}
	void process_players() {
		std::vector<corrode_damage_t> player_removals;
		for(const auto& entry : corrode_player_list()) {
			if(entry.cleanup) {
				m_debug("cleanup requested for entry: " << entry.id);
				player_removals.emplace_back(entry);
				continue;
			}
			m_debug("victim: " << entry.target);
			auto victim = ptr_by_uuid(entry.target);
			if(!victim || victim->position() == POS_DEAD || !victim->get_affect_dissolver().has_affect(mods::affects::affect_t::CORRODE)) {
				player_removals.emplace_back(entry);
				continue;
			}
			auto percent = victim->corrosive_resistance_percent();
			auto damage = mods::rand::roll(entry.dice_count,entry.dice_sides);
			int dam = damage - ((percent * 0.01) * damage);
			auto attacker = ptr_by_uuid(entry.attacker);
			if(attacker) {
				attacker->sendln(CAT("{grn}[+",dam,"] corrosive damage to ",victim->name(),"{/grn}"));
			}
			victim->sendln(CAT("{grn}[-",dam,"] corrosive damage.{/grn}"));
			victim->hp() -= dam;
			mods::weapons::damage_types::update_and_process_death(attacker,victim);
		}
		for(const auto& entry : player_removals) {
			corrode_player_list().remove(entry);
		}
	}
	void process_objects() {
		std::vector<corrode_damage_t> object_removals;
		/**
		 * This is defined out since object damage needs to be fleshed out before
		 * we can process corrosion damage
		 */
		for(const auto& entry : corrode_object_list()) {
			if(entry.cleanup) {
				object_removals.emplace_back(entry);
				continue;
			}
			auto object = optr_by_uuid(entry.target);
			if(!object) {
				object_removals.emplace_back(entry);
				continue;
			}
			if(object->corrode_end_tick >= CURRENT_TICK()) {
				object_removals.emplace_back(entry);
				continue;
			}
			auto attacker = ptr_by_uuid(entry.attacker);
			auto damage = mods::rand::roll(entry.dice_count,entry.dice_sides);

			if(object->armor_hp() > 0) {
				if(attacker) {
					attacker->sendln(CAT("{grn}[+",damage,"] corrosive damage to ",object->name,"{/grn}"));
				}
				object->adjust_armor_hp(-damage);
				continue;
			}
			if(attacker) {
				attacker->sendln(CAT("{grn}[+",damage,"] corrosive damage to ",object->name,"{/grn}"));
			}
			object->adjust_hp(-damage);
			if(object->hp() <= 0) {
				/** TODO: dispose object here */
				object_removals.emplace_back(entry);
			}
		}
		for(const auto& pair : object_removals) {
			corrode_object_list().remove(pair);
		}
	}
	void process_corrosion() {
		m_debug("process_corrosion");
		process_players();
		process_objects();
	}
	void disable_electronics(room_rnum room) {
		return;
	}
	void roll_melt_equipment_in_room(room_rnum room) {
		return;
	}
	void roll_melt_equipment_on_player(player_ptr_t victim) {
		return;
	}
	void corrode_clears_up(player_ptr_t victim) {
		return;
	}
	void corrode_clears_up(room_rnum room) {
		return;
	}
	void propagate_chemical_blast(room_rnum& room_id,obj_ptr_t device) {
		return;
	}
	void queue_corrode_player(obj_ptr_t device,player_ptr_t& victim,const int16_t& damage) {
		corrode_player_list().emplace_front(device->get_owner(),
		                                    victim->uuid(),
		                                    device
		                                   );
	}
	void unqueue_player(const uuid_t& player_uuid) {
		for(auto& entry : corrode_player_list()) {
			if(entry.target == player_uuid) {
				entry.cleanup = true;
			}
		}
	}
	void queue_corrode_object(obj_ptr_t device,obj_ptr_t& obj, const int16_t& damage, const uint16_t& ticks) {
		/** TODO: uncomment when object damage is fleshed out */
#if 0
		corrode_object_list().emplace_front(device->get_owner(), obj->uuid,device);
		obj->corrode_end_tick = CURRENT_TICK() + ticks;
#endif
	}

#undef m_error
};
