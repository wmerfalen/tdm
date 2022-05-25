#ifndef __MENTOC_MODS_ORM_generic_orm_HEADER__
#define __MENTOC_MODS_ORM_generic_orm_HEADER__

#define __MENTOC_ORM_UTIL_DEBUG_OUTPUT__
#include "orm-base.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	using sql_response_t = std::tuple<int16_t,std::string>;

	struct notch_t : public mods::orm::orm_base<notch_t,std::string> {
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
#define NOTCH_ORM_TUPLE ( \
(uint64_t,id,0,1,null,0), \
(uint64_t,n_points,0,1,null,0), \
(uint64_t,n_player_id,0,1,null,1), \
(std::string,n_name,"",3,name,1) \
)
		MENTOC_ORM_CLASS(NOTCH_ORM_TUPLE,"notch");

	};
};

#endif
