#include "vest-bc89.hpp"
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

#define __MENTOC_DST7A_DEBUG__ 1
#ifdef __MENTOC_DST7A_DEBUG__
#define dst_debug(A) std::cerr << "[DST7A::DEBUG]: " << A << "\n";
#else
#define dst_debug(A) ;;
#endif
extern void send_to_room(room_rnum room, const char *messg, ...);
namespace mods::armor {
	static std::map<uuid_t,bc89_ptr_t> bc89_list;
	bc89_ptr_t make_bc89(obj_ptr_t& object) {
		auto p = std::make_shared<mods::armor::bc89>(object);
		bc89_list.insert({p->uuid(),p});
		return p;
	}

	bc89_ptr_t find_bc89(const uuid_t& uuid) {
		auto it = bc89_list.find(uuid);
		if(it == bc89_list.end()) {
			return nullptr;
		}
		return it->second;
	}
	namespace bc89_commands {
		ADMINCMD(do_bc89) {
			DO_HELP("admin:bc89");
			ADMIN_REJECT();
			auto d = create_object(ITEM_ARMOR,bc89::YAML_FILE.data());
			player->equip(d,WEAR_VEST);
			ADMIN_DONE();
		}
		void init() {
			ADD_ADMIN_COMMAND("admin:bc89",do_bc89);
		}
	};
	void erase_bc89(const uuid_t& uuid) {

		auto d = find_bc89(uuid);
		if(d) {
			d.reset();
			bc89_list.erase(uuid);
		}
		dst_debug("bc89_list size: " << bc89_list.size());
	}

	bc89::~bc89() {
		m_object.reset();
		m_event_subscriptions.clear();
		m_damaged_enemy_list.clear();
		dst_debug("[bc89::~bc89] --> destructor HIT");
	}
	bc89::bc89(obj_ptr_t& object) {
		m_object = object;
		m_object->armor()->attributes->unique_armor_id = mods::armor::unique_armor_id::BC89;
	}

	player_ptr_t bc89::player_upkeep() {
		if(!m_object) {
			return nullptr;
		}
		return ptr(m_object->worn_by);
	}
	void bc89::hits_armor_event(player_ptr_t attacker) {
		if(!attacker) {
			return;
		}

	}
	void bc89::hits_flesh_event(player_ptr_t attacker) {
		//static uint8_t call_count = 0;
		if(!attacker) {
			return;
		}
		//if(++call_count > 3) {
		//	call_count = 0;
	}
	const room_rnum& bc89::player_room() {
		return m_player->room();
	}
	void bc89::dispatch_event(damage_event_t event,player_ptr_t attacker) {
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
};
