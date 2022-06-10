#ifndef __MENTOC_MODS_ORM_raid_HEADER__
#define __MENTOC_MODS_ORM_raid_HEADER__

#include "orm-base.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	static constexpr std::string_view raid_table_name = "raid";

#define RAID_ORM_MEMBERS ( \
(uint64_t,id,0,1,null,1), \
(std::string,r_level,"",3,null,1), \
(std::string,r_name,"",3,null,1), \
(std::string,r_type,"",3,null,1), \
(std::string,r_status,"",3,null,1))

#define RAID_INITIALIZE_ROW_MEMBERS \
(r_name) \
(r_level) \
(r_type) \
(r_status) \
 


	struct raid : public mods::orm::orm_base<raid,std::string> {
		MENTOC_ORM_CLASS(RAID_ORM_MEMBERS,"raid");
		auto vnum() {
			return id;
		}
	
		raid() {
			this->init();
			loaded = 0;
			id = 0;
		}
		~raid() = default;
	
		MENTOC_ORM_PRIMARY_KEY_FUNCTIONS();
		MENTOC_ORM_FEED_MULTI(RAID_ORM_MEMBERS,raid);
	
		std::deque<raid> rows;
		auto destroy() {
			return  remove(this);
		}
	#ifdef RAID_INITIALIZE_ROW_MEMBERS
		MENTOC_ORM_INITIALIZE_ROW_USING(RAID_INITIALIZE_ROW_MEMBERS);
	#endif
	};
	struct raid_status_t {
		std::tuple<int16_t,std::string> orm_status;
		std::unique_ptr<raid> record;
		raid_status_t() = default;
		raid_status_t(raid_status_t&& other) : orm_status(other.orm_status),
			record(std::move(other.record)) {}
	};
	
	using raid_list_t = std::deque<std::shared_ptr<raid>>;
	
	raid_list_t& raid_list();
	
	raid_list_t load_all_raid_list();
};

#endif
