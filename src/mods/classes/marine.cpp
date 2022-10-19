#include "marine.hpp"
#include "../orm/inventory.hpp"

namespace mods::classes {
	marine::marine() {
		this->init();
	}
	void marine::init() {
		using s = base::ability_data_t::skillset_t;
		uint16_t M = 0;
		m_abilities = create_abilities({
			{TRACKING_SHOT,"ts","Tracking Shot",s::SNIPING,&m_tracking_shot,M},
			{LIGHT_BANDAGE,"lb","Light Bandage",s::MEDICAL,&m_light_bandage,M},
			{SUTURE,"suture","Suture",s::MEDICAL,&m_suture,M},
			{ADRENALINE_SHOT,"as","Adrenaline Shot",s::MEDICAL,&m_adrenaline_shot,M},
			{EMP_NADE,"emp","EMP Grenade",s::DEMOLITIONS,&m_emp_nade,M},
			{CHAFF_NADE,"chaff","Chaff Grenade",s::DEMOLITIONS,&m_chaff_nade,M},
			{SENSOR_NADE,"sensor","Sensor Grenade",s::INTELLIGENCE,&m_sensor_nade,M},
			{UB_SHOTGUN,"ubs","Underbarrel Shotgun",s::DEMOLITIONS,&m_ub_shotgun,M},
			{UB_FRAG,"ubf","Underbarrel Nade Launcher",s::DEMOLITIONS,&m_ub_frag,M},
			{GUIDED_MISSILE,"gm","Guided Missile",s::DEMOLITIONS,&m_guided_missile,M},
			{TARGET_LIMB,"limb","Target Limb",s::SNIPING,&m_target_limb,M},
			{PLANT_CLAYMORE,"claymore","Plant Claymore",s::DEMOLITIONS,&m_plant_claymore,M},
			{SHRAPNEL_CLAYMORE,"smine","Shrapnel Claymore",s::DEMOLITIONS,&m_plant_shrapnel_claymore,M},
			{CORROSIVE_CLAYMORE,"cmine","Corrosive Claymore",s::DEMOLITIONS,&m_plant_corrosive_claymore,M},
			{XRAY_SHOT,"xray","X-Ray Shot",s::SNIPING,&m_xray_shot,M},
			{REQUEST_RECON,"recon","Request Recon",s::INTELLIGENCE,&m_request_recon,M},
		});
	}

	void marine::replenish() {

	}
	marine::marine(player_ptr_t p) {
		this->init();
		load_by_player(p);
	}
	int16_t marine::new_player(player_ptr_t& player, primary_choice_t primary_choice) {
		if(primary_choice == primary_choice_t::NONE) {
			report("marine::new_player. got primary_choice of zero. defaulting to M16A4");
			primary_choice = primary_choice_t::M16A4;
		}
		auto db_id = m_orm.initialize_row(player);
		if(db_id == 0) {
			return -2;
		}
		load_by_player(player);
		return 0;
	}
	int16_t marine::load_by_player(player_ptr_t& player) {
		m_player = player;
		auto result = m_orm.load_by_player(player->db_id());
		if(result < 0) {
			report(CAT("unable to load marine class by player id: ",(player->db_id()),".. return status: ",(result),"player:",player->name().c_str()));
			return -100 - result;
		}
		initialize_skills_for_player(player);
		return result;
	}
	int16_t marine::save() {
		return m_orm.save();
	}
	std::shared_ptr<marine> create_marine(player_ptr_t& in_player) {
		return std::move(std::make_shared<marine>(in_player));
	}
};

namespace mods::class_abilities::marine {
	void init() {

	}
};
