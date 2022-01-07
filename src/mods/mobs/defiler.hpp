#ifndef __MENTOC_MODS_MOBS_defiler_HEADER__
#define  __MENTOC_MODS_MOBS_defiler_HEADER__

#include "../../globals.hpp"
#include "../damage-event.hpp"
#include "../npc.hpp"
#include "smart-mob.hpp"
#include <stack>
#include <forward_list>
#include "../scan.hpp"
#include "resistance.hpp"

namespace mods::mobs {
	using tick_t = uint64_t;
	struct defiler : public smart_mob {
			static constexpr mob_vnum MOB_VNUM = 666;
			/**
			 * Brainstorming some ideas for the DEFILER
			 *
			 * - Perhaps this could be a pure melee boss?
			 *   - Throwing daggers from far off
			 *   - Uses corpse explosion
			 *   - Uses Chainsaw to attack
			 *   - Uses barbed wire to ensnare you
			 * - Weaknesses
			 *   - Uses NIGHT VISION goggles, so he can see you,
			 *   but is susceptible to flashbangs or rooms that
			 *   are on fire
			 * - Weapons
			 *   - Radioactive Daggers (THROWING)
			 *   - Butcher Knife (MELEE)
			 *   - Barbed wire (ensnaring)
			 *
			 * - Resistances
			 *   - BLEED
			 *   - SHRAPNEL
			 * -
			 *
			 */
			enum telegraph_action_type : uint8_t {
				ENSNARING_YOU,
				CORPSE_EXPLOSION,
				DAGGER_THROW,
				CHAINSAW_ATTACK,
			};
			static constexpr std::string_view BACKPACK_WEAPON = "rifle/defiler-scarh.yml";
			static constexpr std::string_view DESCRIPTION =
			    "The DEFILER\r\n"
			    "Loadout:\r\n"
			    "Eyes: armor/night-vision-goggles.yml\r\n"
			    "Head: armor/titanium-mask.yml\r\n"
			    "Body: armor/sleeveless-kevlar-vest.yml\r\n"
			    "Belt: armor/gas-canister-extended-belt.yml\r\n"
			    "Legs: armor/titan-shielding-leg-armor.yml\r\n"
			    "Primary: rifle/scarh.yml\r\n"
			    "Seconary: melee/shock-naginata.yml\r\n"
			    ;
			static constexpr std::string_view ABILITIES =
			    "Ability 1:\r\n"
			    "-------------------------------------------------------------------\r\n"
			    "[ ] Ensnare target\r\n"
			    " - Throws an impact grenade that causes a quick hardening substance to wrap\r\n"
			    "   the target's feet.\r\n"
			    " - DURATION: 3 seconds\r\n"
			    " - During duration:\r\n"
			    "    - Player cannot move, or flee\r\n"
			    "    - Player can snipe\r\n"
			    "    - Player cannot use explosive devices\r\n"
			    "Ability 2:\r\n"
			    "-------------------------------------------------------------------\r\n"
			    "[ ] Corpse explosion\r\n"
			    " - Fires a projectile which causes a corpse to explode on impact\r\n"
			    "";
			static constexpr std::string_view PHASES =
			    "PHASES\r\n"
			    "-------------------------------------------------------------------\r\n"

			    "CORROSION WEAKNESS PHASE\r\n"
			    " [ ] Defiler is susceptible to corrosive attacks.\r\n"
			    " -- Corrosion will cause his armor to be less reliable\r\n"
			    " -- At 75 percent health, his boots and shin guards will be completely deteriorated\r\n"
			    "    from the corrosive effects.\r\n"

			    "GLASS SHARDS WEARKNESS PHASE\r\n"
			    " [ ] Defiler, having had his boots and shin guards made useless, will now be susceptible to\r\n"
			    "     BLEED attacks. Particularly with broken glass and shrapnel.\r\n"
			    "  -- This will cause blood to give away his location\r\n"

			    "CORPSE EXPLOSION PHASE\r\n"
			    " [ ] You'll notice that every room has a corpse in it. This is on purpose.\r\n"
			    "     Defiler will detonate these corpses when you are near them.\r\n"

			    "ENSNARING PHASE\r\n"
			    " [ ] Defiler, upon finding you, will use the Roots of Mayhem skill which will ensnare the player\r\n"
			    "  -- Player cannot move for 3 seconds.\r\n"
			    "  -- Player can snipe and use any ranged attacks\r\n"
			    "  -- Player will have -50 movement points once ensnare is done\r\n"

			    "NIGHT VISION PHASE\r\n"
			    " [ ] Defiler will use his night vision goggles and turn off all lights.\r\n"
			    "  - Counter this with:\r\n"
			    "  	- [ ] Igniting fires in each room by destroying the bottles of alcohol and setting fire to them.\r\n"

			    "FLASHBANG WEAKNESS PHASE\r\n"
			    " [ ] Defiler uses night vision goggles at all times.\r\n"
			    "  - A flashbang grenade will blind him for twice as many ticks as normal.\r\n"

			    "DAGGER PHASE\r\n"
			    " [ ] Defiler can throw daggers extremely far distances.\r\n"
			    "  - When the player gets hit, they must type 'pull dagger' to remove it\r\n"
			    "   - Player endures bleed for N ticks according to roll of dice\r\n"
			    "\r\n"

			    "";
			static constexpr std::string_view WEAKNESSES =
			    "Weakness 1:\r\n"
			    "-------------------------------------------------------------------\r\n"
			    "[ ] Flashbangs cause Defiler to have prolonged blindness\r\n"
			    " - The Defiler must wear the night vision goggles attached to his eyes\r\n"
			    " - This means that any bright flashes of light will be amplified and blindness will ensue\r\n"
			    "\r\n"
			    "Weakness 2:\r\n"
			    "-------------------------------------------------------------------\r\n"
			    "[ ] Corrosive damage can reduce the effectiveness of his armor.\r\n"
			    "\r\n"
			    "Weakness 3:\r\n"
			    "-------------------------------------------------------------------\r\n"
			    "[ ] The Defiler is very susceptible to bleeding.\r\n"
			    " - The Defailer has an odd genetic condition that makes him susceptible to bleed out if\r\n"
			    "   a large enough cut were made.\r\n"
			    " - In addition, Shrapnel can cause the Defailer to bleed profusely.\r\n"
			    "\r\n"
			    "Other notes:\r\n"
			    "-------------------------------------------------------------------\r\n"
			    "\r\n"
			    "It is possible to beat the Defiler by:\r\n"
			    " - corrosive shotguns.\r\n"
			    " - Shrapnel or Corrosive claymore mines\r\n"
			    " - Shrapnel or Corrosive grenades\r\n"
			    "";

			enum btree_t : int16_t {
				DEFILER_NONE = -1,
				DEFILER_ROAM = 0,
				DEFILER_HOSTILE,
				//DEFILER_DAGGER_ATTACK,
				DEFILER_CORPSE_EXPLOSION_ATTACK,
				DEFILER_ENSNARE_ATTACK,
				DEFILER_DESPERATION_MOVE,
				DEFILER_LAST_DITCH_EFFORT_MOVE,
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


			/**
			 * When certain criteria is met, the defiler will respond accordingly.
			 * These functions help keep track of what that criteria is
			 */

			/**
			 * Model the behaviour of the defiler slightly after the
			 * first boss in nuclear throne.
			 *
			 * [hostile]
			 */
			void hostile_phase_1();

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
			void found_target(player_ptr_t& player);

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
			bool melee_attack(player_ptr_t& victim);

			/** state management */
			void clear_state();
			void remember_item(const mods::scan::vec_player_data_element& data);
			const uuidlist_t& get_remembered_items() const;
			bool has_found_item();
			void set_found_item(bool status);
			std::vector<std::string>& roaming_patterns() const;

			/** rival npc helpers */
			bool is_rival(player_ptr_t& player);
			uint8_t scan_depth() const;
			player_ptr_t spawn_near_someone();
			void attack(player_ptr_t& player);
			bool attack_anyone_in_same_room();



			/**===========*/
			/** Abilities */
			/**===========*/
			void ensnare(player_ptr_t& player);
			void corpse_explosion_near(player_ptr_t& player);
			void throw_daggers_toward(player_ptr_t& player);

			/**==============
			 * Weaknesses
			 *===============*/
			void flash_bang_me();

			/**
			 * Area screams
			 */
			void shout_to_area(uint8_t e_msg);

			using weapons_list_t = std::array<obj_ptr_t,4>;
			virtual weapons_list_t& weapons();

			/**
			 * =============================
			 * Telegraphs
			 * -- What is a telegraph?
			 *  In hand to hand combat, a telegraph is a "tell"
			 *  If you "telegraph your punch" it means your body betrays you
			 *  and gives the opponent a hint that you are goign to throw a punch
			 *
			 *==============================
			 */
			void telegraph_action(uint8_t action);
			void reset_last_attacker();
			std::pair<bool,std::string> move_to(const direction_t& dir) override;

			void telegraph(player_ptr_t& victim,std::string_view saying);
			void shout(std::string_view message);

			enum status_t : uint8_t {
				IDLING,
				LOOKING_FOR_A_FIGHT,
			};
			bool is(status_t status);

			/**
			 * Scan for targets to attack
			 */
			void scan_for_targets();
			void hunt(uuid_t target);
			void door_entry_event(player_ptr_t& player,const room_rnum room_id);

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
			std::forward_list<obj_ptr_t> m_backpack;
			weapons_list_t m_weapons;
			uint8_t m_cant_find;
			status_t m_status;


			std::vector<std::tuple<uuid_t,direction_t,uint8_t>> m_scanned_targets;
			uuid_t m_hunt;
			std::shared_ptr<mods::ranged_combat_totals> RCT;
			bool m_watching_everywhere;
	};

	std::forward_list<std::shared_ptr<defiler>>& defiler_list();
	namespace defiler_callbacks {
		bool dispatch_watcher(const uuid_t& orthos_agent_uuid,player_ptr_t& player, const room_rnum& room_id);
	};
};
#endif
