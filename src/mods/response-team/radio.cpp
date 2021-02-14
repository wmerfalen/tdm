#include "radio.hpp"
#include "../../structs.h"
#include "../builder_util.hpp"
#include "../../globals.hpp"
#include "../npc.hpp"
#include "../mobs/extended-types.hpp"
#include "../../comm.h"
#include "../query-objects.hpp"

namespace mods::response_team::radio {
	const std::vector<std::string_view>& security_force_matches() {
		static std::vector<std::string_view> list;
		static bool instantiated = 0;
		if(!instantiated) {
			list = {"Force two", "Disciple 3", "Disciple 2", "Disciple 1",
			        "Oxide 7"
			       };
		}
		return list;
	}
	std::string broad_location(player_ptr_t& mob) {
		return CAT("at or near ",world[mob->room()].name.str());
	}
	std::string exact_location(player_ptr_t& mob) {
		return CAT(world[mob->room()].x,", ",world[mob->room()].y,", ",world[mob->room()].z);
	}
	std::string interpolate_location(player_ptr_t& mob,std::string_view message) {
		std::string result = "";
		bool capture = 0;
		std::string interpolate = "";
		for(auto ch : message) {
			if(ch == '{') {
				capture = 1;
				continue;
			}
			if(capture && ch == '}') {
				capture = false;
				if(interpolate.compare("broad_location") == 0) {
					result += broad_location(mob);
				}
				if(interpolate.compare("exact_location") == 0) {
					result += exact_location(mob);
				}
				interpolate.clear();
				continue;
			}
			result += ch;
		}
		return std::move(result);
	}
	/**
	 * Radio code meanings
	 * -------------------
	 *  803 niner: officer in distress, medium level alert
	 *  908: officer involved shooting
	 *  Eagle two niner: backup (mostly 2 persons in a car)
	 */
	std::tuple<bool,std::string> respond(player_ptr_t& mob,std::string_view message) {
		if(std::string::npos != message.find_first_of("803 niner")) {
			return {1,CAT("Copy that, ",mob->name().c_str(),". Dispatching Eagle two niner.")};
		}
		if(std::string::npos != message.find_first_of("908")) {
			return {1,CAT("Copy ",mob->name().c_str(),interpolate_location(mob,". All units be advised: code 908 at {exact_location}"))};
		}
		return {0,CAT(mob->name().c_str(),", say again")};
	}
	std::tuple<player_ptr_t,std::string> process_request(uuid_t mob_uuid,std::string_view message) {
		auto npc = ptr_by_uuid(mob_uuid);
		if(!npc) {
			return {nullptr,""};
		}
		message = interpolate_location(npc,message);
		act(CAT("$n speaks into $s radio: '",message.data(),"'!").c_str(),FALSE,npc->cd(),0,0,TO_ROOM);
		auto response = respond(npc,message);
		return {npc,std::get<1>(response)};
	}
	void register_help_message(room_rnum room, severity_t severity) {
		mods::response_team::radio::incidents[room] += (uint8_t)severity;
	}
	void dispatch_help_message(uuid_t mob_uuid,std::string_view message, severity_t severity) {
		auto status = process_request(mob_uuid,message);
		std::string status_message = std::get<1>(status);
		auto npc = ptr_by_uuid(mob_uuid);
		auto room = npc->room();
		mods::globals::defer_queue->push(RADIO_INITIAL_RESPONSE_TICKS(), [room,mob_uuid,status_message,severity]() {
			auto npc = ptr_by_uuid(mob_uuid);
			if(npc) {
				auto ids = mods::query_objects::query_equipment_by_yaml(npc, "gadget/cbradio.yml");
				if(ids.size()) {
					auto radio = optr_by_uuid(ids[0]);
					if(radio) {
						act(CAT(radio->name.c_str()," announces '",status_message.c_str(),"'").c_str(),FALSE,nullptr,0,0,TO_ROOM);
					}
				}
			}
			mods::response_team::radio::register_help_message(room,severity);
		});
	}


	void help(uuid_t mob_uuid, std::string_view message) {
		dispatch_help_message(mob_uuid,message,HELP_WANTED);
	}
	void report_violence(uuid_t mob_uuid, std::string_view message) {
		dispatch_help_message(mob_uuid,message,REPORT_VIOLENCE);
	}
	void help_dazed(uuid_t mob_uuid, std::string_view message) {
		dispatch_help_message(mob_uuid,message,OFFICER_IS_DAZED);
	}

};//end namespace
