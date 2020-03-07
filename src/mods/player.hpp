#ifndef __MENTOC_MODS_PLAYER_HEADER__
#define  __MENTOC_MODS_PLAYER_HEADER__

#include <iostream>
#include <sys/socket.h>
#include <netdb.h>
#include <vector>

namespace mods {
	struct player;
	namespace classes {
		struct sentinel;
	};
};
#include "flags.hpp"
#include "classes/types.hpp"
#include "classes/base.hpp"
#include "classes/medic.hpp"
#include "classes/sniper.hpp"
#include "classes/marine.hpp"
#include "classes/support.hpp"
#include "classes/engineer.hpp"
#include "../conf.h"
#include "../sysdep.h"
#include "../structs.h"
#include "../types.hpp"
#include "drone.hpp"
#include <chrono>
#include "acl_list.hpp"
#include "overhead_map.hpp"
#include "weapon.hpp"
#include "affects.hpp"
#include <cstdio> // for FILE*

#define WEAPON_SET_NUM 1
extern size_t send_to_char(char_data *ch, const char *messg, ...);
extern room_rnum real_room(room_vnum);
namespace mods::acl_list {
	extern void set_access_rights(
			player_ptr_t,const std::string&,bool);
};
extern player_ptr_t ptr(char_data*);
namespace mods::classes {
	struct medic;
	struct sniper;
	struct sentinel;
};

namespace mods::globals {
	extern player_ptr_t player_nobody;
};
namespace mods {
	struct player {
		using obj_ptr_t = std::shared_ptr<obj_data>;
		public:
			using 	access_level_t = player_level;
			using   class_type = mods::classes::types;
			using 	descriptor_data_t = std::deque<descriptor_data>;
			using 	descriptor_data_iterator_t = descriptor_data_t::iterator;
			using 	weapon_set = short;
			using 	class_capability_t = std::vector<class_type>;
			using		class_info_t = std::vector<std::shared_ptr<mods::classes::base>>;
			using   chdata = char_data;
			using   chdata_ptr = char_data *;
			using		descriptor_t = mods::descriptor_data;
			using		descriptor_iterator_t = std::deque<mods::descriptor_data>::iterator;
			using		time_type_t = unsigned long;//std::chrono::system_clock::time_point;
			operator chdata_ptr(){ return cd(); }
			enum player_type_enum_t { 
				PLAYER, MOB, DRONE,
				PLAYER_MUTED_DESCRIPTOR,
				MOB_MUTED_DESCRIPTOR,
				DRONE_MUTED_DESCRIPTOR
			};

			/* constructors and destructors */
			player();
			player(char_data*);
			player(mods::player*);
			player(player_type_enum_t);
			~player();
			void init();
			friend void mods::acl_list::set_access_rights(
					player_ptr_t,const std::string&,bool);

			void set_db_id(aligned_int_t id);
			aligned_int_t get_db_id() const;
			aligned_int_t db_id() const { return m_db_id; }
			void set_password(std::string pw);
			std::string get_password();
			std::string password(){
				return this->get_password();
			}
			bool god_mode() const;
			bool implementor_mode() const;
			bool builder_mode() const;
			void set_god_mode(bool b);
			void set_imp_mode(bool b);
			void set_bui_mode(bool b);
			static constexpr int PAGE_SIZE = 40;

			/** for the player.time.* variables */
			void set_time_birth(time_t b);
			time_t get_time_birth() const;
			void set_time_logon(time_t t);
			time_t get_time_login() const;
			void set_time_played(int t);
			int get_time_played() const;

			/* Javascript functions */
			bool is_executing_js() const;
			std::string js_object();
			void executing_js(bool);

			/* has_* functions */
			bool has_ammo();
			bool has_class_capability(class_type);
			bool has_equipment_tag(const std::string&);
			bool has_inventory_capability(int);
			bool has_thermite();
			bool has_weapon_capability(uint8_t);
			bool has_builder_data();
			bool room_pave_mode();
			bool zone_pave_mode();


			/* class info */
			player_class_t get_class() const { return m_class; }
			void set_class(player_class_t c);

			/* ammo gettters */
			obj_data* get_first_ammo_of_type(const weapon_type_t&) const;

			/* combat functions */
			bool can_snipe(char_data *target);
			bool is_weapon_loaded();
			bool carrying_ammo_of_type(const weapon_type_t&);
			obj_data* carrying();
			void uncarry(obj_ptr_t obj);
			std::deque<obj_ptr_t>& real_carrying(){ return cd()->m_carrying; }
			void carry(obj_ptr_t);
			void ammo_adjustment(int);
			int  ammo_type_adjustment(int,const weapon_type_t&);

			/* setters */
			void set_cd(char_data* ch);
			void set_shared_ptr(player_ptr_t&);

			time_type_t time() const;

			/** Affects */
			/**
			 * @return returns the legacy AFF_ flag given the modern aff flag
			 */
			bool has_affect(aligned_int_t f);
			void affect(aligned_int_t flag);
			void remove_affect(aligned_int_t flag);
			aligned_int_t get_affected();
			void clear_all_affected();


			/** TODO these need to go */
			void affect(mods::flags::aff flag);
			void remove_affect(mods::flags::aff flag);
			bool has_affect(mods::flags::aff f);
			bool has_affect_plr(mods::flags::plr flag);
			void affect_plr(mods::flags::plr flag);
			void remove_affect_plr(mods::flags::plr flag);
			/** END got to go */

			/** PLR_* Affects */
			bool has_affect_plr(aligned_int_t flag);
			void affect_plr(aligned_int_t flag);
			void remove_affect_plr(aligned_int_t flag);
			aligned_int_t get_affected_plr();
			void clear_all_affected_plr();

			/** char_special_data_saved */
			char_special_data& char_specials(){
				return m_char_data->char_specials;
			}
			player_ptr_t fighting(){
				return ptr(this->char_specials().fighting);
			}
			bool is_fighting(char_data* ch){
				return this->char_specials().fighting == ch;
			}
			bool is_fighting(player_ptr_t p){
				return this->char_specials().fighting == p->cd();
			}
			char_data* hunting(){
				return this->char_specials().hunting;
			}
			byte& position(){
				return this->char_specials().position;
			}

			int&	carry_weight(){
				return this->char_specials().carry_weight;
			}
			byte& carry_items(){
				return this->char_specials().carry_items;
			}
			int&	timer(){
				return this->char_specials().timer;
			}

			char_special_data_saved& saved(){
				return this->char_specials().saved;
			}

			char_ability_data& real_abils(){	 /* Abilities without modifiers   */
				return m_char_data->real_abils;
			}
			char_ability_data& aff_abils(){	 /* Abils with spells/stones/etc  */
				return m_char_data->aff_abils;
			}

			sh_int* apply_saving_throw(){
				return this->char_specials().saved.apply_saving_throw;
			}
			sh_int& apply_saving_throw(std::size_t i){
				return this->char_specials().saved.apply_saving_throw[i];
			}


			/* informational functions */

			sh_int& mana() {
				return m_char_data->points.mana;
			}
			sh_int& max_mana() {
				return m_char_data->points.max_mana;
			}
			sh_int& hp() {
				return m_char_data->points.hit;
			}
			sh_int& max_hp() {
				return m_char_data->points.max_hit;
			}
			sh_int& move() {
				return m_char_data->points.move;
			}
			sh_int& max_move() {
				return m_char_data->points.max_move;
			}

			sh_int& armor() {
				return m_char_data->points.armor;
			}
			int& gold() {
				return m_char_data->points.gold;
			}
			int& bank_gold() {
				return m_char_data->points.bank_gold;
			}
			int& exp() {
				return m_char_data->points.exp;
			}
			sbyte& hitroll() {
				return m_char_data->points.hitroll;
			}
			sbyte& damroll() {
				return m_char_data->points.damroll;
			}
			byte& level(){
				return m_char_data->player.level;
			}
			ubyte& weight(){
				return m_char_data->player.weight;
			}
			ubyte& height(){
				return m_char_data->player.height;
			}
			void set_title(const std::string& t){
				m_char_data->player.title.assign(t);
			}
			mods::string& title(){
				return m_char_data->player.title;
			}
			mods::string& short_descr(){
				return m_char_data->player.short_descr;
			}
			mods::string& long_descr(){
				return m_char_data->player.long_descr;
			}
			mods::string& description(){
				return m_char_data->player.description;
			}
			byte& chclass(){
				return m_char_data->player.chclass;
			}
			sh_int& hometown(){
				return m_char_data->player.hometown;
			}
			time_data& ch_time(){
				return m_char_data->player.time;
			}
			void set_name(std::string n);

			std::string ucname();
			std::string& name();
			void set_sex(byte s){
				m_char_data->player.sex = s;
			}
			byte& sex(){
				return m_char_data->player.sex;
			}
			void set_room(room_rnum r){
				m_char_data->in_room = r;
			}
			room_rnum& room(){
				return m_char_data->in_room;
			}
			uuid_t& uuid() const {
				return m_char_data->uuid;
			}
			char_data* cd() const {
				return m_char_data;
			}
			rifle_data_t* rifle();
			uint16_t ammo(){ auto eq = m_equipment[WEAR_WIELD]; return eq ? eq->obj_flags.ammo : 0 ; }
			uint16_t set_ammo(uint16_t value){
				auto eq = m_equipment[WEAR_WIELD]; 
				if(eq){ return eq->obj_flags.ammo += value; }
				return 0;
			}

			obj_data*      get_ammo(const weapon_type_t&);
			mods::string weapon_name();

			/* conversion operator to char_data* */
			operator chdata_ptr() const {
				return m_char_data;
			}

			/* weapon cooldown functions */

			void weapon_cooldown_start(uint16_t duration,weapon_set set);
			bool weapon_cooldown_expired(weapon_set);

			/* communication functions */
			void stc_room(const room_rnum&);
			void stc_room_desc(const room_rnum&);
			void stc(const char* m);
			void stc(const std::string m);
			void stc(const mods::string& m);
			void stc(int m);
			void stc(std::string_view);
			void sendln(std::string_view str);
			void sendln(mods::string& str);
			/** 'plain' sendln (no color eval needed) */
			void psendln(std::string_view str);
			void psendln(mods::string& str);
			void raw_send(const mods::string&);
			void done();
			size_t send(const char *messg, ...);

			/* pager functions */
			player&             pager_start();
			player&             pager_end();
			void                pager_next_page();
			void                page(int page_number);
			/** TODO: all these queueing functions and overloaded operators need to accomodate
			 * the new player_type_enum_t values when it comes to muting queued output.
			 */
			void                queue_page_fragment(std::string_view fragment) {
				for(auto c : fragment) {
					m_current_page_fragment += c;
					if(c == '\n') {
						m_pages.push_back(m_current_page_fragment);
						m_current_page_fragment = "";
					}
				}
			}
			player& operator<<(const mods::string& m) {
				if(m_do_paging) {
					queue_page_fragment(m.c_str());
				} else {
					stc(m);
				} return *this;
			}

			player& operator<<(const char* m) {
				if(m_do_paging) {
					queue_page_fragment(m);
				} else {
					stc(m);
				} return *this;
			}
			player& operator<<(const std::string& m) {
				if(m_do_paging) {
					queue_page_fragment(m);
				} else {
					stc(m);
				}
				return *this;
			}
			player& operator<<(int m) {
				if(m_do_paging) {
					queue_page_fragment(std::to_string(m));
				} else {
					stc(m);
				}

				return *this;
			}
			void pager_clear();
			bool paging() const {
				return m_pages.size();
			}
			void set_desc(std::deque<descriptor_data>::iterator it);
			void set_desc(std::shared_ptr<descriptor_data> it);
			void set_char_on_descriptor(std::deque<descriptor_data>::iterator it);
			void set_socket(socket_t d);
			socket_t socket();
			void increment_bad_password_count();
			int get_bad_password_count();
			void set_bad_password_count(int);

			descriptor_data& desc();
			void set_state(int);
			int state();
			void set_host(std::string host);
			std::string host() const;

			/* captured output */
			void capture_output(bool capture_status);
			std::string_view get_captured_output();
			void clear_captured_output();
			void set_type(player_type_enum_t t);
			player_type_enum_t type(){
				return m_type;
			}
			std::shared_ptr<builder_data_t> builder_data;

			/** TODO: call a CRM function. In fact, maybe don't even
			 * store this deactivate_account() function on this 
			 * object
			 */
			void deactivate_account();
			bool authenticated() const { return m_authenticated; }
			void set_authenticated(bool b) { m_authenticated = b; }
			void set_prefs(long prefs){ cd()->player_specials->saved.pref = prefs; }
			auto get_prefs(){ return cd()->player_specials->saved.pref; }
			auto get_lense(){ return m_lense_type; }
			void set_lense(const lense_type_t& lense){ m_lense_type = lense; }

			/**
			 * The m_flag structure is the better design choice for any flag or preference
			 * saving mechanism. The affect/plr affect function interface on this object
			 * wastes a lot of space by using enums which are 64 bits in size. For that size
			 * we could have 64 flags instead of one enum that's 64 bits long. 
			 *
			 *
			 * The m_flag interface philosophy is that you can essentially fill the std::array
			 * that is m_flags with "chunks" of data. Each chunk corresponds to the set of 
			 * flags that the user is interested in. If we want to dedicate 64 bits to a user's
			 * preferences, all we'd have to do is agree on the index for which "chunk" that
			 * the flags will be stored in. 
			 *
			 * If we wanted to store legacy values, we could dedicate the first chunk (0, zero)
			 * to the m_flags chunk as follows:
			 * m_flags[0] |= AFF_BLIND;
			 * ...
			 *
			 * Of course, we have helper functions below that will do the job for us:
			 *
			 * player->set_flag(LEGACY_AFF_CHUNK,AFF_BLIND);
			 *
			 */
			using chunk_type_t = mods::flags::chunk_type_t;
			aligned_int_t set_flag(chunk_type_t chunk,aligned_int_t bit){
				m_flags[chunk] |= bit;
				return m_flags[chunk];
			}
			aligned_int_t toggle_flag(chunk_type_t chunk,aligned_int_t bit){
				m_flags[chunk] ^= bit; 
				return m_flags[chunk];
			}
			aligned_int_t remove_flag(chunk_type_t chunk,aligned_int_t bit){
				m_flags[chunk] &= ~(bit);
				return m_flags[chunk];
			}
			aligned_int_t get_chunk(chunk_type_t chunk) const {
				return m_flags[chunk];
			}
			bool has_flag(chunk_type_t chunk,aligned_int_t bit) const {
				return m_flags[chunk] & bit;
			}

			/**
			 * Overhead map functions
			 */
			uint8_t get_overhead_map_width() const { 
				printf("map-width: %d\n",m_overhead_map_width);
				return m_overhead_map_width;
			}
			void set_overhead_map_width(uint8_t w){ m_overhead_map_width = w; }
			uint8_t get_overhead_map_height() const {
				printf("map-height: %d\n",m_overhead_map_height);
				return m_overhead_map_height;
			}
			void set_overhead_map_height(uint8_t h){ m_overhead_map_height = h; }
			void equip(obj_ptr_t obj,int pos);
			void unequip(int pos);
			obj_ptr_t equipment(int pos);
			std::vector<affected_type>& get_affected_by() { return m_affected_by; }
			std::vector<affected_type>& add_affected_by(affected_type&& add_this){
				add_this.index = m_affected_by.size();
				m_affected_by.emplace_back(std::move(add_this));
				return m_affected_by;
			}
			//void set_affected_by(std::vector<affected_type>& affect_list) { m_affected_by = affect_list; }
			void clr_affected_by(){ m_affected_by.clear(); }
			std::vector<affected_type>& del_affected_by(const std::size_t& idx){
				if(idx < m_affected_by.size()){
					m_affected_by.erase(m_affected_by.begin()+idx);
				}
				return m_affected_by;
			}
			std::vector<affected_type>& del_affected_by(const affected_type& af){
				return del_affected_by(af.index);
			}
			std::shared_ptr<mods::classes::sniper> 	cl_sniper();
			std::shared_ptr<mods::classes::medic> 	cl_medic();
			std::shared_ptr<mods::classes::sentinel> cl_sentinel(){ return m_class_sentinel; }
			void set_sentinel(std::shared_ptr<mods::classes::sentinel> s){
				m_class_sentinel = s;
			}
			obj_data_ptr_t sniper_rifle();
			void start_histfile();
			void write_histfile(std::string_view);
			void stop_histfile();
			std::string hf_pack(uint32_t index,uint8_t op,unsigned long time_stamp);
			int pack_get(
					uint32_t index,
					uint8_t op,
					unsigned long time_stamp,
					std::string& got);

			int pack_set(
					uint32_t index,
					uint8_t op,
					unsigned long time_stamp,
					void* value,
					std::size_t v_size);
			bool is(char_data* ch) const { return ch == m_char_data; }
			bool is(player_ptr_t p) const { return m_char_data == p->cd(); }
			bool is_nobody() const { return m_char_data == nullptr; }

			bool is_npc() const {
				return IS_NPC(cd());
			}
			mods::affects::dissolver& get_affect_dissolver();
		protected:
			void perform_equip_calculations(int pos,bool equip);
			lense_type_t m_lense_type;
			uint8_t m_overhead_map_width;
			uint8_t m_overhead_map_height;
			char_data*   m_char_data;
			std::shared_ptr<char_data> m_shared_ptr;
			//std::deque<std::shared_ptr<obj_data>> m_carrying;
			std::vector<affected_type> m_affected_by;
		private: 
			bool         m_executing_js;
			bool m_authenticated;
			std::shared_ptr<builder_data_t> m_builder_data;
			std::string m_password;
			aligned_int_t m_db_id;
			void m_set_time();
			bool m_god_mode;
			bool m_imp_mode;
			bool m_bui_mode;
			std::array<aligned_int_t,mods::flags::chunk_type_t::LAST + 1> m_flags;
			std::shared_ptr<mods::descriptor_data> m_desc;
			std::string	m_name;
			std::string m_ucname;
			player_class_t m_class;
			class_capability_t m_class_capability;
			std::array<uint16_t,WEAPON_SET_NUM> m_weapon_cooldown;
			weapon_set   m_weapon_set;
			bool         m_do_paging;
			bool         m_capture_output;
			std::string  m_captured_output;
			unsigned int m_page;
			unsigned int m_current_page;
			std::string  m_current_page_fragment;
			std::vector<std::string> m_pages;
			class_info_t m_class_info;
			std::shared_ptr<player_special_data> m_player_specials;
			time_type_t	m_time;
			player_type_enum_t m_type;
			weapon_type_t m_weapon_type;
			weapon_type_t m_weapon_flags;
			std::shared_ptr<mods::classes::sentinel> m_class_sentinel;
			std::shared_ptr<mods::classes::sniper> m_class_sniper;
			std::shared_ptr<mods::classes::medic> m_class_medic;
			FILE* m_histfile_fp;
			bool m_histfile_on;
			uint32_t m_histfile_index;
			mods::affects::dissolver m_affects;
			std::array<obj_ptr_t,NUM_WEARS> m_equipment;
	};
};

#include "util.hpp"


#endif
