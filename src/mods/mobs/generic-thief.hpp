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
			using uuidlist_t = std::forward_list<uuid_t>;
			static constexpr uint16_t SHOULD_DO_ROAM = 0;
			static constexpr uint16_t SHOULD_DO_RANDOM_TRIVIAL = 1;
			/**====================================================*/
			/** factory and destruct */
			/**====================================================*/
			static void create(uuid_t uuid, std::string);
			static void free_mob(uuid_t uuid);

			/**====================================================*/
			/** initialization routine called by constructor */
			/** can be called anytime after construct if needed */
			/**====================================================*/
			void init();
			void set_variation(std::string);

			/**====================================================*/
			/** preferred construct method */
			/**====================================================*/
			generic_thief(uuid_t mob_uuid,std::string variation);
			generic_thief() = delete;
			virtual ~generic_thief() = default;

			/**================*/
			/** debugging info */
			/**================*/
			str_map_t report();

			/**=====================================*/
			/** hunting helpers & state maintenance */
			/**=====================================*/
			/** hunting helpers */
			feedback_t& spray(uint8_t dir);

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

			/**================*/
			/** combat helpers */
			/**================*/
			void melee_attack_within_range();
			void move_closer_to_target();

			/** state management */
			void remember_item(const mods::scan::vec_player_data_element& data);
			const uuidlist_t& get_remembered_items() const;
			bool has_found_item();
			void set_found_item(bool status);


			/** rival npc helpers */
			bool is_rival(player_ptr_t& player);


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
	};
	using generic_thief_map_t = std::map<uuid_t,std::shared_ptr<generic_thief>>;
	generic_thief_map_t& generic_thief_map();

};

static inline bool has_generic_thief_ptr(const uuid_t& u) {
	return mods::mobs::generic_thief_map().find(u) != mods::mobs::generic_thief_map().end();
}

static inline std::shared_ptr<mods::mobs::generic_thief> generic_thief_ptr(const uuid_t& u) {
	return mods::mobs::generic_thief_map()[u];
}
#endif
