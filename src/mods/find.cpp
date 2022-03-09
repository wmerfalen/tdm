#include "../globals.hpp"
#include "player.hpp"

extern int generic_find(char *arg, bitvector_t bitvector, char_data *ch, char_data **tar_ch, struct obj_data **tar_obj);

namespace mods::find {
	static constexpr uint16_t FIND_CHAR_ROOM     = (1 << 0);
	static constexpr uint16_t FIND_CHAR_WORLD    = (1 << 1);
	static constexpr uint16_t FIND_OBJ_INV       = (1 << 2);
	static constexpr uint16_t FIND_OBJ_ROOM      = (1 << 3);
	static constexpr uint16_t FIND_OBJ_WORLD     = (1 << 4);
	static constexpr uint16_t FIND_OBJ_EQUIP     = (1 << 5);

	obj_ptr_t obj(std::string_view arg,player_ptr_t& viewer) {
		obj_data* found_object = nullptr;
		char_data* found_char = nullptr;
		auto bits = generic_find((char*)arg.data(), FIND_OBJ_INV | FIND_OBJ_ROOM | FIND_OBJ_EQUIP | FIND_CHAR_ROOM, viewer->cd(), &found_char, &found_object);
		if(!bits) {
			return nullptr;
		}
		if(found_object) {
			return optr(found_object);
		}
		return nullptr;
	}

};
