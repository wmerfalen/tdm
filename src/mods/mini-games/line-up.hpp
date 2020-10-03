#ifndef __MENTOC_MODS_MINI_GAMES_LINE_UP_HEADER__
#define __MENTOC_MODS_MINI_GAMES_LINE_UP_HEADER__

#include "../../globals.hpp"
#include "../orm/integral-object.hpp"

namespace mods::mini_games {
	struct line_up {
		static constexpr int FROM = 3;
		static constexpr int TO = 6;
		enum highlight_strategy_t : uint8_t {
			HIGHLIGHT_FIRST_NUMBER,
			HIGHLIGHT_ODD_NUMBER,
			HIGHLIGHT_EVEN_NUMBER
		};
		std::string get_body();
		std::string rotate_right(int row);
		std::string rotate_left(int row);
		std::string invalid_row();
		int row_count();
		line_up();
		void seed();
		void reset();
		bool should_highlight(int row_counter,uint8_t value);
		int device_id();
		void set_device_id(int i);

		private:
		int m_device_id;
		highlight_strategy_t highlight_strategy;
		bool needs_regen;
		int position_count;
		std::vector<uint8_t> positions;
		std::vector<std::vector<uint8_t>> rows;
		std::vector<std::vector<uint8_t>> original_rows;
		std::vector<uint8_t> solutions;
		std::string body_content;
		std::vector<uint8_t> first_values;
	};
	void rotate_right(player_ptr_t& player);
	void rotate_left(player_ptr_t& player);
	void reset_hack(player_ptr_t& player);
	void next_row(player_ptr_t& player);
};

ACMD(do_hack);
ACMD(do_rotate_right);
ACMD(do_rotate_left);
ACMD(do_reset_hack);
ACMD(do_next_row);

#endif
