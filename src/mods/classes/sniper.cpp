#include "sniper.hpp"
#include "../weapon.hpp"

namespace mods::classes {
	std::shared_ptr<mods::weapons::sniper_rifle::psg1> sniper::rifle(){ 
			return m_rifle;
	}
	sniper::sniper(){
		m_rifle = std::make_shared<mods::weapons::sniper_rifle::psg1>();
		rifle()->bound_to = 0; /** TODO: update this player->uuid(); */
		rifle()->id = 0; /**TODO: update this*/
	}

};
