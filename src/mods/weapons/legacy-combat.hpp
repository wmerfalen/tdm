#ifndef __MENTOC_MODS_WEAPONS_LEGACY_COMBAT_HEADER__
#define __MENTOC_MODS_WEAPONS_LEGACY_COMBAT_HEADER__
//#include "../combat-composer.hpp"
#include "../rifle.hpp"
#include "../scan.hpp"
#include "../player.hpp"
#include "../ranged-combat-totals.hpp"
#include "../damage-event.hpp"
#include "../rooms.hpp"
#include "../object-utils.hpp"
#include "../player-utils.hpp"
#include "../calc-visibility.hpp"
#include "../classes/ghost.hpp"
#include "../levels.hpp"

#include "../../fight.hpp"
#include "../mobs/damage-event.hpp"
#include "elemental.hpp"

namespace mods::weapons::legacy {
	using de = damage_event_t;
	void solo_gain(player_ptr_t& attacker,player_ptr_t& victim);
	void death_cry(char_data *ch);

	void die(char_data *ch);

	void die(char_data* killer,char_data *victim);
	void solo_gain(player_ptr_t& attacker,player_ptr_t& victim);
	int step_one(char_data *ch, char_data *victim, int dam, int attacktype);
	int step_two(char_data *ch, char_data *victim, int dam, int attacktype);
	int get_damage(char_data *ch, char_data *victim, int dam, int attacktype);
	void deal_damage(char_data *ch, char_data *victim, int dam, int attacktype);
	void update_position(char_data* ch);
	void send_combat_messages(char_data *ch, char_data *victim, int dam, int attacktype);
	void help_linkless(char_data *ch, char_data *victim, int dam, int attacktype);
	int perform_damage_cleanup(char_data *ch, char_data *victim, int dam, int attacktype);
	int damage(char_data *attacker, char_data *victim, int dam, int attacktype);
	void player_died(player_ptr_t& attacker,player_ptr_t& victim);
	void player_died(player_ptr_t& victim);
	int get_legacy_attack_type(obj_ptr_t& weapon);
};
#endif
