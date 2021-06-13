#ifdef __MENTOC_SHOW_COOLDOWN_DEBUG__
#define cdebug(a) std::cerr << "[mods::weapons::damage_types::set_player_weapon_cooldown][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#else
#define cdebug(a)
#endif
namespace mods::weapons::damage_types {
	bool can_be_injured(player_ptr_t& victim);
	bool attack_injures(player_ptr_t& player,player_ptr_t& victim,obj_ptr_t& weapon,feedback_t feedback) {
		if(mods::super_users::player_is(victim)) {
			return 0;
		}
		float chance = 0;
		if(weapon->has_rifle()) {
			chance = weapon->rifle()->attributes->chance_to_injure;
		} else if(weapon->has_melee()) {
			chance = weapon->melee()->attributes->chance_to_injure;
		}
		if(mods::skills::player_can(player,"INCREASED_INJURE_CHANCE")) {
			chance += CHANCE_TO_INJURE_SKILL_MODIFIER();
		}
		if(mods::skills::player_can(victim,"INJURE_RESISTANCE")) {
			chance -= INJURE_RESISTANCE_SKILL_MODIFIER();
		}
		switch(mods::levels::player_tier(victim)) {
			case 1:
				chance -= 0.01 * victim->constitution();
				break;
			case 2:
				chance -= 0.02 * victim->constitution();
				break;
			case 3:
				chance -= 0.11 * victim->constitution();
				break;
			default:
				break;
		}
		return mods::injure::do_injure_roll(chance);
	}

	std::tuple<bool,feedback_t> can_continue(player_ptr_t& player,obj_ptr_t weapon,std::variant<player_ptr_t,obj_ptr_t,int8_t> victim,direction_t direction, const uint8_t& item_type) {
		feedback_t feedback;
		feedback.hits = 0;
		feedback.damage = 0;
		feedback.from_direction = OPPOSITE_DIR(direction);
		feedback.attacker = player->uuid();

		if((std::holds_alternative<player_ptr_t>(victim) && !std::get<player_ptr_t>(victim)) ||
		        (std::holds_alternative<obj_ptr_t>(victim) && !std::get<obj_ptr_t>(victim))) {
			feedback.damage_event = de::COULDNT_FIND_TARGET_EVENT;
			player->damage_event(feedback);
			return {false,feedback};
		}

		if(mods::rooms::is_peaceful(player->room())) {
			feedback.damage_event = de::YOURE_IN_PEACEFUL_ROOM;
			player->damage_event(feedback);
			return {false,feedback};
		}

		if(!weapon) {
			feedback.damage_event = de::NO_PRIMARY_WIELDED_EVENT;
			player->damage_event(feedback);
			return {false,feedback};
		}
		switch(item_type) {
			case ITEM_RIFLE:
				if(!weapon->has_rifle()) {
					feedback.damage_event = de::NO_PRIMARY_WIELDED_EVENT;
					player->damage_event(feedback);
					return {false,feedback};
				}
				if(!player->can_attack_again()) {
					feedback.damage_event = de::COOLDOWN_IN_EFFECT_EVENT;
					player->damage_event(feedback);
					return {false,feedback};
				}
				/* Check ammo */
				if(mods::object_utils::get_ammo(weapon) == 0) {
					feedback.damage_event = de::OUT_OF_AMMO_EVENT;
					player->damage_event(feedback);
					return {false,feedback};
				}
				/* FIXME this is duplicate but different code  */
				if(weapon->has_rifle() && mods::weapon::has_clip(player->rifle()) && player->ammo() <= 0) {
					feedback.damage_event = de::OUT_OF_AMMO_EVENT;
					player->damage_event(feedback);
					return {false,feedback};
				}
				break;
			case ITEM_MELEE:
				if(weapon && weapon->has_melee() == false) {
					feedback.damage_event = de::NO_PRIMARY_WIELDED_EVENT;
					player->damage_event(feedback);
					return {false,feedback};
				}
				break;
			default:
				break;
		}
		return {true,feedback};
	}

	int calculate_weapon_cooldown(player_ptr_t& attacker,player_ptr_t& victim,obj_ptr_t& attackers_weapon, feedback_t& feedback) {
		static const float modifiers[] = {0,
		                                  WEAPON_COOLDOWN_MODIFIER_TIER_ONE(),
		                                  WEAPON_COOLDOWN_MODIFIER_TIER_TWO(),
		                                  WEAPON_COOLDOWN_MODIFIER_TIER_THREE()
		                                 };
		auto tier = mods::levels::player_tier(attacker);
		if(tier >= sizeof(modifiers)) {
			cdebug("Warning! player tier returned unsupported value. Defaulting to 1");
			log("SYSERR: player_tier returned an invalid value. Using default of 1!");
		}
		tier = std::clamp(tier,(uint8_t)1,(uint8_t)3);
		int cooldown = 0;
		if(attackers_weapon->has_rifle()) {
			cdebug("weapon has rifle");
			cooldown = attackers_weapon->rifle()->attributes->cooldown_between_shots;
			cdebug("rifle's cooldown: " << cooldown);
		}
		cooldown -= attacker->weapon_handling() * modifiers[tier];
		cdebug("attacker level: " << attacker->level() << ", tier:" << tier);
		cdebug("cooldown *AFTER* weapon_handling modifier: " << cooldown << ", modifier: " << attacker->weapon_handling() * modifiers[tier]);
		return cooldown;
	}

	void set_player_weapon_cooldown(player_ptr_t& attacker,player_ptr_t& victim,obj_ptr_t& attackers_weapon, feedback_t& feedback) {
		auto cooldown = calculate_weapon_cooldown(attacker,victim,attackers_weapon,feedback);
		if(cooldown <= 0) {
			attacker->weapon_cooldown_clear();
		} else {
			attacker->weapon_cooldown_start(cooldown);
		}
	}
#undef cdebug

	int calculate_tracked_damage(player_ptr_t& player,int original_damage) {
		if(player->has_affect(AFF_TRACKED)) {
			return mods::affects::apply_tracked_bonus_damage(original_damage);
		}
		return original_damage;
	}

	void remember_event(player_ptr_t& victim,player_ptr_t& attacker) {
		if(IS_NPC(victim->cd())) {
			remember(victim->cd(),attacker->cd());
		}
	}
	bool can_be_injured(player_ptr_t& victim) {
		return victim->hp() <= (victim->max_hp() * INJURED_MAX_HP_MULTIPLIER());
	}

	uint8_t calculate_spray_chance(player_ptr_t& player) {
		uint8_t spray_chance = mods::values::SPRAY_CHANCE();
		if(mods::skills::player_can(player,"SPRAY_CHANCE")) {
			spray_chance += mods::values::SPRAY_CHANCE_SKILL_MODIFIER();
		}
		return spray_chance;
	}
#undef MFEEDBACK
};
#ifdef DMG_DEBUG
#undef DMG_DEBUG
#endif
