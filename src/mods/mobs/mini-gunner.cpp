#include "mini-gunner.hpp"
#include <map>
#include <memory>
namespace mods::mobs {
	static std::map<uuid_t,std::shared_ptr<mini_gunner>> mini_gunners;
#define __MENTOC_MODS_MOBS_MINI_GUNNER_SHOW_DEBUG__
#ifdef __MENTOC_MODS_MOBS_MINI_GUNNER_SHOW_DEBUG__
#define mini_debug(a) std::cerr << "[mods::mobs::mini_gunner][FILE:'" << __FILE__ << "'][LINE:" << __LINE__ << "]->'" << a << "'\n";
#else
#define mini_debug(a) ;;
#endif

	mini_gunner::mini_gunner(uuid_t mob_uuid){
		this->init();
		this->uuid = mob_uuid;
		auto p = ptr_by_uuid(mob_uuid);
		if(!p){
			std::cerr << "MINI-GUNNER exiting since we couldn't find a uuid\n";
			this->loaded = false;
			this->error = true;
			return;
		}
		auto ch = p->cd();
		ch->mob_specials.extended_mob_type = mob_special_data::extended_mob_type_t::MINI_GUNNER;
		ch->mob_specials.set_behaviour_tree("mini_gunner");
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
	void free_mob(uuid_t uuid){
		mini_gunners.erase(uuid);
	}
};
