#include "chargen.hpp"
#include <vector>
#include <map>
#include <string>

namespace mods::orm {
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	int16_t player_classes::feed(const pqxx::result::reference & result){
			id = static_cast<uint64_t>(result["id"].as<uint64_t>());
			//MENTOC_ORM_STR(pclass_name);
			//MENTOC_ORM_STR(pclass_description);
			return 0;
	}
	int16_t player_races::feed(const pqxx::result::reference & result){
			id = static_cast<uint64_t>(result["id"].as<uint64_t>());
			//MENTOC_ORM_STR(prace_name);
			//MENTOC_ORM_STR(prace_description);
			return 0;
	}
	/*
	int16_t player_races::feed(const pqxx::result::reference & result){
			id = static_cast<uint64_t>(result["id"].as<uint64_t>());
			//MENTOC_ORM_STR(prace_name);
			//MENTOC_ORM_STR(prace_description);
			return 0;
	}
	*/
	int16_t player_race_perks::feed(const pqxx::result::reference & result){
			id = static_cast<uint64_t>(result["id"].as<uint64_t>());
			prperk_id = static_cast<uint64_t>(result["prperk_id"].as<uint64_t>());
			//MENTOC_ORM_STR(prperk_name);
			//MENTOC_ORM_STR(prperk_description);
			//MENTOC_ORM_STR(prperk_base_attribute_mutate);
			//MENTOC_ORM_STR(prperk_base_attribute_amount_add);
			return 0;
	}
	int16_t skill_trees::feed(const pqxx::result::reference & result){
		id = static_cast<uint64_t>(result["id"].as<uint64_t>());
		sktree_class = static_cast<uint64_t>(result["sktree_class"].as<uint64_t>());
		/** TODO: FIXME: std::shared_ptr<player_classes> sktree_class_ptr; */
		std::string sktree_skill_name;//VARCHAR(32) NOT NULL,
		//MENTOC_ORM_STR(sktree_skill_name);
		//MENTOC_ORM_STR(sktree_skill_description);
		//MENTOC_ORM_UINT(sktree_skill_level_unlock);//INTEGER NOT NULL,
		//MENTOC_ORM_UINT(sktree_skill_parent_id);//INTEGER REFERENCES skill_trees(id),
		//MENTOC_ORM_UINT(sktree_skill_order);//INTEGER NOT NULL DEFAULT 0
		/** TODO: FIXME: std::shared_ptr<skill_trees> sktree_skill_parent_id_ptr;//INTEGER REFERENCES skill_trees(id), */
		return 0;
	}
	int16_t specialty_skills::feed(const pqxx::result::reference & result){
		//MENTOC_ORM_UINT(id);
		//MENTOC_ORM_STR(spskill_name);//VARCHAR(64) NOT NULL,
		//MENTOC_ORM_STR(spskill_description);//TEXT
		return 0;
	};
	int16_t player_generated_data::feed(const pqxx::result::reference & result){
		//MENTOC_ORM_UINT(id);
		//MENTOC_ORM_UINT(pgdata_player_id);//INTEGER NOT NULL REFERENCES player(id),
		//MENTOC_ORM_UINT(pgdata_class);//INTEGER NOT NULL REFERENCES player_classes(id),
		//MENTOC_ORM_UINT(pgdata_race);//INTEGER NOT NULL REFERENCES player_races(id),
		//MENTOC_ORM_UINT(pgdata_race_perks);//INTEGER NOT NULL REFERENCES player_race_perks(id),
		//MENTOC_ORM_UINT(pgdata_specialty_skill);//INTEGER REFERENCES specialty_skills(id)
		/** TODO FIXME 
		player_ptr_t pgdata_player_id_ptr;//INTEGER NOT NULL REFERENCES player(id),
		std::shared_ptr<player_classes> pgdata_class_ptr;//INTEGER NOT NULL REFERENCES player_classes(id),
		std::shared_ptr<player_races> pgdata_race_ptr;// INTEGER NOT NULL REFERENCES player_races(id),
		std::shared_ptr<player_race_perks> pgdata_race_perks_ptr;//INTEGER NOT NULL REFERENCES player_race_perks(id),
		std::shared_ptr<specialty_skills> pgdata_specialty_skill_ptr;// INTEGER REFERENCES specialty_skills(id)
		*/
		return 0;
	}
	int16_t player_skill_trees_unlocked::feed(const pqxx::result::reference & result){
		//MENTOC_ORM_UINT(id);//SERIAL PRIMARY KEY,
		//MENTOC_ORM_UINT(pstun_player_id);//INTEGER NOT NULL REFERENCES player(id),
		//MENTOC_ORM_UINT(pstun_skill_tree_id);//INTEGER NOT NULL REFERENCES skill_trees(id)
		/** TODO FIXME 
		player_ptr_t stun_player_id_ptr;//INTEGER NOT NULL REFERENCES player(id),
		std::shared_ptr<skill_trees> pstun_skill_tree_id_ptr;// INTEGER NOT NULL REFERENCES skill_trees(id)
		*/
		return 0;
	}
		void player_classes::set_class(player_class_t pclass){
			switch(pclass){
				case CLASS_ENGINEER:
					pclass_name = "ENGINEER";
					break;
				case CLASS_MEDIC:
					pclass_name = "MEDIC";
					break;
				case CLASS_MARKSMAN:
					pclass_name = "MARKSMAN";
					break;
				case CLASS_PSYOP:
					pclass_name = "PSYOP";
					break;
				case CLASS_SUPPORT:
					pclass_name = "SUPPORT";
					break;
				case CLASS_MARINE:
					pclass_name = "MARINE";
					break;
				default:
					pclass_name = "<unknown>";
					break;
			}
		}
	void seed_player_races() {
			for(auto mapped_values : std::vector<std::map<std::string,std::string>>{
				{
					{"name","British S.A.S."},
					{"description","Formed during World War 2, the British Special Air Service is the British's most renowned special forces unit in operation today. It operates as a beacon of light and ultimate template for the ideal special forces orgranization. "}
				},
				{
					{"name","Navy Seals"},
					{"description","Characterized by it's use of unconventional warfare and operations under extreme conditions, the Seals are superbly trained in all aspects of warfare."},
				},
				{
					{"name","Russian Spetsnaz"},
					{"description","Covertly and silently operates in the dead of night. Highly specialized as the eyes and ears of the Russian Military Complex."},
				},
				{
					{"name","Polish GROM"},
					{"description","Poland's most well-known military unit. It's name chosen in homage to the battle-tested Polish soldiers of World War 2. The phrase \"GROM\" means thunder in the Polish language"}
				},
				{
					{"name","U.S.M.C. Recon"},
					{"description","The United States Marine Core FORCE Recon's mission is to train, recruit, and deploy scalable counter insurgency operations around the globe."},
				},
				{
					{"name","German G.S.G. 9"},
					{"description","The G.S.G. 9 is the elite tactical unit of the German Federal Police. It is deployed in cases of hostage-taking, kidnapping, terrorism, and extortion."},
				},
				{
					{"name","Joint Task Force 2"},
					{"description","The J.T.F. 2 is an extremely capable and precise special forces unit designed to combat terrorism and threats to Canadians at home and abroad."},
				},
				{
					{"name","French GIGN"},
					{"description","The French GIGN is an elite counter-terrorism unit that doubles as an elite Police unit comparable to the United States's S.W.A.T."},
				},
			}){
				try{
					auto insert_transaction = txn();
					sql_compositor comp("player_races",&insert_transaction);
					auto up_sql = comp
						.insert()
						.into("player_races")
						.values(mapped_values)
						.sql();
					mods::pq::exec(insert_transaction,up_sql);
					mods::pq::commit(insert_transaction);
				}catch(std::exception& e){
					std::cerr << __FILE__ << ": " << __LINE__ << ": error seeding player_classes: '" << e.what() << "'\n";
				}
			}
	}
	void seed_player_classes() {
			for(auto mapped_values : std::vector<std::map<std::string,std::string>>{
				{
					{"name","ENGINEER"},
					{"description","Natural affinity towards gadgets, demolitions, and unconventional warfare"}
				},
				{
					{"name","MEDIC"},
					{"description","Battlefield suture expert. Capable of absorbing high amounts of damage. Can revive and heal downed teammates."}
				},
				{
					{"name","MARKSMAN"},
					{"description","Ranged weapons expert. Spawns with a sniper rifle that only you can use."}
				},
				{
					{"name","PSYOP"},
					{"description","Psychological warfare and advanced technology expert. Gifted at technomancy and occult tactics."},
				},
				{
					{"name","SUPPORT"},
					{"description","Capable of hauling and supplying intense levels of L.M.G. suppressing fire. Can craft weaponry and ammo from equipment found on the battlefield."},
				},
				{
					{"name","MARINE"},
					{"description","Well-rounded and battle-tested direct-action counter insurgency. Very high Sub Machine Gun and close-range melee damage."}
				}
			}){
				try{
					auto insert_transaction = txn();
					sql_compositor comp("player_classes",&insert_transaction);
					auto up_sql = comp
						.insert()
						.into("player_classes")
						.values(mapped_values)
						.sql();
					mods::pq::exec(insert_transaction,up_sql);
					mods::pq::commit(insert_transaction);
				}catch(std::exception& e){
					std::cerr << __FILE__ << ": " << __LINE__ << ": error seeding player_classes: '" << e.what() << "'\n";
				}
			}//end for
	}

};

