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
#include "util.hpp"
#include "scan.hpp"
#include "date-time.hpp"
#include <chrono>
/**
 * TODO: All these stc* functions need to be altered to accomodate
 * the new player_type_enum_t values. If output is to be muted, then
 * we can stop the output from this class and avoid the overhead of
 * calling whatever functions the send_to_char function calls to queue
 * output.
 */

extern void do_auto_exits(struct char_data *ch);
extern mods::player::descriptor_data_t descriptor_list;
extern mods::scan::find_results_t mods::scan::los_find(chptr hunter,chptr hunted);
enum histfile_type_t {
	HISTFILE_FILE = 1, HISTFILE_LMDB = 2, HISTFILE_DUAL = 3
};
static constexpr histfile_type_t HISTFILE_STRATEGY = histfile_type_t::HISTFILE_FILE;
namespace mods {
	using mask_t = mods::weapon::mask_type;
	using lmdb_db = db_handle;


	void player::set_shared_ptr(player_ptr_t& self_ptr) {
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
		this->set_overhead_map_width(16);
		this->set_overhead_map_height(10);
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

	player::player(char_data* ch) : m_char_data(ch), m_executing_js(false), m_do_paging(false),
	m_page(0),m_current_page(0),m_current_page_fragment("") {
		m_set_time();
	};
	bool player::can_snipe(char_data *target) {
		return mods::scan::los_find(
				std::make_shared<mods::player>(cd()),
				std::make_shared<mods::player>(target)
				).found;
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
	obj_data* player::legacy_equipment(int pos) {
		return m_char_data->equipment[pos];
	}
	std::shared_ptr<obj_data> player::equipment(int pos) {
		return std::make_shared<obj_data>(*m_char_data->equipment[pos]);
	}
	void player::equip(obj_data* obj,int pos) {
		if(pos == WEAR_WIELD){
			m_weapon_flags = obj->obj_flags.weapon_flags;
		}
		m_char_data->equipment[pos] = obj;
	}
	void player::unequip(int pos) {
		if(pos == WEAR_WIELD){
			/** FIXME: this needs to negate the bit */
			m_weapon_flags = m_char_data->equipment[pos]->obj_flags.weapon_flags;
		}
		m_char_data->equipment[pos] = nullptr;
	}
	bool player::has_weapon_capability(uint8_t type) {
		auto w = weapon();

		if(w &&  w->type == type){
			return true;
		}
		return false;
	}
	bool player::has_inventory_capability(int type) {
		return true; //FIXME:
	}
	void player::weapon_cooldown_start(uint16_t duration,weapon_set set) {
		m_weapon_cooldown[set] = static_cast<unsigned long>(::time(NULL)) + duration;
	}
	bool player::weapon_cooldown_expired(weapon_set set) {
		auto cts = static_cast<unsigned long>(::time(NULL));
		return cts >= m_weapon_cooldown[set];
	}
	bool player::carrying_ammo_of_type(const weapon_type_t& type) {
		if(m_char_data->carrying == 0){
			return false;
		}
		for(auto item = m_char_data->carrying; item->next; item = item->next) {
			if(item->weapon() && item->weapon()->is_ammo && type == item->weapon()->type){
				return true;
			}
		}

		return false;
	}
	/** TODO: do this */
	obj_data* player::carry(obj_data* obj){
		return new obj_data;
	}
	obj_data* player::carrying(){
		return cd()->carrying;
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
		/** TODO: FIXME */
		write_to_char(m_char_data, "[stub] FIXME",1,1);
		return false;
	}
	std::shared_ptr<mods::classes::sniper> 	player::cl_sniper(){
		return m_class_sniper;
	}
	std::shared_ptr<mods::classes::medic> 	player::cl_medic(){
		return m_class_medic;
	}
	void player::set_class(player_class_t pclass){
		m_class = pclass;
		switch(pclass){
			case CLASS_SNIPER:
				std::cerr << "[mods::player::set_class] player is sniper\n";
				m_class_sniper = std::make_shared<mods::classes::sniper>();
				cl_sniper()->rifle()->bound_to = uuid();
				break;
			case CLASS_MEDIC:
				std::cerr << "[mods::player::set_class] player is medic\n";
				m_class_medic = std::make_shared<mods::classes::medic>();
				break;
			default:
				break;
		}
	}

	std::string& player::name(){ return m_name; }
	std::string player::ucname(){ return m_ucname; }
	void player::set_name(std::string n){
		cd()->player.name.assign(n);
		m_name = n;
		m_ucname = m_name;
		if(m_name[0] > 96){
			m_ucname[0] = (char)(m_name[0] - 32);
		}
		for(std::size_t i=1; i < m_name.length();i++){
			if(m_name[i] > 96){
				m_ucname[i] = m_name[i];
				continue;
			}
			if(m_name[i] > 64 && m_name[i] < 91){
				m_ucname[i] = (char)(m_name[i] + 32);
				continue;
			}
			m_ucname[i] = m_name[i];
		}
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
			if(item->weapon() && item->weapon()->is_ammo && 
					item->weapon()->type == type && m_char_data == item->carried_by) {
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
			if(item->weapon() && item->weapon()->is_ammo && 
					item->weapon()->type == type && m_char_data == item->carried_by) {
				item->weapon()->ammo += increment;
				return item->weapon()->ammo;
			}
		}
		return 0;
	}
	void player::psendln(std::string_view str) {
		write_to_char(m_char_data, str,1,1);
	}
	void player::psendln(mods::string& str) {
		write_to_char(m_char_data, str.view(),1,1);
	}
	void player::sendln(mods::string& str) {
		write_to_char(m_char_data, str.view(), 1,0);
	}
	void player::sendln(std::string_view str) {
		write_to_char(m_char_data, str, 1,0);
	}
	void player::stc_room(const room_rnum& rnum) {
		if(rnum < 0 || std::size_t(rnum) >= world.size()){
			return;
		}
		if(world[rnum].name) {
			/** note: using 1 for the plain parameter since colr eval is done on room load from postgres */
			write_to_char(m_char_data, world[rnum].name.view(),1,1);
		}
		if(builder_mode()){
			write_to_char(m_char_data,(std::string("[room_id:") + std::to_string(room()) + "|number:" + 
						std::to_string(world[room()].number) + "|zone:" + 
						std::to_string(world[room()].zone)
					),1,1
			);
		}
	}
	void player::stc(const char* m) {
		/* FIXME: this does not scale */
		if(m_capture_output) {
			m_captured_output += m;
		}
		write_to_char(m_char_data,m,0,0);
	}
	void player::stc(const mods::string& m){
		write_to_char(m_char_data,m.view(),0,0);
	}
	void player::stc(std::string_view sview) {
		write_to_char(m_char_data,sview,0,0);
	}
	void player::stc(const std::string m) {
		/* FIXME: this does not scale */
		if(m_capture_output) {
			m_captured_output += m;
		}
		write_to_char(m_char_data,m,0,0);
	}
	void player::stc(int m) {
		if(m_capture_output) {
			m_captured_output += std::to_string(m);
		}

		/** note, using 1 for plain parameter */
		write_to_char(m_char_data,std::to_string(m).c_str(),0,1);
	}
	void player::stc_room_desc(const room_rnum& rnum) {
		if(rnum < 0 || std::size_t(rnum) >= world.size()){
			return;
		}
		raw_send(world[rnum].description);
		if(((get_prefs()) & PRF_OVERHEAD_MAP)){
			stc(mods::overhead_map::generate<mods::player*>(this,room()));
		}
	}
	void player::raw_send(const mods::string& str){
		 write_to_descriptor(m_desc->descriptor,str.c_str());
	}
	mods::string player::weapon_name(){
		return GET_EQ(m_char_data, WEAR_WIELD)->name;
	}
	weapon_data_t* player::weapon() {
		return GET_EQ(m_char_data, WEAR_WIELD)->weapon();
	}
	obj_data* player::get_ammo(const weapon_type_t& type) {
		for(auto item = m_char_data->carrying; item->next; item = item->next) {
			if(item->weapon() && item->weapon()->is_ammo &&
					m_char_data == item->carried_by &&
					type == item->weapon()->type
				) {
				return item;
			}
		}

		return nullptr;
	}
	void player::set_char_on_descriptor(std::deque<descriptor_data>::iterator it){
		it->character = this->cd();
		it->character->desc = m_desc;
		it->character->has_desc = true;
	}
	void player::init(){
		m_histfile_on = false;
		m_histfile_fp = nullptr;
		m_weapon_type = 0;
		m_weapon_flags = 0;
		m_authenticated = false;
		set_god_mode(false);
		set_imp_mode(false);
		set_bui_mode(false);
		if(m_desc){
			m_desc.reset();
		}
		m_name.clear();
		m_class = CLASS_UNDEFINED;
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
		m_lense_type = NORMAL_SIGHT;
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


	/**
	 * =========================
	 * =========================
	 * =========================
	 * Affect flag manipulation
	 * =========================
	 * =========================
	 * =========================
	 *
	 */
	void player::affect(uint64_t flag){
		SET_BIT(AFF_FLAGS(cd()), flag);
		set_flag(mods::flags::chunk_type_t::LEGACY_AFF,flag);
	}
	//void player::affect(mods::flags::aff flag){
	//	SET_BIT(AFF_FLAGS(cd()), mods::util::aff2legacy(flag));
	//	set_flag(mods::flags::chunk_type_t::LEGACY_AFF,flag);
	//}
	void player::remove_affect(uint64_t flag){
		REMOVE_BIT(AFF_FLAGS(cd()), flag);
		remove_flag(mods::flags::chunk_type_t::LEGACY_AFF,flag);
	}
	//void player::remove_affect(mods::flags::aff flag){
	//	REMOVE_BIT(AFF_FLAGS(cd()), mods::util::aff2legacy(flag));
	//	remove_flag(mods::flags::chunk_type_t::LEGACY_AFF,flag);
	//}
	void player::clear_all_affected(){
		m_flags[mods::flags::chunk_type_t::LEGACY_AFF] = 0;
		AFF_FLAGS(cd()) = 0;
	}
	aligned_int_t player::get_affected(){
		return AFF_FLAGS(cd());
		return get_chunk(mods::flags::chunk_type_t::LEGACY_AFF);
	}
	/**
	 * =============================
	 * =============================
	 * =============================
	 * PLR Affect flag manipulation
	 * =============================
	 * =============================
	 * =============================
	 *
	 */
	void player::affect_plr(uint64_t flag){
		SET_BIT(PLR_FLAGS(cd()), flag);
		set_flag(mods::flags::chunk_type_t::LEGACY_PLR,flag);
	}
	//void player::affect_plr(mods::flags::plr flag){
	//	SET_BIT(PLR_FLAGS(cd()), mods::util::plr2legacy(flag));
	//	set_flag(mods::flags::chunk_type_t::LEGACY_PLR,flag);
	//}
	void player::remove_affect_plr(uint64_t flag){
		REMOVE_BIT(PLR_FLAGS(cd()), flag);
		remove_flag(mods::flags::chunk_type_t::LEGACY_PLR,flag);
	}
	//void player::remove_affect_plr(mods::flags::plr flag){
	//	REMOVE_BIT(PLR_FLAGS(cd()), mods::util::plr2legacy(flag));
	//	remove_flag(mods::flags::chunk_type_t::LEGACY_PLR,flag);
	//}
	void player::clear_all_affected_plr(){
		PLR_FLAGS(cd()) = 0;
	}
	aligned_int_t player::get_affected_plr(){
		return PLR_FLAGS(cd());
		return get_chunk(mods::flags::chunk_type_t::LEGACY_PLR);
	}

	bool player::has_affect(uint64_t flag){
		return has_flag(mods::flags::chunk_type_t::LEGACY_AFF,flag);
	}
	bool player::has_affect(mods::flags::aff flag){
		return has_flag(mods::flags::chunk_type_t::LEGACY_AFF,flag);
	}
	bool player::has_affect_plr(uint64_t flag) {
		return has_flag(mods::flags::chunk_type_t::LEGACY_PLR,flag);
	}
	bool player::has_affect_plr(mods::flags::plr flag) {
		return has_flag(mods::flags::chunk_type_t::LEGACY_PLR,flag);
	}

	/**
	 * ======================
	 * ======================
	 * ======================
	 * END FLAG MANIPULATIONS
	 * ======================
	 * ======================
	 * ======================
	 *
	 */




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
		if(m_bui_mode){
			if(!builder_data){
				builder_data = std::make_shared<builder_data_t>();
			}
		}
	}
	void player::done() {
		psendln("It has been done.");
	}
	bool player::has_builder_data(){
		if(builder_data){
			return true;
		}
		return false;
	}
	bool player::room_pave_mode(){
		if(this->has_builder_data()){
			return builder_data->room_pave_mode;
		}
		return false;
	}
	bool player::zone_pave_mode(){
		if(this->has_builder_data()){
			return builder_data->zone_pave_mode;
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
			std::cerr << "warning: set_bad_password_count called but m_desc is null\n";
		}
	}
	void player::increment_bad_password_count(){
		if(m_desc){
			++m_desc->bad_pws;
		}else{
			std::cerr << "warning: increment_bad_password_count called but m_desc is null\n";
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

	/**  Affects */
	void player::set_socket(socket_t d){
		this->desc().descriptor = d;
		this->cd()->desc->descriptor = d;
	}
	socket_t player::socket(){
		return this->desc().descriptor;
	}
	/* Javascript functions */
	obj_data_ptr_t player::sniper_rifle(){
		return cl_sniper()->rifle()->obj();
	}


	std::string grab_raw_histfile_seconds(){
		time_t seconds;
		seconds = time(nullptr);
		char buffer[sizeof(seconds)+1];
		bcopy(&seconds,buffer,sizeof(seconds));
		buffer[sizeof(seconds)] = 0;
		std::string data(sizeof(buffer),0);
		std::copy(buffer,buffer+sizeof(buffer), data.begin());
		return data;
	}

	enum histfile_opcode_t {
		HFO_INDEX =1,HFO_START=2,HFO_STOP=3,HFO_LOG=4
	};
	std::string player::hf_pack(uint32_t index,
			uint8_t op,
			unsigned long time_stamp){
		std::size_t amount = sizeof(m_db_id) +
			sizeof(op) +
			sizeof(index) +
			1 +
			sizeof(time_stamp);
		std::string data(amount,0);
		char* db_id = reinterpret_cast<char*>(&m_db_id);
		char* op_id = reinterpret_cast<char*>(&op);
		char* index_id = reinterpret_cast<char*>(&index);
		char* time_id = reinterpret_cast<char*>(&time_stamp);
		std::copy(db_id,db_id + sizeof(m_db_id),data.begin());
		std::copy(op_id,op_id + sizeof(op),data.begin() + sizeof(m_db_id));
		std::copy(index_id,index_id + sizeof(index),data.begin() + sizeof(m_db_id) + sizeof(op));
		data[sizeof(op) + sizeof(m_db_id) + sizeof(index)] = '|';
		std::copy(time_id,time_id + sizeof(time_id),
				data.begin() + sizeof(m_db_id) + sizeof(op) + sizeof(index));
		return data;
	}
	int player::pack_get(
			uint32_t index,
			uint8_t op,
			unsigned long time_stamp,
			std::string& got){
		auto packed_index = hf_pack(index,op,time_stamp);
		auto status = LMDBNGET(packed_index.data(),packed_index.size(),got);
		if(status == EINVAL){
			LMDBRENEW();
			return this->pack_get(index,op,time_stamp,got);
		}
		return status;
	}

	int player::pack_set(
			uint32_t index,
			uint8_t op,
			unsigned long time_stamp,
			void* value,
			std::size_t v_size){
		auto key = this->hf_pack(index,op,time_stamp);

		auto status = LMDBNSET(key.data(),key.size(),value,v_size);
		if(status == EINVAL){
			LMDBRENEW();
			return LMDBNSET(key.data(),key.size(),value,v_size);
		}
		return status;
	}
	void player::start_histfile(){
		auto lib_dir = mods::js::current_working_dir();
		lib_dir += "/../log/";
		lib_dir += name();
		lib_dir += "-" + std::to_string(std::time(nullptr));
		lib_dir += ".log";
		std::cerr << lib_dir << "\n";
		m_histfile_fp = (FILE*)fopen(lib_dir.c_str(),"a+");
		if(!m_histfile_fp){
			std::cerr << "can't open log file '" << lib_dir.c_str() << "'\n";
		}
	}
	void player::write_histfile(std::string_view line){
		if(!m_histfile_fp){ return; }
		std::string prefix = "[";
		prefix += std::to_string(std::time(nullptr));
		prefix += "]->";
		fwrite(prefix.c_str(),sizeof(char),prefix.length(),m_histfile_fp);
		fwrite(line.data(),sizeof(char),line.length(),m_histfile_fp);
		fwrite("\n",sizeof(char),sizeof("\n"),m_histfile_fp);
		fflush(m_histfile_fp);
	}

	void player::stop_histfile(){
		if(!m_histfile_fp){ return; }
		fflush(m_histfile_fp);
		fclose(m_histfile_fp);
	}

	//void player::start_histfile() {
	//	std::string got = "";
	//	auto status = this->pack_get(-1,HFO_INDEX,-1,got);
	//	m_histfile_index = 0;
	//	if(status == lmdb_db::KEY_FETCHED_OKAY){
	//		m_histfile_index = (uint32_t)*((uint32_t*)(got.data()));
	//	}
	//	++m_histfile_index;
	//	this->sendln("pack_get index: " + std::to_string(status) + 
	//			"|m_histfile_index|" + std::to_string(m_histfile_index)
	//	);

	//	char* hf_ptr = reinterpret_cast<char*>(&m_histfile_index);
	//	std::string value(sizeof(m_histfile_index),0);
	//	std::copy(hf_ptr,hf_ptr + sizeof(m_histfile_index),value.begin());
	//	status = this->pack_set(-1,HFO_INDEX,-1,value.data(),value.size());
	//	if(status == 0){
	//		std::cerr << "start_histfile --- pack_set success\n";
	//	}

	//	std::cerr << "start_histfile put 1 (index): " << status << "\n";
	//	value = grab_raw_histfile_seconds();
	//	status = this->pack_set(m_histfile_index,
	//		HFO_START,
	//		-1,
	//		value.data(),
	//		value.size()
	//	);
	//	std::cerr << "start_histfile put 2 (start): " << status << "\n";

	//}

	//void player::write_histfile(std::string_view line) {
	//	auto status = this->pack_set(m_histfile_index,
	//			HFO_LOG,
	//			(unsigned long)std::time(nullptr),
	//			(void*)line.data(),
	//			line.size()
	//	);
	//}
	//void player::stop_histfile() {
	//	auto packed = hf_pack(m_histfile_index,HFO_STOP,-1);
	//	auto value = grab_raw_histfile_seconds();
	//	LMDBNSET(packed.data(),packed.size(),value.data(),value.size());
	//	m_histfile_on = false;
	//}

	player::~player() {
		std::cerr << "[~player] " << m_name.c_str() << "\n";
		if(m_histfile_on){ stop_histfile(); }
		m_weapon_type = 0;
		m_weapon_flags = 0;
		m_authenticated = false;
		set_god_mode(false);
		set_imp_mode(false);
		set_bui_mode(false);
		if(m_desc){
			m_desc.reset();
		}
		m_name.clear();
		m_class = CLASS_UNDEFINED;
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
		m_lense_type = NORMAL_SIGHT;
	}

	mods::affects::dissolver& player::get_affect_dissolver() {
		return m_affects;
	}

};

#endif
