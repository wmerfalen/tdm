#include "chargen.hpp"
#include <vector>
#include <map>
#include <string>

namespace mods::orm {
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
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
					{"name","British Special Air Service"},
					{"description",""},
				},
				{
					{"name","Navy Seals"},
					{"description",""},
				},
				{
					{"name","Russian Spetsnaz"},
					{"description",""},
				},
				{
					{"name","Polish GROM"},
					{"description",""},
				},
				{
					{"name","USMC Reconnaissance"},
					{"description",""},
				},
				{
					{"name","German GSG 9"},
					{"description",""},
				},
				{
					{"name","Joint Task Force 2"},
					{"description",""},
				},
				{
					{"name","French GIGN"},
					{"description",""},
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

