#include "intent.hpp"
ACMD(do_gen_door);
#define intent_debug(a) std::cerr << "[mods::intent][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
namespace mods::intent {
	static std::vector<mods::intent::intent_t> intents;
	void register_intent_to_attack(
			uuid_t player,
			std::vector<uuid_t> targets,
			uint8_t direction
	){
		auto p = ptr_by_uuid(player);
		if(!p){
			return;
		}
		mods::intent::intent_t i;
		i.wants_to = mods::intent::intent_to_t::ATTACK;
		i.player = player;
		i.targets = targets;
		i.direction = direction;
		i.from_room_id = p->room();
		i.target_room_id = world[p->room()].dir_option[direction]->to_room;
		i.created_at = mods::globals::current_tick;
		i.timeout = i.created_at + INTENT_TO_ATTACK_TIMEOUT();
		mods::intent::intents.emplace_back(std::move(i));
	}
	void register_intent_to_flee(
			uuid_t player
	){
		auto p = ptr_by_uuid(player);
		if(!p){
			return;
		}
		mods::intent::intent_t i;
		i.wants_to = mods::intent::intent_to_t::FLEE;
		i.player = player;
		for(auto uuid : p->cd()->mob_specials.memory){
			i.targets.emplace_back(uuid);
		}
		i.direction = NORTH;/* not relevant */
		i.from_room_id = p->room();
		i.target_room_id = NORTH; /* not relevant */
		i.created_at = mods::globals::current_tick;
		i.timeout = i.created_at + INTENT_TO_FLEE_TIMEOUT();
		mods::intent::intents.emplace_back(std::move(i));
	}
	uint8_t decorate_door_issues(intent_t& i){
		uint8_t door_issues = 0;
		auto di = world[i.from_room_id].dir_option[i.direction];
		if(!di){
			i.blockers.emplace_back("invalid direction");
			return 1;
		}
#define MENTOC_EX_CHECK(A,MSG)\
		if(di->exit_info & A){\
			i.blockers.emplace_back(MSG);\
			++door_issues;\
		}
		if(di->exit_info & EX_CLOSED){
			MENTOC_EX_CHECK(EX_CLOSED,"door is closed");
			MENTOC_EX_CHECK(EX_LOCKED,"door is locked");
			MENTOC_EX_CHECK(EX_PICKPROOF,"door is pickproof");
			MENTOC_EX_CHECK(EX_REINFORCED,"door is reinforced");
			MENTOC_EX_CHECK(EX_ELECTRIFIED,"door is electrified");
			MENTOC_EX_CHECK(EX_QUEST_LOCKED,"door is quest locked");
			MENTOC_EX_CHECK(EX_HIDDEN, "door is hidden");
		}
#undef MENTOC_EX_CHECK
		return door_issues;
	}

	bool can_attack(intent_t & i){
		/** is the door closed? */
		auto di = world[i.from_room_id].dir_option[i.direction];
		if(!di){
			i.blockers.emplace_back("invalid direction");
			return false;
		}
		auto door_issues = decorate_door_issues(i);
		if(door_issues){
			auto p = ptr_by_uuid(i.player);
			do_gen_door(p->cd(),CAT("open ",dirstr(i.direction)).data(),0,0,p);
		}
		return true;
	}
	bool can_flee(intent_t & i){
		return true;
	}
	void run_intents(){
		std::cerr << "[run_intents]\n";
		std::vector<intent_t> keep;
		for(auto & intent : mods::intent::intents){
			auto p = ptr_by_uuid(intent.player);
			if(!p){
				continue;
			}
			if(intent.timeout >= mods::globals::current_tick){
				continue;
			}
			switch(intent.wants_to){
				case intent_to_t::ATTACK:
					if(can_attack(intent)){
						continue;
					}
					break;
				case intent_to_t::FLEE:
					if(can_flee(intent)){
						continue;
					}
					break;
				default:
					continue;
			}
			keep.emplace_back(intent);
		}
		mods::intent::intents = keep;
	}

};
