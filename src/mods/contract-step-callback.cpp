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
		dbg_print(green_str("load_all_contract_step_callbacks"));
		auto status = mods::orm::load_all_contract_step_callbacks();
		if(std::get<0>(status)) {
			dbg_print(green_str("load_all_contract_step_callbacks:") << "'" << std::get<1>(status) << "'");
		} else {
			dbg_print(red_str("load_all_contract_step_callbacks:") << "'" << std::get<1>(status) << "'");
		}
	}
	contract_step_callback::contract_step_callback(const contract_step_callback& other) {
		dbg_print(green_str("[contract_step_callback]::copy constructor"));
		contract_vnum = other.contract_vnum;
		sequence_vnum = other.sequence_vnum;
		task_vnum = other.task_vnum;
		task_type = other.task_type;
		task_target = other.task_target;
		for(const auto& step : other.steps) {
			steps.emplace_back(step);
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
		if(argshave()->first_is_any({"reload","reload-all","reload-all-orm"})->passed()) {
			player->sendln("Reloading...");
			load_all_contract_step_callbacks();
			player->sendln("Done");
			return;
		}

		if(args()->first_is("help")) {
			std::string help =
			    CAT(
			        "list - lists all callacks\r\n",
			        "list-by-contract-vnum - lists by contract vnum\r\n",
			        "list-by-sequence-vnum - lists by sequence vnum\r\n",
			        "create <contract-vnum> <task-type> <task-target> <task-vnum> <sequence-vnum>\r\n"
			        "delete <contract-vnum> <sequence-vnum> - delete matching callbacks that have contract and sequence vnum that matches\r\n"
			        "reload - clears then reloads all contract step callback rows into memory\r\n"
			    );
			player->sendln(help);
			return;
		}
		if(args()->first_is("reload")) {
			player->sendln("Reloading...");
			std::tuple<int16_t,std::string> status = mods::orm::load_all_contract_step_callbacks();
			player->send(CAT("code: ",std::get<0>(status),", msg: '",std::get<1>(status),"'\r\n").c_str());
			player->sendln("Done.");
			return;
		}
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
		/**
		 * args()=>
		 * [0] create
		 * [1] => contract-vnum
		 * [2] => task-type (csv)
		 * [3] => task-target
		 * [4] => task-vnum
		 * [5] => seq_vnum
		 */
		if(argshave()->int_at({1,4,5})->first_is("create")->size_gt(5)->passed()) {
			player->sendln("Searching...");
			auto map = args()->int_map({1,4,5});
			player->sendln(CAT("contract:",map[1],", task:",map[4],"sequence:",map[5]));
			mods::orm::contract_step_callback orm(map[1],argat(2),argat(3),map[4],map[5]);
			player->sendln(CAT("{grn}Created row dump:{/grn}", orm.dump(),"\r\n"));
			player->sendln("Done listing");
			return;
		}
		/**
		 * args()
		 * [0] delete
		 * [1] contract-vnum
		 * [2] sequence-vnum
		 */
		if(argshave()->int_at({1,2})->first_is("delete")->size_gt(2)->passed()) {
			player->sendln("Searching...");
			for(const auto& c : find_contract_step_callback(args()->int_at(1))) {
				player->sendln(c->dump());
			}
			player->sendln("Done listing");
			return;
		}

		/**
		 * args()
		 * [0] list-by-contract-vnum
		 * [1] contract vnum
		 * [N] ...
		 */
		if(argshave()->int_at(1)->first_is("list-by-contract-vnum")->size_gt(1)->passed()) {
			player->sendln("Searching...");
			for(const auto& c_vnum : args()->gather_integers_starting_at(1)) {
				for(const auto& c : find_contract_step_callback(c_vnum)) {
					if(c->contract_vnum == c_vnum) {
						player->sendln(c->dump());
					}
				}
			}
			player->sendln("Done listing");
			return;
		}
		/**
		 * args()
		 * [0] list-by-sequence-vnum
		 * [1] contract vnum
		 * [N] ...
		 */
		if(argshave()->int_at(1)->first_is("list-by-sequence-vnum")->size_gt(2)->passed()) {
			player->sendln("Searching...");
			for(const auto& seq_vnum : args()->gather_integers_starting_at(1)) {
				for(const auto& c : find_contract_step_callback(seq_vnum)) {
					if(c->sequence_vnum == seq_vnum) {
						player->sendln(c->dump());
					}
				}
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
