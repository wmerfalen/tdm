#ifndef __MENTOC_MODS_ORM_HQ_HEADER__
#define __MENTOC_MODS_ORM_HQ_HEADER__

#include "../../globals.hpp"
#include "orm-base.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	struct hq : public mods::orm::orm_base<hq,uint8_t> {
		static constexpr const char* table_name_value = "hq_locations";
		std::string table_name() {
			return table_name_value;
		}
		std::string column_prefix() {
			return "hq_";
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

		hq() : id(0) {
			this->init();
			loaded = 0;
		}
		~hq() = default;

		uint64_t initialize_row(std::string_view affiliation, room_vnum rv,uint16_t level);
		int16_t feed(const pqxx::result::reference&);
		void init();
		strmap_t export_class();
		primary_choice_t primary_type() {
			return 0;
		}
		int16_t save() {
			return std::get<0>(this->update<hq>(this));
		}
		bool destroy();

		int16_t				load_by_profile(std::string_view name);
		uint64_t id;
		std::string hq_affiliation;
		uint64_t hq_level;
		uint32_t hq_room_vnum;
		uint16_t hq_basic_mob_count;
		uint16_t hq_advanced_mob_count;
		uint16_t hq_elite_mob_count;
		uint16_t hq_suv_count;
		uint16_t hq_sedan_count;
		uint16_t hq_armored_van_count;
		uint16_t hq_replenish_ticks;
		uint16_t hq_replenish_basic_count;
		uint16_t hq_replenish_advanced_count;
		uint16_t hq_replenish_elite_count;
		long created_at;
		long updated_at;
		bool loaded;
	};
	extern std::deque<std::shared_ptr<mods::orm::hq>> hq_list;
};
#endif
