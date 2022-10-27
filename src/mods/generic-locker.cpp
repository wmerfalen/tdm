#include "integral-objects.hpp"
#include "integral-objects-db.hpp"
#include "generic-locker.hpp"
#include "orm/locker.hpp"
#include "object-utils.hpp"
#include "query-objects.hpp"

//#define __MENTOC_MODS_GENERIC_LOCKER_DEBUG__
#ifdef  __MENTOC_MODS_GENERIC_LOCKER_DEBUG__
	#define mw_debug(A) std::cerr << red_str("[mods::generic_locker_t]:") << A <<"\n";
#else
	#define mw_debug(A)
#endif
extern void obj_to_obj(obj_ptr_t from_object, obj_ptr_t to_object);
namespace mods {
	static std::set<room_vnum> rooms_with_lockers;

	bool generic_locker_t::replenish_if(std::string_view in_type) {
		mw_debug("checking for Replenishing " << type << " in: " << room);
		if(mods::orm::locker::to_type_integral(in_type) == type) {
			mw_debug("Replenishing " << type << " in: " << room);
			replenish();
			return true;
		}
		return false;
	}
	generic_locker_t::generic_locker_t() {
		container = nullptr;
		room = NOWHERE;
		room_id = NOWHERE;
		type = 0;
		good = false;
		items = 0;
	}
	generic_locker_t::generic_locker_t(const generic_locker_t& copy) {
		orm = copy.orm;
		container = copy.container;
		room = copy.room;
		room_id = copy.room_id;
		type = copy.type;
		db_errors = copy.db_errors;
		good = copy.good;
		items = copy.items;
	}
	void generic_locker_t::init() {
	}
	generic_locker_t::generic_locker_t(std::string_view in_type,const room_vnum& in_room) {
		container = nullptr;
		room = in_room;
		room_id = real_room(in_room);
		type = mods::orm::locker::to_type_integral(in_type);
		good = false;
		items = 0;
		load(in_type,room);
	}
	generic_locker_t::~generic_locker_t() {
		orm.clear();
		if(container) {
			mods::globals::dispose_object(container->uuid);
			container = nullptr;
		}
	}
	void generic_locker_t::load(std::string_view in_type,const room_vnum& in_room) {
		mw_debug("load of type " << in_type << " in " << in_room);
		using type_t = mods::orm::locker::type_t;
		orm.clear();
		good = false;
		type = mods::orm::locker::to_type_integral(in_type);
		room = in_room;
		room_id = real_room(in_room);
		items = mods::orm::locker::get_lockers_by_type(in_type,room,&orm);
		good = true;
		rooms_with_lockers.insert(in_room);
		switch(static_cast<type_t>(type)) {
			case type_t::UNKNOWN:
			default:
				log("SYSERR: trying to load locker full of items that are of unknown type. Ignoring...");
				good = false;
				break;
			case type_t::WEAPON:
				query = "weapon-locker";
				container_yaml = "weapon-locker.yml";
				break;
			case type_t::ARMOR:
				query = "armor-locker";
				container_yaml = "armor-locker.yml";
				break;
			case type_t::AMMO:
				query = "ammo-locker";
				container_yaml = "ammo-locker.yml";
				break;
		}
		container = mods::integral_objects_db::first_or_create(in_room,query, ITEM_CONTAINER, container_yaml);
	}
	void generic_locker_t::replenish() {
		room_id = real_room(room);
		if(!container) {
			log("creating another container");
			container = mods::integral_objects_db::first_or_create(room,query, ITEM_CONTAINER, container_yaml);
		}
		for(const auto& row : orm) {
			const std::string_view yaml = row.l_yaml;
			if(!mods::object_utils::assert_sane_object(yaml)) {
				log("Not a sane yaml file (generic_feed): '%s'",yaml.data());
				continue;
			}
			auto yaml_file = mods::object_utils::get_yaml_file(yaml);
			auto uuid_list = mods::query_objects::query_contents_by_yaml(container,yaml_file);
			if(uuid_list.size() < row.l_count) {
				mw_debug("Filling with: " << yaml);
				auto obj = create_object(mods::object_utils::get_yaml_type(yaml),yaml_file);
				obj_to_obj(obj,container);
			}
		}
	}


};

