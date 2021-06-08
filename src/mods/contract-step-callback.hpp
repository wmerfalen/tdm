#ifndef __MENTOC_MODS_CONTRACT_STEP_CALLBACK_HEADER__
#define __MENTOC_MODS_CONTRACT_STEP_CALLBACK_HEADER__
#include "../structs.h"
#include "contract-steps.hpp"
#include "contract-types.hpp"
#include "contracts.hpp"
#include "scripted-step.hpp"
#include <vector>

namespace mods {
	using sequence_vnum_t = uint32_t;
	using task_vnum_t = uint32_t;
	struct contract_step_callback {
		contract_vnum_t contract_vnum;
		sequence_vnum_t sequence_vnum;
		task_vnum_t task_vnum;
		std::string task_type;
		std::string task_target;
		std::string dump() const {
			auto d = CAT(
			             "{grn}[contract_vnum]:{/grn}{yel}",contract_vnum,"{yel}\r\n",
			             "{grn}[sequence_vnum]:{/grn}{yel}",sequence_vnum,"{yel}\r\n",
			             "{grn}[task_vnum]:{/grn}{yel}",task_vnum,"{yel}\r\n",
			             "{grn}[task_type]:{/grn}{yel}",task_type,"{yel}\r\n",
			             "{grn}[task_target]:{/grn}{yel}",task_target,"{yel}\r\n"
			         );
			unsigned i = 0;
			for(const auto& s : steps) {
				d += CAT("{grn}[step ",i++,"]{/grn}\r\n",
				         "{red}-------[DUMP START]--------{/red}\r\n",
				         s.dump(),
				         "{red}-------[DUMP END]--------{/red}\r\n"
				        );
			}
			return d;
		}
		contract_step_callback() = default;
		contract_step_callback(const contract_step_callback& other);
		std::vector<scripted_step> steps;
	};
};
#endif
