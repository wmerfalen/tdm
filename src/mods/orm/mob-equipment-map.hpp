#ifndef __MENTOC_MODS_ORM_MOB_EQUIPMENT_MAP_HEADER__
#define __MENTOC_MODS_ORM_MOB_EQUIPMENT_MAP_HEADER__

#include "../../globals.hpp"
#include "orm-base.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	struct mob_equipment_map : public mods::orm::orm_base<mob_equipment_map,uint8_t> {
		const char* table = "mob_equipment_map";
		std::string table_name() {
			return table;
		}
		std::string column_prefix() {
			return "mmap_";
		}
		std::string id_column() {
			return "id";
		}
		std::string primary_key_name() {
			return this->id_column();
		}
		std::string primary_key_value() {
			return std::to_string(this->id);
		}

		mob_equipment_map() : id(0) {
			this->init();
			loaded = 0;
		}
		mob_equipment_map(const uint64_t& mob_vn, const uint64_t& eq_vn) : id(0) {
			this->init();
			loaded = 0;
			mmap_mob_vnum = mob_vn;
			mmap_mob_equipment_vnum = eq_vn;
			this->id = this->initialize_row(mmap_mob_vnum,mmap_mob_equipment_vnum);
		}
		~mob_equipment_map() = default;

		uint64_t initialize_row(const uint64_t& mob_vnum, const uint64_t& equipment_vnum);
		int16_t feed(const pqxx::result::reference&);
		void init();
		strmap_t export_class();
		primary_choice_t primary_type() {
			return 0;
		}
		int16_t save() {
			return std::get<0>(this->update<mob_equipment_map>(this));
		}
		bool destroy();

		int16_t				load_by_mob_vnum(const uint64_t& mob_vnum);
		uint64_t id;
		uint64_t mmap_mob_vnum;
		uint64_t mmap_mob_equipment_vnum;
		long created_at;
		long updated_at;
		bool loaded;
		bool destroyed;
	};
	std::deque<std::shared_ptr<mods::orm::mob_equipment_map>>& mob_equipment_map_list();
};
#endif
