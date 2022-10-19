#ifndef __MENTOC_MODS_CLASSES_CONTAGION_HEADER__
#define  __MENTOC_MODS_CLASSES_CONTAGION_HEADER__
#include "../orm/contagion.hpp"
#include "super-user-fiddler.hpp"
#include "base.hpp"

using contagion_orm_t = mods::orm::contagion;
namespace mods::classes {
	struct contagion : base {
			bool has_mana_for_skill(uint16_t skill) {
				return true;
			}
			void use_mana_for_skill(uint16_t skill) {
			}

			static constexpr std::string_view PATHOGEN_AMMUNITION_YAML = "pox-ordinance-pathogen-ammunition.yml";
			friend class mods::classes::super_user_fiddler;

			//using shotgun_ub_t = mods::weapons::attachment_shotgun_underbarrel;
			//using frag_ub_t = mods::weapons::attachment_frag_underbarrel;
			//using adrenaline_shot_t = mods::boosters::adrenaline_shot;

			enum ability_t {
				NONE = 0,
				DETONATE_LIMB,
				SUFFERING_SHOT,

				PATHOGEN_AMMUNITION,
				GRIM_AURA,
				MELT,
				SUFFOCATE,
				HELLFIRE_INCANTATION,

				SHREDDED_CANTRIP,
				MUSCLE_MEMORY,

				HELLFIRE_CIRCLE,
				PARTICLE_DECELERATION,
				GHASTLY_DOUBLE,
				MINOR_SHIELDING,
				CURSED_BALLISTICS,
				NEUTRON_SHIELD,
				BLADED_ARRAY,

				ROOTS_OF_MAYHEM,
				MORBID_DOUBT,
				INTIMIDATE,
				FORCE_OUT,
				ATTRACT,

				CORPSE_EXPLOSION,
				SHRAPNEL_CORPSE_EXPLOSION,
				CORROSIVE_CORPSE_EXPLOSION,
				HELLFIRE_CORPSE_EXPLOSION,

				LIFE_TAP,
				LEECH,
				PARASITIC_CORPSE_TAP,
				EXTRACT_ORGANS,

				RECRUIT,
				DEMONIC_INCANTATION,

				SHADOW_SIGHT,
				MORBID_INSIGHT,
				CONFUSE,
				DRAG_CORPSE,
			};
			std::vector<ability_data_t>& get_abilities() override {
				return m_abilities;
			}

			player_ptr_t get_player_ptr() override {
				return m_player;
			}

			long created_at;

			static int16_t destroy(player_ptr_t& player);
			types kind() {
				return types::CONTAGION;
			}

			/* constructors and destructors */
			contagion();
			contagion(player_ptr_t);
			~contagion() = default;
			void init();

			void set_player(player_ptr_t);

			/** will return how many turns the attacker will be distracted until it re-engages the player */
			std::pair<int16_t,std::string> attract(direction_t);
			void intimidate(uuid_t);
			std::pair<int16_t,std::string> attract();
			std::tuple<bool,std::string> intimidate_target(uuid_t npc_uuid);

			std::pair<int16_t,std::string> cast_corpse_explosion(obj_ptr_t& corpse);
			std::pair<int16_t,std::string> cast_hellfire_corpse_explosion(obj_ptr_t& corpse);
			std::pair<int16_t,std::string> cast_shrapnel_corpse_explosion(obj_ptr_t& corpse);
			std::pair<int16_t,std::string> cast_minor_shielding();
			std::tuple<int16_t,std::string> drag_corpse(obj_ptr_t& corpse,const direction_t& direction);

			/** database routines */
			int16_t load_by_player(player_ptr_t&);
			int16_t new_player(player_ptr_t&);
			int64_t db_id() const;
			int16_t save();

			void replenish();

		private:
			void replenish_notify(std::string_view);
			uuid_t m_target;
			uint16_t m_xray_shot_charges;
			player_ptr_t m_player;
			std::map<std::string,bool> m_preferences;

			skill_t m_life_tap;
			skill_t m_corpse_explosion;
			skill_t m_attract;
			skill_t m_confuse;
			skill_t m_hellfire_incantation;
			skill_t m_detonate_limb;
			skill_t m_suffering_shot;
			skill_t m_shrapnel_corpse_explosion;
			skill_t m_corrosive_corpse_explosion;
			skill_t m_hellfire_corpse_explosion;

			skill_t m_drag_corpse;

			skill_t m_pathogen_ammunition;
			skill_t m_grim_aura;
			skill_t m_melt;
			skill_t m_suffocate;
			skill_t m_shredded_cantrip;
			skill_t m_muscle_memory;
			skill_t m_hellfire_circle;
			skill_t m_particle_deceleration;
			skill_t m_ghastly_double;
			skill_t m_minor_shielding;
			skill_t m_cursed_ballistics;
			skill_t m_neutron_shield;
			skill_t m_bladed_array;
			skill_t m_roots_of_mayhem;
			skill_t m_morbid_doubt;
			skill_t m_intimidate;
			skill_t m_force_out;
			skill_t m_leech;
			skill_t m_parasitic_corpse_tap;
			skill_t m_extract_organs;
			skill_t m_recruit;
			skill_t m_demonic_incantation;
			skill_t m_shadow_sight;
			skill_t m_morbid_insight;
			ability_list_t m_abilities;
			contagion_orm_t	m_orm;
	};
	std::shared_ptr<mods::classes::contagion> create_contagion(player_ptr_t& player);
};

#endif
