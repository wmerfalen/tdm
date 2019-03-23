#include "flags.hpp"
#include "sql.hpp"
#include "pq.hpp"
namespace mods::flags {
	bool save_player_flags_by_chunk(std::shared_ptr<mods::player> player,
			const std::vector<chunk_type_t>& chunks){
		/** TODO: I could really use an update or insert command for sql */
		for(auto chunk_index : chunks){
			try {
				auto t = txn();
				mods::pq::exec(t,std::string("DELETE FROM player_flags where player_id=") 
						+ t.quote(player->get_db_id()) + 
						std::string(" AND chunk_index=") + 
						t.quote(static_cast<int>(chunk_index))
				);
				mods::pq::commit(t);
			}catch(const std::exception &e){
				std::cerr << "[save_player_flags]::FAILED! Exception: '" << e.what() << "'\n";
			}
			mutable_map_t m; 
			m["chunk_index"] = std::to_string(chunk_index);
			m["player_id"] = std::to_string(player->get_db_id());
			m["flag_value"] = std::to_string(player->get_chunk(chunk_index));
			try{
				auto up_txn = txn();
				mods::sql::compositor comp("player_flags",&up_txn);
				auto up_sql = comp
					.insert()
					.into("player_flags")
					.values(m)
					.sql();
				std::cerr << "[debug]: sql: " << up_sql << "\n";
				mods::pq::exec(up_txn,up_sql);
				mods::pq::commit(up_txn);
			}catch(std::exception& e){
				std::cerr << __FILE__ << ": " << __LINE__ << ": error: '" << e.what() << "'\n";
			}
		}
		return true;
	}
	bool save_player_flags(std::shared_ptr<mods::player> player){
		std::vector<chunk_type_t> chunks;
		for(auto chunk = chunk_type_t::FIRST; chunk != chunk_type_t::LAST;){
			chunks.push_back(chunk);
			int64_t i_chunk = static_cast<int64_t>(chunk);
			chunk = static_cast<chunk_type_t>(++i_chunk);
		}
		return save_player_flags_by_chunk(player,chunks);
	}
};
