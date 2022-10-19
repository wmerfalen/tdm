#include "medic.hpp"
#include "../player.hpp"

extern void send_to_room_except(room_rnum room, player_ptr_t except_me, const char *messg, ...);
extern void send_to_room_except(room_rnum room, const std::vector<char_data*>& except, const char *messg, ...);
namespace mods::classes {
	template <typename TPlayer>
	int scaled_heal(TPlayer& target) {
		return (33 * (target->level() * 0.5));
	}
	void medic::replenish() {

	}
	uint8_t& medic::stim_pistol_ammo() {
		return m_stim_pistol_ammo;
	}
	void medic::init() {
		m_stim_pistol_ammo = medic::DEFAULT_STARTING_STIM_PISTOL_AMMO;
		using s = base::ability_data_t::skillset_t;
		m_abilities = {
			{STIM_SHOT,"stim","Stim Shot",s::MEDICAL,&m_stim_shot,0},
			{PARASITIC_AMMO,"ps","Parasitic Ammo",s::MEDICAL,&m_parasitic_ammo,0},
			{ADRENALINE_SHOT,"adrenal","Adrenaline Shot",s::MEDICAL,&m_adrenaline_shot,0},
			{DRONE_HEALER,"drone","Drone Healer",s::ELECTRONICS,&m_drone_healer,0},
			{HEARTBEAT_SENSOR,"hbs","Heartbeat Sensor",s::ELECTRONICS,&m_heartbeat_sensor,0},
			{EMERGENCY_EVAC,"evac","Emergency Evac",s::STRATEGY,&m_emergency_evac,0},
			{DEUTERIUM_SHOTGUN_UNDERBARREL,"dts","Deuterium Underbarrel",s::DEMOLITIONS,&m_deuterium_shotgun_ub,0},
			{CORROSIVE_AGENT,"agent","Corrosive Agent",s::DEMOLITIONS,&m_corrosive_agent,0},
			{SEAL_ROOM,"seal","Seal Room",s::DEMOLITIONS,&m_seal_room,0},
			{SHOTGUN_SNIPER,"ss","Shotgun Sniper",s::SNIPING,&m_shotgun_sniper,0},
			{HAZARDOUS_WASTE_SHOT,"hazard","Hazardous Waste Shot",s::SNIPING,&m_hazardous_waste_shot,0},
			{SEDATIVE_HYPNOTIC,"sed","Sedative Hypnotic",s::SNIPING,&m_sedative_hypnotic,0},
			{ARMOR_BONUS,"armor","Armor Bonus",s::ARMOR,&m_armor_bonus,0},
			{NO_FACTOR_ARMOR,"nof","No Factor Armor",s::ARMOR,&m_no_factor_armor,0},
			{SURGICAL_PRECISION,"surg","Surgical Precision",s::WEAPON_HANDLING,&m_surgical_precision,0},
		};
	}
	void medic::heal_player(player_ptr_t& target) {
		if(m_stim_pistol_ammo == 0) {
			m_player->psendln("Out of ammo.");
			return;
		}

		if(target->cd() == m_player->cd()) {
			const char* self_name = (m_player->sex() == SEX_MALE ? "himself" : "herself");
			const char* short_self_name = (m_player->sex() == SEX_MALE ? "his" : "her");
			m_player->psendln("You aim your stim pistol at yourself...");
			send_to_room_except(m_player->room(),
			    m_player,
			    "%s aims a stim pistol at %s\r\n",
			    m_player->name().c_str(),self_name);
			m_player->psendln("You fire your stim pistol.");
			send_to_room_except(m_player->room(),
			    m_player,
			    "%s fires a stim pistol into %s arm\r\n",
			    m_player->name().c_str(),short_self_name);
			auto amount = scaled_heal(m_player);
			m_player->hp() += amount;
			m_player->sendln(CAT("{grn}You gain {blu}", amount,"{/blu} {grn}hit points.{/grn}"));
			return;
		}

		*m_player << "You aim your stim pistol at " <<
		    target->name() << " carefully...\r\n";
		const char* gender = (m_player->cd()->player.sex == SEX_MALE ? "his" : "her");
		send_to_room_except(m_player->room(),
		{m_player->cd(),target->cd()},
		"%s aims a stim pistol at %s\r\n",
		m_player->name().c_str(),
		target->name().c_str()
		);
		*target << m_player->name() << " aims " << gender <<
		    " stim pistol at you...\r\n";
		*m_player << "You fire your stim pistol.\r\n";
		*target << m_player->name() << " fires!\r\n";
		send_to_room_except(m_player->room(),
		{m_player->cd(),target->cd()},
		"%s fires a stim pistol at %s\r\n",
		m_player->name().c_str(),
		target->name().c_str()
		);
		auto amount = scaled_heal(target);
		target->hp() += amount;
		*target << "You gain " << amount << " hit points.\r\n";
	}
	std::shared_ptr<medic> create_medic(player_ptr_t& in_player) {
		return std::move(std::make_shared<medic>(in_player));
	}
	int16_t medic::new_player(player_ptr_t& player, primary_choice_t primary_choice) {
		auto db_id = m_orm.initialize_row(player);
		if(db_id == 0) {
			report(std::vector<std::string> {"medic::new_player. failed to initialize_row for player:",player->name().c_str()});
			return -2;
		}
		player->set_db_id(db_id);
		load_by_player(player);
		initialize_skills_for_player(player);
		return 0;
	}
	int16_t medic::load_by_player(player_ptr_t& player) {
		m_player = player;
		auto result = m_orm.load_by_player(player->db_id());
		if(result < 0) {
			report({"unable to load medic class by player id: ",std::to_string(player->db_id()),".. return status: ",std::to_string(result),"player:",player->name().c_str()});
			return -100 - result;
		}
		initialize_skills_for_player(player);
		return result;
	}
	medic::medic() {
		this->init();
	}
	medic::medic(player_ptr_t p) {
		this->init();
		load_by_player(p);
	}
	player_ptr_t 	medic::player() {
		return m_player;
	}
};
