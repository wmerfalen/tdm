#include "attachment-pathogen-ammunition.hpp"
/** db.h for read_object() */
#include "../../db.h"
#include "../../globals.hpp"
#include "../projectile.hpp"

#ifdef __MENTOC_SHOW_ATTACHMENT_PATHOGEN_AMMUNITION_DEBUG_OUTPUT__
#define m_debug(a) std::cerr << green_str("[attachment-pathogen-ammunition]:") << a << "\n";
#else
#define m_debug(a)
#endif
namespace mods::weapons {
	std::tuple<bool,std::string> attachment_pathogen_ammunition::fire(player_ptr_t& victim) {
		if(!m_attached) {
			return {0,"Not attached!"};
		}
		if(ammo()) {
			auto & attr = m_obj->attachment()->attributes;
			/** Initially set this ammunition amount */
			attr->ammunition_amount;
			
			/** affects how it hits and range */
			attr->accuracy_points;
			attr->zoom_multiplier;
			
			/** Base elemental damage */
			attr->radiation_damage;
			attr->incendiary_damage;
			
			/** if disorients */
			attr->disorient_amount;
			
			/** Add onto damage */
			attr->damage_percent_bonus;
			
			/** reduce loudness when this is a valid dynamic */
			//attr->loudness_reduction;
			
			/** Armor penetration dynamic */
			//attr->armor_penetration_amount;
			
			/** Apply elemental damage once hit is done */
			attr->incendiary_damage_percent;
			attr->explosive_damage_percent;
			attr->shrapnel_damage_percent;
			attr->corrosive_damage_percent;
			attr->cryogenic_damage_percent;
			attr->radiation_damage_percent;
			attr->emp_damage_percent;
			attr->shock_damage_percent;
			attr->anti_matter_damage_percent;
			
			//attr->recoil_reduction;
			
			/** When calculating consumed ammo. roll these */
			attr->free_ammo_chance;
			attr->regenerate_ammo_chance;
			
			/** Roll this and deploy bleed if success */
			attr->bleed_chance;
			
			/** When this is a valid dynamic */
			//attr->aimed_limb_accuracy_percent;
			
			consume_ammo();
			return {1,"You fire pathogen ammunition!"};
		}
		m_attached = false;
		detach();
		return {0,"*** CLICK ***"};
	}
	void attachment_pathogen_ammunition::init() {
		m_attached = false;
		m_obj = nullptr;
		m_sniper_rifle = nullptr;
	}
	attachment_pathogen_ammunition::~attachment_pathogen_ammunition() {
		m_attached = false;
	}

	attachment_pathogen_ammunition::attachment_pathogen_ammunition() {
		init();
	}

	uint16_t attachment_pathogen_ammunition::ammo() {
		return m_obj ? m_obj->obj_flags.ammo : 0;
	}

	std::tuple<bool,std::string> attachment_pathogen_ammunition::detach() {
		if(!m_attached) {
			return {1,"Already unloaded."};
		}
		m_attached = false;
		return {1,"You unload a clip of pathogen ammunition."};
	}

	std::tuple<bool,std::string> attachment_pathogen_ammunition::attach_to(obj_ptr_t weapon, const uint8_t& player_tier) {
		if(m_attached) {
			return {true,"Already loaded."};
		}
		m_obj = create_object(ITEM_ATTACHMENT,"attachment/.yml");
		m_obj->obj_flags.ammo = m_obj->attachment()->attributes->ammunition_amount;
		m_sniper_rifle = weapon;
		m_attached = true;
		m_obj->obj_flags.ammo += player_tier * 2;
		return {true,CAT("You attach pathogen ammunition to ",weapon->name)};
	}

	bool attachment_pathogen_ammunition::is_attached() const {
		return m_attached;
	}

	void attachment_pathogen_ammunition::consume_ammo() {
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
	obj_ptr_t attachment_pathogen_ammunition::obj() {
		if(m_attached) {
			return m_obj;
		}
		return nullptr;
	}

};
#undef m_debug
