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
#include "mods/yaml.hpp"


namespace mods::fs {
	void ls(std::shared_ptr<mods::player> player,std::string_view _path) {
		std::string path = _path.data();
		DIR *dir;
		struct dirent *ent;
		if ((dir = opendir (path.c_str())) != NULL) {
			/* print all the files and directories within directory */
			player->stc("{grn}--[ begin dump: '");
			player->stc(_path.data());
			player->stc("']--{/grn}\r\n");
			while ((ent = readdir (dir)) != NULL) {
				player->sendln(ent->d_name);
			}
			player->stc("{grn}--[ end dump ]--{/grn}\r\n");
			closedir (dir);
		}
	}
};


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

void fill_rifle_object(std::shared_ptr<obj_data>& obj) {
	obj_data &proto = *obj;
	for(unsigned i = 0; i < MAX_OBJ_AFFECT; i++) {
		proto.affected[i].location = 0;
		proto.affected[i].modifier = 0;
	}

	proto.item_number = 1;
	proto.name = strdup(obj->weapon()->rifle_attributes->name.c_str());
	proto.description = proto.name;
	proto.short_description = proto.name;
	proto.action_description = proto.name;
	proto.ex_description = (extra_descr_data*) calloc(1,sizeof(extra_descr_data));
	proto.ex_description->next = nullptr;
	proto.ex_description->keyword = proto.ex_description->description = nullptr;
	proto.ex_description->next = nullptr;
	proto.worn_on = WEAR_WIELD;
	proto.type = 0;
	proto.carried_by = proto.worn_by = nullptr;
	proto.next_content = nullptr;
	proto.contains = nullptr;
	proto.in_obj = nullptr;
	proto.worn_by = nullptr;
	proto.carried_by = nullptr;
	memset(&proto.obj_flags.value[0],0,sizeof(proto.obj_flags.value));
	proto.obj_flags.ammo_max = 99;
	proto.obj_flags.clip_size = 20;
	proto.obj_flags.weapon_flags =0;
	proto.obj_flags.bitvector = 0;
	proto.obj_flags.type_flag = 0;
	proto.obj_flags.wear_flags = ITEM_WEAR_TAKE | ITEM_WEAR_WIELD;
	proto.obj_flags.extra_flags = 0;
	proto.obj_flags.weight = 0;
	proto.obj_flags.cost = 0;
	proto.obj_flags.cost_per_day = 0;
	proto.obj_flags.timer = 0;
}

/* load the zones */
std::shared_ptr<obj_data> rifle_object(std::string_view file){
	auto obj = blank_object();
	obj->weapon(0);
	obj->weapon()->rifle_attributes = std::make_unique<mods::yaml::rifle_description_t>();
	obj->weapon()->rifle_attributes->feed(file);
	fill_rifle_object(obj);
	return std::move(obj);
}

/**
 * yaml_import <rifle> <file>		#imports said file in current dir
 * yaml_import ls 			#lists files in current dir
 */
/** TODO: generalize this file system ls interface for use with other stuff */
ACMD(do_yaml_import){
	MENTOC_PREAMBLE();
	auto vec_args = mods::util::arglist<std::vector<std::string>>(std::string(argument));
	if(vec_args.size() == 0 || vec_args[0].compare("ls") == 0) {
		player->pager_start();
		mods::fs::ls(player,mods::yaml::current_working_dir());
		player->pager_end();
		player->page(0);
		return;
	}

	mods::yaml::rifle_description_t rifle;
	if(vec_args.size() == 2){
		if(std::string(vec_args[0]).compare("rifle") == 0){
			player->sendln("importing...");
			player->sendln(vec_args[1]);
			auto obj = rifle_object(vec_args[1]);
			obj_to_room(obj.get(),IN_ROOM(player->cd()));
			player->sendln("done importing...");
			return;
		}
	}
	player->sendln("Unknown type/file combination. Nothing imported.");

}

ACMD(do_yaml_example){
	MENTOC_PREAMBLE();
	auto vec_args = mods::util::arglist<std::vector<std::string>>(std::string(argument));
	if(vec_args.size() == 0 || vec_args[0].compare("list") == 0) {
		for(auto type : {"rifle"}) {
			player->sendln(type);
		}
	}
	mods::yaml::rifle_description_t rifle;
	if(std::string(vec_args[0]).compare("rifle") == 0){
		player->sendln("rifle.yml");
		rifle.write_example_file("rifle.yml");
	}
	player->sendln("[+] done");
}

