#include "breacher.hpp"
#include "../orm/inventory.hpp"
#include "../affects.hpp"

extern void stop_fighting(char_data *ch);
namespace mods::orm::inventory {
	extern int16_t flush_player(player_ptr_t & player);
};
namespace mods::classes {
		breacher::breacher(player_ptr_t p){
			load_by_player(p);
		}
		void breacher::replenish(){

		}
		player_ptr_t 	breacher::player(){
			return m_player;
		}
		void breacher::set_player(player_ptr_t p){
			m_player = p;
		}
		int16_t breacher::new_player(player_ptr_t &player){
			auto db_id = m_orm.initialize_row(player);
			if(db_id == 0){
				return -2;
			}
			load_by_player(player);
			return 0;
		}
		int16_t breacher::load_by_player(player_ptr_t & player){
			auto result = m_orm.load_by_player(player->db_id());
			if(result < 0){
				report(CAT("breacher::load_by_player unable to load breacher class by player id: ",(player->db_id()),".. return status: ",(result)));
				return result;
			}
			set_player(player);

			/** TODO: create catchy name using the deep object parser */
			auto primary = create_object(ITEM_RIFLE,"mp5.yml");
			player->equip(primary,WEAR_PRIMARY);
			player->equip(create_object(ITEM_RIFLE,"sasg12.yml"),WEAR_SECONDARY);
			auto fatal = create_object(ITEM_EXPLOSIVE,"flashbang-grenade.yml");
			player->carry(fatal);
			return result;
		}
		/* constructors and destructors */
		breacher::breacher(){
			m_player = nullptr;
		}
		std::shared_ptr<breacher> create_breacher(player_ptr_t &in_player){
			return std::move(std::make_shared<breacher>(in_player));
		}
};
	std::shared_ptr<mods::classes::breacher> create_breacher(player_ptr_t &player){
		return std::move(std::make_shared<mods::classes::breacher>(player));
	}
ACMD(do_teep){
	player->sendln("Command not implemented.");
	return;
#if 0
	PLAYER_CAN("breacher.teep");
	auto vec_args = PARSE_ARGS();
	if(vec_args.size() < 1){
		player->sendln("Which direction?!");
		return;
	}
	auto s = to_direction(vec_args[0]);
	if(!s.first){
		player->sendln("Which direction?!");
		return;
	}
	auto msg = std::get<1>(player->breacher()->teep(s.second));
	player->sendln(msg);
#endif
}
