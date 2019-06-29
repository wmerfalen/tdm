#ifndef __MENTOC_MODS_OBJECT_HEADER__
#define  __MENTOC_MODS_OBJECT_HEADER__

#include <iostream>
#include <vector>

#include "../structs.h"
#include "../types.hpp"
#include "weapon.hpp"
namespace mods {
	struct object {
		public:
			enum object_type_enum_t { 
				OBJECT = 0,
				/** These are for the constants that the mods/builder class
				 * is referencing. I'm not 100% happy with these values, so
				 * for the time being we will have this here, but in the future
				 * I'm likely to remove them in favor of our other enumerated
				 * values.
				 */
				__BROAD_CATEGORY_START__,
				WEAPON,
				CONTAINER,
				SENTINEL,
				__BROAD_CATEGORY_END__,
				__WEAPON_START__,
				/** Types of melee weapons */
				__MELEE_WEAPON_START__,
				KNIFE,
				MACHETE,
				SLEDGEHAMMER,
				SHOCK,
				__MELEE_WEAPON_END__,
				/** End types of melee weapons */
				/** Types of rifles */
				SNIPER_RIFLE,
				ASSAULT_RIFLE,
				LIGHT_MACHINE_GUN,
				SHOTGUN,
				FLAMETHROWER,
				SUB_MACHINE_GUN,
				__WEAPON_END__,
				/** END types of rifles */
				/** Generic qualifiers. "AUTOMATIC" can apply to shotguns too */
				__GENERIC_QUALIFIERS_START__,
				AUTOMATIC,
				SHORT_RANGE,
				LONG_RANGE,
				__GENERIC_QUALIFIERS_END__,
				/** END generic qualifiers */
				/** Armor positions */
				__ARMOR_POSITION_START__,
				GAUNTLETS,
				LEGS,
				SHOULDER,
				VEST,
				HELMET,
				__ARMOR_POSITION_END__,
				/** END Armor positions */
				/** Armor has brands which are tiered */
				__ARMOR_BRANDS_START__,
				BALLISTIC,
				RAVEN_RECON,
				VIPER_RECON,
				CYCLONE_RECON,
				DIAMONDBACK_COMBAT,
				TITAN_COMBAT,
				VULTURE_COMBAT,
				ANACONDA_ASSAULT,
				FALCON_ASSAULT,
				GOLIATH_ASSAULT,
				HURRICANE_ASSAULT,
				__ARMOR_BRANDS_END__,
				/** END armor brands */
				FIRST = OBJECT,
				LAST = __ARMOR_BRANDS_END__
			};
			using type = object_type_enum_t;

			/* constructors and destructors */
			~object();
			object();
			void init();
			object& add_type(object_type_enum_t);
			object& remove_type(object_type_enum_t);
			std::shared_ptr<obj_data> get_shared_ptr();
			object& set_shared_ptr(std::shared_ptr<obj_data>&);
			object(object*);
		private: 
			std::shared_ptr<obj_data> m_shared_ptr;
			std::vector<object_type_enum_t> m_type;
	};
};

#include "util.hpp"


#endif
