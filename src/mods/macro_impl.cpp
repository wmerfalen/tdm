#include "macro_impl.hpp"
namespace mods::macro_impl {
	byte& __GET_POS(char_data* ch){
		return ch->char_specials.position;
	}
};
