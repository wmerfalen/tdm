#include "message-server.hpp"
#include <zmq.hpp>
#include <zmq_addon.hpp>

namespace mods::message_server {
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
			std::vector<zmq::message_t> recv_msgs;
			zmq::recv_result_t result =
			    zmq::recv_multipart(subscriber, std::back_inserter(recv_msgs));
			assert(result && "recv failed");
			assert(*result == 2);

			std::cout << "Thread2: [" << recv_msgs[0].to_string() << "] "
			          << recv_msgs[1].to_string() << std::endl;
			if(recv_msgs[1].to_string().compare("NOW") == 0) {
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
			std::vector<zmq::message_t> recv_msgs;
			zmq::recv_result_t result =
			    zmq::recv_multipart(subscriber, std::back_inserter(recv_msgs));

			for(const auto& msg : recv_msgs) {
				std::cout << "[" << *result << "] User Connection Subscriber: '" << msg.to_string() << "'\n";
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(800));
		}
		std::cerr << "[login_subscriber] exiting due to shutdown_now\n";
	}

	void admin_subscriber() {
		while(!shutdown_now) {
			std::this_thread::sleep_for(std::chrono::milliseconds(800));
		}
	}

	void spawn() {
		//thread_list().push_front(std::thread(admin_subscriber));
		thread_list().push_front(std::thread(user_connection_subscriber));
		thread_list().push_front(std::thread(shutdown_subscriber));
		for(auto& thread : thread_list()) {
			thread.detach();
		}

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

	}
	void user_logged_out(player_ptr_t& player) {

	}

	void user_connection_rejected(const sockaddr_in& peer,hostent* from) {

	}
	void publish(const channel_t& channel, const std::string&  msg) {
		auto& publisher = pub_socket();
		publisher.send(zmq::buffer(to_string(channel)), zmq::send_flags::sndmore);
		publisher.send(zmq::buffer(msg));
	}
	void publish(const channel_t& channel, const std::vector<std::string>& list) {
		auto& publisher = pub_socket();
		std::string compiled;
		std::size_t i =0;
		for(const auto& msg : list) {
			compiled += CAT("[",i++,"]:{",msg.length(),"}:",msg);
		}
		publisher.send(zmq::buffer(to_string(channel)), zmq::send_flags::sndmore);
		publisher.send(zmq::buffer(compiled));
	}
	void publish(const channel_t& channel, const std::vector<zmq::const_buffer>& list) {
		zmq::socket_t sock1(fetch_context(), zmq::socket_type::push);
		sock1.bind(queue.data());
		const std::string last_endpoint = sock1.get(zmq::sockopt::last_endpoint);

		/*
		auto& publisher = pub_socket();
		for(const auto& msg : list) {
			publisher.send(zmq::buffer(to_string(channel)), zmq::send_flags::sndmore);
			publisher.send(zmq::buffer(msg));
		}
		*/
		//auto& publisher = pub_socket();
		std::vector<zmq::const_buffer> send_msgs;
		for(const auto& msg : list) {
			std::cerr << "creating send_msgs...\n";
			send_msgs.emplace_back(zmq::buffer(msg));
		}
		zmq::send_multipart(sock1,send_msgs);
	}

}; //end namespace mods::message_server
