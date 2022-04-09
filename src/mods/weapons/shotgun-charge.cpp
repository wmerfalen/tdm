#include "damage-calculator.hpp"
#include "elemental.hpp"
#include "damage-types.hpp"
#include "shotgun-charge.hpp"
/** db.h for read_object() */
#ifdef __MENTOC_SHOW_CORROSIVE_CLAYMORE_DEBUG_OUTPUT__
#define m_debug(a) std::cerr << green_str("[shotgun-claymore]:") << a << "\n";
#else
#define m_debug(a)
#endif
void obj_from_room(obj_ptr_t in_object);
namespace mods::weapons {
	std::forward_list<std::shared_ptr<shotgun_charge>>& shotgun_charge_list() {
		static std::forward_list<std::shared_ptr<shotgun_charge>> c;
		return c;
	}
	obj_ptr_t shotgun_charge::create() {
		auto s = std::make_shared<shotgun_charge>();
		shotgun_charge_list().emplace_front(s);
		return s->obj();
	}


	obj_ptr_t shotgun_charge::obj() {
		return m_obj;
	}
	shotgun_charge::shotgun_charge(const shotgun_charge& copy) {
		m_debug("[shotgun_charge copycon]");
		m_obj = std::move(copy.m_obj);
		m_uuid = copy.m_uuid;

	}
	shotgun_charge::~shotgun_charge() {
		m_debug("[~shotgun_charge]");
		m_obj = nullptr;
	}

	void shotgun_charge::hit(player_ptr_t& victim) {

	}
	void shotgun_charge::hit(obj_ptr_t& object) {

	}

	shotgun_charge::shotgun_charge() {
		m_obj = nullptr;
		m_obj = create_object(ITEM_EXPLOSIVE,"shotgun-charge.yml");
		m_uuid = m_obj->uuid;
	}

};
#undef m_debug
