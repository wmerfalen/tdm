#include "../pq.hpp"

namespace mods::orm::connector {
#ifdef __MENTOC_SHOW_ZEROMQ_DEBUG__
#define m_debug(A) mentoc_prefix_debug(CAT(__FILE__,":",__LINE__,":",__FUNCTION__)) << A << "\n";
#define debug_dump_list() 		for(auto i = 0; i < list.size(); i++) {\
				m_debug("[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[\n");\
				m_debug("[" << i << "]->'" << list[i] << "'\n");\
				m_debug("[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[\n");\
			}
#else
#define m_debug(A)
#define debug_dump_list()
#endif

	static inline auto&& make() {
		m_debug("ENTRY");
		return std::move(std::make_unique<pqxx::connection>(mods::conf::pq_connection().c_str()));
	}
};//end namespace mods::orm::connector
