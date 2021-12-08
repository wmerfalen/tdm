#ifndef __MENTOC_MODS_CHAT_HEADER__
#define __MENTOC_MODS_CHAT_HEADER__
#include "../structs.h"
#include "util.hpp"
#include "player.hpp"
namespace mods::chat {
	using str_t = std::string;
	void add_public_channel(str_t channel,str_t verb);
	template <typename TPlayer,typename TActionString>
	void communicate(TPlayer obj,TActionString type,TActionString message) {}

	struct channel {
			channel() = delete;
			channel(std::string_view name,std::string_view verb,bool b_private) :
				m_name(name), m_verb(verb), m_private(b_private) {}
			~channel() = default;
			void set_name(std::string_view);
			void set_verb(std::string_view);
			void set_private(bool b_private) {
				m_private = b_private;
			}
			bool get_private() const {
				return m_private;
			}
			std::string_view get_name() const;
			std::string_view get_verb() const;
			void add_subscriber(const socket_t&);
			void remove_subscriber(const socket_t&);
			void add_private_publisher(std::string_view user);
			const std::vector<std::string>& get_private_publishers() const {
				return m_private_publishers;
			}
			void transmit(std::string_view user,std::string_view message);
		private:
			std::vector<socket_t> m_subscribers;
			std::vector<std::string> m_private_publishers;
			std::string m_name;
			std::string m_verb;
			bool m_private;
	};
	void transmit(std::string verb,std::string_view player_name,std::string_view message);
	void setup_public_channels();
	bool handle_chat(player_ptr_t& player,std::string_view);
};
#endif
