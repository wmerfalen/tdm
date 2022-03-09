#ifndef __MENTOC_MODS_MESSAGE_SERVER_HEADER__
#define __MENTOC_MODS_MESSAGE_SERVER_HEADER__

#include "../cppzmq/zmq.hpp"
#include <thread>
#include <chrono>
#include <forward_list>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "player.hpp"

#include <zmq.hpp>
#include <zmq_addon.hpp>

namespace mods::message_server {
	enum channel_t : uint16_t {
		CHAN_ADMIN,
		CHAN_EVENT,
		CHAN_USER_LOGGED_IN,
		CHAN_USER_LOGGED_OUT,
		CHAN_SHUTDOWN,
		CHAN_BROADCAST,
		CHAN_NEW_CONNECTION,
		CHAN_BAN_IP,
		CHAN_BAN_HOSTNAME,
		CHAN_BAN_USERNAME,
		CHAN_USER_CONNECTION_REJECTED,
		CHAN_USER_DESTROYED,
		CHAN_UNBAN,
	};
	static inline std::string to_string(const channel_t& c) {
		switch(c) {
			case CHAN_UNBAN:
				return "UNBAN";
			case CHAN_NEW_CONNECTION:
				return "NEW_CONNECTION";
			case CHAN_BAN_IP:
				return "BAN_IP";
			case CHAN_BAN_HOSTNAME:
				return "BAN_HOSTNAME";
			case CHAN_BAN_USERNAME:
				return "BAN_USERNAME";
			case CHAN_USER_CONNECTION_REJECTED:
				return "USER_CONNECTION_REJECTED";
			case CHAN_USER_LOGGED_OUT:
				return "USER_LOGGED_OUT";
			case CHAN_USER_LOGGED_IN:
				return "USER_LOGGED_IN";
			case CHAN_ADMIN:
				return "ADMIN";
			case CHAN_EVENT:
				return "EVENT";
			case CHAN_SHUTDOWN:
				return "SHUTDOWN";
			case CHAN_BROADCAST:
				return "*";
			default:
				return std::to_string((uint16_t)c);
		}
	}

	static constexpr std::string_view queue = "inproc://#1";

	static inline zmq::context_t& fetch_context() {
		static zmq::context_t context;
		return context;
	}
	static inline std::forward_list<std::thread>&  thread_list() {
		static std::forward_list<std::thread> list;
		return list;
	}
	static inline zmq::socket_t& pub_socket() {
		static zmq::socket_t publisher(fetch_context(), zmq::socket_type::pub);
		static bool publisher_bound = false;
		if(!publisher_bound) {
			publisher.bind(queue.data());
			publisher_bound = true;
		}
		return publisher;
	}
	void publish(const channel_t& channel, const std::string& msg);
	void publish(const channel_t& channel, const std::vector<std::string>& msg);

	/**
	 * Main entry point.
	 * Call this at or around the beginning of the main() function
	 */
	void spawn();


	/**
	 * Various events that we'd like to publish
	 */
	void trigger_mud_shutdown();
	void new_connection(const sockaddr_in& peer,hostent* from);
	void user_logged_in(player_ptr_t& player);
	void user_logged_out(player_ptr_t& player);
	void user_connection_rejected(const sockaddr_in& peer,hostent* from);
	void ban_ip(std::string ip);
	void ban_hostname(std::string hostname);
	void ban_username(std::string username);


}; //end namespace mods::message_server

#endif
