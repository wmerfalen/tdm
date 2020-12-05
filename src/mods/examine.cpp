#include "examine.hpp"

#ifdef __MENTOC_MODS_EXAMINE_DEBUG__
#define mex_debug(A) std::cerr << "[mods::examine][debug]:" << A <<"\n";
#else
#define mex_debug(A)
#endif

extern int generic_find(char *arg, bitvector_t bitvector, char_data *ch, char_data **tar_ch, struct obj_data **tar_obj);
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

	uuid_t find_player_by_name(player_ptr_t& player, std::string_view arg){
		char_data *found_char = nullptr;
		obj_data* found_obj = nullptr;

#define FIND_CHAR_ROOM     (1 << 0)


		generic_find((char*)arg.data(), FIND_CHAR_ROOM, player->cd(), &found_char, &found_obj);

		if(found_char != nullptr) {
			return found_char->uuid;
		}
		return 0;
	}
};
