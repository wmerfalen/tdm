#include "pyrexia.hpp"
#include "../orm/inventory.hpp"
#include "../affects.hpp"
#include "../interpreter.hpp"

extern void stop_fighting(char_data *ch);
namespace mods::orm::inventory {
	extern int16_t flush_player(player_ptr_t & player);
};
namespace mods::classes {
	/** KEEP ME */
	void pyrexia_advance_level(player_ptr_t& player){
		/** TODO fill me in */
		player->send("[stub] file:%s line:%d\r\n",__FILE__,__LINE__);
	}
		pyrexia::pyrexia(player_ptr_t p){
			load_by_player(p);
		}
		player_ptr_t 	pyrexia::player(){
			return m_player;
		}
		void pyrexia::set_player(player_ptr_t p){
			m_player = p;
		}
		int16_t pyrexia::new_player(player_ptr_t &player){
			auto db_id = m_orm.initialize_row(player);
			if(db_id == 0){
				return -2;
			}
			load_by_player(player);
			return 0;
		}
		int16_t pyrexia::load_by_player(player_ptr_t & player){
			auto result = m_orm.load_by_player(player->db_id());
			if(result < 0){
				report(CAT("pyrexia::load_by_player unable to load pyrexia class by player id: ",(player->db_id()),".. return status: ",(result)));
				return result;
			}
			set_player(player);

			//player->equip(create_object(ITEM_RIFLE,"czp10.yml"),WEAR_SECONDARY);
			return result;
		}
		/* constructors and destructors */
		pyrexia::pyrexia(){
			m_player = nullptr;
			m_fire_nade_charges = PYREXIA_FIRE_NADE_CHARGES_DEFAULT();
			m_fire_nade_level = skill_familiarity_t::INITIATE;
			switch(m_fire_nade_level){
				default:
				case skill_familiarity_t::INITIATE:
					m_fire_nade_charges = PYREXIA_FIRE_NADE_CHARGES_INITIATE();
					break;
				case skill_familiarity_t::FAMILIAR:
					m_fire_nade_charges = PYREXIA_FIRE_NADE_CHARGES_FAMILIAR();
					break;
				case skill_familiarity_t::MASTER:
					m_fire_nade_charges = PYREXIA_FIRE_NADE_CHARGES_MASTER();
					break;
			}
		}
		std::shared_ptr<pyrexia> create_pyrexia(player_ptr_t &in_player){
			return std::move(std::make_shared<pyrexia>(in_player));
		}
		std::pair<bool,std::string> pyrexia::throw_fire_nade(uint8_t direction,uint8_t depth){
			int replenish_ticks = 0;
			switch(m_fire_nade_level){
				default:
				case skill_familiarity_t::INITIATE:
					replenish_ticks = PYREXIA_FIRE_NADE_RECHARGE_TICKS_INITIATE();
					break;
				case skill_familiarity_t::FAMILIAR:
					replenish_ticks = PYREXIA_FIRE_NADE_RECHARGE_TICKS_FAMILIAR();
					break;
				case skill_familiarity_t::MASTER:
					replenish_ticks = PYREXIA_FIRE_NADE_RECHARGE_TICKS_MASTER();
					break;
			}
			if(m_fire_nade_charges){
				auto player_uuid = m_player->uuid();
				mods::globals::defer_queue->push(replenish_ticks, [player_uuid]() {
					auto player = ptr_by_uuid(player_uuid);
					player->pyrexia()->replenish_charge();
				});
			}

			return {0,""};
		}
		void pyrexia::replenish_charge(){
			++m_fire_nade_charges;
			m_player->sendln(PYREXIA_REPLENISH_MESSAGE());
		}
		std::pair<bool,std::string> pyrexia::drench_object_in_gasoline(uuid_t npc){
			switch(m_drench_object_in_gasoline_level){
				default:
				case skill_familiarity_t::NONE:
					break;
				case skill_familiarity_t::INITIATE:
					break;
				case skill_familiarity_t::FAMILIAR:
					break;
				case skill_familiarity_t::MASTER:
					break;
			}

			return {0,""};
		}
		std::pair<bool,std::string> pyrexia::drench_room_in_gasoline(uuid_t npc){
			switch(m_drench_room_in_gasoline_level){
				default:
				case skill_familiarity_t::NONE:
					break;
				case skill_familiarity_t::INITIATE:
					break;
				case skill_familiarity_t::FAMILIAR:
					break;
				case skill_familiarity_t::MASTER:
					break;
			}
			return {0,""};
		}
		std::pair<bool,std::string> pyrexia::convert_to_fire_nade(obj_ptr_t& object){
			switch(m_convert_to_fire_nade_level){
				default:
				case skill_familiarity_t::NONE:
					break;
				case skill_familiarity_t::INITIATE:
					break;
				case skill_familiarity_t::FAMILIAR:
					break;
				case skill_familiarity_t::MASTER:
					break;
			}
			return {0,""};
		}
		std::pair<bool,std::string> pyrexia::attach_incendiary_grenade_launcher(obj_ptr_t& weapon){
			switch(m_attach_incendiary_grenade_launcher_level){
				default:
				case skill_familiarity_t::NONE:
					break;
				case skill_familiarity_t::INITIATE:
					break;
				case skill_familiarity_t::FAMILIAR:
					break;
				case skill_familiarity_t::MASTER:
					break;
			}
			return {0,""};
		}
		std::pair<bool,std::string> pyrexia::attach_flamethrower(obj_ptr_t& weapon){
			switch(m_attach_flamethrower_level){
				default:
				case skill_familiarity_t::NONE:
					break;
				case skill_familiarity_t::INITIATE:
					break;
				case skill_familiarity_t::FAMILIAR:
					break;
				case skill_familiarity_t::MASTER:
					break;
			}
			return {0,""};
		}
		std::pair<bool,std::string> pyrexia::create_wall_of_fire(uint8_t direction){
			switch(m_create_wall_of_fire_level){
				default:
				case skill_familiarity_t::NONE:
					break;
				case skill_familiarity_t::INITIATE:
					break;
				case skill_familiarity_t::FAMILIAR:
					break;
				case skill_familiarity_t::MASTER:
					break;
			}
			return {0,""};
		}
		std::pair<bool,std::string> pyrexia::seal_off_room_with_wall_of_fire(room_rnum room_id){
			switch(m_seal_off_room_with_wall_of_fire_level){
				default:
				case skill_familiarity_t::NONE:
					break;
				case skill_familiarity_t::INITIATE:
					break;
				case skill_familiarity_t::FAMILIAR:
					break;
				case skill_familiarity_t::MASTER:
					break;
			}
			return {0,""};
		}
		uint16_t pyrexia::roll_incendiary_shot(obj_ptr_t& attacking_weapon,uuid_t target){
			feedback_t inc_shot;
			inc_shot.damage_event = damage_event_t::YOU_GOT_HIT_BY_INCENDIARY_AMMO;
			inc_shot.damage_event = damage_event_t::YOU_INFLICTED_INCENDIARY_AMMO;
			inc_shot.damage = 0;
			inc_shot.hits = 3;
			inc_shot.injured.emplace_back(target);
			inc_shot.damage_info.emplace_back(target,inc_shot.damage,m_player->room());
			inc_shot.from_direction = NORTH;
			inc_shot.attacker = m_player->uuid();
			return inc_shot.damage;
		}
		uint16_t pyrexia::roll_heat_accumulation_hp_gain(obj_ptr_t& attacking_weapon){
			return 15;
		}

};

ACMD(do_fire_nade){
	PLAYER_CAN("pyrexia.fire_nade");
	DO_HELP("fire_nade");
	auto vec_args = PARSE_ARGS();
	if(vec_args.size() < 2){
		player->sendln("usage: fire_nade <direction> <distance>");
		return;
	}
	direction_t dir = to_direction(vec_args[0]).second;
	int distance = mods::util::stoi(vec_args[1]).value_or(-1);
	if(distance <= 0){
		player->sendln("Invalid distance.\r\nusage: fire_nade <direction> <distance>");
		return;
	}
	player->pyrexia()->throw_fire_nade(dir,distance);
}
