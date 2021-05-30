#include "npc.hpp"
#include "../structs.h"
#include "../utils.h"
namespace mods {
	npc::npc(const mob_rnum& i) {
		std::cerr << "[npc::npc] constructor with mob_rnum: " << i << "\n";
		set_type(player_type_enum_t::MOB);
		name().assign(mob_proto[i].player.name.c_str());
		short_descr().assign(mob_proto[i].player.short_descr.c_str());
		std::cerr << "npc player name/short descr: '" << name().c_str() << "'\n";
	}
	const mob_vnum& npc::vnum() const {
		return cd()->mob_specials.vnum;
	}
	npc::~npc() {
		std::cerr << "[npc::~npc] destructor\n";
	}
	mob_special_data& npc::mob_specials() {
		return this->cd()->mob_specials;
	}
	bool npc::has_tree()  {
		return mob_specials().behaviour_tree;
	}

};
