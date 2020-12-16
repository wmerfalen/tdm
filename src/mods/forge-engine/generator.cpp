#include "generator.hpp"
#include "../interpreter.hpp"

namespace mods::forge_engine {
	generator::generator(){
		this->init<std::mt19937,std::uniform_int_distribution<random_number_type_t>>();
		//this->init<std::mt19937,std::uniform_real_distribution<random_number_type_t>>();
	}
	generator::~generator(){
		std::cout << "[generator::~generator]\n";
	}
	random_number_type_t generator::roll(){
		return (*m_distribution)(*m_generator);
	}

	std::string to_string(skill_types_t t){
		switch(t){
#define M_LAZY(A) case A: return #A;
			M_LAZY(SKILL_STR);
			M_LAZY(SKILL_STR_ADD );
			M_LAZY(SKILL_INTEL );
			M_LAZY(SKILL_WIS );
			M_LAZY(SKILL_DEX );
			M_LAZY(SKILL_CON );
			M_LAZY(SKILL_CHA );
			M_LAZY(SKILL_ELECTRONICS );
			M_LAZY(SKILL_ARMOR );
			M_LAZY(SKILL_MARKSMANSHIP );
			M_LAZY(SKILL_SNIPING );
			M_LAZY(SKILL_DEMOLITIONS );
			M_LAZY(SKILL_CHEMISTRY );
			M_LAZY(SKILL_WEAPON_HANDLING );
			M_LAZY(SKILL_STRATEGY );
			M_LAZY(SKILL_MEDICAL );
#undef M_LAZY
			default:
			return "<unknown>";
		}
	}

	std::string to_string(item_types_t t){
		switch(t){
#define M_LAZY(A) case A: return #A;
			M_LAZY(TYPE_ITEM_RIFLE);
			M_LAZY(TYPE_ITEM_EXPLOSIVE);
			M_LAZY(TYPE_ITEM_GADGET);
			M_LAZY(TYPE_ITEM_DRONE);
			M_LAZY(TYPE_ITEM_WEAPON);
			M_LAZY(TYPE_ITEM_ATTACHMENT);
			M_LAZY(TYPE_ITEM_ARMOR);
			M_LAZY(TYPE_ITEM_CONSUMABLE);
			M_LAZY(TYPE_ITEM_TRAP);
#undef M_LAZY
			default:
			std::cerr << "unknown: " << (int)t << "\n";
			return "<unknown>";
		}
	}
	std::string to_string(rifle_types_t t){
		switch(t){
#define M_LAZY(A) case A: return #A;
			M_LAZY(RIFLE_TYPE_SHOTGUN);
			M_LAZY(RIFLE_TYPE_ASSAULT_RIFLE);
			M_LAZY(RIFLE_TYPE_SUB_MACHINE_GUN);
			M_LAZY(RIFLE_TYPE_SNIPER);
			M_LAZY(RIFLE_TYPE_HANDGUN);
			M_LAZY(RIFLE_TYPE_PISTOL);
			M_LAZY(RIFLE_TYPE_MACHINE_PISTOL);
			M_LAZY(RIFLE_TYPE_LIGHT_MACHINE_GUN);
#undef M_LAZY
			default:
			return "<unknown>";
		}
	}

	std::string to_string(rifle_attributes_t t){
		switch(t){
#define M_LAZY(A) case A: return #A;
			M_LAZY(RIFLE_ATTRIBUTES_AMMO_MAX);
			M_LAZY(RIFLE_ATTRIBUTES_CHANCE_TO_INJURE);
			M_LAZY(RIFLE_ATTRIBUTES_CLIP_SIZE);
			M_LAZY(RIFLE_ATTRIBUTES_COOLDOWN_BETWEEN_SHOTS);
			M_LAZY(RIFLE_ATTRIBUTES_CRITICAL_CHANCE);
			M_LAZY(RIFLE_ATTRIBUTES_CRITICAL_RANGE);
			M_LAZY(RIFLE_ATTRIBUTES_DAMAGE_PER_SECOND);
			M_LAZY(RIFLE_ATTRIBUTES_DISORIENT_AMOUNT);
			M_LAZY(RIFLE_ATTRIBUTES_HEADSHOT_BONUS);
			M_LAZY(RIFLE_ATTRIBUTES_MAX_RANGE);
			M_LAZY(RIFLE_ATTRIBUTES_RANGE_MULTIPLIER);
			M_LAZY(RIFLE_ATTRIBUTES_RELOAD_TIME);
			M_LAZY(RIFLE_ATTRIBUTES_ROUNDS_PER_MINUTE);
			M_LAZY(RIFLE_ATTRIBUTES_MUZZLE_VELOCITY);
			M_LAZY(RIFLE_ATTRIBUTES_EFFECTIVE_FIRING_RANGE);
			M_LAZY(RIFLE_ATTRIBUTES_DAMAGE_DICE_COUNT);
			M_LAZY(RIFLE_ATTRIBUTES_DAMAGE_DICE_SIDES);
#undef M_LAZY
			default:
			return "<unknown>";
		}
	}

	rifle_types_t generator::random_rifle_type(){
		uint32_t size = std::distance(valid_rifle_types.begin(),valid_rifle_types.end());
		return valid_rifle_types.at(this->roll() % size);
	}
	rifle_attributes_t generator::random_rifle_attribute(){
		uint32_t size = std::distance(valid_rifle_affects.begin(),valid_rifle_affects.end());
		return valid_rifle_affects.at(this->roll() % size);
	}

	std::vector<std::pair<rifle_attributes_t,uint32_t>> generator::generate_rifle_attributes(){
		return generate_random<rifle_attributes_t>(valid_rifle_affects);
	}

	std::vector<std::pair<skill_types_t,uint32_t>> generator::generate_rifle_requirements(){
		return generate_random<skill_types_t>(valid_rifle_requirements_based_on);
	}

	item_types_t generator::random_item_type(){
		uint32_t size = std::distance(active_item_types.begin(),active_item_types.end());
		return active_item_types.at(this->roll() % size);
	}
	void init(){
		mods::forge_engine::generator gen;
		std::map<rifle_attributes_t,uint32_t> counts;
		for(int i = 0; i < 1500; i++){
			//std::cerr << "random item type: " << mods::forge_engine::to_string(gen.random_item_type()) << "\n";
			//std::cerr << "rifle: " << to_string(gen.random_rifle_type()) << "\n";
			//for(auto  requirement : gen.generate_rifle_requirements()){
			//	//std::cerr << "rifle requirement: " << mods::forge_engine::to_string(requirement.first) << ": " << std::to_string(requirement.second) << "\n";
			//}
			for(auto  attribute : gen.generate_rifle_attributes()){
				counts[attribute.first]++;
				//std::cerr << "rifle attribute: " << mods::forge_engine::to_string(attribute.first) << ": " << std::to_string(attribute.second) << "\n";
			}

		}
		for(auto & pair : counts){
			std::cerr << "rifle attribute: " << mods::forge_engine::to_string(pair.first) << ": " << std::to_string(pair.second) << "\n";
		}
		for(int i=0; i < 50;i++){
			gen.run_exp_rolls(5000);
		}
		int mod = rand_number(1,200);
		std::map<uint64_t,uint64_t> e_counts;
		while(1)
		{
			for(int i=0; i < 900;i++){
				if(rand_number(1,100) >= rand_number(1,100)){
					mod++;
				}else{
					mod--;
				}
				if(mod >= 200){
					mod = rand_number(1,200);
				}
				if(mod <= 0){
					mod = 3;
				}
				uint64_t r = gen.op_based_roll(gen.exp_roll());
				if(r == 0){
					r = 1;
				}
				r  = r % mod;
				e_counts[r]++;
			}
			for(auto pair : e_counts){
				std::cerr << "rand: " << pair.first << ": " << pair.second << "\n";
			}
			sleep(3);
		}
		sleep(2000);
	}
};
