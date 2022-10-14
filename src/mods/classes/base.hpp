#ifndef __MENTOC_MODS_CLASSES_BASE_HEADER__
#define  __MENTOC_MODS_CLASSES_BASE_HEADER__

#include "types.hpp"
#include "../weapon.hpp"
#include "../bugs-fixtures.hpp"
#include <map>
#include <string>
#include "../orm/util.hpp"
#include "../player.hpp"
#include "../rand.hpp"

namespace mods::classes {
	struct skill_t {
			static constexpr const char* table_name = "skill_usage";
			virtual float get_increment();
			virtual float get_usage_increment();
			skill_t() = default;

			std::tuple<bool,std::string> load_skill_by_player(std::string_view skill_name, player_ptr_t& player);
			const float& get_current_level() const;
			void use_skill(player_ptr_t& player);
			std::tuple<bool,std::string> practice(player_ptr_t& player);
			std::map<std::string,std::string>& export_class();
			static constexpr float NOT_LEARNED = 0.0;
			static constexpr float AWFUL = 15.0;
			static constexpr float OKAY = 250.0;
			static constexpr float LEARNED = 280.0;
			static constexpr float MASTER = 10000.0;
			static constexpr float ELITE = 50000.0;

			bool not_learned() const;

			bool terrible() const;
			bool awful() const;
			bool okay() const;
			bool learned() const;
			bool mastered() const;
			bool elite() const;

			std::string get_proficiency() const;
			int16_t feed(const pqxx::result::reference& row);
			std::tuple<bool,std::string> load(player_ptr_t& player,std::string_view skill_name);
			std::tuple<bool,std::string> save(player_ptr_t& player);
			std::string dump();

		private:
			std::string m_name;
			float m_current_level;
			std::map<std::string,std::string> m_data;
			uint64_t m_db_id;
			bool m_loaded_properly;
			std::tuple<bool,std::string> m_load_status;
	};
	struct base {
			virtual types kind() = 0;
			virtual player_ptr_t get_player_ptr() = 0;
			struct ability_data_t {
				enum skillset_t : uint8_t {
					NONE = 0,
					STRENGTH,
					INTELLIGENCE,
					DEXTERITY,
					CONSTITUTION,
					ELECTRONICS,
					ARMOR,
					MARKSMANSHIP,
					SNIPING,
					DEMOLITIONS,
					CHEMISTRY,
					WEAPON_HANDLING,
					STRATEGY,
					MEDICAL
				};
				uint8_t ability_value;
				std::string shortened;
				std::string pretty;
				skillset_t category;
				skill_t* skill_ptr;
				ability_data_t() = default;
				ability_data_t(uint8_t av,std::string sh,std::string p,skillset_t c,skill_t* ptr) :
					ability_value(av), shortened(sh), pretty(p), category(c), skill_ptr(ptr) { }
				std::string dump() const;
			};

			using ability_list_t = std::vector<ability_data_t>;
			enum ability_t {
				ASSAULT_RIFLES,
				SNIPER_RIFLES,
				SUB_MACHINE_GUNS,
				SHOTGUNS,
				PISTOLS,
				MACHINE_PISTOLS,
				LIGHT_MACHINE_GUNS,
				JAB_TO_HEAD,
				JAB_TO_BODY,
				CROSS_TO_HEAD,
				CROSS_TO_BODY,
				LEFT_HOOK_TO_HEAD,
				RIGHT_HOOK_TO_HEAD,
				LEFT_HOOK_TO_BODY,
				RIGHT_HOOK_TO_BODY,
				LEFT_UPPERCUT,
				RIGHT_UPPERCUT,
				LEFT_ELBOW,
				RIGHT_ELBOW,
				RIGHT_UPWARD_ELBOW,
				LEFT_UPWARD_ELBOW,
				RIGHT_OBLIQUE,
				LEFT_OBLIQUE,
				LEFT_TEEP,
				RIGHT_TEEP,
				LEFT_FRONT_KICK,
				RIGHT_FRONT_KICK,
				LEFT_KNEE_TO_HEAD,
				RIGHT_KNEE_TO_HEAD,
				LEFT_KNEE_TO_BODY,
				RIGHT_KNEE_TO_BODY,
				KNIFE_DISARM,
				PISTOL_DISARM,
				RIGHT_LEG_KICK,
				LEFT_LEG_KICK,
				RIGHT_KICK_TO_HEAD,
				LEFT_KICK_TO_HEAD,
				RIGHT_STOMP_TO_HEAD,
				LEFT_STOMP_TO_HEAD,
			};
			ability_list_t& weapon_abilities();
			ability_list_t& cqc_abilities();
			ability_list_t create_abilities(ability_list_t additional_list);
			virtual ability_list_t& get_abilities() = 0;

			skill_t* skill_ptr(std::string_view skill);
			skill_t* skill_ptr(uint8_t skill);
			std::tuple<bool,std::string> practice(std::string_view skill);
			std::tuple<bool,std::string> roll_skill_success(uint8_t skill);
			void initialize_skills_for_player(player_ptr_t& player);
			std::string skills_page();
			std::string shorthand_page();
			std::string request_page_for(std::string_view page);
			void report(std::vector<std::string> msgs);
			void report(std::string_view msg);
			skill_t& wpn_assault_rifles() {
				return m_wpn_assault_rifles;
			}
			skill_t& wpn_sniper_rifles() {
				return m_wpn_sniper_rifles;
			}
			skill_t& wpn_sub_machine_guns() {
				return m_wpn_sub_machine_guns;
			}
			skill_t& wpn_shotguns() {
				return m_wpn_shotguns;
			}
			skill_t& wpn_pistols() {
				return m_wpn_pistols;
			}
			skill_t& wpn_machine_pistols() {
				return m_wpn_machine_pistols;
			}
			skill_t& wpn_light_machine_guns() {
				return m_wpn_light_machine_guns;
			}
			skill_t& cqc_jab_to_head() {
				return m_cqc_jab_to_head;
			}
			skill_t& cqc_jab_to_body() {
				return m_cqc_jab_to_body;
			}
			skill_t& cqc_cross_to_head() {
				return m_cqc_cross_to_head;
			}
			skill_t& cqc_cross_to_body() {
				return m_cqc_cross_to_body;
			}
			skill_t& cqc_left_hook_to_head() {
				return m_cqc_left_hook_to_head;
			}
			skill_t& cqc_right_hook_to_head() {
				return m_cqc_right_hook_to_head;
			}
			skill_t& cqc_left_hook_to_body() {
				return m_cqc_left_hook_to_body;
			}
			skill_t& cqc_right_hook_to_body() {
				return m_cqc_right_hook_to_body;
			}
			skill_t& cqc_left_uppercut() {
				return m_cqc_left_uppercut;
			}
			skill_t& cqc_right_uppercut() {
				return m_cqc_right_uppercut;
			}
			skill_t& cqc_left_elbow() {
				return m_cqc_left_elbow;
			}
			skill_t& cqc_right_elbow() {
				return m_cqc_right_elbow;
			}
			skill_t& cqc_right_upward_elbow() {
				return m_cqc_right_upward_elbow;
			}
			skill_t& cqc_left_upward_elbow() {
				return m_cqc_left_upward_elbow;
			}
			skill_t& cqc_right_oblique() {
				return m_cqc_right_oblique;
			}
			skill_t& cqc_left_oblique() {
				return m_cqc_left_oblique;
			}
			skill_t& cqc_left_teep() {
				return m_cqc_left_teep;
			}
			skill_t& cqc_right_teep() {
				return m_cqc_right_teep;
			}
			skill_t& cqc_left_front_kick() {
				return m_cqc_left_front_kick;
			}
			skill_t& cqc_right_front_kick() {
				return m_cqc_right_front_kick;
			}
			skill_t& cqc_left_knee_to_head() {
				return m_cqc_left_knee_to_head;
			}
			skill_t& cqc_right_knee_to_head() {
				return m_cqc_right_knee_to_head;
			}
			skill_t& cqc_left_knee_to_body() {
				return m_cqc_left_knee_to_body;
			}
			skill_t& cqc_right_knee_to_body() {
				return m_cqc_right_knee_to_body;
			}
			skill_t& cqc_knife_disarm() {
				return m_cqc_knife_disarm;
			}
			skill_t& cqc_pistol_disarm() {
				return m_cqc_pistol_disarm;
			}
			skill_t& cqc_right_leg_kick() {
				return m_cqc_right_leg_kick;
			}
			skill_t& cqc_left_leg_kick() {
				return m_cqc_left_leg_kick;
			}
			skill_t& cqc_right_kick_to_head() {
				return m_cqc_right_kick_to_head;
			}
			skill_t& cqc_left_kick_to_head() {
				return m_cqc_left_kick_to_head;
			}
			skill_t& cqc_right_stomp_to_head() {
				return m_cqc_right_stomp_to_head;
			}
			skill_t& cqc_left_stomp_to_head() {
				return m_cqc_left_stomp_to_head;
			}
		private:
			ability_list_t m_weapon_abilities;
			ability_list_t m_cqc_abilities;
			skill_t m_wpn_assault_rifles;
			skill_t m_wpn_sniper_rifles;
			skill_t m_wpn_sub_machine_guns;
			skill_t m_wpn_shotguns;
			skill_t m_wpn_pistols;
			skill_t m_wpn_machine_pistols;
			skill_t m_wpn_light_machine_guns;
			skill_t m_cqc_jab_to_head;
			skill_t m_cqc_jab_to_body;
			skill_t m_cqc_cross_to_head;
			skill_t m_cqc_cross_to_body;
			skill_t m_cqc_left_hook_to_head;
			skill_t m_cqc_right_hook_to_head;
			skill_t m_cqc_left_hook_to_body;
			skill_t m_cqc_right_hook_to_body;
			skill_t m_cqc_left_uppercut;
			skill_t m_cqc_right_uppercut;
			skill_t m_cqc_left_elbow;
			skill_t m_cqc_right_elbow;
			skill_t m_cqc_right_upward_elbow;
			skill_t m_cqc_left_upward_elbow;
			skill_t m_cqc_right_oblique;
			skill_t m_cqc_left_oblique;
			skill_t m_cqc_left_teep;
			skill_t m_cqc_right_teep;
			skill_t m_cqc_left_front_kick;
			skill_t m_cqc_right_front_kick;
			skill_t m_cqc_left_knee_to_head;
			skill_t m_cqc_right_knee_to_head;
			skill_t m_cqc_left_knee_to_body;
			skill_t m_cqc_right_knee_to_body;
			skill_t m_cqc_knife_disarm;
			skill_t m_cqc_pistol_disarm;
			skill_t m_cqc_right_leg_kick;
			skill_t m_cqc_left_leg_kick;
			skill_t m_cqc_right_kick_to_head;
			skill_t m_cqc_left_kick_to_head;
			skill_t m_cqc_right_stomp_to_head;
			skill_t m_cqc_left_stomp_to_head;
	};
	void unblock_event(uint32_t unblock_event,uuid_t player);
};

#endif
