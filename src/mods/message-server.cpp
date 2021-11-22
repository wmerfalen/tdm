#include "message-server.hpp"
#include "message-encoder.hpp"
#include <zmq.hpp>
#include <zmq_addon.hpp>
#include "orm/user-logins.hpp"
#include "message-parser.hpp"

namespace mods::message_server {
#define IS_CHANNEL(A) list[0].compare(A) == 0

	struct channel_message_t {
		uint16_t channel;
		uint16_t control;
		uint64_t message_id;
		std::size_t size;
		void* message;
	};
	constexpr std::size_t header_size = sizeof(channel_t) + sizeof(channel_message_t::control) + sizeof(channel_message_t::message_id) + sizeof(std::size_t);
	static uint64_t message_counter = 0;

	bool shutdown_now = false;
	void subscribe_to(zmq::socket_t* subscriber,std::vector<channel_t> list) {
		for(const auto& channel : list) {
			subscriber->set(zmq::sockopt::subscribe, to_string(channel));
		}
	}



	std::tuple<bool,channel_message_t,std::vector<std::string>> parse_multipart_message(zmq::socket_t* subscriber) {
		std::vector<zmq::message_t> msg;
		zmq::recv_result_t result =
		    zmq::recv_multipart(*subscriber, std::back_inserter(msg));
		assert(result && "recv failed");
		assert(*result > 1);

		channel_message_t m;
		memcpy(&m,msg[1].data(),header_size);

		auto channel_name = to_string((channel_t)m.channel);
		std::cerr << "[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[\n";
		std::cerr << "CHANNEL NAME: -----['" << channel_name << "']-----------\n";
		std::cerr << "[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[\n";
		std::vector<char> raw_message;
		if(msg[1].size() <= header_size) {
			std::cerr << "[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[\n";
			std::cerr << "INVALID HEADER SIZE " << __FUNCTION__ << ":" << __FILE__ << "\n";
			std::cerr << "[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[\n";
			return {0,m,{}};
		}
		int data_size = msg[1].size() - header_size;
		assert(data_size > 0);
		raw_message.resize(data_size + 1);
		char* message_start = (char*)msg[1].data();
		memcpy(&raw_message[0],&message_start[header_size],data_size);
		std::vector<std::string> list = mods::message_parser::extract_entries(&message_start[header_size]);
		for(auto i = 0; i < list.size(); i++) {
			std::cerr << "[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[\n";
			std::cerr << "[" << i << "]->'" << list[i] << "'\n";
			std::cerr << "[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[\n";
		}
		return {1,m,list};
	}



	void shutdown_subscriber() {
		zmq::socket_t subscriber(fetch_context(), zmq::socket_type::sub);
		subscriber.connect(mods::message_server::queue.data());

		subscribe_to(&subscriber, {CHAN_SHUTDOWN});

		while(!shutdown_now) {
			// Receive all parts of the message
			auto tuple = parse_multipart_message(&subscriber);
			bool parsed_okay = std::get<0>(tuple);
			//const auto& m = std::get<1>(tuple);
			const auto& list = std::get<2>(tuple);
			if(!parsed_okay) {
				log("%s failed to parse multipart message",__FUNCTION__);
				continue;
			}

			//std::cout << "Thread2: [" << messages[0].to_string() << "] "
			//          << messages[1].to_string() << std::endl;
			if(list[0].compare("NOW") == 0) {
				shutdown_now = true;
				continue;
			}
		}
	}
	void user_connection_logger() {
		std::cerr << "user_connection_logger ENTRY\n";
		zmq::socket_t subscriber(fetch_context(), zmq::socket_type::sub);
		subscriber.connect(mods::message_server::queue.data());

		subscribe_to(&subscriber, {
			CHAN_USER_LOGGED_IN,
		});


		while(!shutdown_now) {
			auto tuple = parse_multipart_message(&subscriber);
			bool parsed_okay = std::get<0>(tuple);
			//const auto& m = std::get<1>(tuple);
			const auto& list = std::get<2>(tuple);
			if(!parsed_okay) {
				log("%s failed to parse multipart message",__FUNCTION__);
				continue;
			}
			for(auto i = 0; i < list.size(); i++) {
				std::cerr << "[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[\n";
				std::cerr << "[" << i << "]->'" << list[i] << "'\n";
				std::cerr << "[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[\n";
			}
			assert(list.size() > 1);
			mods::orm::user_logins orm;
			orm.user_logged_in(list[0],list[1]);
		}
	}

#if 0
	void admin_subscriber() {
		while(!shutdown_now) {
			std::cerr << "\n###:-" << __FUNCTION__ << "-:###\n";
			std::this_thread::sleep_for(std::chrono::milliseconds(60000));
		}
	}
#endif

	struct message_t_string_adaptor_t {
		message_t_string_adaptor_t(const zmq::message_t& m) : str(m.to_string()) {}
		~message_t_string_adaptor_t() = default;
		message_t_string_adaptor_t() = delete;
		const char* c_str() const {
			return str.c_str();
		}
		std::size_t compare(std::string_view other) {
			return str.compare(other.data());
		}
		std::string str;
	};
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
			// Receive all parts of the message
			auto tuple = parse_multipart_message(&subscriber);
			if(!std::get<0>(tuple)) {
				log("%s failed to parse multipart message", __FUNCTION__);
				continue;
			}
			const auto& m = std::get<1>(tuple);
			const auto& list = std::get<2>(tuple);
			const char* channel = to_string((channel_t)m.channel).c_str();

			switch((channel_t)m.channel) {
				case CHAN_NEW_CONNECTION:
					log("%s: ip: '%s' hostname: '%s'",channel,list[0].c_str(),list[1].c_str());
					break;
				case CHAN_USER_LOGGED_IN:
					log("%s: ip: '%s' hostname: '%s' username: '%s'",channel,list[0].c_str(),list[1].c_str(),list[2].c_str());
					break;
				case CHAN_USER_LOGGED_OUT:
					log("%s: ip: '%s' hostname: '%s' username: '%s'",channel,list[0].c_str(),list[1].c_str(),list[2].c_str());
					break;
				case CHAN_USER_CONNECTION_REJECTED:
					log("%s: ip: '%s'",channel,list[0].c_str());
					break;
				case CHAN_BAN_IP:
					log("%s: ip: '%s'",channel,list[0].c_str());
					break;
				case CHAN_BAN_HOSTNAME:
					log("%s: host: '%s'",channel,list[0].c_str());
					break;
				case CHAN_BAN_USERNAME:
					log("%s: user: '%s'",channel,list[0].c_str());
					break;
				default:
					break;
			}
		}
	}
	void trigger_mud_shutdown() {
		publish(CHAN_SHUTDOWN,"NOW");
	}
	void new_connection(const sockaddr_in& peer,hostent* from) {
		std::vector<std::string> send_msgs = {
			inet_ntoa(peer.sin_addr),
			from ? from->h_name : "dns-failure"
		};
		publish(CHAN_NEW_CONNECTION,send_msgs);
	}
	void user_logged_in(player_ptr_t& player) {
		std::cerr << __FUNCTION__ << "player->ip(): '" << player->ip() << "'\n";
		std::cerr << __FUNCTION__ << "player->desc().ip: '" << player->desc().ip.str() << "'\n";
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
	std::vector<char> construct_payload(const channel_t& channel, const std::string& msg, const uint64_t& message_id, bool is_last = true) {
		channel_message_t m;
		m.channel = channel;
		m.control = (is_last ? 1 : 0);
		m.message_id = message_id;
		m.size = msg.size();

		std::vector<char> buffer;
		buffer.resize(header_size + m.size + 1);
		memcpy(&buffer[0],&m,header_size);
		assert(header_size-1 > 0);
		memcpy(&buffer[header_size],msg.data(),m.size);
		buffer.back() = '\0';
		return buffer;
	}
	void publish(const channel_t& channel, const std::string&  msg) {
		auto& publisher = pub_socket();
		std::string m = CAT("{",msg.length(),"}",msg);
		auto buffer = construct_payload(channel,m,++message_counter,true);
		publisher.send(zmq::buffer(to_string(channel)),zmq::send_flags::sndmore);
		publisher.send(zmq::const_buffer((void*)buffer.data(),buffer.size()));
	}
	void publish(const channel_t& channel, const std::vector<std::string>& list) {
		auto& publisher = pub_socket();
		const auto& message_id = ++message_counter;
		std::string msg;
		for(auto i = 0; i < list.size(); i++) {
			msg += CAT("{",list[i].length(),"}",list[i]);
		}
		auto buffer = construct_payload(channel,msg,message_id);
		publisher.send(zmq::buffer(to_string(channel)),zmq::send_flags::sndmore);
		publisher.send(zmq::const_buffer((void*)buffer.data(),buffer.size()));
	}



	void spawn() {
		//thread_list().push_front(std::thread(admin_subscriber));
		//thread_list().push_front(std::thread(user_connection_subscriber));
		//thread_list().push_front(std::thread(shutdown_subscriber));
		thread_list().push_front(std::thread(user_connection_logger));
		for(auto& thread : thread_list()) {
			thread.detach();
		}

	}

#undef IS_CHANNEL
}; //end namespace mods::message_server
