#include "chat.hpp"

namespace mods::chat{
	void channel::set_name(std::string_view name){
		m_name = name;
	}
	void channel::set_verb(std::string_view verb){
		m_verb = verb;
	}
	std::string_view channel::get_name() const{ return m_name; }
	std::string_view channel::get_verb() const{ return m_verb; }
	void channel::add_subscriber(const socket_t & sock){
		if(std::find(m_subscribers.begin(),m_subscribers.end(),sock) == m_subscribers.end()){
			m_subscribers.push_back(sock);
		}
	}
	void channel::remove_subscriber(const socket_t & sock){
		auto it = std::find(m_subscribers.begin(),m_subscribers.end(),sock);
		if(it != m_subscribers.end()){
			m_subscribers.erase(it);
		}
	}
	void channel::transmit(std::string_view user,std::string_view message){
		std::string msg = "{gld}";
		msg += user;
		msg += " ";
		msg += m_verb;
		msg += ": ";
		msg += message;
		msg += "{/gld}";
		msg = mods::util::color_eval(msg);
		for(auto descriptor : m_subscribers){
			write(descriptor,msg.c_str(),msg.length());
		}
	}
};
