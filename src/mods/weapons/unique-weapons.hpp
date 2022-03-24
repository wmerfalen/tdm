#ifndef __MENTOC_MODS_WEAPONS_UNIQUES_HEADER__
#define __MENTOC_MODS_WEAPONS_UNIQUES_HEADER__

#include "../weapon.hpp"
#include "../pqxx-types.hpp"
#include "../damage-event.hpp"

#define STUB_PREFIX(F) std::cerr << "[STUB]{mods::weapons}[" << F <<"]:" << __FILE__ << "|" << __LINE__
namespace mods::weapons {
	enum unique_weapon_id : uint16_t {
		DST7A,
		VC88,
	};
	using event_list_t = std::vector<damage_event_t>;

	/**
	 * The thing that we want to accomplish with this class is that
	 * every time a unique weapon experiences a transition event
	 * (such as hitting an enemey) we want to dispatch an event
	 * that allows us to do custom functionality.
	 *
	 * So for example, the first unique weapon is the DST7A shotgun.
	 * What happens when the DST7A deals damage to a target is that
	 * we have radiation and incendiary damage that builds up on
	 * the target. It doesn't end there though: that damage ends up
	 * melting the armor and/or flesh of the target.. effectively
	 * melting the skin off their bones.
	 *
	 * This functionality would not be possible without a custom
	 * class that listens for events such as HITS_ARMOR or HITS_FLESH.
	 * In the case of the DST7A, we listen for HITS_FLESH and immediately
	 * start a counter on the target which begins to melt their flesh
	 * and eventually disarms the target.
	 *
	 * A unique weapon will almost always need to work closely with
	 * the deferred and affects systems for obvious reasons.
	 *
	 */
	struct unique_ranged_weapon {
			unique_ranged_weapon();
			virtual ~unique_ranged_weapon();

			virtual void feed(const mods::pq::row_type&);
			virtual obj_data_ptr_t obj();
			virtual void add_target(const uuid_t& target_uuid);
			virtual void remove_target(const uuid_t& target_uuid);
			uuid_t& get_target_uuid();
			virtual const event_list_t& get_subscriptions() {
				static event_list_t subs = {};
				return subs;
			}
			virtual void dispatch_event(damage_event_t event, player_ptr_t victim);
			virtual std::vector<uuid_t>& damaged_enemy_list();

		protected:
			event_list_t m_event_subscriptions;
			std::vector<uuid_t> m_damaged_enemy_list;
			obj_ptr_t m_object;
	};
	void dispatch_unique_ranged_weapon_event(const uuid_t& attacker, damage_event_t event, player_ptr_t victim);
	void dispose_unique_weapon(const uuid_t& obj_uuid);
	void unique_weapon_register_object(obj_ptr_t& obj);
};
#endif

