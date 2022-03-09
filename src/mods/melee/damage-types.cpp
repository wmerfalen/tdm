#include "damage-types.hpp"
#include "../../spells.h"
#include "../object-utils.hpp"
#include "../injure.hpp"
#include "../rooms.hpp"
#include "../skills.hpp"
#include "../projectile.hpp"
#include "../levels.hpp"
#include "../interpreter-include.hpp"
//#include "damage-calculator.hpp"
#include "../mobs/damage-event.hpp"
#include "../calc-visibility.hpp"
#include "../loot.hpp"
#include "../weapons/damage-types.hpp"
#include "../players/messages.hpp"


#include <variant>
//#include "damage-event-broadcaster.hpp"
//#include "damage-decisions.hpp"
//#include "../classes/ghost.hpp"
//#include "../classes/breacher.hpp"
//#include "../classes/marine.hpp"
//#include "../corrosive.hpp"

#ifdef __MENTOC_DAMAGE_TYPES_DEBUG_OUTPUT__
#define dty_debug(a) std::cerr << "[mods::melee::damage_types][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#else
#define dty_debug(a)
#endif

#ifdef __MENTOC_SHOW_DAMAGE_TYPES_DEBUG__
#define md(A) std::cerr << green_str("damage_types_debug:") << A << "\n";
#else
#define md(A)
#endif

#ifndef TO_ROOM
#define TO_ROOM		1
#endif

extern void extract_char_final(char_data *ch);
extern void make_corpse(char_data *ch);
extern void death_cry(char_data *ch);
extern void change_alignment(char_data *ch, char_data *victim);
extern void obj_to_char(obj_ptr_t  object, player_ptr_t player);
extern int ok_damage_shopkeeper(char_data *ch, char_data *victim);
extern void set_fighting(char_data *ch, char_data *vict);
extern void appear(char_data *ch);
extern int pk_allowed;
extern void check_killer(char_data *ch, char_data *vict);
extern int skill_message(int dam, char_data *ch, char_data *vict, int attacktype);
extern void dam_message(int dam, char_data *ch, char_data *victim, int w_type);
extern void send_to_room(room_rnum room, const char *messg, ...);
extern ACMD(do_flee);
#if __MENTOC_USE_GROUP_GAIN__
extern void group_gain(char_data *ch, char_data *victim);
#endif
extern void forget(char_data *ch,char_data *victim);
#ifndef IS_WEAPON
#define IS_WEAPON(type) (((type) >= TYPE_HIT) && ((type) < TYPE_SUFFERING))
#endif
extern void act(const std::string& str, int hide_invisible, char_data *ch, obj_data *obj, void *vict_obj, int type);

#include "../affects.hpp"

namespace mods::melee::damage_types {
	using de = damage_event_t;
	using vpd = mods::scan::vec_player_data;
#define MFEEDBACK(HITS,DAMAGE,EVENT)\
		{\
				feedback_t f;\
				f.hits = HITS;\
				f.damage = DAMAGE;\
				f.from_direction = OPPOSITE_DIR(direction);\
				f.attacker = player->uuid();\
				f.damage_event = EVENT;\
			victim->damage_event(f);\
		}

	void player_died(player_ptr_t& attacker,player_ptr_t& victim) {
		md("target dead!");
		feedback_t feedback;
		feedback.damage = 0;
		feedback.hits = 0;
		feedback.from_direction = NORTH;
		feedback.damage_event= de::TARGET_DEAD_EVENT;
		feedback.attacker = victim->uuid();
		attacker->damage_event(feedback);
		stop_fighting(attacker->cd());
		stop_fighting(victim->cd());
		mods::weapons::damage_types::legacy::die(attacker->cd(),victim->cd());
		if(!victim->is_npc()) {
			mods::players::messages::consume_messages_by_player(victim->uuid());
		}
	}
	void player_died(player_ptr_t& victim) {
		stop_fighting(victim->cd());
		mods::weapons::damage_types::legacy::die(victim->cd());
	}

	std::tuple<feedback_t,feedback_t> send_cqc_damage(const int& hits, const int& dam, player_ptr_t& attacker, player_ptr_t& victim) {
		feedback_t feedback;
		feedback.hits = hits;
		feedback.damage = dam;
		feedback.attacker = attacker->uuid();
		feedback.damage_info.emplace_back(victim->uuid(),dam,victim->room());
		victim->set_attacker(attacker->uuid());
		auto attacker_feedback = feedback;
		feedback.damage_event = de::HIT_BY_CQC_DAMAGE;
		attacker_feedback.damage_event = de::YOU_INFLICTED_CQC_DAMAGE;
		victim->damage_event(feedback);
		attacker->damage_event(attacker_feedback);
		/** TODO: find analogue for this
		 * injure_roll(attacker,victim,weapon,feedback);
		 */
		/**
		 * TODO: find analogue for this
		if(mods::weapons::damage_calculator::attack_disorients(attacker,weapon,victim)) {
			mods::affects::affect_player_for({mods::affects::affect_t::DISORIENT},victim,mods::weapons::damage_calculator::disorient_ticks(attacker,weapon,victim));
			feedback.damage_event= de::YOU_ARE_DISORIENTED_EVENT;
			victim->damage_event(feedback);

			feedback.damage_event= de::YOU_DISORIENTED_SOMEONE_EVENT;
			attacker->damage_event(feedback);
		}
		*/
		return {feedback,attacker_feedback};
	}

	void missed(player_ptr_t& attacker,player_ptr_t& victim) {
		feedback_t feedback;
		feedback.hits = 0;
		feedback.damage = 0;
		feedback.from_direction = NORTH;
		feedback.attacker = attacker->uuid();
		feedback.damage_event = de::ATTACKER_NARROWLY_MISSED_YOU_EVENT;
		victim->damage_event(feedback);

		feedback.attacker = attacker->uuid();
		feedback.damage_event = de::YOU_MISSED_YOUR_TARGET_EVENT;
		attacker->damage_event(feedback);
	}
	std::tuple<bool,feedback_t> cqc_can_continue(player_ptr_t& attacker,player_ptr_t& victim) {
		feedback_t feedback;
		feedback.hits = 0;
		feedback.damage = 0;
		feedback.from_direction = NORTH;
		feedback.attacker = attacker->uuid();
		feedback.damage_event = de::YOURE_IN_PEACEFUL_ROOM;
		if(mods::rooms::is_peaceful(attacker->room())) {
			attacker->damage_event(feedback);
			md("is_peaceful room");
			return {false,feedback};
		}

		if(mods::rooms::is_peaceful(attacker->room())) {
			attacker->damage_event(feedback);
			md("is_peaceful(2)");
			return {false,feedback};
		}
		return {true,feedback};
	}
	void remember_event(player_ptr_t& victim,player_ptr_t& attacker) {
		if(IS_NPC(victim->cd())) {
			remember(victim->cd(),attacker->cd());
		}
	}

	feedback_t cqc_damage_with_feedback(
	    player_ptr_t& player,
	    player_ptr_t victim,
	    uint16_t dam
	) {
		using de = damage_event_t;
		feedback_t feedback;

		auto s = cqc_can_continue(player,victim);
		if(std::get<0>(s) == false) {
			return std::get<1>(s);
		}
		feedback = std::get<1>(s);

		feedback.hits = 1;
		feedback.damage = 0;
		feedback.from_direction = NORTH;

		if(!victim || !mods::calc_visibility::is_visible(player,victim,0)) {
			feedback.damage_event = de::COULDNT_FIND_TARGET_EVENT;
			player->damage_event(feedback);
			md("victim not present. ignoring");
			return feedback;
		}

		/**
		 * TODO: find analogues/create code to support these two bonuses
		 *
		int str_bonus = mods::melee::damage_calculator::calculate_strength_bonus(player,weapon,victim,dam);
		md("strength bonus: " << str_bonus);

		int con_dampener = mods::melee::damage_calculator::calculate_constitution_resistance(player,weapon,victim,dam);
		md("constitution dampener amount: " << con_dampener);
		*/

		//dam = dam + str_bonus - con_dampener;

		md("dam: " << dam);

		/*
		dam = calculate_tracked_damage(player,dam);
		md("tracked damage: " << dam);
		*/

		if(victim->position() > POS_DEAD) {
			md("deploying damage: " << dam);
			damage(player->cd(),victim->cd(),dam,TYPE_HIT);
			if(dam == 0) {
				md("missed victim");
				missed(player,victim);
			} else if(dam > 0) {
				md("sending melee damage: " << feedback.dump());
				auto f = send_cqc_damage(feedback.hits, dam, player, victim);
				feedback = std::get<0>(f);
				if(victim->is_npc()) {
					mods::mobs::damage_event::meleed(victim,feedback);
				}
			}
			md("remembering");
			remember_event(victim,player);
		}
		if(victim->position() == POS_DEAD) {
			mods::weapons::damage_types::legacy::player_died(player,victim);
		}

		md("setting fight timestamp");
		player->set_fight_timestamp();
		md("processing elemental damage");
		/** TODO: find analogue for this:
		 * mods::melee::elemental::process_elemental_damage(player,weapon,victim,feedback);
		 */
		return feedback;
	}// end melee_damage_with_feedback

};
