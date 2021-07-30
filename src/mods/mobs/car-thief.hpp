#ifndef __MENTOC_MODS_MOBS_car_thief_HEADER__
#define  __MENTOC_MODS_MOBS_car_thief_HEADER__

#include "../../globals.hpp"
#include "../damage-event.hpp"
#include "../npc.hpp"
#include "smart-mob.hpp"
#include <stack>
#include <forward_list>
#include "../scan.hpp"

namespace mods::mobs {
	using tick_t = uint64_t;
	struct car_thief : public smart_mob {
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
			car_thief(uuid_t mob_uuid,std::string variation);
			car_thief() = delete;
			virtual ~car_thief() = default;

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

			/**=================*/
			/** event callbacks */
			/**=================*/
			void setup_damage_callbacks();
			void enemy_spotted(room_rnum room,uuid_t player);
			void found_witness(const mods::scan::vec_player_data_element& data);
			void found_vehicle(const mods::scan::vec_player_data_element& data);
			void attacked(const feedback_t& feedback);
			void door_entry_event(player_ptr_t& player);

			/**===================*/
			/** calculate heading */
			/**===================*/
			int8_t determine_heading_from_found_vehicles();
			void clear_scanned_cars();

			/**================*/
			/** combat helpers */
			/**================*/
			void melee_attack_within_range();
			void move_closer_to_target();

			/** state management */
			void remember_car(const mods::scan::vec_player_data_element& data);
			const uuidlist_t& get_remembered_cars() const;
			bool has_found_car();
			void set_found_car(bool status);


			/** rival npc helpers */
			bool is_rival(player_ptr_t& player);


			std::string_view type() {
				return "car_thief";
			}
		private:
			player_ptr_t get_next_attacking_priority();
			player_ptr_t m_last_attacker;
			std::forward_list<player_ptr_t> m_attackers;
			obj_ptr_t m_weapon;
			std::optional<direction_t> m_attackers_last_direction;
			std::vector<mods::scan::vec_player_data_element> m_scanned_cars;
			uuidlist_t m_remembered_cars;
			std::forward_list<mods::scan::vec_player_data_element> m_hostiles;
			bool m_found_car;
	};
	using car_thief_map_t = std::map<uuid_t,std::shared_ptr<car_thief>>;
	car_thief_map_t& car_thief_map();

};

static inline bool has_car_thief_ptr(const uuid_t& u) {
	return mods::mobs::car_thief_map().find(u) != mods::mobs::car_thief_map().end();
}

static inline std::shared_ptr<mods::mobs::car_thief> car_thief_ptr(const uuid_t& u) {
	return mods::mobs::car_thief_map()[u];
}
#endif
