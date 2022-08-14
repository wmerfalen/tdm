#ifndef __MENTOC_MODS_MOBS_goat_HEADER__
#define  __MENTOC_MODS_MOBS_goat_HEADER__

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
	struct goat : public smart_mob {
			static constexpr mob_vnum MOB_VNUM = 1;
			static constexpr std::string_view BACKPACK_WEAPON = "rifle/goat-scarh.yml";
			static constexpr std::string_view DESCRIPTION =
			    "A cross-eyed goat\r\n"
			    "Loadout:\r\n"
			    "Eyes: armor/night-vision-goggles.yml\r\n"
			    "Head: armor/titanium-mask.yml\r\n"
			    "Body: armor/sleeveless-kevlar-vest.yml\r\n"
			    "Neck: armor/gas-canister-extended-belt.yml\r\n"
			    "Legs: armor/titan-shielding-leg-armor.yml\r\n"
			    "Primary: rifle/scarh.yml\r\n"
			    "Secondary: rifle/scarh.yml\r\n"
			    ;
			static constexpr std::string_view ABILITIES =
			    "Ability 1:\r\n"
			    "-------------------------------------------------------------------\r\n"
			    "[ ] Interdimensional Rift\r\n"
			    " - This ability only triggers if you offend the goat\r\n"
			    " - When threatened, rolls a 2d6. If the result is above 4, a rift in time and space\r\n"
			    "   is opened and any player's in the current room are sucked through.\r\n"
			    " - DURATION: 3 seconds\r\n"
			    " - During duration:\r\n"
			    "    - Player cannot move, or flee\r\n"
			    "\r\n"

			    "Ability 2:\r\n"
			    "-------------------------------------------------------------------\r\n"
			    "[ ] Bleat\r\n"
			    " - Baaa-a-a-a--a-a-aaaatttttt\r\n"
			    " - Rolls a 2d6. If the result is above 4, each player in the room is given\r\n"
			    "   a random stat buff that lasts 1d5 minutes\r\n"
			    "\r\n"

			    "Ability 3:\r\n"
			    "-------------------------------------------------------------------\r\n"
			    "[ ] Graze\r\n"
			    " - Every 60 seconds this ability activates\r\n"
			    " - Rolls a 2d6. If the result is above 9, changes one of the textures in the room\r\n"
			    "\r\n"

			    "Ability 4:\r\n"
			    "-------------------------------------------------------------------\r\n"
			    "[ ] Prayer\r\n"
			    " - The player types 'pray goat'\r\n"
			    " - The goat offers a randomized stat increase\r\n"
			    " - If the player types 'accept goat'\r\n"
			    "   - The goat says 'I want you to kill <N> <NPC>'s'\r\n"
			    "     -- TODO: make this more flowery language\r\n"
			    "   - Where N is a 2d6 roll and NPC is a randomized NPC\r\n"
			    " - Once contract is completed, player gets that randomized stat for\r\n"
			    "   one hour\r\n"
			    "\r\n";

			static constexpr std::array<std::string_view,3> PRAYER_OFFER = {
				"Blaaaa--a-a-aaat-t-tttt-ttaa-aaaaaa-tttt...\r\n"
				"<translation>: For the period of one hour, you will get the following stat increase: +<STAT>\r\n"
				,
				"Blaaa--a-a-a aaattt!! attatat---aa--a-att blaaaa-aaa-aa-attt...\r\n"
				"<translation>: In return, you must kill <N> <NPC>'s.\r\n"
				,
				"Blat... aa-aat-a-a-aa-a-a-tt-att-att... at...\r\n"
				"<translation>: Do you accept?\r\n"
			};

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

			enum wear_slots_t : uint8_t {
				GOAT_WEAR_HORN_LEFT = 0,
				GOAT_WEAR_HORN_RIGHT,
				GOAT_WEAR_FACE,
				GOAT_WEAR_NECK,
				GOAT_WEAR_PRIMARY,
				GOAT_WEAR_SECONDARY,
				GOAT_WEAR_FRONT_LEFT_HOOF,
				GOAT_WEAR_FRONT_RIGHT_HOOF,
				GOAT_WEAR_BACK_LEFT_HOOF,
				GOAT_WEAR_BACK_RIGHT_HOOF,
				GOAT_WEAR_TAIL,
				GOAT_WEAR_BACK,
				/** You can add more here, but don't touch anything below this line */

				/** DO NOT REMOVE THESE \/ \/ */
				__GOAT_WEAR_END,
				__GOAT_WEAR_LENGTH = __GOAT_WEAR_END,
				/** KEEP THESE ^^  */
			};

			enum btree_t : int16_t {
				GOAT_NONE = -1,
				GOAT_ROAM = 0,
				GOAT_GLARE,
				GOAT_GAZE,
				GOAT_HOSTILE,
				//GOAT_DAGGER_ATTACK,
				GOAT_CORPSE_EXPLOSION_ATTACK,
				GOAT_ENSNARE_ATTACK,
				GOAT_DESPERATION_MOVE,
				GOAT_LAST_DITCH_EFFORT_MOVE,


				/** DO NOT REMOVE THESE \/ \/ */
				__GOAT_BTREE_END,
				__GOAT_BTREE_LENGTH = __GOAT_BTREE_END,
				/** KEEP THESE ^^ */
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
			/** START: SIMPLIFIED ATTACK LOOP */
			/**====================================================*/
			//void start_turn();
			//bool find_same_room_targets();
			//void hostile_phase_1();
			//void rifle_attack(const target_t& target);
			//void rct_upkeep();
			//std::optional<target_t> acquire_target();
			//bool find_someone_to_attack();
			//void roam(uint8_t times);

			void roam();
			void glare();
			void graze();

			/**
			 * Generic entry point for running the goat's behaviours.
			 * Goats currently don't have behaviour trees, so whatever you want
			 * the goat to do every goat_pulse() ticks, then do that here
			 */
			void run();

			//std::vector<player_ptr_t> m_same_room_targets;
			//player_ptr_t m_engaged_target;
			//std::vector<target_t> m_targets;
			//std::size_t m_hostile_phase_1_attempts;

			/**====================================================*/
			/** END: SIMPLIFIED ATTACK LOOP */
			/**====================================================*/





			/**====================================================*/
			/** preferred construct method */
			/**====================================================*/
			goat(const uuid_t& mob_uuid,std::string_view variation);
			goat() = delete;
			goat(const goat& other) = delete;
			goat& operator=(const goat&) = delete;
			virtual ~goat() = default;

			/**================*/
			/** debugging info */
			/**================*/
			bool debug_mode_on();
			bool m_debug_mode;
			void mention(std::string_view msg);
			str_map_t report();
			str_map_t usages();
			std::string_view type() {
				return "goat";
			}

			/**=====================================*/
			/** hunting helpers & state maintenance */
			/**=====================================*/
			/** hunting helpers */
			//feedback_t& spray(uint8_t dir);


			///**=================*/
			///** event callbacks */
			///**=================*/
			//void setup_damage_callbacks();
			////void enemy_spotted(room_rnum room,uuid_t player);
			//void found_witness(const mods::scan::vec_player_data_element& data);
			//void found_item(const mods::scan::vec_player_data_element& data);
			//void attacked(const feedback_t& feedback);
			//void door_entry_event(player_ptr_t& player);
			//void found_target(player_ptr_t& player);

			///**===================*/
			///** calculate heading */
			///**===================*/
			//int8_t determine_heading_from_found_victims();
			//void clear_scanned_items();

			///**================*/
			///** combat helpers */
			///**================*/
			//void melee_attack_within_range();
			//void move_closer_to_target();
			//void extra_attack();
			//bool melee_attack(player_ptr_t& victim);

			///** state management */
			//void clear_state();
			//void remember_item(const mods::scan::vec_player_data_element& data);
			//const uuidlist_t& get_remembered_items() const;
			//bool has_found_item();
			//void set_found_item(bool status);
			//std::vector<std::string>& roaming_patterns() const;

			///** rival npc helpers */
			//bool is_rival(player_ptr_t& player);
			//uint8_t scan_depth() const;
			//player_ptr_t spawn_near_someone();
			//void attack(player_ptr_t& player);
			//bool attack_anyone_in_same_room();



			/**===========*/
			/** Abilities */
			/**===========*/
			//void ensnare(player_ptr_t& player);
			//void corpse_explosion_near(player_ptr_t& player);
			//void throw_daggers_toward(player_ptr_t& player);

			///**==============
			// * Weaknesses
			// *===============*/
			//void flash_bang_me();

			void slapped_by(uuid_t player_uuid);

			/**
			 * Area screams
			 */
			void shout_to_area(uint8_t e_msg);

			using weapons_list_t = std::array<obj_ptr_t,4>;
			virtual weapons_list_t& weapons();

			///**
			// * =============================
			// * Telegraphs
			// * -- What is a telegraph?
			// *  In hand to hand combat, a telegraph is a "tell"
			// *  If you "telegraph your punch" it means your body betrays you
			// *  and gives the opponent a hint that you are goign to throw a punch
			// *
			// *==============================
			// */
			//void telegraph_action(uint8_t action);
			//void reset_last_attacker();
			virtual std::pair<bool,std::string> move_to(const direction_t& dir);

			//void telegraph(player_ptr_t& victim,std::string_view saying);
			//virtual void shout(std::string_view message);

			enum status_t : uint8_t {
				IDLING,
				LOOKING_FOR_A_FIGHT,
			};
			virtual bool is(status_t status);

			/**
			 * Scan for targets to attack
			 */
			//void scan_for_targets();
			//void hunt(uuid_t target);
			//void door_entry_event(player_ptr_t& player,const room_rnum room_id);
			//direction_t determine_chase_direction();

			uuid_t uuid;
		private:
			//player_ptr_t get_next_attacking_priority();
			//player_ptr_t m_last_attacker;
			//std::forward_list<player_ptr_t> m_attackers;
			//std::optional<direction_t> m_attackers_last_direction;
			//std::vector<mods::scan::vec_player_data_element> m_scanned_items;
			//uuidlist_t m_remembered_items;
			//std::forward_list<mods::scan::vec_player_data_element> m_hostiles;
			//bool m_found_item;
			std::vector<std::string> m_random_acts;
			std::forward_list<obj_ptr_t> m_backpack;
			weapons_list_t m_weapons;
			//uint8_t m_cant_find;
			status_t m_status;


			//std::vector<std::tuple<uuid_t,direction_t,uint8_t>> m_scanned_targets;
			//uuid_t m_hunt;
			//std::shared_ptr<mods::ranged_combat_totals> RCT;
			//bool m_watching_everywhere;
	};

	static inline const uint16_t& goat_pulse() {
		static constexpr const uint16_t PULSE = 33;
		return PULSE;
	}
	extern std::vector<std::shared_ptr<goat>> goat_list;
	namespace goat_callbacks {
		bool dispatch_watcher(const uuid_t& orthos_agent_uuid,player_ptr_t& player, const room_rnum& room_id);
	};
	void process_goat_slap(player_ptr_t& attacker,player_ptr_t& victim);
	void run_goat_pulse();
};
#endif
