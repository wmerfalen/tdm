#ifndef __MENTOC_MODS_ORM_loot_ammo_HEADER__
#define __MENTOC_MODS_ORM_loot_ammo_HEADER__

#include "orm-base.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	static constexpr std::string_view raid_table_name = "raid";

#define LOOT_AMMO_ORM_MEMBERS ( \
(uint64_t,id,0,1,null,1), \
(std::string,la_types,"",3,null,1), \
(std::string,la_levels,"",3,null,1), \
(room_vnum,la_room,0,1,null,1), \
(uint16_t,la_count,0,1,null,1))

#define LOOT_AMMO_INITIALIZE_ROW_MEMBERS \
(la_room) \
(la_types) \
(la_levels) \
(la_count) \
 


	struct loot_ammo : public mods::orm::orm_base<loot_ammo,std::string> {
		MENTOC_ORM_CLASS(LOOT_AMMO_ORM_MEMBERS,"loot_ammo");
		auto vnum() {
			return id;
		}
	
		loot_ammo() {
			this->init();
			loaded = 0;
			id = 0;
		}
		~loot_ammo() = default;
	
		MENTOC_ORM_PRIMARY_KEY_FUNCTIONS();
		MENTOC_ORM_FEED_MULTI(LOOT_AMMO_ORM_MEMBERS,loot_ammo);
	
		std::deque<loot_ammo> rows;
		auto destroy() {
			return  remove(this);
		}
		auto save() {
			return std::get<0>(this->update<loot_ammo>(this));
		}
	#ifdef LOOT_AMMO_INITIALIZE_ROW_MEMBERS
		MENTOC_ORM_INITIALIZE_ROW_USING(LOOT_AMMO_INITIALIZE_ROW_MEMBERS);
	#endif
	};
	struct loot_ammo_status_t {
		std::tuple<int16_t,std::string> orm_status;
		std::unique_ptr<loot_ammo> record;
		loot_ammo_status_t() = default;
		loot_ammo_status_t(loot_ammo_status_t&& other) : orm_status(other.orm_status),
			record(std::move(other.record)) {}
	};
	
	using loot_ammo_list_t = std::deque<std::shared_ptr<loot_ammo>>;
	
	loot_ammo_list_t& loot_ammo_list();
	
	loot_ammo_list_t load_all_loot_ammo_list();
};

#endif
