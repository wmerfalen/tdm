#include "affects.hpp"
#include "../globals.hpp"
#include <algorithm>
#include <cctype>
#ifdef __MENTOC_MODS_AFFECTS_SHOW_DEBUG_OUTPUT__
#define maffects_debug(a) std::cerr << "[mods::affects]" << __FILE__ << "|" << __LINE__ << "->" << a << "\n";
#else
#define maffects_debug(a) /**/
#endif
namespace mods::affects {
	static std::set<player_ptr_t> needs_dissolve;
	dissolver::dissolver() {
		for(unsigned i =0; i < m_affects.size();i++){
			m_affects[i] = 0;
		}
		m_has_affects = 0;
	}
	void dissolver::affect_map(affect_map_t affects){
		for(auto& item : affects){
			m_affects[item.first] += item.second;
			if(m_affects[item.first] == 0){
				this->remove(item.first);
			}
		}
	}
	void dissolver::affect(affect_t aff_id){
		affect(aff_id,mods::affects::DEFAULT_AMOUNT);
	}
	void dissolver::clear(std::vector<affect_t> affects){
		for(auto a : affects){
			m_affects[a] = 0;
			this->remove(a);
		}
	}
	void dissolver::affect(affect_t affect,int amount){
		if((m_affects[affect] += amount) == 0){
			this->remove(affect);
		}
	}
	std::size_t dissolver::tick(){
		d("Dissolver tick");
		std::size_t incremented = 0;
		for(unsigned i=0; i < AFFECT_DISSOLVE_COUNT; i++){
			if(!m_affects[i]){
				continue;
			}
			--m_affects[i];
			++incremented;
			if(!m_affects[i]){
				this->remove(static_cast<affect_t>(i));
			}
		}
		m_has_affects = incremented;
		return incremented;
	}
	void dissolver::remove(affect_t id){
	}
	affect_t to_affect(std::string str){
		std::string aff_name;
		aff_name.reserve(str.length());
		std::transform(
				str.begin(), str.end(), aff_name.begin(),
				::tolower
		);
		auto it = mods::affects::string_map.find(aff_name);
		if(it != mods::affects::string_map.end()){ 
			return it->second;
		}
		return affect_t::NONE;
	}

	mods::affects::affect_vector_t to_affect(std::vector<std::string> strings) {
		mods::affects::affect_vector_t a;
		for(auto & _orig_aff_name : strings){
			auto real_affect = to_affect(_orig_aff_name);
			if(real_affect != affect_t::NONE){
				a.emplace_back(real_affect);
			}
		}
		return a;
	}

	void process(){
		for(auto it = needs_dissolve.begin(); it != needs_dissolve.end();){
			if(0 == (*it)->get_affect_dissolver().tick()){
				maffects_debug("[process] tick reached zero for player:'" << (*it)->name() << "'");
				needs_dissolve.erase(it++);
				continue;
			} 
			it++;
		}
	}
	void affect_player(affect_vector_t a,player_ptr_t player){
		needs_dissolve.insert(player);
		player->get_affect_dissolver().affect_via(a);
	}
	void affect_player_for(affect_vector_t a,player_ptr_t p,uint64_t ticks){
		needs_dissolve.insert(p);
		for(auto current_affect : a){
			p->get_affect_dissolver().affect(current_affect,ticks);
		}
	}
	void str_affect_player(std::vector<std::string> a,player_ptr_t p){
		affect_player(to_affect(a),p);
	}
std::vector<std::string>& affect_string_list_impl() {
	static std::vector<std::string> list;
	static bool list_set = false;
	if(list_set){ return list; }
	for(auto & pair : mods::affects::string_map){
		list.emplace_back(pair.first);
	}
	list_set = true;
	return list;
}
	/*
		 void removal_message(player_ptr_t player, affect_t afid){
		 switch(afid){
		 case affect_t::BLIND:
		 player->sendln("Your vision clears up and the world fades back into existence.");
		 break;
		 case affect_t::DISORIENT:
		 player->sendln("You regain control over your faculties. You are no longer disoriented.");
		 break;
		 case affect_t::POISON:
		 player->sendln("The effects of the poison start to wear off.");
		 break;
		 default: break;
		 }
		 }
		 */


};
std::vector<std::string>& affect_string_list() { return mods::affects::affect_string_list_impl(); }

void str_queue_on_room(std::vector<std::string> affects,int room_id){
	for(auto & player : mods::globals::get_room_list(room_id)){
		mods::affects::str_affect_player(affects,player);
	}
}

void str_queue_on_player(std::vector<std::string> affects,player_ptr_t player){
	mods::affects::str_affect_player(affects,player);
}

void queue_on_room(std::vector<mods::affects::affect_t> affects,int room_id){
	for(auto & player : mods::globals::get_room_list(room_id)){
		mods::affects::affect_player(affects,player);
	}
}

void queue_on_room(std::vector<mods::affects::affect_t> affects,player_ptr_t player){
	mods::affects::affect_player(affects,player);
}
