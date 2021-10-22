#ifndef __MENTOC_MODS_WEAPONS_attachment_underbarrel_HEADER__
#define __MENTOC_MODS_WEAPONS_attachment_underbarrel_HEADER__

#include "../weapon.hpp"
#include "../pqxx-types.hpp"

namespace mods::weapons {
	struct attachment_underbarrel {
			attachment_underbarrel();
			~attachment_underbarrel();

			virtual void init();
			virtual void consume_ammo();

			void set_yaml_file(std::string_view y);
			std::string_view get_yaml_file();
			obj_ptr_t obj();
			bool is_attached() const;
			uint16_t ammo();

		protected:
			std::string m_yaml_file;
			obj_ptr_t m_obj;
			bool m_attached;
	};
};
#endif

