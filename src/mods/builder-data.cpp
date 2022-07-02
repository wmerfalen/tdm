#include "builder-data.hpp"
#include <deque>
//#include "../utils.h"
#include "../structs.h"
#include "../builder/raid.hpp"
//#include ".../comm.h"
//#include <array>

extern int next_room_pavement_transaction_id();
extern std::deque<room_data> world;
int builder_data_t::raid_id() {
	if(raid) {
		std::cout << "raid orm id: " << raid->orm.id << "\n";
		return raid->orm.id;
	}
	std::cout << "raid just returning zero\n";
	return 0;
}

builder_data_t::builder_data_t(int type,int start_room,int start_zone) {
	room_pavements.start_room = start_room;
	room_pavements.transact_id = next_room_pavement_transaction_id();
	room_pavements.zone_id = start_zone;
	raid = nullptr;
	raid_pave = false;
}
std::tuple<bool,std::string,uint64_t> builder_data_t::raid_pave_on(std::string_view name, std::string_view level, std::string_view type) {
	if(raid) {
		return {false,"A raid has already been started. Close that out first",0};
	}
	raid = std::make_shared<mods::builder::raid_t>(name,level,type);
	auto id = raid->orm.initialize_row(name,level,type,"INCOMPLETE");
	raid->orm.id = 0;
	if(id <= 0) {
		return {false,"Failed to init row",id};
	}
	raid->orm.id = id;
	raid_pave = true;
	return {true,"okay",id};
}
std::tuple<bool,std::string,uint64_t> builder_data_t::raid_pave_off() {
	uint64_t id = 0;
	std::string msg;
	bool okay = false;
	raid_pave = false;
	if(raid) {
		msg = "closing";
		id = raid->orm.id;
		okay = true;
	} else {
		msg = "pave was never started!";
		id = -1;
		okay = false;
	}
	raid = nullptr;
	return {okay,msg,id};
}
void room_recorder_t::start_recording() {
	record = true;
	room_list.clear();
}
void room_recorder_t::stop_recording() {
	record = false;

}
room_recorder_t::room_list_t& room_recorder_t::rooms() {
	return room_list;
}
bool room_recorder_t::should_record() const {
	return record;
}
void room_recorder_t::char_to_room(const room_rnum& r) {
	if(record) {
		room_list.emplace_back(r);
	}
}
room_recorder_t::room_recorder_t() : record(0) {}
void room_recorder_t::clear() {
	room_list.clear();
}
room_recorder_t::room_vnum_list_t room_recorder_t::room_vnums() {
	room_recorder_t::room_vnum_list_t r;
	for(const auto& rnum : room_list) {
		r.emplace_back(world[rnum].number);
	}
	return r;
}
