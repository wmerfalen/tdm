#ifndef __MENTOC_MODS_WEAPONS_SHOTGUN_DST7A_HEADER__
#define __MENTOC_MODS_WEAPONS_SHOTGUN_DST7A_HEADER__

#include "../weapon.hpp"
#include "../pqxx-types.hpp"
#include <string_view>

namespace mods::weapons::shotgun {
	struct dst7A {
			static constexpr std::string_view description =
			    "  ______________________________________________________________________\r\n"
			    " /                                                                     /\r\n"
			    "/                                                                     /\r\n"
			    "| .  .  .  .  .  .  .  . .  .  .  . .  .  .  . .  .  .  . .  .  .  . /,\r\n"
			    "| Weapon: The DST7A                                                   |\r\n"
			    "|                                                                     |\r\n"
			    "+-[ Type: SHOTGUN ]--:[ MOD_TYPE: Radiation+Incendiary ]:-------------+\r\n"
			    "|                                                                     |\r\n"
			    "| Manufacturer: HARM TECH                                             |\r\n"
			    "| Acronym: [D]e[s][t]abilize [A]lpha ([7] round clips)                |\r\n"
			    "|                                                                     |\r\n"
			    "| Description:                                                        |\r\n"
			    "| The DST7A was designed with cruelty in mind. The DST7A is a semi-   |\r\n"
			    "| automatic shotgun that deals more than just shrapnel damage to it's |\r\n"
			    "| intended targets. A proprietary mechanism within the barrell of the |\r\n"
			    "| shotgun causes a chemical reaction which coats each shell as it     |\r\n"
			    "| travels towards it's target. A cruel mixture of radioactive isotopes|\r\n"
			    "| and chemicals used in pyrotechnics taint the shrapnel as it leaves  |\r\n"
			    "| the barrell. Upon hitting flesh, the mixture causes the flesh and   |\r\n"
			    "| bone of the target to effectively melt away into uselessness.       |\r\n"
			    "| occurs and the flesh of the target begins to melt away.             |\r\n"
			    "|                                                                     |\r\n"
			    "| Initial testing of the DST7A caused many QA engineers to fall ill to|\r\n"
			    "| radiation sickness. As of yet, the DST7A is considered HIGHLY EXPER-|\r\n"
			    "| IMENTAL. While production of the DST7A halted, the blueprints have  |\r\n"
			    "| since been copied and the shotgun has survived in the black market. |\r\n"
			    "|                                                                     |\r\n"
			    "+-[ Known stats ]-----------------------------------------------------|\r\n"
			    "| 8d13  - Base damage                                                 |\r\n"
			    "| +150% - Incendiary damage                                           |\r\n"
			    "| +250% - Radioactive damage                                          |\r\n"
			    "| 10d10 - Melt damage per 9 ticks per shot                            |\r\n"
			    "| ????? - Bone marrow damage                                          |\r\n"
			    "| ????? - Kevlar damage                                               |\r\n"
			    "| 1d6   - Reload time                                                 |\r\n"
			    "|                                                                     |\r\n"
			    "+-[ R: 001 ]---------------------------------------[ Docu-gen v8.1.0 ]+\r\n";
			static obj_data_ptr_t make();
			static obj_data_ptr_t feed_by_file(std::string_view file);
			dst7A();
			~dst7A();

			uint16_t bound_to;
			uint16_t id;
			void feed(const mods::pq::row_type&);
			obj_data_ptr_t obj;
			void increment_shot();
			void set_target(const uuid_t& target_uuid) {
				m_target_uuid = target_uuid;
			}
			uuid_t& get_target_uuid() {
				return m_target_uuid;
			}
			uint16_t base_damage();
			uint16_t incendiary_damage();
			uint16_t radioactive_damage();
			uint16_t melt_damage();
			uint16_t bone_marrow_damage();
			uint16_t kevlar_damage();
			uint8_t reload_time();
		protected:
			uint8_t m_knockback_charges;
			uint32_t m_damage_accumulation;
			uuid_t m_target_uuid;
	};
};
using dst7A_ptr_t = std::shared_ptr<mods::weapons::shotgun::dst7A>;
#endif

