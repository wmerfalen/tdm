#include "npc.hpp"
#include "../structs.h"
#include "../utils.h"
namespace mods {
	npc::npc(){
		init();
	}
	npc::npc(const mob_rnum & i){
		init();
		m_shared_ptr = std::make_shared<char_data>(&mob_proto[i]);
		m_char_data = m_shared_ptr.get();
		std::cerr << "npc's position: " << this->position() << "\n";
	}
	npc::npc(char_data* ch) : player(ch) {
	}
	npc::npc(mods::npc* npc) : player(npc->cd()) {

	}
	npc::~npc(){
		std::cerr << "mods::npc::~npc destructor called: " << name().c_str() << "\n";
	}
	mob_special_data& npc::mob_specials(){
		return cd()->mob_specials;
	}
	bool npc::has_tree()  {
		return mob_specials().behaviour_tree;
	}

};
