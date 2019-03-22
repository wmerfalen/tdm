#include "flags.hpp"
#include "sql.hpp"
#include "pq.hpp"
namespace mods::flags {
	bool save_player_flags(std::shared_ptr<mods::player> player){
		/** TODO: I could really use an update or insert command for sql */
		try {
			auto t = txn();
			mods::pq::exec(t,std::string("DELETE FROM player_flags where player_id=") 
					+ t.quote(player->get_db_id()));
			mods::pq::commit(t);
		}catch(const std::exception &e){
			std::cerr << "[save_player_flags]::FAILED! Exception: '" << e.what() << "'\n";
			return false;
		}
		for(int i =mods::flags::chunk_type_t::FIRST ;
				i <= mods::flags::chunk_type_t::LAST; ++i){
			mutable_map_t m; 
			m["chunk_index"] = std::to_string(i);
			m["player_id"] = std::to_string(player->get_db_id());
			m["flag_value"] = std::to_string(player->get_chunk((chunk_type_t)i));
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
				return 0;
			}
		}
		return true;

	}
};
