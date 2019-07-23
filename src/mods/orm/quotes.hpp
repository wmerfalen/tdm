#ifndef __MENTOC_MODS_ORM_QUOTES_HEADER__
#define __MENTOC_MODS_ORM_QUOTES_HEADER__

#include <iostream>
#include <vector>
#include "../pq.hpp"
#include "../sql.hpp"
#include "base.hpp"

namespace mods::orm {
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	template <typename TIDType,typename TTimestampType>
		struct base_quotes : public base {
			static constexpr const char* table_name = "quotes";
			TIDType id;
			std::string quote_title;
			std::string quote_content;
			TTimestampType quote_created_at;
			bool load_by_id(const TIDType & id){
				mutable_map_t values;
				auto row_count = db::load_record("quotes", id, values);
				if(row_count){
					quote_title = values["quote_title"];
					quote_content = values["quote_content"];
					quote_created_at = static_cast<TTimestampType>(values["quote_created_at"]);
				}
				return row_count;
			}
		};

	using quotes = base_quotes<uint64_t,uint32_t>;

};

#endif
