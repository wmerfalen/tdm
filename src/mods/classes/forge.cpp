#include "forge.hpp"
#include "../orm/inventory.hpp"
#include "../affects.hpp"
#include "../player-utils.hpp"
#include "../object-utils.hpp"
#include "../replenish.hpp"
#include "../interpreter.hpp"
#include "../demolitions.hpp"
#include "../date-time.hpp"
#include "../weapons/damage-types.hpp"

extern void stop_fighting(char_data *ch);
namespace mods::orm::inventory {
	extern int16_t flush_player(player_ptr_t & player);
};
namespace mods::classes {
	void forge_advance_level(player_ptr_t& player){
		/** TODO fill me in */
		player->send("[stub] file:%s line:%d\r\n",__FILE__,__LINE__);

	}
	void forge::init(){
		m_player = nullptr;

	}
	int16_t forge::save(){
		return this->m_orm.save();
	}
	forge::forge(player_ptr_t p){
		this->init();
		load_by_player(p);
	}
	player_ptr_t 	forge::player(){
		return m_player;
	}
	void forge::set_player(player_ptr_t p){
		m_player = p;
	}
	int16_t forge::new_player(player_ptr_t &player){
		auto db_id = m_orm.initialize_row(player);
		if(db_id == 0){
			return -2;
		}
		load_by_player(player);
		return 0;
	}
	int16_t forge::load_by_player(player_ptr_t & player){
		set_player(player);
		auto result = m_orm.load_by_player(player->db_id());
		if(result < 0){
			report(CAT("forge::load_by_player unable to load forge class by player id: ",(player->db_id()),".. return status: ",(result)));
			return result;
		}
		return result;
	}
	/* constructors and destructors */
	forge::forge(){
		m_player = nullptr;
	}
	void forge::replenish(){
		static uint8_t call_count = 0;
		++call_count;
	}
	std::shared_ptr<forge> create_forge(player_ptr_t &in_player){
		return std::move(std::make_shared<forge>(in_player));
	}
	std::string forge::get_proficiency_by_name(std::string_view prof) const {
			if(!prof.compare("craft_shotgun_underbarrel")){ return mods::util::proficiency_to_string((int)m_craft_shotgun_underbarrel_level); }
			if(!prof.compare("craft_incendiary_underbarrel")){ return mods::util::proficiency_to_string((int)m_craft_incendiary_underbarrel_level); }
			if(!prof.compare("craft_flash_underbarrel")){ return mods::util::proficiency_to_string((int)m_craft_flash_underbarrel_level); }
			if(!prof.compare("craft_silencer")){ return mods::util::proficiency_to_string((int)m_craft_silencer_level); }
			if(!prof.compare("craft_ammunition")){ return mods::util::proficiency_to_string((int)m_craft_ammunition_level); }
			if(!prof.compare("craft_armor_plating")){ return mods::util::proficiency_to_string((int)m_craft_armor_plating_level); }
			if(!prof.compare("craft_high_velocity_ammo")){ return mods::util::proficiency_to_string((int)m_craft_high_velocity_ammo_level); }
			if(!prof.compare("dismantle_weapon")){ return mods::util::proficiency_to_string((int)m_dismantle_weapon_level); }
			if(!prof.compare("trade_attribute")){ return mods::util::proficiency_to_string((int)m_trade_attribute_level); }
			if(!prof.compare("repair_armor")){ return mods::util::proficiency_to_string((int)m_repair_armor_level); }
			if(!prof.compare("repair_weapon")){ return mods::util::proficiency_to_string((int)m_repair_weapon_level); }
			if(!prof.compare("reinforce_weapon")){ return mods::util::proficiency_to_string((int)m_reinforce_weapon_level); }
		return "UNKNOWN SKILL";
	}
	std::string forge::skill_screen() const {
		std::string screen;
		for(auto str : {
			"craft_shotgun_underbarrel",
			"craft_incendiary_underbarrel",
			"craft_flash_underbarrel",
			"craft_silencer",
			"craft_ammunition",
			"craft_armor_plating",
			"craft_high_velocity_ammo",
			"dismantle_weapon",
			"trade_attribute",
			"repair_armor",
			"repair_weapon",
			"reinforce_weapon"
			}){
			screen += CAT("{yel}",str,"{/yel}\t->\t",get_proficiency_by_name(str),"\r\n");
		}
		return screen;
	}
};
namespace mods::class_abilities::forge {
	void init(){
		//mods::interpreter::add_command("use_flash_underbarrel", POS_RESTING, do_use_flash_underbarrel, 0,0);
		//mods::help::register_help_command_with_permission("forge.plant_claymore",plant_claymore_usage,LVL_MORTAL);
	}
};
