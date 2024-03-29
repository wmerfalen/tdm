#include "support.hpp"
#include "../weapon.hpp"
#include "../orm/inventory.hpp"

namespace mods::classes {
	void support::replenish() {

	}
	support::support() {
	}
	support::support(player_ptr_t p) {
		load_by_player(p);
	}
	player_ptr_t 	support::player() {
		return m_player;
	}

	int16_t support::new_player(player_ptr_t& player, primary_choice_t primary_choice) {
		if(primary_choice == primary_choice_t::NONE) {
			mods::bugs::fixtures("support::new_player. got primary_choice of zero. defaulting to MK46");
			primary_choice = primary_choice_t::MK46;
		}
		auto db_id = m_orm.initialize_row(player);
		if(db_id == 0) {
			return -2;
		}
		load_by_player(player);
		return 0;
	}
	int16_t support::load_by_player(player_ptr_t& player) {
		m_player = player;
		auto result = m_orm.load_by_player(player->db_id());
		if(result < 0) {
			mods::bugs::fixtures(CAT("support::load_by_player.",(result)));
		}
		obj_ptr_t primary = nullptr;
		/** TODO: create catchy name using the deep object parser */
		primary = create_object(ITEM_RIFLE,"hk21.yml");
		player->equip(primary,WEAR_PRIMARY);
		player->equip(create_object(ITEM_RIFLE,"magnum-revolver.yml"),WEAR_SECONDARY);
		auto fatal = create_object(ITEM_EXPLOSIVE,"incendiary-grenade.yml");
		player->carry(fatal);
		return result;
	}
	int16_t support::save() {
		return m_orm.save();
	}
	std::shared_ptr<support> create_support(player_ptr_t& in_player) {
		return std::move(std::make_shared<support>(in_player));
	}
};
