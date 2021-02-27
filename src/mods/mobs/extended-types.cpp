#include "extended-types.hpp"
#include "../util.hpp"
#include "mini-gunner.hpp"
#include "lowly-security.hpp"
#include "../behaviour_tree_impl.hpp"

namespace mods::mobs {
	void decorate(uuid_t mob_id) {
		auto p = ptr_by_uuid(mob_id);
		if(!p) {
			std::cerr << "[mods::mobs::decorate] NOT-FOUND cant find by uuid:" << mob_id << "\n";
			return;
		}
		auto ch = p->cd();
		switch(ch->mob_specials.extended_mob_type) {
			default:
			case extended_types_t::NONE:
				std::cerr << "[extended mob type]: no extended mob type for mob: ('" << ch->player.name.c_str() << "')\n";
				break;
			case extended_types_t::LOWLY_SECURITY:
				std::cerr << "[ found lowly security guard ]\n";
				lowly_security::create(mob_id, "normal");
				break;
			case extended_types_t::MINI_GUNNER:
				std::cerr << "[ found mini gunner ]\n";
				mini_gunner::create(mob_id, "normal");
				break;
			case extended_types_t::MINI_GUNNER_SENTINEL:
				std::cerr << "[ found mini gunner sentinel ]\n";
				mini_gunner::create(mob_id, "sentinel");
				break;
		}
		/** simply add the mob to the list of mobs with behaviour trees */
		mods::behaviour_tree_impl::register_mob(ch->uuid);
	}
};

namespace mods::mobs::extended_types {
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
