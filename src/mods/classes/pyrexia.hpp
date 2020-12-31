#ifndef __MENTOC_MODS_CLASSES_PYREXIA_HEADER__
#define  __MENTOC_MODS_CLASSES_PYREXIA_HEADER__
#include <variant>
#include "../orm/pyrexia.hpp"
#include "base.hpp"
#include "../skills.hpp"
#include "../super-users.hpp"

namespace mods::classes {
	struct super_user_fiddler;
};
using pyrexia_orm_t = mods::orm::pyrexia;
namespace mods::classes {
	struct pyrexia : base {
			friend class mods::classes::super_user_fiddler;
			enum skillset_t : uint8_t {
				THROW_FIRE_NADE = 0,
				DOUSE_OBJECT,
				DOUSE_ROOM,
				CONVERT_TO_FIRE_NADE,
				ATTACH_LAUNCHER,
				ATTACH_THROWER,
				FIREWALL,
				LETHAL_ROOM_SEALANT,
				__FIRST = THROW_FIRE_NADE,
				__LAST = LETHAL_ROOM_SEALANT
			};
			struct skillset_info_t {
				skillset_t skill;
				std::string name;
				std::string description;
			};

			std::list<skillset_info_t> skills;
			static int16_t destroy(player_ptr_t& player);
			types kind() {
				return types::PYREXIA;
			}

			/* constructors and destructors */
			pyrexia();
			pyrexia(player_ptr_t);
			~pyrexia() = default;

			player_ptr_t 	player();
			void replenish();

			void set_player(player_ptr_t);

			std::pair<bool,std::string> throw_fire_nade(uint8_t direction,uint8_t depth);
			std::pair<bool,std::string> drench_object_in_gasoline(uuid_t npc);
			std::pair<bool,std::string> drench_room_in_gasoline(uuid_t npc);
			std::pair<bool,std::string> convert_to_fire_nade(obj_ptr_t& object);
			std::pair<bool,std::string> attach_incendiary_grenade_launcher(obj_ptr_t& weapon);
			std::pair<bool,std::string> attach_flamethrower(obj_ptr_t& weapon);
			std::pair<bool,std::string> create_wall_of_fire(uint8_t direction);
			std::pair<bool,std::string> seal_off_room_with_wall_of_fire(room_rnum room_id);

			void replenish_charge();

			/** passive ability */
			uint16_t roll_incendiary_shot(obj_ptr_t& attacking_weapon, uuid_t target);
			uint16_t roll_heat_accumulation_hp_gain(obj_ptr_t& attacking_weapon);

			/** database routines */
			int16_t load_by_player(player_ptr_t&);
			int16_t new_player(player_ptr_t&);
			int64_t db_id() const;
			int16_t save();
		private:
			skill_familiarity_t m_fire_nade_level;
			skill_familiarity_t m_drench_object_in_gasoline_level;
			skill_familiarity_t m_drench_room_in_gasoline_level;
			skill_familiarity_t m_convert_to_fire_nade_level;
			skill_familiarity_t m_attach_incendiary_grenade_launcher_level;
			skill_familiarity_t m_attach_flamethrower_level;
			skill_familiarity_t m_create_wall_of_fire_level;
			skill_familiarity_t m_seal_off_room_with_wall_of_fire_level;
			uint8_t m_fire_nade_charges;
			pyrexia_orm_t	m_orm;
			player_ptr_t m_player;
	};
	std::shared_ptr<mods::classes::pyrexia> create_pyrexia(player_ptr_t& player);
};


#endif
