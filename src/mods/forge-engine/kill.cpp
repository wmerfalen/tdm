#include "kill.hpp"
#include "../mobs/find.hpp"

#ifdef __MENTOC_SHOW_MODS_FORGE_ENGINE_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::forge_engine::kill]")  << MSG << "\n";
#else
#define m_debug(MSG) ;;
#endif

namespace mods::forge_engine {
	kill_t::kill_t() {
		victim = 0;
		killer = nullptr;
		m_mob_found = false;
		m_proto = std::nullopt;
		m_cost = 0;
	}
	kill_t::kill_t(const kill_t& copy) {
		victim = copy.victim;
		killer = copy.killer;
		m_cost = copy.m_cost;
		find_mob();
	}

	void kill_t::find_mob() {
		m_mob_found = false;
		m_proto = std::nullopt;
		auto opt_proto_iterator = mods::mobs::find_by_vnum(victim);
		if(opt_proto_iterator.has_value()) {
			m_proto = *(opt_proto_iterator.value());
			m_mob_found = true;
		}
	}

	std::optional<char_data> kill_t::cd() {
		return m_proto;
	}
	int32_t kill_t::cost() {
		return m_cost;
	}
	void kill_t::set_cost(int32_t c) {
		m_cost = c;
	}
	void kill_t::add_cost(int32_t c) {
		m_cost += c;
	}
	void kill_t::consume_cost(int32_t c) {
		m_cost -= c;
	}

};

