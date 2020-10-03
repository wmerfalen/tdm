#ifndef __MENTOC_MODS_ORM_MINI_GAME_HEADER__
#define __MENTOC_MODS_ORM_MINI_GAME_HEADER__

#include "orm-base.hpp"
#include "../mini-games.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;

	/** 
	 * it should be noted that this class is the first orm class to allow
	 * multiple values for a single variable (game_room_vnum_list).
	 * This is the proposed model that should be utilized for normalized
	 * data. I don't know of a better way to perform this (given what's
	 * already been coded). 
	 *
	 * will - 2020-09-28
	 */
	struct mini_game : public mods::orm::orm_base<mini_game,std::string> {
		using id_type_t = uint64_t;
		using room_vnum_t = uint32_t;
		using game_vnum_t = uint32_t;
		std::string table_name() const { return "mini_game"; }
		std::string column_prefix() const { return "game_"; }
		std::string id_column() const { return "game_id"; }
		mini_game() : game_id(0) {}
		~mini_game() = default;

		std::string primary_key_name() { return id_column(); }
		std::string primary_key_value(){ return std::to_string(this->id); }

		template <typename TResultSet>
		int16_t feed_multi(TResultSet rows){
			result_set = rows;
			for(auto row : rows){
				mods::mini_games::mini_game_payload_t g;
				g.primary_key = mods::util::stoi(row[this->primary_key_name().c_str()].c_str()).value_or(0);
				g.type = row["game_type"].c_str();
				g.vnum = mods::util::stoi(row["game_vnum"].c_str()).value_or(0);
				g.name = row["game_name"].c_str();
				g.action = row["game_action"].c_str();
				g.game_room_vnum = mods::util::stoi(row["game_room_vnum"].c_str()).value_or(0);
				g.order = mods::util::stoi(row["game_order"].c_str()).value_or(0);
				mods::mini_games::load(g);
			}
			return 0;
		}
		void load_by_room_vnum(room_vnum);
		void load_all();
		auto create_game(mods::mini_games::mini_game_payload_t game){
			return this->create<mini_game>(this);
		}
		auto update_game(uint64_t pk_id,mods::mini_games::mini_game_payload_t game){
			return this->update<mini_game>(this);
		}
		strmap_t export_class();

		id_type_t game_id;
		std::string game_type;
		game_vnum_t game_vnum;
		std::string game_name;
		std::string game_action;
		room_vnum_t game_room_vnum;
		int game_order;
		long created_at;
		long updated_at;
		pqxx::result result_set;
		std::vector<room_vnum_t> game_room_vnum_list;
		bool loaded;
	};

};

#endif
