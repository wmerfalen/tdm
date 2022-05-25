#ifndef __MENTOC_MODS_ORM_notch_HEADER__
#define __MENTOC_MODS_ORM_notch_HEADER__

//#define __MENTOC_ORM_UTIL_DEBUG_OUTPUT__
#include "orm-base.hpp"
#include "generic-orm.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	static constexpr std::string_view notch_table_name = "notch";
	struct notch_record_t {
		uint64_t id;
		std::string n_name;
		uint16_t n_points;
		uint64_t n_player_id;
		strmap_t export_class() {
			strmap_t v;
			v["n_name"] = n_name;
			v["n_points"] = std::to_string(this->n_points);
			v["n_player_id"] = std::to_string(this->n_player_id);
			return std::move(v);
		}
		std::string table_name() const {
			return notch_table_name.data();
		}
		std::string primary_key_name() {
			return "id";
		}
	};

	struct notch : public mods::orm::orm_base<notch,std::string> {
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

		using primary_choice_t = std::string;
		std::string table_name() const {
			return notch_table_name.data();
		}
		std::string column_prefix() const {
			return "n_";
		}
		std::string id_column() const {
			return "id";
		}
		notch() {
			this->init();
			loaded = 0;
			id = 0;
		}
		~notch() = default;

		std::string primary_key_name() {
			return id_column();
		}
		std::string primary_key_value();

		void load_multi(const pqxx::result::reference&);
		void populate(const uint64_t& player_id, const std::map<std::string,uint16_t>& data);
		std::map<std::string,uint16_t> get_player_levels(const uint64_t& player_id, std::string_view notch);

		void feed_multi(pqxx::result&);


		std::tuple<int16_t,std::string> delete_by_player(const uint64_t& player_id);
		std::tuple<int16_t,std::string> load_by_player(const uint64_t& player_id);
		std::tuple<int16_t,std::string> load_by_player_and_notch(const uint64_t& player_id,std::string_view notch);

		std::vector<notch_record_t> rows;
	};
	std::tuple<int16_t,std::string> increment_player_notch(player_ptr_t& player,std::string_view notch);
};

#endif
