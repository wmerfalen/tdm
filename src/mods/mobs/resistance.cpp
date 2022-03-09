#include "resistance.hpp"
#include <map>
#include <optional>

#ifdef  __MENTOC_MODS_MOBS_LOWLY_SECURITY_SHOW_DEBUG_OUTPUT__
#define sm_debug(a) mentoc_prefix_debug("mods::mobs::resistance") << a << "\n";
#else
#define sm_debug(a) ;;
#endif
namespace mods::mobs::resistance {
	using resistance_map_t = std::map<mob_vnum,resistance_list_t>;
	namespace data {
		static resistance_map_t list;
	};//end namespace data

	static resistance_map_t& resistance_list() {
		return data::list;
	}
	std::optional<resistance_list_t> resistance_list_by_mob_vnum(const mob_vnum& mob) {
		if(data::list.find(mob) != data::list.end()) {
			return data::list[mob];
		}
		return std::nullopt;
	}
	void register_resistances_for_mob_vnum(
	    const mob_vnum& mob_vnumber,
	    const std::vector<resistance_t>& resistances
	) {
		resistance_list()[mob_vnumber] = resistance_list_t(mob_vnumber,resistances);
	}


	void resistance_list_t::clear_all() {
		m_list.clear();
	}
	void resistance_list_t::add_resistance(resistance_t r) {
		m_list.emplace_back(r);
	}
	const auto& resistance_list_t::get_resistances() {
		return m_list;
	}
	resistance_list_t::resistance_list_t(const resistance_list_t& other) { // copy constructor
		m_list = other.m_list;
		m_mob_vnum = other.m_mob_vnum;
	}
};
