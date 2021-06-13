#include "damage-types.hpp"
#include "../../spells.h"
#include "../object-utils.hpp"
#include "../injure.hpp"
#include "../rooms.hpp"
#include "../skills.hpp"
#include "../projectile.hpp"
#include "../levels.hpp"
#include "../interpreter-include.hpp"
#include "elemental.hpp"
#include "damage-calculator.hpp"
#include <variant>

#define dty_debug(a) std::cerr << "[mods::weapons::damage_types][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#ifndef TO_ROOM
#define TO_ROOM		1
#endif

extern int ok_damage_shopkeeper(char_data *ch, char_data *victim);
extern void die(char_data* killer,char_data *victim);
extern void die(char_data *victim);
extern void set_fighting(char_data *ch, char_data *vict);
extern void appear(char_data *ch);
extern int pk_allowed;
extern void check_killer(char_data *ch, char_data *vict);
extern int skill_message(int dam, char_data *ch, char_data *vict, int attacktype);
extern void dam_message(int dam, char_data *ch, char_data *victim, int w_type);
extern ACMD(do_flee);
extern void group_gain(char_data *ch, char_data *victim);
extern void solo_gain(char_data *ch, char_data *victim);
extern void forget(char_data *ch,char_data *victim);

#ifndef IS_WEAPON
#define IS_WEAPON(type) (((type) >= TYPE_HIT) && ((type) < TYPE_SUFFERING))
#endif
extern void act(const std::string& str, int hide_invisible, char_data *ch, obj_data *obj, void *vict_obj, int type);

#include "../affects.hpp"

namespace mods::weapons::damage_types {
	extern bool attack_injures(player_ptr_t& player,player_ptr_t& victim,obj_ptr_t& weapon,feedback_t feedback);
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

	void reflect_munitions(player_ptr_t& attacker,player_ptr_t& victim,int dam,int from_direction);
	bool can_be_injured(player_ptr_t& victim);
	void handle_assault_rifle_shrapnel_skill(player_ptr_t& attacker,player_ptr_t& victim,obj_ptr_t& weapon,feedback_t& feedback);

	feedback_t send_target_dead(player_ptr_t& attacker,player_ptr_t& victim,feedback_t& feedback) {
		feedback.damage_event= de::TARGET_DEAD_EVENT;
		feedback.attacker = victim->uuid();
		attacker->damage_event(feedback);
		stop_fighting(attacker->cd());
		stop_fighting(victim->cd());
		return feedback;
	}

	void send_rifle_damage(int hits,const int& dam,const direction_t& direction,player_ptr_t& attacker,player_ptr_t& victim,obj_ptr_t& weapon, feedback_t& feedback) {
		if(mods::skills::player_can(victim,"MUNITIONS_REFLECTOR") && dice(1,100) <= 4) {
			reflect_munitions(attacker,victim,dam,OPPOSITE_DIR(direction));
		}
		feedback.hits++;
		feedback.damage += dam;
		feedback.damage_info.emplace_back(victim->uuid(),dam,victim->room());
		victim->set_attacker(attacker->uuid());
		const auto& player = attacker;
		MFEEDBACK(feedback.hits,dam,de::HIT_BY_SPRAY_ATTACK);
		if(attack_injures(attacker,victim,weapon,feedback) && can_be_injured(victim)) {
			mods::injure::injure_player(victim);
			feedback.injured.emplace_back(victim->uuid());
			MFEEDBACK(feedback.hits,dam,de::YOU_ARE_INJURED_EVENT);
		}
		if(weapon->rifle()->attributes->type == mw_rifle::ASSAULT_RIFLE) {
			handle_assault_rifle_shrapnel_skill(attacker,victim,weapon,feedback);
		}
	}

	std::tuple<feedback_t,feedback_t> send_melee_damage(const int& hits, const int& dam, player_ptr_t& attacker, player_ptr_t& victim, obj_ptr_t& weapon) {
		feedback_t feedback;
		feedback.hits = hits;
		feedback.damage = dam;
		feedback.damage_info.emplace_back(victim->uuid(),dam,victim->room());
		victim->set_attacker(attacker->uuid());
		auto attacker_feedback = feedback;
		if(mods::object_utils::is_bladed_weapon(weapon)) {
			feedback.damage_event = de::HIT_BY_BLADED_MELEE_ATTACK;
			attacker_feedback.damage_event = de::YOU_INFLICTED_BLADED_MELEE_ATTACK;
		} else if(mods::object_utils::is_blunt_weapon(weapon)) {
			feedback.damage_event = de::HIT_BY_BLUNT_MELEE_ATTACK;
			attacker_feedback.damage_event = de::YOU_INFLICTED_BLUNT_MELEE_ATTACK;
		} else {
			feedback.damage_event = de::HIT_BY_MELEE_ATTACK;
			attacker_feedback.damage_event = de::YOU_INFLICTED_MELEE_ATTACK;
		}
		victim->damage_event(feedback);
		attacker->damage_event(attacker_feedback);
		if(attack_injures(attacker,victim,weapon,feedback)) {
			feedback.injured.emplace_back(victim->uuid());
			feedback.damage_event= de::YOU_ARE_INJURED_EVENT;
			victim->damage_event(feedback);

			feedback.damage_event= de::YOU_INJURED_SOMEONE_EVENT;
			attacker->damage_event(feedback);
			mods::injure::injure_player(victim);
		}
		if(mods::weapons::damage_calculator::attack_disorients(attacker,weapon,victim)) {
			mods::affects::affect_player_for({mods::affects::affect_t::DISORIENT},victim,mods::weapons::damage_calculator::disorient_ticks(attacker,weapon,victim));
			feedback.damage_event= de::YOU_ARE_DISORIENTED_EVENT;
			victim->damage_event(feedback);

			feedback.damage_event= de::YOU_DISORIENTED_SOMEONE_EVENT;
			attacker->damage_event(feedback);
		}
		return {feedback,attacker_feedback};
	}


	void missed(player_ptr_t& player,player_ptr_t& victim) {
		feedback_t feedback;
		feedback.attacker = player->uuid();
		feedback.damage_event = de::ATTACKER_NARROWLY_MISSED_YOU_EVENT;
		victim->damage_event(feedback);

		feedback.attacker = player->uuid();
		feedback.damage_event = de::YOU_MISSED_YOUR_TARGET_EVENT;
		player->damage_event(feedback);
	}
	void ar_shrapnel(const int& from_direction, player_ptr_t& attacker,player_ptr_t& victim) {
		feedback_t shrapnel;
		shrapnel.hits = 1;
		shrapnel.from_direction = from_direction;
		shrapnel.attacker = attacker->uuid();
		int dice_count = ASSAULT_RIFLE_SHRAPNEL_SKILL_DICE_COUNT();
		int dice_sides = ASSAULT_RIFLE_SHRAPNEL_SKILL_DICE_SIDES();
		shrapnel.damage = mods::projectile::deploy_shrapnel_at(victim,dice_count,dice_sides,from_direction);
		shrapnel.damage_event = de::YOU_INFLICTED_AR_SHRAPNEL;
		attacker->damage_event(shrapnel);
		shrapnel.damage_event = de::YOU_GOT_HIT_BY_AR_SHRAPNEL;
		victim->damage_event(shrapnel);
	}

	void reflect_munitions(player_ptr_t& attacker,player_ptr_t& victim,int dam,int from_direction) {
		feedback_t f;
		f.hits = 1;
		f.damage = dam;
		f.from_direction = from_direction;
		f.attacker = attacker->uuid();

		f.damage_event = de::YOU_GOT_HIT_BY_REFLECTED_MUNITIONS_EVENT;
		victim->damage_event(f);

		f.attacker = victim->uuid();
		f.damage_event = de::YOU_REFLECTED_MUNITIONS_EVENT;
		attacker->damage_event(f);
	}

	void handle_assault_rifle_shrapnel_skill(player_ptr_t& attacker,player_ptr_t& victim,obj_ptr_t& weapon,feedback_t& feedback) {
		if(mods::super_users::player_is(victim)) {
			return;
		}
		if(mods::skills::player_can(attacker,"ASSAULT_RIFLE_SHRAPNEL") &&
		        dice(1,100) <= ASSAULT_RIFLE_SHRAPNEL_SKILL_CHANCE()) {
			ar_shrapnel(feedback.from_direction, attacker,victim);
		}
	}

};
#ifdef DMG_DEBUG
#undef DMG_DEBUG
#endif
