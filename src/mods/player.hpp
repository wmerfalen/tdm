#ifndef __MENTOC_MODS_PLAYER_HEADER__
#define  __MENTOC_MODS_PLAYER_HEADER__

#include <iostream>
#include <sys/socket.h>
#include <netdb.h>
#include <vector>

namespace mods { class player; };
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

#define WEAPON_SET_NUM 1
extern size_t send_to_char(struct char_data *ch, const char *messg, ...);
namespace mods {
	namespace weapon {
		enum mask_type { SMG, SNIPE, SHOTGUN, GRENADE };
	};
    class player {
		public:
			using   class_type = mods::classes::types;
			typedef short weapon_set;
			typedef std::vector<class_type> class_capability_t;
			typedef std::vector<std::unique_ptr<mods::classes::base>> class_info_t;
			using   chdata = struct char_data;
			using   chdata_ptr = struct char_data *;

			/* constructors and destructors */
			player  () = delete;
			player  (char_data* ch);
			player  (mods::player* p);
			~player () = default;

			static constexpr int PAGE_SIZE = 40;

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

			/* class info */
			std::unique_ptr<mods::classes::base>& get_class(class_type);

			/* ammo gettters */
			obj_data* get_first_ammo_of_type(const weapon_type_t&) const;

			/* combat functions */
			bool can_snipe(char_data *target);
			bool is_weapon_loaded();
			bool carrying_ammo_of_type(const weapon_type_t &);
			void ammo_adjustment(int);
			int  ammo_type_adjustment(int,const weapon_type_t&);

			/* setters */
			void set_class_capability(const class_capability_t & caps);
			void set_cd(char_data* ch){ m_char_data = ch; }
			void set_shared_ptr(std::shared_ptr<mods::player>&);

			/* informational functions */
			const char*    name()       { return m_char_data->player.name; }
			uuid_t         uuid() const { return m_char_data->uuid; }
			char_data*     cd()   const { return m_char_data; }
			obj_data*      weapon();
			obj_data*      get_ammo(const weapon_type_t &);

			/* conversion operator to char_data* */
			operator chdata_ptr() const {
				return m_char_data;
			}

			/* weapon cooldown functions */
			void weapon_cooldown_start    (unsigned long duration,weapon_set set);
			bool weapon_cooldown_expired  (weapon_set);


			/* communication functions */
			void stc_room       (const room_rnum &);
			void stc_room_desc  (const room_rnum &);
			void exits          ();
			void stc            (const char* m);
			void stc            (const std::string m);
			void stc            (int m);

			/* pager functions */
			player&             pager_start();
			player&             pager_end();
			void                pager_next_page();
			void                page(int page_number);
			void                queue_page_fragment(std::string_view fragment){
				for(auto c : fragment){
					m_current_page_fragment += c;
					if(c == '\n'){
						m_pages.push_back(m_current_page_fragment);
						m_current_page_fragment = "";
					}
				}
			}
			player& operator<<(const char* m){
				if(m_do_paging){
					queue_page_fragment(m);
				}else{
					stc(m);	
				}
				return *this;
			}
			player& operator<<(const std::string m){
				if(m_do_paging){
					queue_page_fragment(m);
				}else{
					stc(m);	
				}
				return *this;
			}
			player& operator<<(int m){
				if(m_do_paging){
					queue_page_fragment(std::to_string(m));
				}else{
					stc(m);	
				}
				return *this;
			}
			void pager_clear();
			bool paging() const { return m_pages.size(); }

			/* captured output */
			void capture_output(bool capture_status);
			std::string_view get_captured_output();
			void clear_captured_output();
		private:
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
			std::shared_ptr<mods::player> m_self_ptr;
    };
};

#include "util.hpp"

#endif
