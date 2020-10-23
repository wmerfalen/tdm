#include "integral-objects.hpp"
#include "help.hpp"
#include "super-users.hpp"
#include "interpreter.hpp"
#include "orm/integral-object.hpp"
#include "db.hpp"

extern std::string sanitize_key(std::string key);
extern void obj_to_obj(obj_ptr_t from_object, obj_ptr_t to_object);
namespace mods::interpreter {
	extern void add_command(std::string command_string, byte position, mods::interpreter::acmd_function func, sh_int minimum_level,int subcmd);
};
namespace mods::integral_objects {
	int write_db_values(std::string_view section_name, std::string_view prefix, const std::vector<std::string>& values){
		using namespace mods::db;
		std::string value = "";
		return put_section_vector(section_name.data(),prefix.data(), values);
	}
	/*
CREATE TABLE integral_object (
	object_id SERIAL,
	object_room_vnum INTEGER NOT NULL,
	object_type VARCHAR(16) NOT NULL,
	object_vnum INTEGER NOT NULL,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE camera_feed (
	feed_id SERIAL,
	feed_type VARCHAR(16) NOT NULL,
	feed_vnum INTEGER NOT NULL,
	feed_room_vnum INTEGER NOT NULL,
	feed_order INTEGER NOT NULL DEFAULT 0,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);
*/
	void install_weapon_locker(player_ptr_t& player, std::vector<std::string>& args){
		using namespace mods::db;
		std::string value = "";
		auto vnum = std::to_string(world[player->room()].number);
		std::string section_name = "weapon-locker", prefix = vnum;
		auto status = put_section_vector(section_name,prefix, args);
		player->send("status: %d\r\n",status);
		std::vector<std::string> values;
		status = get_section_vector(section_name,prefix,values);
		player->send("get status: %d\r\nTo confirm, we placed these values...\r\n",status);
		for(auto line : values){
			player->send("[item]: '%s'\r\n",line.c_str());
		}
		player->sendln("Done listing.");
	}
	void install_armor_locker(player_ptr_t& player, std::vector<std::string>& args){
		using namespace mods::db;
		std::vector<std::string> values;
		std::string value = "";
		auto vnum = std::to_string(world[player->room()].number);
		std::string section_name = "armor-locker", prefix = vnum;
		auto status = put_section_vector(section_name,prefix, args);
		player->send("status: %d\n",status);
		status = get_section_vector(section_name,prefix,values);
		player->send("get status: %d\r\nTo confirm, we placed these values...\r\n",status);
		for(auto line : values){
			player->send("[item]: '%s'\r\n",line.c_str());
		}
		player->sendln("Done listing.");
	}
	int get_yaml_type(std::string& yaml){
		auto exploded = EXPLODE(yaml.c_str(),'/');
		return ITEM_RIFLE;
	}
	obj_ptr_t first_or_create(room_rnum room,std::string query){
		//
		return create_object(ITEM_RIFLE,"g36c.yml");
	}
	void install_camera_feed(player_ptr_t& player, std::vector<std::string>& args){
		write_db_values("camera-feed",std::to_string(world[player->room()].number), args);
	}
	void feed_weapon_locker(room_vnum room){
		return;
		std::vector<std::string> values;
		mods::db::get_section_vector("weapon-locker", std::to_string(room), values);
		auto locker = first_or_create(real_room(room),"weapon-locker");
		return;
		for(auto yaml : values){
			for(int i=0; i < 10;i++){
				auto obj = create_object(get_yaml_type(yaml),yaml);
				obj_to_obj(obj,locker);
			}
		}
	}
	void feed_armor_locker(room_vnum room){
		return;
		std::vector<std::string> values;
		mods::db::get_section_vector("armor-locker", std::to_string(room), values);
		auto locker = first_or_create(real_room(room),"weapon-locker");
		for(auto yaml : values){
			auto obj = create_object(get_yaml_type(yaml),yaml);
			for(int i=0; i < 10;i++){
				obj_to_obj(obj,locker);
			}
		}
	}
	void feed_weapon_locker(std::vector<std::string>& values){

	}
	void feed_armor_locker(std::vector<std::string>& values){

	}
	void feed_camera_feed(room_vnum room_id,std::vector<std::string>& values){

	}


	void uninstall_camera_feed(player_ptr_t& player, std::vector<std::string>& args){
		using namespace mods::db;
		auto status = delete_section_vector("camera-feed",std::to_string(world[player->room()].number));
		player->send("delete status: %d\r\n",status);
	}
	void uninstall_weapon_locker(player_ptr_t& player, std::vector<std::string>& args){
		using namespace mods::db;
		auto status = delete_section_vector("weapon-locker",std::to_string(world[player->room()].number));
		player->send("delete status: %d\r\n",status);
	}
	void uninstall_armor_locker(player_ptr_t& player, std::vector<std::string>& args){
		using namespace mods::db;
		auto status = delete_section_vector("armor-locker",std::to_string(world[player->room()].number));
		player->send("delete status: %d\r\n",status);
	}
};

ACMD(do_install_camera_feed){
	DO_HELP("install_camera_feed");
	ADMIN_REJECT();
	/** code here */
	auto vec_args = PARSE_ARGS();
	mods::integral_objects::install_camera_feed(player,vec_args);
	ADMIN_DONE();
}

ACMD(do_uninstall_camera_feed){
	DO_HELP("uninstall_camera_feed");
	ADMIN_REJECT();
	/** code here */
	auto vec_args = PARSE_ARGS();
	mods::integral_objects::uninstall_camera_feed(player,vec_args);
	ADMIN_DONE();
}

ACMD(do_install_computer_choice){
	DO_HELP("install_computer_choice");
	ADMIN_REJECT();
	/** code here */
	ADMIN_DONE();
}

ACMD(do_install_armor_locker){
	DO_HELP("install_armor_locker");
	ADMIN_REJECT();
	/** code here */
	auto vec_args = PARSE_ARGS();
	mods::integral_objects::install_armor_locker(player,vec_args);
	ADMIN_DONE();
}

ACMD(do_install_weapon_locker){
	DO_HELP("install_weapon_locker");
	ADMIN_REJECT();
	/** code here */
	auto vec_args = PARSE_ARGS();
	mods::integral_objects::install_weapon_locker(player,vec_args);
	ADMIN_DONE();
}
ACMD(do_uninstall_armor_locker){
	DO_HELP("uninstall_armor_locker");
	ADMIN_REJECT();
	/** code here */
	auto vec_args = PARSE_ARGS();
	mods::integral_objects::uninstall_armor_locker(player,vec_args);
	ADMIN_DONE();
}

ACMD(do_uninstall_weapon_locker){
	DO_HELP("uninstall_weapon_locker");
	ADMIN_REJECT();
	/** code here */
	auto vec_args = PARSE_ARGS();
	mods::integral_objects::uninstall_weapon_locker(player,vec_args);
	ADMIN_DONE();
}


namespace mods::integral_objects {
	void init(){
		{
			auto func = do_install_armor_locker;
			byte position = POS_RESTING;
			std::string command_string = "install_armor_locker";
			mods::interpreter::add_command(command_string, position, func, 0,0);
		}
		{
			auto func = do_install_weapon_locker;
			byte position = POS_RESTING;
			std::string command_string = "install_weapon_locker";
			mods::interpreter::add_command(command_string, position, func, 0,0);
		}
	}
};
