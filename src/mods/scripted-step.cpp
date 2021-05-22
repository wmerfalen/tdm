#include "scripted-step.hpp"
#include "interpreter.hpp"
#ifdef  __MENTOC_SHOW_CONTRACT_OUTPUT__
#define dbg_print(a) std::cerr << "[mods::scripted_step][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#else
#define dbg_print(a)
#endif
namespace mods {

	/**
	 *
	 * Data storage
	 *
	 */
	std::deque<std::shared_ptr<scripted_step>>& scripted_step_master_list() {
		static std::deque<std::shared_ptr<scripted_step>> list;
		return list;
	}
	void load_all_scripted_step() {
#if __MENTOC_USING_ORM_LAYER_FOR_SCRIPTED_SEQUENCES__
		auto& master_list = scripted_step_master_list();
		auto status = mods::orm::load_all_scripted_step(&master_list);
		if(std::get<0>(status)) {
			dbg_print(green_str("load_all_scripted_step:") << "'" << std::get<1>(status) << "'");
		} else {
			dbg_print(red_str("load_all_scripted_step:") << "'" << std::get<1>(status) << "'");
		}
#endif
	}

	/**
	 *
	 * Data querying
	 *
	 */
	std::optional<std::shared_ptr<scripted_step>> find_scripted_step(const scripted_step_vnum_t& scripted_step_vnum) {
		for(const auto& c : scripted_step_master_list()) {
			if(c->vnum == scripted_step_vnum) {
				return c;
			}
		}
		return std::nullopt;
	}

	/**
	 *
	 * control
	 *
	 */
	void start_scripted_step(player_ptr_t& player, scripted_step_vnum_t scripted_step_vnum) {
	}

	ACMD(do_scripted_step) {
		DO_HELP_WITH_ZERO("scripted_step");

		if(args()->first_is_any({"step","steps","current"})) {
			player->sendln("Listing...");
			player->sendln("Done listing...");
			return;
		}

		if(args()->first_is("list")) {
			player->pager_start();
			player->sendln("Listing...");
			for(const auto& con : scripted_step_master_list()) {
				player->sendln(CAT("{yel}[{/yel}",con->vnum,"{yel}]:{/yel}\r\n"));
			}
			player->sendln("Done listing.");
			player->pager_end();
			player->page(0);
			return;
		}
		int scripted_step_num = 0;
		if(args()->save_integer(1)->if_nth_has_either(0, {"join","leave"})) {
			scripted_step_num = args()->fetch_parsed_integer(1);

			if(scripted_step_num == -1) {
				*player << "{red}Invalid scripted_step number.{/red}\r\n";
				return;
			}
			/** find the scripted_step by vnum */
			auto opt_scripted_step = find_scripted_step(scripted_step_num);
			if(opt_scripted_step.has_value() == false) {
				player->send("{red}We could not find a scripted_step with that vnum{/red}\r\n");
				return;
			}
		}

		if(args()->first_is("join")) {
			start_scripted_step(player,scripted_step_num);
			player->sendln("{grn}Your scripted_step has been started! Good luck!{/grn}");
			return;
		}

		if(args()->first_is("leave")) {
			*player << "{red}You have left the scripted_step.{/red}\n";
			return;
		}
	}
	namespace scripted_step_init {
		void init() {
			load_all_scripted_step();
			mods::interpreter::add_command("scripted_step", POS_RESTING, do_scripted_step, 0,0);
		}
	};

};
