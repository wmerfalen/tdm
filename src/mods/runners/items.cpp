#include "../../structs.h"
#include "../player.hpp"
#include "includes.hpp"
#include "../item.hpp"

#ifdef __MENTOC_SCRIPTED_SEQUENCE_RUNNER_DEBUG__
#define dbg_print(A) *player << "{grn}scripted_sequence_runner_debug{/grn}: " << A << "\r\n";
#else
#define dbg_print(A)
#endif

namespace mods::runners::items {
	using namespace mods::scripted_sequence_runner;
	void mob_gives_yaml(step_ptr_t step,const uuid_t& player_uuid) {
		auto mob = qmob(step->room,step->mob);
		if(mob == nullptr) {
			std::cerr << red_str("Error finding mob in room:") << step->dump() << "\n";
		} else {
			std::tuple<int,std::string> y = mods::util::extract_yaml_info_from_path(step->yaml);
			if(std::get<0>(y) < 0) {
				log(CAT("SYSERR: issue extracting type and path from mob_gives_yaml runner. {",std::get<0>(y),",'",std::get<1>(y),"'}").c_str());
			} else {
				auto player = ptr_by_uuid(player_uuid);
				if(!player) {
					log(CAT("SYSERR: issue from mob_gives_yaml runner. Player uuid doesn't exist. Play most likely disconnected... {",std::get<0>(y),",'",std::get<1>(y),"'}").c_str());
				} else {
					for(uint8_t i=0; i < step->quantity; i++) {
						auto obj = create_object(std::get<0>(y),std::get<1>(y));
						mods::item::perform_give(mob, player, obj);
					}
				}
			}
		}
		queue_for_deferred_removal(step);
	}
};//end namespace runners

