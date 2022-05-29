#ifndef __MENTOC_MODS_ORM_raid_HEADER__
#define __MENTOC_MODS_ORM_raid_HEADER__

#include "orm-base.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	static constexpr std::string_view raid_table_name = "raid";

#define RAID_ORM_MEMBERS ( \
(uint64_t,id,0,1,null,1), \
(std::string,r_name,"",3,null,1), \
(std::string,r_level,"",3,null,1), \
(std::string,r_type,"",3,null,1), \
(std::string,r_status,"incomplete",3,null,1) \
)

	struct raid : public mods::orm::orm_base<raid,std::string> {
		/**
		 * member tuple csv columns:
		 * Member Var Type, Member Var Name, initailized value, pqxx conversion type, native object field, slot list
		 *
		 * Member var type: int, std::string, etc
		 * Member Var Name: id, s_sequence_vnum, etc
		 * initialized value: the value to set it to when ::init() is called
		 * pqxx conversion type: 1 = use .as<var_type>(), 2 = use vectorize, 3 = use .c_str()
		 * native object field: the member variable name of mods::scripted_step
		 * slot list: 1 = list this var in slot_list(), 0 = don't list in slot_list
		 */
		MENTOC_ORM_CLASS(RAID_ORM_MEMBERS,"raid");

		raid() {
			this->init();
			loaded = 0;
			id = 0;
		}
		~raid() = default;

		void feed_multi(pqxx::result&);

		MENTOC_ORM_PRIMARY_KEY_FUNCTIONS();

		std::vector<raid> rows;
	};
	struct raid_status_t {
		std::tuple<int16_t,std::string> orm_status;
		std::unique_ptr<raid> record;
		raid_status_t() = default;
		raid_status_t(raid_status_t&& other) : orm_status(other.orm_status),
			record(std::move(other.record)) {}
	};
	raid_status_t create_raid(
	    std::string_view name,
	    std::string_view level,
	    std::string_view type);
};

#endif
