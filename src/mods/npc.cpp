#include "npc.hpp"
#include "../structs.h"
#include "../utils.h"
namespace mods {
	npc::npc(const mob_rnum& i) {
		set_type(player_type_enum_t::MOB);
		name().assign(mob_proto[i].player.name.c_str());
		short_descr().assign(mob_proto[i].player.short_descr.c_str());
	}
	const mob_vnum& npc::vnum() const {
		return cd()->mob_specials.vnum;
	}
	mob_special_data& npc::mob_specials() {
		return this->cd()->mob_specials;
	}
	bool npc::has_tree()  {
		return mob_specials().behaviour_tree;
	}

};
