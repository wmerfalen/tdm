#ifndef __MENTOC_MODS_ORM_static_loot_HEADER__
#define __MENTOC_MODS_ORM_static_loot_HEADER__

#include "orm-base.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	static constexpr std::string_view raid_table_name = "raid";

#define STATIC_LOOT_ORM_MEMBERS ( \
(uint64_t,id,0,1,null,1), \
(std::string,sl_yaml,"",3,null,1), \
(room_vnum,sl_room,0,1,null,1), \
(uint16_t,sl_count,0,1,null,1))

#define STATIC_LOOT_INITIALIZE_ROW_MEMBERS \
(sl_room) \
(sl_yaml) \
(sl_count) \
 


	struct static_loot : public mods::orm::orm_base<static_loot,std::string> {
		MENTOC_ORM_CLASS(STATIC_LOOT_ORM_MEMBERS,"static_loot");
		auto vnum() {
			return id;
		}
	
		static_loot() {
			this->init();
			loaded = 0;
			id = 0;
		}
		~static_loot() = default;
	
		MENTOC_ORM_PRIMARY_KEY_FUNCTIONS();
		MENTOC_ORM_FEED_MULTI(STATIC_LOOT_ORM_MEMBERS,static_loot);
	
		std::deque<static_loot> rows;
		auto destroy() {
			return  remove(this);
		}
		auto save() {
			return std::get<0>(this->update<static_loot>(this));
		}
	#ifdef STATIC_LOOT_INITIALIZE_ROW_MEMBERS
		MENTOC_ORM_INITIALIZE_ROW_USING(STATIC_LOOT_INITIALIZE_ROW_MEMBERS);
	#endif
	};
	struct static_loot_status_t {
		std::tuple<int16_t,std::string> orm_status;
		std::unique_ptr<static_loot> record;
		static_loot_status_t() = default;
		static_loot_status_t(static_loot_status_t&& other) : orm_status(other.orm_status),
			record(std::move(other.record)) {}
	};
	
	using static_loot_list_t = std::deque<std::shared_ptr<static_loot>>;
	
	static_loot_list_t& static_loot_list();
	
	static_loot_list_t load_all_static_loot_list();
};

#endif
