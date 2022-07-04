#ifndef __MENTOC_MODS_ORM_raid_areas_HEADER__
#define __MENTOC_MODS_ORM_raid_areas_HEADER__

#include "orm-base.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	static constexpr std::string_view raid_table_name = "raid";

#define RAID_AREAS_ORM_MEMBERS ( \
(uint64_t,id,0,1,null,1), \
(std::string,r_name,"",3,null,1), \
(std::string,r_roam_pattern,"",3,null,1), \
(room_vnum,r_spawn,0,1,null,1))

#define RAID_AREAS_INITIALIZE_ROW_MEMBERS \
(r_spawn) \
(r_name) \
(r_roam_pattern) \
 


	struct raid_areas : public mods::orm::orm_base<raid_areas,std::string> {
		MENTOC_ORM_CLASS(RAID_AREAS_ORM_MEMBERS,"raid_areas");
		auto vnum() {
			return id;
		}
	
		raid_areas() {
			this->init();
			loaded = 0;
			id = 0;
		}
		~raid_areas() = default;
	
		MENTOC_ORM_PRIMARY_KEY_FUNCTIONS();
		MENTOC_ORM_FEED_MULTI(RAID_AREAS_ORM_MEMBERS,raid_areas);
	
		std::deque<raid_areas> rows;
		auto destroy() {
			return  remove(this);
		}
	#ifdef RAID_AREAS_INITIALIZE_ROW_MEMBERS
		MENTOC_ORM_INITIALIZE_ROW_USING(RAID_AREAS_INITIALIZE_ROW_MEMBERS);
	#endif
	};
	struct raid_areas_status_t {
		std::tuple<int16_t,std::string> orm_status;
		std::unique_ptr<raid_areas> record;
		raid_areas_status_t() = default;
		raid_areas_status_t(raid_areas_status_t&& other) : orm_status(other.orm_status),
			record(std::move(other.record)) {}
	};
	
	using raid_areas_list_t = std::deque<std::shared_ptr<raid_areas>>;
	
	raid_areas_list_t& raid_areas_list();
	
	raid_areas_list_t load_all_raid_areas_list();
};

#endif
