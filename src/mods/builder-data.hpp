#ifndef __MENTOC_MODS_BUILDER_DATA_HEADER__
#define __MENTOC_MODS_BUILDER_DATA_HEADER__
#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <list>
#include <set>
#include "string.hpp"
#include <array>
#include "index-type.hpp"

namespace mods::builder {
	struct raid_t;
};

/* Pave mode structure for builders */
struct room_pavement_t {
	int start_room;
	int transact_id;
	room_vnum current_room_number;
	int zone_id;
	std::vector<int> rooms;
	room_pavement_t() : start_room(0), transact_id(-1),
		current_room_number(0), zone_id(-1) {}
	room_pavement_t(int start,int z_id) :  start_room(start), transact_id(0),
		current_room_number(0),zone_id(z_id) {}
	room_pavement_t(int start,int z_id,int t_id) :  start_room(start), transact_id(t_id),
		current_room_number(0),zone_id(z_id) {}
	~room_pavement_t() = default;
};

struct zone_pavement_t {
	typedef std::vector<mob_vnum> mob_pavements_t;
	typedef std::vector<obj_vnum> obj_pavements_t;
	mob_pavements_t mob;
	obj_pavements_t obj;
	zone_pavement_t() = default;
	~zone_pavement_t() = default;
};

struct room_recorder_t {
		using room_list_t = std::vector<room_rnum>;
		using room_vnum_list_t = std::vector<room_rnum>;
		void start_recording();
		void stop_recording();
		room_list_t& rooms();
		room_vnum_list_t room_vnums();
		bool should_record() const;
		void char_to_room(const room_rnum& r);
		room_recorder_t();
		void clear();
	private:
		bool record;
		room_list_t room_list;
};

struct builder_data_t {
		bool room_pave_mode;
		bool zone_pave_mode;
		builder_data_t(int type,int start_room,int start_zone);

		room_pavement_t room_pavements;
		zone_pavement_t zone_pavements;
		std::vector<room_pavement_t> room_pavement_list;
		std::vector<zone_pavement_t> zone_pavement_list;
		builder_data_t() :
			room_pave_mode(false),zone_pave_mode(false),
			room_transaction_id(0),
			zone_transaction_id(0), raid_pave(false),
			raid(nullptr)
		{ }
		builder_data_t(const builder_data_t& other) :
			room_pave_mode(other.room_pave_mode),zone_pave_mode(other.zone_pave_mode),
			room_transaction_id(other.room_transaction_id),
			zone_transaction_id(other.zone_transaction_id), raid_pave(other.raid_pave),
			raid(other.raid) {
			//std::cout << "[builder_data_t::copy constructor]\n";
		}
		~builder_data_t() = default;
		int room_transaction_id;
		int zone_transaction_id;
		std::map<std::string,room_rnum> bookmarks;
		room_recorder_t room_recorder;
		std::tuple<bool,std::string,uint64_t> raid_pave_on(std::string_view name, std::string_view level, std::string_view type,room_vnum r);
		std::tuple<bool,std::string,uint64_t> raid_pave_off();
		std::tuple<bool,std::string,uint64_t> raid_set_spawn(room_vnum);
		int raid_id();
	private:
		bool raid_pave;
		std::shared_ptr<mods::builder::raid_t> raid;
};

#endif
