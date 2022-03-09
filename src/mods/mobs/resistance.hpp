#ifndef __MENTOC_MODS_MOBS_resistance_HEADER__
#define  __MENTOC_MODS_MOBS_resistance_HEADER__

#include "../../globals.hpp"
#include "../elemental.hpp"
namespace mods::mobs::resistance {
	using index_id_t = uint64_t;
	enum unit_type_t : uint8_t {
		PERCENTAGE = (1 << 0),
		DAMPENER = (1 << 1),
		DIVISOR = (1 << 2),
	};
	struct resistance_t {
		float amount;
		unit_type_t type;
		mods::elemental_types_t elemental_type;
	};


	/**
	 * Resistance lists can be attached/used for anything, but the main
	 * purpose is to define a single list of resistances that belong to
	 * a specific NPC. The main identifier is the mob virtual number.
	 * Currently, we are using resistance lists for the defiler mob
	 * boss type. However, any mob could have a list of resistances.
	 *
	 * The strategy here is to instantiate a list of resistances ONCE
	 * per mob vnum. Instead of creating duplicate data and having each
	 * smart_mob instance carry it's own copy of resistance lists, we instead
	 * use this simply resistance_list_t wrapper class to handle all that.
	 */
	struct resistance_list_t {
			resistance_list_t() :m_mob_vnum(0) {}
			resistance_list_t(const mob_vnum& mob_vnumber,const std::vector<resistance_t>& a) : m_mob_vnum(mob_vnumber), m_list(a) {
			}
			resistance_list_t(const resistance_list_t& other);// copy constructor

			~resistance_list_t() {
				m_mob_vnum = 0;
				m_list.clear();
			}
			void clear_all();
			void add_resistance(resistance_t r);
			const auto& get_resistances();
		private:
			mob_vnum m_mob_vnum;
			std::vector<resistance_t> m_list;
	};
	void register_resistances_for_mob_vnum(
	    const mob_vnum& mob_vnumber,
	    const std::vector<resistance_t>& resistances
	);
	std::optional<resistance_list_t> resistance_list_by_mob_vnum(const mob_vnum& mob);

};
#endif
