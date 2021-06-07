#include "elemental.hpp"
#include "../object-utils.hpp"
#include "../interpreter-include.hpp"
#define __MENTOC_MODS_SHOW_ELEMENTAL_DEBUG_OUTPUT__
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
	void elemental_damage(player_ptr_t& player,std::string_view message,int damage) {
		if(damage  <= 0) {
			return;
		}
		player->send(message.data(),damage);
		mods::weapons::damage_types::deal_hp_damage(player,damage);
	}

	void incendiary_damage(player_ptr_t& player,int requested_damage) {
		elemental_damage(player, MSG_FIRE_DAMAGE(), reduce_elemental_resistance(requested_damage,player->incendiary_resistance_percent()));
	}

	void explosive_damage(player_ptr_t& player,int requested_damage) {
		elemental_damage(player, MSG_EXPLOSIVE_DAMAGE(), reduce_elemental_resistance(requested_damage,player->explosive_resistance_percent()));
	}
	void shrapnel_damage(player_ptr_t& player,int requested_damage) {
		elemental_damage(player, MSG_SHRAPNEL_DAMAGE(), reduce_elemental_resistance(requested_damage,player->shrapnel_resistance_percent()));
	}
	void corrosive_damage(player_ptr_t& player,int requested_damage) {
		elemental_damage(player, MSG_CORROSIVE_DAMAGE(), reduce_elemental_resistance(requested_damage,player->corrosive_resistance_percent()));
	}
	void cryogenic_damage(player_ptr_t& player,int requested_damage) {
		elemental_damage(player, MSG_CRYOGENIC_DAMAGE(), reduce_elemental_resistance(requested_damage,player->cryogenic_resistance_percent()));
	}
	void radioactive_damage(player_ptr_t& player,int requested_damage) {
		elemental_damage(player, MSG_RADIOACTIVE_DAMAGE(), reduce_elemental_resistance(requested_damage,player->radiation_resistance_percent()));
	}
	void emp_damage(player_ptr_t& player,int requested_damage) {
		elemental_damage(player, MSG_EMP_DAMAGE(), reduce_elemental_resistance(requested_damage,player->emp_resistance_percent()));
	}
	void shock_damage(player_ptr_t& player,int requested_damage) {
		elemental_damage(player, MSG_SHOCK_DAMAGE(), reduce_elemental_resistance(requested_damage,player->shock_resistance_percent()));
	}
	void anti_matter_damage(player_ptr_t& player,int requested_damage) {
		elemental_damage(player, MSG_ANTI_MATTER_DAMAGE(), reduce_elemental_resistance(requested_damage,player->anti_matter_resistance_percent()));
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
		for(const auto& elem : mods::forge_engine::fetch_valid_elemental_types()) {
			switch((elemental_types_t)elem) {
				case ELEM_INCENDIARY:
					if(incendiary > 0.0) {
						m_debug("weapon has incendiary damage. processing...");
						incendiary_damage(victim, feedback.damage * incendiary);
					}
					break;
				case ELEM_EXPLOSIVE:
					if(explosive > 0.0) {
						m_debug("weapon has explosive damage. processing...");
						explosive_damage(victim, feedback.damage * explosive);
					}
					break;
				case ELEM_SHRAPNEL:
					if(shrapnel > 0.0) {
						m_debug("weapon has shrapnel damage. processing...");
						shrapnel_damage(victim, feedback.damage * shrapnel);
					}
					break;
				case ELEM_CORROSIVE:
					if(corrosive > 0.0) {
						m_debug("weapon has corrosive damage. processing...");
						corrosive_damage(victim, feedback.damage * corrosive);
					}
					break;
				case ELEM_CRYOGENIC:
					if(cryogenic > 0.0) {
						m_debug("weapon has cryogenic damage. processing...");
						cryogenic_damage(victim, feedback.damage * cryogenic);
					}
					break;
				case ELEM_RADIOACTIVE:
					if(radioactive > 0.0) {
						m_debug("weapon has radioactive damage. processing...");
						radioactive_damage(victim, feedback.damage * radioactive);
					}
					break;
				case ELEM_EMP:
					if(emp > 0.0) {
						m_debug("weapon has emp damage. processing...");
						emp_damage(victim, feedback.damage * emp);
					}
					break;
				case ELEM_SHOCK:
					if(shock > 0.0) {
						m_debug("weapon has shock damage. processing...");
						shock_damage(victim, feedback.damage * shock);
					}
					break;
				case ELEM_ANTI_MATTER:
					if(anti_matter > 0.0) {
						m_debug("weapon has anti_matter damage. processing...");
						anti_matter_damage(victim, feedback.damage * anti_matter);
					}
					break;
				default:
					break;
			}
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
		incendiary_damage(player,damage);
	}

	void init() {
		ADD_INTERPRETER_COMMAND(inc_damage,LVL_BUILDER);
	}

};
#undef m_debug
