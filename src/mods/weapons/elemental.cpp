#include "elemental.hpp"
#include "../object-utils.hpp"
#include "../interpreter-include.hpp"
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
	void process_elemental_damage(player_ptr_t& player,obj_ptr_t& weapon,player_ptr_t& victim,feedback_t& feedback) {
		if(!feedback.hits) {
			m_debug("no hits detected. not processing elemental damage");
			return;
		}
		m_debug("detected hits. checking elemental damage");
		for(const auto& elem : mods::forge_engine::fetch_valid_elemental_types()) {
			switch((elemental_types_t)elem) {
				case ELEM_INCENDIARY:
					if(weapon->rifle()->attributes->incendiary_damage) {
						m_debug("weapon has incediary damage. processing...");
						incendiary_damage(victim, feedback.damage * (weapon->rifle()->attributes->incendiary_damage * 0.01));
					}
					break;
				case ELEM_EXPLOSIVE:
					if(weapon->rifle()->attributes->explosive_damage) {
						m_debug("weapon has incediary damage. processing...");
						explosive_damage(victim, feedback.damage * (weapon->rifle()->attributes->explosive_damage * 0.01));
					}
					break;
				case ELEM_SHRAPNEL:
					if(weapon->rifle()->attributes->shrapnel_damage) {
						m_debug("weapon has incediary damage. processing...");
						shrapnel_damage(victim, feedback.damage * (weapon->rifle()->attributes->shrapnel_damage * 0.01));
					}
					break;
				case ELEM_CORROSIVE:
					if(weapon->rifle()->attributes->corrosive_damage) {
						m_debug("weapon has incediary damage. processing...");
						corrosive_damage(victim, feedback.damage * (weapon->rifle()->attributes->corrosive_damage * 0.01));
					}
					break;
				case ELEM_CRYOGENIC:
					if(weapon->rifle()->attributes->cryogenic_damage) {
						m_debug("weapon has incediary damage. processing...");
						cryogenic_damage(victim, feedback.damage * (weapon->rifle()->attributes->cryogenic_damage * 0.01));
					}
					break;
				case ELEM_RADIOACTIVE:
					if(weapon->rifle()->attributes->radioactive_damage) {
						m_debug("weapon has incediary damage. processing...");
						radioactive_damage(victim, feedback.damage * (weapon->rifle()->attributes->radioactive_damage * 0.01));
					}
					break;
				case ELEM_EMP:
					if(weapon->rifle()->attributes->emp_damage) {
						m_debug("weapon has incediary damage. processing...");
						emp_damage(victim, feedback.damage * (weapon->rifle()->attributes->emp_damage * 0.01));
					}
					break;
				case ELEM_SHOCK:
					if(weapon->rifle()->attributes->shock_damage) {
						m_debug("weapon has incediary damage. processing...");
						shock_damage(victim, feedback.damage * (weapon->rifle()->attributes->shock_damage * 0.01));
					}
					break;
				case ELEM_ANTI_MATTER:
					if(weapon->rifle()->attributes->anti_matter_damage) {
						m_debug("weapon has incediary damage. processing...");
						anti_matter_damage(victim, feedback.damage * (weapon->rifle()->attributes->anti_matter_damage * 0.01));
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
