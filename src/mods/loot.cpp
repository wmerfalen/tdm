#include "loot.hpp"
#include "loot-container.hpp"
#include "orm/rifle-index.hpp"

#define __MENTOC_MODS_LOOT_SHOW_DEBUG_OUTPUT__
#ifdef  __MENTOC_MODS_LOOT_SHOW_DEBUG_OUTPUT__
#define m_debug(a) std::cerr << "[mods::loot]: '" << a << "'\n";
#else
#define m_debug(a)
#endif
namespace mods::loot::events {

	void player_killed_npc(player_ptr_t& player,player_ptr_t& npc) {
		mods::loot_container::store(player,std::move(mods::loot::reward_player(player)));
	}
};

namespace mods::loot {
	obj_ptr_t reward_player(player_ptr_t& player) {
		return std::move(mods::forge_engine::reward_player(player));
	}

	/**
	 * manually call the loot system and get an object in your inventory
	 */
	SUPERCMD(do_reward) {
		ADMIN_REJECT();
		DO_HELP_WITH_ZERO("reward");
		static constexpr const char* usage = "usage: reward <type> <level>...<level N>\r\n"
		                                     "valid types:\r\n"
		                                     "\t- rifle\r\n"
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
				player->carry(reward_player(player));
			}
		}
		/** generate rifle */
		if(vec_args[0].compare("armor") == 0) {
			for(uint8_t i = 1; i < vec_args.size(); ++i) {
				int level = mods::util::stoi(vec_args[i]).value_or(-1);

				if(level <= 0) {
					player->error(CAT("Invalid numeric value encountered at string: '", vec_args[i], "'\r\n"));
					continue;
				}

				player->level() = level;
				generated_armor_t armor(player);
				player->carry(armor.roll());
				armor.send_stats_to_player(player);
				player->sendln("Warning: this is not being saved in armor_instance");
			}
		}
		player->level() = saved_level;
	}

	/**
	 * add a rifle filename/type to the rifle_index table
	 */
	SUPERCMD(do_add_rifle_index) {
		ADMIN_REJECT();
		DO_HELP_WITH_ZERO("add_rifle_index");
		static constexpr const char* usage = "usage: add_rifle_index <type> <yaml-file>\r\n"
		                                     "valid types:\r\n"
		                                     "\t- ar\r\n"
		                                     "\t- pistol\r\n"
		                                     "\t- mp\r\n"
		                                     "\t- shotgun\r\n"
		                                     "\t- smg\r\n"
		                                     "\t- lmg\r\n"
		                                     "\t- sniper\r\n"
		                                     "example: add_rifle_index sniper psg1.yml\r\n";
		auto vec_args = PARSE_ARGS();
		std::vector<std::string> screen;

		if(vec_args.size() < 2) {
			player->sendln(usage);
			return;
		}
		mods::orm::rifle_index r;
		std::vector<std::string> files;
		for(int i=1; i < vec_args.size(); i++) {
			files.emplace_back(vec_args[i]);
		}
		r.populate(vec_args[0], files);
		r.save();
	}

	/**
	 * add a armor filename/type to the armor_index table
	 */
	SUPERCMD(do_add_armor_index) {
		ADMIN_REJECT();
		DO_HELP_WITH_ZERO("add_armor_index");
		static constexpr const char* usage = "usage: add_armor_index <type> <yaml-file>...<yaml-file-N>\r\n"
		                                     "valid types:\r\n"
		                                     "\t- finger\r\n"
		                                     "\t- neck\r\n"
		                                     "\t- body\r\n"
		                                     "\t- head\r\n"
		                                     "\t- legs\r\n"
		                                     "\t- feet\r\n"
		                                     "\t- hands\r\n"
		                                     "\t- arms\r\n"
		                                     "\t- shield\r\n"
		                                     "\t- about\r\n"
		                                     "\t- waist\r\n"
		                                     "\t- wrist\r\n"
		                                     "\t- wield\r\n"
		                                     "\t- hold\r\n"
		                                     "\t- secondary\r\n"
		                                     "\t- shoulders\r\n"
		                                     "\t- vestpack\r\n"
		                                     "\t- elbow\r\n"
		                                     "\t- backpack\r\n"
		                                     "\t- goggles\r\n"
		                                     "example: add_armor_index shield iron-shield.yml fullbody-shield.yml\r\n";
		auto vec_args = PARSE_ARGS();
		std::vector<std::string> screen;

		if(vec_args.size() < 2) {
			player->sendln(usage);
			return;
		}
		mods::orm::armor_index r;
		std::vector<std::string> files;
		for(int i=1; i < vec_args.size(); i++) {
			files.emplace_back(vec_args[i]);
		}
		r.populate(vec_args[0], files);
		r.save();
	}


	/** game init */
	void init() {
		mods::interpreter::add_command("reward", POS_RESTING, do_reward, LVL_BUILDER, 0);
		mods::interpreter::add_command("add_rifle_index", POS_RESTING, do_add_rifle_index, LVL_BUILDER, 0);
		mods::interpreter::add_command("add_armor_index", POS_RESTING, do_add_armor_index, LVL_BUILDER, 0);
	}
};

