#include "npc.hpp"
#include "../structs.h"
#include "../utils.h"
namespace mods {
	npc::npc(const mob_rnum & i){
		m_shared_ptr = std::move(std::make_shared<char_data>(&mob_proto[i]));
		m_char_data = m_shared_ptr.get();
		m_player_ptr = std::make_shared<mods::player>(m_char_data);
	}
	const mob_vnum& npc::vnum() const { return cd()->nr; }
	npc::~npc(){
		m_shared_ptr.reset();
		m_player_ptr.reset();
	}
	mob_special_data& npc::mob_specials(){
		return cd()->mob_specials;
	}
	bool npc::has_tree()  {
		return mob_specials().behaviour_tree;
	}
	player_ptr_t& npc::player_ptr(){ return m_player_ptr; }

};
