#ifndef __MENTOC_MODS_FIND_HEADER__
#define __MENTOC_MODS_FIND_HEADER__
#include "../globals.hpp"

extern int generic_find(char *arg, bitvector_t bitvector, char_data *ch, char_data **tar_ch, struct obj_data **tar_obj);

namespace mods::find {
	obj_ptr_t obj(std::string_view arg,player_ptr_t& viewer);


};

#endif
