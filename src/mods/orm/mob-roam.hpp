#ifndef __MENTOC_MODS_ORM_MOB_ROAM_HEADER__
#define __MENTOC_MODS_ORM_MOB_ROAM_HEADER__

#include "orm-base.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	struct mob_roam_record_t {
		uint32_t mob_virtual_number;
		uint32_t room_virtual_number;
		std::string profile_name;
		strmap_t export_class() {
			strmap_t v;
			v["mob_virtual_number"] = std::to_string(this->mob_virtual_number);
			v["room_virtual_number"] = std::to_string(this->room_virtual_number);
			v["profile_name"] = profile_name;
			return std::move(v);
		}
		std::string table_name() const {
			return "mob_roam";
		}
		std::string primary_key_name() {
			return "id";
		}
	};

	struct mob_roam : public mods::orm::orm_base<mob_roam,std::string> {
		using primary_choice_t = std::string;
		std::string table_name() const {
			return "mob_roam";
		}
		std::string column_prefix() const {
			return "mob_";
		}
		std::string id_column() const {
			return "id";
		}
		mob_roam() : id(0) {
			this->init();
			loaded = 0;
		}
		~mob_roam() = default;

		std::string primary_key_name() {
			return id_column();
		}
		std::string primary_key_value() {
			return std::to_string(this->id);
		}

		void load_multi(const pqxx::result::reference&);
		void populate(uint32_t mob_vnum, const std::vector<room_vnum>& rooms,std::string_view profile_name);
		void init();

		std::tuple<int16_t,std::string> load_all();
		void feed_multi(pqxx::result&);

		strmap_t export_class();
		int16_t save();
		using id_type_t = uint64_t;
		using room_vnum_t = uint32_t;

		std::tuple<int16_t,std::string> delete_by_profile_name(std::string_view);
		std::tuple<int16_t,std::string> delete_by_mob_vnum(mob_vnum);

		void get_compound_list();
		void delete_foobar_profile();

		id_type_t id;
		room_vnum_t event_room_vnum;
		std::string profile_name;
		uint32_t mob_virtual_number;
		std::vector<mob_roam_record_t> rows;
		bool loaded;
	};
	std::tuple<int16_t,std::string> load_all_roaming_data(std::map<mob_vnum,std::vector<room_vnum>>* storage_ptr);
};

#endif
