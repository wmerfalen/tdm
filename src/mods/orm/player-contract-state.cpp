#include "player-contract-state.hpp"
#include "contract-steps.hpp"
#include "util.hpp"
#include "../player.hpp"

namespace mods::orm {
	int16_t player_contract_state::save() {
		auto insert_result = mods::orm::util::insert_returning<player_contract_state,sql_compositor>(this, "id");
		if(!ORM_SUCCESS(insert_result)) {
			std::cerr << red_str("Issue saving player_contract_state:'") << std::get<1>(insert_result) << "'\n";
		}
		return 0;
	}

	void player_contract_state::init() {
		id = 0;
		pc_contract_vnum = 0;
		pc_player_id = 0;
		pc_state_data.clear();
		loaded = 0;
		created_at = updated_at = 0;
	}

	std::tuple<int16_t,std::string> load_player_contract_state(player_ptr_t& player,contract_vnum_t c_vnum,std::string& in_buffer) {
		using statement = std::vector<mods::orm::util::statement_t>;
		using c = std::vector<pqxx::result::reference>;
		c container;
		statement statements;
		statements.emplace_back("pc_contract_vnum","=",std::to_string(c_vnum),true,false);
		statements.emplace_back("pc_player_id","=",std::to_string(player->db_id()),false,false);
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
