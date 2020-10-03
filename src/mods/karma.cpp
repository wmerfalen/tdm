#include "karma.hpp"
#include "help.hpp"
#include "super-users.hpp"

namespace mods {
	static std::map<uint64_t,karma> karma_index;
	void init(){
		//TODO: mods::orm::karma db;
		//TODO: db.load_all();
		
	}
	void load(player_ptr_t& player){
		std::cerr << "[mods::karma::load]: " << player->name().str() << "\n";
	}
};
