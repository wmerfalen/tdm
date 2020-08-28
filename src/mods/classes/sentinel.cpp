#include "sentinel.hpp"
#include "../orm/inventory.hpp"
#include "../weapon.hpp"
#include "../affects.hpp"

namespace mods::orm::inventory {
	extern int16_t flush_player(player_ptr_t & player);
};
namespace mods::classes {
		int16_t destroy(player_ptr_t& player){
			/** TODO */
			player = nullptr;
			return 0;
		}
		sentinel::sentinel(player_ptr_t p){
			m_mp5 = nullptr;
			m_sasg12 = nullptr;
			m_czp10 = nullptr;
			this->load_by_player(p);
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
		mp5_ptr_t sentinel::create_mp5(uint64_t id){
			m_mp5 = std::make_shared<mods::weapons::smg::mp5>();
			auto result = db_get_by_meta("object_rifle","rifle_id",std::to_string(id));
			for(auto&& row : result){
				m_mp5->feed_by_file(row["rifle_file"].as<std::string>());
			}
			return m_mp5;
		}
		sasg12_ptr_t sentinel::create_sasg12(uint64_t id){
			m_sasg12 = std::make_shared<mods::weapons::shotgun::sasg12>();
			auto result = db_get_by_meta("object_rifle","rifle_id",std::to_string(id));
			for(auto&& row : result){
				m_sasg12->feed_by_file(row["rifle_file"].as<std::string>());
			}
			return m_sasg12;
		}
		czp10_ptr_t sentinel::create_czp10(uint64_t id){
			m_czp10 = std::make_shared<mods::weapons::pistol::czp10>();
			auto result = db_get_by_meta("object_rifle","rifle_id",std::to_string(id));
			for(auto&& row : result){
				m_czp10->feed_by_file(row["rifle_file"].as<std::string>());
			}
			return m_czp10;
		}
		player_ptr_t 	sentinel::player(){
			return m_player;
		}
		void sentinel::set_player(player_ptr_t p){
			m_player = p;
		}
		int16_t sentinel::new_player(player_ptr_t &player,
				std::string_view primary_choice
				){
			using primary = mods::weapon::sentinel::primary_choice_t;
			auto pchoice = 0;
			if(!primary_choice.compare("MP5")){
				pchoice = primary::SENTINEL_PRIMARY_MP5;
			}
			if(!primary_choice.compare("SASG12")){
				pchoice = primary::SENTINEL_PRIMARY_SASG12;
			}
			if(pchoice == 0){
				std::cerr << "invalid primary weapon choice for sentinel class...\n";
				return -1;
			}
			auto db_id = m_orm.initialize_row(player,static_cast<primary>(pchoice));
			if(db_id == 0){
				return -2;
			}
			load_by_player(player);
			return 0;
		}
		int16_t sentinel::load_by_player(player_ptr_t & player){
			auto result = m_orm.load_by_player(player->db_id());
			if(result < 0){
				std::cerr << "unable to load sentinel class by player id: " << player->db_id() << ".. return status: " << result << "\n";
				return result;
			}
			set_player(player);
			m_heal_level = static_cast<cure_levels_t>(m_orm.sentinel_heal_level);
			m_intimidate_level = static_cast<intimidate_levels_t>(m_orm.sentinel_intimidate_level);
			m_human_shield_level = static_cast<human_shield_levels_t>(m_orm.sentinel_human_shield_level);
			m_deny_entry_level = static_cast<deny_entry_levels_t>(m_orm.sentinel_deny_entry_level);
			m_gadget_shield_level = static_cast<gadget_shield_levels_t>(m_orm.sentinel_gadget_shield_level);

			std::cerr << "[sentinel][load_by_player]\n";
			return 0;
		}
		int16_t sentinel::save() {
			mods::orm::inventory::flush_player(this->m_player);
			return m_orm.save(this);
		}
void sentinel::sendln(std::string_view msg){ m_player->sendln(msg); }
void sentinel::psendln(std::string_view msg){ m_player->psendln(msg); }
		void sentinel::heal(player_ptr_t& target){
			int healing = 0;
			switch(m_heal_level) {
				default:
				case SENTINEL_CURE_NONE:
					psendln("It looks like you still need to train that skill");
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
					target->psendln("Your health increases drastically.");
					break;
			}
			target->hp() += healing;
		}
		void sentinel::intimidate(player_ptr_t& target){
			int intimidate = 0;
			switch(m_intimidate_level) {
				default:
				case SENTINEL_INTIMIDATE_NONE:
					psendln("It looks like you still need to train that skill");
					return;
				case SENTINEL_INTIMIDATE_LIGHT:
					intimidate = dice(1, 8) + 1 + (m_player->level() / 4);
					m_player->send("%s flees in fear!", target->name());
					break;

				case SENTINEL_INTIMIDATE_CRITIC:
					intimidate = dice(3, 8) + 3 + (m_player->level() / 4);
					m_player->send("%s experiences a massive adrenaline dump", target->name());
					break;

				case SENTINEL_INTIMIDATE_HEART_ATTACK:
					intimidate = 100 + dice(3, 8);
					m_player->send("%s grips his chest and vomits uncontrollably", target->name());
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
		std::shared_ptr<sentinel> create_sentinel(player_ptr_t &in_player){
			return std::move(std::make_shared<sentinel>(in_player));
		}
};
