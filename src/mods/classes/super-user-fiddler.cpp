#include "super-user-fiddler.hpp"
#include "pyrexia.hpp"
#include "../interpreter.hpp"

namespace mods::classes {
	void super_user_fiddler::set_fire_nades(player_ptr_t& player,int charges) {
		player->pyrexia()->m_fire_nade_charges = charges;
	}
	void super_user_fiddler::set_fire_nade_level(player_ptr_t& player,int level) {
		player->pyrexia()->m_fire_nade_level = (skill_familiarity_t)level;
	}

	namespace mocks {
		static std::map<uuid_t,uint8_t> player_level;
	};
	void set_player_level_mock(uuid_t& player,uint8_t level) {
		mocks::player_level[player] = level;
	}
	uint8_t mock_player_level(uuid_t& player,uint8_t or_else) {
		if(mocks::player_level[player]) {
			return mocks::player_level[player];
		}
		return or_else;
	}
	SUPERCMD(do_mock_level) {
		if(!ALLOW_MOCK_LEVELS()) {
			player->errorln("Disabled");
			return;
		}
		auto vec_args = PARSE_ARGS();
		if(vec_args.size() == 0) {
			player->errorln("Usage: mock_level <level>");
			return;
		}
		int level = mods::util::stoi(vec_args[0]).value_or(-1);
		if(level <= 0) {
			player->errorln("Invalid integer given.");
			return;
		}
		set_player_level_mock(player->uuid(),level);
		player->send("Set your level to %d\r\n",player->level());
	}

	void init() {
		mods::interpreter::add_command("mock_level", POS_RESTING, do_mock_level, 0,0);
	}
};
