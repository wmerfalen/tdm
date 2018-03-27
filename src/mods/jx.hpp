#ifndef __MENTOC_MODS_JX_HEADER__
#define __MENTOC_MODS_JX_HEADER__
#include <map>
#include <vector>
#include <iostream>
/*
 * Example usage:
 *       mods::jx::compositor jx;
 *       std::cout << jx.push("foo","bar")
 *               .object_start("x")
 *                       .push("a","b")
 *                       .push("foo",10.00)
 *                       .array_start("times")
 *                               .push("10")
 *                               .push(10)
 *                               .push(100.10)
 *                       .array_end()
 *               .object_end()
 *       .get();
 *
 ** The above prints:
 * {foo:"bar",x:{a:"b",foo:"10.000000",times:["10",10,100.100000]}}
 */
namespace mods::jx {
	struct compositor {
			compositor() : m_current_level(0) {
				object_start("");
			}
			~compositor() = default;

			std::string str_escape(const std::string& value) {
				std::string after;
				after.reserve(value.length() + (value.length() * 0.10));

				for(std::size_t i = 0; i < value.length(); i++) {
					switch(value[i]) {
						case '"':
						case '\\':
							after += '\\';
							break;

						default:
							after += value[i];
							break;
					}
				}

				return after;
			}
			compositor& push(bool value) {
				if(m_levels[m_current_level]) {
					m_parts.push_back(",");
				}

				if(value) {
					m_parts.push_back("true");
				} else {
					m_parts.push_back("false");
				}

				m_levels[m_current_level]++;
				return *this;
			}
			compositor& push(const char* value) {
				return push(std::string(value));
			}
			compositor& push(std::nullptr_t value) {
				if(m_levels[m_current_level]) {
					m_parts.push_back(",");
				}

				m_parts.push_back("null");
				m_levels[m_current_level]++;
				return *this;
			}
			compositor& push(int value) {
				if(m_levels[m_current_level]) {
					m_parts.push_back(",");
				}

				m_parts.push_back(std::to_string(value));
				m_levels[m_current_level]++;
				return *this;
			}
			compositor& push(float value) {
				if(m_levels[m_current_level]) {
					m_parts.push_back(",");
				}

				m_parts.push_back(std::to_string(value));
				m_levels[m_current_level]++;
				return *this;
			}
			compositor& push(double value) {
				if(m_levels[m_current_level]) {
					m_parts.push_back(",");
				}

				m_parts.push_back(std::to_string(value));
				m_levels[m_current_level]++;
				return *this;
			}
			compositor& push(const std::string& value) {
				if(m_levels[m_current_level]) {
					m_parts.push_back(",");
				}

				m_parts.push_back(std::string("\"") + str_escape(value) + std::string("\""));
				m_levels[m_current_level]++;
				return *this;
			}
			compositor& push(const std::string& key, double value) {
				push(key,std::to_string(value));
				return *this;
			}
			compositor& push(const std::string& key, int value) {
				push(key,std::to_string(value));
				return *this;
			}
			compositor& push(const std::string& key, unsigned int value) {
				push(key,std::to_string(value));
				return *this;
			}
			compositor& push(const std::string& key, const std::string& value) {
				if(m_levels[m_current_level]) {
					m_parts.push_back(",");
				}

				m_parts.push_back(key + std::string(":\"") +  str_escape(value) + std::string("\""));
				m_levels[m_current_level]++;
				return *this;
			}
			compositor& push(const std::string& key, float value) {
				push(key,std::to_string(value));
				return *this;
			}
			compositor& object_start(const std::string& key) {
				if(m_levels[m_current_level]) {
					m_parts.push_back(",");
				}

				m_levels[m_current_level]++;

				++m_current_level;

				if(key.length() == 0) {
					m_parts.push_back("{");
				} else {
					m_parts.push_back(key + std::string(":{"));
				}

				return *this;
			}
			compositor& object_end() {
				m_levels[m_current_level] = 0;
				--m_current_level;
				m_parts.push_back("}");
				return *this;
			}
			compositor& array_start(const std::string& key) {
				if(m_levels[m_current_level]) {
					m_parts.push_back(",");
				}

				++m_current_level;

				if(key.length() == 0) {
					m_parts.push_back("[");
				} else {
					m_parts.push_back(key + ":[");
				}

				return *this;
			}
			compositor& array_end() {
				m_levels[m_current_level] = 0;
				--m_current_level;
				m_parts.push_back("]");
				return *this;
			}

			std::string get() {
				object_end();
				m_jx = "";

				for(auto& str: m_parts) {
					m_jx += str;
				}

				return m_jx;
			}
		private:
			uint32_t m_current_level;
			std::string m_jx;
			std::vector<std::string> m_parts;
			std::map<uint32_t,uint32_t> m_levels;
	};

};

#endif
