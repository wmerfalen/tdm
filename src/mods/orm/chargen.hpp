#ifndef __MENTOC_MODS_ORM_CHARGEN_HEADER__
#define __MENTOC_MODS_ORM_CHARGEN_HEADER__

#include <iostream>
#include <vector>
#include "../player.hpp"
#include "../../structs.h"
#include "../pq.hpp"
#include "../sql.hpp"

namespace mods::orm {
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	void seed_player_classes();
	void seed_player_races();
	void seed_player_race_perks();
	void seed_skill_trees();
	void seed_specialty_skills();
	
	struct player_classes {
		void set_class(player_class_t pclass);
		uint64_t id;
		std::string pclass_name;
		std::string pclass_description;
	};

	struct player_races {
		uint64_t id;
		std::string prace_name;
		std::string prace_description;
	};
	const std::string base_attributes[] = {"STR","INT","DEX","WIS","CHA","CON","LCK"};
	struct player_race_perks {
		uint64_t id;
		uint64_t prperk_id;// INTEGER NOT NULL REFERENCES player_races(id),
		std::shared_ptr<player_races> prperk_id_ptr;
		std::string prperk_name;//VARCHAR(64) NOT NULL,
		std::string prperk_description;//TEXT,
		std::string prperk_base_attribute_mutate; //base_attribute NOT NULL,
		std::string prperk_base_attribute_amount_add; // INTEGER NOT NULL DEFAULT 0
	};
	struct skill_trees {
		uint64_t id;//SERIAL PRIMARY KEY,
		uint64_t sktree_class;//INTEGER NOT NULL REFERENCES player_classes(id),
		std::shared_ptr<player_classes> sktree_class_ptr;
		std::string sktree_skill_name;//VARCHAR(32) NOT NULL,
		std::string sktree_skill_description;//TEXT,
		uint64_t sktree_skill_level_unlock;//INTEGER NOT NULL,
		uint64_t sktree_skill_parent_id;//INTEGER REFERENCES skill_trees(id),
		std::shared_ptr<skill_trees> sktree_skill_parent_id_ptr;//INTEGER REFERENCES skill_trees(id),
		uint64_t sktree_skill_order;//INTEGER NOT NULL DEFAULT 0
	};
	struct specialty_skills {
		uint64_t id;
		std::string spskill_name;//VARCHAR(64) NOT NULL,
		std::string spskill_description;//TEXT
	};
	struct player_generated_data {
		uint64_t id;//SERIAL PRIMARY KEY,
		uint64_t pgdata_player_id;//INTEGER NOT NULL REFERENCES player(id),
		std::shared_ptr<mods::player> pgdata_player_id_ptr;//INTEGER NOT NULL REFERENCES player(id),
		uint64_t pgdata_class;//INTEGER NOT NULL REFERENCES player_classes(id),
		std::shared_ptr<player_classes> pgdata_class_ptr;//INTEGER NOT NULL REFERENCES player_classes(id),
		uint64_t pgdata_race;//INTEGER NOT NULL REFERENCES player_races(id),
		std::shared_ptr<player_races> pgdata_race_ptr;// INTEGER NOT NULL REFERENCES player_races(id),
		uint64_t pgdata_race_perks;//INTEGER NOT NULL REFERENCES player_race_perks(id),
		std::shared_ptr<player_race_perks> pgdata_race_perks_ptr;//INTEGER NOT NULL REFERENCES player_race_perks(id),
		uint64_t pgdata_specialty_skill;//INTEGER REFERENCES specialty_skills(id)
		std::shared_ptr<specialty_skills> pgdata_specialty_skill_ptr;// INTEGER REFERENCES specialty_skills(id)
	};
	struct player_skill_trees_unlocked {
		uint64_t id;//SERIAL PRIMARY KEY,
		uint64_t pstun_player_id;//INTEGER NOT NULL REFERENCES player(id),
		std::shared_ptr<mods::player> stun_player_id_ptr;//INTEGER NOT NULL REFERENCES player(id),
		uint64_t pstun_skill_tree_id;//INTEGER NOT NULL REFERENCES skill_trees(id)
		std::shared_ptr<skill_trees> pstun_skill_tree_id_ptr;// INTEGER NOT NULL REFERENCES skill_trees(id)
	};
};

#endif
