#include "unique-weapons.hpp"
#include "../../globals.hpp"
#include "../player.hpp"
#include "../object-utils.hpp"
#include "shotgun-dst7A.hpp"
#include "smg-vc88.hpp"

#define STUB(A) STUB_PREFIX("unique_ranged_weapon::") << A << "\n"
namespace mods::weapons {
	unique_ranged_weapon::unique_ranged_weapon() {
	}
	unique_ranged_weapon::~unique_ranged_weapon() {
		if(m_object) {
			m_object.reset();
		}
		m_event_subscriptions.clear();
		m_damaged_enemy_list.clear();
	}

	void unique_ranged_weapon::feed(const mods::pq::row_type& row) {
		STUB("feed");
	}
	obj_data_ptr_t unique_ranged_weapon::obj() {
		return m_object;
	}
	void unique_ranged_weapon::add_target(const uuid_t& target_uuid) {
		STUB("add_target");
	}
	void unique_ranged_weapon::remove_target(const uuid_t& target_uuid) {
		STUB("remove_target");

	}
	void unique_ranged_weapon::dispatch_event(damage_event_t event, player_ptr_t victim) {
		STUB("dispatch_event w/uuid");
	}

	std::vector<uuid_t>& unique_ranged_weapon::damaged_enemy_list() {
		return m_damaged_enemy_list;

	}

	void dispatch_unique_ranged_weapon_event(const uuid_t& attacker_uuid, damage_event_t event, player_ptr_t victim) {
		using namespace mods::object_utils;
		auto attacker = ptr_by_uuid(attacker_uuid);
		if(!attacker) {
			return;
		}
		auto weapon = attacker->primary();
		if(!weapon) {
			return;
		}

		if(is_dst7a(weapon)) {
			auto ptr = mods::weapons::shotgun::find_dst7a(weapon->uuid);
			if(!ptr) {
				return;
			}
			ptr->dispatch_event(event,victim);
			return;
		}
		if(is_vc88(weapon)) {
			auto ptr = mods::weapons::smg::find_vc88(weapon->uuid);
			if(!ptr) {
				return;
			}
			ptr->dispatch_event(event,victim);
			return;
		}

	}
	void dispose_unique_weapon(const uuid_t& obj_uuid) {
		using namespace mods::object_utils;
		auto weapon = optr_by_uuid(obj_uuid);
		if(is_dst7a(weapon)) {
			mods::weapons::shotgun::erase_dst7a(obj_uuid);
			return;
		}
		if(is_vc88(weapon)) {
			mods::weapons::smg::erase_vc88(obj_uuid);
			return;
		}

	}
	void unique_weapon_register_object(obj_ptr_t& obj) {
		if(obj->feed_file().compare("dst7a.yml") ==0) {
			mods::weapons::shotgun::make_dst7a(obj);
		}
		if(obj->feed_file().compare("vc88.yml") ==0) {
			mods::weapons::smg::make_vc88(obj);
		}
	}
};
#undef STUB
