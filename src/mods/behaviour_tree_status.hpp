#ifndef __MENTOC_MODS_BEHAVIOUR_TREE_STATUS_HEADER__
#define  __MENTOC_MODS_BEHAVIOUR_TREE_STATUS_HEADER__

namespace mods {
	enum behaviour_tree_status {
		SUCCESS,
		FAILURE,
		RUNNING,
		NO_LOGIC,
		UNKNOWN_STATUS,
		MOB_DOESNT_EXIST
	};
};


#endif
