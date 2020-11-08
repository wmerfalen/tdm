#include "examine.hpp"

#ifdef __MENTOC_MODS_EXAMINE_DEBUG__
#define mex_debug(A) std::cerr << "[mods::examine][debug]:" << A <<"\n";
#else
#define mex_debug(A)
#endif

namespace mods::examine {
	void list_obj_contents_to_char(obj_ptr_t& object,player_ptr_t& player,int mode,int show){
		struct obj_data *i;
		bool found = FALSE;
		std::map<std::string,int> occurrences;
		std::map<std::string,obj_ptr_t> objects;

		for(i = object->contains; i; i = i->next_content) {
			if(!i){
				break;
			}
			if(CAN_SEE_OBJ(player->cd(), i)) {
				occurrences[i->name.c_str()]++;
				objects[i->name.c_str()] = optr(i);
				found = TRUE;
			}
		}
		for(const auto & pair : occurrences){
			show_obj_to_char(objects[pair.first], player, mode,pair.second);
		}

		if(!found && show) {
			player->sendln(" Nothing.");
		}

	}

};
