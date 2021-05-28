#include "sql.hpp"
#include "colors.hpp"

namespace mods::sql {
	void error_log(std::string_view msg) {
		using namespace mods::colors;
		std::cerr << red_str("SQL ERROR:") << yellow_str(msg.data()) << "\n";
	}
};
