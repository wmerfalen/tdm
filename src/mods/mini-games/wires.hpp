#ifndef __MENTOC_MODS_MINI_GAMES_WIRES_HEADER__
#define __MENTOC_MODS_MINI_GAMES_WIRES_HEADER__

#include "../../globals.hpp"
#include "../orm/integral-object.hpp"

namespace mods::mini_games {
	/**
	 * A set of wires is exposed in this panel. 
	 * You see two rows of three ethernet ports
	 * The two rows have letters next to them.
	 * The first row is labeled A, B, C from left to right.
	 * The second row is labeled D, E, F from left to right.
	 * Which ethernet port do you want to hook into?
	 *
	 * A -> You hear one beep as you insert into ethernet port A
	 *   -> The light next to ethernet port A lights up. You must now 
	 *      insert into an ethernet port on the second row.
	 * B -> You hear two beeps as you insert into ethernet port B
	 * C -> Nothing happens
	 *
	 * D -> You hear two beeps as you insert into ethernet port D
	 *   -> Resets the game
	 * E -> You hear nothing
	 *   -> Resets the game
	 * F -> You hear one beep as you insert into ethernet port F
	 *   -> Solution reached. Good job.
	 *
	 * [solution]
	 * You have to insert a port into ethernet port A and ethernet port F
	 * This will then run the mini game action.
	 * The game will then reset.
	 */
	struct wires {
		static constexpr std::string_view HEADER = "[ TN 3 v1.0.12 - Baphomet O.S. ]\r\n"
		 "A set of RJ45 Ethernet ports are exposed in this panel. \r\n"
		 "You see two rows of three ethernet ports.\r\n"
		 "The two rows have letters next to them.\r\n"
		 "The first row is labeled A, B, C from left to right.\r\n"
		 "The second row is labeled D, E, F from left to right.\r\n"
		 ;
		static constexpr std::string_view DISPLAY = "A small LED display on the panel says: ";
		 
		static constexpr std::string_view FOOTER = "\r\n"
			"You notice an engraving in the bottom of the panel:\r\n"
			"[ BOS SYSTEMS ]\r\n";

		static constexpr uint8_t TO = 2;
		static constexpr uint8_t FROM = 0;
		static constexpr int ROWS  = 2;
		static constexpr int COLUMNS = 3;
		enum highlight_strategy_t : uint8_t {
			HIGHLIGHT_CURRENT_ROW
		};
		enum port_behaviour_t {
			BEEP_ONCE,
			BEEP_TWICE,
			BEEP_ZERO
		};
		std::string get_body();
		std::string plug_into(char port);
		wires();
		void seed();
		void reset();
		int device_id();
		void set_device_id(int i);
		void unplug_all();
		void next_row();

		private:
		std::vector<bool> plugged_in;
		std::tuple<uint8_t,uint8_t> solution;
		uint8_t current_row;
		uint32_t m_device_id;
		highlight_strategy_t highlight_strategy;
		bool needs_regen;
		std::vector<uint8_t> beeps;
		std::string body_content;
	};
};


#endif
