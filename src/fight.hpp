#ifndef __MENTOC_FIGHT_HEADER_FILE__
#define __MENTOC_FIGHT_HEADER_FILE__

//for TO_ROOM
#include "comm.h"
#include "spells.h" // for TYPE_HIT const

#include "mods/projectile.hpp"
#include "mods/corpse.hpp" // for make_corpse
#include "mods/corrosive.hpp" // for unqueue_player
#include "mods/loot.hpp"

#ifdef __MENTOC_SHOW_DAMAGE_TYPES_DEBUG__
#define m_debug(A) std::cerr << green_str("combat_composer_debug:") << A << "\n";
#else
#define m_debug(A)
#endif
extern void extract_char_final(char_data *ch);
extern void stop_fighting(char_data *ch);
extern void change_alignment(char_data *ch, char_data *victim);
extern void set_fighting(char_data *ch, char_data *vict);
extern void remember(char_data *ch,char_data *victim);
extern void appear(char_data *ch);
extern int pk_allowed;
extern void check_killer(char_data *ch, char_data *vict);
#define IS_WEAPON(type) (((type) >= TYPE_HIT) && ((type) < TYPE_SUFFERING))
extern int skill_message(int dam, char_data *ch, char_data *vict, int attacktype);
extern void dam_message(int dam, char_data *ch, char_data *victim, int w_type);
ACMD(do_flee);
extern void remember_event(player_ptr_t& victim,player_ptr_t& attacker);

#endif
