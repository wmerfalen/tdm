#include "scripted-sequence-runner.hpp"
#include "interpreter.hpp"
#include "../comm.h"
#include <list>
#include "doors.hpp"
#include "runners/includes.hpp"

#define __MENTOC_SCRIPTED_SEQUENCE_RUNNER_DEBUG__
#ifdef __MENTOC_SCRIPTED_SEQUENCE_RUNNER_DEBUG__
#define dbg_print(A) *player << "{grn}scripted_sequence_runner_debug{/grn}: " << A << "\r\n";
#else
#define dbg_print(A)
#endif

namespace mods::scripted_sequence_runner {

	using step_ptr_t = std::shared_ptr<mods::scripted_step>;
	std::size_t hash_step(step_ptr_t s) {
		return std::hash<step_ptr_t>()(s);
	}
	std::size_t step_id_generator() {
		static std::size_t index = 0;
		return index++;
	}
	struct step_info_t {
		std::size_t id;
		step_ptr_t step;
		step_info_t(step_ptr_t& s) : id(step_id_generator()), step(s) {
			s->temp_id = id;
		}
	};

	static std::list<step_info_t> deferred_list;
	static std::set<std::size_t> steps_to_remove;

	void defer_step(uint32_t ticks,step_ptr_t s,player_ptr_t& player) {
		deferred_list.emplace_back(s);
		DEFER_STEP(ticks,deferred_list.back().id,player->uuid());
	}

	void queue_for_deferred_removal(step_ptr_t in_step) {
		steps_to_remove.insert(in_step->temp_id);
	}

	void cleanup_scripted_steps() {
		if(steps_to_remove.size() == 0) {
			return;
		}
		std::cerr << green_str("Size of deferred_list BEFORE:") << deferred_list.size() << "\n";
		deferred_list.remove_if([&](const auto& step_info) -> bool {
			auto it = steps_to_remove.find(step_info.id);
			if(it != steps_to_remove.end()) {
				steps_to_remove.erase(it);
				return true;
			}
			return false;
		});
		std::cerr << green_str("Size of deferred_list AFTER:") << deferred_list.size() << "\n";
	}

	/**
	 * This gets called by the deferred class when you register
	 * a step using push_step()
	 */
	void step_runner(std::size_t id,const uuid_t& player_uuid) {
		using namespace mods::runners;
		for(const auto& i : deferred_list) {
			if(i.id == id) {
				if(i.step->type.compare("act") == 0) {
					return communication::act(i.step,player_uuid);
				}
				if(i.step->type.compare("mob_gives_yaml") == 0) {
					return items::mob_gives_yaml(i.step,player_uuid);
				}
				if(i.step->type.compare("say") == 0 || i.step->type.compare("dialogue") == 0) {
					return communication::dialogue(i.step,player_uuid);
				}
				if(i.step->type.compare("open_east") == 0) {
					return movement::open(i.step,EAST,player_uuid);
				}
				if(i.step->type.compare("open_south") == 0) {
					return movement::open(i.step,SOUTH,player_uuid);
				}
				if(i.step->type.compare("open_west") == 0) {
					return movement::open(i.step,WEST,player_uuid);
				}
				if(i.step->type.compare("open_north") == 0) {
					return movement::open(i.step,NORTH,player_uuid);
				}
				if(i.step->type.compare("open_up") == 0) {
					return movement::open(i.step,UP,player_uuid);
				}
				if(i.step->type.compare("open_down") == 0) {
					return movement::open(i.step,DOWN,player_uuid);
				}
				if(i.step->type.compare("unlock_east") == 0) {
					return movement::unlock(i.step,EAST,player_uuid);
				}
				if(i.step->type.compare("unlock_south") == 0) {
					return movement::unlock(i.step,SOUTH,player_uuid);
				}
				if(i.step->type.compare("unlock_west") == 0) {
					return movement::unlock(i.step,WEST,player_uuid);
				}
				if(i.step->type.compare("unlock_north") == 0) {
					return movement::unlock(i.step,NORTH,player_uuid);
				}
				if(i.step->type.compare("unlock_up") == 0) {
					return movement::unlock(i.step,UP,player_uuid);
				}
				if(i.step->type.compare("unlock_down") == 0) {
					return movement::unlock(i.step,DOWN,player_uuid);
				}

				if(i.step->type.compare("walk_north") == 0) {
					return movement::walk(i.step,NORTH,player_uuid);
				}
				if(i.step->type.compare("walk_east") == 0) {
					return movement::walk(i.step,EAST,player_uuid);
				}
				if(i.step->type.compare("walk_south") == 0) {
					return movement::walk(i.step,SOUTH,player_uuid);
				}
				if(i.step->type.compare("walk_west") == 0) {
					return movement::walk(i.step,WEST,player_uuid);
				}
				if(i.step->type.compare("walk_up") == 0) {
					return movement::walk(i.step,UP,player_uuid);
				}
				if(i.step->type.compare("walk_down") == 0) {
					return movement::walk(i.step,DOWN,player_uuid);
				}
				if(i.step->type.compare("seal_room_south") == 0) {
					return movement::seal_room(i.step,SOUTH,player_uuid);
				}
				if(i.step->type.compare("seal_room_north") == 0) {
					return movement::seal_room(i.step,NORTH,player_uuid);
				}
				if(i.step->type.compare("seal_room_east") == 0) {
					return movement::seal_room(i.step,EAST,player_uuid);
				}
				if(i.step->type.compare("seal_room_west") == 0) {
					return movement::seal_room(i.step,WEST,player_uuid);
				}
				if(i.step->type.compare("seal_room_up") == 0) {
					return movement::seal_room(i.step,UP,player_uuid);
				}
				if(i.step->type.compare("seal_room_down") == 0) {
					return movement::seal_room(i.step,DOWN,player_uuid);
				}
				if(i.step->type.compare("seal_room_all") == 0) {
					for(const auto& dir : directions()) {
						movement::seal_room(i.step,dir,player_uuid);
					}
					return;
				}
				if(i.step->type.compare("force_south") == 0) {
					return movement::force(i.step,SOUTH,player_uuid);
				}
				if(i.step->type.compare("force_north") == 0) {
					return movement::force(i.step,NORTH,player_uuid);
				}
				if(i.step->type.compare("force_east") == 0) {
					return movement::force(i.step,EAST,player_uuid);
				}
				if(i.step->type.compare("force_west") == 0) {
					return movement::force(i.step,WEST,player_uuid);
				}
				if(i.step->type.compare("force_up") == 0) {
					return movement::force(i.step,UP,player_uuid);
				}
				if(i.step->type.compare("force_down") == 0) {
					return movement::force(i.step,DOWN,player_uuid);
				}
			}
		}
	}

	void dispatch(player_ptr_t& player,sequence_vnum_t sequence_vnum) {
		uint64_t ticks = 0;
		for(const auto& seq : mods::scripted_sequences_master_list()) {
			if(seq->vnum == sequence_vnum) {
				for(const auto& step : seq->steps) {
					ticks += std::clamp((int)step->wait_ticks,1,UINT16_MAX);
					defer_step(ticks,step,player);
				}
			}
		}
	}
	void dispatch(player_ptr_t& player,sequence_vnum_t sequence_vnum,contract_vnum_t contract_vnum,step_t step_copy) {
		uint64_t ticks = 0;
		for(const auto& seq : mods::scripted_sequences_master_list()) {
			if(seq->vnum == sequence_vnum) {
				for(const auto& step : seq->steps) {
					ticks += std::clamp((int)step->wait_ticks,1,UINT16_MAX);
					defer_step(ticks,step,player);
				}
			}
		}
	}

	SUPERCMD(do_run_sequence) {
		if(argshave()->first_is("list")->passed()) {
			player->sendln("Listing...");
			for(const auto& seq : mods::scripted_sequences_master_list()) {
				player->sendln(CAT(seq->vnum).c_str());
			}
			player->sendln("Done listing...");
			return;
		}
		if(argshave()->int_at(0)->size_gt(0)->passed()) {
			for(const auto& seq : mods::scripted_sequences_master_list()) {
				if(seq->vnum == intat(0)) {
					player->sendln(CAT("Running sequence [",intat(0),"]...").c_str());
					dispatch(player,seq->vnum);
					player->sendln("Dispatched.");
				}
			}
			return;
		}
	}

	void init() {
		ADD_BUILDER_COMMAND("run_sequence",do_run_sequence);
	}



};
