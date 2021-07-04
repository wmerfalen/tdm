#include "npc.hpp"
#include "../structs.h"
#include "../utils.h"
namespace mods {
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
