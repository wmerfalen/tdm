#ifndef __MENTOC_MODS_MOB_ROAM_HEADER__
#define __MENTOC_MODS_MOB_ROAM_HEADER__
#include "../structs.h"
#include "util.hpp"
#include "../globals.hpp"
#include <string_view>

extern void r_error(const player_ptr_t & player,std::string_view msg);
extern void r_success(const player_ptr_t & player,std::string_view msg);
extern void r_status(const player_ptr_t & player,std::string_view msg);
namespace mods::mob_roam {
	struct roam_recorder {
		enum mode_t {
			RR_MODE_MANUAL = 1,
			RR_MODE_PAVE = 2
		};
		roam_recorder();
		~roam_recorder() = default;
		roam_recorder(
				player_ptr_t builder,
				mob_vnum mob_virtual_number,
				std::string_view roaming_profile_name
		);
		roam_recorder(
				player_ptr_t builder,
				std::vector<mob_vnum> mob_virtual_number,
				std::string_view roaming_profile_name
		);
		std::string_view roaming_profile_name();
		void set_roaming_profile_name(std::string_view n);
		std::vector<room_vnum>& rooms();
		std::vector<mob_vnum>& mobs();

		uuid_t builder(){ return m_builder->uuid(); }
		void init();
		void add_room(room_vnum room);
		void clear_rooms();

		void set_mode(mode_t m);
		mode_t get_mode() const { return m_mode; }

		std::tuple<int16_t,std::string> start();
		std::tuple<int16_t,std::string> stop();

		protected:
			bool m_capture;
			mode_t m_mode;
			std::string m_roaming_profile_name;
			player_ptr_t m_builder;
			std::vector<mob_vnum> m_mobs;
			std::vector<room_vnum> m_rooms;
	};
	bool handle_roam_recorder(player_ptr_t& player, const std::vector<std::string>& arguments);
	const std::vector<std::string>& roam_recorder_help_screen();
	void boot();
};
#endif
