#include "../structs.h"
#include "../globals.hpp"
#include "orm/admin/banned.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "orm/admin/banned.hpp"

namespace mods::ban_system {
	std::tuple<bool,std::string> ban_player(player_ptr_t& player) {
		mods::orm::admin::banned orm;
		orm.ban_player(player);
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
		return {1,"saved"};
	}
	std::tuple<bool,std::string> ban_hostname(std::string_view hostname) {
		mods::orm::admin::banned orm;
		orm.ban_hostname(hostname);
		if(orm.saved_success == false) {
			return {0,"Failed to save"};
		}
		return {1,"saved"};
	}
	std::tuple<bool,std::string> ban_username(std::string_view username) {
		mods::orm::admin::banned orm;
		orm.ban_username(username);
		if(orm.saved_success == false) {
			return {0,"Failed to save"};
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
};
