#ifndef __MENTOC_MODS_CONTRACT_TYPES_HEADER__
#define __MENTOC_MODS_CONTRACT_TYPES_HEADER__
#include "../globals.hpp"
#include "contract-steps.hpp"
#ifdef  __MENTOC_SHOW_CONTRACT_OUTPUT__
#define dbg_print(a) std::cerr << "[mods::contracts][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#else
#define dbg_print(a)
#endif

namespace mods::orm {
	struct player_contract_state;
};

namespace mods::contracts {
	using contract_vnum_t = uint32_t;
	struct contract {
		contract() = delete;
		contract(contract_vnum_t c_vnum,std::string in_title,std::string desc) : vnum(c_vnum), title(in_title), description(desc) {

		}
		contract(const contract& other);
		~contract() = default;
		contract_vnum_t vnum;
		std::string title;
		std::string description;
		std::vector<contract_step> steps;
	};//end class

	std::deque<std::shared_ptr<contract>>& contract_master_list();
	void load_all_contracts();

};
#undef dbg_print
#endif
