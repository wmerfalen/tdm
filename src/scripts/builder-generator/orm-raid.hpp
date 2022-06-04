#ifndef __MENTOC_MODS_ORM____[[[TABLE]]]____HEADER__
#define __MENTOC_MODS_ORM____[[[TABLE]]]____HEADER__

#include "orm-base.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	static constexpr std::string_view raid_table_name = "raid";

#define ___[[[TABLE]]]____ORM_MEMBERS ( \
(uint64_t,id,0,1,null,1), \
___[[[MEMBERS_TUPLE]]]___
	)

#define ___[[[TABLE]]]____ORM_STRING_LIMITS ( \
___[[[STRING_LIMITS]]]___
	)

	struct ___[[[TABLE]]]___ : public mods::orm::orm_base<___[[[TABLE]]]___,std::string> {
		MENTOC_ORM_STRING_LIMITS(___[[[TABLE]]]____ORM_STRING_LIMITS);
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
		MENTOC_ORM_CLASS(___[[[TABLE]]]____ORM_MEMBERS,"___[[[TABLE]]]___");
		auto vnum() {
			return id;
		}

		___[[[TABLE]]]___() {
			this->init();
			loaded = 0;
			id = 0;
		}
		~___[[[TABLE]]]___() = default;

		void feed_multi(pqxx::result&);

		MENTOC_ORM_PRIMARY_KEY_FUNCTIONS();

		std::deque<___[[[TABLE]]]___> rows;
		auto destroy() {
			return  remove(this);
		}
		___[[[INITIALIZE_ROW]]]___
	};
	struct ___[[[TABLE]]]____status_t {
		std::tuple<int16_t,std::string> orm_status;
		std::unique_ptr<___[[[TABLE]]]___> record;
		___[[[TABLE]]]____status_t() = default;
		___[[[TABLE]]]____status_t(___[[[TABLE]]]____status_t&& other) : orm_status(other.orm_status),
			record(std::move(other.record)) {}
	};

	using ___[[[TABLE]]]____list_t = std::deque<std::shared_ptr<___[[[TABLE]]]___>>;

	___[[[TABLE]]]____status_t create____[[[TABLE]]]___(
	    std::string_view name,
	    std::string_view level,
	    std::string_view type);

	___[[[TABLE]]]____list_t& ___[[[TABLE]]]____list();

	___[[[TABLE]]]____list_t load_all____[[[TABLE]]]____list();
};

#endif
