#ifndef __MENTOC_MODS_MOBS_defiler_HEADER__
#define  __MENTOC_MODS_MOBS_defiler_HEADER__

#include "../../globals.hpp"
#include "../damage-event.hpp"
#include "../npc.hpp"
#include "smart-mob.hpp"
#include <stack>
#include <forward_list>
#include "../scan.hpp"

namespace mods::mobs {
	using tick_t = uint64_t;
	struct defiler : public smart_mob {
			enum btree_t : int16_t {
				SHOPL_NONE = -1,
				SHOPL_ROAM = 0,
				SHOPL_HOSTILE,
			};
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
			defiler(const uuid_t& mob_uuid,std::string_view variation);
			defiler() = delete;
			virtual ~defiler() = default;

			/**================*/
			/** debugging info */
			/**================*/
			str_map_t report();
			str_map_t usages();
			std::string_view type() {
				return "defiler";
			}

			/**=====================================*/
			/** hunting helpers & state maintenance */
			/**=====================================*/
			/** hunting helpers */
			feedback_t& spray(uint8_t dir);


			/** behaviour tree helpers */
			void btree_none();
			void btree_roam();
			void btree_hostile();
			void perform_random_act();

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
			int8_t determine_heading_from_found_victims();
			void clear_scanned_items();

			/**================*/
			/** combat helpers */
			/**================*/
			void melee_attack_within_range();
			void move_closer_to_target();
			void extra_attack();

			/** state management */
			void clear_state();
			void remember_item(const mods::scan::vec_player_data_element& data);
			const uuidlist_t& get_remembered_items() const;
			bool has_found_item();
			void set_found_item(bool status);

			/** rival npc helpers */
			bool is_rival(player_ptr_t& player);
			uint8_t scan_depth() const;
			player_ptr_t spawn_near_someone();
			void attack(player_ptr_t& player);
			bool attack_anyone_in_same_room();


		private:
			player_ptr_t get_next_attacking_priority();
			player_ptr_t m_last_attacker;
			std::forward_list<player_ptr_t> m_attackers;
			obj_ptr_t m_weapon;
			std::optional<direction_t> m_attackers_last_direction;
			std::vector<mods::scan::vec_player_data_element> m_scanned_items;
			uuidlist_t m_remembered_items;
			std::forward_list<mods::scan::vec_player_data_element> m_hostiles;
			bool m_found_item;
			std::vector<std::string> m_random_acts;
	};

	std::forward_list<std::shared_ptr<defiler>>& defiler_list();
};
#endif
