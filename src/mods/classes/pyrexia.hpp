#ifndef __MENTOC_MODS_CLASSES_GHOST_HEADER__
#define  __MENTOC_MODS_CLASSES_GHOST_HEADER__
#include <variant>
#include "../orm/pyrexia.hpp"
#include "base.hpp"
#include "../skills.hpp"

using pyrexia_orm_t = mods::orm::pyrexia;
namespace mods::classes {
	struct pyrexia : base {
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
			mods::skills::skillset_level_t level;
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

		void set_player(player_ptr_t);

		std::pair<bool,std::string> throw_fire_nade(uint8_t direction,uint8_t depth);
		std::pair<bool,std::string> drench_object_in_gasoline(uuid_t npc);
		std::pair<bool,std::string> drench_room_in_gasoline(uuid_t npc);
		std::pair<bool,std::string> convert_to_fire_nade(obj_ptr_t& object);
		std::pair<bool,std::string> attach_incendiary_grenade_launcher(obj_ptr_t& weapon);
		std::pair<bool,std::string> attach_flamethrower(obj_ptr_t& weapon);
		std::pair<bool,std::string> create_wall_of_fire(uint8_t direction);
		std::pair<bool,std::string> seal_off_room_with_wall_of_fire(room_rnum room_id);

		/** passive ability */
		bool roll_chance_for_incendiary_shot(obj_ptr_t& attacking_weapon);

		/** database routines */
		int16_t load_by_player(player_ptr_t&);
		int16_t new_player(player_ptr_t&);
		int64_t db_id() const;
		int16_t save();
		private:
			pyrexia_orm_t	m_orm;
			player_ptr_t m_player;
	};
	std::shared_ptr<mods::classes::pyrexia> create_pyrexia(player_ptr_t &player);
};


#endif
