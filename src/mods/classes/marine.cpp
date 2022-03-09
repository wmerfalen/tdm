#include "marine.hpp"
#include "../orm/inventory.hpp"

namespace mods::classes {
	marine::marine() {
		this->init();
	}
	void marine::init() {
		using s = base::ability_data_t::skillset_t;
		m_abilities = create_abilities({
			{TRACKING_SHOT,"ts","Tracking Shot",s::SNIPING,&m_tracking_shot},
			{LIGHT_BANDAGE,"lb","Light Bandage",s::MEDICAL,&m_light_bandage},
			{SUTURE,"suture","Suture",s::MEDICAL,&m_suture},
			{ADRENALINE_SHOT,"as","Adrenaline Shot",s::MEDICAL,&m_adrenaline_shot},
			{EMP_NADE,"emp","EMP Grenade",s::DEMOLITIONS,&m_emp_nade},
			{CHAFF_NADE,"chaff","Chaff Grenade",s::DEMOLITIONS,&m_chaff_nade},
			{SENSOR_NADE,"sensor","Sensor Grenade",s::INTELLIGENCE,&m_sensor_nade},
			{UB_SHOTGUN,"ubs","Underbarrel Shotgun",s::DEMOLITIONS,&m_ub_shotgun},
			{UB_FRAG,"ubf","Underbarrel Nade Launcher",s::DEMOLITIONS,&m_ub_frag},
			{GUIDED_MISSILE,"gm","Guided Missile",s::DEMOLITIONS,&m_guided_missile},
			{TARGET_LIMB,"limb","Target Limb",s::SNIPING,&m_target_limb},
			{PLANT_CLAYMORE,"claymore","Plant Claymore",s::DEMOLITIONS,&m_plant_claymore},
			{SHRAPNEL_CLAYMORE,"smine","Shrapnel Claymore",s::DEMOLITIONS,&m_plant_shrapnel_claymore},
			{CORROSIVE_CLAYMORE,"cmine","Corrosive Claymore",s::DEMOLITIONS,&m_plant_corrosive_claymore},
			{XRAY_SHOT,"xray","X-Ray Shot",s::SNIPING,&m_xray_shot},
			{REQUEST_RECON,"recon","Request Recon",s::INTELLIGENCE,&m_request_recon},
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
