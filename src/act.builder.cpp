#include "conf.h"
#include "sysdep.h"

#include "structs.h"
#include "utils.h"
#include "comm.h"
#include "interpreter.h"
#include "handler.h"
#include "db.h"
#include "screen.h"
#include "globals.hpp"
#include "mods/pq.hpp"
#include "mods/db.hpp"
#include "mods/sql.hpp"
#include "mods/lmdb.hpp"

using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
ACMD(do_next_object_number){
	MENTOC_PREAMBLE();
		try{
			auto select_transaction = txn();
			sql_compositor comp("object",&select_transaction);
			auto object_sql = comp.select("currval(obj_item_number) + 1 as obj_item_number")
				.from("object")
				.sql();
			auto obj_record = mods::pq::exec(select_transaction,object_sql);
			if(obj_record.size() == 0){
				player->stc("1");
			}else{
				player->stc(obj_record[0]["obj_item_number"].c_str());
			}
		}catch(std::exception& e){
			std::cerr << __FILE__ << ": " << __LINE__ << ": error loading character by pkid: '" << e.what() << "'\n";
		}
	}

ACMD(do_next_room_number){
	MENTOC_PREAMBLE();
		try{
			auto select_transaction = txn();
			sql_compositor comp("room",&select_transaction);
			auto room_sql = comp.select("currval(room_number) + 1 as room_number")
				.from("room")
				.sql();
			auto room_record = mods::pq::exec(select_transaction,room_sql);
			if(room_record.size() == 0){
				player->stc("1");
			}else{
				player->stc(room_record[0]["room_number"].c_str());
			}
		}catch(std::exception& e){
			std::cerr << __FILE__ << ": " << __LINE__ << ": error loading character by pkid: '" << e.what() << "'\n";
		}
}
ACMD(do_next_mob_number){
	MENTOC_PREAMBLE();
		try{
			auto select_transaction = txn();
			sql_compositor comp("mobile",&select_transaction);
			auto mob_sql = comp.select("currval(mob_virtual_number) + 1 as mob_number")
				.from("mobile")
				.sql();
			auto mob_record = mods::pq::exec(select_transaction,mob_sql);
			if(mob_record.size() == 0){
				player->stc("1");
			}else{
				player->stc(mob_record[0]["mob_number"].c_str());
			}
		}catch(std::exception& e){
			std::cerr << __FILE__ << ": " << __LINE__ << ": error loading character by pkid: '" << e.what() << "'\n";
		}

}
