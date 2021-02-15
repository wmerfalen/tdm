#ifndef __MENTOC_MODS_AFFILIATIONS_HEADER__
#define __MENTOC_MODS_AFFILIATIONS_HEADER__

namespace mods {
	enum affiliation_t {
		RTA_POLICE,	// town police
		RTA_MF13,		// gang
		RTA_RUSSIAN_MOB,	// mafia ran by russia
		RTA_K12_SYNDICATE,	// gang
		RTA_CONTRA_SEC,	// Private contractors
	};
	static inline const std::map<std::string_view,affiliation_t>& affiliation_map() {
		static std::map<std::string_view,affiliation_t> list;
		if(list.size()) {
			return list;
		}
		list = {
			{"POLICE",RTA_POLICE},
			{"MF13",RTA_MF13},
			{"RUSSIAN_MOB",RTA_RUSSIAN_MOB},
			{"K12_SYNDICATE",RTA_K12_SYNDICATE},
			{"CONTRA_SEC",RTA_CONTRA_SEC},
		};
		return list;
	};

};


#endif
