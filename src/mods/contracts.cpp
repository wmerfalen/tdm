#include "contracts.hpp"
#include "interpreter.hpp"
#include "pfind.hpp"
#include "orm/contract-steps.hpp"
#include "builder/encode.hpp"
#include "orm/contracts.hpp"
#include "orm/player-contract-state.hpp"
#include "player-contract-instance.hpp"
#ifdef  __MENTOC_SHOW_CONTRACT_OUTPUT__
#define dbg_print(a) std::cerr << "[mods::contracts][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#else
#define dbg_print(a)
#endif
namespace mods::contracts {
	using task_t = contract_step::task_type_t;
	using target_t = contract_step::task_target_t;

	/**
	 *
	 * Data storage
	 *
	 */
	std::deque<std::shared_ptr<contract>>& contract_master_list() {
		static std::deque<std::shared_ptr<contract>> list;
		return list;
	}
	void load_all_contracts() {
		auto& master_list = contract_master_list();
		auto status = mods::orm::load_all_non_orm_contracts(&master_list);
		if(std::get<0>(status)) {
			dbg_print(green_str("load_all_contracts:") << "'" << std::get<1>(status) << "'");
		} else {
			dbg_print(red_str("load_all_contracts:") << "'" << std::get<1>(status) << "'");
		}
	}
	void clear_all_contracts() {
		contract_master_list().clear();
	}

	/**
	 *
	 * Data querying
	 *
	 */
	std::optional<std::shared_ptr<contract>> find_contract(const contract_vnum_t& contract_vnum) {
		for(const auto& c : contract_master_list()) {
			if(c->vnum == contract_vnum) {
				return c;
			}
		}
		return std::nullopt;
	}

	/**
	 *
	 * Contract control
	 *
	 */
	void start_contract(player_ptr_t& player, contract_vnum_t contract_vnum) {
		player->start_contract(contract_vnum);
	}

	std::tuple<bool,std::string> stop_contract(player_ptr_t& player, contract_vnum_t contract_vnum) {
		player->stop_contract(contract_vnum);
		return {0,"stub"};
	}

	void punish_for_leaving_contract(std::shared_ptr<mods::player>& player,int contract_num) {
		/** TODO: dock player mp */
	}

	bool has_contract(player_ptr_t& player) {
		return player->has_contract();
	}

	ACMD(do_contract) {
		DO_HELP_WITH_ZERO("contract");

		if(args()->first_is_any({"step","steps","current"})) {
			if(!has_contract(player)) {
				player->sendln("You have no current contracts.");
				return;
			}
			player->sendln("Listing...");
			for(const auto& c : player->contracts()) {
				if(c->finished()) {
					player->send(CAT("{grn}Finished{/grn}:\t{yel}[",c->vnum(),"]{/yel}::{grn}",c->title().data(),"{/grn}\r\n").c_str());
					continue;
				}
				player->send(CAT("{red}Unfinished{/red}:\t{yel}[",c->vnum(),"]{/yel}::{grn}",c->title().data(),"{/grn}\r\n").c_str());
				if(args()->first_is_any({"step","steps"})) {
					player->send(c->pretty_dump_step().data());
				}
			}
			player->sendln("Done listing...");
			return;
		}
		if(argshave()->first_is_any({"show","describe"})->nth_has_integer(1)->size_gt(1)->passed()) {
			auto v = intat(1);
			player->sendln("\r\nYou pull up a list of contracts in your iDroid...");
			player->sendln("\r\nYou run a search for a detailed list of contracts matching your inquiry...");
			for(const auto& c : contract_master_list()) {
				if(c->vnum == v) {
					player->sendln("{grn}Your iDroid beeps a confirmation...{/grn}");
					player->sendln(CAT("{grn}Contract ID:{yel}",v,"{/yel}\r\n\r\n",
					                   "{grn}----[ BEGIN TRANSCRIPT ]----{/grn}\r\n\r\n",
					                   c->description.data(),"\r\n",
					                   "{/grn}----[ END TRANSCRIPT ]----{/grn}\r\n"));
					return;
				}
			}
			player->sendln("Your iDroid blurts a negative audio tone: {red}Zero results returned{/red}");
			return;
		}


		if(argshave()->first_is("list")->passed()) {
			player->sendln("Listing...");
			for(const auto& con : contract_master_list()) {
				player->sendln(CAT("{yel}[{/yel}",con->vnum,"{yel}]:{/yel} {grn}'",con->title,"'{/grn}\r\n"));
			}
			player->sendln("Done listing.");
			return;
		}
		int contract_num = 0;
		if(args()->save_integer(1)->if_nth_has_either(0, {"join","leave"})) {
			contract_num = args()->fetch_parsed_integer(1);

			if(contract_num == -1) {
				*player << "{red}Invalid contract number.{/red}\r\n";
				return;
			}
			/** find the contract by vnum */
			auto opt_contract = find_contract(contract_num);
			if(opt_contract.has_value() == false) {
				player->send("{red}We could not find a contract with that vnum{/red}\r\n");
				return;
			}
		}

		if(args()->first_is("join")) {
			if(has_contract(player)) {
				player->send("{red}You are already part of a contract. Quit or finish that one before starting a new contract.{/red}\r\n");
				return;
			}
			start_contract(player,contract_num);
			player->sendln("{grn}Your contract has been started! Good luck!{/grn}");
			return;
		}

		if(args()->first_is("leave")) {
			stop_contract(player,contract_num);
			*player << "{red}You have left the contract.{/red}\n";
			mods::contracts::punish_for_leaving_contract(player,contract_num);
			return;
		}
	}
	void init() {
		mods::interpreter::add_command("contract", POS_RESTING, do_contract, 0,0);
		mods::interpreter::add_command("quest", POS_RESTING, do_contract, 0,0);
		mods::interpreter::add_command("quests", POS_RESTING, do_contract, 0,0);
		mods::interpreter::add_command("contracts", POS_RESTING, do_contract, 0,0);
		load_all_contracts();
	}

};
