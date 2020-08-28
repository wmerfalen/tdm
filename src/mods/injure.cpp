#include "injure.hpp"
#ifdef __MENTOC_SHOW_MODS_APPLY_CRITICAL_DEBUG_OUTPUT__
#define apcrit_debug(MSG) mentoc_prefix_debug("[mods::projectile::apply_critical_damage]")  << MSG << "\n";
#else
#define apcrit_debug(MSG) ;;
#endif
extern void send_to_room_except(room_rnum room, player_ptr_t, const char *messg, ...);

namespace mods::injure {
	void injure_player(player_ptr_t& person){
		send_to_room_except(person->room(), person,"%s is injured!\r\n",person->name().c_str());
		person->position() = POS_INCAP;
		person->hp() = mods::values::INJURED_HP();
		if(IS_NPC(person->cd())){
			person->cd()->mob_specials.clear_behaviour_tree();
			return;
		}
		msg::youre_injured(person);
	}
	bool do_injure_roll(uint8_t chance){
		return dice(1,100) <= chance;
	}
	bool do_critical_roll(int chance){
		return dice(1,100) <= chance;
	}
	void handle_crit_injure(int critical_chance, int injure_chance, player_ptr_t& victim, int type){
		auto crit = do_critical_roll(critical_chance);
		auto injure = do_injure_roll(injure_chance);
		if(crit){
			msg::crit(victim);
			/** TODO: be smarter about how we calculate this.. possibly.. */
			int amount = 0;
			switch(type){
				case ITEM_EXPLOSIVE:
					amount = victim->max_hp() * mods::values::EXPLOSIVE_CRITICAL_MULTIPLIER();
					break;
				case ITEM_RIFLE:
					amount = victim->max_hp() * mods::values::RIFLE_CRITICAL_MULTIPLIER();
					break;
				default:
					log("WARNING: using default of / 4 for crit damage due to unrecognized type");
					amount = victim->max_hp() / 4;
					break;
			}
			apcrit_debug("player max_hp: " << victim->max_hp() << ", critical amount: " << amount);
			auto hp = victim->hp();
			if(amount >= hp){
				apcrit_debug("amount is greater than victim hp.. setting to zero");
				victim->hp() = 0;
				return;
			}
			victim->hp() -= amount;
		}
		if(injure){
			injure_player(victim);
			msg::youre_injured(victim);
		}
	}
	namespace explosive {
		void handle_crit_injure(obj_ptr_t& object, player_ptr_t& player){
			mods::injure::handle_crit_injure(object->explosive()->attributes->critical_chance,
					object->explosive()->attributes->chance_to_injure,
					player,
					ITEM_EXPLOSIVE
					);
		}
	};
	namespace rifle {
		void handle_crit_injure(obj_ptr_t& object, player_ptr_t& player){
			mods::injure::handle_crit_injure(object->rifle()->attributes->critical_chance,
					object->rifle()->attributes->chance_to_injure,
					player,
					ITEM_RIFLE
					);
		}
	};
};
