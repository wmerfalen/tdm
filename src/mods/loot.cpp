#include "loot.hpp"
#include "notch.hpp"
#include "loot-container.hpp"
#include "orm/rifle-index.hpp"

#include "orm/loot-payload.hpp"
#include "orm/loot-ammo.hpp"

#define __MENTOC_MODS_LOOT_SHOW_DEBUG_OUTPUT__
#ifdef  __MENTOC_MODS_LOOT_SHOW_DEBUG_OUTPUT__
#define m_debug(a) std::cerr << "[mods::loot]: '" << a << "'\n";
#else
#define m_debug(a)
#endif

namespace mods::loot {
	static constexpr std::string_view PAYLOAD_YAML_FILE = "container/loot-payload.yml";
	static constexpr std::string_view AMMO_YAML_FILE = "container/loot-ammo.yml";
	void new_room(room_data* room) {
		for(const auto& p : mods::orm::loot_payload_list()) {
			if(p->lp_room == room->number) {
				m_debug("Loot PAYLOAD: found room to place payload in... ");
				auto obj = create_object(PAYLOAD_YAML_FILE);
				obj_to_room(obj.get(),real_room(room->number));
			}
		}
		for(const auto& p : mods::orm::loot_ammo_list()) {
			if(p->la_room == room->number) {
				m_debug("Loot AMMO: found room to place ammo in... ");
				auto obj = create_object(AMMO_YAML_FILE);
				obj_to_room(obj.get(),real_room(room->number));
			}
		}
	}
	/**
	 * Loot is also controlled by make_corpse and mods::drops.cpp
	 */
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

	obj_ptr_t reward_player(player_ptr_t& player,mob_vnum victim) {
		return std::move(mods::forge_engine::reward_player(player,victim));
	}
	obj_ptr_t reward_player_with(std::string_view type,player_ptr_t& player,mob_vnum victim) {
		return std::move(mods::forge_engine::reward_player_with(type.data(),player,victim));
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
				player->carry(reward_player(player,mob_proto[0].nr));
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
	 * manually generate a rifle of a specific type
	 */
	ADMINCMD(do_reward_with) {
		ADMIN_REJECT();
		DO_HELP_WITH_ZERO("admin:reward:with");
		static constexpr const char* usage = "usage: admin:reward:with <type> <level>...<level N>\r\n"
		                                     "SHOTGUN\r\n"
		                                     "ASSAULT_RIFLE\r\n"
		                                     "SUB_MACHINE_GUN\r\n"
		                                     "SNIPER\r\n"
		                                     "HANDGUN\r\n"
		                                     "PISTOL\r\n"
		                                     "MACHINE_PISTOL\r\n"
		                                     "LIGHT_MACHINE_GUN\r\n"
		                                     "example: admin:reward:with: SNIPER 5\r\n";

		auto vec_args = PARSE_ARGS();
		std::vector<std::string> screen;

		if(vec_args.size() < 2) {
			player->sendln(usage);
			return;
		}

		using namespace mods::forge_engine;
		auto saved_level = player->level();

		/** generate rifle */
		for(uint8_t i = 1; i < vec_args.size(); ++i) {
			int level = mods::util::stoi(vec_args[i]).value_or(-1);

			if(level <= 0) {
				player->error(CAT("Invalid numeric value encountered at string: '", vec_args[i], "'\r\n"));
				continue;
			}

			player->level() = level;
			player->carry(reward_player_with(vec_args[0],player,mob_proto[0].nr));
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
		mods::interpreter::add_command("reward", POS_RESTING, do_reward, LVL_BUILDER, 0);
		ADD_ADMIN_COMMAND("admin:reward:with", do_reward_with);
		mods::interpreter::add_command("add_rifle_index", POS_RESTING, do_add_rifle_index, LVL_BUILDER, 0);
		mods::interpreter::add_command("add_armor_index", POS_RESTING, do_add_armor_index, LVL_BUILDER, 0);
	}
};
namespace mods::loot::events {
	void award_tier_one_loot_to_player(player_ptr_t& player, player_ptr_t& npc) {
		mods::loot::store(player,std::move(mods::loot::reward_player(player,npc->cd()->nr)));

	}
	void award_tier_two_loot_to_player(player_ptr_t& player, player_ptr_t& npc) {
		mods::loot::store(player,std::move(mods::loot::reward_player(player,npc->cd()->nr)));

	}
	void award_tier_three_loot_to_player(player_ptr_t& player, player_ptr_t& npc) {
		mods::loot::store(player,std::move(mods::loot::reward_player(player,npc->cd()->nr)));

	}
	void award_by_rolling_between_tiers(player_ptr_t& player,player_ptr_t& npc,int tier_1,int tier_2) {
		/** TODO: roll and decide if user gets tier 1 or tier 2 */
		mods::loot::store(player,std::move(mods::loot::reward_player(player,npc->cd()->nr)));
	}

	/**
	 * Tiered loot
	 * ------------
	 *  Each piece of loot exists within a block of levels.
	 *  For example, when you are level 5, you fall in the block of levels between
	 *  1 and 10.
	 *
	 * Each tier is less powerful than the one before it. So, Tier 1 loot is
	 * more powerful than Tier 2 and Tier 3 loot.
	 *
	 * Tier 1 loot
	 * -----------
	 * A piece of loot in the block of levels between player levels 1 and 10
	 * would be powerful enough to not only be useful between levels 1 and 10,
	 * but might actually carry you to levels 15 and above.
	 *
	 * The characteristics
	 * -------------------
	 * - powerful enough to carry you until (block[1] + (block[1] / 2))
	 * - does (at most) 2.5 times as much damage as a Tier 2 weapon can
	 * - have atleast one elemental damage slot filled.
	 *
	 * Tier 2 loot
	 * -----------
	 *  Tier 2 loot is less powerful than tier 1 loot, and by design, does not have
	 *  very powerful elemental abilities.
	 *
	 * The characteristics
	 * -------------------
	 * - will carry you to block[1] but becomes obsolete by tier 2 loot at block[1] + 1
	 * - does not contain any elemental slots
	 * - damage output is (at best) average for the block
	 *
	 */

	void player_killed_npc(player_ptr_t& player,player_ptr_t& npc) {
		bool janitor = npc->level() * 2 <= player->level();
		bool bully = npc->level() * 3 <= player->level();

		if(!janitor && !bully && player->level() < 20) {
			award_tier_three_loot_to_player(player,npc);
			return;
		}
		if(janitor) {
			/** No loot for players who kill mobs less than half their level */
			mods::notch::easy_level_achievement(player,"Janitorial services");
			award_tier_three_loot_to_player(player,npc);
			return;
		}
		if(bully) {
			mods::notch::easy_level_achievement(player,"Bully");
			award_tier_three_loot_to_player(player,npc);
			return;
		}

		/** Lower rewards */
		bool npc_between_8_and_6 = (npc->level() < player->level() && player->level() - 8 <= npc->level() && player->level() - 6 >= npc->level());
		bool npc_between_6_and_3 = (npc->level() < player->level() && player->level() - 6 <= npc->level() && player->level() - 3 >= npc->level());
		bool npc_between_3_and_1 = (npc->level() < player->level() && player->level() - 3 <= npc->level() && player->level() - 1 >= npc->level());


		/** Decent rewards */
		bool same_level = npc->level() == player->level();


		/** higher rewards */
		bool npc_is_1_higher = (npc->level() > player->level() && player->level() + 1 == npc->level());
		bool npc_is_2_higher = (npc->level() > player->level() && player->level() + 2 == npc->level());
		bool npc_is_3_higher = (npc->level() > player->level() && player->level() + 3 == npc->level());
		bool npc_is_4_higher = (npc->level() > player->level() && player->level() + 4 == npc->level());

		/**
		 * If npc level is greater than or equal to player level
		 */
		if(npc->level() >= player->level()) {
			if(npc->level() / 2 >= player->level()) {
				/** WOW. This player just slayed an npc twice it's level?  */
				award_tier_one_loot_to_player(player,npc);
				mods::notch::elite_level_achievement(player,"I ain't afraid of no ghosts");
				return;
			} else if(npc->level() / 1.5 >= player->level()) {
				award_tier_one_loot_to_player(player,npc);
				mods::notch::advanced_level_achievement(player,"Sheesh");
				return;
			} else if(npc_is_1_higher) {
				award_tier_one_loot_to_player(player,npc);
				mods::notch::medium_level_achievement(player,"Punching up");
				return;
			} else if(npc_is_2_higher) {
				award_tier_one_loot_to_player(player,npc);
				mods::notch::medium_level_achievement(player,"Looking for a fight");
				return;
			} else if(npc_is_3_higher) {
				award_tier_one_loot_to_player(player,npc);
				mods::notch::medium_level_achievement(player,"Looking for a fight");
				return;
			} else if(npc_is_4_higher) {
				award_tier_one_loot_to_player(player,npc);
				mods::notch::advanced_level_achievement(player,"Who wants some?");
				return;
			} else if(same_level) {
				award_by_rolling_between_tiers(player,npc,2,3);
				mods::notch::medium_level_achievement(player,"Physically fit");
				return;
			} else {
				/** User falls between npc->level() / 1.5 and same level of player.
				 * So, even though we don't have a specific name/test for this range,
				 * it's still an advanced range.
				 */
				award_tier_one_loot_to_player(player,npc);
				mods::notch::advanced_level_achievement(player,"Natural born killer.");
				return;
			}
		}
		if(npc_between_8_and_6 || npc_between_6_and_3 || npc_between_3_and_1) {
			award_tier_three_loot_to_player(player,npc);
			mods::notch::easy_level_achievement(player,"Getting my feet wet.");
			return;
		}
		///**
		// * If player killed a low level NPC, only rarely reward them with a rifle
		// */
		//if(npc->level() < LOOT_REWARD_RIFLE_LEVEL()) {
		//	/**
		//	 * TODO: generate a random consumable
		//	 */
		//	return;
		//}
		mods::loot::store(player,std::move(mods::loot::reward_player(player,npc->cd()->nr)));
		player->sendln("{grn}#######################################{/grn}");
		player->sendln("{grn}# A piece of loot was awarded to you! #{/grn}");
		player->sendln("{grn}#######################################{/grn}");
	}

};


