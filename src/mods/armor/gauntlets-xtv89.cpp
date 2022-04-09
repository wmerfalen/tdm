#include "gauntlets-xtv89.hpp"
/** db.h for read_object() */
#include "../../db.h"
#include "../weapon.hpp"
#include "../blind.hpp"
#include "../rand.hpp"
#include "../player.hpp"
#include "../interpreter.hpp"
#include "../../comm.h"
#include "../melt.hpp"
#include "../projectile.hpp"

#define __MENTOC_XTV89_DEBUG__ 1
#ifdef __MENTOC_XTV89_DEBUG__
#define xtv_debug(A) std::cerr << "[XTV89::DEBUG]: " << A << "\n";
#else
#define xtv_debug(A) ;;
#endif
extern void send_to_room(room_rnum room, const char *messg, ...);
namespace mods::armor {
	static std::map<uuid_t,xtv89_ptr_t> xtv89_list;
	xtv89_ptr_t make_xtv89(obj_ptr_t& object) {
		auto p = std::make_shared<mods::armor::xtv89>(object);
		xtv89_list.insert({p->uuid(),p});
		return p;
	}

	const std::vector<std::string>& xtv89::get_usage_screen() {
		static std::vector<std::string> screen;
		static std::size_t list_size = 0;
		static bool set = false;
		if(list_size != xtv89_list.size() || set == false) {
			screen.clear();
			screen.emplace_back(CAT("{grn}--------------------------------------------------------|{/grn}"));
			screen.emplace_back(CAT("{grn}Field                         | Value                   |{/grn}"));
			screen.emplace_back(CAT("{grn}------------------------------+-------------------------|{/grn}"));
			screen.emplace_back(CAT("{grn}xtv89_list.size():            | ",xtv89_list.size(),"{/grn}"));
			screen.emplace_back(CAT("{grn}------------------------------+-------------------------|{/grn}"));
			list_size = xtv89_list.size();
			set = true;
		}
		return screen;
	}
	xtv89_ptr_t find_xtv89(const uuid_t& uuid) {
		auto it = xtv89_list.find(uuid);
		if(it == xtv89_list.end()) {
			return nullptr;
		}
		return it->second;
	}
	namespace xtv89_commands {
		ADMINCMD(do_xtv89) {
			DO_HELP("admin:xtv89");
			ADMIN_REJECT();
			auto d = create_object(ITEM_ARMOR,xtv89::YAML_FILE.data());
			player->equip(d,WEAR_ARMS);
			ADMIN_DONE();
		}
		ADMINCMD(do_xtv89_usage) {
			DO_HELP("admin:xtv89:usage");
			ADMIN_REJECT();
			for(const auto& line : xtv89::get_usage_screen()) {
				player->sendln(line);
			}
			ADMIN_DONE();
		}
		void init() {
			ADD_ADMIN_COMMAND("admin:xtv89:usage",do_xtv89_usage);
			ADD_ADMIN_COMMAND("admin:xtv89",do_xtv89);
		}
	};
	void erase_xtv89(const uuid_t& uuid) {

		auto d = find_xtv89(uuid);
		if(d) {
			d.reset();
			xtv89_list.erase(uuid);
		}
		xtv_debug("xtv89_list size: " << xtv89_list.size());
	}

	xtv89::~xtv89() {
		m_object.reset();
		m_event_subscriptions.clear();
		m_damaged_enemy_list.clear();
		xtv_debug("[xtv89::~xtv89] --> destructor HIT");
	}
	xtv89::xtv89(obj_ptr_t& object) {
		m_object = object;
		m_object->armor()->attributes->unique_armor_id = mods::armor::unique_armor_id::XTV89;
	}

	player_ptr_t xtv89::player_upkeep() {
		if(!m_object) {
			return nullptr;
		}
		return ptr(m_object->worn_by);
	}
	void xtv89::hits_armor_event(player_ptr_t attacker) {
		if(m_player) {
			m_player->sendln("{grn}Hits armor event{/grn}");
		}
		if(!attacker) {
			return;
		}
	}
	const room_rnum& xtv89::player_room() {
		return m_player->room();
	}
	void xtv89::dispatch_event(damage_event_t event,player_ptr_t attacker) {
		m_player = player_upkeep();
		if(!m_player) {
			return;
		}
		switch(event) {
			case damage_event_t::YOU_HIT_ARMOR:
				hits_armor_event(attacker);
				return;
			default:
				return;
		}
	}
<<<<<<< HEAD
	void xtv89::dispatch_bc89_synergy(damage_event_t event, player_ptr_t attacker, obj_ptr_t& vest) {
		auto roll = dice(stats::create_shotgun_charge_chance);
		if(roll) {
			auto feedback = add_shotgun_charge();
			if(feedback.status) {
				m_player->sendln(CAT("{grn}",feedback.message,"{/grn}"));
			}
		}
	}
	user_feedback xtv89::add_shotgun_charge() {
		if(m_shotgun_charges.size() >= xtv89::stats::max_shotgun_charges) {
			return {false,"At max shotgun charges."};
		}
		m_shotgun_charges.push(shotgun_charge_t());
		return {true,CAT("A shotgun charge has been added to your ",m_object->name.c_str())};
	}
	user_feedback xtv89::consume_shotgun_charge() {
		if(m_shotgun_charges.size() == 0) {
			return {false,"***CLICK*** It's empty!"};
		}
		m_shotgun_charges.pop();
		return {true,"A shotgun charge gets dispersed!"};
	}
=======
>>>>>>> 63c98e4e (- use the bc89::YAML_FILE string instead of hardcoded yamls)
};
