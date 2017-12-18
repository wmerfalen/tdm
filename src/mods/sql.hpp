#pragma once
/**
* The purpose of this small compositor library is to be a simple sql
* generator. It doesn't support complex queries or advanced concepts.
* It's just supposed to help with the most common sql tasks that involve
* ridiculous amounts of concatenation and annoying boilerplate.
* -wm
*/
#ifndef __MENTOC_MODS_SQL_HEADER__
#define __MENTOC_MODS_SQL_HEADER__
#include <array>
#include <map>
#if defined(__cpp_lib_string_view) && __cpp_lib_string_view==201603
#include <string_view>
typedef std::string_view str_object;
#else
#include <string>
typedef std::string str_object;
#endif

namespace mods::sql {
	template <typename T>
	struct compositor {
		compositor() = delete;
		~compositor() = default;
		compositor(str_object table, T* txn_ptr) :
			m_table(table), m_txn_ptr(txn_ptr)
		{}
		typedef std::map<std::string, std::string> value_map;
		compositor<T>& select(str_object fields) {
			std::string sql = "SELECT ";
			sql += fields.data();
			m_query[0] = sql;
			return *this;
		}
		compositor<T>& from(str_object table) {
			m_table = table.data();
			std::string sql = " FROM ";
			sql += m_table + " ";
			m_query[1] = sql;
			return *this;
		}
		compositor<T>& create() {
			m_query[0] = "CREATE TABLE ";
			return *this;
		}
		compositor<T>& table(str_object table) {
			m_table = table.data();
			m_query[1] = m_table + " ";
			return *this;
		}
		compositor<T>& definition(const std::vector<std::string> & members) {
			std::string definition = "( ";
			unsigned ctr = 0;
			for (auto & line : members) {
				definition += line;
				if (ctr + 1 != members.size()) {
					definition += ", ";
				}
				++ctr;
			}
			definition += ") ";
			m_query[2] = definition;
			m_query[3].clear();
			return *this;
		}

		compositor<T>& update(const value_map & values) {
			m_query[0] = "UPDATE ";
			m_query[1] = m_table;
			std::string set = " SET ";
			unsigned v_size = values.size();
			unsigned ctr = 0;
			for (auto& pair : values) {
				set += pair.first;
				set += "=";
				set += m_txn_ptr->quote(pair.second);
				if (ctr + 1 != v_size) {
					set += ", ";
				}
				++ctr;
			}
			m_query[2] = set;
			return *this;
		}

		compositor<T>& insert() {
			std::string sql = "INSERT INTO ";
			m_query[0] = sql;
			return *this;
		}
		compositor<T>& into(str_object table) {
			m_query[1] = m_table = table.data();
			return *this;
		}
		compositor<T>& values(const value_map & values) {
			std::string fields = "(";
			unsigned v_size = values.size();
			unsigned ctr = 0;
			for (auto &pair : values) {
				fields += pair.first;
				if (ctr + 1 != v_size) {
					fields += ", ";
				}
				++ctr;
			}
			fields += ") ";
			m_query[2] = fields;
			std::string value_sql = "VALUES(";
			ctr = 0;
			for (auto &pair : values) {
				value_sql += m_txn_ptr->quote(pair.second);
				if (ctr + 1 != v_size) {
					value_sql += ", ";
				}
				++ctr;
			}
			value_sql += ")";
			m_query[3] = value_sql;
			return *this;
		}
		compositor<T>& where(str_object lhs,
			str_object op,
			str_object rhs) {
			std::string sql = " WHERE ";
			sql += lhs.data();
			sql += " ";
			sql += op.data();
			sql += " ";
			sql += m_txn_ptr->quote(std::string(rhs.data()));
			m_query[3] = sql;
			return *this;
		}

		compositor<T>& clear() {
			m_table = m_sql = "";
			std::fill(m_query.begin(), m_query.end(), "");
			return *this;
		}

		str_object sql() {
			m_sql = "";
			for (unsigned i = 0; i < 4; i++) {
				m_sql += m_query[i];
			}
			return m_sql;
		}
	private:
		std::string m_sql;
		std::array<std::string, 4> m_query;
		std::string m_table;
		T* m_txn_ptr;
	};

};

#endif
