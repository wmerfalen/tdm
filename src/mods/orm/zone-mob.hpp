#ifndef __MENTOC_MODS_ORM_zone_mob_HEADER__
#define __MENTOC_MODS_ORM_zone_mob_HEADER__

#include "orm-base.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;

#define ZONE_MOB_ORM_MEMBERS ( \
(uint64_t,id,0,1,null,1), \
(mob_vnum,zm_mob_vnum,0,1,null,1), \
(room_vnum,zm_room_vnum,0,1,null,1), \
(uint16_t,zm_max,0,1,null,1))

#define ZONE_MOB_INITIALIZE_ROW_MEMBERS \
(zm_room_vnum) \
(zm_mob_vnum) \
(zm_max) \
 


	struct zone_mob : public mods::orm::orm_base<zone_mob,std::string> {
		MENTOC_ORM_CLASS(ZONE_MOB_ORM_MEMBERS,"zone_mob");
		auto vnum() {
			return id;
		}
	
		zone_mob() {
			this->init();
			loaded = 0;
			id = 0;
		}
		~zone_mob() = default;
	
		MENTOC_ORM_PRIMARY_KEY_FUNCTIONS();
		MENTOC_ORM_FEED_MULTI(ZONE_MOB_ORM_MEMBERS,zone_mob);
	
		std::deque<zone_mob> rows;
		auto destroy() {
			return  remove(this);
		}
		auto save() {
			return std::get<0>(this->update<zone_mob>(this));
		}
	#ifdef ZONE_MOB_INITIALIZE_ROW_MEMBERS
		MENTOC_ORM_INITIALIZE_ROW_USING(ZONE_MOB_INITIALIZE_ROW_MEMBERS);
	#endif
	};
	struct zone_mob_status_t {
		std::tuple<int16_t,std::string> orm_status;
		std::unique_ptr<zone_mob> record;
		zone_mob_status_t() = default;
		zone_mob_status_t(zone_mob_status_t&& other) : orm_status(other.orm_status),
			record(std::move(other.record)) {}
	};
	
	using zone_mob_list_t = std::deque<std::shared_ptr<zone_mob>>;
	
	zone_mob_list_t& zone_mob_list();
	
	zone_mob_list_t load_all_zone_mob_list();
};

#endif
