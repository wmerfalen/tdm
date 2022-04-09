#ifndef __MENTOC_MODS_ARMOR_UNIQUES_HEADER__
#define __MENTOC_MODS_ARMOR_UNIQUES_HEADER__

#include "../armor.hpp"
#include "../pqxx-types.hpp"
#include "../damage-event.hpp"

namespace mods::armor {
	enum unique_armor_id : uint16_t {
		BC89,
		XTV89,
	};
	using event_list_t = std::vector<damage_event_t>;

	struct unique_armor {
			unique_armor();
			virtual ~unique_armor();

			virtual void feed(const mods::pq::row_type&);
			virtual obj_data_ptr_t obj();
			virtual const event_list_t& get_subscriptions() {
				static event_list_t subs = {};
				return subs;
			}
			virtual void dispatch_event(damage_event_t event, player_ptr_t victim);

		protected:
			event_list_t m_event_subscriptions;
			obj_ptr_t m_object;
	};
	void dispatch_unique_armor_event(const uuid_t& attacker, damage_event_t event, player_ptr_t victim);
	void dispose_unique_armor(const uuid_t& obj_uuid);
	void unique_armor_register_object(obj_ptr_t& obj);
	bool has_unique_description(const obj_ptr_t& armor);
	std::string_view get_unique_armor_description(const obj_ptr_t& armor);
};
#endif

