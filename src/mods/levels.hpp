#ifndef __MENTOC_MODS_LEVELS_HEADER__
#define __MENTOC_MODS_LEVELS_HEADER__

#include "../globals.hpp"
#include "orm/integral-object.hpp"

namespace mods::levels {
	static constexpr std::array<player_class_t,2> implemented_classes = {
		GHOST,
		PYREXIA
	};
	void gain_exp(player_ptr_t player,int gain);
	void advance_level(player_ptr_t& player);
	static inline bool class_is_implemented(player_class_t pc){
		return std::find(implemented_classes.begin(),implemented_classes.end(),pc) != implemented_classes.end();
	}
	void reward_player_for_advancing_levels(player_ptr_t&);
	void generic_advance(player_ptr_t& player);
	/** 
	 * if you add any values after CHARISMA, make sure to update the STAT_INDEXES_SIZE value
	 */
	enum stat_indexes : uint8_t {
		STAT_HP = 0,
		STAT_STRENGTH,
		STAT_CONSTITUTION,
		STAT_DEXTERITY,
		STAT_INTELLIGENCE,
		STAT_WISDOM,
		STAT_ELECTRONICS,
		STAT_CHEMISTRY,
		STAT_STRATEGY,
		STAT_MARKSMANSHIP,
		STAT_SNIPING,
		STAT_WEAPON_HANDLING,
		STAT_DEMOLITIONS,
		STAT_ARMOR,
		STAT_MEDICAL,
		STAT_CHARISMA,
		STAT_INDEXES_SIZE = STAT_CHARISMA + 1,
	};
	void report_advances(std::array<float,STAT_INDEXES_SIZE> stats);
	std::array<float,STAT_INDEXES_SIZE> calculate_based_on_triads(std::array<uint8_t,5> triads,int level);
	std::array<uint8_t,5> get_triads_by_class(player_class_t);
	void init();
};
#endif
