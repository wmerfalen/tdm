#include "sql.hpp"
#include "colors.hpp"
#include "pq.hpp"

namespace mods::sql {
	void error_log(std::string_view msg) {
		using namespace mods::colors;
		std::cerr << red_str("SQL ERROR:") << yellow_str(msg.data()) << "\n";
	}
	template <>
	std::string compositor<mods::pq::transaction>::sql() {
		m_sql = "";
		m_query[3] = "";

		for(auto& join : m_joins) {
			m_query[3] += join + " ";
		}

		for(unsigned i = 0; i < query_parts; i++) {
			m_sql += m_query[i];
		}
		if(m_returning.length()) {
			m_sql += m_returning;
		}
		if(m_order_by.length()) {
			m_sql += m_order_by;
		}
		if(m_limit.length()) {
			m_sql += m_limit;
		}

#ifdef __MENTOC_SHOW_SQL_DEBUG_OUTPUT__
		std::cerr << m_sql << "\n";
#endif
		std::cerr << "EASC: " << (bool)ECHO_ALL_SQL_COMMANDS() << "\n";
		if((bool)ECHO_ALL_SQL_COMMANDS()) {
			std::cerr << "[ECHO_ALL_SQL_COMMANDS()]: '" << m_sql << "'\n";
		}
		return m_sql;
	}
};
