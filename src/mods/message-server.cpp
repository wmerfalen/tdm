#include "message-server.hpp"
#include "message-parser.hpp"
#include "message-encoder.hpp"
#include <zmq.hpp>
#include <zmq_addon.hpp>
#include "orm/user-logins.hpp"

namespace mods::message_server {
#define IS_CHANNEL(A) messages[0].to_string().compare(A) == 0

	bool shutdown_now = false;
	void subscribe_to(zmq::socket_t* subscriber,std::vector<channel_t> list) {
		for(const auto& channel : list) {
			subscriber->set(zmq::sockopt::subscribe, to_string(channel));
		}
	}
	void shutdown_subscriber() {
		zmq::socket_t subscriber(fetch_context(), zmq::socket_type::sub);
		subscriber.connect(mods::message_server::queue.data());

		subscribe_to(&subscriber, {CHAN_SHUTDOWN});

		while(!shutdown_now) {
			// Receive all parts of the message
			std::vector<zmq::message_t> messages;
			zmq::recv_result_t result =
			    zmq::recv_multipart(subscriber, std::back_inserter(messages));
			assert(result && "recv failed");
			assert(*result == 2);

			std::cout << "Thread2: [" << messages[0].to_string() << "] "
			          << messages[1].to_string() << std::endl;
			if(messages[1].to_string().compare("NOW") == 0) {
				for(int i=0; i < 10; i++) {
					std::cerr << "shutdown_subscriber got shutdown signal for NOW!\n";
				}
				shutdown_now = true;
				continue;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(800));
			std::cout << "[shutdown sub]\n";
		}
		std::cerr << "[shutdown_subscriber] exiting due to shutdown_now\n";
	}
	void user_connection_logger() {
		zmq::socket_t subscriber(fetch_context(), zmq::socket_type::sub);
		subscriber.connect(mods::message_server::queue.data());

		subscribe_to(&subscriber, {
			CHAN_USER_LOGGED_IN,
		});

		while(!shutdown_now) {
			std::this_thread::sleep_for(std::chrono::milliseconds(10000));
			// Receive all parts of the message
			std::vector<zmq::message_t> messages;
			zmq::recv_result_t result =
			    zmq::recv_multipart(subscriber, std::back_inserter(messages));
			assert(result && "recv failed");
			assert(*result == 2);

			auto list = mods::message_parser::extract_entries(messages[1].to_string());
			if(list.size() < 3) {
				log("NEEDS_FIXING -> %s:%s:%d -> raw: '%s'",__FILE__,__FUNCTION__,__LINE__,messages[1].to_string().c_str());
			} else {
				mods::orm::user_logins orm;
				orm.user_logged_in(list[0],list[2]);
			}
		}
		std::cerr << "[" << __FUNCTION__ << "] exiting due to shutdown_now\n";
	}

	void admin_subscriber() {
		while(!shutdown_now) {
			std::this_thread::sleep_for(std::chrono::milliseconds(800));
		}
	}

	void user_connection_subscriber() {
		zmq::socket_t subscriber(fetch_context(), zmq::socket_type::sub);
		subscriber.connect(mods::message_server::queue.data());

		subscribe_to(&subscriber, {
			CHAN_USER_LOGGED_IN,
			CHAN_USER_LOGGED_OUT,
			CHAN_NEW_CONNECTION,
			CHAN_BAN_IP,
			CHAN_BAN_HOSTNAME,
			CHAN_BAN_USERNAME,
			CHAN_USER_CONNECTION_REJECTED,
			CHAN_USER_DESTROYED,
		});

		while(!shutdown_now) {
			std::this_thread::sleep_for(std::chrono::milliseconds(800));
			// Receive all parts of the message
			std::vector<zmq::message_t> messages;
			zmq::recv_result_t result =
			    zmq::recv_multipart(subscriber, std::back_inserter(messages));
			assert(result && "recv failed");
			assert(*result == 2);
#ifdef __MENTOC_SUBSCRIBER_DEBUG__
			for(const auto& msg : messages) {
				std::cout << "[" << *result << "] User Connection Subscriber: '" << msg.to_string() << "'\n";
			}
#endif
			const char* channel = messages[0].to_string().c_str();
			if(IS_CHANNEL("NEW_CONNECTION")) {
				auto list = mods::message_parser::extract_entries(messages[1].to_string());
				if(list.size() < 2) {
					log("%s: raw: '%s'",channel,messages[1].to_string().c_str());
				} else {
					log("%s: ip: '%s' hostname: '%s'",channel,list[0].c_str(),list[1].c_str());
				}
			} else if(IS_CHANNEL("USER_LOGGED_IN")) {
				auto list = mods::message_parser::extract_entries(messages[1].to_string());
				if(list.size() < 3) {
					log("%s: raw: '%s'",channel,messages[1].to_string().c_str());
				} else {
					log("%s: ip: '%s' hostname: '%s' username: '%s'",channel,list[0].c_str(),list[1].c_str(),list[2].c_str());
				}
			} else if(IS_CHANNEL("USER_LOGGED_OUT")) {
				auto list = mods::message_parser::extract_entries(messages[1].to_string());
				if(list.size() < 3) {
					log("%s: raw: '%s'",channel,messages[1].to_string().c_str());
				} else {
					log("%s: ip: '%s' hostname: '%s' username: '%s'",channel,list[0].c_str(),list[1].c_str(),list[2].c_str());
				}
			} else if(IS_CHANNEL("USER_CONNECTION_REJECTED")) {
				auto list = mods::message_parser::extract_entries(messages[1].to_string());
				if(list.size() < 2) {
					log("%s: raw: '%s'",channel,messages[1].to_string().c_str());
				} else {
					log("%s: ip: '%s' hostname: '%s'",channel,list[0].c_str(),list[1].c_str());
				}
			} else if(IS_CHANNEL("BAN_IP")) {
				log("%s: ip: '%s'",channel,messages[1].to_string().c_str());
			} else if(IS_CHANNEL("BAN_HOSTNAME")) {
				log("%s: host: '%s'",channel,messages[1].to_string().c_str());
			} else if(IS_CHANNEL("BAN_USERNAME")) {
				log("%s: user: '%s'",channel,messages[1].to_string().c_str());
			}
		}
		std::cerr << "[login_subscriber] exiting due to shutdown_now\n";
	}
	void trigger_mud_shutdown() {
		std::cout << "sending chan shutdown signal!\n";
		publish(CHAN_SHUTDOWN,"NOW");
		std::cout << "done sending signal...\n";
	}
	void new_connection(const sockaddr_in& peer,hostent* from) {
		std::vector<std::string> send_msgs = {
			inet_ntoa(peer.sin_addr),
			from ? from->h_name : "dns-failure"
		};
		publish(CHAN_NEW_CONNECTION,send_msgs);
	}
	void user_logged_in(player_ptr_t& player) {
		std::vector<std::string> payload = {
			player->ip(),
			player->host(),
			player->name().c_str()
		};
		publish(CHAN_USER_LOGGED_IN,payload);
	}

	void user_logged_out(player_ptr_t& player) {
		std::vector<std::string> payload = {
			player->ip(),
			player->host(),
			player->name().c_str()
		};
		publish(CHAN_USER_LOGGED_OUT,payload);
	}

	void ban_ip(std::string ip) {
		publish(CHAN_BAN_IP,ip);
	}
	void ban_hostname(std::string hostname) {
		publish(CHAN_BAN_HOSTNAME,hostname);
	}
	void ban_username(std::string username) {
		publish(CHAN_BAN_USERNAME,username);
	}

	void user_connection_rejected(const sockaddr_in& peer,hostent* from) {
		std::vector<std::string> payload = {
			inet_ntoa(peer.sin_addr),
			from ? from->h_name : "dns-failure"
		};
		publish(CHAN_USER_CONNECTION_REJECTED,payload);
	}
	void publish(const channel_t& channel, const std::string&  msg) {
		auto& publisher = pub_socket();
		publisher.send(zmq::buffer(to_string(channel)), zmq::send_flags::sndmore);
		publisher.send(zmq::buffer(msg));
	}
	void publish(const channel_t& channel, const std::vector<std::string>& list) {
		auto& publisher = pub_socket();
		std::string compiled = mods::message_encoder::encode(list);
		publisher.send(zmq::buffer(to_string(channel)), zmq::send_flags::sndmore);
		publisher.send(zmq::buffer(compiled));
	}



	void spawn() {
		//thread_list().push_front(std::thread(admin_subscriber));
		thread_list().push_front(std::thread(user_connection_subscriber));
		thread_list().push_front(std::thread(shutdown_subscriber));
		thread_list().push_front(std::thread(user_connection_logger));
		for(auto& thread : thread_list()) {
			thread.detach();
		}

	}

#undef IS_CHANNEL
}; //end namespace mods::message_server
