#ifndef __MENTOC_MODS_ORM_MOB_EQUIPMENT_HEADER__
#define __MENTOC_MODS_ORM_MOB_EQUIPMENT_HEADER__

#include "../../globals.hpp"
#include "orm-base.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	struct mob_equipment : public mods::orm::orm_base<mob_equipment,uint8_t> {
		static constexpr const char* table = "mob_equipment";
		std::string table_name() const {
			return table;
		}
		static std::vector<std::string> get_slot_list() {
			return {
				"profile_name",
				"vnum",
				"light",
				"finger_r",
				"finger_l",
				"neck_1",
				"neck_2",
				"body",
				"head",
				"legs",
				"feet",
				"hands",
				"arms",
				"shield",
				"about",
				"waist",
				"wrist_r",
				"wrist_l",
				"wield",
				"hold",
				"secondary",
				"shoulders_l",
				"shoulders_r",
				"backpack",
				"goggles",
				"vest_pack",
				"elbow_l",
				"elbow_r",
			};
		};

		auto vnum() {
			return meq_vnum;
		}
		std::string column_prefix() {
			return "meq_";
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
		std::tuple<bool,std::string> set_slot(std::string_view slot, std::string_view value);

		mob_equipment() : id(0) {
			this->init();
			loaded = 0;
		}
		~mob_equipment() = default;

		uint64_t initialize_row(std::string_view profile_name, uint64_t vnum);
		int16_t feed(const pqxx::result::reference&);
		void init();
		strmap_t export_class();
		primary_choice_t primary_type() {
			return 0;
		}
		int16_t save() {
			return std::get<0>(this->update<mob_equipment>(this));
		}
		bool destroy();

		int16_t				load_by_profile(std::string_view name);
		uint64_t id;
		std::array<std::string,NUM_WEARS> convert_to_array() const;
		std::string meq_profile_name;
		uint64_t meq_vnum;
		std::string meq_light;
		std::string meq_finger_r;
		std::string meq_finger_l;
		std::string meq_neck_1;
		std::string meq_neck_2;
		std::string meq_body;
		std::string meq_head;
		std::string meq_legs;
		std::string meq_feet;
		std::string meq_hands;
		std::string meq_arms;
		std::string meq_shield;
		std::string meq_about;
		std::string meq_waist;
		std::string meq_wrist_r;
		std::string meq_wrist_l;
		std::string meq_wield;
		std::string meq_hold;
		std::string meq_secondary;
		std::string meq_shoulders_l;
		std::string meq_shoulders_r;
		std::string meq_backpack;
		std::string meq_goggles;
		std::string meq_vest_pack;
		std::string meq_elbow_l;
		std::string meq_elbow_r;
		long created_at;
		long updated_at;
		bool loaded;
	};
	std::deque<std::shared_ptr<mods::orm::mob_equipment>>& mob_equipment_list();
};
#endif
