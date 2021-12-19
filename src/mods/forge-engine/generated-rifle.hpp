#ifndef __MENTOC_MODS_FORGE_ENGINE_GENERATED_RIFLE_HEADER__
#define __MENTOC_MODS_FORGE_ENGINE_GENERATED_RIFLE_HEADER__
#include "../../structs.h"
#include "../pq.hpp"
#include "../sql.hpp"
#include "value-scaler.hpp"
#include "requirements.hpp"
#include "generator.hpp"
#include "../orm/admin/rifle-attribute-limits.hpp"
#include <functional>

#ifdef __MENTOC_SHOW_MODS_FORGE_ENGINE_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::forge_engine::generated_rifle]")  << MSG << "\n";
#else
#define m_debug(MSG) ;;
#endif


namespace mods::forge_engine {
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	void rifle_index_changed();
	using kill_t = mods::forge_engine::kill_t;

	using attributes_t = std::vector<std::pair<rifle_attributes_t,std::variant<uint32_t,float>>>;
	struct attribute_limits_t {
		rifle_attributes_t attribute;
		int low;
		int high;
		int overpowered;
	};
	using attribute_limits_orm_t = mods::orm::admin::rifle_attribute_limits;

	struct generated_rifle_t {
			using attr_pair_t = std::pair<rifle_attributes_t,std::variant<uint32_t,float>>;
			generated_rifle_t() = delete;
			generated_rifle_t (kill_t& player);
			generated_rifle_t (kill_t& player,std::string_view type);
			obj_ptr_t roll();
			void send_stats_to_player(player_ptr_t& player);
			std::string get_dump();
			void fill(obj_ptr_t& rifle);
			void fill_attributes(obj_ptr_t& rifle);
			void fill_elemental(obj_ptr_t& rifle);
			void fill_stats(obj_ptr_t& rifle);
			const std::vector<std::string>& yaml_list(rifle_types_t);
			void load_from_sql();
			attributes_t generate_rifle_attributes();
			attribute_limits_t fetch_limits(const rifle_attributes_t& which);
			void init();
			std::pair<bool,std::variant<uint32_t,float>> get_attribute(rifle_attributes_t attr) {
				for(auto p : m_attributes) {
					if(p.first == attr) {
						return {true,p.second};
					}
				}
				return {false,(uint32_t)0};
			}


		private:
			bool m_force_type;
			kill_t m_player;
			rifle_types_t m_type;
			requirements_t m_requirements;
			std::vector<std::pair<rifle_attributes_t,std::variant<uint32_t,float>>> m_attributes;
			std::vector<std::pair<elemental_types_t,std::variant<uint32_t,float>>> m_elemental_damages;
			std::vector<std::pair<stat_types_t,std::variant<uint32_t,float>>> m_stat_boosts;
			obj_ptr_t m_instance;
			bool compute_weapon_cost();
	};

	enum test_type_t {
		TEST_GREATER_THAN,
		TEST_LESS_THAN,
		TEST_GTE,
		TEST_LTE,
		TEST_EQUAL,

	};

	/**
	 * [ balancing ]
	 * [ RPM - balances - DMG ]
	 * ------------------------
	 * If cooldown between shots less than (high - low)
	 * 	THEN
	 * 	dmg dice count/sides should be nerfed
	 * 	base damage should be nerfed
	 */

	/**
	 * [ balancing ]
	 * [ CRIT % - balances - CBS ]
	 * If critical chance >= high
	 * 	THEN
	 *  cooldown between shots needs to be higher
	 */
	struct balancer_t {
		rifle_attributes_t attribute;
		test_type_t test;
		int value;
	};

	struct balance_ruleset_t {
			/**
			 * How to use this class.
			 * Load it up with tests and logic using the two parameter
			 * constructor. Then when you want to test a rifle, use
			 * the run(rifle*) function.
			 * If you do this correctly, and some(or all) of your tests pass,
			 * then your logic functions should be ran until either one of
			 * the following happens:
			 * 1) we reach the end of the logic functions you passed
			 * or
			 * 2) one of your logic functions returns false signalling to
			 * not call the next logic function in the deque
			 */
			using fixer_function_t = std::function<bool(void*,void*,void*)>;
			using test_list_t = std::deque<balancer_t>;
			using logic_list_t = std::deque<fixer_function_t>;
			balance_ruleset_t(test_list_t t, logic_list_t l) :
				tests(t), logic(l), rifle(nullptr)
			{}


			balance_ruleset_t() {
				this->__init();
			}
			~balance_ruleset_t() {
				tests.clear();
				logic.clear();
				rifle = nullptr;
			}
			void run(generated_rifle_t* r) {
				this->rifle = r;
				for(const auto& t : tests) {
					auto pair = this->rifle->get_attribute(t.attribute);
					if(pair.first == false) {
						continue;
					}
					auto vari = pair.second;
					int attr = 0;
					if(std::holds_alternative<uint32_t>(vari)) {
						attr = std::get<uint32_t>(vari);
					} else {
						attr = (int)std::get<float>(vari);
					}
					bool passed = false;
					switch(t.test) {
						case test_type_t::TEST_GREATER_THAN:
							passed = attr > t.value;
							break;
						case test_type_t::TEST_LESS_THAN:
							passed = attr < t.value;
							break;
						case test_type_t::TEST_GTE:
							passed = attr >= t.value;
							break;
						case test_type_t::TEST_LTE:
							passed = attr <= t.value;
							break;
						case test_type_t::TEST_EQUAL:
							passed = attr == t.value;
							break;
						default:
							break;
					}
					if(passed) {
						this->test_passed(t);
					}
					passed = false;
				}
				run_logic();
				rifle = nullptr;
			}
		private:
			test_list_t tests;
			test_list_t passed_tests;
			logic_list_t logic;
			generated_rifle_t* rifle;
			void __init() {
				tests.clear();
				logic.clear();
				rifle = nullptr;
			}
			void run_logic() {
				if(!rifle) {
					std::cerr << "rifle is a nullptr! --> " <<  __FUNCTION__ << "\n";
					return;
				}
				for(const auto func : logic) {
					if(!func((void*)&passed_tests,(void*)rifle,(void*)this)) {
						return;
					}
				}
			}
			void test_passed(const balancer_t& test) {
				passed_tests.emplace_back(test);
			}
	};//end balance_ruleset_t

	static inline void bal_loaders() {

	}

	/**
	 * A "weapon shaper" is a device in which we balance a
	 * weapon by it's weapon type to act within the limits
	 * of a predictable weapon of that type.
	 * This device is a molder, a limiter of sorts. The
	 * attributes of weapon are tamed so that a shotgun
	 * isn't doing insane damage at the range of a sniper
	 * rifle.
	 */
	struct weapon_shaper_t {
		/** Test if these attributes are high */
		std::vector<balancer_t> balancer_functions;
		rifle_attributes_t target_set;
		std::function<void(void*,balancer_t*)> balancer;
	};
	/*
	RIFLE_TYPE_SHOTGUN,
	RIFLE_TYPE_ASSAULT_RIFLE,
	RIFLE_TYPE_SUB_MACHINE_GUN,
	RIFLE_TYPE_SNIPER,
	RIFLE_TYPE_HANDGUN,
	RIFLE_TYPE_PISTOL,
	RIFLE_TYPE_MACHINE_PISTOL,
	RIFLE_TYPE_LIGHT_MACHINE_GUN,
	*/

};

#undef m_debug
#endif
