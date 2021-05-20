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
	std::deque<std::shared_ptr<contract>>& contract_master_list() {
		static std::deque<std::shared_ptr<contract>> list;
		return list;
	}
	std::optional<std::shared_ptr<contract>> find_contract(const contract_vnum_t& contract_vnum) {
		for(const auto& c : contract_master_list()) {
			if(c->vnum == contract_vnum) {
				return c;
			}
		}
		return std::nullopt;
	}
	void start_contract(player_ptr_t& player, contract_vnum_t contract_vnum) {
		player->contracts().emplace_back(std::make_shared<player_contract_instance>(contract_vnum,player->db_id()));
	}

	std::tuple<bool,std::string> stop_contract(player_ptr_t& player, contract_vnum_t contract_vnum) {
		auto& d = player->contracts();
		std::remove_if(d.begin(),d.end(),[contract_vnum](std::shared_ptr<player_contract_instance>& ptr) -> bool {
			bool is_contract = ptr->contract_vnum() == contract_vnum;
			if(is_contract) {
				ptr->stop_contract();
			}
			return is_contract;
		});
		player->contracts() = std::move(d);
		return {0,"stub"};
	}

	void load_c_functions(duk_context *ctx) {

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

	void punish_for_leaving_contract(std::shared_ptr<mods::player>& player,int contract_num) {
		/** TODO: dock player mp */
	}

	/**
	 * @brief generates player_name:contract
	 *
	 * @param player
	 *
	 * @return
	 */
	std::string current_contract_key(player_ptr_t& player) {
		return CAT(player->name().c_str(),":contract");
	}

	/**
	 * @brief generates ROOMVNUM:CONTRACT_INDEX
	 *
	 * @param room
	 * @param t_index
	 *
	 * @return
	 */
	std::string current_contract_value(room_vnum room,int t_index) {
		return CAT(std::to_string(room),":",std::to_string(t_index));
	}

	/* Whether or not the contract has been completed */
	/**
	 * @brief generates player_name:ROOMVNUM:CONTRACT_INDEX:contract_complete
	 *
	 * @param player
	 * @param room
	 * @param n_index
	 *
	 * @return
	 */
	std::string complete_key(player_ptr_t& player,room_vnum room,int n_index) {
		return CAT(player->name().c_str(),":",room,":",n_index,":contract_complete");
	}

	/**
	 * @brief generates player_name:ROOMVNUM:CONTRACT_INDEX:contract_trigger
	 *
	 * @param player
	 * @param room
	 * @param n_index
	 *
	 * @return
	 */
	std::string trigger_key(player_ptr_t& player,room_vnum room,int n_index) {
		return CAT(player->name().c_str(),":",room,":",n_index,":contract_trigger");
	}

	bool has_contract(player_ptr_t& player) {
		return !!player->contracts().size();
	}

	void load_contract_code(player_ptr_t& player,room_vnum rvnum,int contract_id) {
		std::string js_code = CAT("contract_trigger_",rvnum,"_",contract_id,"(",player->uuid(),");");
		DBSET(trigger_key(player,rvnum,contract_id),js_code);
	}

	int contracts_file_to_lmdb(player_ptr_t& player,const std::string& contracts_file,const std::string& lmdb_key) {
		std::ifstream include_file(contracts_file,std::ios::in);

		if(!include_file.good() || !include_file.is_open()) {
			dbg_print("not opening contracts file" << contracts_file);
			return -1;
		} else {
			std::vector<char> buffer;
			struct stat statbuf;

			if(stat(contracts_file.c_str(), &statbuf) == -1) {
				return -2;
			}

			buffer.reserve(statbuf.st_size + 1);
			std::fill(buffer.begin(),buffer.end(),0);
			include_file.read((char*)&buffer[0],statbuf.st_size);
			std::string buf = static_cast<char*>(&buffer[0]);
			/* start interpolation */
			buf = mods::globals::replace_all(buf,"{character}",player->name().c_str());
			DBSET(lmdb_key,buf);
			return statbuf.st_size;
		}
	}

	int run_trigger(player_ptr_t& player) {
		std::string js_code;
		std::string vnum_index;
		DBGET(current_contract_key(player),vnum_index);
		/**
		 * format of vnum_index will be Q_CURRENT_KEY
		 * which is: rvnum:INDEX
		 */
		std::vector<std::string> parts;
		std::string current = "";
		for(auto ch : vnum_index) {
			if(ch == ':') {
				parts.emplace_back(current);
				current.clear();
				continue;
			}
			current += ch;
		}
		parts.emplace_back(current);

		if(parts.size() < 2) {
			dbg_print(red_str("ERROR: contract current key is not formatted correctly"));
			return -1;
		}
		DBGET(trigger_key(player,mods::util::stoi(parts[0]).value_or(-1),mods::util::stoi(parts[1]).value_or(-1)),js_code);
		dbg_print("js_code for trigger: " << green_str(js_code));
		mods::js::eval_string(js_code);
		return 0;	//TODO: make use of return value to signify something?
	}

	bool trigger_exists(player_ptr_t& player,int contract_id) {
		return false;
	}

	/**
	 * @brief gives the player the rewards for completing the contract
	 *
	 * @param player
	 * @param contract_major
	 */
	void award_contract(player_ptr_t& player,int contract_major) {
		//TODO: Calculate contract reward tiers
		player->gold() += 5000;
		player->exp() += 5000;
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

		if(args()->first_is("list")) {
			player->pager_start();
			player->sendln("Listing...");
			for(const auto& con : contract_master_list()) {
				player->sendln(CAT("{yel}[{/yel}",con->vnum,"{yel}]:{/yel} {grn}'",con->title,"'{/grn}\r\n"));
			}
			player->sendln("Done listing.");
			player->pager_end();
			player->page(0);
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
