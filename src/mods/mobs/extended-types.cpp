#include "extended-types.hpp"
#include "../util.hpp"
#include "mini-gunner.hpp"
#include "lowly-security.hpp"
#include "mp-shotgunner.hpp"
#include "car-thief.hpp"
#include "generic-thief.hpp"
#include "chaotic-meth-addict.hpp"
#include "shoplifter.hpp"
#include "melee-combatant.hpp"
#include "defiler.hpp"
#include "orthos-agent.hpp"
#include "../behaviour_tree_impl.hpp"

//#define  __MENTOC_MODS_MOBS_SHOW_DEBUG_OUTPUT__
#ifdef  __MENTOC_MODS_MOBS_SHOW_DEBUG_OUTPUT__
#define m_debug(a) mentoc_prefix_debug("m|mobs") << a << "\n";
#else
#define m_debug(a) ;;
#endif

namespace mods::mobs {
	void decorate(uuid_t mob_id) {
		auto p = ptr_by_uuid(mob_id);
		if(!p) {
			m_debug("[mods::mobs::decorate] NOT-FOUND cant find by uuid:" << mob_id);
			return;
		}
		auto ch = p->cd();
		m_debug("_------------------------------_");
		m_debug("mob vnum: '" << red_str(CAT(ch->mob_specials.vnum)) << "'");
		m_debug(red_str("_------------------------------_"));

		const auto& vnum = ch->mob_specials.vnum;
		m_debug("extended_mob_type for mob_id:'" << ch->mob_specials.extended_mob_type << "', mob_id:'" << mob_id << "'");
		bool register_mob_with_btree = true;
		switch(ch->mob_specials.extended_mob_type) {
			default:
			case extended_types_t::NONE:
				m_debug("[extended mob type][" << __FILE__ << "]: no extended mob type for mob: ('" << p->name().c_str() << "')");
				break;
			/**! @NEW_BEHAVIOUR_TREE@ !**/
			case extended_types_t::MP_SHOTGUNNER:
				m_debug("[ found mp shotgunner ]");
				mp_shotgunner::create(mob_id, "normal");
				break;
			case extended_types_t::LOWLY_SECURITY:
				m_debug("[ found lowly security guard ]");
				lowly_security::create(mob_id, "normal");
				break;
			case extended_types_t::MINI_GUNNER:
				m_debug("[ found mini gunner ]");
				mini_gunner::create(mob_id, "normal");
				break;
			case extended_types_t::MINI_GUNNER_SENTINEL:
				m_debug("[ found mini gunner sentinel ]");
				mini_gunner::create(mob_id, "sentinel");
				break;
			case extended_types_t::CAR_THIEF:
				m_debug("[ found car thief ]");
				car_thief::create(mob_id, "normal");
				break;
			case extended_types_t::GENERIC_THIEF:
				m_debug("[ found generic thief ]");
				generic_thief::create(mob_id, mods::mobs::extended_types::get_mobs_targets(vnum));
				register_mob_with_btree = false;
				break;
			case extended_types_t::CHAOTIC_METH_ADDICT:
				m_debug("[ found chaotic meth addict ]");
				chaotic_meth_addict::create(mob_id, mods::mobs::extended_types::get_mobs_targets(vnum));
				register_mob_with_btree = false;
				break;
			case extended_types_t::SHOPLIFTER:
				m_debug("[ found shoplifter ]");
				m_debug("SHOPLIFTER VNUM: '" << vnum << "'");
				shoplifter::create(mob_id, mods::mobs::extended_types::get_mobs_targets(vnum));
				register_mob_with_btree = false;
				break;
			case extended_types_t::MELEE_COMBATANT:
				m_debug("[ found melee_combatant ]");
				melee_combatant::create(mob_id, mods::mobs::extended_types::get_mobs_targets(vnum));
				register_mob_with_btree = false;
				break;
			case extended_types_t::DEFILER:
				m_debug("[ found defiler ]");
				defiler::create(mob_id, mods::mobs::extended_types::get_mobs_targets(vnum));
				register_mob_with_btree = false;
				break;
			case extended_types_t::ORTHOS_AGENT:
				m_debug("[ found orthos agent ]");
				orthos_agent::create(mob_id, mods::mobs::extended_types::get_mobs_targets(vnum));
				register_mob_with_btree = false;
				break;
		}
		if(register_mob_with_btree) {
			/** simply add the mob to the list of mobs with behaviour trees */
			mods::behaviour_tree_impl::register_mob(ch->uuid);
		}
	}
};

namespace mods::mobs::extended_types {
	static std::map<mob_rnum,std::string> mob_target_map;
	void register_targets(const mob_rnum& vnum, std::string_view items) {
		mob_target_map[vnum] = items.data();
	}
	std::string_view get_mobs_targets(const mob_rnum& vnum) {
		return mob_target_map[vnum];
	}
	std::vector<std::string> strings() {
		std::vector<std::string> m;
		for(auto pair : ext_map) {
			m.emplace_back(pair.second);
		}
		return m;
	}

	std::optional<extended_types_t> from_string(std::string str) {
		for(auto pair : ext_map) {
			if(mods::util::is_lower_match(pair.second.c_str(),str.c_str())) {
				return pair.first;
			}
		}
		return std::nullopt;
	}
	std::string description(std::string str) {
		auto opt = from_string(str);
		if(opt.has_value() == false) {
			return "<unknown>";
		}
		return description_strings[opt.value()];
	}
	std::string description(extended_types_t type) {
		return description_strings[type];
	}

	std::string to_string(extended_types_t t) {
		return ext_map[t];
	}
};
#undef m_debug
