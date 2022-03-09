#include "player.hpp"
#include "../spells.h"	// for SKILL_SNEAK

extern void affect_to_char(char_data *ch,affected_type *af);
extern void affect_from_char(char_data *ch,int);
namespace mods::sneak {
	void apply_absolute_sneak(player_ptr_t& player, uint8_t duration) {
		affected_type af;

		af.type = SKILL_SNEAK;
		af.duration = duration;
		af.modifier = 0;
		af.location = APPLY_NONE;
		af.bitvector = AFF_SNEAK;
		affect_to_char(player->cd(), &af);
	}
	void clear_sneak(player_ptr_t& player) {
		affect_from_char(player->cd(), SKILL_SNEAK);
	}
};
