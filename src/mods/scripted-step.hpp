#ifndef __MENTOC_MODS_scripted_stepS_HEADER__
#define __MENTOC_MODS_scripted_stepS_HEADER__
#include "../structs.h"
#include "contracts.hpp"

namespace mods::orm {
	struct scripted_step;
};

namespace mods {
	using contract_vnum_t = mods::contracts::contract_vnum_t;
	using task_type_t = mods::contracts::contract_step::task_type_t;
	using task_target_t = mods::contracts::contract_step::task_target_t;
	using scripted_step_vnum_t = uint32_t;
	struct scripted_step {
		uint16_t wait_ticks; /** how many ticks to wait until executing this */
		std::string dialogue; /** text to display */
		mob_vnum mob;
		obj_vnum obj;
		room_vnum room;
		std::string yaml;
		std::string type;
		std::vector<std::string> interpret;
		uint16_t quantity;
		scripted_step_vnum_t vnum;
		uint16_t order;
		std::size_t temp_id;

		scripted_step() = default;
		scripted_step(const scripted_step& other) {
			std::cerr << green_str("[scripted_step]:: copy constructor") << "\n";
			wait_ticks = other.wait_ticks;
			dialogue.assign(other.dialogue);
			mob = other.mob;
			obj = other.obj;
			room = other.room;
			yaml.assign(other.yaml);
			type.assign(other.type);
			interpret = other.interpret;
			quantity = other.quantity;
			vnum = other.vnum;
			order = other.order;
			temp_id = other.temp_id;
		}
		std::string dump() const {
			return CAT(
			           "{grn}[wait_ticks]{/grn}{yel}",wait_ticks,"{/yel}\r\n",
			           "{grn}[dialogue]{/grn}{yel}",dialogue.c_str(),"{/yel}\r\n",
			           "{grn}[mob]{/grn}{yel}",mob,"{/yel}\r\n",
			           "{grn}[obj]{/grn}{yel}",obj,"{/yel}\r\n",
			           "{grn}[room]{/grn}{yel}",room,"{/yel}\r\n",
			           "{grn}[yaml]{/grn}{yel}",yaml.c_str(),"{/yel}\r\n",
			           "{grn}[type]{/grn}{yel}",type.c_str(),"{/yel}\r\n",
			           "{grn}[interpret]{/grn}{yel}",IMPLODE(interpret,",").c_str(),"{/yel}\r\n",
			           "{grn}[quantity]{/grn}{yel}",quantity,"{/yel}\r\n",
			           "{grn}[vnum]{/grn}{yel}",vnum,"{/yel}\r\n",
			           "{grn}[order]{/grn}{yel}",order,"{/yel}\r\n"
			       );
		}
	};
	void register_scripted_sequence(uint32_t sequence_vnum, std::vector<int> sequence);
	void register_contract_step_callback(contract_vnum_t, task_type_t, task_target_t, uint32_t task_vnum, uint32_t sequence_vnum);
};
#endif
