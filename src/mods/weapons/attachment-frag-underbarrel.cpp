#include "attachment-frag-underbarrel.hpp"
/** db.h for read_object() */
#include "../../db.h"
#include "../../globals.hpp"
#include "../projectile.hpp"

#ifdef __MENTOC_SHOW_SHOTGUN_ATTACHMENT_UNDERBARREL_DEBUG_OUTPUT__
#define m_debug(a) std::cerr << green_str("[attachment-frag-underbarrel]:") << a << "\n";
#else
#define m_debug(a)
#endif
namespace mods::weapons {
	std::tuple<bool,std::string> attachment_frag_underbarrel::fire(player_ptr_t& attacker,const direction_t& direction, const uint8_t& distance) {
		if(!m_attached) {
			return {0,"Not attached!"};
		}
		if(ammo()) {
			auto s =  mods::projectile::launch_underbarrel_grenade(
			              attacker,
			              m_obj,
			              direction,
			              distance
			          );

			consume_ammo();
			return s;
		}
		m_attached = false;
		detach();
		return {0,"*** CLICK ***"};
	}
	void attachment_frag_underbarrel::init() {
		m_attached = false;
		m_obj = nullptr;
		m_sniper_rifle = nullptr;
	}
	attachment_frag_underbarrel::~attachment_frag_underbarrel() {
		m_attached = false;
	}

	attachment_frag_underbarrel::attachment_frag_underbarrel() {
		init();
	}

	uint16_t attachment_frag_underbarrel::ammo() {
		return m_obj ? m_obj->obj_flags.ammo : 0;
	}

	std::tuple<bool,std::string> attachment_frag_underbarrel::detach() {
		if(!m_attached) {
			return {1,"Already detached."};
		}
		m_attached = false;
		return {1,"You detach a frag underbarrel."};
	}

	std::tuple<bool,std::string> attachment_frag_underbarrel::attach_to(obj_ptr_t weapon, const uint8_t& player_tier) {
		if(m_attached) {
			return {true,"Already attached."};
		}
		m_obj = create_object(ITEM_ATTACHMENT,"attachment/sniper-class-frag-ub.yml");
		m_obj->obj_flags.ammo = m_obj->attachment()->attributes->ammunition_amount;
		m_sniper_rifle = weapon;
		m_attached = true;
		m_obj->obj_flags.ammo += player_tier * 2;
		return {true,CAT("You attach a frag underbarrel to ",weapon->name)};
	}

	bool attachment_frag_underbarrel::is_attached() const {
		return m_attached;
	}

	void attachment_frag_underbarrel::consume_ammo() {
		if(m_attached) {
			m_debug("obj_flags.ammo->" << m_obj->obj_flags.ammo);
			if(m_obj->obj_flags.ammo == 0) {
				m_attached = false;
				return;
			}
			--m_obj->obj_flags.ammo;
			if(m_obj->obj_flags.ammo == 0) {
				m_attached = false;
			}
		}
	}
	obj_ptr_t attachment_frag_underbarrel::obj() {
		if(m_attached) {
			return m_obj;
		}
		return nullptr;
	}

};
#undef m_debug
