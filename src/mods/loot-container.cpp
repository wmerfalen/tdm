#include "loot-container.hpp"

#define __MENTOC_MODS_LOOT_CONTAINER_SHOW_DEBUG_OUTPUT__
#ifdef  __MENTOC_MODS_LOOT_CONTAINER_SHOW_DEBUG_OUTPUT__
#define m_debug(a) std::cerr << "[mods::loot_container]: '" << a << "'\n";
#else
#define m_debug(a)
#endif

namespace mods::loot_container {
	struct loot_t {
		uint32_t id;
		std::string player;
		obj_ptr_t loot;
	};

	static std::vector<loot_t> loot_list;
	void store(player_ptr_t& player, obj_ptr_t&& obj) {
		static uint32_t id = 0;
		loot_t l;
		l.id = ++id;
		l.player = player->name();
		l.loot = std::move(obj);
		loot_list.emplace_back(std::move(l));
	}
	void delete_loot(loot_t loot) {
		auto id = loot.id;
		loot_list.erase(
		    std::remove_if(loot_list.begin(), loot_list.end(),
		[&](const auto & o) {
			return o.id == id;
		}),
		loot_list.end());
	}

	std::vector<loot_t> get_loot(player_ptr_t& player) {
		std::vector<loot_t> list;
		for(const auto& row : loot_list) {
			if(row.player.compare(player->name().c_str()) == 0) {
				list.emplace_back(row);
			}
		}
		return list;
	}

	ACMD(do_show_loot) {
		DO_HELP_WITH_ZERO("show_loot");
		static const char* usage = "usage: show_loot <index>\r\n";
		if(argshave()->int_at(0)->passed() == false) {
			player->sendln(usage);
			return;
		}
		auto index = intat(0);

		int ctr = 1;
		for(const auto& loot : get_loot(player)) {
			if(ctr == index) {
				player->sendln("[+] Listing...");
				player->sendln(loot.loot->generate_stat_page());
				player->sendln("[+] End of list.");
				return;
			}
			++ctr;
		}
	}
	ACMD(do_take_loot) {
		DO_HELP_WITH_ZERO("take_loot");
		static const char* usage = "usage: take_loot <index>\r\n";
		if(argshave()->int_at(0)->passed() == false) {
			player->sendln(usage);
			return;
		}
		auto index = intat(0);

		int ctr = 1;
		for(const auto& loot : get_loot(player)) {
			if(ctr == index) {
				player->sendln(CAT("You carry a ",loot.loot->name.c_str()));
				player->carry(std::move(loot.loot));
				delete_loot(loot);
				return;
			}
			++ctr;
		}
	}
	ACMD(do_list_loot) {
		DO_HELP_WITH_ZERO("list_loot");
		/*
		static constexpr const char* usage = "usage: reward <type> <level>...<level N>\r\n"
		                                     "valid types:\r\n"
		                                     "\t- rifle\r\n"
		                                     "\t- armor\r\n"
		                                     "example: reward rifle 1 10 30\r\n"
		                                     "\t this example will spin the forge engine for 3 rifles at levels 1, 10, and 30.\r\n";
																				 */
		auto vec_args = PARSE_ARGS();
		std::vector<std::string> screen;

		player->sendln("[+] Listing...");
		int ctr = 1;
		for(const auto& loot : get_loot(player)) {
			player->sendln(CAT("[",ctr,"]-> ",loot.loot->name.c_str()));
			++ctr;
		}
		player->sendln("[+] End of list.");

#if 0
		if(vec_args.size() < 2) {
			player->sendln(usage);
			return;
		}

#endif
	}

	/** game init */
	void init() {
		mods::interpreter::add_command("list_loot", POS_RESTING, do_list_loot, 0, 0);
		mods::interpreter::add_command("show_loot", POS_RESTING, do_show_loot, 0, 0);
		mods::interpreter::add_command("take_loot", POS_RESTING, do_take_loot, 0, 0);
	}
};

