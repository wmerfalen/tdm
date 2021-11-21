#include "message-server.hpp"

namespace mods::message_server {
	void main() {
		zmq::context_t ctx;
		zmq::socket_t sock(ctx, zmq::socket_type::push);
		sock.bind("inproc://test");
		sock.send(zmq::str_buffer("Hello, world"), zmq::send_flags::dontwait);
	}
}; //end namespace mods::message_server

