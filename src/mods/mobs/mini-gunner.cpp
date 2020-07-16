#include "mini-gunner.hpp"
#include <map>
#include <memory>
#include "../weapons/damage-types.hpp"

#define __MENTOC_MODS_MOBS_MINI_GUNNER_SHOW_DEBUG_OUTPUT__
#ifdef __MENTOC_MODS_MOBS_MINI_GUNNER_SHOW_DEBUG_OUTPUT__
#define mini_debug(a) mentoc_prefix_debug("mods::mobs::mini_gunner") << a << "\n";
#else
#define mini_debug(a) ;;
#endif
namespace mods::mobs {
	std::map<uuid_t,std::shared_ptr<mini_gunner>> mg_map;
	void mini_gunner::create(uuid_t mob_uuid){
		mini_debug("mini_gunner create on uuid:" << mob_uuid);
		auto p = ptr_by_uuid(mob_uuid);
		if(!p){
			log("SYSERR: did not find player to populate mini_gunner with: %d",mob_uuid);
			return;
		}
		mg_map.insert({mob_uuid,std::make_shared<mini_gunner>(mob_uuid)});
	}
	void mini_gunner::free_mob(uuid_t uuid){
		mg_map.erase(uuid);
	}
	void mini_gunner::wear(int where,std::string_view yaml){
		std::cerr << "mini_gunner wearing: [where:" << where << "]->'" << yaml.data() << "'\n";
		std::tuple<int,std::string> yaml_tuple = mods::util::extract_yaml_info_from_path(yaml);
		if(std::get<0>(yaml_tuple) < 0){
			return;
		}
		if(!mods::util::yaml_file_exists(yaml.data())){
			std::cerr << "[mini_gunner] WARNING: yaml file doesn't exist!->'" << yaml.data() << "'\n";
			return;
		}
		auto obj = create_object(std::get<0>(yaml_tuple),std::get<1>(yaml_tuple));
		this->player_ptr->equip(obj,where);
	}
	mini_gunner::mini_gunner(uuid_t mob_uuid){
		this->init();
		this->uuid = mob_uuid;
		auto p = ptr_by_uuid(mob_uuid);
		if(!p){
			log("SYSERR: did not find player to populate mini_gunner with: %d",mob_uuid);
			this->loaded = false;
			this->error = true;
			return;
		}
		this->player_ptr = p;
		auto ch = p->cd();
		ch->mob_specials.extended_mob_type = mob_special_data::extended_mob_type_t::MINI_GUNNER;
		ch->mob_specials.set_behaviour_tree("mini_gunner_roam");
		MENTOC_MOB_WEARS(MINI_GUNNER);
		this->loaded = true;
		this->error = false;
	}
	mini_gunner::~mini_gunner(){
		this->uuid = 0;
	}
	void mini_gunner::init(){
		this->uuid = 0;
		this->loaded = false;
	}
	void mini_gunner::set_heading(int dir){
		this->player_ptr->cd()->mob_specials.heading = dir;
	}
	void mini_gunner::spray(int dir){
		mini_debug("SPRAYING: " << dirstr(dir));
		this->spray_direction = dir;
		mods::weapons::damage_types::spray_direction(this->player_ptr,dir);
	}
};
