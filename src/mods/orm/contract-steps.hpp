#ifndef __MENTOC_MODS_ORM_contract_steps_HEADER__
#define __MENTOC_MODS_ORM_contract_steps_HEADER__

#include "orm-base.hpp"
#include "../contract-types.hpp"

#ifdef  __MENTOC_SHOW_CONTRACT_OUTPUT__
#define dbg_print(a) std::cerr << "[mods::contracts][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#else
#define dbg_print(a)
#endif
namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	using contract_vnum_t = uint32_t;
	using task_type_t = uint16_t;
	using task_target_t = uint16_t;
	using quota_t = uint8_t;
	struct contract_steps_record_t {
		const char* table = "contract_steps";
		uint64_t id;
		contract_vnum_t s_contract_vnum;
		task_type_t s_task_type;
		task_target_t s_task_target;
		std::string s_description;
		mob_vnum s_mob_vnum;
		room_vnum s_room_vnum;
		quota_t s_quota;
		uint32_t s_reward_xp;
		uint32_t s_reward_money;
		std::string s_reward_1;
		std::string s_reward_2;
		std::string s_reward_3;
		std::string s_reward_4;
		std::string s_reward_5;
		std::string s_reward_6;
		std::string s_reward_7;
		std::string s_reward_8;
		std::string s_reward_9;
		std::string s_reward_10;
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
			v["s_reward_xp"] = std::to_string(s_reward_xp);
			v["s_reward_money"] = std::to_string(s_reward_money);
			v["s_reward_1"] = s_reward_1;
			v["s_reward_2"] = s_reward_2;
			v["s_reward_3"] = s_reward_3;
			v["s_reward_4"] = s_reward_4;
			v["s_reward_5"] = s_reward_5;
			v["s_reward_6"] = s_reward_6;
			v["s_reward_7"] = s_reward_7;
			v["s_reward_8"] = s_reward_8;
			v["s_reward_9"] = s_reward_9;
			v["s_reward_10"] = s_reward_10;
			return std::move(v);
		}
		std::string table_name() const {
			return table;
		}
		std::string primary_key_name() {
			return "id";
		}
	};

	struct contract_steps : public mods::orm::orm_base<contract_steps,std::string> {
		using primary_choice_t = std::string;
		const char* table = "contract_steps";
		std::string table_name() {
			return table;
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
			dbg_print("contract_steps(contract_vnum) constructor status: " << std::get<0>(s) << "|'" << std::get<1>(s) << "'");
		}
		void fill(mods::contracts::contract& c) {
			for(auto& row : rows) {
				dbg_print(green_str("contract_steps filling contract:") << c.vnum);
				c.steps.emplace_back();
				auto& step = c.steps.back();
				step.goal = (mods::contracts::contract_step::task_type_t)row.s_task_type;
				step.target = (mods::contracts::contract_step::task_target_t)row.s_task_target;
				step.description = row.s_description;
				step.mob_vnum = row.s_mob_vnum;
				step.object_yaml = row.s_object_yaml;
				step.room = row.s_room_vnum;
				step.quota = row.s_quota;
				step.is_optional = row.s_is_optional;
				step.reward_xp = row.s_reward_xp;
				step.reward_money = row.s_reward_money;
				step.reward_1 = row.s_reward_1;
				step.reward_2 = row.s_reward_2;
				step.reward_3 = row.s_reward_3;
				step.reward_4 = row.s_reward_4;
				step.reward_5 = row.s_reward_5;
				step.reward_6 = row.s_reward_6;
				step.reward_7 = row.s_reward_7;
				step.reward_8 = row.s_reward_8;
				step.reward_9 = row.s_reward_9;
				step.reward_10 = row.s_reward_10;
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
				"s_object_yaml",
				"s_reward_xp",
				"s_reward_money",
				"s_reward_1",
				"s_reward_2",
				"s_reward_3",
				"s_reward_4",
				"s_reward_5",
				"s_reward_6",
				"s_reward_7",
				"s_reward_8",
				"s_reward_9",
				"s_reward_10",
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
		uint32_t s_reward_xp;
		uint32_t s_reward_money;
		std::string s_object_yaml;
		std::string s_reward_1;
		std::string s_reward_2;
		std::string s_reward_3;
		std::string s_reward_4;
		std::string s_reward_5;
		std::string s_reward_6;
		std::string s_reward_7;
		std::string s_reward_8;
		std::string s_reward_9;
		std::string s_reward_10;
		long created_at;
		long updated_at;
	};
	std::deque<std::shared_ptr<contract_steps>>& contract_steps_list();
	std::tuple<int16_t,std::string> gather_contract_steps_by_contract_vnum(const contract_vnum_t& contract_vnum,std::deque<std::shared_ptr<mods::orm::contract_steps>>* list);
	//std::tuple<int16_t,std::string> load_player_skill_data(player_ptr_t& player, std::map<uint32_t,uint16_t>* data);
	//std::tuple<int16_t,std::string> sync_player_with_class_skills(const uint64_t& player_id, std::string_view player_class);
};

#undef dbg_print
#endif
