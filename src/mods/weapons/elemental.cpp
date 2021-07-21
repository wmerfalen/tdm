#include "elemental.hpp"
#include "../object-utils.hpp"
#include "../interpreter-include.hpp"
#include "../mobs/damage-event.hpp"
#ifdef __MENTOC_MODS_SHOW_ELEMENTAL_DEBUG_OUTPUT__
#define m_debug(a) std::cerr << "[mods::weapons::damage_types][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#else
#define m_debug(a)
#endif

#include "../forge-engine/elemental-enum.hpp"
#include "damage-types.hpp"

namespace mods::weapons::elemental {
	using de = damage_event_t;
	int reduce_elemental_resistance(int requested_damage, int16_t resistance) {
		if(resistance <= 0) {
			return requested_damage;
		}
		int damage = requested_damage - (requested_damage * (float)(resistance / 100.0));
		if(damage <= 0) {
			return 0;
		}
		return damage;
	}
	void process_elemental_damage(player_ptr_t& attacker,player_ptr_t& victim,int requested_damage,int type) {
		if(attacker->position() == POS_DEAD || victim->position() == POS_DEAD) {
			return;
		}
		feedback_t feedback;
		feedback.hits = 1;
		auto atk_event = de::YOU_INFLICTED_INCENDIARY_DAMAGE;
		auto vic_event = de::HIT_BY_INCENDIARY_DAMAGE;
		switch(type) {
			case ELEM_SHOCK:
				feedback.damage = reduce_elemental_resistance(requested_damage,victim->shock_resistance_percent());
				atk_event =  de::YOU_INFLICTED_SHOCK_DAMAGE;
				vic_event = de::HIT_BY_SHOCK_DAMAGE;
				break;
			case ELEM_INCENDIARY:
				feedback.damage = reduce_elemental_resistance(requested_damage,victim->incendiary_resistance_percent());
				atk_event =  de::YOU_INFLICTED_INCENDIARY_DAMAGE;
				vic_event = de::HIT_BY_INCENDIARY_DAMAGE;
				break;
			case ELEM_EXPLOSIVE:
				feedback.damage = reduce_elemental_resistance(requested_damage,victim->explosive_resistance_percent());
				atk_event =  de::YOU_INFLICTED_EXPLOSIVE_DAMAGE;
				vic_event = de::HIT_BY_EXPLOSIVE_DAMAGE;
				break;

			case ELEM_SHRAPNEL:
				feedback.damage = reduce_elemental_resistance(requested_damage,victim->shrapnel_resistance_percent());
				atk_event =  de::YOU_INFLICTED_SHRAPNEL_DAMAGE;
				vic_event = de::HIT_BY_SHRAPNEL_DAMAGE;
				break;

			case ELEM_EMP:
				feedback.damage = reduce_elemental_resistance(requested_damage,victim->emp_resistance_percent());
				atk_event =  de::YOU_INFLICTED_EMP_DAMAGE;
				vic_event = de::HIT_BY_EMP_DAMAGE;
				break;
			case ELEM_CORROSIVE:
				feedback.damage = reduce_elemental_resistance(requested_damage,victim->corrosive_resistance_percent());
				atk_event =  de::YOU_INFLICTED_CORROSIVE_DAMAGE;
				vic_event = de::HIT_BY_CORROSIVE_DAMAGE;
				break;

			case ELEM_CRYOGENIC:
				feedback.damage = reduce_elemental_resistance(requested_damage,victim->cryogenic_resistance_percent());
				atk_event =  de::YOU_INFLICTED_CRYOGENIC_DAMAGE;
				vic_event = de::HIT_BY_CRYOGENIC_DAMAGE;
				break;

			case ELEM_RADIOACTIVE:
				feedback.damage = reduce_elemental_resistance(requested_damage,victim->radiation_resistance_percent());
				atk_event =  de::YOU_INFLICTED_RADIOACTIVE_DAMAGE;
				vic_event = de::HIT_BY_RADIOACTIVE_DAMAGE;
				break;
			case ELEM_ANTI_MATTER:
				feedback.damage = reduce_elemental_resistance(requested_damage,victim->anti_matter_resistance_percent());
				atk_event =  de::YOU_INFLICTED_ANTI_MATTER_DAMAGE;
				vic_event = de::HIT_BY_ANTI_MATTER_DAMAGE;
				break;
			default:
				return;
				break;
		}
		if(feedback.damage > 0) {
			victim->hp() -= feedback.damage;
			if(victim->is_npc()) {
				mods::mobs::damage_event::received_elemental_damage(victim,feedback);
			}
			feedback.damage_info.emplace_back(victim->uuid(),feedback.damage,victim->room());
			feedback.damage_event = atk_event;
			attacker->damage_event(feedback);

			feedback.damage_info.clear();

			feedback.attacker = attacker->uuid();
			feedback.damage_event = vic_event;
			victim->damage_event(feedback);
		}
	}
	void incendiary_damage(player_ptr_t& attacker,player_ptr_t& victim,int requested_damage) {
		process_elemental_damage(attacker,victim,requested_damage,ELEM_INCENDIARY);
	}

	void explosive_damage(player_ptr_t& attacker,player_ptr_t& victim,int requested_damage) {
		process_elemental_damage(attacker,victim,requested_damage,ELEM_EXPLOSIVE);
	}

	void shrapnel_damage(player_ptr_t& attacker,player_ptr_t& victim,int requested_damage) {
		process_elemental_damage(attacker,victim,requested_damage,ELEM_SHRAPNEL);
	}

	void corrosive_damage(player_ptr_t& attacker,player_ptr_t& victim,int requested_damage) {
		process_elemental_damage(attacker,victim,requested_damage,ELEM_CORROSIVE);
	}
	void cryogenic_damage(player_ptr_t& attacker,player_ptr_t& victim,int requested_damage) {
		process_elemental_damage(attacker,victim,requested_damage,ELEM_CRYOGENIC);
	}
	void radioactive_damage(player_ptr_t& attacker,player_ptr_t& victim,int requested_damage) {
		process_elemental_damage(attacker,victim,requested_damage,ELEM_RADIOACTIVE);
	}
	void emp_damage(player_ptr_t& attacker,player_ptr_t& victim,int requested_damage) {
		process_elemental_damage(attacker,victim,requested_damage,ELEM_EMP);
	}
	void shock_damage(player_ptr_t& attacker,player_ptr_t& victim,int requested_damage) {
		process_elemental_damage(attacker,victim,requested_damage,ELEM_SHOCK);
	}
	void anti_matter_damage(player_ptr_t& attacker,player_ptr_t& victim,int requested_damage) {
		process_elemental_damage(attacker,victim,requested_damage,ELEM_ANTI_MATTER);
	}

	/**
	 * @brief if the feedback member has proven damage was dealt, will process all elemental damages
	 *
	 * @param player
	 * @param weapon
	 * @param victim
	 * @param feedback
	 */
	void process_elemental_damage(player_ptr_t& attacker,obj_ptr_t& weapon,player_ptr_t& victim,feedback_t& feedback) {
		if(!feedback.hits) {
			m_debug("no hits detected. not processing elemental damage");
			return;
		}
		m_debug("detected hits. checking elemental damage");
		float incendiary = 0;
		float explosive = 0;
		float shrapnel = 0;
		float corrosive = 0;
		float cryogenic = 0;
		float radioactive = 0;
		float emp = 0;
		float shock = 0;
		float anti_matter = 0;
		if(weapon->has_melee()) {
			incendiary =  weapon->melee()->attributes->incendiary_damage * 0.01;
			explosive =  weapon->melee()->attributes->explosive_damage * 0.01;
			shrapnel =  weapon->melee()->attributes->shrapnel_damage * 0.01;
			corrosive =  weapon->melee()->attributes->corrosive_damage * 0.01;
			cryogenic =  weapon->melee()->attributes->cryogenic_damage * 0.01;
			radioactive =  weapon->melee()->attributes->radioactive_damage * 0.01;
			emp =  weapon->melee()->attributes->emp_damage * 0.01;
			shock =  weapon->melee()->attributes->shock_damage * 0.01;
			anti_matter =  weapon->melee()->attributes->anti_matter_damage * 0.01;
		} else {
			auto rifle_variant = mods::rifle_attachments::by_uuid(weapon->uuid);
			incendiary = (rifle_variant ? rifle_variant->incendiary_damage_percent : weapon->rifle()->attributes->incendiary_damage) * 0.01;
			explosive = (rifle_variant ? rifle_variant->explosive_damage_percent : weapon->rifle()->attributes->explosive_damage) * 0.01;
			shrapnel = (rifle_variant ? rifle_variant->shrapnel_damage_percent : weapon->rifle()->attributes->shrapnel_damage) * 0.01;
			corrosive = (rifle_variant ? rifle_variant->corrosive_damage_percent : weapon->rifle()->attributes->corrosive_damage) * 0.01;
			cryogenic = (rifle_variant ? rifle_variant->cryogenic_damage_percent : weapon->rifle()->attributes->cryogenic_damage) * 0.01;
			radioactive = (rifle_variant ? rifle_variant->radiation_damage_percent : weapon->rifle()->attributes->radioactive_damage) * 0.01;
			emp = (rifle_variant ? rifle_variant->emp_damage_percent : weapon->rifle()->attributes->emp_damage) * 0.01;
			shock = (rifle_variant ? rifle_variant->shock_damage_percent : weapon->rifle()->attributes->shock_damage) * 0.01;
			anti_matter = (rifle_variant ? rifle_variant->anti_matter_damage_percent : weapon->rifle()->attributes->anti_matter_damage) * 0.01;
		}
		if(incendiary > 0.0) {
			m_debug("weapon has incendiary damage. processing...");
			incendiary_damage(attacker,victim, feedback.damage * incendiary);
		}
		if(explosive > 0.0) {
			m_debug("weapon has explosive damage. processing...");
			explosive_damage(attacker,victim, feedback.damage * explosive);
		}
		if(shrapnel > 0.0) {
			m_debug("weapon has shrapnel damage. processing...");
			shrapnel_damage(attacker,victim, feedback.damage * shrapnel);
		}
		if(corrosive > 0.0) {
			m_debug("weapon has corrosive damage. processing...");
			corrosive_damage(attacker,victim, feedback.damage * corrosive);
		}
		if(cryogenic > 0.0) {
			m_debug("weapon has cryogenic damage. processing...");
			cryogenic_damage(attacker,victim, feedback.damage * cryogenic);
		}
		if(radioactive > 0.0) {
			m_debug("weapon has radioactive damage. processing...");
			radioactive_damage(attacker,victim, feedback.damage * radioactive);
		}
		if(emp > 0.0) {
			m_debug("weapon has emp damage. processing...");
			emp_damage(attacker,victim, feedback.damage * emp);
		}
		if(shock > 0.0) {
			m_debug("weapon has shock damage. processing...");
			shock_damage(attacker,victim, feedback.damage * shock);
		}
		if(anti_matter > 0.0) {
			m_debug("weapon has anti_matter damage. processing...");
			anti_matter_damage(attacker,victim, feedback.damage * anti_matter);
		}
	}

	void room_fire_damage(player_ptr_t& player,uint16_t damage) {
		player->send(mods::values::MSG_FIRE_DAMAGE().c_str(),damage);
		mods::weapons::damage_types::deal_hp_damage(player,damage);
	}

	void room_smoke_damage(player_ptr_t& player, uint16_t damage) {
		player->send(mods::values::MSG_SMOKE_DAMAGE().c_str(),damage);
		mods::weapons::damage_types::deal_hp_damage(player,damage);
	}

	SUPERCMD(do_inc_damage) {
		auto vec_args = PARSE_ARGS();
		static const char* usage = "usage: inc_damage <resistance_percent> <damage>\r\n";
		if(vec_args.size() < 2) {
			player->sendln(usage);
			return;
		}
		int resistance = mods::util::stoi(vec_args[0]).value_or(-1);
		if(resistance == -1) {
			player->errorln("Invalid resistance");
			return;
		}
		int damage = mods::util::stoi(vec_args[1]).value_or(0);
		if(damage == -1) {
			player->errorln("Invalid damage");
			return;
		}
		auto obj = create_object(ITEM_RIFLE,"g36c.yml");
		player->incendiary_resistance_percent() = resistance;
		player->send("Your resistance: %f\r\n",player->incendiary_resistance_percent());
		incendiary_damage(player,player,damage);
	}

	void init() {
		ADD_INTERPRETER_COMMAND(inc_damage,LVL_BUILDER);
	}

};
#undef m_debug
