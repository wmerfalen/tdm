#include "contracts.hpp"
#include "interpreter.hpp"
#include "pfind.hpp"
#include "orm/contract-steps.hpp"
#ifdef  __MENTOC_SHOW_CONTRACT_OUTPUT__
#define dbg_print(a) std::cerr << "[mods::mobs::lowly_security_behaviour_tree][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#else
#define dbg_print(a)
#endif
namespace mods::contracts {
	/**
	 * TODO: make it so that a player can only track one contract at a time
	 */
	/**
	 * @brief abort contract
	 *
	 * @param ctx
	 *
	 * @return
	 */
	static duk_ret_t contract_abort(duk_context *ctx) {
		std::string pname = duk_to_string(ctx,0);
		std::string contract_major = duk_to_string(ctx,1);
		std::string contract_minor = duk_to_string(ctx,2);

		auto player = mods::pfind::by_name(pname.c_str());
		if(player == mods::globals::player_nobody) {
			return 0;
		}
		auto i_contract_major = mods::util::stoi(contract_major);
		if(i_contract_major.has_value()) {
			leave_contract(player,i_contract_major.value());
		}
		return 0;
	}
	/**
	 * @brief called when contract is completed. awards and leaves quest
	 *
	 * @param ctx
	 *
	 * @return
	 */
	static duk_ret_t contract_complete(duk_context *ctx) {
		std::string pname = duk_to_string(ctx,0);
		std::string contract_major = duk_to_string(ctx,1);
		std::string contract_minor = duk_to_string(ctx,2);

		auto player = mods::pfind::by_name(pname.c_str());
		if(player == mods::globals::player_nobody) {
			return 0;
		}
		auto i_contract_major = mods::util::stoi(contract_major);
		if(i_contract_major.has_value()) {
			leave_contract(player,i_contract_major.value());
			award_contract(player,i_contract_major.value());
		}
		return 0;
	}
	/**
	 * @brief get a list of contracts available in this current room
	 *
	 * @param ctx
	 *
	 * @return
	 */
	static duk_ret_t list_contracts(duk_context *ctx) {
		std::string pname = duk_to_string(ctx,0);

		auto player = mods::pfind::by_name(pname.c_str());
		if(player == mods::globals::player_nobody) {
			return 0;
		}
		auto contracts = list_contracts(player->vnum());
		for(auto qname : contracts) {
			dbg_print(qname);
			*player << "{grn}[ QUEST ]{/grn} " << qname << "\r\n";
		}
		return 0;
	}

	/**
	 * @brief returns true if the npc/player is still alive
	 *
	 * @param ctx pass in uuid
	 *
	 * @return js:boolean if npc/player is alive
	 */
	static duk_ret_t still_alive(duk_context *ctx) {
		uuid_t npc_uuid = duk_to_number(ctx,0);
		auto npc = ptr_by_uuid(npc_uuid);
		duk_push_boolean(ctx,!!npc);
		return 1;
	}
	void load_c_functions(duk_context *ctx) {
		//
		duk_push_c_function(ctx,mods::contracts::list_contracts,1);
		duk_put_global_string(ctx,"list_contracts");
		duk_push_c_function(ctx,mods::contracts::contract_complete,3);
		duk_put_global_string(ctx,"contract_complete");
		duk_push_c_function(ctx,mods::contracts::contract_abort,3);
		duk_put_global_string(ctx,"contract_abort");
		duk_push_c_function(ctx,mods::contracts::still_alive,2);
		duk_put_global_string(ctx,"still_alive");
	}
	std::deque<contract>& contract_list() {
		static std::deque<contract> list;
		return list;
	}
	void load_all_contracts() {

	}

	void punish_for_leaving_contract(std::shared_ptr<mods::player>& player,int contract_num) {
		/** TODO: dock player mp */
	}

	/*
	 * contract:rvnum:N:name
	 * contract:rvnum:N:description
	 * <player.name()>.contract -> contract:rvnum:N
	 * <player.name()>.contract:rvnum:N:trigger:N -> javascript block
	 * <player.name()>.contract:rvnum:N:trigger:N -> javascript block
	 * <player.name()>.contract:rvnum:N:trigger:N -> javascript block
	 * <player.name()>.contract:rvnum:N:complete -> boolean
	 */
	std::vector<std::string> list_contracts(room_vnum rvnum) {
		std::string value = "1";
		std::vector<std::string> contracts;

		for(unsigned ctr = 0; true ; ctr++) {
			std::string key  = CAT("contract:",(rvnum),":",ctr,":name");
			value = "";
			mods::globals::db->get(key,value);

			if(value.length()) {
				contracts.emplace_back(value);
			} else {
				break;
			}
		}

		return contracts;
	}

	/* TODO: load triggers from disk on startup */

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

	/**
	 * @brief fetches the current contract id that the players is pursuing
	 *
	 * @param player
	 *
	 * @return
	 */
	std::string current_contract(player_ptr_t& player) {
		std::string current_contract_id = "";
		/** TODO: this needs to be a fast structure in RAM that we check.
		 * Since we've ripped out lmdb as a fast key/value pair system,
		 * our next available option will be the speedy RAM structures.
		 */
		DBGET(current_contract_key(player),current_contract_id);
		dbg_print("current contract: " << current_contract_id);
		return current_contract_id;
	}

	bool has_contract(player_ptr_t& player) {
		return current_contract(player).length() > 0;
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
			std::cerr << red_str("ERROR: contract current key is not formatted correctly") << "\n";
			return -1;
		}
		DBGET(trigger_key(player,mods::util::stoi(parts[0]).value_or(-1),mods::util::stoi(parts[1]).value_or(-1)),js_code);
		dbg_print("js_code for trigger: " << green_str(js_code));
		mods::js::eval_string(js_code);
		return 0;	//TODO: make use of return value to signify something?
	}

	bool trigger_exists(player_ptr_t& player,int contract_id) {
		std::string value;
		DBGET(current_contract(player) + std::string(":") + std::to_string(contract_id), value);
		return value.length() > 0;
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
	/**
	 * @brief sets the current contract value in lmdb to a blank string
	 *
	 * @param player
	 * @param contract_id
	 */
	void leave_contract(player_ptr_t& player,int contract_id) {
		DBSET(current_contract_key(player),"");
	}

	bool start_contract(player_ptr_t& player,int contract_id) {
		auto key = CAT("contract:",std::to_string(player->vnum()),":",std::to_string(contract_id));
		dbg_print("key : " << key);

		std::string name = key + ":name";
		std::string desc = key + ":desc";
		std::string value= "";
		mods::globals::db->get(name,value);

		dbg_print(name << "->" << value);
		if(value.length()) {
			/* Set the current contract number to this room id and contract id */
			DBSET(current_contract_key(player),current_contract_value(player->vnum(),contract_id));
			/* set the current contract to incomplete ("0") */
			DBSET(complete_key(player,player->vnum(),contract_id),"0");
			load_contract_code(player,player->vnum(),contract_id);
			run_trigger(player);
			return true;
		} else {
			return false;
		}
	}
	ACMD(do_contract) {
		auto vec_args = PARSE_ARGS();
		DO_HELP_WITH_ZERO("contract");

		if(vec_args.size() > 0 && vec_args[0].compare("list") == 0) {
			auto contract_names = mods::contracts::list_contracts(player->vnum());
			if(contract_names.size() == 0) {
				player->sendln("{red}There are no contracts in this room.{/red}");
				return;
			}
			player->sendln("{grn}Listing available contracts in this room...{/grn}");
			for(auto qn : contract_names) {
				*player << "{grn}[ CONTRACT ]{/grn} {yel}" << qn << "{/yel}\r\n";
			}
			player->sendln("{grn}Done listing.{/grn}");

			return;
		}

		if(vec_args.size() > 1 && vec_args[0].compare("join") == 0) {
			auto contract_num = mods::util::stoi(vec_args[1]);

			if(contract_num.value_or(-1) == -1) {
				*player << "{red}Invalid contract number.{/red}\r\n";
				return;
			}
			if(mods::contracts::start_contract(player,contract_num.value())) {
				player->sendln("{grn}Your contract has been started! Good luck!{/grn}");
				return;
			}
			player->sendln("{red}There is no contract here that matches your criteria.{/red}");
			return;
		}

		if(vec_args.size() > 1 && vec_args[0].compare("leave") == 0) {
			auto contract_num = mods::util::stoi(vec_args[1]);

			if(contract_num.value_or(-1) == -1) {
				*player << "{red}Invalid contract number{/red}\r\n";
				return;
			}

			mods::contracts::leave_contract(player,contract_num.value());
			if(mods::contracts::current_contract(player).compare(vec_args[1]) != 0) {
				player->sendln("{red}You are not part of that contract.{/red}");
				return;
			}
			*player << "{red}You have left the contract.{/red}\n";
			mods::contracts::punish_for_leaving_contract(player,contract_num.value());
			return;
		}
	}
	void init() {
		mods::interpreter::add_command("contract", POS_RESTING, do_contract, 0,0);
		mods::interpreter::add_command("quest", POS_RESTING, do_contract, 0,0);
		mods::interpreter::add_command("quests", POS_RESTING, do_contract, 0,0);
		mods::interpreter::add_command("contracts", POS_RESTING, do_contract, 0,0);
	}

	using player_contract_map_t = std::map<uint64_t,std::deque<std::shared_ptr<player_contract_instance>>>;

	player_contract_map_t player_id_to_contracts_map;

	std::tuple<bool,std::string> start_contract(player_ptr_t& player, contract_vnum_t contract_vnum) {
		player_id_to_contracts_map[player->db_id()].emplace_back(std::make_shared<player_contract_instance>("start",contract_vnum));
		return {0,"stub"};
	}
	std::tuple<bool,std::string> resume_contract(player_ptr_t& player, contract_vnum_t contract_vnum) {
		player_id_to_contracts_map[player->db_id()].emplace_back(std::make_shared<player_contract_instance>("resume",contract_vnum));
		return {0,"stub"};
	}
	std::tuple<bool,std::string> stop_contract(player_ptr_t& player, contract_vnum_t contract_vnum) {
		auto& d = player_id_to_contracts_map[player->db_id()];
		std::remove_if(d.begin(),d.end(),[contract_vnum](std::shared_ptr<player_contract_instance>& ptr) -> bool {
			return ptr->contract_vnum == contract_vnum;
		});
		return {0,"stub"};
	}

	std::vector<std::shared_ptr<contract>>& contract_master_list() {
		static std::vector<std::shared_ptr<contract>> list;
		return list;
	}
};
