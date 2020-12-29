#include "skills.hpp"
#include "skill-orm-adaptor.hpp"
#include "interpreter.hpp"
#include "super-users.hpp"

#ifdef __MENTOC_MODS_SKILLS_SHOW_DEBUG_OUTPUT__
#define m_debug(A) std::cerr << "[mods::skills][debug]:'" << A << "'\n";
#else
#define m_debug(A)
#endif

#define FOREACH_SKILLSET_AS(PROF_NAME) for(auto & skillset : mods::skills::proficiencies::list) for(auto & PROF_NAME : std::get<1>(skillset))
namespace mods::skill_orm_adaptor {
	int get_player_map(player_ptr_t& player, std::vector<std::string,std::string>& skills) {
		return 0;
	}
	void put_player_map(player_ptr_t& player,const std::map<std::string,std::string>& map) {

	}

};
#undef m_debug
