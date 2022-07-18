#ifndef __MENTOC_MODS_ORM_loot_payload_HEADER__
#define __MENTOC_MODS_ORM_loot_payload_HEADER__

#include "orm-base.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	static constexpr std::string_view raid_table_name = "raid";

#define LOOT_PAYLOAD_ORM_MEMBERS ( \
(uint64_t,id,0,1,null,1), \
(std::string,lp_rarity,"",3,null,1), \
(std::string,lp_levels,"",3,null,1), \
(room_vnum,lp_room,0,1,null,1), \
(uint16_t,lp_count,0,1,null,1))

#define LOOT_PAYLOAD_INITIALIZE_ROW_MEMBERS \
(lp_rarity) \
(lp_levels) \
(lp_count) \
 


	struct loot_payload : public mods::orm::orm_base<loot_payload,std::string> {
		MENTOC_ORM_CLASS(LOOT_PAYLOAD_ORM_MEMBERS,"loot_payload");
		auto vnum() {
			return id;
		}
	
		loot_payload() {
			this->init();
			loaded = 0;
			id = 0;
		}
		~loot_payload() = default;
	
		MENTOC_ORM_PRIMARY_KEY_FUNCTIONS();
		MENTOC_ORM_FEED_MULTI(LOOT_PAYLOAD_ORM_MEMBERS,loot_payload);
	
		std::deque<loot_payload> rows;
		auto destroy() {
			return  remove(this);
		}
	#ifdef LOOT_PAYLOAD_INITIALIZE_ROW_MEMBERS
		MENTOC_ORM_INITIALIZE_ROW_USING(LOOT_PAYLOAD_INITIALIZE_ROW_MEMBERS);
	#endif
	};
	struct loot_payload_status_t {
		std::tuple<int16_t,std::string> orm_status;
		std::unique_ptr<loot_payload> record;
		loot_payload_status_t() = default;
		loot_payload_status_t(loot_payload_status_t&& other) : orm_status(other.orm_status),
			record(std::move(other.record)) {}
	};
	
	using loot_payload_list_t = std::deque<std::shared_ptr<loot_payload>>;
	
	loot_payload_list_t& loot_payload_list();
	
	loot_payload_list_t load_all_loot_payload_list();
};

#endif
