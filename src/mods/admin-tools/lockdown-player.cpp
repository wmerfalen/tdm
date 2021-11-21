#include "lockdown-player.hpp"
#include "../classes/includes.hpp"
#include "../replenish.hpp"
#include "../orm/inventory.hpp"
#include "../db.hpp"
#include "../orm/player-base-ability.hpp"
#include "../pq.hpp"
#include "../sql.hpp"
#include "../levels.hpp"
#include "../orm/player-skill-points.hpp"
#include "../orm/player-skill-usage.hpp"
#include "../orm/skill-trees.hpp"
#include "../orm/rifle-attachment.hpp"
#include "../movement.hpp"
#include "../players/banish.hpp"
#include "../pfind.hpp"
#include "../world-configuration.hpp"
#include "../interpreter.hpp"
#include "../ban-system.hpp"
#include "../players/db-load.hpp"

#ifdef __MENTOC_SHOW_MODS_PLAYERS_DB_LOAD_DEBUG_OUTPUT__
#define m_debug(a) std::cerr << "[mods::admin_tools::lockdown][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#define m_crit(a) std::cerr << red_str("[CRITICAL]") << "mods::admin_tools::lockdown][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#else
#define m_debug(a)
#define m_crit(a)
#endif

extern void write_aliases(char_data *ch);
extern void read_aliases(char_data *ch);
extern int destroy_player(player_ptr_t&& player);

namespace mods::admin_tools::lockdown {
	static reporter_t report_function;
	static bool reporter_function_set = false;

	void set_reporter_lambda(reporter_t f) {
		report_function = f;
		reporter_function_set = true;
	}
	void report(int64_t code, std::string_view msg) {
		if(!reporter_function_set) {
			m_debug("[fallback reporter]: " << red_str(CAT("code:",(code))) <<
			        " message: '" << red_str(msg.data()) << "'");
			return;
		}
		report_function(code,msg);
	}
	enum ban_type_t : uint8_t {
		BAN_BY_IP = (1 << 0),
		BAN_BY_HOST = (1 << 1),
		BAN_BY_USERNAME = (1 << 2),
		BAN_BY_ALL = (BAN_BY_IP | BAN_BY_HOST | BAN_BY_USERNAME),
	};
	void ban_player_via(player_ptr_t& admin,const auto& vec_args,ban_type_t type) {
		for(auto name : vec_args) {
			auto p = mods::pfind::optby_name(name.c_str());
			if(!p.has_value()) {
				if(type & BAN_BY_USERNAME || type & BAN_BY_ALL) {
					admin->send(CAT("[+] Banning player '",name,"' by user name...").c_str());
					mods::ban_system::ban_username(name.c_str());
					mods::players::db_load::delete_char_by_name(name);
					admin->sendln("{grn}[DONE]{/grn}");
				} else {
					if(type & BAN_BY_IP) {
						admin->sendln(
						    CAT("{red}[-] Cannot ban player '",name,"' by IP since they are not logged in...")
						);
						admin->sendln("Please consider using admin:ban:user to ban the user by user name");
					}
					if(type & BAN_BY_HOST) {
						admin->sendln(
						    CAT("{red}[-] Cannot ban player '",name,"' by HOST since they are not logged in...")
						);
						admin->sendln("Please consider using admin:ban:user to ban the user by user name");
					}
				}
			} else {
				auto douche = p.value();
				admin->send(CAT("[+] Placing player '",name,"' on lockdown...").c_str());
				douche->lockdown(true);
				admin->sendln("{grn}[DONE]{/grn}");

				admin->send(CAT("[+] Pulling player '",name,"'...").c_str());
				char_from_room(douche->cd());
				char_to_room(douche->cd(),mods::world_conf::real_frozen());
				admin->send(CAT("[+] Beginning ban process for '",name,"'...").c_str());
				if(type & BAN_BY_ALL) {
					admin->send("  - [+] Banning by IP, Hostname, and username...");
					mods::ban_system::ban_player(douche);
				} else if(type & BAN_BY_IP) {
					admin->send("  - [+] Banning player by IP...");
					mods::ban_system::ban_ip(douche->ip());
					admin->sendln("{grn}[DONE]{/grn}");
				} else if(type & BAN_BY_HOST) {
					admin->send(CAT("  - [+] Banning player by hostname '",douche->host(),"'...").c_str());
					mods::ban_system::ban_hostname(douche->host());
					admin->sendln("{grn}[DONE]{/grn}");
				} else if(type & BAN_BY_USERNAME) {
					admin->send(CAT("  - [+] Banning player by username '",douche->name(),"'...").c_str());
					mods::ban_system::ban_username(douche->name());
					admin->sendln("{grn}[DONE]{/grn}");
				}
				admin->send("  - [+] Deleting player from database...");
				mods::players::db_load::delete_char(douche);
				admin->sendln("{grn}[DONE]{/grn}");
				admin->send("  - [+] Destroying player instance in game world...");
				destroy_player(std::move(douche));
				admin->sendln(CAT("{grn}[Completed ban process for player: '",name,"'{/grn}"));
			}
		}
	}

	ADMINCMD(do_mute) {
		DO_HELP("admin:mute");
		ADMIN_REJECT();
		auto vec_args = PARSE_ARGS();
		if(vec_args.size() > 0) {
			for(auto name : vec_args) {
				auto p = mods::pfind::optby_name(name.c_str());
				if(!p.has_value()) {
					player->sendln(CAT("[ERROR] couldn't find player by name '",name,"'"));
				} else {
					auto douche = p.value();
					player->send("[+] Placing player on lockdown...");
					/**
					 * TODO: implement mods::player::can_talk(bool );
					 */
					douche->lockdown(true);
					player->sendln("{grn}[DONE]{/grn}");
				}
			}
			ADMIN_DONE();
			return;
		}
		ADMIN_FAIL();
	}

	ADMINCMD(do_unmute) {
		DO_HELP("admin:unmute");
		ADMIN_REJECT();
		auto vec_args = PARSE_ARGS();
		if(vec_args.size() > 0) {
			for(auto name : vec_args) {
				auto p = mods::pfind::optby_name(name.c_str());
				if(!p.has_value()) {
					player->sendln(CAT("[ERROR] couldn't find player by name '",name,"'"));
				} else {
					auto douche = p.value();
					player->send("[+] Placing player on lockdown...");
					douche->lockdown(true);
					player->sendln("{grn}[DONE]{/grn}");

					player->send(CAT("[+] Pulling player '",name,"'...").c_str());
					char_from_room(douche->cd());
					char_to_room(douche->cd(),mods::world_conf::real_frozen());
					player->sendln("{grn}[DONE]{/grn}");
				}
			}
			ADMIN_DONE();
			return;
		}
		ADMIN_FAIL();
	}

	ADMINCMD(do_banish) {
		if(argshave()->size_gt(1)->first_is("except")->passed()) {
			auto keep = args()->gather_uuids_starting_at(1);
			keep.emplace_back(player->uuid());
			auto cnt = mods::movement::banish_except(player->room(),mods::players::banish::room(),keep);
			player->sendln(CAT("Forced ",cnt," people out"));
			return;
		}
		if(argshave()->first_is("all")->passed()) {
			std::vector<uuid_t> keep = {player->uuid()};
			auto cnt = mods::movement::banish_except(player->room(),mods::players::banish::room(), keep);
			player->sendln(CAT("Forced ",cnt," people out"));
			return;
		}
		player->sendln("usage: banish <except> <uuid>...[uuid-N]");
		player->sendln("usage: banish <all> # will banish everyone besides you");
	}

	ADMINCMD(do_unban) {
		DO_HELP("admin:unban");
		ADMIN_REJECT();
		auto vec_args = PARSE_ARGS();
		if(vec_args.size() > 0) {
			for(auto name : vec_args) {
				auto p = mods::pfind::optby_name(name.c_str());
				if(!p.has_value()) {
					player->sendln(CAT("[ERROR] couldn't find player by name '",name,"'"));
				} else {
					auto douche = p.value();
					player->send("[+] Removing player's lockdown status...");
					douche->lockdown(false);
					player->sendln("{grn}[DONE]{/grn}");

					player->send(CAT("[+] Pulling player '",name,"'...").c_str());
					char_from_room(douche->cd());
					char_to_room(douche->cd(),mods::world_conf::real_mortal_start());
					player->sendln("{grn}[DONE]{/grn}");
				}
			}
			ADMIN_DONE();
			return;
		}
		ADMIN_FAIL();
	}
	ADMINCMD(do_ban) {
		DO_HELP("admin:ban:*");
		ADMIN_REJECT();
		auto vec_args = PARSE_ARGS();
		if(vec_args.size() > 0) {
			ban_player_via(player,vec_args,BAN_BY_ALL);
			return;
		}
		ADMIN_FAIL();
	}
	ADMINCMD(do_ban_user) {
		DO_HELP("admin:ban:user");
		ADMIN_REJECT();
		auto vec_args = PARSE_ARGS();
		if(vec_args.size() > 0) {
			ban_player_via(player,vec_args,BAN_BY_USERNAME);
			return;
		}
		ADMIN_FAIL();
	}
	ADMINCMD(do_ban_ip) {
		DO_HELP("admin:ban:ip");
		ADMIN_REJECT();
		auto vec_args = PARSE_ARGS();
		if(vec_args.size() > 0) {
			ban_player_via(player,vec_args,BAN_BY_IP);
			return;
		}
		ADMIN_FAIL();
	}
	ADMINCMD(do_ban_host) {
		DO_HELP("admin:ban:host");
		ADMIN_REJECT();
		auto vec_args = PARSE_ARGS();
		if(vec_args.size() > 0) {
			ban_player_via(player,vec_args,BAN_BY_HOST);
			return;
		}
		ADMIN_FAIL();
	}
	ADMINCMD(do_pull) {
		DO_HELP("admin:pull");
		ADMIN_REJECT();
		auto vec_args = PARSE_ARGS();
		if(vec_args.size() > 0) {
			for(auto name : vec_args) {
				auto p = mods::pfind::optby_name(name.c_str());
				if(!p.has_value()) {
					player->sendln(CAT("[ERROR] couldn't find player by name '",name,"'"));
				} else {
					auto douche = p.value();
					player->send(CAT("[+] Pulling player '",name,"'...").c_str());
					char_from_room(douche->cd());
					char_to_room(douche->cd(),player->room());
					player->sendln("{grn}[DONE]{/grn}");
				}
			}
			ADMIN_DONE();
			return;
		}
		ADMIN_FAIL();
	}

	namespace commands {
		void init() {
			ADD_ADMIN_COMMAND("admin:mute",do_mute);
			ADD_ADMIN_COMMAND("admin:unmute",do_unmute);
			ADD_ADMIN_COMMAND("admin:ban:ip",do_ban_ip);
			ADD_ADMIN_COMMAND("admin:ban:host",do_ban_host);
			ADD_ADMIN_COMMAND("admin:ban:user",do_ban_user);
			ADD_ADMIN_COMMAND("admin:ban:*",do_ban);
			ADD_ADMIN_COMMAND("admin:unban",do_unban);
			ADD_ADMIN_COMMAND("admin:pull",  do_pull);
		}
	};
};
