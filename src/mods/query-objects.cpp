#include "query-objects.hpp"
#include "help.hpp"
#include "super-users.hpp"
#include "interpreter.hpp"
#include "screen-searcher.hpp"
#include "db.hpp"
#include "util.hpp"
#include "zone.hpp"
#include "loops.hpp"

#ifdef __MENTOC_MODS_INTEGRAL_OBJECTS_DEBUG__
#define mc_debug(A) std::cerr << "[mods::query_objects][debug]:" << A <<"\n";
#else
#define mc_debug(A)
#endif

/* Generic Find, designed to find any object/character
 *
 * Calling:
 *  *arg     is the pointer containing the string to be searched for.
 *           This string doesn't have to be a single word, the routine
 *           extracts the next word itself.
 *  bitv..   All those bits that you want to "search through".
 *           Bit found will be result of the function
 *  *ch      This is the person that is trying to "find"
 *  **tar_ch Will be NULL if no character was found, otherwise points
 * **tar_obj Will be NULL if no object was found, otherwise points
 *
 * The routine used to return a pointer to the next word in *arg (just
 * like the one_argument routine), but now it returns an integer that
 * describes what it filled in.
 */


extern int generic_find(char *arg, bitvector_t bitvector, char_data *ch, char_data **tar_ch, struct obj_data **tar_obj);

namespace mods::query_objects {
	static constexpr int FIND_CHAR_ROOM    = (1 << 0);
	static constexpr int FIND_CHAR_WORLD   = (1 << 1);
	static constexpr int FIND_OBJ_INV      = (1 << 2);
	static constexpr int FIND_OBJ_ROOM     = (1 << 3);
	static constexpr int FIND_OBJ_WORLD    = (1 << 4);
	static constexpr int FIND_OBJ_EQUIP    = (1 << 5);

	obj_ptr_t query_room_for_object(const room_rnum& room, std::string_view name) {
		for(auto obj = world[room].contents; obj ; obj = obj->next_content) {
			if(isname(name.data(), obj->name)) {
				return optr_by_uuid(obj->uuid);
			}
		}
		return nullptr;
	}
	std::vector<uuid_t> query_room_for_object_by_yaml(const room_rnum& room, std::string_view name) {
		std::vector<uuid_t> objects;
		for(auto obj = world[room].contents; obj ; obj = obj->next_content) {
			if(name.compare(obj->feed_file().data()) == 0) {
				objects.emplace_back(obj->uuid);
			}
		}
		return objects;
	}
	std::vector<uuid_t> query_room_for_object_by_yaml_multi(const room_rnum& room, const std::vector<std::string>& files) {
		std::vector<uuid_t> objects;
		for(auto obj = world[room].contents; obj ; obj = obj->next_content) {
			for(const auto& file : files) {
				mc_debug("[file]:'" << file << "', object->'" << obj->feed_file().data() << "'");
				if(file.compare(obj->feed_file().data()) == 0) {
					objects.emplace_back(obj->uuid);
				}
			}
		}
		return objects;
	}

	std::vector<uuid_t> query_contents_by_yaml(obj_ptr_t& found_object,std::string_view file) {
		std::vector<uuid_t> objects;
		for(auto obj = found_object->contains; obj != nullptr; obj = obj->next_content) {
			if(obj->feed_file().compare(file.data()) == 0) {
				objects.emplace_back(obj->uuid);
			}
		}
		return objects;
	}
	std::vector<uuid_t> query_contents_by_yaml_multi(obj_ptr_t& found_object,const std::vector<std::string>& files) {
		std::vector<uuid_t> objects;
		for(auto obj = found_object->contains; obj != nullptr; obj = obj->next_content) {
			for(const auto& yaml : files) {
				if(obj->feed_file().compare(yaml.data()) == 0) {
					objects.emplace_back(obj->uuid);
				}
			}
		}
		return objects;
	}
	std::vector<uuid_t> query_contents_by_yaml_multi(room_rnum room,std::string_view container,const std::vector<std::string>& files) {
		std::vector<uuid_t> objects;
		auto found_object = query_room_for_object(room,container);
		if(!found_object) {
			return objects;
		}
		for(auto obj = found_object->contains; obj != nullptr; obj = obj->next_content) {
			for(const auto& yaml : files) {
				if(obj->feed_file().compare(yaml.data()) == 0) {
					objects.emplace_back(obj->uuid);
				}
			}
		}
		return objects;
	}

	std::vector<uuid_t> query_inventory_by_yaml(player_ptr_t& player, std::string_view yaml_file) {
		std::vector<uuid_t> objects;
		for(const auto& obj : player->vcarrying()) {
			if(obj->feed_file().compare(yaml_file.data()) == 0) {
				objects.emplace_back(obj->uuid);
			}
		}
		return objects;
	}
	std::vector<uuid_t> query_inventory_for_object(player_ptr_t& player, std::string_view name) {
		std::vector<uuid_t> objects;
		for(const auto& obj : player->vcarrying()) {
			if(isname(name.data(),obj->name)) {
				objects.emplace_back(obj->uuid);
			}
		}
		return objects;
	}


	std::vector<uuid_t> query_inventory_container_by_yaml(player_ptr_t& player, std::string_view container, std::string_view yaml_file) {
		std::vector<uuid_t> objects;
		obj_data* tar_obj;
		int result = generic_find((char*)container.data(),FIND_OBJ_INV,player->cd(),nullptr,&tar_obj);
		if(result == 0) {
			return objects;
		}
		auto found_object = optr(tar_obj);
		return query_contents_by_yaml(found_object,yaml_file);
	}

	std::vector<uuid_t> query_equipment_by_yaml(player_ptr_t& player, std::string_view name) {
		std::vector<uuid_t> objects;
		for(const auto& obj : player->equipment()) {
			if(isname(name.data(),obj->name)) {
				objects.emplace_back(obj->uuid);
			}
		}
		return objects;
	}

	bool room_has_vehicle(const room_rnum& room_id) {
		bool has_vehicle = false;
		mods::loops::foreach_object_in_room(room_id, [&](auto& obj) -> bool {
			if(obj->has_vehicle()) {
				has_vehicle = true;
				return false;
			}
			return true;
		});
		return has_vehicle;
	}
};

SUPERCMD(do_query_container) {
	ADMIN_REJECT();
	DO_HELP("query_container");
	/** code here */
	auto vec_args = PARSE_ARGS();
	static constexpr const char * usage = "usage: query_container <item> <yaml-file> ... <yaml-file-N>";
	if(vec_args.size() < 2) {
		player->errorln(usage);
		return;
	}
	auto found_object = mods::query_objects::query_room_for_object(player->room(),vec_args[0]);
	if(!found_object) {
		player->errorln(CAT("Couldn't find anything in the room that matches the search string of '",vec_args[0],"'"));
		return;
	}
	auto uuid_list = mods::query_objects::query_contents_by_yaml_multi(found_object,vec_args);
	for(auto& uuid : uuid_list) {
		player->sendln(optr_by_uuid(uuid)->name);
	}
	ADMIN_DONE();
}
SUPERCMD(do_query_room) {
	ADMIN_REJECT();
	DO_HELP("query_room");
	/** code here */
	auto vec_args = PARSE_ARGS();
	static constexpr const char * usage = "usage: query_room <yaml-file> ... <yaml-file-N>";
	if(vec_args.size() < 1) {
		player->errorln(usage);
		return;
	}
	auto uuid_list = mods::query_objects::query_room_for_object_by_yaml_multi(player->room(),vec_args);
	for(auto& uuid : uuid_list) {
		player->sendln(optr_by_uuid(uuid)->name);
	}
	ADMIN_DONE();
}
SUPERCMD(do_query_inventory) {
	ADMIN_REJECT();
	DO_HELP("query_inventory");
	/** code here */
	auto vec_args = PARSE_ARGS();
	std::vector<uuid_t> uuid_list;
	static constexpr const char * usage = "usage: query_inventory <yaml-file> ... <yaml-file-N>";
	if(vec_args.size() < 1) {
		player->errorln(usage);
		return;
	}
	for(auto& obj : player->vcarrying()) {
		for(std::size_t i = 0; i < vec_args.size(); ++i) {
			if(obj->feed_file().compare(vec_args[i].c_str()) == 0) {
				uuid_list.emplace_back(obj->uuid);
			}
		}
	}
	for(auto& uuid : uuid_list) {
		player->sendln(optr_by_uuid(uuid)->name);
	}
	ADMIN_DONE();
}
SUPERCMD(do_query_inventory_container) {
	ADMIN_REJECT();
	DO_HELP("query_inventory_container");
	/** code here */
	auto vec_args = PARSE_ARGS();
	static constexpr const char * usage = "usage: query_inventory <item> <yaml-file>";
	if(vec_args.size() < 2) {
		player->errorln(usage);
		return;
	}
	obj_data* tar_obj;
	int result = generic_find((char*)vec_args[0].c_str(),mods::query_objects::FIND_OBJ_INV,player->cd(),nullptr,&tar_obj);
	if(result == 0) {
		player->errorln(CAT("Couldn't find anything in the room that matches the search string of '",vec_args[0],"'"));
		return;
	}
	auto uuid_list = mods::query_objects::query_inventory_container_by_yaml(player,vec_args[0],vec_args[1]);
	for(auto& uuid : uuid_list) {
		player->sendln(optr_by_uuid(uuid)->name);
	}
	ADMIN_DONE();
}
SUPERCMD(do_query_zone) {
	ADMIN_REJECT();
	DO_HELP("query_zone");
	/** code here */
	//auto vec_args = PARSE_ARGS();
	//auto uuid_list = mods::query_objects::query_zone(player,vec_args);
	//for(auto & uuid : uuid_list){
	//	player->sendln(optr_by_uuid(uuid)->name);
	//}
	ADMIN_DONE();
}

namespace mods::query_objects {
	void init() {
		mods::interpreter::add_command("query_container", POS_RESTING, do_query_container, LVL_BUILDER,0);
		mods::interpreter::add_command("query_room", POS_RESTING, do_query_room, LVL_BUILDER,0);
		mods::interpreter::add_command("query_inventory", POS_RESTING, do_query_inventory, LVL_BUILDER,0);
		mods::interpreter::add_command("query_inventory_container", POS_RESTING, do_query_inventory_container, LVL_BUILDER,0);
		mods::interpreter::add_command("query_zone", POS_RESTING, do_query_zone, LVL_BUILDER,0);
	}
};
#undef mc_debug
