#include "../structs.h"
#include "../globals.hpp"
#include "orm/admin/banned.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "orm/admin/banned.hpp"

#include "db.hpp"
#include "pq.hpp"
#include "sql.hpp"
#include "levels.hpp"
#include "movement.hpp"
#include "players/banish.hpp"
#include "pfind.hpp"
#include "world-configuration.hpp"
#include "interpreter.hpp"
#include "players/db-load.hpp"
#include "message-server.hpp"

#include "player.hpp"

extern int destroy_player(player_ptr_t&& player);
//static reporter_t report_function;

namespace mods::ban_system {
	std::tuple<bool,std::string> ban_player(player_ptr_t& player) {
		mods::orm::admin::banned orm;
		orm.ban_player(player);
		if(orm.saved_success == false) {
			return {0,"Failed to save"};
		}
		//TODO: mods::message_server::ban_player(player);
		return {1,"saved"};
	}
	std::tuple<bool,std::string> unban_ip(std::string_view ip) {
		mods::orm::admin::banned orm;
		orm.unban_ip(ip);
		if(orm.saved_success == false) {
			return {0,"Failed to save"};
		}
		//TODO: mods::message_server::unban_ip(ip);
		return {1,"saved"};
	}
	std::tuple<bool,std::string> unban_hostname(std::string_view hostname) {
		mods::orm::admin::banned orm;
		orm.unban_hostname(hostname);
		if(orm.saved_success == false) {
			return {0,"Failed to save"};
		}
		//TODO: mods::message_server::unban_hostname(ip);
		return {1,"saved"};
	}
	std::tuple<bool,std::string> unban_username(std::string_view username) {
		mods::orm::admin::banned orm;
		orm.unban_username(username);
		if(orm.saved_success == false) {
			return {0,"Failed to save"};
		}
		return {1,"saved"};
	}

	std::tuple<bool,std::string> ban_ip(std::string_view ip) {
		mods::orm::admin::banned orm;
		orm.ban_ip(ip);
		if(orm.saved_success == false) {
			return {0,"Failed to save"};
		}
		{
			auto str = ip.data();
			mods::message_server::ban_ip(str);
		}
		return {1,"saved"};
	}
	std::tuple<bool,std::string> ban_hostname(std::string_view hostname) {
		mods::orm::admin::banned orm;
		orm.ban_hostname(hostname);
		if(orm.saved_success == false) {
			return {0,"Failed to save"};
		}
		{
			auto str = hostname.data();
			mods::message_server::ban_hostname(str);
		}
		return {1,"saved"};
	}
	std::tuple<bool,std::string> ban_username(std::string_view username) {
		mods::orm::admin::banned orm;
		orm.ban_username(username);
		if(orm.saved_success == false) {
			return {0,"Failed to save"};
		}
		{
			auto str = username.data();
			mods::message_server::ban_username(str);
		}
		return {1,"saved"};
	}
	namespace hostname {
		bool is_banned(std::string hostname) {
			std::cerr << "is_banned(hostname): " << hostname << "\n";
			mods::orm::admin::banned orm;
			auto banned = orm.is_hostname_banned(hostname);
			std::cerr << "is_banned(hostname) check from sql got: '" << banned << "'\n";
			return banned;
		}
		bool is_banned(const sockaddr_in& peer) {
			struct hostent *from = gethostbyaddr((char *) &peer.sin_addr, sizeof(peer.sin_addr), AF_INET);
			if(from && mods::ban_system::hostname::is_banned(from->h_name)) {
				return true;
			}
			return false;
		}
	};
	namespace ip {
		bool is_banned(std::string ip_address) {
			std::cerr << "is_banned(ip): " << ip_address << "\n";
			mods::orm::admin::banned orm;
			auto banned = orm.is_ip_banned(ip_address);
			std::cerr << "is_banned(ip) check from sql got: '" << banned << "'\n";
			return banned;
		}
		bool is_banned(const sockaddr_in& peer) {
			if(mods::ban_system::ip::is_banned(std::string(inet_ntoa(peer.sin_addr)))) {
				return true;
			}
			return false;
		}
	};
	namespace username {
		bool is_banned(std::string user_name) {
			mods::orm::admin::banned orm;
			auto banned = orm.is_username_banned(user_name);
			std::cerr << "is_banned(user_name) check from sql got: '" << banned << "'\n";
			return banned;
		}
	};
	//static bool reporter_function_set = false;

	//void set_reporter_lambda(reporter_t f) {
	//	report_function = f;
	//	reporter_function_set = true;
	//}
	//void report(int64_t code, std::string_view msg) {
	//	if(!reporter_function_set) {
	//		m_debug("[fallback reporter]: " << red_str(CAT("code:",(code))) <<
	//		        " message: '" << red_str(msg.data()) << "'");
	//		return;
	//	}
	//	report_function(code,msg);
	//}
	enum ban_type_t : uint8_t {
		BAN_BY_IP = (1 << 0),
		BAN_BY_HOST = (1 << 1),
		BAN_BY_USERNAME = (1 << 2),
		BAN_BY_ALL = (BAN_BY_IP | BAN_BY_HOST | BAN_BY_USERNAME),
	};
	void unban_ip(player_ptr_t& admin,std::string_view ip) {
		admin->sendln(CAT("[+] Beginning un-ban process for ip '",ip.data(),"'...").c_str());
		mods::ban_system::unban_ip(ip);
		admin->sendln("{grn}[DONE]{/grn}");
	}
	void unban_hostname(player_ptr_t& admin,std::string_view hostname) {
		admin->sendln(CAT("[+] Beginning un-ban process for host '",hostname.data(),"'...").c_str());
		mods::ban_system::unban_hostname(hostname);
		admin->sendln("{grn}[DONE]{/grn}");
	}
	void unban_username(player_ptr_t& admin,std::string_view username) {
		admin->sendln(CAT("[+] Beginning un-ban process for user '",username.data(),"'...").c_str());
		mods::ban_system::unban_username(username);
		admin->sendln("{grn}[DONE]{/grn}");
	}
	void ban_ip(player_ptr_t& admin,std::string_view ip) {
		admin->sendln(CAT("[+] Beginning ban process for ip '",ip.data(),"'...").c_str());
		mods::ban_system::ban_ip(ip);
		admin->sendln("{grn}[DONE]{/grn}");
	}
	void ban_hostname(player_ptr_t& admin,std::string_view hostname) {
		admin->sendln(CAT("[+] Beginning ban process for host '",hostname.data(),"'...").c_str());
		mods::ban_system::ban_hostname(hostname);
		admin->sendln("{grn}[DONE]{/grn}");
	}
	void ban_username(player_ptr_t& admin,std::string_view username) {
		admin->sendln(CAT("[+] Beginning ban process for user '",username.data(),"'...").c_str());
		mods::ban_system::ban_username(username);
		admin->sendln("{grn}[DONE]{/grn}");
	}
	void ban_player(player_ptr_t& admin,std::string player_name) {
		auto p = mods::pfind::optby_name(player_name.c_str());
		if(!p.has_value()) {
			admin->sendln("{red} [-] Can't band player's IP or Hostname since they aren't connected..{/red}");
			admin->sendln(CAT("[+] Banning player '",player_name,"' by user name...").c_str());
			mods::ban_system::ban_username(player_name);
			admin->sendln("{grn}[DONE]{/grn}");
			admin->sendln("  - [+] Destroying player instance in game world...");
			mods::players::db_load::delete_char_by_name(player_name);
			admin->sendln("{grn}[DONE]{/grn}");
			admin->sendln(CAT("{grn}[Completed PARTIAL ban process for player: '",player_name,"'{/grn}"));
			return;
		} else {
			auto douche = p.value();
			admin->sendln(CAT("[+] Placing player '",player_name,"' on lockdown...").c_str());
			douche->lockdown(true);
			admin->sendln("{grn}[DONE]{/grn}");

			admin->sendln(CAT("[+] Pulling player '",player_name,"'...").c_str());
			char_from_room(douche->cd());
			char_to_room(douche->cd(),mods::world_conf::real_frozen());
			admin->sendln(CAT("[+] Beginning ban process for '",player_name,"'...").c_str());
			admin->sendln("  - [+] Banning by IP, Hostname, and username...");
			mods::ban_system::ban_player(douche);
			admin->sendln("  - [+] Deleting player from database...");
			mods::players::db_load::delete_char(douche);
			admin->sendln("{grn}[DONE]{/grn}");
			admin->sendln("  - [+] Destroying player instance in game world...");
			destroy_player(std::move(douche));
			admin->sendln(CAT("{grn}[Completed ban process for player: '",player_name,"'{/grn}"));
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
					player->sendln("[+] Placing player on lockdown...");
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
					player->sendln("[+] Placing player on lockdown...");
					douche->lockdown(true);
					player->sendln("{grn}[DONE]{/grn}");

					player->sendln(CAT("[+] Pulling player '",name,"'...").c_str());
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

	ADMINCMD(do_unban_host) {
		DO_HELP("admin:unban:host");
		ADMIN_REJECT();
		auto vec_args = PARSE_ARGS();
		if(vec_args.size() > 0) {
			for(auto host : vec_args) {
				unban_hostname(player,host);
			}
			ADMIN_DONE();
			return;
		}
		ADMIN_FAIL();
	}
	ADMINCMD(do_unban_ip) {
		DO_HELP("admin:unban:ip");
		ADMIN_REJECT();
		auto vec_args = PARSE_ARGS();
		if(vec_args.size() > 0) {
			for(auto ip : vec_args) {
				unban_ip(player,ip);
			}
			ADMIN_DONE();
			return;
		}
		ADMIN_FAIL();
	}
	ADMINCMD(do_unban_user) {
		DO_HELP("admin:unban:user");
		ADMIN_REJECT();
		auto vec_args = PARSE_ARGS();
		if(vec_args.size() > 0) {
			for(auto username : vec_args) {
				unban_username(player,username);
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
		player->sendln("WE ARE IN DO_BAN!!!!!");
		player->sendln("WE ARE IN DO_BAN!!!!!");
		player->sendln("WE ARE IN DO_BAN!!!!!");
		player->sendln("WE ARE IN DO_BAN!!!!!");
		player->sendln("WE ARE IN DO_BAN!!!!!");
		player->sendln("WE ARE IN DO_BAN!!!!!");
		for(const auto& name : vec_args) {
			ban_player(player,name);
		}
	}
	ADMINCMD(do_ban_user) {
		DO_HELP("admin:ban:user");
		ADMIN_REJECT();
		auto vec_args = PARSE_ARGS();
		for(const auto& username : vec_args) {
			ban_username(player,username);
		}
	}
	ADMINCMD(do_ban_ip) {
		DO_HELP("admin:ban:ip");
		ADMIN_REJECT();
		auto vec_args = PARSE_ARGS();
		for(const auto& ip : vec_args) {
			ban_ip(player,ip);
		}
	}
	ADMINCMD(do_ban_host) {
		DO_HELP("admin:ban:host");
		ADMIN_REJECT();
		auto vec_args = PARSE_ARGS();
		for(const auto& host : vec_args) {
			ban_hostname(player,host);
		}
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
					player->sendln(CAT("[+] Pulling player '",name,"'...").c_str());
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
			ADD_ADMIN_COMMAND("admin:unban:ip",do_unban_ip);
			ADD_ADMIN_COMMAND("admin:unban:host",do_unban_host);
			ADD_ADMIN_COMMAND("admin:unban:user",do_unban_user);
			ADD_ADMIN_COMMAND("admin:pull",  do_pull);
		}
	};
};//end namespace mods::ban_system
