#ifndef __MENTOC_MODS_CHAT_HEADER__
#define __MENTOC_MODS_CHAT_HEADER__
#include "../structs.h"
#include "util.hpp"
namespace mods::chat {
	using str_t = std::string;
	void add_public_channel(str_t channel,str_t verb);
	template <typename TPlayer,typename TActionString>
	void communicate(TPlayer obj,TActionString type,TActionString message) {
#ifndef TO_VICT
#define TO_VICT		2
#define TO_SLEEP	128	/* to char, even if sleeping */
#define MENTOC_HACK_USED 1
#define KNRM  "\x1B[0m"
#define C_NRM	2
#endif
		auto ch = obj->cd();
		std::string buffer = "$n ";
		buffer += type.data();
		buffer += "s, '";
		buffer += message.data();
		buffer += "'";
		/* now send all the strings out */
		for(auto& i : descriptor_list) {
			if(STATE(i) == CON_PLAYING && i.character && i.character->uuid != ch->uuid &&
			        //!PRF_FLAGGED(i.character, channels[subcmd]) &&
			        !PLR_FLAGGED(i.character, PLR_WRITING) &&
			        !ROOM_FLAGGED(IN_ROOM(i.character), ROOM_SOUNDPROOF)) {

				if(std::string(type.data()).compare("shout") == 0 &&
				        ((world[IN_ROOM(ch)].zone != world[IN_ROOM(i.character)].zone) ||
				         !AWAKE(i.character))) {
					continue;
				}

#ifdef __MENTOC_USE_COLOR_ON__
				if(COLOR_LEV(i.character) >= C_NRM) {
					send_to_char(i.character, "%s", color_on);
				}
#endif

				act(buffer.c_str(), FALSE, ch, 0, i.character, TO_VICT | TO_SLEEP);
#ifdef __MENTOC_USE_COLOR_ON__
				if(COLOR_LEV(i.character) >= C_NRM) {
					ptr(i.character)->send(KNRM);
				}
#endif
			}
		}

#ifdef MENTOC_HACK_USED
#undef TO_VICT
#undef TO_SLEEP
#undef MENTOC_HACK_USED
#undef KNRM
#undef C_NRM
#endif

	}
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
