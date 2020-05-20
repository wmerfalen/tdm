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
#include "object-utils.hpp"
/**
 * TODO: All these stc* functions need to be altered to accomodate
 * the new player_type_enum_t values. If output is to be muted, then
 * we can stop the output from this class and avoid the overhead of
 * calling whatever functions the send_to_char function calls to queue
 * output.
 */

extern void do_auto_exits(char_data *ch);
extern mods::player::descriptor_data_t descriptor_list;
extern mods::scan::find_results_t mods::scan::los_find(chptr hunter,chptr hunted);
extern void affect_modify(char_data *ch, 
		byte loc, sbyte mod, bitvector_t bitv, bool add);
extern void affect_total(char_data *ch);
extern void obj_to_room(obj_ptr_t in_object, room_rnum room);
enum histfile_type_t {
	HISTFILE_FILE = 1, HISTFILE_LMDB = 2, HISTFILE_DUAL = 3
};
static constexpr histfile_type_t HISTFILE_STRATEGY = histfile_type_t::HISTFILE_FILE;

namespace mods::orm::inventory::lmdb {
	extern void add_player_wear(uint64_t player_db_id, uint64_t object_db_id, uint8_t obj_type, uint8_t position);
	extern void remove_player_wear(uint64_t player_db_id, uint8_t position);
	extern void add_player_inventory(uint64_t player_db_id, uint64_t object_db_id, uint16_t obj_type);
	extern void remove_player_inventory(uint64_t player_db_id, uint64_t object_db_id);
};

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
		this->init();
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
		m_quitting = 0;
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
		this->init();
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
		m_char_data->uuid = mods::globals::player_uuid();
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
		m_quitting = 0;
		std::fill(m_misc_pref.begin(),m_misc_pref.end(),false);
		m_sync_equipment();
	}
	player::player(mods::player* ptr) {
		this->init();
		m_char_data = ptr->m_char_data;
		/**TODO: should we set the queue_behaviour flags on the descriptor data items on *this? */
		std::fill(m_misc_pref.begin(),m_misc_pref.end(),false);
		m_misc_pref = ptr->m_misc_pref;
		m_shared_ptr = ptr->m_shared_ptr;
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
		m_quitting = 0;
		m_equipment = ptr->m_equipment;
		m_sync_equipment();
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

	player::player(char_data* ch) {
		this->init();
		m_char_data = ch;
		m_executing_js = false;
		m_do_paging = false;
		m_page = 0;
		m_current_page = 0;
		m_current_page_fragment = "";
		m_set_time();
		m_quitting = 0;
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
	obj_ptr_t player::equipment(int pos) {
		if(pos >= NUM_WEARS || pos < 0){
			return nullptr;
		}
		//this->m_sync_equipment();
		return m_equipment[pos];
	}
	void player::equip(obj_ptr_t in_object,int pos) {
		if(pos < NUM_WEARS){
			if(pos == WEAR_WIELD){
				this->m_weapon_flags = in_object->obj_flags.weapon_flags;
			}
			GET_EQ(m_char_data, pos) = in_object.get();
			in_object->worn_by = this->cd();
			in_object->worn_on = pos;
			m_equipment[pos] = in_object;
			mods::orm::inventory::lmdb::add_player_wear(this->db_id(),in_object->db_id(),in_object->type,pos);
			std::cerr << "[stub][player.cpp]-> perform equip calculations\n";
			//perform_equip_calculations(pos,true);
			this->m_sync_equipment();
		}
	}
	void player::equip(uuid_t obj_uuid,int pos){
		std::cerr << "player::equip(uuid): " << obj_uuid << "\n";
		auto obj = optr_by_uuid(obj_uuid);
		if(!obj){
			exit(3);
		}
		this->equip(obj,pos);
	}
	void player::unequip(int pos) {
		if(pos < NUM_WEARS && m_equipment[pos]){
			auto item = m_equipment[pos];
			if(pos == WEAR_WIELD){
				/** FIXME: this needs to negate the bit */
				m_weapon_flags = 0;//m_equipment[pos]->obj_flags.weapon_flags;
			}
			std::cerr << "[stub][player.cpp]-> perform equip calculations\n";
			//perform_equip_calculations(pos,false);
			m_equipment[pos]->worn_by = nullptr;
			m_equipment[pos]->worn_on = -1;
			m_equipment[pos] = nullptr;
			mods::orm::inventory::lmdb::remove_player_wear(this->db_id(),pos);
			this->m_sync_equipment();
		}
	}
	void player::perform_equip_calculations(int pos,bool equip){
		if(m_equipment[pos]->obj_flags.type_flag == ITEM_ARMOR) {
			int factor = 1;
			switch(pos) {
				case WEAR_BODY:
					factor = 3;
					break;			/* 30% */

				case WEAR_HEAD:
					factor = 2;
					break;			/* 20% */

				case WEAR_LEGS:
					factor = 2;
					break;			/* 20% */

				default:
					factor = 1;
					break;			/* all others 10% */
			}
			armor() += factor * m_equipment[pos]->obj_flags.value[0];
		}
		int8_t light = 0;
		//TODO: FIXME: perform updated modern logic of the below code
		if(pos == WEAR_LIGHT && m_equipment[pos]->obj_flags.type_flag == ITEM_LIGHT){
			if(equip){
				if(m_equipment[pos]->obj_flags.value[2]) {	/* if light is ON */
					light = 1;
				}
			}
			if(!equip){
				if(m_equipment[pos]->obj_flags.value[2]) {	/* if light is ON */
					light = -1;
				}
			}
			if(light != 0){
				mods::globals::affect_room_light(room(),light);
			}
		}
		for(unsigned j = 0; j < MAX_OBJ_AFFECT; j++){
			affect_modify(m_char_data, m_equipment[pos]->affected[j].location,
					m_equipment[pos]->affected[j].modifier,
					m_equipment[pos]->obj_flags.bitvector, equip);
		}

		this->m_sync_equipment();
		affect_total(m_char_data);

	}
	bool player::has_weapon_capability(uint8_t type) {
		auto w = rifle();

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
			if(item->obj_flags.is_ammo && type == item->obj_flags.type){
				return true;
			}
		}

		return false;
	}
	/** TODO: do this */
	void player::carry(obj_ptr_t obj){
		if(obj == nullptr){
			m_char_data->m_carrying.clear();
			m_char_data->carrying = nullptr;
			return;
		}
		obj->carried_by = m_char_data;
		m_char_data->m_carrying.emplace_back(obj);
		m_char_data->carrying = obj.get();
		mods::orm::inventory::lmdb::add_player_inventory(this->db_id(), obj->db_id(), obj->type);
	}
	void player::uncarry(obj_ptr_t obj){
		obj_data* temp = 0;
		REMOVE_FROM_LIST(obj.get(), obj->carried_by->carrying,next_content);
		IS_CARRYING_W(obj->carried_by) -= GET_OBJ_WEIGHT(obj);
		IS_CARRYING_N(obj->carried_by)--;
		obj->carried_by = nullptr;
		obj->next_content = nullptr;
		auto it = std::find(m_char_data->m_carrying.begin(),m_char_data->m_carrying.end(),obj);
		if(it != m_char_data->m_carrying.end()){
			m_char_data->m_carrying.erase(it);
			if(m_char_data->carrying == obj.get()){
				if(m_char_data->m_carrying.size()){
					m_char_data->carrying = m_char_data->m_carrying[0].get();
				}else{
					m_char_data->carrying = nullptr;
				}
			}
		}
		mods::orm::inventory::lmdb::remove_player_inventory(this->db_id(), obj->db_id());
	}
	obj_data* player::carrying(){
		return m_char_data->carrying;
	}
	std::string player::js_object() {
		std::string obj = std::string("{ 'name': '") + std::string(cd()->player.name) + std::string("','uuid':");
		return obj + std::to_string(cd()->uuid) + "}";
	}
	bool player::is_weapon_loaded() {
		auto w = rifle();

		if(!w) {
			return false;
		}

		return true;
	}
	bool player::has_ammo() {
		return true; //FIXME
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
				m_class_sniper = std::make_shared<mods::classes::sniper>();
				cl_sniper()->rifle()->bound_to = uuid();
				break;
			case CLASS_MEDIC:
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
		set_ammo(ammo() + increment);
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
			if(item->obj_flags.is_ammo && 
					item->obj_flags.type == type && m_char_data == item->carried_by) {
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
			if(item->obj_flags.is_ammo && 
					item->obj_flags.type == type && m_char_data == item->carried_by) {
				item->obj_flags.ammo += increment;
				return item->obj_flags.ammo;
			}
		}
		return 0;
	}
	void player::psendln(std::string_view str) {
		write_to_char(m_char_data, str,1,1);
		desc().has_prompt = 0;
	}
	void player::psendln(mods::string& str) {
		write_to_char(m_char_data, str.view(),1,1);
		desc().has_prompt = 0;
	}
	void player::sendln(mods::string& str) {
		write_to_char(m_char_data, str.view(), 1,0);
		desc().has_prompt = 0;
	}
	void player::sendln(std::string_view str) {
		write_to_char(m_char_data, str, 1,0);
		desc().has_prompt = 0;
	}
	void player::stc_room(const room_rnum& rnum) {
		std::cerr << __FILE__ << "|" << __LINE__ << " .. checking .. ";
		if(rnum < 0 || std::size_t(rnum) >= world.size()){
			std::cerr << __FILE__ << "|" << __LINE__ << " .. invalid ..\n";
			return;
		}
		raw_send(world[rnum].name);
		//if(world[rnum].name) {
		//	/** note: using 1 for the plain parameter since colr eval is done on room load from postgres */
		//	std::cerr << __FILE__ << "|" << __LINE__ << "-> name:'" << world[rnum].name.c_str() << "'\n";
		//	//write_to_char(m_char_data, world[rnum].name.view(),1,1);
		//}
		if(builder_mode()){
			write_to_char(m_char_data,(std::string("[room_id:") + std::to_string(rnum) + "|number:" + 
						std::to_string(world[rnum].number) + "|zone:" + 
						std::to_string(world[rnum].zone)
						),1,1
					);
		}
		desc().has_prompt = 0;
	}
	void player::stc(const char* m) {
		/* FIXME: this does not scale */
		if(m_capture_output) {
			m_captured_output += m;
		}
		write_to_char(m_char_data,m,0,0);
		desc().has_prompt = 0;
	}
	void player::stc(const mods::string& m){
		write_to_char(m_char_data,m.view(),0,0);
		desc().has_prompt = 0;
	}
	void player::stc(std::string_view sview) {
		write_to_char(m_char_data,sview,0,0);
		desc().has_prompt = 0;
	}
	void player::stc(const std::string m) {
		/* FIXME: this does not scale */
		if(m_capture_output) {
			m_captured_output += m;
		}
		write_to_char(m_char_data,m,0,0);
		desc().has_prompt = 0;
	}
	void player::stc(int m) {
		if(m_capture_output) {
			m_captured_output += std::to_string(m);
		}

		/** note, using 1 for plain parameter */
		write_to_char(m_char_data,std::to_string(m).c_str(),0,1);
		desc().has_prompt = 0;
	}
	void player::stc_room_desc(const room_rnum& rnum) {
		if(rnum < 0 || std::size_t(rnum) >= world.size()){
			std::cerr << "[stc_room_desc] invalid rnum!\n";
			return;
		}
		raw_send(world[rnum].description);
		if(((get_prefs()) & PRF_OVERHEAD_MAP)){
			stc(mods::overhead_map::generate<mods::player*>(this,rnum));
		}
		desc().has_prompt = 0;
	}
	void player::raw_send(const mods::string& str){
		write_to_descriptor(m_desc->descriptor,str.c_str());
		//desc().has_prompt = 0;
	}
	mods::string player::weapon_name(){
		return GET_EQ(m_char_data, WEAR_WIELD)->name;
	}
	rifle_data_t* player::rifle() {
		return GET_EQ(m_char_data, WEAR_WIELD)->rifle();
	}
	obj_data* player::get_ammo(const weapon_type_t& type) {
		for(auto item = m_char_data->carrying; item->next; item = item->next) {
			if(item->obj_flags.is_ammo &&
					m_char_data == item->carried_by &&
					type == item->obj_flags.type
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
		m_blocked_until = 0;
		m_has_block_event = false;
		m_camera = nullptr;
		m_camera_viewing = false;
		m_quitting = false;
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
		for(unsigned i=0;i < NUM_WEARS;i++){ m_equipment[i] = nullptr; }
		std::fill(m_misc_pref.begin(),m_misc_pref.end(),false);
		this->m_sync_equipment();
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

	void player::start_histfile(){
		std::string lib_dir = MENTOC_CURRENT_WORKING_DIR;
		lib_dir += "/../log/";
		lib_dir += name();
		lib_dir += "-" + std::to_string(std::time(nullptr));
		lib_dir += ".log";
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

	player::~player() {
		std::cerr << "[~player]\n";
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
	size_t player::send(const char *messg, ...) {
		if(messg && *messg) {
			size_t left;
			va_list args;

			va_start(args, messg);
			left = vwrite_to_output(*(cd()->desc), messg, args);
			va_end(args);
			desc().has_prompt = 0;
			return left;
		}

		desc().has_prompt = 0;
		return 0;
	}


	mods::affects::dissolver& player::get_affect_dissolver() {
		return m_affects;
	}

	void player::m_sync_equipment(){
		if(!m_char_data){ return; }
		for(unsigned i=0; i < NUM_WEARS; i++){
			if(m_equipment[i]){
				m_char_data->equipment[i] = m_equipment[i].get();
			}else{
				m_char_data->equipment[i] = nullptr;
			}
		}
	}
	void player::set_camera_viewing(bool b){
		m_camera_viewing = b;
	}
	void player::set_camera(obj_data_ptr_t obj){
		m_camera = std::make_shared<mods::camera>(this->uuid(),obj->uuid);
	}
	void player::block_for(uint16_t ticks, uint32_t unblock_event,uuid_t optional_uuid){
		m_block_data[unblock_event] = optional_uuid;
		m_block_event = mods::globals::defer_queue->push_ticks_event(ticks,{this->uuid(),unblock_event});
		m_has_block_event = true;
		m_blocked_until = unblock_event;
	}
	void player::cancel_block(){
		if(m_has_block_event){
			mods::globals::defer_queue->cancel_event(m_block_event);
			m_has_block_event = false;
		}
		m_blocked_until = 0;
	}
	bool player::is_blocked(){ return m_blocked_until != 0; }
	void player::unblock_event(uint32_t unblock){
		d("[player::unblock_event]:" << unblock);
		uuid_t target = 0;
		target = m_block_data[unblock];
		switch(unblock){
			case mods::deferred::EVENT_PLAYER_UNBLOCK_INSTALLATION:
				{
					auto obj = optr_by_uuid(target);
					if(!obj){
						std::cerr << "[WARNING] got nullptr from EVENT_PLAYER_UNBLOCK_INSTALLATION\n";
						break;
					}
					obj_to_room(obj, obj->in_room);
					mods::object_utils::set_done_installing(obj);
					this->send("\r\nYou successfully deploy a %s\r\n", obj->name.c_str());
					break;
				}
			case mods::deferred::EVENT_PLAYER_UNBLOCK_BREACH:
				{
					auto obj = optr_by_uuid(target);
					if(!obj){
						std::cerr << "[WARNING] got nullptr from EVENT_PLAYER_UNBLOCK_INSTALLATION\n";
						break;
					}
					mods::object_utils::set_done_breaching(obj);
					break;
				}
			default:
				std::cerr << "[WARNING] unhandled unblock event: " << unblock << "\n";
				break;
		}
		m_block_data.erase(unblock);
		m_blocked_until = 0;
		m_has_block_event = false;
	}
	uint32_t player::current_block(){
		return m_blocked_until;
	}
	bool player::camera_viewing(){ return m_camera_viewing; }
	int player::viewing_room(){
		if(this->camera_viewing()){
				return m_camera->room();
		}
		return this->room();
	}
	std::shared_ptr<mods::camera> player::get_camera() const {
		return m_camera;
	}
	void player::clear_camera(){
		m_camera_viewing = false;
		m_camera = nullptr;
	}

	obj_data_ptr_t player::primary(){
		return this->equipment(WEAR_PRIMARY);
	}
	obj_data_ptr_t player::secondary(){
		return this->equipment(WEAR_SECONDARY);
	}

	void player::set_attacking_with_primary(){
		m_attacking_with = this->primary();
	}
	void player::set_attacking_with_secondary(){
		m_attacking_with = this->secondary();
	}

};

#endif
