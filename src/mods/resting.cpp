#include "resting.hpp"
#include "interpreter.hpp"
#include "super-users.hpp"
#include "levels.hpp"

#ifdef __MENTOC_MODS_RESTING_SHOW_DEBUG_OUTPUT__
#define m_debug(A) std::cerr << "[mods::resting][debug]:'" << A << "'\n";
#define m_error(A) std::cerr << "[mods::resting][error]:'" << A << "'\n";
#else
#define m_debug(A)
#define m_error(A)
#endif

namespace mods::resting {

	static std::vector<uuid_t> players_resting;
	void add_player_resting(player_ptr_t& player) {
		players_resting.emplace_back(player->uuid());
	}
	void remove_player_resting(player_ptr_t player) {
		players_resting.erase(
		    std::remove_if(
		        players_resting.begin(),
		        players_resting.end(),
		[&](auto uuid) -> bool {
			return uuid == player->uuid();
		}
		    )
		);
	}
	void process_hp_regen(player_ptr_t& player) {
		static std::size_t call_count = 0;
		++call_count;
		if((call_count % PROCESS_PLAYER_RESTING_HP_MODULOUS()) != 0) {
			return;
		}
		call_count = 0;
		auto medical_hp_amount = player->medical() * MEDICAL_HP_MULTIPLIER();

		int16_t addition = 1 + medical_hp_amount;
		switch(player->position()) {
			default:
				break;
			case POS_SLEEPING:
			case POS_RESTING:
			case POS_SITTING:
				if(player->max_hp() > player->hp()) {
					player->hp() += addition;
				}
				if(player->hp() > player->max_hp()) {
					player->hp() = player->max_hp();
				}
				break;
		}
	}

	void process_mana_regen(player_ptr_t& player) {
		static std::size_t call_count = 0;
		++call_count;
		if((call_count % PROCESS_PLAYER_RESTING_MANA_MODULOUS()) != 0) {
			return;
		}
		call_count = 0;
		auto intel_bonus  = player->intelligence() * INTELLIGENCE_MANA_MULTIPLIER();
		auto wisdom_bonus  = player->intelligence() * WISDOM_MANA_MULTIPLIER();

		int16_t mana_addition = 1 + intel_bonus + wisdom_bonus;
		switch(player->position()) {
			default:
				break;
			case POS_SLEEPING:
			case POS_RESTING:
			case POS_SITTING:
				if(player->max_mana() > player->mana()) {
					player->mana() += mana_addition;
				}
				if(player->mana() > player->max_mana()) {
					player->mana() = player->max_mana();
				}
				break;
		}
	}

	void process_move_regen(player_ptr_t& player) {
		static std::size_t call_count = 0;
		++call_count;
		if((call_count % PROCESS_PLAYER_RESTING_MOVE_MODULOUS()) != 0) {
			return;
		}
		call_count = 0;
		auto dex_bonus = player->dexterity() * DEXTERITY_MOVEMENT_MULTIPLIER();

		int16_t addition = 2 + dex_bonus;
		switch(player->position()) {
			default:
				break;
			case POS_SLEEPING:
			case POS_RESTING:
			case POS_SITTING:
				if(player->max_move() > player->move()) {
					player->move() += addition;
				}
				if(player->move() > player->max_move()) {
					player->move() = player->max_move();
				}
				break;
		}
	}

	void process_players_resting() {
		for(const auto& player_uuid : players_resting) {
			auto ptr = ptr_by_uuid(player_uuid);
			if(!ptr) {
				continue;
			}
			process_hp_regen(ptr);
			process_mana_regen(ptr);
			process_move_regen(ptr);
		}
	}

};

#undef m_debug
#undef m_error
