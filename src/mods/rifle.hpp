#ifndef __MENTOC_MODS_rifle_HEADER__
#define __MENTOC_MODS_rifle_HEADER__
#include "../globals.hpp"

namespace mods::rifle {
	bool is_rifle(const obj_ptr_t& weapon);
	bool is_rifle_attachment(obj_data* obj);
	float get_zoom_magnification(obj_ptr_t& weapon);
	int16_t get_max_range(obj_ptr_t& weapon);
	int16_t get_critical_range(obj_ptr_t& weapon);
	float get_range_multiplier(obj_ptr_t& weapon);
	int16_t get_effective_firing_range(obj_ptr_t& weapon);


};
#endif
