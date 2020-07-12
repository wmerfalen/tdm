#include "mini-gunner.hpp"
#include <map>
#include <memory>

#define __MENTOC_MODS_MOBS_MINI_GUNNER_SHOW_DEBUG_OUTPUT__
#ifdef __MENTOC_MODS_MOBS_MINI_GUNNER_SHOW_DEBUG_OUTPUT__
#define mini_debug(a) mentoc_prefix_debug("mods::mobs::mini_gunner") << a << "\n";
#else
#define mini_debug(a) ;;
#endif
namespace mods::mobs {
	static std::map<uuid_t,std::shared_ptr<mini_gunner>> mini_gunners;
	void mini_gunner::create(uuid_t mob_uuid){
		mini_debug("mini_gunner create on uuid:" << mob_uuid);
		auto p = ptr_by_uuid(mob_uuid);
		if(!p){
			log("SYSERR: did not find player to populate mini_gunner with: %d",mob_uuid);
			return;
		}
		mini_gunners.insert({mob_uuid,std::make_shared<mini_gunner>(mob_uuid)});
	}
	void mini_gunner::free_mob(uuid_t uuid){
		mini_gunners.erase(uuid);
	}
	void mini_gunner::wear(int where,std::string_view yaml){
		std::cerr << "mini_gunner wearing: [where:" << where << "]->'" << yaml.data() << "'\n";
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
		auto ch = p->cd();
		ch->mob_specials.extended_mob_type = mob_special_data::extended_mob_type_t::MINI_GUNNER;
		ch->mob_specials.set_behaviour_tree("mini_gunner");
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
};
