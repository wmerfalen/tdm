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
			static constexpr float AWFUL = 50.0;
			static constexpr float OKAY = 250.0;
			static constexpr float LEARNED = 1000.0;
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
	};
	void unblock_event(uint32_t unblock_event,uuid_t player);
};

#endif
