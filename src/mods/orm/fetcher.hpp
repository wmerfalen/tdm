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

namespace generator {
	/**
	 * fetch_by_meta<TRowType,TIterable>(std::string_view column,std::string_view value);
	 * @param std::string_view table_name
	 * @param std::string_view column name
	 * @param std::string_view  value of the column
	 * @return std::vector<TRowType>
	 */
	template <typename TRowType,typename TFunctionCallback>
	uint64_t fetch_by_meta(std::string_view table_name,
			std::string_view column,std::string_view value,
			TFunctionCallback callback) {
		auto result_container = db_get_by_meta(
				svtostr(table_name),svtostr(column),svtostr(value));
		uint64_t ctr = 0;
		for(auto && record : result_container){
			++ctr;
			if(!callback(record)){
				break;
			}
		}
		return ctr;
	}
	/**
	 * find<TRowType,TIterable>(id)
	 * @return TRowType struct
	 */
	template <typename TRowType,typename TFunctionCallback>
	uint64_t find(std::string_view table_name,const uint64_t& id,TFunctionCallback callback){
		auto result_container = db_get_by_meta(
				table_name.data(), "id",std::to_string(id));
		uint64_t ctr =0;
		for(auto && record : result_container){
			++ctr;
			if(!callback(record)){
				break;
			}
		}
		return ctr;
	}
	template <typename TRowType,typename TFunctionCallback>
	uint64_t find_by_column(std::string_view table_name,const uint64_t& id,TFunctionCallback callback,std::string_view column){
		auto result_container = db_get_by_meta(
				table_name.data(), column.data(),std::to_string(id));
		uint64_t ctr =0;
		for(auto && record : result_container){
			++ctr;
			if(!callback(record)){
				break;
			}
		}
		return ctr;
	}


	/**
	 * find<TFunctionaCallback>(std::string_view table,std::iterator id_list,callback);
	 * @param std::string_view table name
	 * @param std::iterator id_list iterable container of ids to fetch
	 * @param TFunctionCallback function callack
	 * @return std::vector<TRowType>
	 */
	template <typename TIdList,typename TFunctionCallback>
	uint64_t find_multi(std::string_view table_name,TIdList id_list, TFunctionCallback callback){
		auto result_container = db_get_by_meta_multi(
				svtostr(table_name), "id",id_list
		);
		uint64_t ctr = 0;
		for(auto && record : result_container){
			++ctr;
			if(!callback(record)){
				break;
			}
		}
		return ctr;
	}
	/**
	 * find<TFunctionaCallback>(std::string_view table,std::iterator id_list,callback);
	 * @param std::string_view table name
	 * @param std::iterator id_list iterable container of ids to fetch
	 * @param TFunctionCallback function callack
	 * @return std::vector<TRowType>
	 */
	template <typename TIdList,typename TFunctionCallback>
	uint64_t find_multi_by_column(
			std::string_view table_name,
			TIdList id_list,
			TFunctionCallback callback,
			std::string_view column){
		auto result_container = db_get_by_meta_multi(
				table_name.data(), column.data(),id_list
		);
		uint64_t ctr = 0;
		for(auto && record : result_container){
			++ctr;
			if(!callback(record)){
				break;
			}
		}
		return ctr;
	}
};


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
