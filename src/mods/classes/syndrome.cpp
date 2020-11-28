#include "syndrome.hpp"
#include "../orm/inventory.hpp"
#include "../affects.hpp"
#include "../interpreter.hpp"

extern void stop_fighting(char_data *ch);
namespace mods::orm::inventory {
	extern int16_t flush_player(player_ptr_t & player);
};
namespace mods::classes {
	/** KEEP ME */
	void syndrome_advance_level(player_ptr_t& player){
		/** TODO fill me in */
		player->send("[stub] file:%s line:%d\r\n",__FILE__,__LINE__);
	}
		syndrome::syndrome(player_ptr_t p){
			load_by_player(p);
		}
		player_ptr_t 	syndrome::player(){
			return m_player;
		}
		void syndrome::set_player(player_ptr_t p){
			m_player = p;
		}
		int16_t syndrome::new_player(player_ptr_t &player){
			auto db_id = m_orm.initialize_row(player);
			if(db_id == 0){
				return -2;
			}
			load_by_player(player);
			return 0;
		}
		int16_t syndrome::load_by_player(player_ptr_t & player){
			auto result = m_orm.load_by_player(player->db_id());
			if(result < 0){
				report(CAT("syndrome::load_by_player unable to load syndrome class by player id: ",(player->db_id()),".. return status: ",(result)));
				return result;
			}
			set_player(player);

			//player->equip(create_object(ITEM_RIFLE,"czp10.yml"),WEAR_SECONDARY);
			return result;
		}
		/* constructors and destructors */
		syndrome::syndrome(){
			m_player = nullptr;
		}
		std::shared_ptr<syndrome> create_syndrome(player_ptr_t &in_player){
			return std::move(std::make_shared<syndrome>(in_player));
		}
		void syndrome::replenish(){
			static uint8_t call_count = 0;
			++call_count;
#ifdef __MENTOC_SEND_SYNDROME_PLAYER_REPLENISH_DEBUG_MESSAGE__
			m_player->sendln("Replenish");
#endif
		}

};

