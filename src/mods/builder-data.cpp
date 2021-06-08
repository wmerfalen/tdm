#include "builder-data.hpp"
#include <deque>
//#include "../utils.h"
#include "../structs.h"
//#include ".../comm.h"
//#include <array>

extern int next_room_pavement_transaction_id();
extern std::deque<room_data> world;

builder_data_t::builder_data_t(int type,int start_room,int start_zone) {
	//switch(type){
	//	default:
	//	case ROOM_PAVEMENT:
	//		/** TODO: I want this functionality to be done next
	//		room_pavement_list.emplace_back(start_room,start_zone);
	//		room_pavement_list.back().transact_id = next_room_pavement_transaction_id();
	//		*/
	room_pavements.start_room = start_room;
	room_pavements.transact_id = next_room_pavement_transaction_id();
	room_pavements.zone_id = start_zone;
	//			break;
	//		case ZONE_PAVEMENT:
	//			/** FIXME: this is incomplete code. fix this */
	//			d("[builder_data_t::create_pavement] WARNING: zone pavements are experimental!\n");
	//			break;
	//	}
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
