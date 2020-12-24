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
	void player_carry(player_ptr_t& player, obj_ptr_t& object) {
	}
	void player_uncarry(player_ptr_t& player, obj_ptr_t& object) {

	}

	void player_wear(player_ptr_t& player,obj_ptr_t& object) {
		if(object->rifle()) {
			player->aff_abils().str += player->object->rifle()->attributes->stat_strength;
			player->aff_abils().str_add += player->object->rifle()->attributes->stat_strength;
			player->aff_abils().intel += player->object->rifle()->attributes->stat_intelligence;
			player->aff_abils().wis += player->object->rifle()->attributes->stat_wisdom;
			player->aff_abils().dex += player->object->rifle()->attributes->stat_dexterity;
			player->aff_abils().con += player->object->rifle()->attributes->stat_constitution;
			player->aff_abils().electronics += player->object->rifle()->attributes->stat_electronics;
			player->aff_abils().armor += player->object->rifle()->attributes->stat_armor;
			player->aff_abils().marksmanship += player->object->rifle()->attributes->stat_marksmanship;
			player->aff_abils().sniping += player->object->rifle()->attributes->stat_sniping;
			player->aff_abils().demolitions += player->object->rifle()->attributes->stat_demolitions;
			player->aff_abils().chemistry += player->object->rifle()->attributes->stat_chemistry;
			player->aff_abils().weapon_handling += player->object->rifle()->attributes->stat_weapon_handling;
			player->aff_abils().strategy += player->object->rifle()->attributes->stat_strategy;
			player->aff_abils().medical += player->object->rifle()->attributes->stat_medical;
		}
	}

	void player_unwear(player_ptr_t& player,obj_ptr_t& object) {
		if(object->rifle()) {
			player->aff_abils().str -= player->object->rifle()->attributes->stat_strength;
			player->aff_abils().str_add -= player->object->rifle()->attributes->stat_strength;
			player->aff_abils().intel -= player->object->rifle()->attributes->stat_intelligence;
			player->aff_abils().wis -= player->object->rifle()->attributes->stat_wisdom;
			player->aff_abils().dex -= player->object->rifle()->attributes->stat_dexterity;
			player->aff_abils().con -= player->object->rifle()->attributes->stat_constitution;
			player->aff_abils().electronics -= player->object->rifle()->attributes->stat_electronics;
			player->aff_abils().armor -= player->object->rifle()->attributes->stat_armor;
			player->aff_abils().marksmanship -= player->object->rifle()->attributes->stat_marksmanship;
			player->aff_abils().sniping -= player->object->rifle()->attributes->stat_sniping;
			player->aff_abils().demolitions -= player->object->rifle()->attributes->stat_demolitions;
			player->aff_abils().chemistry -= player->object->rifle()->attributes->stat_chemistry;
			player->aff_abils().weapon_handling -= player->object->rifle()->attributes->stat_weapon_handling;
			player->aff_abils().strategy -= player->object->rifle()->attributes->stat_strategy;
			player->aff_abils().medical -= player->object->rifle()->attributes->stat_medical;
		}
	}


};

