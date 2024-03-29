#ifndef __MENTOC_MODS_LEVELS_HEADER__
#define __MENTOC_MODS_LEVELS_HEADER__

#include "../globals.hpp"
#include "orm/integral-object.hpp"

enum triad : uint8_t {
	MELEE = 0,
	WEAPONS,
	INTEL,
	SPEED,
	ARMOR
};
namespace mods::levels {
	/**
	 * Changing the FIRST_TIER value will have effects not only for
	 * newer players, but for existing players as well.
	 * The way that leveling works is that every stat and attribute (hp/mana/move)
	 * is *assigned* to as opposed to *added to*.
	 * So what this means is that if at some point you change FIRST_TIER to a
	 * lower number, it would appear to existing players as if they are gaining less
	 * or even subtracting from their existing stats/attributes when it comes time
	 * for them to level up.
	 *
	 * So it should be noted here that this one constant is the determining factor
	 * of how balanced the game is in accordance with players leveling up.
	 */
	static constexpr float FIRST_TIER = 10.0;
	static constexpr float SECOND_TIER = FIRST_TIER * 0.75;
	static constexpr float THIRD_TIER = FIRST_TIER * 0.50;
	static constexpr float PASSIVE_TIER = FIRST_TIER * 0.25;
	static constexpr float SPLIT_TIER = FIRST_TIER * 0.10;
	static constexpr float SPLIT_TIER_HALF = SPLIT_TIER / 2;

	static constexpr float LOW_TRIAD = (FIRST_TIER / 3);
	static constexpr float MID_TRIAD = ((FIRST_TIER / 3) * 2);
	static constexpr float HIGH_TRIAD = (FIRST_TIER);

	static constexpr uint8_t MAX_PLAYER_LEVEL = 100;
	static constexpr std::array<player_class_t,2> implemented_classes = {
		GHOST,
		PYREXIA
	};
	void list_skills(player_ptr_t& player);
	int gain_exp(player_ptr_t& player,int gain);
	int gain_exp_from_killing(player_ptr_t& attacker,player_ptr_t& victim);
	int reduce_exp_from_dying(player_ptr_t& player);
	int gain_mp_from_killing(player_ptr_t& attacker,player_ptr_t& victim);
	void advance_level(player_ptr_t& player);
	uint8_t player_tier(player_ptr_t& player);
	uint8_t player_tier(const uint8_t& level);
	static inline bool class_is_implemented(player_class_t pc) {
		return std::find(implemented_classes.begin(),implemented_classes.end(),pc) != implemented_classes.end();
	}
	void reward_player_for_advancing_levels(player_ptr_t&);
	void generic_advance(player_ptr_t& player);
	/**
	 * if you add any values after CHARISMA, make sure to update the STAT_INDEXES_SIZE value
	 */
	enum stat_indexes : uint8_t {
		STAT_HP = 0,
		STAT_MANA,
		STAT_MOVE,
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
	std::array<uint16_t,STAT_INDEXES_SIZE> calculate_based_on_triads(std::array<uint8_t,5> triads,int level);
	std::array<uint8_t,5> get_triads_by_class(player_class_t);
	void init();
	int csv_export_report();
	int level_exp(int level);
	uint8_t award_practice_sessions(uint8_t level);
	uint8_t calculate_available_practice_sessions(uint8_t level);
	std::string get_practice_dump();
};
#endif
