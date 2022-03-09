#ifndef __MENTOC_MODS_ORM_ADMIN_rifle_attribute_limits_HEADER__
#define __MENTOC_MODS_ORM_ADMIN_rifle_attribute_limits_HEADER__

#include "../orm-base.hpp"

namespace mods::orm::admin {
	struct rifle_attribute_limits_record_t {
		uint64_t id;
		std::string type;
		std::string attribute;
		int low;
		int high;
		int overpowered;
		long created_at;
	};
	struct rifle_attribute_limits : public mods::orm::orm_base<rifle_attribute_limits,std::string> {
		using id_type_t = uint64_t;
		using type_t = std::string;
		using attr_t = std::string;
		using measurement_t = int;
		static constexpr std::string_view table = "rifle_attribute_limits";
		std::string table_name() const {
			return "rifle_attribute_limits";
		}
		std::string column_prefix() const {
			return "ral_";
		}
		std::string id_column() const {
			return "id";
		}
		rifle_attribute_limits() : id(0) {}
		~rifle_attribute_limits() = default;

		int16_t feed(const pqxx::result::reference&);
		int16_t load_by_type(type_t type);
		void init();
		strmap_t export_class();
		int16_t save() {
			return std::get<0>(this->update<rifle_attribute_limits>(this));
		}

		std::string primary_key_name() {
			return id_column();
		}
		std::string primary_key_value() {
			return std::to_string(this->id);
		}

		std::string type_column() {
			return "ral_type";
		}
		std::string type_value() {
			return ral_type;
		}

#define S(a) std::to_string(a)
		std::string attribute_column() {
			return "ral_attribute";
		}
		std::string attribute_value() {
			return ral_attribute;
		}

		std::string low_column() {
			return "ral_low";
		}
		std::string low_value() {
			return S(ral_low);
		}

		std::string high_column() {
			return "ral_high";
		}
		std::string high_value() {
			return S(ral_high);
		}

		std::string overpowered_column() {
			return "ral_overpowered";
		}
		std::string overpowered_value() {
			return S(ral_overpowered);
		}

		std::string created_at_column() {
			return "created_at";
		}
		std::string created_at_value() {
			return S(created_at);
		}
#undef S

		void load_all();

		id_type_t id;
		type_t ral_type;
		attr_t ral_attribute;
		measurement_t ral_low;
		measurement_t ral_high;
		measurement_t ral_overpowered;
		long created_at;
		bool loaded;
		bool saved_success;
	};


	std::deque<rifle_attribute_limits_record_t> load_all_rifle_attribute_limits();
};

#endif
