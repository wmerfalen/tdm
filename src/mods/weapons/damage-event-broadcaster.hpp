namespace mods::weapons::damage_types {
	extern bool attack_injures(player_ptr_t& player,player_ptr_t& victim,obj_ptr_t& weapon,feedback_t feedback);
	using de = damage_event_t;
	using vpd = mods::scan::vec_player_data;

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
	void injure_roll(player_ptr_t& attacker,player_ptr_t& victim,obj_ptr_t& weapon,feedback_t& feedback) {
#ifdef __MENTOC_DISABLE_INJURE_DYNAMICS__
#else
		if(attack_injures(attacker,victim,weapon,feedback) && can_be_injured(victim)) {
			mods::injure::injure_player(victim);
			feedback.attacker = attacker->uuid();
			feedback.injured.emplace_back(victim->uuid());
			feedback.damage_event = de::YOU_ARE_INJURED_EVENT;
			{
				auto attacker_feedback = feedback;
				attacker_feedback.attacker = 0;
				attacker_feedback.damage_event = de::YOU_INJURED_SOMEONE_EVENT;
				attacker_feedback.injured.emplace_back(victim->uuid());
				attacker->damage_event(attacker_feedback);
			}
		}
#endif
	}
	namespace classes::sniper {
		void send_innate_bonus(const int& dam,const direction_t& direction,player_ptr_t& attacker,player_ptr_t& victim,obj_ptr_t& weapon) {
#ifdef __MENTOC_USE_MUNITIONS_REFLECTOR_FOR_INNATE_SNIPER_BONUS__
			if(mods::skills::player_can(victim,"MUNITIONS_REFLECTOR") && dice(1,100) <= 4) {
				reflect_munitions(attacker,victim,dam,OPPOSITE_DIR(direction));
			}
#endif
			feedback_t feedback;
			feedback.hits = 1;
			feedback.damage = dam;
			feedback.damage_info.emplace_back(victim->uuid(),dam,victim->room());
			victim->set_attacker(attacker->uuid());
			feedback.from_direction = OPPOSITE_DIR(direction);
			feedback.damage_event = de::HIT_BY_BONUS_INNATE_SNIPER_RIFLE_ATTACK;
			victim->damage_event(feedback);

			{
				auto attacker_feedback = feedback;
				attacker_feedback.damage_event = de::YOU_INFLICTED_BONUS_INNATE_SNIPER_RIFLE_ATTACK;
				attacker->damage_event(attacker_feedback);
			}
			injure_roll(attacker,victim,weapon,feedback);

		}
	};//end namespace classes::sniper


	void send_rifle_damage(int hits,const int& dam,const direction_t& direction,player_ptr_t& attacker,player_ptr_t& victim,obj_ptr_t& weapon, feedback_t& feedback) {
		if(mods::skills::player_can(victim,"MUNITIONS_REFLECTOR") && dice(1,100) <= 4) {
			reflect_munitions(attacker,victim,dam,OPPOSITE_DIR(direction));
		}
		feedback.hits++;
		feedback.damage += dam;
		feedback.damage_info.emplace_back(victim->uuid(),dam,victim->room());
		victim->set_attacker(attacker->uuid());
		const auto& player = attacker;
		feedback.from_direction = OPPOSITE_DIR(direction);
		feedback.damage_event = de::HIT_BY_SPRAY_ATTACK;
		victim->damage_event(feedback);

		{
			auto attacker_feedback = feedback;
			attacker_feedback.damage_event = de::YOU_INFLICTED_SPRAY_DAMAGE;
			player->damage_event(attacker_feedback);
		}
		injure_roll(attacker,victim,weapon,feedback);

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
		} else {
			feedback.damage_event = de::HIT_BY_BLUNT_MELEE_ATTACK;
			attacker_feedback.damage_event = de::YOU_INFLICTED_BLUNT_MELEE_ATTACK;
		}
		victim->damage_event(feedback);
		attacker->damage_event(attacker_feedback);
		injure_roll(attacker,victim,weapon,feedback);
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
#undef MFEEDBACK
};
