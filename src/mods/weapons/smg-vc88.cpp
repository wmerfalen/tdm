#include "smg-vc88.hpp"
/** db.h for read_object() */
#include "../../db.h"
#include "../weapon.hpp"
#include "../rand.hpp"
#include "../player.hpp"
#include "../interpreter.hpp"
#include "../../comm.h"
#include "../melt.hpp"

#define __MENTOC_VC88_DEBUG__ 1
extern void send_to_room(room_rnum room, const char *messg, ...);
namespace mods::weapons::smg {
	static std::map<uuid_t,vc88_ptr_t> vc88_list;
	vc88_ptr_t make_vc88(obj_ptr_t& object) {
		auto p = std::make_shared<mods::weapons::smg::vc88>(object);
		vc88_list.insert({p->uuid(),p});
		return p;
	}

	vc88_ptr_t find_vc88(const uuid_t& uuid) {
		auto it = vc88_list.find(uuid);
		if(it == vc88_list.end()) {
			return nullptr;
		}
		return it->second;
	}
	namespace vc88_commands {
		ADMINCMD(do_vc88) {
			DO_HELP("admin:vc88");
			ADMIN_REJECT();
			auto d = create_object(ITEM_RIFLE,"vc88.yml");
			player->equip(d,WEAR_PRIMARY);
			ADMIN_DONE();
		}
		void init() {
			ADD_ADMIN_COMMAND("admin:vc88",do_vc88);
		}
	};
	void erase_vc88(const uuid_t& uuid) {

		auto d = find_vc88(uuid);
		if(d) {
			d.reset();
			vc88_list.erase(uuid);
		}
#ifdef __MENTOC_VC88_DEBUG__
		std::cerr << "vc88_list size: " << vc88_list.size() << "\n";
#endif
	}

	vc88::~vc88() {
		m_object.reset();
		m_event_subscriptions.clear();
		m_damaged_enemy_list.clear();
#ifdef __MENTOC_DST7A_DEBUG__
		std::cerr << "[vc88::~vc88]\n";
#endif
	}
	vc88::vc88(obj_ptr_t& object) {
		m_object = object;
		m_object->rifle()->attributes->unique_weapon_id = mods::weapons::unique_weapon_id::VC88;
	}

	std::vector<uuid_t>& vc88::damaged_enemy_list() {
		return m_damaged_enemy_list;
	}
	player_ptr_t vc88::player_upkeep() {
		if(!m_object) {
			return nullptr;
		}
		return ptr(m_object->worn_by);
	}
	void vc88::hits_flesh_event(player_ptr_t victim) {
		if(!victim) {
			return;
		}
		/**
		 * TODO: increment shot
		 */
	}
	const room_rnum& vc88::player_room() {
		return m_player->room();
	}
	void vc88::dispatch_event(damage_event_t event,player_ptr_t victim) {
		m_player = player_upkeep();
		if(!m_player) {
			return;
		}
		switch(event) {
			case damage_event_t::YOU_HIT_FLESH:
				hits_flesh_event(victim);
				return;
			default:
				return;
		}
	}
	void vc88::reload_event() {
		std::cout << "[VC88] a legend reloads...\n";
	}
};
