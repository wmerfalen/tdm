#ifndef __MENTOC_MODS_ORM_FETCH_HEADER__
#define __MENTOC_MODS_ORM_FETCH_HEADER__

#include <iostream>
#include <vector>
#include "../pq.hpp"
#include "../sql.hpp"
#include "quotes.hpp"
#include "chargen.hpp"
#include "../lmdb.hpp"

namespace mods::orm {
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	/**
	 * fetch_by_meta<TRowType,TIterable>(std::string_view column,std::string_view value);
	 * @param std::string_view column name
	 * @param std::string_view  value of the column
	 * @return std::vector<TRowType>
	 */
	template <typename TRowType,typename TIterable>
	std::vector<TRowType>&& fetch_by_meta(std::string_view column,std::string_view value) {
		auto result_container = db_get_by_meta(
				TRowType::table_name, column,value);
		std::vector<TRowType> results;
		for(auto && record : result_container){
			TRowType row;
			row.feed(record);
			results.emplace_back(row);
		}
		return std::move(results);
	}

	/**
	 * find<TRowType,TIterable>(id)
	 * @return TRowType struct
	 */
	template <typename TRowType,typename TIterable>
	TRowType&& find(const uint64_t& id){
		auto result_container = db_get_by_meta(
				TRowType::table_name, "id",std::to_string(id));
		TRowType result;
		for(auto && record : result_container){
			result = record;
		}
		return std::move(result);
	}

	/**
	 * find<TRowType,TIterable>(std::iterator &id_list);
	 * @param std::iterator id_list iterable container of ids to fetch
	 * @return std::vector<TRowType>
	 */
	template <typename TRowType,typename TIterable>
	std::vector<TRowType>&& find_multi(TIterable & id_list){
		auto result_container = db_get_by_meta_multi(
				TRowType::table_name, "id",id_list
		);
		std::vector<TRowType> results;
		for(auto && record : result_container){
			TRowType row;
			row.feed(record);
			results.emplace_back(row);
		}
		return std::move(results);
	}
};

#endif
