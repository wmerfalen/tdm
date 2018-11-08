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

extern void do_auto_exits(struct char_data *ch);
extern mods::player::descriptor_data_t descriptor_list;
namespace mods {
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
		m_set_time();
		switch(type){
			case DRONE:
				player();
				break;
			default:
				player();
				break;
		}
	}
	player::player(){
		m_set_time();
		m_shared_ptr = std::make_shared<char_data>();
		m_char_data = m_shared_ptr.get();
		/** I don't like this class call FIXME */
		set_class_capability({mods::classes::types(0)});
		/** Need a better uuid generator than this */
		m_char_data->uuid = mods::globals::player_list.size();
		/** I H_A_T_E that we are still tied to the linked list FIXME */
		m_char_data->next = character_list;
		character_list = m_char_data;
		/** FIXME: need to set the m_char_data->desc */
		m_page = 0;
		m_current_page = 0;
		m_do_paging = false;
		m_current_page_fragment = "";
		m_capture_output = false;
		m_executing_js = false;
	}
	player::player(mods::player* ptr) {
		m_shared_ptr = std::make_shared<char_data>(ptr->cd());
		m_char_data = ptr->cd();
		m_page = 0;
		m_current_page = 0;
		m_do_paging = false;
		m_current_page_fragment = "";
		m_capture_output = false;
		m_executing_js = false;
		m_time = ptr->time();
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
};

#endif
