#include "item.hpp"
#include "../globals.hpp"
#include "player.hpp"
#include "../utils.h"

cpp_extern const struct str_app_type str_app[];
extern void obj_from_char(obj_ptr_t in_object) ;
extern void obj_to_char(obj_ptr_t  object, player_ptr_t player);
extern void act(const std::string& str, int hide_invisible, char_data *ch, obj_data *obj, void *vict_obj, int type);

/* I/O functions */
void	write_to_q(std::string_view txt, mods::descriptor_data&, int aliased);

namespace mods::item {
	bool is_capable_of_single(player_ptr_t& player, obj_ptr_t& object, capability_t action) {
		return false;
	}
	bool is_capable_of_all(player_ptr_t& player, obj_ptr_t& object, cap_list_t& actions) {
		return false;
	}
	bool is_capable_of_any(player_ptr_t& player, obj_ptr_t& object, cap_list_t& actions) {
		return false;
	}
	static constexpr uint8_t TO_ROOM = 1;
	static constexpr uint8_t TO_VICT = 2;
	static constexpr uint8_t TO_NOTVICT = 3;
	static constexpr uint8_t TO_CHAR = 4;
	static constexpr uint8_t TO_SLEEP  = 128;
	std::tuple<int16_t,std::string> perform_give(player_ptr_t& player,player_ptr_t& vict,obj_ptr_t& obj) {
		if(OBJ_FLAGGED(obj.get(), ITEM_NODROP)) {
			return {-1,"You can't let go of it!"};
		}

		if(IS_CARRYING_N(vict->cd()) >= CAN_CARRY_N(vict->cd())) {
			return {-2,"Target's hands are full."};
		}

		if(GET_OBJ_WEIGHT(obj) + IS_CARRYING_W(vict->cd()) > CAN_CARRY_W(vict->cd())) {
			return {-3,"Target can't carry that much weight"};
		}
		obj_to_char(obj, player);
		obj_from_char(obj);
		obj_to_char(obj, vict);
		act("You give $p to $N.", FALSE, player->cd(), obj.get(), vict->cd(), TO_CHAR);
		act("$n gives you $p.", FALSE, player->cd(), obj.get(), vict->cd(), TO_VICT);
		act("$n gives $p to $N.", TRUE, player->cd(), obj.get(), vict->cd(), TO_NOTVICT);
		return {0,"You give the item"};
	}
};
