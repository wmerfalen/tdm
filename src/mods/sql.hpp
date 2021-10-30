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
#include <vector>
#include <iostream>
#include <string>
#if defined(__cpp_lib_string_view) && __cpp_lib_string_view==201603
#include <string_view>
typedef std::string_view str_object;
#else
#include <string>
typedef std::string str_object;
#endif

namespace mods::values {
	extern bool ECHO_ALL_SQL_COMMANDS();
};

#define __MENTOC_SHOW_SQL_DEBUG_OUTPUT__
namespace mods::sql {
	static inline std::string sanitize_table(std::string_view table_name) {
		std::string cleaned = "";
		for(const auto& ch : table_name) {
			if(isalpha(ch) || isdigit(ch) || ch == '_') {
				cleaned += ch;
			}
		}
		return cleaned;
	}
	template <typename T>
	struct compositor {
			constexpr static int query_parts = 5;
			compositor() = delete;
			~compositor() = default;
			compositor(str_object table, T* txn_ptr) :
				m_table(sanitize_table(table)), m_txn_ptr(txn_ptr) {
			}
			typedef std::map<std::string, std::string> value_map;
			compositor<T>& select(str_object fields) {
				std::string sql = "SELECT ";
				sql += fields.data();
				m_query[0] = sql;
				return *this;
			}
			compositor<T>& select_json_agg(std::string_view table) {
				m_query[0] = "SELECT json_agg(";
				m_query[0] += sanitize_table(table);
				m_query[0] += ") ";
				return *this;
			}
			compositor<T>& select_row_to_json(std::string_view table) {
				m_query[0] = "SELECT row_to_json(";
				m_query[0] += sanitize_table(table);
				m_query[0] += ") ";
				return *this;
			}
			compositor<T>& del() {
				m_query[0] = "DELETE ";
				return *this;
			}
			compositor<T>& from(str_object table) {
				m_table = sanitize_table(table);
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
				m_table = sanitize_table(table);
				m_query[1] = m_table + " ";
				return *this;
			}
			compositor<T>& definition(const std::vector<std::string>& members) {
				std::string definition = "( ";
				unsigned ctr = 0;

				for(auto& line : members) {
					definition += line;

					if(ctr + 1 != members.size()) {
						definition += ", ";
					}

					++ctr;
				}

				definition += ") ";
				m_query[2] = definition;
				return *this;
			}

			compositor<T>& update(str_object table) {
				m_query[0] = "UPDATE ";
				m_query[1] = m_table = sanitize_table(table);
				return *this;
			}

			compositor<T>& set_with_password(const value_map& values,std::string_view password_field) {
				std::string set = " SET ";
				unsigned v_size = values.size();
				unsigned ctr = 0;

				for(auto& pair : values) {
					if(pair.first.compare(password_field) == 0) {
						set += pair.first;
						set += "=";
						set += "crypt(";
						set += m_txn_ptr->quote(pair.second);
						set += ",gen_salt('bf'))";
					} else {
						set += pair.first;
						set += "=";
						set += m_txn_ptr->quote(pair.second);
					}

					if(ctr + 1 != v_size) {
						set += ", ";
					}

					++ctr;
				}

				m_query[2] = set;
				return *this;
			}

			compositor<T>& set(const value_map& values) {
				std::string set = " SET ";
				unsigned v_size = values.size();
				unsigned ctr = 0;

				for(auto& pair : values) {
					set += pair.first;
					set += "=";
					if(pair.second.length() == 0) {
						set += "NULL";
					} else {
						set += m_txn_ptr->quote(pair.second);
					}

					if(ctr + 1 != v_size) {
						set += ", ";
					}

					++ctr;
				}

				m_query[2] = set;
				return *this;
			}

			compositor<T>& insert() {
				m_query[0] = "INSERT INTO ";
				return *this;
			}
			compositor<T>& into(str_object table) {
				m_query[1] = m_table = sanitize_table(table);
				return *this;
			}
			compositor<T>& values_with_password(const value_map& values, std::string_view password_field) {
				std::string fields = "(";
				unsigned v_size = values.size();
				unsigned ctr = 0;

				for(auto& pair : values) {
					fields += pair.first;

					if(ctr + 1 != v_size) {
						fields += ", ";
					}

					++ctr;
				}

				fields += ") ";
				m_query[2] = fields;
				std::string value_sql = "VALUES(";
				ctr = 0;

				for(auto& pair : values) {
					if(pair.first.compare(password_field) == 0) {
						value_sql += "crypt(";
						value_sql += m_txn_ptr->quote(pair.second);
						value_sql += ",gen_salt('bf'))";
					} else {
						value_sql += m_txn_ptr->quote(pair.second);
					}

					if(ctr + 1 != v_size) {
						value_sql += ", ";
					}

					++ctr;
				}

				value_sql += ")";
				m_query[4] = value_sql;
				return *this;
			}
			compositor<T>& values(const value_map& values) {
				std::string fields = "(";
				unsigned v_size = values.size();
				unsigned ctr = 0;

				for(auto& pair : values) {
					fields += pair.first;

					if(ctr + 1 != v_size) {
						fields += ", ";
					}

					++ctr;
				}

				fields += ") ";
				m_query[2] = fields;
				std::string value_sql = "VALUES(";
				ctr = 0;

				for(auto& pair : values) {
					if(pair.second.length() == 0) {
						value_sql += "NULL";
					} else {
						value_sql += m_txn_ptr->quote(pair.second);
					}

					if(ctr + 1 != v_size) {
						value_sql += ", ";
					}

					++ctr;
				}

				value_sql += ")";
				m_query[4] = value_sql;
				return *this;
			}
			compositor<T>& left_join(std::string_view table) {
				m_current_join = std::string("LEFT JOIN ")
				                 + sanitize_table(table) + " ";
				return *this;
			}
			compositor<T>& left_outer_join(std::string_view table) {
				m_current_join = std::string("LEFT OUTER JOIN ")
				                 + sanitize_table(table) + " ";
				return *this;
			}
			compositor<T>& right_join(std::string_view table) {
				m_current_join = std::string("RIGHT JOIN ")
				                 + sanitize_table(table) + " ";
				return *this;
			}
			compositor<T>& right_outer_join(std::string_view table) {
				m_current_join = std::string("RIGHT OUTER JOIN ")
				                 + sanitize_table(table) + " ";
				return *this;
			}
			compositor<T>& inner_join(std::string_view table) {
				m_current_join = std::string("INNER JOIN ")
				                 + sanitize_table(table) + " ";
				return *this;
			}
			compositor<T>& on(std::string_view lhs,
			                  std::string_view op,
			                  std::string_view rhs) {
				m_current_join += std::string(" ON ") + std::string(lhs) +
				                  std::string(op) + std::string(rhs);
				m_joins.push_back(m_current_join);
				m_current_join.clear();
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
				m_query[4] = sql;
				return *this;
			}
			compositor<T>& where(str_object lhs,
			                     str_object rhs) {
				std::string sql = " WHERE ";
				sql += lhs.data();
				sql += " = ";
				sql += m_txn_ptr->quote(std::string(rhs.data()));
				m_query[4] = sql;
				return *this;
			}
			compositor<T>& where(str_object lhs,
			                     const int64_t& rhs) {
				std::string sql = " WHERE ";
				sql += lhs.data();
				sql += " = ";
				sql += m_txn_ptr->quote(std::to_string(rhs));
				m_query[4] = sql;
				return *this;
			}

			compositor<T>& where(str_object lhs,
			                     const uint64_t& rhs) {
				std::string sql = " WHERE ";
				sql += lhs.data();
				sql += " = ";
				sql += m_txn_ptr->quote(std::to_string(rhs));
				m_query[4] = sql;
				return *this;
			}

			compositor<T>& where(str_object lhs,
			                     const uint32_t& rhs) {
				std::string sql = " WHERE ";
				sql += lhs.data();
				sql += " = ";
				sql += m_txn_ptr->quote(std::to_string(rhs));
				m_query[4] = sql;
				return *this;
			}

			compositor<T>& where(str_object lhs,
			                     const int32_t& rhs) {
				std::string sql = " WHERE ";
				sql += lhs.data();
				sql += " = ";
				sql += m_txn_ptr->quote(std::to_string(rhs));
				m_query[4] = sql;
				return *this;
			}

			compositor<T>& where(str_object lhs,
			                     const uint16_t& rhs) {
				std::string sql = " WHERE ";
				sql += lhs.data();
				sql += " = ";
				sql += m_txn_ptr->quote(std::to_string(rhs));
				m_query[4] = sql;
				return *this;
			}

			compositor<T>& where(str_object lhs,
			                     const int16_t& rhs) {
				std::string sql = " WHERE ";
				sql += lhs.data();
				sql += " = ";
				sql += m_txn_ptr->quote(std::to_string(rhs));
				m_query[4] = sql;
				return *this;
			}

			compositor<T>& where(str_object lhs,
			                     const uint8_t& rhs) {
				std::string sql = " WHERE ";
				sql += lhs.data();
				sql += " = ";
				sql += m_txn_ptr->quote(std::to_string(rhs));
				m_query[4] = sql;
				return *this;
			}


			compositor<T>& where(str_object lhs,
			                     const int8_t& rhs) {
				std::string sql = " WHERE ";
				sql += lhs.data();
				sql += " = ";
				sql += m_txn_ptr->quote(std::to_string(rhs));
				m_query[4] = sql;
				return *this;
			}

			compositor<T>& where_crypt(str_object lhs,
			                           str_object rhs) {
				std::string sql = " WHERE ";
				sql += lhs.data();
				sql += " = crypt(";
				sql += m_txn_ptr->quote(std::string(rhs.data()));
				sql += ", ";
				sql += m_txn_ptr->esc(std::string(lhs.data()));
				sql += ") ";
				m_query[4] = sql;
				return *this;
			}

			template <typename TIterable>
			compositor<T>& where_in(str_object lhs,
			                        TIterable id_list) {
				std::string sql = " WHERE ";
				sql += lhs.data();
				sql += " IN(";
				for(auto it = id_list.begin(); it != id_list.end(); ++it) {
					sql += m_txn_ptr->quote(std::string(*it));
					if((it + 1) != id_list.end()) {
						sql += ", ";
					}
				}
				sql += ") ";
				m_query[4] = sql;
				return *this;
			}


			compositor<T>& op_or(str_object lhs,
			                     str_object op,
			                     str_object rhs) {
				std::string sql = " OR ";
				sql += lhs.data();
				sql += " ";
				sql += op.data();
				sql += " ";
				sql += m_txn_ptr->quote(std::string(rhs.data()));
				m_query[4] += sql;
				return *this;
			}

			compositor<T>& op_or(str_object lhs,
			                     str_object rhs) {
				std::string sql = " OR ";
				sql += lhs.data();
				sql += " = ";
				sql += m_txn_ptr->quote(std::string(rhs.data()));
				m_query[4] += sql;
				return *this;
			}

			compositor<T>& op_and(str_object lhs,
			                      const std::string_view& rhs) {
				std::string sql = " AND ";
				sql += lhs.data();
				sql += " = ";
				sql += m_txn_ptr->quote(rhs.data());
				m_query[4] += sql;
				return *this;
			}
			compositor<T>& op_and(str_object lhs,
			                      const int16_t& rhs) {
				std::string sql = " AND ";
				sql += lhs.data();
				sql += " = ";
				sql += m_txn_ptr->quote(std::to_string(rhs));
				m_query[4] += sql;
				return *this;
			}

			compositor<T>& op_and(str_object lhs,
			                      const uint16_t& rhs) {
				std::string sql = " AND ";
				sql += lhs.data();
				sql += " = ";
				sql += m_txn_ptr->quote(std::to_string(rhs));
				m_query[4] += sql;
				return *this;
			}
			compositor<T>& op_and(str_object lhs,
			                      const uint32_t& rhs) {
				std::string sql = " AND ";
				sql += lhs.data();
				sql += " = ";
				sql += m_txn_ptr->quote(std::to_string(rhs));
				m_query[4] += sql;
				return *this;
			}
			compositor<T>& op_and(str_object lhs,
			                      const int32_t& rhs) {
				std::string sql = " AND ";
				sql += lhs.data();
				sql += " = ";
				sql += m_txn_ptr->quote(std::to_string(rhs));
				m_query[4] += sql;
				return *this;
			}
			compositor<T>& op_and(str_object lhs,
			                      const uint64_t& rhs) {
				std::string sql = " AND ";
				sql += lhs.data();
				sql += " = ";
				sql += m_txn_ptr->quote(std::to_string(rhs));
				m_query[4] += sql;
				return *this;
			}
			compositor<T>& op_and(str_object lhs,
			                      const int64_t& rhs) {
				std::string sql = " AND ";
				sql += lhs.data();
				sql += " = ";
				sql += m_txn_ptr->quote(std::to_string(rhs));
				m_query[4] += sql;
				return *this;
			}

			compositor<T>& op_and(str_object lhs,
			                      str_object op,
			                      str_object rhs) {
				std::string sql = " AND ";
				sql += lhs.data();
				sql += " ";
				sql += op.data();
				sql += " ";
				sql += m_txn_ptr->quote(std::string(rhs.data()));
				m_query[4] += sql;
				return *this;
			}
			compositor<T>& order_by(std::string column,std::string direction) {
				if(direction.compare("ASC") != 0) {
					direction = "DESC";
				}
				std::string sanitized_column;
				for(auto ch : column) {
					if(isspace(ch)) {
						continue;
					}
					if(!isalpha(ch) && ch != '.' && ch != '_' && !isdigit(ch)) {
						continue;
					}
					sanitized_column += ch;
				}

				m_order_by = " order by " + sanitized_column + " " + direction + " ";
				return *this;
			}

			compositor<T>& limit(const std::size_t count) {
				m_limit = " LIMIT " + m_txn_ptr->quote(std::to_string(count));
				return *this;
			}
			compositor<T>& clear() {
				m_current_join = m_table = m_sql = "";
				m_order_by = m_returning = m_limit = "";
				std::fill(m_query.begin(), m_query.end(), "");
				m_joins.clear();
				return *this;
			}
			compositor<T>& returning(str_object column) {
#ifdef __MENTOC_SHOW_SQL_DEBUG_OUTPUT__
				std::cerr << "[sql-compositor--returning]: status of sql: '" << m_sql << "'\n";
#endif
				m_returning = std::string(" RETURNING ") + column.data();
				return *this;
			}

			std::string sql();
		private:
			std::string m_sql;
			std::array<std::string, query_parts> m_query;
			std::vector<std::string> m_joins;
			std::string m_current_join;
			std::string m_order_by;
			std::string m_table;
			std::string m_returning;
			std::string m_limit;
			T* m_txn_ptr;
	};

	void error_log(std::string_view);
};

#endif
