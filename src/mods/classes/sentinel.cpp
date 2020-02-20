#include "sentinel.hpp"
#include "../weapon.hpp"
#include "../affects.hpp"

namespace mods::classes {
		std::shared_ptr<sentinel> sentinel::create(player_ptr_t &in_player){
			return std::move(std::make_shared<sentinel>(in_player));
		}
		int16_t destroy(player_ptr_t& player){
			/** TODO */
			player = nullptr;
			return 0;
		}
		sentinel::sentinel(player_ptr_t p){
			m_mp5 = nullptr;
			m_sasg12 = nullptr;
			m_czp10 = nullptr;
			this->set_player(p);
		}
		mp5_ptr_t sentinel::mp5(){
			return m_mp5;
		}
		sasg12_ptr_t sentinel::sasg12(){
			return m_sasg12;
		}
		czp10_ptr_t sentinel::secondary(){
			return m_czp10;
		}
		czp10_ptr_t sentinel::czp10(){
			return m_czp10;
		}
		mp5_ptr_t sentinel::create_mp5(){
			m_mp5 = std::make_shared<mods::weapons::smg::mp5>();
			return m_mp5;
		}
		sasg12_ptr_t sentinel::create_sasg12(){
			m_sasg12 = std::make_shared<mods::weapons::shotgun::sasg12>();
			return m_sasg12;
		}
		czp10_ptr_t sentinel::create_czp10(){
			m_czp10 = std::make_shared<mods::weapons::pistol::czp10>();
			return m_czp10;
		}
		player_ptr_t 	sentinel::player(){
			return m_player;
		}
		void sentinel::set_player(player_ptr_t p){
			m_player = p;
		}
		int16_t sentinel::new_player(player_ptr_t &player,
				mods::weapon::sentinel::primary_choice_t primary_choice
				){
			switch(primary_choice){
				case primary_choice_t::SENTINEL_PRIMARY_MP5:
				case primary_choice_t::SENTINEL_PRIMARY_SASG12:
					break;
				default:
					std::cerr << "invalid primary weapon choice for sentinel class...\n";
					return -1;
			}

			return 0;
		}
		void sentinel::heal(player_ptr_t& target){
			int healing = 0;
			switch(m_heal_level) {
				default:
				case SENTINEL_CURE_NONE:
					m_player->psendln("It looks like you still need to train that skill");
					return;
				case SENTINEL_CURE_LIGHT:
					healing = dice(1, 8) + 1 + (m_player->level() / 4);
					target->psendln("You feel better.");
					break;

				case SENTINEL_CURE_CRITIC:
					healing = dice(3, 8) + 3 + (m_player->level() / 4);
					target->psendln("You feel a lot better!");
					break;

				case SENTINEL_HEAL:
					healing = 100 + dice(3, 8);
					target->psendln("A warm feeling floods your body.");
					break;
			}
			target->hp() += healing;
		}
		void sentinel::intimidate(player_ptr_t& target){
			int intimidate = 0;
			switch(m_intimidate_level) {
				default:
				case SENTINEL_INTIMIDATE_NONE:
					m_player->psendln("It looks like you still need to train that skill");
					return;
				case SENTINEL_INTIMIDATE_LIGHT:
					intimidate = dice(1, 8) + 1 + (m_player->level() / 4);
					m_player->psendln("Your stare intimidates the enemy.");
					break;

				case SENTINEL_INTIMIDATE_CRITIC:
					intimidate = dice(3, 8) + 3 + (m_player->level() / 4);
					m_player->psendln("Your stare forces the enemy to freeze");
					break;

				case SENTINEL_INTIMIDATE_HEART_ATTACK:
					intimidate = 100 + dice(3, 8);
					m_player->send("%s is filled with overwhelming amounts of dread. They freeze as adrenaline saturates their reasoning", target->name());
					break;
			}
			auto & affects = target->get_affect_dissolver();
			affects.affect(mods::affects::affect_t::INTIMIDATED,intimidate);
		}
		/* constructors and destructors */
		sentinel::sentinel(){
			m_mp5 = nullptr;
			m_sasg12 = nullptr;
			m_czp10 = nullptr;
			m_player = nullptr;
		}
};
