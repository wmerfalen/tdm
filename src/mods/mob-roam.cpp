#include "mob-roam.hpp"
#include <memory>
#include "orm/mob-roam.hpp"

namespace mods::mob_roam {
	using roaming_data_t = std::vector<room_vnum>;
	static std::map<mob_vnum,roaming_data_t> roaming_data;
	static std::vector<std::shared_ptr<mods::mob_roam::roam_recorder>> roam_recorder_data;
	roam_recorder::roam_recorder(
				player_ptr_t builder,
				mob_vnum mob_virtual_number,
				std::string_view roaming_profile_name
		){
		m_builder = builder;
		m_mobs.emplace_back(mob_virtual_number);
		m_roaming_profile_name = roaming_profile_name;
	}
	void roam_recorder::add_room(room_vnum room){
		m_rooms.emplace_back(room);
	}
	void roam_recorder::clear_rooms(){
		m_rooms.clear();
	}
	void roam_recorder::init(){
		m_rooms.clear();
		m_mobs.clear();
		m_builder = nullptr;
		m_roaming_profile_name = "";
		m_capture = 0;
	}
	std::string_view roam_recorder::roaming_profile_name() {
		return m_roaming_profile_name;
	}
	void roam_recorder::set_roaming_profile_name(std::string_view n){
		m_roaming_profile_name = n;
	}
	std::vector<room_vnum>& roam_recorder::rooms() {
		return m_rooms;
	}
	std::vector<mob_vnum>& roam_recorder::mobs() {
		return m_mobs;
	}
	roam_recorder::roam_recorder(
			player_ptr_t builder,
			std::vector<mob_vnum> mob_list,
			std::string_view roaming_profile_name
	){
		m_builder = builder;
		m_mobs = mob_list;
		m_roaming_profile_name = roaming_profile_name;
	}
	std::tuple<int16_t,std::string> roam_recorder::start(){
		m_capture = true;
		return {0,"started"};
	}
	std::tuple<int16_t,std::string> roam_recorder::stop(){
		m_capture = false;
		return {0,"stopped"};
	}
	std::vector<std::shared_ptr<roam_recorder>> get_roam_recorder_data(player_ptr_t& player){
		std::vector<std::shared_ptr<roam_recorder>> found;
		for(auto & recorder_ptr : roam_recorder_data){
			if(player->uuid() == recorder_ptr->builder()){
				found.emplace_back(recorder_ptr);
			}
		}
		return found;
	}
	std::vector<std::shared_ptr<roam_recorder>> get_roam_recorder_data_by_profile_name(player_ptr_t& player,std::string_view profile_name){
		std::vector<std::shared_ptr<roam_recorder>> found;
		for(auto & recorder_ptr : roam_recorder_data){
			if(player->uuid() == recorder_ptr->builder() && recorder_ptr->roaming_profile_name().compare(profile_name) == 0){
				found.emplace_back(recorder_ptr);
			}
		}
		return found;
	}

	std::tuple<int16_t,std::string> start(player_ptr_t& player, const std::vector<mob_vnum>& mobs, std::string_view profile_name){
			roam_recorder_data.emplace_back(std::make_shared<mods::mob_roam::roam_recorder>(player,mobs,profile_name));
			return {0,"Starting recording... move around now. When done, type {grn}mbuild roam:stop{/grn}"};
	}
	std::tuple<int16_t,std::string> stop(player_ptr_t& player, std::string_view profile_name){
		int16_t count_stopped = -1;
		std::string issues = "";
		for(auto & data : roam_recorder_data){
			if(data->builder() == player->uuid() && data->roaming_profile_name().compare(profile_name) == 0){
				data->stop();
				++count_stopped;
			}
		}
		return {count_stopped,issues};
	}
	bool handle_roam_recorder(player_ptr_t& player, const std::vector<std::string>& vec_args){
		if(vec_args.size() >= 3 && vec_args[0].compare("roam:start") == 0){
			/** [0] => roam:start
			 * [1] => profile-name
			 * [2] => mob-vnum
			 * [N] => mob-vnumN
			 */
			std::vector<mob_vnum> mobs;
			for(unsigned i=2; i < vec_args.size();i++){
				mobs.emplace_back(mods::util::stoi<int>(vec_args[i]));
			}
			auto status = start(player,mobs,vec_args[1]);
			if(std::get<0>(status) < 0){
				r_error(player,std::get<1>(status));
			}else{
				r_success(player,std::get<1>(status));
			}
			return true;
		}
		if(vec_args.size() > 1 && vec_args[0].compare("roam:stop") == 0){
			for(unsigned i = 1; i < vec_args.size();i++){
				auto status = stop(player,vec_args[1]);
				if(std::get<0>(status) < 0){
					r_error(player,CAT("Error while saving profile:'",vec_args[i],"': ",std::get<1>(status)));
				}else{
					r_success(player,CAT("Successfully saved profile:'",vec_args[i],"'",std::get<1>(status)));
				}
			}
			return true;
		}
		return false;
	}
	const std::vector<std::string>& roam_recorder_help_screen(){
		static std::vector<std::string> screen = {
			" {grn}mbuild{/grn} {red}roam:start <profile-name> <mob-vnum>...<mob-vnum-N>{/red}",
			"  |--> Will start recording the rooms you visit and set the mob-vnum's roam data to those rooms." ,
			"  {grn}|____[example]{/grn}" ,
			"  |:: {wht}mbuild{/wht} {gld}roam:start \"corporal-roaming-1\" 410 8 3 40{/gld}" ,
			" {grn}mbuild{/grn} {red}roam:stop <profile-name>...<profile-name-N>{/red}",
			" {grn}mbuild{/grn} {red}roam:save <profile-name>{/red}",
			"  |--> Will save the recording based on the profile name you gave in the roam:start command." ,
			"  {grn}|____[example]{/grn}" ,
			"  |:: {wht}mbuild{/wht} {gld}roam:save \"corporal-roaming-1\"{/gld}" ,
			"[documentation written on 2020-12-15]",
		};
		return screen;
	}
	void boot(){
		std::tuple<int16_t,std::string> status = mods::orm::load_all_roaming_data(&roaming_data);
	}
};
