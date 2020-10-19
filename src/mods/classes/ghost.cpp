#include "ghost.hpp"
#include "../orm/inventory.hpp"
#include "../affects.hpp"
#include "../player-utils.hpp"
#include "../object-utils.hpp"

extern void stop_fighting(char_data *ch);
namespace mods::orm::inventory {
	extern int16_t flush_player(player_ptr_t & player);
};
namespace mods::classes {
		ghost::ghost(player_ptr_t p){
			load_by_player(p);
		}
		player_ptr_t 	ghost::player(){
			return m_player;
		}
		void ghost::set_player(player_ptr_t p){
			m_player = p;
		}
		int16_t ghost::new_player(player_ptr_t &player){
			auto db_id = m_orm.initialize_row(player,(primary_choice_t)0);
			if(db_id == 0){
				return -2;
			}
			load_by_player(player);
			return 0;
		}
		int16_t ghost::load_by_player(player_ptr_t & player){
			auto result = m_orm.load_by_player(player->db_id());
			if(result < 0){
				report(CAT("ghost::load_by_player unable to load ghost class by player id: ",(player->db_id()),".. return status: ",(result)));
				return result;
			}
			set_player(player);
			m_drone_scan_level = static_cast<drone_scan_levels_t>(m_orm.ghost_drone_scan_level);
			m_xray_shot_level = static_cast<xray_shot_levels_t>(m_orm.ghost_xray_shot_level);
			m_stealth_level = static_cast<stealth_levels_t>(m_orm.ghost_stealth_level);
			m_feign_death_level = static_cast<feign_death_levels_t>(m_orm.ghost_feign_death_level);
			m_summon_extraction_level = static_cast<summon_extraction_levels_t>(m_orm.ghost_summon_extraction_level);

			player->equip(create_object(ITEM_RIFLE,"czp10.yml"),WEAR_SECONDARY);
			return result;
		}
		void ghost::apply_stealth_to(obj_ptr_t& target){
			int stealth = 0;
			switch(m_stealth_level) {
				default:
				case stealth_levels_t::GHOST_STEALTH_NONE:
					m_player->sendln("It looks like you still need to train that skill");
					return;
				case stealth_levels_t::GHOST_STEALTH_INITIATE:
					stealth = dice(1, 8) + 1 + (m_player->level() / 4);
					break;

				case stealth_levels_t::GHOST_STEALTH_FAMILIAR:
					stealth = dice(3, 8) + 3 + (m_player->level() / 4);
					break;

				case stealth_levels_t::GHOST_STEALTH_MASTER:
					stealth = 100 + dice(3, 8);
					break;
			}
			mods::object_utils::change_visibility(target,- stealth);
		}
		void ghost::apply_stealth_to_player(player_ptr_t& target){
			int stealth = 0;
			switch(m_stealth_level) {
				default:
				case stealth_levels_t::GHOST_STEALTH_NONE:
					m_player->sendln("It looks like you still need to train that skill");
					return;
				case stealth_levels_t::GHOST_STEALTH_INITIATE:
					stealth = dice(1, 8) + 1 + (m_player->level() / 4);
					break;

				case stealth_levels_t::GHOST_STEALTH_FAMILIAR:
					stealth = dice(3, 8) + 3 + (m_player->level() / 4);
					break;

				case stealth_levels_t::GHOST_STEALTH_MASTER:
					stealth = 100 + dice(3, 8);
					break;
			}
			mods::player_utils::change_visibility(target,- stealth);
		}
		void ghost::feign_death_done(){
			m_player->set_position(POS_STANDING);
			m_player->sendln("You stand up and dust yourself off.");
		}
		std::pair<int16_t,std::string> ghost::feign_death(){
			uint16_t duration = 0;
			switch(m_feign_death_level){
				default:
				case feign_death_levels_t::GHOST_FEIGN_DEATH_NONE:
				case feign_death_levels_t::GHOST_FEIGN_DEATH_INITIATE:
					duration = GHOST_FEIGN_DEATH_INITIATE_DURATION() + dice(3,6);
					break;
				case feign_death_levels_t::GHOST_FEIGN_DEATH_FAMILIAR:
					duration = GHOST_FEIGN_DEATH_FAMILIAR_DURATION() + dice(3,6);
					break;
				case feign_death_levels_t::GHOST_FEIGN_DEATH_MASTER:
					duration =  GHOST_FEIGN_DEATH_MASTER_DURATION() + dice(3,6);
					break;
			}
			m_player->position() = POS_DEAD;
			player_ptr_t attacker = m_player->fighting();
			if(attacker){
				feedback_t f;
				f.damage_event = damage_event_t::TARGET_DEAD_EVENT;
				f.damage = 1;
				f.hits = 1;
				f.injured = {};
				f.damage_info.emplace_back(m_player->uuid(),0,0);
				f.from_direction = NORTH;
				f.attacker = m_player->uuid();
				attacker->damage_event(f);
				stop_fighting(m_player->cd());
				stop_fighting(attacker->cd());
			}
			m_player->block_for(duration, mods::deferred::EVENT_PLAYER_FINISHES_FEIGN_DEATH, 0);
			m_player->set_position(POS_DEAD);
			return {1, MSG_YOU_FEIGN_DEATH()};
		}
		std::pair<int16_t,std::string> ghost::summon_extraction(room_rnum room){
			return {0,""};
		}
		/** requires drone assisted sniping mode */
		std::pair<int16_t,std::string> ghost::xray_shot(){

			return {0,""};
		}
		/* constructors and destructors */
		ghost::ghost(){
			m_player = nullptr;
		}
		std::shared_ptr<ghost> create_ghost(player_ptr_t &in_player){
			return std::move(std::make_shared<ghost>(in_player));
		}
};
ACMD(do_go_dark){
	PLAYER_CAN("ghost.go_dark");
}
ACMD(do_conceal){
	PLAYER_CAN("ghost.conceal");

}
ACMD(do_feign_death){
	PLAYER_CAN("ghost.feign_death");
	auto msg = std::get<1>(player->ghost()->feign_death());
	player->sendln(msg);
}
ACMD(do_summon_extraction){
	PLAYER_CAN("ghost.summon_extraction");

}
ACMD(do_xray_shot){
	PLAYER_CAN("ghost.xray_shot");

}
