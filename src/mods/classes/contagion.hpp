#ifndef __MENTOC_MODS_CLASSES_CONTAGION_HEADER__
#define  __MENTOC_MODS_CLASSES_CONTAGION_HEADER__
#include "../orm/contagion.hpp"
#include "super-user-fiddler.hpp"
#include "base.hpp"

using contagion_orm_t = mods::orm::contagion;
namespace mods::classes {
	struct contagion : base {
			friend class mods::classes::super_user_fiddler;

			//using shotgun_ub_t = mods::weapons::attachment_shotgun_underbarrel;
			//using frag_ub_t = mods::weapons::attachment_frag_underbarrel;
			//using adrenaline_shot_t = mods::boosters::adrenaline_shot;

			enum ability_t {
				NONE = 0,
				LIFE_TAP,
				CORPSE_EXPLOSION,
				SHRAPNEL_CORPSE_EXPLOSION,
				CORROSIVE_CORPSE_EXPLOSION,
				HELLFIRE_CORPSE_EXPLOSION,
				ATTRACT,
				CONFUSE,
				HELLFIRE_INCANTATION,
				DETONATE_LIMB,
				SUFFERING_SHOT,

			};
			std::vector<ability_data_t>& get_abilities() {
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

			/** database routines */
			int16_t load_by_player(player_ptr_t&);
			int16_t new_player(player_ptr_t&);
			int64_t db_id() const;
			int16_t save();

			void replenish();

		private:
			void replenish_notify(std::string_view);

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

			ability_list_t m_abilities;
			contagion_orm_t	m_orm;
	};
	std::shared_ptr<mods::classes::contagion> create_contagion(player_ptr_t& player);
};

#endif
