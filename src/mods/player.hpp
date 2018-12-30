#ifndef __MENTOC_MODS_PLAYER_HEADER__
#define  __MENTOC_MODS_PLAYER_HEADER__

#include <iostream>
#include <sys/socket.h>
#include <netdb.h>
#include <vector>

namespace mods {
	struct player;
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

#define WEAPON_SET_NUM 1
extern size_t send_to_char(char_data *ch, const char *messg, ...);
namespace mods::acl_list {
extern void set_access_rights(
					std::shared_ptr<mods::player>,const std::string&,bool);
};
namespace mods {
	namespace weapon {
		enum mask_type { SMG, SNIPE, SHOTGUN, GRENADE };
	};
	class player {
		public:
			using   class_type = mods::classes::types;
			using 	descriptor_data_t = std::deque<descriptor_data>;
			using 	descriptor_data_iterator_t = descriptor_data_t::iterator;
			typedef short weapon_set;
			typedef std::vector<class_type> class_capability_t;
			typedef std::vector<std::shared_ptr<mods::classes::base>> class_info_t;
			using   chdata = char_data;
			using   chdata_ptr = char_data *;
			using		descriptor_t = mods::descriptor_data;
			using		descriptor_iterator_t = std::deque<mods::descriptor_data>::iterator;
			using		time_type_t = unsigned long;//std::chrono::system_clock::time_point;
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
					std::shared_ptr<mods::player>,const std::string&,bool);

			void set_db_id(aligned_int_t id);
			aligned_int_t get_db_id() const;
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
			bool has_weapon_capability(int);
			bool has_builder_data();
			bool room_pave_mode();
			bool zone_pave_mode();

			/* class info */
			std::shared_ptr<mods::classes::base>& get_class(class_type);

			/* ammo gettters */
			obj_data* get_first_ammo_of_type(const weapon_type_t&) const;

			/* combat functions */
			bool can_snipe(char_data *target);
			bool is_weapon_loaded();
			bool carrying_ammo_of_type(const weapon_type_t&);
			void ammo_adjustment(int);
			int  ammo_type_adjustment(int,const weapon_type_t&);

			/* setters */
			void set_class_capability(const class_capability_t& caps);
			void set_cd(char_data* ch);
			void set_shared_ptr(std::shared_ptr<mods::player>&);

			time_type_t time() const;

			/** Affects */
			/**
			 * @return returns the legacy AFF_ flag given the modern aff flag
			 */
			bool has_affect(uint64_t f);
			bool has_affect(mods::flags::aff f);
			void affect(uint64_t flag);
			void affect(mods::flags::aff flag);
			void remove_affect(uint64_t flag);
			void remove_affect(mods::flags::aff flag);
			const std::map<mods::flags::aff,bool>& get_affected();
			void clear_all_affected();

			/** PLR_* Affects */
			bool has_affect_plr(uint64_t flag);
			bool has_affect_plr(mods::flags::plr flag);
			void affect_plr(uint64_t flag);
			void affect_plr(mods::flags::plr flag);
			void remove_affect_plr(uint64_t flag);
			void remove_affect_plr(mods::flags::plr flag);
			const std::map<mods::flags::plr,bool>& get_affected_plr();
			void clear_all_affected_plr();

			/** char_special_data_saved */
			char_special_data& char_specials(){
				return m_char_data->char_specials;
			}
			char_data* fighting(){
				return this->char_specials().fighting;
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
			void set_name(std::string n){
				this->cd()->player.name.assign(n);
				m_name = n;
			}
			std::string& name() {
				m_name = this->cd()->player.name.c_str();
				return m_name;
			}
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
			obj_data*      weapon();
			obj_data*      get_ammo(const weapon_type_t&);

			/* conversion operator to char_data* */
			operator chdata_ptr() const {
				return m_char_data;
			}

			/* weapon cooldown functions */
			void weapon_cooldown_start(unsigned long duration,weapon_set set);
			bool weapon_cooldown_expired(weapon_set);

			/* communication functions */
			void stc_room(const room_rnum&);
			void stc_room_desc(const room_rnum&);
			void exits();
			void stc(const char* m);
			void stc(const std::string m);
			void stc(int m);
			void done();

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
		private: 
			std::shared_ptr<builder_data_t> m_builder_data;
			std::string m_password;
			aligned_int_t m_db_id;
			void m_set_time();
			bool m_god_mode;
			bool m_imp_mode;
			bool m_bui_mode;
			std::map<mods::flags::aff,bool> m_affected;
			std::map<mods::flags::plr,bool> m_affected_plr;
			std::shared_ptr<mods::descriptor_data> m_desc;
			std::string	m_name;
			class_capability_t m_class_capability;
			bool         m_executing_js;
			char_data*   m_char_data;
			std::array<unsigned long,WEAPON_SET_NUM> m_weapon_cooldown;
			weapon_set   m_weapon_set;
			bool         m_do_paging;
			bool         m_capture_output;
			std::string  m_captured_output;
			unsigned int m_page;
			unsigned int m_current_page;
			std::string  m_current_page_fragment;
			std::vector<std::string> m_pages;
			class_info_t m_class_info;
			std::shared_ptr<char_data> m_shared_ptr;
			std::shared_ptr<player_special_data> m_player_specials;
			time_type_t	m_time;
			player_type_enum_t m_type;
	};
};

#include "util.hpp"


#endif
