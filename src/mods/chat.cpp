#include "chat.hpp"
#include "../utils.h"

namespace mods::chat {
	void channel::set_name(std::string_view name) {
		m_name = name;
	}
	void channel::set_verb(std::string_view verb) {
		m_verb = verb;
	}
	std::string_view channel::get_name() const {
		return m_name;
	}
	std::string_view channel::get_verb() const {
		return m_verb;
	}
	void channel::add_subscriber(const socket_t& sock) {
		if(std::find(m_subscribers.begin(),m_subscribers.end(),sock) == m_subscribers.end()) {
			m_subscribers.push_back(sock);
		}
	}
	void channel::remove_subscriber(const socket_t& sock) {
		auto it = std::find(m_subscribers.begin(),m_subscribers.end(),sock);

		if(it != m_subscribers.end()) {
			m_subscribers.erase(it);
		}
	}
	void channel::add_private_publisher(std::string_view user) {
		auto it = std::find(m_private_publishers.begin(),m_private_publishers.end(),user);

		if(it == m_private_publishers.end()) {
			m_private_publishers.push_back(user.data());
		}
	}
	void channel::transmit(std::string_view user,std::string_view message) {
		if(m_private) {
			if(std::find(
			            m_private_publishers.begin(),
			            m_private_publishers.end(),
			            user) ==
			        m_private_publishers.end()) {
				return;
			}
		}

		std::string msg = "{gld}";
		msg += user;
		msg += " ";
		msg += m_verb;
		msg += ": ";
		msg += message;
		msg += "{/gld}";
		msg = mods::globals::color_eval(msg);

		for(auto descriptor : m_subscribers) {
			auto write_value = write(descriptor,msg.c_str(),msg.length());
			if(write_value == -1){
				log(
					(
					 std::string("channel::transmit::write error: ") + 
					 strerror(errno)
					 ).c_str()
				);
			}
		}
	}
};

ACMD(do_chanmgr) {
	

	if(std::string(argument).length() == 0 || std::string(argument).compare("help") == 0) {
		player->pager_start() << "usage: \r\n" <<
		                      " chanmgr help\r\n" <<
		                      "  |--> this help menu\r\n" <<
		                      "  |____[example]\r\n" <<
		                      "  |:: chanmgr help\r\n" <<
		                      "  |:: (this help menu will show up)\r\n" <<
		                      "\r\n" <<
		                      " chanmgr create <channel> <verb>\r\n" <<
		                      "  |--> create a channel to chat over\r\n" <<
		                      "  |____[example]\r\n"<<
		                      "  |:: chanmgr create gossip gossip\r\n" <<
		                      "  |:: (creates a 'gossip' channel that can be accessed by typing 'gossip'.\r\n" <<
		                      "  |:: this channel will be accessbile to all players of the mud globally.\r\n" <<
		                      "  |:: to remove a subscriber from a channel, the subscriber themselves\r\n" <<
		                      "  |:: has to issue a 'mute <channel>'. In this case it would be 'mute gossip'\r\n" <<
		                      "  |:: )\r\n" <<
		                      "\r\n" <<
		                      " chanmgr create-hidden <channel> <verb>\r\n" <<
		                      "  |--> create a hidden channel (for admins mostly)\r\n" <<
		                      "  |____[example]\r\n" <<
		                      "  |:: chanmgr create-hidden nerd admin1\r\n" <<
		                      "  |:: (creates a hidden channel called nerd which can be accessed by\r\n" <<
		                      "  |:: typing 'admin1'. Nobody will be subscribed to this channel initially\r\n" <<
		                      "  |:: so a subsequent 'chanmgr add <publisher> <channel>' call must be done to add\r\n" <<
		                      "  |:: publishers to that channel. hidden channels cannot be muted.)\r\n" <<
		                      "\r\n" <<
		                      " chanmgr add <publisher> <channel> ... <channel N>\r\n" <<
		                      "  |--> add a user (publisher) to a (usually) hidden channel(s). This command\r\n" <<
		                      "  |--> can take several <channel> arguments.\r\n" <<
		                      "  |____[example]\r\n" <<
		                      "  |:: chanmgr add terry admin1\r\n" <<
		                      "  |:: (adds the user 'terry' to the 'admin1' channel. They can now use\r\n" <<
		                      "  |:: the associated verb to chat on the admin1 channel)\r\n" <<
		                      "  |____[example]\r\n" <<
		                      "  |:: chanmgr add terry admin1 gossip market trivia\r\n" <<
		                      "  |:: (adds the user 'terry' to the 'admin1', 'gossip','market' \r\n" <<
		                      "  |:: and 'trivia' channels all in one nice command)\r\n" <<
		                      "\r\n" <<
		                      " [documentation written on 2017-12-05]\r\n" <<
		                      "\r\n";
		player->pager_end();
		player->page(0);
		return;
	}

	constexpr unsigned int max_char = 14;
	std::array<char,max_char> command;

	if(std::string(&command[0]).compare("add") == 0) {
		std::string arg = argument;
		auto past = arg.substr(arg.find("add ") + 4);
		/* i.e.: chanmgr add terry gossip */
		auto arglist = mods::util::arglist<std::vector<std::string>>(past);

		if(arglist.size() < 2) {
			*player << "{red}Missing parameters\r\n";
			return;
		}

		for(unsigned i = 1; i < arglist.size(); i++) {
			for(auto channel : mods::globals::chan) {
				if(std::string(channel.get_name()).compare(arglist[i]) == 0) {
					channel.add_private_publisher(arglist[0]);
					*player << "{red}Added user to {gld}" << arglist[i] << "{/gld}\r\n";
				}
			}
		}

		return;
	}

	if(std::string(&command[0]).compare("create-hidden") == 0) {
		std::string arg = argument;
		auto past = arg.substr(arg.find("create-hidden ") + 14);
		/* i.e.: chanmgr create go gossip */
		auto arglist = mods::util::arglist<std::vector<std::string>>(past);

		if(arglist.size() < 2) {
			*player << "{red}Missing parameters\r\n";
			return;
		}

		/* Emplaced back: name_of_channel,verb */
		mods::globals::chan.emplace_back(arglist[0],arglist[1],true);
		mods::globals::chan_verbs.emplace_back(arglist[1]);
	}

	if(std::string(&command[0]).compare("create") == 0) {
		std::string arg = argument;
		auto past = arg.substr(arg.find("create ") + 7);
		/* i.e.: chanmgr create go gossip */
		auto arglist = mods::util::arglist<std::vector<std::string>>(past);

		if(arglist.size() < 2) {
			*player << "{red}Missing parameters\r\n";
			return;
		}

		/* Emplaced back: name_of_channel,verb */
		mods::globals::chan.emplace_back(arglist[0],arglist[1],false);
		mods::globals::chan_verbs.emplace_back(arglist[1]);
		return;
	}

}
