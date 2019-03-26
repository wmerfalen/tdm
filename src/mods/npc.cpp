#include "npc.hpp"
#include "../structs.h"
#include "../utils.h"
namespace mods {
	npc::npc(){
		init();
	}
	npc::npc(char_data* ch){
		init();
		m_shared_ptr = std::make_shared<char_data>(ch);
		m_char_data = m_shared_ptr.get();
	}
	npc::npc(mods::npc* npc){
		init();
		m_shared_ptr = npc->m_shared_ptr;
		m_char_data = npc->m_shared_ptr.get();
	}
	npc::~npc(){
		m_shared_ptr.reset();
		m_char_data = nullptr;
	}
	void npc::init(){
		if(m_shared_ptr){
			m_shared_ptr.reset();
		}
		m_char_data = nullptr;
	}
	/* setters */
	void npc::set_cd(char_data* ch){
		init();
		m_shared_ptr = std::make_shared<char_data>(ch);
		m_char_data = m_shared_ptr.get();
	}
	void npc::set_shared_ptr(std::shared_ptr<mods::npc>& ptr){
		init();
		m_shared_ptr = ptr->m_shared_ptr;
		m_char_data = m_shared_ptr.get();
	}

	std::shared_ptr<char_data> npc::get_shared_ptr(){
		return m_shared_ptr;
	}
};
