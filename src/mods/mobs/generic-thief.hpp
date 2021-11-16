#ifndef __MENTOC_MODS_MOBS_generic_thief_HEADER__
#define  __MENTOC_MODS_MOBS_generic_thief_HEADER__

#include "../../globals.hpp"
#include "../damage-event.hpp"
#include "../npc.hpp"
#include "smart-mob.hpp"
#include <stack>
#include <forward_list>
#include "../scan.hpp"

namespace mods::mobs {
	using tick_t = uint64_t;
	struct generic_thief : public smart_mob {
			enum btree_t : int16_t {
				GT_NONE = -1,
				GT_ROAM = 0,
				GT_HOSTILE,
				GT_WITNESS_HUNTING,
				GT_WIMPY,
				GT_ATTEMPT_THIEVERY
			};
			enum target_t : uint8_t {
				NONE =0,
				HOUSEHOLDS,
				PARKS,
				CARS,
				KIDS,
				DARK_ROOM,
				DRUGS,
				WEAPONS,
				UTILITY_STORES,
				CHURCH,
			};
			static inline target_t str_to_target(std::string_view s) {
				if(s.compare("CHURCH") == 0) {
					return target_t::CHURCH;
				}
				if(s.compare("HOUSEHOLDS") == 0) {
					return target_t::HOUSEHOLDS;
				}
				if(s.compare("PARKS") == 0) {
					return target_t::PARKS;
				}
				if(s.compare("CARS") == 0) {
					return target_t::CARS;
				}
				if(s.compare("KIDS") == 0) {
					return target_t::KIDS;
				}
				if(s.compare("DARK_ROOM") == 0) {
					return target_t::DARK_ROOM;
				}
				if(s.compare("DRUGS") == 0) {
					return target_t::DRUGS;
				}
				if(s.compare("WEAPONS") == 0) {
					return target_t::WEAPONS;
				}
				if(s.compare("UTILITY_STORES") == 0) {
					return target_t::UTILITY_STORES;
				}
				return target_t::NONE;
			}
			using uuidlist_t = std::forward_list<uuid_t>;
			static constexpr uint16_t SHOULD_DO_ROAM = 0;
			static constexpr uint16_t SHOULD_DO_RANDOM_TRIVIAL = 1;
			/**====================================================*/
			/** factory and destruct */
			/**====================================================*/
			static void create(const uuid_t& uuid, std::string_view);
			static void free_mob(const uuid_t& uuid);

			/**====================================================*/
			/** initialization routine called by constructor */
			/** can be called anytime after construct if needed */
			/**====================================================*/
			void init();
			void set_variation(const std::string&);
			void set_behavior_tree_directly(const btree_t& t);
			bool has_tree();
			btree_t get_tree();

			/**====================================================*/
			/** preferred construct method */
			/**====================================================*/
			generic_thief(const uuid_t& mob_uuid,std::string_view variation);
			generic_thief() = delete;
			virtual ~generic_thief() = default;

			/**================*/
			/** debugging info */
			/**================*/
			str_map_t report();
			str_map_t usages();

			/**=====================================*/
			/** hunting helpers & state maintenance */
			/**=====================================*/
			/** hunting helpers */
			feedback_t& spray(uint8_t dir);
			void snipe(player_ptr_t victim,direction_t direction,uint8_t distance);
			void melee_attack(player_ptr_t victim);


			/** behaviour tree helpers */
			void btree_none();
			void btree_roam();
			void btree_hostile();
			void btree_witness_hunting();
			void btree_wimpy();
			void btree_attempt_thievery();

			/**=================*/
			/** event callbacks */
			/**=================*/
			void setup_damage_callbacks();
			void enemy_spotted(room_rnum room,uuid_t player);
			void found_witness(const mods::scan::vec_player_data_element& data);
			void found_item(const mods::scan::vec_player_data_element& data);
			void attacked(const feedback_t& feedback);
			void door_entry_event(player_ptr_t& player);

			/**===================*/
			/** calculate heading */
			/**===================*/
			int8_t determine_heading_from_found_vehicles();
			void clear_scanned_items();
			void increment_last_attack_significance();

			/**================*/
			/** combat helpers */
			/**================*/
			bool rifle_attack_within_range();
			bool melee_attack_within_range();
			int8_t move_closer_to_target();

			/** state management */
			void remember_item(const mods::scan::vec_player_data_element& data);
			const uuidlist_t& get_remembered_items() const;
			bool has_found_item();
			void set_found_item(bool status);

			/**===================*/
			/** targets/interests */
			/**===================*/
			std::forward_list<target_t>& parse_targets(std::string_view targets);
			std::forward_list<target_t>& get_targets();

			/** rival npc helpers */
			bool is_rival(player_ptr_t& player);

			std::string_view type() {
				return "generic_thief";
			}

			std::string summarize(bool regen);

			virtual bool should_steal_from(player_ptr_t& victim);
			virtual bool roll_steal_success(player_ptr_t& victim);


		private:
			std::forward_list<target_t> m_targets;
			player_ptr_t get_next_attacking_priority();
			player_ptr_t m_last_attacker;
			std::forward_list<player_ptr_t> m_attackers;
			obj_ptr_t m_weapon;
			std::optional<direction_t> m_attackers_last_direction;
			std::vector<mods::scan::vec_player_data_element> m_scanned_items;
			uuidlist_t m_remembered_items;
			std::forward_list<mods::scan::vec_player_data_element> m_hostiles;
			bool m_found_item;
	};

	std::forward_list<std::shared_ptr<generic_thief>>& generic_thief_list();
};

#if 0
static inline bool has_generic_thief_ptr(const uuid_t& u) {
	return mods::mobs::generic_thief_map().find(u) != mods::mobs::generic_thief_map().end();
}

static inline std::shared_ptr<mods::mobs::generic_thief> generic_thief_ptr(const uuid_t& u) {
	return mods::mobs::generic_thief_map()[u];
}
#endif
#endif
