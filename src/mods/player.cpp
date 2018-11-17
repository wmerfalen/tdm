#ifndef __MENTOC_MODS_PLAYER_SOURCE__
#define  __MENTOC_MODS_PLAYER_SOURCE__

#include "player.hpp"
#include "../structs.h"
#include "../utils.h"
#include "weapon.hpp"
#include "../globals.hpp"
#include "acl/color.hpp"
#include "prefs.hpp"
#include <chrono>
#include <bitset>
#include <algorithm>
/**
 * TODO: All these stc* functions need to be altered to accomodate
 * the new player_type_enum_t values. If output is to be muted, then
 * we can stop the output from this class and avoid the overhead of
 * calling whatever functions the send_to_char function calls to queue
 * output.
 */

extern void do_auto_exits(struct char_data *ch);
extern mods::player::descriptor_data_t descriptor_list;
namespace mods {
	constexpr static int64_t PLR_FLAG_COUNT = plr::__LAST + 1;
	constexpr static int64_t AFF_FLAG_COUNT = aff::__LAST + 1;
				const std::array<std::pair<mods::player::plr,int64_t>,PLR_FLAG_COUNT> all_plr_flags = {
					{plr::KILLER,PLR_KILLER},{plr::THIEF,PLR_THIEF},{plr::FROZEN,PLR_FROZEN},{plr::DONTSET,PLR_DONTSET},
					{plr::WRITING,PLR_WRITING},{plr::MAILING,PLR_MAILING},{plr::CRASH,PLR_CRASH},{plr::SITEOK,PLR_SITEOK},
					{plr::NOSHOUT,PLR_NOSHOUT},{plr::NOTITLE,PLR_NOTITLE},{plr::DELETED,PLR_DELETED},{plr::LOADROOM,PLR_LOADROOM},
					{plr::NOWIZLIST,PLR_NOWIZLIST},{plr::INVSTART,PLR_INVSTART},{plr::CRYO,PLR_CRYO},
					{plr::NOTDEADYET,PLR_NOTDEADYET}
				};
				const std::array<int64_t,AFF_FLAG_COUNT> all_aff_flags = {
{aff::BLIND,AFF_BLIND},
 {aff::INVISIBLE,AFF_INVISIBLE},
 {aff::DETECT_ALIGN,AFF_DETECT_ALIGN},
 {aff::DETECT_INVIS,AFF_DETECT_INVIS},
 {aff::DETECT_MAGIC,AFF_DETECT_MAGIC},
 {aff::SENSE_LIFE,AFF_SENSE_LIFE},
 {aff::WATERWALK,AFF_WATERWALK},
{aff::SANCTUARY,AFF_SANCTUARY},
 {aff::GROUP,AFF_GROUP},
 {aff::CURSE,AFF_CURSE},
 {aff::INFRAVISION,AFF_INFRAVISION},
 {aff::POISON,AFF_POISON},
 {aff::PROTECT_EVIL,AFF_PROTECT_EVIL},
 {aff::PROTECT_GOOD,AFF_PROTECT_GOOD},
{aff::SLEEP,AFF_SLEEP},
 {aff::NOTRACK,AFF_NOTRACK},
 {aff::UNUSED16,AFF_UNUSED16},
 {aff::UNUSED17,AFF_UNUSED17},
 {aff::SNEAK,AFF_SNEAK},
 {aff::HIDE,AFF_HIDE},
{aff::UNUSED20,AFF_UNUSED20},
{aff::CHARM,AFF_CHARM}
				};
	std::string just_color_evaluation(std::string final_buffer) {
		final_buffer = mods::globals::replace_all(final_buffer,"{grn}","\033[32m");
		final_buffer = mods::globals::replace_all(final_buffer,"{red}","\033[31m");
		final_buffer = mods::globals::replace_all(final_buffer,"{blu}","\033[34m");
		final_buffer = mods::globals::replace_all(final_buffer,"{wht}","\033[37m");
		final_buffer = mods::globals::replace_all(final_buffer,"{/grn}","\033[0m");
		final_buffer = mods::globals::replace_all(final_buffer,"{/wht}","\033[0m");
		final_buffer = mods::globals::replace_all(final_buffer,"{/red}","\033[0m");
		final_buffer = mods::globals::replace_all(final_buffer,"{/blu}","\033[0m");
		return final_buffer;
	}
	void stc_color_evaluation(const std::string& title,player* p) {
		*p << just_color_evaluation(title) << "\r\n";
	}

	std::string word_wrap(std::string_view paragraph,int width) {
		std::string buffer;

		if(width <= 0) {
			return std::string(paragraph);
		}

		int position = 1;

		for(unsigned cint = 0; cint < paragraph.length(); cint++) {
			/* if we run into an open brace then that means it's a color code.
			 * Push past the color code and decrement the position counter as
			 * we go.
			 */
			if(paragraph[cint] == '{') {
				while(paragraph[cint] != '}'
				        && cint < paragraph.length()
				     ) {
					buffer += paragraph[cint++];
					--position;
				}

				buffer += '}';
				continue;
			}

			/* Edge case where the first character in the paragraph is
			 * a color code. The position needs to be 1 since after pushing
			 * past the color code, the position would be a negative value
			 */
			if(position < 1) {
				position = 1;
			}

			if(isspace(paragraph[cint])) {
				/* if the buffer is non-empty and that most recent character
				 * pushed is a space then we would be essentially doubling
				 * up on spaces if we didn't 'continue' here.
				 */
				if(buffer.begin() != buffer.end() &&
				        isspace(*(buffer.end() - 1))) {
					continue;
				} else {
					/* the most recent character was *not* a space so we
					 * can safely append a space without fear of doubling
					 * down on spaces
					 */
					buffer += ' ';
					++position;
				}
			} else {
				buffer += paragraph[cint];
				++position;
			}

			/* we have pushed past the desired screen width. For this,
			 * back track to the last space and insert a newline
			 */
			if(position >= width) {
				if(!isspace(paragraph[cint])) {
					//Perform backtracking
					//====================
					for(int k = buffer.size(); k > 0; k--) {
						if(isspace(buffer[k])) {
							buffer[k] = '\n';
							break;
						}
					}

					/* reset the position back to 1 */
					position = 1;
				}
			}
		}

		return buffer;
	}

	void player::set_class_capability(const class_capability_t& caps) {
		m_class_info.clear();

		/*
		for(auto& capability : caps) {
			switch(capability) {
				case class_type::MEDIC:
					m_class_info.push_back(std::make_shared<classes::medic>(m_self_ptr));
					break;

				case class_type::SNIPER:
					m_class_info.push_back(std::make_shared<classes::sniper>(m_self_ptr));
					break;

				case class_type::MARINE:
					m_class_info.push_back(std::make_shared<classes::marine>(m_self_ptr));
					break;

				case class_type::SUPPORT:
					m_class_info.push_back(std::make_shared<classes::support>(m_self_ptr));
					break;

				case class_type::ENGINEER:
					m_class_info.push_back(std::make_shared<classes::engineer>(m_self_ptr));
					break;
				case class_type::UNDEFINED:
				default:
					break;
			}

			m_class_capability.push_back(capability);
		}
		*/
	}
	void player::set_shared_ptr(std::shared_ptr<mods::player>& self_ptr) {
		std::cerr << "[deprecated] set_shared_ptr\n";
		return;
		/*
		m_self_ptr = self_ptr;
		cd()->player_ptr = self_ptr;
		*/
	}
	void player::m_set_time(){
		//m_time = std::chrono::system_clock::now();
		m_time = ::time(nullptr);
	}

	player::~player(){
		if(IN_ROOM(cd()) != NOWHERE){
			char_from_room(cd());
		}
	}
	player::player(player_type_enum_t type){
		set_god_mode(false);
		set_bui_mode(false);
		set_imp_mode(false);
		m_set_time();
		switch(type){
			case DRONE:
				player();
				break;
			default:
				player();
				break;
		}
		set_type(type);
	}
	void player::set_type(player_type_enum_t type){
		/**
		 * FIXME: take appropriate action on a per type basis. 
		 * TODO: If it's a drone, then the appropriate drone flags need to be set
		 * TODO: if it's a mob, then it needs to have IS_NPC return true.. so, set that flag
		 * TODO: if it's a player, set the IS_NPC flag to false. Also, figure out how to handle descriptor_data
		 */
		m_type = type;
		switch(type){
			case player_type_enum_t::PLAYER:
			case player_type_enum_t::MOB:
			case player_type_enum_t::DRONE:
				if(m_desc){
					m_desc->set_queue_behaviour(mods::descriptor_data::queue_behaviour_enum_t::NORMAL);
				}
				break;
			case player_type_enum_t::PLAYER_MUTED_DESCRIPTOR:
			case player_type_enum_t::MOB_MUTED_DESCRIPTOR:
			case player_type_enum_t::DRONE_MUTED_DESCRIPTOR:
				if(m_desc){
					m_desc->set_queue_behaviour(mods::descriptor_data::queue_behaviour_enum_t::IGNORE_ALL);
				}
				break;
		}
	}
	player::player(){
		set_god_mode(false);
		set_bui_mode(false);
		set_imp_mode(false);
		m_set_time();
		descriptor_list.emplace_back();
		auto descriptor = descriptor_list.end()-1;
		m_shared_ptr = std::make_shared<char_data>();
		m_char_data = m_shared_ptr.get();
		descriptor->character  = this->cd();
		set_desc(descriptor);
		set_char_on_descriptor(descriptor);
		/** I don't like this class call FIXME */
		//set_class_capability({mods::classes::types(0)});
		m_player_specials = std::make_shared<player_special_data>();
		m_char_data->player_specials = m_player_specials;
		/** Need a better uuid generator than this */
		/** FIXME: this is not how uuid's should be generated */
		m_char_data->uuid = mods::globals::player_list.size();
		m_char_data->next = character_list;
		character_list = m_char_data;
		/** FIXME: need to set the m_char_data->desc */
		m_page = 0;
		m_current_page = 0;
		m_do_paging = false;
		m_current_page_fragment = "";
		m_capture_output = false;
		m_executing_js = false;
		set_type(player_type_enum_t::PLAYER);
	}
	player::player(mods::player* ptr) {
		/**TODO: should we set the queue_behaviour flags on the descriptor data items on *this? */
		m_shared_ptr = std::make_shared<char_data>(ptr->cd());
		m_char_data = ptr->cd();
		m_page = 0;
		m_current_page = 0;
		m_do_paging = false;
		m_current_page_fragment = "";
		m_capture_output = false;
		m_executing_js = false;
		m_time = ptr->time();
		set_god_mode(ptr->god_mode());
		set_bui_mode(ptr->builder_mode());
		set_imp_mode(ptr->implementor_mode());
		/** TODO: investigate this function. I have a feeling that m_desc needs to be updated here */
	}
	void player::capture_output(bool capture_status) {
		m_capture_output = capture_status;
	}

	std::string_view player::get_captured_output() {
		return m_captured_output;
	}

	void player::clear_captured_output() {
		m_captured_output = "";
	}

	using mask_t = mods::weapon::mask_type;
	player::player(char_data* ch) : m_executing_js(false), m_char_data(ch),m_do_paging(false),
		m_page(0),m_current_page(0),m_current_page_fragment("") {
			m_set_time();
	};
	bool player::can_snipe(char_data *target) {
		/** TODO: figure out why this function was created. */
		return true;
	}
	std::shared_ptr<mods::classes::base>& player::get_class(class_type c_type) {
		for(auto& ptr : m_class_info) {
			if(ptr->kind() == c_type) {
				return ptr;
			}
		}

		return *m_class_info.begin();
	}
	void player::page(int pg) {
		assert(pg >= 0);
		if(m_pages.size() == 0 || static_cast<std::size_t>(pg * mods::player::PAGE_SIZE) >= m_pages.size()) {
			return;
		}

		std::string acc = "";
		unsigned i = pg * mods::player::PAGE_SIZE;

		for(; i < m_pages.size() && i < static_cast<std::size_t>((pg + 1) * mods::player::PAGE_SIZE); i++) {
			acc += m_pages[i];
		}

		if(i < m_pages.size()) {
			acc += std::string("--[ page number ") + std::to_string(pg +1)  + "/";
			acc += std::to_string((m_pages.size() / mods::player::PAGE_SIZE) + 1) + \
			       ":: press (q) to quit or page number ]--";
		} else {
			pager_clear();
			pager_end();
			m_current_page = 0;
		}

		if(acc.length()) {
			stc(acc);
			m_current_page = pg;
		}
	}
	void player::pager_next_page() {
		if((m_current_page +1) * mods::player::PAGE_SIZE >= m_pages.size()) {
			pager_clear();
			pager_end();
			return;
		}

		page(m_current_page +1);
	}
	bool player::has_weapon_capability(int type) {
		auto w = weapon();

		if(!w) {
			return false;
		}

		switch(type) {
			case mods::weapon::mask::snipe:
				return !!(std::string(w->name).find("snipe") != std::string::npos);
				break;

			default:
				return false;
		}

		return false;
	}
	bool player::has_inventory_capability(int type) {
		return true; //FIXME:
	}
	void player::weapon_cooldown_start(unsigned long duration,weapon_set set) {
		m_weapon_cooldown[set] = static_cast<unsigned long>(::time(NULL)) + duration;
	}
	bool player::weapon_cooldown_expired(weapon_set set) {
		auto cts = static_cast<unsigned long>(::time(NULL));
		return cts >= m_weapon_cooldown[set];
	}
	bool player::carrying_ammo_of_type(const weapon_type_t& type) {
		for(auto item = m_char_data->carrying; item->next; item = item->next) {
			if(std::string(item->name).find("[ammo]") != std::string::npos
			        &&
			        m_char_data == item->carried_by
			        &&
			        type == item->weapon_type
			  ) {
				return true;
			}
		}

		return false;
	}
	std::string player::js_object() {
		std::string obj = std::string("{ 'name': '") + std::string(cd()->player.name) + std::string("','uuid':");
		return obj + std::to_string(cd()->uuid) + "}";
	}
	bool player::is_weapon_loaded() {
		auto w = weapon();

		if(!w) {
			return false;
		}

		return true;
	}
	bool player::has_ammo() {
		auto w = weapon();

		if(w->ammo > 0) {
			return true;
		} else {
			return false;
		}
	}
	bool player::has_thermite() {
		if(!m_char_data->carrying) {
			return false;
		}

		for(auto item = m_char_data->carrying; item->next; item = item->next) {
			if(m_char_data != item->carried_by) {
				return false;
			}

			if(item->weapon_type == std::hash<std::string> {}("thermite")) {
				return true;
			}
		}

		return false;
	}
	bool player::has_class_capability(class_type type) {
		return !(std::find(m_class_capability.begin(),m_class_capability.end(),type) == m_class_capability.end());
	}

	bool player::has_equipment_tag(const std::string& tag) {
		if(!m_char_data->carrying) {
			return false;
		}

		/* TODO: find a better way to do this. this loops through all objects in the game and checks if the person carrying it is the current player */
		for(auto item = m_char_data->carrying; item->next; item = item->next) {
			if(std::string(item->name).find(std::string("[") + std::string(tag) + "]") != std::string::npos
			        &&
			        m_char_data == item->carried_by
			  ) {
				return true;
			}
		}

		return false;
	}
	void player::ammo_adjustment(int increment) {
		weapon()->ammo += increment;
	}
	player& player::pager_start() {
		m_do_paging = true;
		m_pages.clear();
		m_page = 0;
		return *this;
	}
	player& player::pager_end() {
		m_do_paging = false;

		if(m_current_page_fragment.length()) {
			if(m_pages.begin() == m_pages.end()) {
				m_pages.push_back(m_current_page_fragment);
			} else {
				auto it = m_pages.end() -1;
				*it += m_current_page_fragment;
			}
		}

		m_current_page_fragment = "";
		return *this;
	}
	void player::executing_js(bool true_false) {
		m_executing_js = true_false;
	}
	bool player::is_executing_js() const {
		return m_executing_js;
	}
	void player::pager_clear() {
		m_pages.clear();
	}
	obj_data* player::get_first_ammo_of_type(const weapon_type_t& type) const {
		for(auto item = m_char_data->carrying; item->next; item = item->next) {
			if(item->weapon_type == type
			        &&
			        std::string(item->name).find("[ammo]")
			        &&
			        m_char_data == item->carried_by
			  ) {
				return item;
			}
		}

		return nullptr;
	}
	/* returns:
	 * 	the `ammo` trait of the obj_data struct after the increment has been applied.
	 * if 0 (zero) is returned, then nothing was subtraced/added from the user's
	 * ammo supply. This means the user doesn't have that ammo type
	 */
	int player::ammo_type_adjustment(int increment,const weapon_type_t& type) {
		if(!m_char_data->carrying) {
			return 0;
		}

		for(auto item = m_char_data->carrying; item->next; item = item->next) {
			if(item->weapon_type == type
			        &&
			        std::string(item->name).find("[ammo]") != std::string::npos
			        &&
			        m_char_data == item->carried_by
			  ) {
				item->ammo += increment;
				return item->ammo;
			}
		}
		return 0;
	}
	void player::stc_room(const room_rnum& rnum) {
		if(rnum < 0 || std::size_t(rnum) >= world.size()){
			return;
		}
		if(world[rnum].name) {
			std::string title = static_cast<const char*>(world[rnum].name);
			stc_color_evaluation(std::string("{grn}") + title + "{/grn}",this);
		}
	}
	void player::stc(const char* m) {
		if(m) {
			if(m_capture_output) {
				m_captured_output += m;
			}

			send_to_char(m_char_data,m);
		}
	}
	void player::stc(const std::string m) {
		if(m.length()) {
			if(m_capture_output) {
				m_captured_output += m;
			}

			send_to_char(m_char_data,m.c_str());
		}
	}
	void player::stc(int m) {
		if(m_capture_output) {
			m_captured_output += std::to_string(m);
		}

		send_to_char(m_char_data,std::to_string(m).c_str());
	}
	void player::stc_room_desc(const room_rnum& rnum) {
		if(rnum < 0 || std::size_t(rnum) >= world.size()){
			return;
		}
		if(world[rnum].description) {
			std::string colored = just_color_evaluation(static_cast<const char*>(world[rnum].description));
			/* TODO: get status of outside world, if EMP, then replace phrase with emp phrase */
			auto player = this;
			auto value = PLAYER_GET("screen_width");

			if(value.length()) {
				auto i_value = mods::util::stoi(value);

				if(i_value.has_value()) {
					stc(word_wrap(colored,i_value.value()) + "\r\n");
				}

				return;
			} else {
				stc(word_wrap(colored,80) + "\r\n");
			}
		}
	}
	obj_data* player::weapon() {
		return GET_EQ(m_char_data, WEAR_WIELD);
	}
	obj_data* player::get_ammo(const weapon_type_t& type) {
		for(auto item = m_char_data->carrying; item->next; item = item->next) {
			if(std::string(item->name).find("[ammo]") != std::string::npos
			        &&
			        m_char_data == item->carried_by
			        &&
			        type == item->weapon_type
			  ) {
				return item;
			}
		}

		return nullptr;
	}
	void player::exits() {
		do_auto_exits(m_char_data);
	}
	void player::set_char_on_descriptor(std::deque<descriptor_data>::iterator it){
		it->character = this->cd();
		it->character->desc = m_desc;
		it->character->has_desc = true;
	}
	void player::init(){
		set_god_mode(false);
		set_imp_mode(false);
		set_bui_mode(false);
		if(m_desc){
			m_desc.reset();
		}
		m_name.clear();
		m_class_capability = {};
		m_executing_js = false;
		if(m_char_data){
			m_char_data = nullptr;
		}
		std::fill(m_weapon_cooldown.begin(),m_weapon_cooldown.end(),0);
		m_weapon_set = {};
		m_do_paging = m_capture_output = false;
		m_captured_output.clear();
		m_page = 0;
		m_current_page = 0;
		m_current_page_fragment.clear();
		m_pages.clear();
		m_class_info = {};
		if(m_shared_ptr){
			m_shared_ptr.reset();
		}
	}
			void player::set_cd(char_data* ch) {
				m_char_data = ch;
			}
			player::time_type_t player::time() const {
				return m_time;
			}
			void player::set_desc(std::deque<descriptor_data>::iterator it){ 
				m_desc = std::make_shared<mods::descriptor_data>(*it);
				set_type(m_type);	//This will indirectly call the set_queue_behaviour function on our new descriptor_data object
			}
			void player::set_desc(std::shared_ptr<descriptor_data> it){ 
				m_desc = it;
				set_type(m_type);	//This will indirectly call the set_queue_behaviour function on our new descriptor_data object
			}
			descriptor_data& player::desc(){ 
				if(m_desc){
					return *m_desc;
				}else{
					std::cerr << "Warning: player::desc() called but m_desc is null. Creating temporary descriptor_data shared_ptr\n";
					set_desc(std::make_shared<mods::descriptor_data>());
					return *m_desc;
				}
			}
			void player::affect(int64_t flag){
				SET_BIT(AFF_FLAGS(cd()), flag);
			}

			void player::remove_affect(int64_t flag){
				REMOVE_BIT(AFF_FLAGS(cd()), flag);
			}
			void player::clear_all_affected(){
				for(auto &pair : this->get_affected()){
					this->remove_affect(pair.first);
				}
			}
			std::map<aff,bool> player::get_affected(){
m_affected[aff::BLIND] = IS_SET(AFF_FLAGS(cd()),AFF_BLIND);
m_affected[aff::INVISIBLE] = IS_SET(AFF_FLAGS(cd()),AFF_INVISIBLE);
m_affected[aff::DETECT_ALIGN] = IS_SET(AFF_FLAGS(cd()),AFF_DETECT_ALIGN);
m_affected[aff::DETECT_INVIS] = IS_SET(AFF_FLAGS(cd()),AFF_DETECT_INVIS);
m_affected[aff::DETECT_MAGIC] = IS_SET(AFF_FLAGS(cd()),AFF_DETECT_MAGIC);
m_affected[aff::SENSE_LIFE] = IS_SET(AFF_FLAGS(cd()),AFF_SENSE_LIFE);
m_affected[aff::WATERWALK] = IS_SET(AFF_FLAGS(cd()),AFF_WATERWALK);
m_affected[aff::SANCTUARY] = IS_SET(AFF_FLAGS(cd()),AFF_SANCTUARY);
m_affected[aff::GROUP] = IS_SET(AFF_FLAGS(cd()),AFF_GROUP);
m_affected[aff::CURSE] = IS_SET(AFF_FLAGS(cd()),AFF_CURSE);
m_affected[aff::INFRAVISION] = IS_SET(AFF_FLAGS(cd()),AFF_INFRAVISION);
m_affected[aff::POISON] = IS_SET(AFF_FLAGS(cd()),AFF_POISON);
m_affected[aff::PROTECT_EVIL] = IS_SET(AFF_FLAGS(cd()),AFF_PROTECT_EVIL);
m_affected[aff::PROTECT_GOOD] = IS_SET(AFF_FLAGS(cd()),AFF_PROTECT_GOOD);
m_affected[aff::SLEEP] = IS_SET(AFF_FLAGS(cd()),AFF_SLEEP);
m_affected[aff::NOTRACK] = IS_SET(AFF_FLAGS(cd()),AFF_NOTRACK);
m_affected[aff::UNUSED16] = IS_SET(AFF_FLAGS(cd()),AFF_UNUSED16);
m_affected[aff::UNUSED17] = IS_SET(AFF_FLAGS(cd()),AFF_UNUSED17);
m_affected[aff::SNEAK] = IS_SET(AFF_FLAGS(cd()),AFF_SNEAK);
m_affected[aff::HIDE] = IS_SET(AFF_FLAGS(cd()),AFF_HIDE);
m_affected[aff::UNUSED20] = IS_SET(AFF_FLAGS(cd()),AFF_UNUSED20);
m_affected[aff::CHARM] = IS_SET(AFF_FLAGS(cd()),AFF_CHARM);
return m_affected;
			}
			void player::affect_plr(int64_t flag){
				SET_BIT(PLR_FLAGS(cd()), flag);
			}

			void player::remove_affect_plr(int64_t flag){
				REMOVE_BIT(PLR_FLAGS(cd()), flag);
			}
			void player::clear_all_affected_plr(){
				for(auto &pair : this->get_affected_plr()){
					this->remove_affect_plr(pair.first);
				}
			}
			std::map<int64_t,bool> player::get_affected_plr(){
m_affected_plr[plr::KILLER] = IS_SET(PLR_FLAGS(cd()),PLR_KILLER);
m_affected_plr[plr::THIEF] = IS_SET(PLR_FLAGS(cd()),PLR_THIEF);
m_affected_plr[plr::FROZEN] = IS_SET(PLR_FLAGS(cd()),PLR_FROZEN);
m_affected_plr[plr::DONTSET] = IS_SET(PLR_FLAGS(cd()),PLR_DONTSET);
m_affected_plr[plr::WRITING] = IS_SET(PLR_FLAGS(cd()),PLR_WRITING);
m_affected_plr[plr::MAILING] = IS_SET(PLR_FLAGS(cd()),PLR_MAILING);
m_affected_plr[plr::CRASH] = IS_SET(PLR_FLAGS(cd()),PLR_CRASH);
m_affected_plr[plr::SITEOK] = IS_SET(PLR_FLAGS(cd()),PLR_SITEOK);
m_affected_plr[plr::NOSHOUT] = IS_SET(PLR_FLAGS(cd()),PLR_NOSHOUT);
m_affected_plr[plr::NOTITLE] = IS_SET(PLR_FLAGS(cd()),PLR_NOTITLE);
m_affected_plr[plr::DELETED] = IS_SET(PLR_FLAGS(cd()),PLR_DELETED);
m_affected_plr[plr::LOADROOM] = IS_SET(PLR_FLAGS(cd()),PLR_LOADROOM);
m_affected_plr[plr::NOWIZLIST] = IS_SET(PLR_FLAGS(cd()),PLR_NOWIZLIST);
m_affected_plr[plr::NODELETE] = IS_SET(PLR_FLAGS(cd()),PLR_NODELETE);
m_affected_plr[plr::INVSTART] = IS_SET(PLR_FLAGS(cd()),PLR_INVSTART);
m_affected_plr[plr::CRYO] = IS_SET(PLR_FLAGS(cd()),PLR_CRYO);
m_affected_plr[plr::NOTDEADYET] = IS_SET(PLR_FLAGS(cd()),PLR_NOTDEADYET);
				return m_affected_plr;
			}

			bool player::god_mode() const{
				return m_god_mode;
			}
			bool player::implementor_mode() const{
				return m_imp_mode;
			}
			bool player::builder_mode() const{
				return m_bui_mode;
			}
			void player::set_god_mode(bool b){
				m_god_mode = b;
			}
			void player::set_imp_mode(bool b){
				m_imp_mode = b;
			}
			void player::set_bui_mode(bool b){
				m_bui_mode = b;
			}
			void player::done() {
				this->stc("It has been done.\n");
			}
			bool player::has_builder_data(){
				if(cd()->builder_data){
					return true;
				}
				return false;
			}
			bool player::room_pave_mode(){
				if(this->has_builder_data()){
					return cd()->builder_data->room_pave_mode;
				}
				return false;
			}
			bool player::zone_pave_mode(){
				if(this->has_builder_data()){
					return cd()->builder_data->zone_pave_mode;
				}
				return false;
			}
			void player::set_db_id(aligned_int_t id){
				m_db_id = id;
			}
			aligned_int_t player::get_db_id() const {
				return m_db_id;
			}

			void player::set_password(std::string pw){
				m_password = pw;
			}
			std::string player::get_password(){
				return m_password;
			}
			void player::set_bad_password_count(int i){
				if(m_desc){
					m_desc->bad_pws = i;
				}else{
					std::cerr << "warning: ncrement_bad_password_count called but m_desc is null\n";
				}
			}
			void player::increment_bad_password_count(){
				if(m_desc){
					++m_desc->bad_pws;
				}else{
					std::cerr << "warning: ncrement_bad_password_count called but m_desc is null\n";
				}
			}
			int player::get_bad_password_count(){
				if(m_desc){
					return m_desc->bad_pws;
				}else{
					std::cerr << "warning: get_bad_password_count called but m_desc is null\n";
					return 0;
				}
			}
			void player::set_state(int state){
				if(m_desc){
					m_desc->connected = state;
				}else{
					std::cerr << "warning: set_state called but m_desc is null\n";
				}
			}

			int player::state(){
				if(m_desc){
					return m_desc->connected;
				}else{
					std::cerr << "warning: state called but m_desc is null\n";
					if(cd()->desc){
						std::cout << "info: char_data ptr has a desc, so we're returning that\n";
						return cd()->desc->connected;
					}
					return 0;
				}
			}
			void player::set_host(std::string host){
				if(m_desc){
					m_desc->host = host;
				}else{
					std::cerr << "warning: player::set_host called but m_desc is null\n";
				}
			}
			std::string player::host() const {
				if(m_desc){
					return m_desc->host.c_str();
				}else{
					std::cerr << "warning: player::host called but m_desc is null\n";
				}
				return "unknown";
			}
			void player::deactivate_account(){
				
				/** TODO: what do we need to do to deactivate the account? 
				 * for now, I think just setting an active flag for the player would be enough
				 */
				std::cout << "stub: player::deactivate_account\n";
			}
			void player::set_time_birth(time_t b){
				this->cd()->player.time.birth = b;
			}
			time_t player::get_time_birth() const {
				return this->cd()->player.time.birth;
			}

			void player::set_time_logon(time_t t){
				this->cd()->player.time.logon = t;
			}
			time_t player::get_time_login() const {
				return this->cd()->player.time.logon;
			}
			void player::set_time_played(int t){
				this->cd()->player.time.played = t;
			}
			int player::get_time_played() const{
				return this->cd()->player.time.played;
			}

			/** PLR_* Affects */
			void player::set_affect_by_serialized(std::string data){
				std::vector<aligned_int_t> buffer;
				std::copy(data.begin(),data.end(),buffer.begin());
				this->clear_all_affected();
				for(unsigned i =0; i < buffer.size(); ++i){
					for(auto &[flag,legacy_flag] : mods::all_aff_flags){
						if(std::find(buffer.begin(),buffer.end(),flag) != buffer.end()){
							this->affect(legacy_flag);
						}else{
							this->remove_affect(legacy_flag);
						}
					}
				}
			}
			std::string player::generic_serialize(std::map<uint64_t,bool>* vals){
				std::vector<uint64_t> buffer;
				for(auto &[flag,status] : *vals){
					if(status){
						buffer.emplace_back(flag);
					}
				}
				if(buffer.size() == 0){
					return "";
				}
				std::string serialized;
				serialized.resize(buffer.size() * sizeof(uint64_t));
				std::fill(serialized.begin(),serialize.end(),0);
				std::copy(buffer.begin(),buffer.end(),serialized.begin());
				return serialized;
			}

			/** PLR_* Affects */
			std::string player::serialize_affect(){
				auto f= this->get_affected();
				return this->generic_serialize(static_cast<std::map<uint64_t,bool>*>(&f));
			}
			std::string player::serialize_affect_plr(){
				auto f= this->get_affected_plr();
				return this->generic_serialize(static_cast<std::map<uint64_t,bool>*>(&f));
			}

			/** PLR_* Affects */
			void player::set_affect_plr_by_serialized(std::string data){
				std::vector<aligned_int_t> buffer;
				std::copy(data.begin(),data.end(),buffer.begin());
				this->clear_all_affected_plr();
				for(unsigned i =0; i < buffer.size(); ++i){
					for(auto &[flag,legacy_flag] : mods::all_plr_flags){
						if(std::find(buffer.begin(),buffer.end(),flag) != buffer.end()){
							this->affect_plr(legacy_flag);
						}else{
							this->remove_affect_plr(legacy_flag);
						}
					}
				}
			}

			bool player::has_affect(uint64_t flag){
				return get_affected()[flag];
			}
			bool player::has_affect_plr(uint64_t flag) {
				return get_affected_plr()[flag];
			}
			void player::set_socket(socket_t d){
				this->desc().descriptor = d;
				this->cd()->desc->descriptor = d;
			}
			socket_t player::socket(){
				return this->desc().descriptor;
			}
			/* Javascript functions */
};

#endif
