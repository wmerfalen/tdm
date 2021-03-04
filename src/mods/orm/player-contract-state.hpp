#ifndef __MENTOC_MODS_ORM_player_contract_state_HEADER__
#define __MENTOC_MODS_ORM_player_contract_state_HEADER__

#include "orm-base.hpp"
#include "../contract-types.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	static constexpr std::string_view player_contract_state_table_name = "player_contract_state";
	using contract_vnum_t = uint32_t;

	struct player_contract_state : public mods::orm::orm_base<player_contract_state,std::string> {
		using primary_choice_t = std::string;
		std::string table_name() const {
			return player_contract_state_table_name.data();
		}
		std::string column_prefix() const {
			return "pc_";
		}
		std::string id_column() const {
			return "id";
		}

		std::string primary_key_name() {
			return id_column();
		}
		std::string primary_key_value() {
			return std::to_string(this->id);
		}
		std::string primary_type() {
			return "";
		}
		int16_t feed(const pqxx::result::reference&) {
			return 0;
		}
		player_contract_state();
		player_contract_state(const pqxx::row& row);
		~player_contract_state() = default;

		std::tuple<int16_t,std::string> delete_by_player_id_contract_vnum(const uint64_t& player_id,const contract_vnum_t& c_vnum);
		std::tuple<int16_t,std::string> delete_by_id(const uint64_t& row_id);
		std::tuple<int16_t,std::string> update_player_data(const uint64_t& player_id,const contract_vnum_t& c_vnum,const std::string& buffer);

		void init();
		void feed_multi(pqxx::result&);

		strmap_t export_class();
		int16_t save();

		bool loaded;

		uint64_t id;
		contract_vnum_t pc_contract_vnum;
		std::string pc_state_data;
		uint64_t pc_player_id;
		long created_at;
		long updated_at;
	};
	/**
	 * @brief wrapper to the player_id version
	 *
	 * @param player_ptr_t&
	 * @param c_vnum
	 * @param in_buffer
	 *
	 * @return {1,"success"} if loaded. {0,"no results"} if not
	 */
	std::tuple<int16_t,std::string> load_player_contract_state(player_ptr_t& player,contract_vnum_t c_vnum,std::string& in_buffer);
	/**
	 * @brief player_id version. loads player state buffer
	 *
	 * @param player_id
	 * @param c_vnum
	 * @param in_buffer
	 *
	 * @return {1,"success"} if loaded. {0,"no results"} if not
	 */
	std::tuple<int16_t,std::string> load_player_contract_state(uint64_t player_id,contract_vnum_t c_vnum,std::string& in_buffer);
};

#endif
