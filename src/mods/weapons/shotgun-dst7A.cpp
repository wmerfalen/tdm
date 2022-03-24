#include "shotgun-dst7A.hpp"
/** db.h for read_object() */
#include "../../db.h"
#include "../weapon.hpp"
#include "../rand.hpp"
#include "../player.hpp"
#include "../interpreter.hpp"
#include "../../comm.h"
#include "../melt.hpp"

#define __MENTOC_DST7A_DEBUG__ 1
extern void send_to_room(room_rnum room, const char *messg, ...);
namespace mods::weapons::shotgun {
	static std::map<uuid_t,dst7A_ptr_t> dst7a_list;
	dst7A_ptr_t make_dst7a(obj_ptr_t& object) {
		auto p = std::make_shared<mods::weapons::shotgun::dst7A>(object);
		dst7a_list.insert({p->uuid(),p});
		return p;
	}

	dst7A_ptr_t find_dst7a(const uuid_t& uuid) {
		auto it = dst7a_list.find(uuid);
		if(it == dst7a_list.end()) {
			return nullptr;
		}
		return it->second;
	}
	namespace dst7a_commands {
		ADMINCMD(do_dst7a) {
			DO_HELP("admin:dst7a");
			ADMIN_REJECT();
			auto d = create_object(ITEM_RIFLE,"dst7a.yml");
			player->equip(d,WEAR_PRIMARY);
			ADMIN_DONE();
		}
		void init() {
			ADD_ADMIN_COMMAND("admin:dst7a",do_dst7a);
		}
	};
	void erase_dst7a(const uuid_t& uuid) {

		auto d = find_dst7a(uuid);
		if(d) {
			d.reset();
			dst7a_list.erase(uuid);
		}
#ifdef __MENTOC_DST7A_DEBUG__
		std::cerr << "dst7A_list size: " << dst7a_list.size() << "\n";
#endif
	}

	dst7A::~dst7A() {
		m_object.reset();
		m_event_subscriptions.clear();
		m_damaged_enemy_list.clear();
#ifdef __MENTOC_DST7A_DEBUG__
		std::cerr << "[dst7A::~dst7A]\n";
#endif
	}
	dst7A::dst7A(obj_ptr_t& object) {
		m_object = object;
		m_object->rifle()->attributes->unique_weapon_id = mods::weapons::unique_weapon_id::DST7A;
	}

	std::vector<uuid_t>& dst7A::damaged_enemy_list() {
		return m_damaged_enemy_list;
	}
	player_ptr_t dst7A::player_upkeep() {
		if(!m_object) {
			return nullptr;
		}
		return ptr(m_object->worn_by);
	}
	void dst7A::pump_outward_event() {
		bool hide_invisible = 1;
		m_player->sendln("You pump the forestock on your {blu}DST7A{/blu} forward with an audible ** SHLKK **");
		act("$n pumps the forestock on $s {blu}DST7A{/blu} forward with an audible ** SHLKK **", hide_invisible, m_player->cd(), 0, 0, TO_ROOM);
		send_to_room(player_room(),"A {red}burning{/red} {grn}acidic{/grn} shell pops up into the air and hits the ground disintegrating immediately...");
		auto f =  dice(dst7A::stats::move_points_per_outward_pump_buff);
		if(f) {
			m_player->move() += f;
			m_player->sendln(CAT("{grn}[+",f," mov]{/grn}"));
		}
	}
	void dst7A::pump_inward_event() {
		bool hide_invisible = 1;
		m_player->sendln("You pump the forestock on your {blu}DST7A{/blu} toward your body with an audible ** SHLKK **");
		act("$n pumps the forestock on $s {blu}DST7A{/blu} toward $m body with an audible ** SHLKK **", hide_invisible, m_player->cd(), 0, 0, TO_ROOM);
		auto f = dice(dst7A::stats::mana_points_per_inward_pump_buff);
		if(f) {
			m_player->mana() += f;
			m_player->sendln(CAT("{grn}[+",f," mana]{/grn}"));
		}
	}
	void dst7A::hits_flesh_event(player_ptr_t victim) {
		if(!victim) {
			return;
		}
		mods::melt::melt_damage(mods::melt::melt_damage_t{m_player->uuid(),victim->uuid(),dst7A::stats::base_damage_dice_additional,dst7A::stats::base_damage});
	}
	const room_rnum& dst7A::player_room() {
		return m_player->room();
	}
	void dst7A::dispatch_event(damage_event_t event,player_ptr_t victim) {
		m_player = player_upkeep();
		if(!m_player) {
			return;
		}
		switch(event) {
			case damage_event_t::RELOAD_SINGLE_SHOT_INTO_SHOTGUN:
				reload_event();
				return;
			case damage_event_t::SHOTGUN_PUMP_INWARD:
				pump_inward_event();
				return;
			case damage_event_t::SHOTGUN_PUMP_OUTWARD:
				pump_outward_event();
				return;
			case damage_event_t::YOU_HIT_FLESH:
				hits_flesh_event(victim);
				return;
			default:
				return;
		}
	}
	void dst7A::reload_event() {
		std::cout << "[DST7A] a legend reloads...\n";
	}
};
