#include "stat-bonuses.hpp"
#include "lmdb.hpp"
#include <map>
#include <string>
#include <vector>
#include "super-users.hpp"
#include "interpreter.hpp"
#include "screen-searcher.hpp"

#define __MENTOC_MODS_VALUES_SHOW_DEBUG_OUTPUT__
#ifdef  __MENTOC_MODS_VALUES_SHOW_DEBUG_OUTPUT__
#define m_debug(a) std::cerr << "[mods::stat_bonuses]: '" << a << "'\n";
#else
#define m_debug(a)
#endif

namespace mods::stat_bonuses {
	void player_carry(uuid_t player_uuid, uuid_t object_uuid) {
		/** TODO */
	}
	void player_uncarry(uuid_t player_uuid, uuid_t object_uuid) {
		/** TODO */
	}

	void player_equip(uuid_t player_uuid,uuid_t object_uuid) {
		auto player = ptr_by_uuid(player_uuid);
		auto object = optr_by_uuid(object_uuid);
		if(object->rifle()) {
			player->aff_abils().str += object->rifle()->attributes->stat_strength;
			player->aff_abils().str_add += object->rifle()->attributes->stat_strength;
			player->aff_abils().intel += object->rifle()->attributes->stat_intelligence;
			player->aff_abils().wis += object->rifle()->attributes->stat_wisdom;
			player->aff_abils().dex += object->rifle()->attributes->stat_dexterity;
			player->aff_abils().con += object->rifle()->attributes->stat_constitution;
			player->aff_abils().electronics += object->rifle()->attributes->stat_electronics;
			player->aff_abils().armor += object->rifle()->attributes->stat_armor;
			player->aff_abils().marksmanship += object->rifle()->attributes->stat_marksmanship;
			player->aff_abils().sniping += object->rifle()->attributes->stat_sniping;
			player->aff_abils().demolitions += object->rifle()->attributes->stat_demolitions;
			player->aff_abils().chemistry += object->rifle()->attributes->stat_chemistry;
			player->aff_abils().weapon_handling += object->rifle()->attributes->stat_weapon_handling;
			player->aff_abils().strategy += object->rifle()->attributes->stat_strategy;
			player->aff_abils().medical += object->rifle()->attributes->stat_medical;
		}
		if(object->armor()) {
			player->aff_abils().str += object->armor()->attributes->stat_strength;
			player->aff_abils().str_add += object->armor()->attributes->stat_strength;
			player->aff_abils().intel += object->armor()->attributes->stat_intelligence;
			player->aff_abils().wis += object->armor()->attributes->stat_wisdom;
			player->aff_abils().dex += object->armor()->attributes->stat_dexterity;
			player->aff_abils().con += object->armor()->attributes->stat_constitution;
			player->aff_abils().electronics += object->armor()->attributes->stat_electronics;
			player->aff_abils().armor += object->armor()->attributes->stat_armor;
			player->aff_abils().marksmanship += object->armor()->attributes->stat_marksmanship;
			player->aff_abils().sniping += object->armor()->attributes->stat_sniping;
			player->aff_abils().demolitions += object->armor()->attributes->stat_demolitions;
			player->aff_abils().chemistry += object->armor()->attributes->stat_chemistry;
			player->aff_abils().weapon_handling += object->armor()->attributes->stat_weapon_handling;
			player->aff_abils().strategy += object->armor()->attributes->stat_strategy;
			player->aff_abils().medical += object->armor()->attributes->stat_medical;
			player->incendiary_resistance_percent() += object->armor()->attributes->incendiary_resistance_percent;
			player->explosive_resistance_percent() += object->armor()->attributes->explosive_resistance_percent;
			player->shrapnel_resistance_percent() += object->armor()->attributes->shrapnel_resistance_percent;
			player->corrosive_resistance_percent() += object->armor()->attributes->corrosive_resistance_percent;
			player->cryogenic_resistance_percent() += object->armor()->attributes->cryogenic_resistance_percent;
			player->radiation_resistance_percent() += object->armor()->attributes->radiation_resistance_percent;
			player->emp_resistance_percent() += object->armor()->attributes->emp_resistance_percent;
			player->shock_resistance_percent() += object->armor()->attributes->shock_resistance_percent;
			player->anti_matter_resistance_percent() += object->armor()->attributes->anti_matter_resistance_percent;
		}
	}

	void player_unequip(uuid_t player_uuid,uuid_t object_uuid) {
		auto player = ptr_by_uuid(player_uuid);
		auto object = optr_by_uuid(object_uuid);
		if(object->rifle()) {
			player->aff_abils().str -= object->rifle()->attributes->stat_strength;
			player->aff_abils().str_add -= object->rifle()->attributes->stat_strength;
			player->aff_abils().intel -= object->rifle()->attributes->stat_intelligence;
			player->aff_abils().wis -= object->rifle()->attributes->stat_wisdom;
			player->aff_abils().dex -= object->rifle()->attributes->stat_dexterity;
			player->aff_abils().con -= object->rifle()->attributes->stat_constitution;
			player->aff_abils().electronics -= object->rifle()->attributes->stat_electronics;
			player->aff_abils().armor -= object->rifle()->attributes->stat_armor;
			player->aff_abils().marksmanship -= object->rifle()->attributes->stat_marksmanship;
			player->aff_abils().sniping -= object->rifle()->attributes->stat_sniping;
			player->aff_abils().demolitions -= object->rifle()->attributes->stat_demolitions;
			player->aff_abils().chemistry -= object->rifle()->attributes->stat_chemistry;
			player->aff_abils().weapon_handling -= object->rifle()->attributes->stat_weapon_handling;
			player->aff_abils().strategy -= object->rifle()->attributes->stat_strategy;
			player->aff_abils().medical -= object->rifle()->attributes->stat_medical;
		}
		if(object->armor()) {
			player->aff_abils().str -= object->armor()->attributes->stat_strength;
			player->aff_abils().str_add -= object->armor()->attributes->stat_strength;
			player->aff_abils().intel -= object->armor()->attributes->stat_intelligence;
			player->aff_abils().wis -= object->armor()->attributes->stat_wisdom;
			player->aff_abils().dex -= object->armor()->attributes->stat_dexterity;
			player->aff_abils().con -= object->armor()->attributes->stat_constitution;
			player->aff_abils().electronics -= object->armor()->attributes->stat_electronics;
			player->aff_abils().armor -= object->armor()->attributes->stat_armor;
			player->aff_abils().marksmanship -= object->armor()->attributes->stat_marksmanship;
			player->aff_abils().sniping -= object->armor()->attributes->stat_sniping;
			player->aff_abils().demolitions -= object->armor()->attributes->stat_demolitions;
			player->aff_abils().chemistry -= object->armor()->attributes->stat_chemistry;
			player->aff_abils().weapon_handling -= object->armor()->attributes->stat_weapon_handling;
			player->aff_abils().strategy -= object->armor()->attributes->stat_strategy;
			player->aff_abils().medical -= object->armor()->attributes->stat_medical;
			player->incendiary_resistance_percent() -= object->armor()->attributes->incendiary_resistance_percent;
			player->explosive_resistance_percent() -= object->armor()->attributes->explosive_resistance_percent;
			player->shrapnel_resistance_percent() -= object->armor()->attributes->shrapnel_resistance_percent;
			player->corrosive_resistance_percent() -= object->armor()->attributes->corrosive_resistance_percent;
			player->cryogenic_resistance_percent() -= object->armor()->attributes->cryogenic_resistance_percent;
			player->radiation_resistance_percent() -= object->armor()->attributes->radiation_resistance_percent;
			player->emp_resistance_percent() -= object->armor()->attributes->emp_resistance_percent;
			player->shock_resistance_percent() -= object->armor()->attributes->shock_resistance_percent;
			player->anti_matter_resistance_percent() -= object->armor()->attributes->anti_matter_resistance_percent;
		}
	}


};

