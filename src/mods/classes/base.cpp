#include "sentinel.hpp"
#include "../weapon.hpp"
#include "../affects.hpp"

namespace mods::classes {
void base::sendln(std::string_view msg){ m_player->sendln(msg); }
void base::psendln(std::string_view msg){ m_player->psendln(msg); }
};
