#include "affects.hpp"
#include "../globals.hpp"
#include <algorithm>
#include <cctype>
#include "player.hpp"
namespace mods::rooms::affects {
	extern void process();
};
namespace mods::affects {
	static std::set<player_ptr_t> needs_dissolve;
	static time_t time_tracker_per_minute = time(nullptr);
	static uint32_t ticks_per_minute = 0;
	static uint32_t ticks_per_minute_sample = 0;
	void player_died(player_ptr_t p) {
		needs_dissolve.erase(p);
	}

	int apply_tracked_bonus_damage(int damage) {
		return damage + (TRACKED_DAMAGE_BONUS_MULTIPLIER * damage);
	}
	using affect_t = mods::affects::affect_t;
	affect_t to_affect(std::string str) {
		std::string aff_name;
		aff_name.reserve(str.length());
		std::transform(
		    str.begin(), str.end(), aff_name.begin(),
		    ::tolower
		);
		auto it = mods::affects::string_map.find(aff_name);
		if(it != mods::affects::string_map.end()) {
			return it->second;
		}
		return affect_t::NONE;
	}

	mods::affects::affect_vector_t to_affect(std::vector<std::string> strings) {
		mods::affects::affect_vector_t a;
		for(auto& _orig_aff_name : strings) {
			auto real_affect = to_affect(_orig_aff_name);
			if(real_affect != affect_t::NONE) {
				a.emplace_back(real_affect);
			}
		}
		return a;
	}
	uint32_t get_ticks_per_minute() {
		return ticks_per_minute_sample;
	}

	/**
	 * \brief called every tick to process player affects. Also calls rooms affects
	 */
	void process() {
		auto seconds = time(nullptr);
		if((seconds - time_tracker_per_minute) >= 60) {
			ticks_per_minute_sample = ticks_per_minute;
			ticks_per_minute = 0;
			time_tracker_per_minute = seconds;
		}
		++ticks_per_minute;

		for(auto it = needs_dissolve.begin(); it != needs_dissolve.end();) {
			if(0 == (*it)->get_affect_dissolver().tick()) {
				maffects_debug("[process] tick reached zero for player:'" << (*it)->name() << "'");
				needs_dissolve.erase(it++);
				continue;
			}
			it++;
		}
		mods::rooms::affects::process();
	}
	/**
	 * \brief affects a player with a vector of affects
	 * @param affect_vector_t list of affects
	 * @param player_ptr_t player
	 */
	void affect_player(affect_vector_t a,player_ptr_t player) {
		needs_dissolve.insert(player);
		player->get_affect_dissolver().affect_via(a);
	}
	/**
	 * \brief affects a player with a vector of affects for specified number of ticks
	 * @param affect_vector_t list of affects
	 * @param player_ptr_t player
	 * @param uint64_t ticks
	 */
	void affect_player_for(affect_vector_t a,player_ptr_t p,uint64_t ticks) {
		maffects_debug("[affect_player_for] " << p->name().c_str() << ": ticks: " << ticks);
		needs_dissolve.insert(p);
		for(auto current_affect : a) {
			p->get_affect_dissolver().affect(current_affect,ticks);
		}
	}
	void blind_for(player_ptr_t& victim,uint16_t ticks) {
		affect_player_for({affect_t::BLIND},victim,ticks);
		victim->affect(AFF_BLIND);
	}
	/**
	 * \brief same as affect_player but uses strings
	 */
	void str_affect_player(std::vector<std::string> a,player_ptr_t p) {
		affect_player(to_affect(a),p);
	}
	std::vector<std::string>& affect_string_list_impl() {
		static std::vector<std::string> list;
		static bool list_set = false;
		if(list_set) {
			return list;
		}
		for(auto& pair : mods::affects::string_map) {
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
std::vector<std::string>& affect_string_list() {
	return mods::affects::affect_string_list_impl();
}

void str_queue_on_room(std::vector<std::string> affects,int room_id) {
	for(auto& player : mods::globals::get_room_list(room_id)) {
		mods::affects::str_affect_player(affects,player);
	}
}

void str_queue_on_player(std::vector<std::string> affects,player_ptr_t player) {
	mods::affects::str_affect_player(affects,player);
}

void queue_on_room(std::vector<mods::affects::affect_t> affects,int room_id) {
	for(auto& player : mods::globals::get_room_list(room_id)) {
		mods::affects::affect_player(affects,player);
	}
}

void queue_on_room(std::vector<mods::affects::affect_t> affects,player_ptr_t player) {
	mods::affects::affect_player(affects,player);
}
