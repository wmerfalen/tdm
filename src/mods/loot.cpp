#include "loot.hpp"
#include "lmdb.hpp"
#include "super-users.hpp"
#include "interpreter.hpp"
#include "screen-searcher.hpp"
#include "forge-engine/generator.hpp"

#define __MENTOC_MODS_LOOT_SHOW_DEBUG_OUTPUT__
#ifdef  __MENTOC_MODS_LOOT_SHOW_DEBUG_OUTPUT__
#define m_debug(a) std::cerr << "[mods::loot]: '" << a << "'\n";
#else
#define m_debug(a)
#endif

namespace mods::loot {
	obj_ptr_t reward_player(player_ptr_t& player) {
		return std::move(mods::forge_engine::reward_player(player));
	}
	ACMD(do_reward) {
		ADMIN_REJECT();
		DO_HELP_WITH_ZERO("reward");
		static constexpr const char* usage = "usage: reward <type> <level>...<level N>\r\n"
		                                     "valid types:\r\n"
		                                     "\t- rifle\r\n"
		                                     "\t- explosive\r\n"
		                                     "\t- drone\r\n"
		                                     "\t- armor\r\n"
		                                     "example: reward rifle 1 10 30\r\n"
		                                     "\t this example will spin the forge engine for 3 rifles at levels 1, 10, and 30.\r\n";
		auto vec_args = PARSE_ARGS();
		std::vector<std::string> screen;

		if(vec_args.size() < 2) {
			player->sendln(usage);
			return;
		}

		using namespace mods::forge_engine;
		auto saved_level = player->level();

		/** generate rifle */
		if(vec_args[0].compare("rifle") == 0) {
			for(uint8_t i = 1; i < vec_args.size(); ++i) {
				int level = mods::util::stoi(vec_args[i]).value_or(-1);

				if(level <= 0) {
					player->error(CAT("Invalid numeric value encountered at string: '", vec_args[i], "'\r\n"));
					continue;
				}

				player->level() = level;
				generated_rifle_t rifle(player);
				player->carry(rifle.roll());
				rifle.send_stats_to_player(player);
			}
		}
		player->level() = saved_level;
	}
	/** game init */
	void init() {
		mods::interpreter::add_command("reward", POS_RESTING, do_reward, LVL_BUILDER, 0);
	}
};

