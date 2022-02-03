#ifndef __MENTOC_MODS_MOBS_SMART_MOB_HEADER__
#define  __MENTOC_MODS_MOBS_SMART_MOB_HEADER__

#include "../../globals.hpp"
#include "../damage-event.hpp"
#include "../npc.hpp"
#include "../scan.hpp"
namespace mods::memory {
	extern void save_footprint(std::string_view type,uuid_t& uuid,str_map_t m);
};

namespace mods::mobs {
	using tick_t = uint64_t;
	struct smart_mob {
		enum weight_index_t : uint8_t {
			WEIGHT_FAST,
			WEIGHT_NORMAL,
			WEIGHT_ENCUMBERED,
		};
			/**====================================================*/
			/** factory and destruct */
			/**====================================================*/
			static void create(uuid_t uuid, std::string);
			static void free_mob(uuid_t uuid);

			/**====================================================*/
			/** initialization routine called by constructor */
			/** can be called anytime after construct if needed */
			/**====================================================*/
			virtual void init();

			/**====================================================*/
			/** preferred construct method */
			/**====================================================*/
			smart_mob(uuid_t mob_uuid,std::string variation);
			smart_mob();
			virtual ~smart_mob();

			/**====================================================*/
			/** used to specify which variation of the mini gunner */
			/** npc type we should use. i.e.: "sentinel"           */
			/**====================================================*/
			virtual void set_variation(std::string);

			uuid_t uuid;
			/**====================================================*/
			/** motion control */
			/**====================================================*/
			void set_heading(const int8_t& dir);
			int8_t get_heading() const {
				return heading;
			}
			void shout(std::string_view);
			void set_behaviour_tree(std::string_view name);
			void set_behaviour_tree_directly(const uint16_t& tree);
			bool has_tree();
			uint16_t get_tree();
			void populate_from_meqbuild_profile();
			virtual void bootstrap_equipment();

			/**================*/
			/** debugging info */
			/**================*/
			virtual str_map_t report() = 0;
			virtual str_map_t usages() {
				return {};
			}

			/**=====================================*/
			/** hunting helpers & state maintenance */
			/**=====================================*/
			int get_weapon_heat() const {
				return weapon_heat;
			}
			room_rnum room();
			char_data* cd();
			/** hunting helpers */
			void watch_nothing();
			void watch_heading();
			void watch(uint8_t direction);
			void watch_directions(vec_t<uint8_t> direction);
			void set_hunting(const uuidvec_t&);
			uuidvec_t& get_hunting();
			tick_t get_last_seen(uuid_t player);
			tick_t get_last_seen_diff(uuid_t player);
			void forget(uuid_t);
			player_ptr_t& player() {
				return player_ptr;
			}
			void save_targets(vec_t<uuid_t>&);
			void save_hostile_targets(vec_t<uuid_t>&);
			const vec_t<uuid_t> get_hostile_targets() const;

			void hunt_hostile_targets();

			/**=================*/
			/** event callbacks */
			/**=================*/
			virtual void setup_damage_callbacks();
			virtual void enemy_spotted(room_rnum room,uuid_t player);

			void set_watching_room(bool status);
			bool watching_room();
			/**====================================================*/
			/** weapon helpers */
			/**====================================================*/
			obj_ptr_t primary();
			void wear(uint8_t where,std::string_view yaml);
			int8_t wear_all();
			bool should_do(uint16_t action) {
				++m_should_do[action];
				if(m_should_do[action] >= m_should_do_max[action]) {
					m_should_do[action] = 0;
					return true;
				}
				return false;
			}
			virtual std::pair<bool,std::string> move_to(const direction_t& dir);
			virtual std::pair<bool,std::string> move_to_on_tick(const direction_t& dir,const uint16_t tick_mod);
			void* extended_type() {
				return m_extended_type;
			}
			void set_extended_type(void* type) {
				m_extended_type = type;
			}
			void act_to_room(std::string_view msg);
			virtual uint8_t scan_depth() const;
			mods::scan::vec_player_data scan_attackable(const direction_t& dir);
			std::vector<direction_t> shuffle_directions();
			bool alive();
			bool can_move();
			bool capable();
			virtual std::string_view type() {
				return "smart-mob";
			}
			str_map_t base_usages();
			template <typename T>
			static inline void clear_list_if_count(T* list,std::size_t size) {
				if(std::distance(list->cbegin(),list->cend()) > size) {
					list->clear();
				}
			}
			bool is_fighting() {
				return player_ptr->char_specials().fighting != nullptr;
			}

			void refill_ammo() {
				player_ptr->primary()->rifle_instance->ammo = 255;
			}
			int optimal_range() const;

			void save_attack_direction(const direction_t& direction);
			direction_t determine_chase_direction();

			/**
			 * TODO: effective_weight_index();
			 */
			weight_index_t effective_weight_index();
		protected:
			direction_t m_last_attack_direction;
			std::map<uint16_t,uint16_t> m_should_do;
			std::map<uint16_t,uint16_t> m_should_do_max;
			std::array<std::string,NUM_WEARS> wear_list;
			vec_t<uuid_t> hostile_targets;
			uint8_t watching;
			std::map<uuid_t,uint64_t> last_seen;
			uuidvec_t hunting;
			std::string variation;
			int weapon_heat;
			int8_t heading;
			uint8_t spray_direction;
			player_ptr_t player_ptr;
			bool loaded;
			bool error;
			feedback_t last_attack;
			vec_t<uuid_t> targeting;
			void* m_extended_type;
	};
};
#endif
