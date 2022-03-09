#include "breacher.hpp"
#include "../orm/inventory.hpp"
#include "../affects.hpp"
#include "../rooms.hpp"

extern void stop_fighting(char_data *ch);
namespace mods::orm::inventory {
	extern int16_t flush_player(player_ptr_t& player);
};
namespace mods::classes {
	breacher::breacher(player_ptr_t p) {
		m_explosive_shot_charges = 0;
		load_by_player(p);
	}
	void breacher::replenish() {
		static uint32_t call_count = 0;
		uint8_t tier = tier(m_player);
		if(++call_count >= BREACHER_REPLENISH_CALL_COUNT()) {
			call_count = 0;
			if(m_explosive_shot_charges + 1 < BREACHER_EXPLOSIVE_SHOT_MAX_COUNT() * tier) {
				++m_explosive_shot_charges;
			}
		}
	}
	player_ptr_t 	breacher::player() {
		return m_player;
	}
	void breacher::set_player(player_ptr_t p) {
		m_player = p;
	}
	int16_t breacher::new_player(player_ptr_t& player) {
		auto db_id = m_orm.initialize_row(player);
		if(db_id == 0) {
			return -2;
		}
		load_by_player(player);
		return 0;
	}
	int16_t breacher::load_by_player(player_ptr_t& player) {
		m_explosive_shot_charges = 0;
		auto result = m_orm.load_by_player(player->db_id());
		if(result < 0) {
			report(CAT("breacher::load_by_player unable to load breacher class by player id: ",(player->db_id()),".. return status: ",(result)));
			return result;
		}
		set_player(player);
		return result;
	}
	/* constructors and destructors */
	breacher::breacher() {
		m_explosive_shot_charges = 0;
		m_player = nullptr;
	}
	std::tuple<bool,std::string> breacher::explosive_shot(direction_t direction) {
		if(0 == m_explosive_shot_charges) {
			return {false,"You must wait until you can use your {grn}Explosive shot{/grn} again."};
		}
		auto& room = world[m_player->room()];
		if(!room.dir_option[direction]) {
			mods::rooms::pave_once(m_player->room(),direction);
			--m_explosive_shot_charges;
			return {true,"{grn}Your explosive shot makes an entrance!{/grn}"};
		}
		return {true,"You enter the pre-existing entrance."};
	}
	void breacher::attempt_direction(direction_t dir) {
		auto p = std::make_pair<>(m_player->room(),dir);
		m_push_count[p]++;
		if(m_push_count[p] >= 2) {
			if(m_explosive_shot_charges == 0) {
				m_player->sendln("{red}You attempt to charge an explosive shot, but you are all out!{/red}");
				return;
			}
			auto status = explosive_shot(dir);
			m_player->sendln(std::get<1>(status));
			if(std::get<0>(status)) {
				m_push_count.erase(p);
			}
			return;
		}
		if(m_push_count[p] < 2 && m_explosive_shot_charges) {
			m_player->sendln(
			    CAT(
			        "{grn}You ready an explosive charge toward the ",dirstr(dir).c_str(),
			        " door...{/grn}"
			    )
			);
		}
	}
	std::shared_ptr<breacher> create_breacher(player_ptr_t& in_player) {
		return std::move(std::make_shared<breacher>(in_player));
	}
};
ACMD(do_teep) {
	player->sendln("Command not implemented.");
	return;
#if 0
	PLAYER_CAN("breacher.teep");
	auto vec_args = PARSE_ARGS();
	if(vec_args.size() < 1) {
		player->sendln("Which direction?!");
		return;
	}
	auto s = to_direction(vec_args[0]);
	if(!s.first) {
		player->sendln("Which direction?!");
		return;
	}
	auto msg = std::get<1>(player->breacher()->teep(s.second));
	player->sendln(msg);
#endif
}
namespace mods::class_abilities::breacher {
	void init() {

	}
};
