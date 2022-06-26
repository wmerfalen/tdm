#ifndef __MENTOC_MODS_ORM_radio_station_HEADER__
#define __MENTOC_MODS_ORM_radio_station_HEADER__

#include "orm-base.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	static constexpr std::string_view raid_table_name = "raid";

#define RADIO_STATION_ORM_MEMBERS ( \
(uint64_t,id,0,1,null,1), \
(std::string,rs_name,"",3,null,1), \
(room_vnum,rs_room_vnum,0,1,null,1), \
(uint16_t,rs_power,0,1,null,1))

#define RADIO_STATION_INITIALIZE_ROW_MEMBERS \
(rs_power) \
(rs_room_vnum) \
(rs_name) \
 


	struct radio_station : public mods::orm::orm_base<radio_station,std::string> {
		MENTOC_ORM_CLASS(RADIO_STATION_ORM_MEMBERS,"radio_station");
		auto vnum() {
			return id;
		}
	
		radio_station() {
			this->init();
			loaded = 0;
			id = 0;
		}
		~radio_station() = default;
	
		MENTOC_ORM_PRIMARY_KEY_FUNCTIONS();
		MENTOC_ORM_FEED_MULTI(RADIO_STATION_ORM_MEMBERS,radio_station);
	
		std::deque<radio_station> rows;
		auto destroy() {
			return  remove(this);
		}
	#ifdef RADIO_STATION_INITIALIZE_ROW_MEMBERS
		MENTOC_ORM_INITIALIZE_ROW_USING(RADIO_STATION_INITIALIZE_ROW_MEMBERS);
	#endif
	};
	struct radio_station_status_t {
		std::tuple<int16_t,std::string> orm_status;
		std::unique_ptr<radio_station> record;
		radio_station_status_t() = default;
		radio_station_status_t(radio_station_status_t&& other) : orm_status(other.orm_status),
			record(std::move(other.record)) {}
	};
	
	using radio_station_list_t = std::deque<std::shared_ptr<radio_station>>;
	
	radio_station_list_t& radio_station_list();
	
	radio_station_list_t load_all_radio_station_list();
};

#endif
