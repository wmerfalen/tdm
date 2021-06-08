#include "../globals.hpp"
#include "contract-events.hpp"
#include "contract-types.hpp"
#include "player.hpp"
#ifdef  __MENTOC_SHOW_MESSAGE_QUEUE_OUTPUT__
#define dbg_print(a) std::cerr << "[mods::contract-events][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#else
#define dbg_print(a)
#endif
namespace mods::contract_events {
	void perform_get(char_data* ch,obj_data* object) {
		auto receiver = ptr(ch);
		auto o = optr(object);
		receiver->contract_find_item(o);
	}
	void perform_give(char_data* ch, char_data* target,obj_data* object) {
		auto giver = ptr(ch);
		auto receiver = ptr(target);
		auto o = optr(object);
		giver->contract_give_item(o,receiver);
	}
	void perform_give(player_ptr_t& giver, player_ptr_t& receiver,obj_ptr_t& object) {
		giver->contract_give_item(object,receiver);
	}
	void perform_get(player_ptr_t& receiver,obj_ptr_t& object) {
		receiver->contract_retrieve_item(object);
		receiver->contract_find_item(object);
	}
	void find_mob(player_ptr_t& player,player_ptr_t& mob) {
		player->contract_find_mob(mob);
	}
	void find_room(player_ptr_t& player,const room_rnum& room_id) {
		player->contract_find_room(room_id);
	}

	void find_item(player_ptr_t& player,obj_ptr_t& object) {
		player->contract_find_item(object);
	}
	void gain_entry_to_room(player_ptr_t& player,const room_rnum& room_id) {
		player->contract_gain_entry(room_id);
	}
	void find_door(player_ptr_t& player,const room_rnum& room_id,const direction_t& dir) {
		player->contract_find_door(room_id,dir);
	}
};
