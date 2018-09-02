#ifndef __MENTOC_MODS_LMDB_HEADER__
#define  __MENTOC_MODS_LMDB_HEADER__

#include <iostream>
#include "../structs.h"
#include "../types.hpp"
#include "../globals.hpp"
#include "extern.hpp"
#include <variant>
#include <vector>
#include <map>
#include <string_view>
#include <memory>
#include <string_view>
#include "string.hpp"
namespace mods::lmdb {
	using variant_t = std::variant<std::string_view,mods::string,std::string,const char*,uint64_t,int64_t,int,nullptr_t,short,char,unsigned char,char*,unsigned char*,sbyte>;

	struct consumer {};
	using mutable_map_t = std::map<std::string_view,variant_t>;

	/**
	 * A typical transaction
	 */
	struct transaction_t {
		using result_container = std::vector<std::map<std::string_view,variant_t>>;
		transaction_t(std::string_view table) : m_table(table){

		}
		std::string_view table(){ return m_table; }
		template <typename T>
		result_container get(std::string_view key,T val){
			//TODO: fetch results
			return result_container();
		}

		template <typename T,typename R>
		result_container get(T key,R val){
			//TODO: fetch results
			std::cout << key << ": " << val << "\n";

			return result_container();
		}
		result_container get_all(){
			//TODO: fetch results
			return  result_container();
		}
		transaction_t* set(const mutable_map_t & values){
			//TODO: write the values
			return this;
		}
		private: 
			std::string m_table;
	};

	/**
	 * Transactions
	 */
	using transaction_ptr = std::unique_ptr<transaction_t>;
	using transaction_pool_t = std::vector<std::unique_ptr<transaction_t>>;
	inline static transaction_pool_t transact_pool;

	inline transaction_ptr exec(std::string_view table){
		return std::move(transact_pool.emplace_back(std::make_unique<transaction_t>(table)));
	}

	inline transaction_ptr update(std::string_view table){
		return std::move(transact_pool.emplace_back(std::make_unique<transaction_t>(table)));
	}
	inline transaction_ptr insert(std::string_view table){
		return std::move(transact_pool.emplace_back(std::make_unique<transaction_t>(table)));
	}
};

#endif
