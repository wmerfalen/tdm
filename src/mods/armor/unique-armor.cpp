#include "unique-armor.hpp"
#include "../../globals.hpp"
#include "../player.hpp"
#include "../object-utils.hpp"
#include "../cached-map.hpp"
#include "vest-bc89.hpp"

#define STUB(A) std::cerr << "[mods::armor::unique_armor]: " << A << "\n";
namespace mods::armor {
	unique_armor::unique_armor() {
	}
	unique_armor::~unique_armor() {
		if(m_object) {
			m_object.reset();
		}
		m_event_subscriptions.clear();
	}

	void unique_armor::feed(const mods::pq::row_type& row) {
		STUB("feed");
	}
	obj_data_ptr_t unique_armor::obj() {
		return m_object;
	}
	void unique_armor::dispatch_event(damage_event_t event, player_ptr_t victim) {
		STUB("dispatch_event w/uuid");
	}

	void dispatch_unique_armor_event(const uuid_t& attacker_uuid, damage_event_t event, player_ptr_t victim) {
		using namespace mods::object_utils;
		auto attacker = ptr_by_uuid(attacker_uuid);
		if(!attacker) {
			return;
		}
		if(!victim) {
			return;
		}
		auto vest = victim->equipment(WEAR_VEST);
		if(!vest) {
			return;
		}
		if(!vest->armor()) {
			return;
		}

		/**
		 * !UNIQUE_ARMOR
		 */
		if(is_bc89(vest)) {
			auto ptr = mods::armor::find_bc89(vest->uuid);
			if(!ptr) {
				return;
			}
			ptr->dispatch_event(event,attacker);
			return;
		}

	}
	void dispose_unique_armor(const uuid_t& obj_uuid) {
		using namespace mods::object_utils;
		auto armor = optr_by_uuid(obj_uuid);
		if(!armor || !armor->armor()) {
			return;
		}
		/**
		 * !UNIQUE_ARMOR
		 */
		//TODO: if(is_dst7a(armor)) {
		//TODO: 	mods::armor::shotgun::erase_dst7a(obj_uuid);
		//TODO: 	return;
		//TODO: }

	}
	static mods::cached_map<std::string_view,std::string_view> descriptions;
	void unique_armor_register_object(obj_ptr_t& obj) {
		/**
		 * !UNIQUE_WEAPONS
		 */
		if(obj->feed_file().compare("bc89.yml") ==0) {
			mods::armor::make_bc89(obj);
			descriptions.set_once(obj->feed_file(), mods::armor::bc89::description);
			/**
			 * REQUIRED!!!
			 */
			obj->unique_armor = 1;
		}
	}
	bool has_unique_description(const obj_ptr_t& armor) {
		return descriptions.has(armor->feed_file());
	}

	std::string_view get_unique_armor_description(const obj_ptr_t& armor) {
		return descriptions.get(armor->feed_file());
	}
};
#undef STUB
