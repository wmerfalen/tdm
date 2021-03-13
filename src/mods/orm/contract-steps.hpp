#ifndef __MENTOC_MODS_ORM_contract_steps_HEADER__
#define __MENTOC_MODS_ORM_contract_steps_HEADER__

#include "orm-base.hpp"
#include "../contract-types.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	static constexpr std::string_view contract_steps_table_name = "contract_steps";
	using contract_vnum_t = uint32_t;
	using task_type_t = uint16_t;
	using task_target_t = uint16_t;
	using quota_t = uint8_t;
	struct contract_steps_record_t {
		uint64_t id;
		contract_vnum_t s_contract_vnum;
		task_type_t s_task_type;
		task_target_t s_task_target;
		std::string s_description;
		mob_vnum s_mob_vnum;
		room_vnum s_room_vnum;
		quota_t s_quota;
		bool s_is_optional;
		uint8_t s_order;
		std::string s_object_yaml;
		long created_at;
		long updated_at;
		strmap_t export_class() {
			strmap_t v;
			v["s_contract_vnum"] = std::to_string(s_contract_vnum);
			v["s_task_type"] = std::to_string(s_task_type);
			v["s_task_target"] = std::to_string(s_task_target);
			v["s_description"] = s_description;
			v["s_mob_vnum"] = std::to_string(s_mob_vnum);
			v["s_room_vnum"] = std::to_string(s_room_vnum);
			v["s_quota"] = std::to_string(s_quota);
			v["s_is_optional"] = std::to_string(s_is_optional);
			v["s_order"] = std::to_string(s_order);
			v["s_object_yaml"] = s_object_yaml;
			return std::move(v);
		}
		std::string table_name() const {
			return contract_steps_table_name.data();
		}
		std::string primary_key_name() {
			return "id";
		}
	};

	struct contract_steps : public mods::orm::orm_base<contract_steps,std::string> {
		using primary_choice_t = std::string;
		static constexpr const char* table_name_value = "contract_steps";
		std::string table_name() const {
			return contract_steps_table_name.data();
		}
		std::string column_prefix() const {
			return "s_";
		}
		std::string id_column() const {
			return "id";
		}
		contract_steps() {
			init();
			loaded = 0;
			id = 0;
		}
		contract_steps(const contract_vnum_t& contract_vnum) {
			auto s = load_by_contract_vnum(contract_vnum);
			std::cerr << "contract_steps(contract_vnum) constructor status: " << std::get<0>(s) << "|'" << std::get<1>(s) << "'\n";
		}
		void fill(mods::contracts::contract& c) {
			for(auto& row : rows) {
				std::cerr << green_str("contract_steps filling contract:") << c.vnum << "\n";
				c.steps.emplace_back();
				auto& step = c.steps.back();
				step.goal = (mods::contracts::contract_step::task_type_t)row.s_task_type;
				step.target = (mods::contracts::contract_step::task_target_t)row.s_task_target;
				step.description = row.s_description;
				step.mob_vnum = row.s_mob_vnum;
				step.object_yaml = row.s_object_yaml;
				step.room = row.s_room_vnum;
				step.quota = row.s_quota;;
				step.is_optional = row.s_is_optional;
				c.steps.emplace_back(step);
			}
		}
		~contract_steps() = default;

		std::string primary_key_name() {
			return id_column();
		}
		std::string primary_key_value();

		void load_multi(const pqxx::result::reference&);
		void init();

		void destroy();
		std::tuple<int16_t,std::string> destroy_status;
		uint64_t initialize_row(const contract_vnum_t& i_vnum);
		std::string& dump() {
			static std::string buffer;
			buffer.clear();
			buffer += CAT("[",primary_key_name(),"]:->'",primary_key_value(),"'\r\n");
			for(const auto& pair : export_class()) {
				buffer += CAT("[",pair.first,"]->'",pair.second,"'\r\n");
			}
			return buffer;
		}


		void feed_multi(pqxx::result&);

		strmap_t export_class();
		int16_t save();

		std::tuple<int16_t,std::string> delete_by_contract_vnum(const contract_vnum_t& contract_vnum);
		std::tuple<int16_t,std::string> load_by_contract_vnum(const contract_vnum_t& contract_vnum);
		std::tuple<int16_t,std::string> update_row();
		int16_t feed(const pqxx::result::reference& row);

		std::vector<contract_steps_record_t> rows;
		bool loaded;
		static const std::vector<std::string>& column_list() {
			static const std::vector<std::string>& list = {
				"id",
				"s_contract_vnum",
				"s_task_type",
				"s_task_target",
				"s_description",
				"s_mob_vnum",
				"s_room_vnum",
				"s_quota",
				"s_is_optional",
				"s_order",
				"s_object_yaml"
			};
			return list;
		}

		uint64_t id;
		contract_vnum_t s_contract_vnum;
		task_type_t s_task_type;
		task_target_t s_task_target;
		std::string s_description;
		mob_vnum s_mob_vnum;
		room_vnum s_room_vnum;
		quota_t s_quota;
		bool s_is_optional;
		uint8_t s_order;
		std::string s_object_yaml;
		long created_at;
		long updated_at;
	};
	std::deque<std::shared_ptr<contract_steps>>& contract_steps_list();
	std::tuple<int16_t,std::string> gather_contract_steps_by_contract_vnum(const contract_vnum_t& contract_vnum,std::deque<std::shared_ptr<mods::orm::contract_steps>>* list);
	//std::tuple<int16_t,std::string> load_player_skill_data(player_ptr_t& player, std::map<uint32_t,uint16_t>* data);
	//std::tuple<int16_t,std::string> sync_player_with_class_skills(const uint64_t& player_id, std::string_view player_class);
};

#endif
