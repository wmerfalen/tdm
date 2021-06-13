#ifndef __MENTOC_MODS_PLAYER_HEADER__
bool has_builder_data();
#define  __MENTOC_MODS_PLAYER_HEADER__


namespace mods {
	struct player;
	namespace classes {
		struct sniper;
		struct marine;
		struct breacher;
		struct engineer;
		struct medic;
		struct ghost;
		struct support;

		struct pyrexia;
		struct striker;
		struct forge;
		struct syndrome;

		struct sentinel;
		struct contagion;
		struct psyop;
	};
};
#include "flags.hpp"
#include "classes/types.hpp"

#include "../structs.h"
#include "affects.hpp"
#include "camera.hpp"
#include "rifle-attachments.hpp"
#include <ctime>

namespace mods::armor {
	struct basic_protection;
	struct advanced_protection;
	struct elite_protection;
};

#include "armor/basic-protection.hpp"
#include "armor/advanced-protection.hpp"
#include "armor/elite-protection.hpp"

#define WEAPON_SET_NUM 1
extern size_t send_to_char(char_data *ch, const char *messg, ...);
extern room_rnum real_room(room_vnum);
namespace mods::acl_list {
	extern void set_access_rights(
	    player_ptr_t,const std::string&,bool);
};
extern player_ptr_t ptr(char_data*);

#include "damage-event.hpp"
using lambda_queue_t = std::multimap<uint64_t,std::function<void()>>;
using event_queue_t = std::multimap<uint32_t,std::tuple<uuid_t,uint32_t>>;
using lambda_queue_iterator = lambda_queue_t::iterator;
using event_queue_iterator = event_queue_t::iterator;

namespace mods::globals {
	extern player_ptr_t player_nobody;
};
namespace mods::contracts {
	struct player_contract_instance;
};


namespace mods {
	struct player {
			using contract_vnum_t = uint32_t;
			using contract_list_t = std::forward_list<std::shared_ptr<mods::contracts::player_contract_instance>>;
			using damage = damage_event_t;
			using obj_ptr_t = std::shared_ptr<obj_data>;
			using affect_t = mods::affects::affect_t;
			using affect_list_t = std::vector<affect_t>;
			using affect_map_t = std::map<affect_t,int16_t>;
			using entity_id = uuid_t;
			using affect_dissolver_t = mods::affects::dissolver<affect_t,affect_list_t,affect_map_t,uuid_t>;
		public:
			using 	access_level_t = player_level;
			using   class_type = mods::classes::types;
			using 	descriptor_data_t = std::deque<descriptor_data>;
			using 	descriptor_data_iterator_t = descriptor_data_t::iterator;
			using 	weapon_set = short;
			using 	class_capability_t = std::vector<class_type>;
			//using		class_info_t = std::vector<std::shared_ptr<mods::classes::base>>;
			using   chdata = char_data;
			using   chdata_ptr = char_data *;
			using		descriptor_t = mods::descriptor_data;
			using		descriptor_iterator_t = std::deque<mods::descriptor_data>::iterator;
			using		time_type_t = unsigned long;
			using rate_limit_data_t = std::vector<player::time_type_t>;
			operator chdata_ptr() {
				return cd();
			}
			enum misc_pref_enum_t {
				HOLD_ANYTHING = 1,
				SIZE = HOLD_ANYTHING
			};
			using mpref = misc_pref_enum_t;
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

			access_level_t access_level();
			void set_db_id(aligned_int_t id);
			aligned_int_t get_db_id() const;
			aligned_int_t db_id() const {
				return m_db_id;
			}
			aligned_int_t pk_id() const {
				return m_db_id;
			}
			void set_password(std::string pw);
			std::string get_password();
			std::string password() {
				return this->get_password();
			}
			bool god_mode() const;
			bool implementor_mode() const;
			bool builder_mode() const;
			bool get_imp_mode() {
				return m_imp_mode;
			}
			bool get_bui_mode() {
				return m_bui_mode;
			}
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

			obj_ptr_t get_ammo_for(obj_ptr_t&);
			void consume_from_carrying(obj_ptr_t& item);

			/* class info */
			player_class_t get_class();
			void set_class(player_class_t);
			mods::string get_class_string() const;
			bool can(std::string_view);

			/* ammo gettters */
			obj_data* get_first_ammo_of_type(const weapon_type_t&) const;

			/* combat functions */
			bool can_snipe(player_ptr_t target);
			bool is_weapon_loaded();
			bool carrying_ammo_of_type(const weapon_type_t&);
			obj_data* carrying();
			std::vector<obj_data*> vcarrying();
			void uncarry(obj_ptr_t obj);
			void carry(obj_ptr_t);
			void ammo_adjustment(int);
			int  ammo_type_adjustment(int,const weapon_type_t&);

			/* setters */
			void set_cd(char_data* ch);
			void set_shared_ptr(player_ptr_t&);
			void quitting(bool s) {
				m_quitting = true;
			}
			bool quitting() {
				return m_quitting;
			}

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
			char_special_data& char_specials() {
				return cd()->char_specials;
			}
			player_ptr_t fighting() {
				return ptr(this->char_specials().fighting);
			}
			bool is_fighting(char_data* ch) {
				return this->char_specials().fighting == ch;
			}
			bool is_fighting(player_ptr_t p) {
				return this->char_specials().fighting == p->cd();
			}
			char_data* hunting() {
				return this->char_specials().hunting;
			}
			byte& position() {
				return this->char_specials().position;
			}
			void set_position(byte p);

			int&	carry_weight() {
				return this->char_specials().carry_weight;
			}
			byte& carry_items() {
				return this->char_specials().carry_items;
			}
			int&	timer() {
				return this->char_specials().timer;
			}

			char_special_data_saved& saved() {
				return this->char_specials().saved;
			}

			char_ability_data& real_abils() {	 /* Abilities without modifiers   */
				return cd()->real_abils;
			}
			char_ability_data& aff_abils() {	 /* Abils with spells/stones/etc  */
				return cd()->aff_abils;
			}

			sh_int* apply_saving_throw() {
				return this->char_specials().saved.apply_saving_throw;
			}
			sh_int& apply_saving_throw(std::size_t i) {
				return this->char_specials().saved.apply_saving_throw[i];
			}


			/* informational functions */

			auto& mana() {
				return cd()->points.mana;
			}
			auto& max_mana() {
				return cd()->points.max_mana;
			}
			auto& hp() {
				return cd()->points.hit;
			}
			auto& max_hp() {
				return cd()->points.max_hit;
			}
			auto& move() {
				return cd()->points.move;
			}
			auto& max_move() {
				return cd()->points.max_move;
			}

			/** base stats - start */
			uint16_t& strength() {
				return this->real_abils().str;
			}
			uint16_t& constitution() {
				return this->real_abils().con;
			}
			uint16_t& dexterity() {
				return this->real_abils().dex;
			}
			uint16_t& intelligence() {
				return this->real_abils().intel;
			}
			uint16_t& wisdom() {
				return this->real_abils().wis;
			}
			uint16_t& electronics() {
				return this->real_abils().electronics;
			}
			uint16_t& chemistry() {
				return this->real_abils().chemistry;
			}
			uint16_t& strategy() {
				return this->real_abils().strategy;
			}
			uint16_t& marksmanship() {
				return this->real_abils().marksmanship;
			}
			uint16_t& sniping() {
				return this->real_abils().sniping;
			}
			uint16_t& weapon_handling() {
				return this->real_abils().weapon_handling;
			}
			uint16_t& demolitions() {
				return this->real_abils().demolitions;
			}
			uint16_t& real_armor() {
				return this->real_abils().armor;
			}
			uint16_t& medical() {
				return this->real_abils().medical;
			}
			uint16_t& charisma() {
				return this->real_abils().cha;
			}
			/** base stats - end */

			sh_int& armor() {
				return cd()->points.armor;
			}
			int& gold() {
				return cd()->points.gold;
			}
			int& mp() {
				return this->gold();
			}
			int& bank_gold() {
				return cd()->points.bank_gold;
			}
			int& exp() {
				return cd()->points.exp;
			}
			auto& hitroll() {
				return cd()->points.hitroll;
			}
			auto& damroll() {
				return cd()->points.damroll;
			}
			uint8_t& level();
			ubyte& weight() {
				return cd()->player.weight;
			}
			ubyte& height() {
				return cd()->player.height;
			}
			void set_title(const std::string& t) {
				cd()->player.title.assign(t);
			}
			mods::string& title() {
				return cd()->player.title;
			}
			mods::string& short_descr() {
				return cd()->player.short_descr;
			}
			mods::string& long_descr() {
				return cd()->player.long_descr;
			}
			mods::string& description() {
				return cd()->player.description;
			}
			uint8_t& chclass() {
				return cd()->player.chclass;
			}
			uint8_t& hometown() {
				return cd()->player.hometown;
			}
			time_data& ch_time() {
				return cd()->player.time;
			}
			void set_name(std::string n);

			std::string ucname();
			std::string& name();
			void set_sex(byte s) {
				cd()->player.sex = s;
			}
			uint8_t& sex() {
				return cd()->player.sex;
			}
			void set_room(room_rnum r) {
				cd()->in_room = r;
			}
			room_rnum& room() {
				return cd()->in_room;
			}
			uuid_t& uuid() const {
				return cd()->uuid;
			}
			char_data* cd() const {
				return m_char_data;
			}
			rifle_data_t* rifle();
			uint16_t ammo() {
				auto eq = m_equipment[WEAR_WIELD];
				return eq ? eq->obj_flags.ammo : 0 ;
			}
			uint16_t set_ammo(uint16_t value) {
				auto eq = m_equipment[WEAR_WIELD];
				if(eq) {
					return eq->obj_flags.ammo += value;
				}
				return 0;
			}

			obj_data*      get_ammo(const weapon_type_t&);
			mods::string weapon_name();

			/* conversion operator to char_data* */
			operator chdata_ptr() const {
				return cd();
			}

			/* weapon cooldown functions */
			void weapon_cooldown_clear();
			void weapon_cooldown_start(const int& ticks);
			const bool& can_attack_again() const;

			/* communication functions */
			void stc_room(const room_rnum&);
			void stc_room_desc(const room_rnum&);
			void stc(const char* m);
			void stc(const std::string m);
			void stc(const mods::string& m);
			void stc(int m);
			void stc(std::string_view);
			void sendln(std::string_view str);
			void send(const std::vector<std::string>&);
			void sendln(mods::string& str);
			/** 'plain' sendln (no color eval needed) */
			void psendln(std::string_view str);
			void psendln(mods::string& str);
			void raw_send(const mods::string&);
			void done();
			size_t send(const char *messg, ...);
			size_t godsend(const char *messg, ...);

			/* pager functions */
			player&             pager_start();
			player&             pager_end();
			void                pager_next_page();
			void                page(int page_number);
			/** TODO: all these queueing functions and overloaded operators need to accomodate
			 * the new player_type_enum_t values when it comes to muting queued output.
			 */
			void queue_send_fragment(const char* messg, ...);
			void queue_page_fragment(std::string_view fragment) {
				for(const auto& c : fragment) {
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
				}
				return *this;
			}

			player& operator<<(const char* m) {
				if(!m) {
					return *this;
				}
				if(m_do_paging) {
					queue_page_fragment(std::string(m));
				} else {
					stc(m);
				}
				return *this;
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
			player_type_enum_t type() {
				return m_type;
			}
			std::shared_ptr<builder_data_t> builder_data;

			/** TODO: call a CRM function. In fact, maybe don't even
			 * store this deactivate_account() function on this
			 * object
			 */
			void deactivate_account();
			bool authenticated() const {
				return m_authenticated;
			}
			void set_authenticated(bool b) {
				m_authenticated = b;
			}
			void set_prefs(long prefs) {
				cd()->player_specials->saved.pref = prefs;
			}
			auto get_prefs() {
				return cd()->player_specials->saved.pref;
			}
			auto get_lense() {
				return m_lense_type;
			}
			void set_lense(const lense_type_t& lense) {
				m_lense_type = lense;
			}

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
			aligned_int_t set_flag(chunk_type_t chunk,aligned_int_t bit) {
				m_flags[chunk] |= bit;
				return m_flags[chunk];
			}
			aligned_int_t toggle_flag(chunk_type_t chunk,aligned_int_t bit) {
				m_flags[chunk] ^= bit;
				return m_flags[chunk];
			}
			aligned_int_t remove_flag(chunk_type_t chunk,aligned_int_t bit) {
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
				return m_overhead_map_width;
			}
			void set_overhead_map_width(uint8_t w) {
				m_overhead_map_width = w;
			}
			uint8_t get_overhead_map_height() const {
				return m_overhead_map_height;
			}
			void set_overhead_map_height(uint8_t h) {
				m_overhead_map_height = h;
			}
			void equip(obj_ptr_t obj,int pos);
			void equip(uuid_t,int pos);
			void unequip(int pos);
			void unequip_into_inventory(int pos);
			obj_ptr_t equipment(int pos);
			std::vector<affected_type>& get_affected_by() {
				return m_affected_by;
			}
			std::vector<affected_type>& add_affected_by(affected_type&& add_this) {
				add_this.index = m_affected_by.size();
				m_affected_by.emplace_back(std::move(add_this));
				return m_affected_by;
			}
			//void set_affected_by(std::vector<affected_type>& affect_list) { m_affected_by = affect_list; }
			void clr_affected_by() {
				m_affected_by.clear();
			}
			std::vector<affected_type>& del_affected_by(const std::size_t& idx) {
				if(idx < m_affected_by.size()) {
					m_affected_by.erase(m_affected_by.begin()+idx);
				}
				return m_affected_by;
			}
			std::vector<affected_type>& del_affected_by(const affected_type& af) {
				return del_affected_by(af.index);
			}
			std::shared_ptr<mods::classes::sniper>& sniper();
			void set_sniper(std::shared_ptr<mods::classes::sniper> g);

			std::shared_ptr<mods::classes::breacher>& breacher();
			void set_breacher(std::shared_ptr<mods::classes::breacher> g);

			std::shared_ptr<mods::classes::marine>& marine();
			void set_marine(std::shared_ptr<mods::classes::marine> g);

			std::shared_ptr<mods::classes::engineer>& engineer();
			void set_engineer(std::shared_ptr<mods::classes::engineer> g);

			std::shared_ptr<mods::classes::medic>& medic();
			void set_medic(std::shared_ptr<mods::classes::medic> g);

			std::shared_ptr<mods::classes::support>& support();
			void set_support(std::shared_ptr<mods::classes::support> g);

			std::shared_ptr<mods::classes::ghost>& ghost();
			void set_ghost(std::shared_ptr<mods::classes::ghost> g);

			std::shared_ptr<mods::classes::pyrexia>& pyrexia();
			void set_pyrexia(std::shared_ptr<mods::classes::pyrexia> g);
			std::shared_ptr<mods::classes::forge>& forge();
			void set_forge(std::shared_ptr<mods::classes::forge> g);
			std::shared_ptr<mods::classes::syndrome>& syndrome();
			void set_syndrome(std::shared_ptr<mods::classes::syndrome> g);

			void start_histfile();
			void write_histfile(std::string_view);
			void stop_histfile();
			bool is(char_data* ch) const {
				return ch == cd();
			}
			bool is(player_ptr_t p) const {
				return cd() == p->cd();
			}
			bool is_nobody() const {
				return cd() == nullptr;
			}

			bool is_npc() const;
			affect_dissolver_t& get_affect_dissolver();
			void set_misc_pref(misc_pref_enum_t bit,bool on_off) {
				m_misc_pref[bit] = on_off;
			}
			bool get_misc_pref(misc_pref_enum_t bit) {
				return m_misc_pref[bit];
			}

			/** viewing modifiers */
			bool has_night_vision() const;
			bool has_thermal_vision() const;

			/** visibility */
			char_data::visibility_t& visibility();

			/** camera dynamics */
			void set_camera(obj_data_ptr_t obj);
			std::shared_ptr<mods::camera> get_camera() const ;
			void clear_camera();
			bool camera_viewing();
			void set_camera_viewing(bool b);
			int viewing_room();

			void block_for(uint16_t ticks, uint32_t unblock_event,uuid_t optional_uuid);
			bool is_blocked();
			void unblock_event(uint32_t unblock);
			void cancel_block();
			uint32_t current_block();

			/** primary+secondary weapon helpers */
			obj_data_ptr_t primary();
			obj_data_ptr_t secondary();
			void set_attacking_with_primary();
			void set_attacking_with_secondary();
			void set_attacking_with(obj_ptr_t weapon) {
				m_attacking_with = weapon;
			}
			void clear_attacking_with() {
				m_attacking_with = nullptr;
			}
			obj_data_ptr_t attacking_with() {
				return m_attacking_with;
			}
			int attacking_with_type();
			void set_fight_timestamp();

			/** reporting utilities */
			std::string get_type_string();
			void report();

			void set_skill(int,uint16_t proficiency);
			uint16_t skill(int);

			void damage_event(feedback_t);
			void set_attacker(uuid_t a) {
				this->m_attacker_uuid = a;
			}
			uuid_t get_attacker() {
				return this->m_attacker_uuid;
			}
			void register_damage_event_callback(damage_event_t a,damage_event_callback_t cb);
			void dispatch_event(feedback_t);
			void set_watching(direction_t dir) {
				this->m_watching = dir;
			}
			direction_t get_watching() {
				return this->m_watching;
			}
			int screen_width();
			room_vnum vnum();

			/** for shift left/right defuse/hack games */
			std::tuple<uint32_t,uint8_t> currently_hacking();
			void set_currently_hacking(uint32_t id,uint8_t row);


			/** Americans with Disabilities Act (ADA) support */
			bool needs_ada() const {
				return m_ada;
			}
			void set_ada(bool s) {
				m_ada = true;
			}
			void set_ada_data(std::string_view,std::string_view);
			std::map<std::string,std::string>& get_ada_data();

			void error(std::string_view msg) {
				send("{red}%s{/red}", msg.data());
			}
			void errorln(std::string_view msg) {
				send("{red}%s{/red}\r\n", msg.data());
			}

			uuid_t& drone_uuid() {
				return cd()->drone_uuid;
			}
			bool& drone_simulate() {
				return cd()->drone_simulate;
			}

			rate_limit_data_t& get_rate_limit_data(int action);
			void rate_limit_hit(int action);

			std::array<uint8_t,5> triads();
			void admin_success(std::string_view msg);
			void admin_fail(std::string_view msg);

			int16_t& incendiary_resistance_percent();
			int16_t& explosive_resistance_percent();
			int16_t& shrapnel_resistance_percent();
			int16_t& corrosive_resistance_percent();
			int16_t& cryogenic_resistance_percent();
			int16_t& radiation_resistance_percent();
			int16_t& emp_resistance_percent();
			int16_t& shock_resistance_percent();
			int16_t& anti_matter_resistance_percent();

			void set_scripted_response(std::string r) {
				m_scripted_response = r;
			}
			std::string consume_scripted_response() {
				if(m_scripted_response.length() == 0) {
					return "";
				}
				auto f = std::move(m_scripted_response);
				m_scripted_response = "";
				return std::move(f);
			}
			const std::array<obj_ptr_t,NUM_WEARS>& equipment() const {
				return m_equipment;
			}
			void start_contract(const contract_vnum_t& contract_vnum);
			void stop_contract(const contract_vnum_t& contract_vnum);
			bool has_contract() const {
				return m_contract_size;
			}
			void contract_give_item(obj_ptr_t& object,player_ptr_t& mob);
			void contract_find_item(obj_ptr_t& object);
			void contract_find_mob(player_ptr_t& mob);
			void contract_find_room(const room_rnum& room_id);
			void contract_find_door(const room_rnum& room_id,const int8_t& direction);
			void contract_destroy_item(obj_ptr_t& object);
			void contract_destroyed_door(const room_rnum& room_id,const int8_t& direction);
			void contract_retrieve_item(obj_ptr_t& object);
			void contract_quota_item_find_increase(obj_ptr_t& object);
			void contract_quota_kill_mob_increase(player_ptr_t& mob);
			void contract_quota_destroyed_door(const room_rnum& room_id,const int8_t& direction);
			void contract_kill_mob(player_ptr_t& mob);
			void contract_gain_entry(const room_rnum& room_id);
			void contract_talk_to(player_ptr_t& mob);
			void contract_install_item(const uuid_t& object_uuid);
			void queue_up(std::string_view);
			bool& moving_to_room();
			void update_contract_status();
			contract_list_t& contracts();

			/** TODO: power this by user preferences */
			std::size_t screen_width() const {
				return 80;
			}
		protected:
			std::map<std::string,std::string> m_ada_data;
			bool m_ada;
			mods::string m_class_string;
			std::array<bool,misc_pref_enum_t::SIZE> m_misc_pref;
			void m_sync_equipment();
			void perform_equip_calculations(int pos,bool equip);
			lense_type_t m_lense_type;
			uint8_t m_overhead_map_width;
			uint8_t m_overhead_map_height;
			char_data*   m_char_data;
			std::shared_ptr<char_data> m_shared_ptr;
			//std::deque<std::shared_ptr<obj_data>> m_carrying;
			std::vector<affected_type> m_affected_by;
			bool m_moving_to_room;

		private:
			std::string m_scripted_response;
			void write_to_char(std::string_view msg, bool newline,bool plain);
			uuid_t m_attacker_uuid;
			event_queue_iterator m_block_event;
			bool m_has_block_event;
			std::unordered_map<uint32_t,uuid_t> m_block_data;
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
			bool         m_weapon_cooldown_expired;
			weapon_set   m_weapon_set;
			bool         m_do_paging;
			bool         m_capture_output;
			std::string  m_captured_output;
			unsigned int m_page;
			unsigned int m_current_page;
			std::string  m_current_page_fragment;
			std::vector<std::string> m_pages;
			std::shared_ptr<player_special_data> m_player_specials;
			time_type_t	m_time;
			player_type_enum_t m_type;
			weapon_type_t m_weapon_type;
			weapon_type_t m_weapon_flags;
			std::shared_ptr<mods::classes::sniper> m_sniper;
			std::shared_ptr<mods::classes::marine> m_marine;
			std::shared_ptr<mods::classes::breacher> m_breacher;
			std::shared_ptr<mods::classes::engineer> m_engineer;
			std::shared_ptr<mods::classes::ghost> m_ghost;
			std::shared_ptr<mods::classes::medic> m_medic;
			std::shared_ptr<mods::classes::support> m_support;

			std::shared_ptr<mods::classes::pyrexia> m_pyrexia;
			std::shared_ptr<mods::classes::forge> m_forge;
			std::shared_ptr<mods::classes::syndrome> m_syndrome;
			//std::shared_ptr<mods::classes::striker> m_striker;
			FILE* m_histfile_fp;
			bool m_histfile_on;
			uint32_t m_histfile_index;
			affect_dissolver_t m_affects;
			std::array<obj_ptr_t,NUM_WEARS> m_equipment;
			bool m_quitting;
			std::shared_ptr<mods::camera> m_camera;
			bool m_camera_viewing;
			uint32_t m_blocked_until;
			obj_ptr_t m_attacking_with;
			std::map<int,uint16_t> m_skills;
			std::map<damage_event_t,damage_event_callback_t> m_damage_event_callbacks;
			direction_t m_watching;
			std::shared_ptr<mods::armor::basic_protection> m_basic_protection;
			std::shared_ptr<mods::armor::advanced_protection> m_advanced_protection;
			std::shared_ptr<mods::armor::elite_protection> m_elite_protection;
			uint32_t m_currently_hacking;
			uint8_t m_hacking_row;
			std::map<int,rate_limit_data_t> m_rate_limits;
			std::array<uint8_t,5> m_triads;
			int16_t m_incendiary_resistance_percent;
			int16_t m_explosive_resistance_percent;
			int16_t m_shrapnel_resistance_percent;
			int16_t m_corrosive_resistance_percent;
			int16_t m_cryogenic_resistance_percent;
			int16_t m_radiation_resistance_percent;
			int16_t m_emp_resistance_percent;
			int16_t m_shock_resistance_percent;
			int16_t m_anti_matter_resistance_percent;
			std::forward_list<std::shared_ptr<mods::contracts::player_contract_instance>> m_contracts;
			std::deque<std::shared_ptr<mods::rifle_attachments_t>> m_rifles;
			int16_t m_incendiary_damage_percent;
			int16_t m_explosive_damage_percent;
			int16_t m_shrapnel_damage_percent;
			int16_t m_corrosive_damage_percent;
			int16_t m_cryogenic_damage_percent;
			int16_t m_radiation_damage_percent;
			int16_t m_emp_damage_percent;
			int16_t m_shock_damage_percent;
			int16_t m_anti_matter_damage_percent;
			uint8_t m_contract_size;
			bool m_contract;
			bool m_can_attack;
			mods::util::stopwatch_t m_timer;
			int m_cooldown_ticks;
	};
};

#include "util.hpp"


#endif
