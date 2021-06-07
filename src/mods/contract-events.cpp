#include "../globals.hpp"
#include "contract-events.hpp"
#include "contract-types.hpp"
#include "player.hpp"
#ifdef  __MENTOC_SHOW_MESSAGE_QUEUE_OUTPUT__
#define dbg_print(a) std::cerr << "[mods::contract-events][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#else
#define dbg_print(a)
#endif
namespace mods::message_queue {
	void player_found_item(player_ptr_t& player,uuid_t item_uuid) {
		//auto item = optr_by_uuid(item_uuid);
		//if(item->contract) {
		player->contract_find_item(item_uuid);
		//}
	}

	void player_found_mob(player_ptr_t& player,uuid_t mob_uuid) {
		//auto mob = ptr_by_uuid(mob_uuid);
		//if(mob->cd()->contract) {
		player->contract_find_mob(mob_uuid);
		//}
	}

	void player_found_room(player_ptr_t& player,room_rnum room_id) {
		//if(world[room_id].contract) {
		player->contract_find_room(room_id);
		//}
	}

	void player_found_door(player_ptr_t& player,room_rnum room_id, int8_t direction) {
		//if(world[room_id].dir_option[direction]->contract) {
		player->contract_find_door(room_id,direction);
		//}
	}

	void player_destroyed_item(player_ptr_t& player,uuid_t item_uuid) {
		//auto item = optr_by_uuid(item_uuid);
		//if(item->contract) {
		player->contract_destroy_item(item_uuid);
		//}
	}

	void player_destroyed_door(player_ptr_t& player,room_rnum room_id,int8_t direction) {
		//if(world[room_id].dir_option[direction]->contract) {
		player->contract_destroyed_door(room_id,direction);
		//}
	}

	void player_retrieved_item(player_ptr_t& player,uuid_t item_uuid) {
		//auto item = optr_by_uuid(item_uuid);
		//if(item->contract) {
		player->contract_retrieve_item(item_uuid);
		//}
	}

	void player_quota_item_find_increase(player_ptr_t& player,uuid_t item_uuid) {
		//auto item = optr_by_uuid(item_uuid);
		//if(item->contract) {
		player->contract_quota_item_find_increase(item_uuid);
		//}
	}

	void player_quota_mob_kill_increase(player_ptr_t& player,uuid_t mob_uuid) {
		//auto mob = ptr_by_uuid(mob_uuid);
		//if(mob->cd()->contract) {
		player->contract_quota_kill_mob_increase(mob_uuid);
		//}
	}

	void player_quota_door_destroyed_increase(player_ptr_t& player,room_rnum room_id,int8_t direction) {
		//if(world[room_id].dir_option[direction]->contract) {
		player->contract_quota_destroyed_door(room_id,direction);
		//}
	}

	void player_kill_mob(player_ptr_t& player,uuid_t mob_uuid) {
		//auto mob = ptr_by_uuid(mob_uuid);
		//if(mob->cd()->contract) {
		player->contract_kill_mob(mob_uuid);
		//}

	}

	void player_gain_entry(player_ptr_t& player,room_rnum room_id) {
		//if(world[room_id].contract) {
		player->contract_gain_entry(room_id);
		//}
	}

	void player_talk_to(player_ptr_t& player,uuid_t mob_uuid) {
		//auto mob = ptr_by_uuid(mob_uuid);
		//if(mob->cd()->contract) {
		player->contract_talk_to(mob_uuid);
		//}

	}

	void player_install_item(player_ptr_t& player,uuid_t item_uuid) {
		//auto item = optr_by_uuid(item_uuid);
		//if(item->contract) {
		player->contract_install_item(item_uuid);
		//}
	}


};

