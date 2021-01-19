#include "generator.hpp"
#include "../interpreter.hpp"
#include "../super-users.hpp"

#ifdef __MENTOC_SHOW_MODS_FORGE_ENGINE_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::fluxkraft::generator]")  << MSG << "\n";
#else
#define m_debug(MSG) ;;
#endif

namespace mods::fluxkraft {
	mods::fluxkraft::generator map_generator;

	static constexpr float ATTRIBUTE_LOW = 1.0;
	static constexpr float ATTRIBUTE_HIGH= 10000.0;
	static constexpr uint8_t MAX_ATTRIBUTES = 35;
	static constexpr float STAT_BOOST_LOW = 1.0;
	static constexpr float STAT_BOOST_HIGH= 10000.0;
	static constexpr uint8_t MAX_RIFLE_STAT_BOOSTS = 35;

	template <>
	bool generator::roll<bool>() {
		return rand_xoroshiro() & 1;
	}
	generator::generator() {
		this->init();
	}
	void generator::init() {
		std::cerr << "init ";
	}
	generator::~generator() {
		std::cout << "[fluxkraft::generator::~generator]\n";
	}

};//end namespace mods::fluxkraft
#undef m_debug
