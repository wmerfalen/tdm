#include "globals.hpp"
#include "conf.h"
#include "mods/extern.hpp"
#include "structs.h"
#include "utils.h"
#include "db.h"


ACMD(do_heal){
	MENTOC_PREAMBLE();
	if(!player->has_class_capability(mods::player::class_type::MEDIC)){
		*player << "You are not a medic\r\n";
		return;
	}

}
