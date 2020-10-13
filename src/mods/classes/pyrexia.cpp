#include "pyrexia.hpp"
#include "../orm/inventory.hpp"
#include "../affects.hpp"

extern void stop_fighting(char_data *ch);
namespace mods::orm::inventory {
	extern int16_t flush_player(player_ptr_t & player);
};
namespace mods::classes {
		pyrexia::pyrexia(player_ptr_t p){
			load_by_player(p);
		}
		player_ptr_t 	pyrexia::player(){
			return m_player;
		}
		void pyrexia::set_player(player_ptr_t p){
			m_player = p;
		}
		int16_t pyrexia::new_player(player_ptr_t &player){
			auto db_id = m_orm.initialize_row(player);
			if(db_id == 0){
				return -2;
			}
			load_by_player(player);
			return 0;
		}
		int16_t pyrexia::load_by_player(player_ptr_t & player){
			auto result = m_orm.load_by_player(player->db_id());
			if(result < 0){
				report(CAT("pyrexia::load_by_player unable to load pyrexia class by player id: ",(player->db_id()),".. return status: ",(result)));
				return result;
			}
			set_player(player);

			//player->equip(create_object(ITEM_RIFLE,"czp10.yml"),WEAR_SECONDARY);
			return result;
		}
		/* constructors and destructors */
		pyrexia::pyrexia(){
			m_player = nullptr;
		}
		std::shared_ptr<pyrexia> create_pyrexia(player_ptr_t &in_player){
			return std::move(std::make_shared<pyrexia>(in_player));
		}
};
