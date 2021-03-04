#include "player-contract-state.hpp"
#include "contract-steps.hpp"
#include "util.hpp"
#include "../player.hpp"

namespace mods::orm {
	std::tuple<int16_t,std::string> player_contract_state::delete_by_id(const uint64_t& row_id) {
		return mods::orm::util::delete_where<contract_steps,sql_compositor>(
		           this->table_name(),
		           this->primary_key_name(),
		           "=",
		           std::to_string(row_id)
		       );
	}

	std::tuple<int16_t,std::string> player_contract_state::delete_by_player_id_contract_vnum(const uint64_t& player_id,const contract_vnum_t& c_vnum) {
		using statement = std::vector<mods::orm::util::statement_t>;
		using c = std::vector<pqxx::result::reference>;
		c container;
		statement statements;
		statements.emplace_back("pc_contract_vnum","=",std::to_string(c_vnum),true,false);
		statements.emplace_back("pc_player_id","=",std::to_string(player_id),false,false);
		mods::orm::util::load_where<c,sql_compositor,statement>(
		    container,
		    player_contract_state_table_name.data(),
		    statements
		);
		if(container.size() == 0) {
			return {1,"no results"};
		}
		return this->delete_by_id(container[0]["id"].as<uint64_t>());
	}
	int16_t player_contract_state::save() {
		auto insert_result = mods::orm::util::insert_returning<player_contract_state,sql_compositor>(this, "id");
		if(!ORM_SUCCESS(insert_result)) {
			std::cerr << red_str("Issue saving player_contract_state:'") << std::get<1>(insert_result) << "'\n";
		}
		return 0;
	}
	player_contract_state::player_contract_state() {
		this->init();
	}
	strmap_t player_contract_state::export_class() {
		strmap_t v;
		v["pc_contract_vnum"] = std::to_string(pc_contract_vnum);
		v["pc_player_id"] = std::to_string(pc_player_id);
		v["pc_state_data"] = pc_state_data;
		return std::move(v);
	}

	void player_contract_state::init() {
		id = 0;
		pc_contract_vnum = 0;
		pc_player_id = 0;
		pc_state_data.clear();
		loaded = 0;
		created_at = updated_at = 0;
	}
	player_contract_state::player_contract_state(const pqxx::row& row) {
		init();
		id = row["id"].as<uint64_t>();
		pc_contract_vnum = row["pc_contract_vnum"].as<contract_vnum_t>();
		pc_state_data = row["pc_state_data"].c_str();
		pc_player_id = row["pc_player_id"].as<uint64_t>();
		created_at = row["created_at"].as<long>();
		updated_at = row["updated_at"].as<long>();
		loaded = true;
	}

	std::tuple<int16_t,std::string> player_contract_state::update_player_data(const uint64_t& player_id,const contract_vnum_t& c_vnum,const std::string& buffer) {
		this->delete_by_player_id_contract_vnum(player_id,c_vnum);
		init();
		pc_contract_vnum = c_vnum;
		pc_state_data = buffer;
		pc_player_id = player_id;
		return {save(),"updated"};
	}
	std::tuple<int16_t,std::string> load_player_contract_state(player_ptr_t& player,contract_vnum_t c_vnum,std::string& in_buffer) {
		return load_player_contract_state(player->db_id(),c_vnum,in_buffer);
	}
	/**
	 * @brief
	 *
	 * @param player_id
	 * @param c_vnum
	 * @param in_buffer
	 *
	 * @return {1,"success"} if loaded. {0,"no results"} if not
	 */
	std::tuple<int16_t,std::string> load_player_contract_state(uint64_t player_id,contract_vnum_t c_vnum,std::string& in_buffer) {
		using statement = std::vector<mods::orm::util::statement_t>;
		using c = std::vector<pqxx::result::reference>;
		c container;
		statement statements;
		statements.emplace_back("pc_contract_vnum","=",std::to_string(c_vnum),true,false);
		statements.emplace_back("pc_player_id","=",std::to_string(player_id),false,false);
		mods::orm::util::load_where<c,sql_compositor,statement>(
		    container,
		    player_contract_state_table_name.data(),
		    statements
		);
		if(container.size() == 0) {
			return {0,"no results"};
		}
		in_buffer.assign(container[0]["pc_state_data"].c_str());
		return {container.size(),"success"};
	}
};
