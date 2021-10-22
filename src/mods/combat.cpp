#include "combat.hpp"
#include "combat-composer/snipe-target.hpp"
#include "interpreter.hpp"

namespace mods::combat {
	namespace rules_of_engagement {
		std::string to_string(mods::combat::rules_of_engagement_t rule) {
			switch(rule) {
				case ROM_BALLISTIC:
					return "BALLISTIC";
				case ROM_CQC:
					return "CQC";
				case ROM_AUXILIARY:
					return "AUXILIARY";
				case ROM_SECONDARY:
					return "SECONDARY";
				default:
					return "<unknown>";
			}
		}
		rules_of_engagement_t from_string(std::string_view rule) {
			if(mods::util::is_lower_match("auxiliary",rule) || mods::util::is_lower_match("aux",rule)) {
				return ROM_AUXILIARY;
			}
			if(mods::util::is_lower_match("cqc",rule)) {
				return ROM_CQC;
			}
			if(mods::util::is_lower_match("secondary",rule)) {
				return ROM_SECONDARY;
			}
			return ROM_BALLISTIC;
		}
	};
	namespace commands {
		ACMD(do_set_rules_of_engagement) {
			const static char* usage = "usage: set_rules_of_engagement <ballistic|cqc|auxiliary|aux|secondary>\r\n";
			if(argshave()->size_gt(0)->passed() == false) {
				player->send(usage);
				return;
			}
			player->sendln(CAT("argat(0): '",argat(0),"'"));
			auto rule = mods::combat::rules_of_engagement::from_string(argat(0));
			player->rules_of_engagement() = rule;
			player->sendln(CAT("Rules of engagement set to: {grn}",mods::combat::rules_of_engagement::to_string(player->rules_of_engagement()),"{/grn}"));
		}
		void init() {
			ADD_USER_COMMAND("set_rules_of_engagement",do_set_rules_of_engagement);
		}
	};
	/**
	 * Handles both ranged and immediate targets
	 */
	void snipe_target(player_ptr_t& attacker,std::string_view target, direction_t direction,uint8_t distance,obj_ptr_t& weapon) {
		mods::combat_composer::snipe_target(attacker,target,direction,distance,weapon);
	}
#if 0

	/**
	 * Handles spraying only
	 */
	void spray_target(player_ptr_t& attacker,direction_t direction,obj_ptr_t& weapon) {
		mods::combat_composer::spray_target(attacker,direction,weapon);

	}

	/**
	 * Handles launching only
	 */
	void launch_towards(player_ptr_t& attacker,direction_t direction,uint8_t distance,obj_ptr_t& weapon) {
		mods::combat_composer::launch_towards(attacker,direction,distance,weapon);

	}
#endif

};
