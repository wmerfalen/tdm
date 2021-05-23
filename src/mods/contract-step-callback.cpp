#include "scripted-step.hpp"
#include "scripted-sequences.hpp"
#include "contract-step-callback.hpp"
#include "interpreter.hpp"
#include "pfind.hpp"
#include "builder/encode.hpp"
#include "orm/scripted-step.hpp"

#ifdef  __MENTOC_SHOW_CONTRACT_OUTPUT__
#define dbg_print(a) std::cerr << "[mods::contract-step-callback][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#else
#define dbg_print(a)
#endif
namespace mods {
	/**
	 *
	 * Data storage
	 *
	 */
	std::deque<std::shared_ptr<contract_step_callback>>& contract_step_callback_master_list() {
		static std::deque<std::shared_ptr<contract_step_callback>> list;
		return list;
	}
	void load_all_contract_step_callbacks() {
		auto status = mods::orm::load_all_contract_step_callbacks();
		if(std::get<0>(status)) {
			dbg_print(green_str("load_all_contract_step_callbacks:") << "'" << std::get<1>(status) << "'");
		} else {
			dbg_print(red_str("load_all_contract_step_callbacks:") << "'" << std::get<1>(status) << "'");
		}
	}

	/**
	 *
	 * Data querying
	 *
	 */
	std::deque<std::shared_ptr<contract_step_callback>> find_contract_step_callback(const contract_vnum_t& contract_vnum) {

		std::deque<std::shared_ptr<contract_step_callback>> list;
		for(const auto& c : contract_step_callback_master_list()) {
			if(c->contract_vnum == contract_vnum) {
				list.emplace_back(c);
			}
		}
		return list;
	}

	SUPERCMD(do_contract_step_callback) {
		DO_HELP_WITH_ZERO("contract_step_callback");

		if(args()->first_is("list")) {
			player->pager_start();
			player->sendln("Listing...");
			for(const auto& con : contract_step_callback_master_list()) {
				player->sendln(con->dump());
			}
			player->sendln("Done listing.");
			player->pager_end();
			player->page(0);
			return;
		}
		if(args()->save_integer(1)->first_is("search")) {
			player->sendln("Searching...");
			for(const auto& c : find_contract_step_callback(args()->fetch_parsed_integer(1))) {
				player->sendln(c->dump());
			}
			player->sendln("Done listing");
			return;
		}

	}
	namespace contract_step_callback_init {
		void init() {
			mods::interpreter::add_command("contract_step_callback", POS_RESTING, do_contract_step_callback, LVL_BUILDER,0);
			load_all_contract_step_callbacks();
		}
	};

};
