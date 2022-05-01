#ifndef __MENTOC_INDEXED_STRING_HEADER__
#define __MENTOC_INDEXED_STRING_HEADER__
#include <iostream>
#include <vector>
#include <string>
#include <set>

namespace mods {
	struct indexed_string {
			/**
			 * Cached map will set the key once and never again.
			 * Repetitive setting results in a no-op.
			 */
			~indexed_string() = default;
			indexed_string();

			bool has(const std::string& key) {
				return m_keys.contains(key);
			}
			void set_once(const std::string& key, const std::string& value) {
				if(has(key)) {
					return;
				}
				m_pointers.emplace_back(malloc(value.size() + 1));
				auto& ptr = m_pointers.back();
				memcpy(ptr,value.data(),value.size());
				m_keys.insert(key);
			}
			void unset(const std::string& key) {
				m_pointers
				.erase(
				std::remove_if(m_pointers.begin(), m_pointers.end(), [&](auto& ptr) -> bool {
					return strcmp(key.c_str(),ptr) == 0;
				}),
				m_pointers.end()
				);

				m_keys.erase(key);
			}
			const auto& get(const std::string& key) {
				for(const auto& ptr : m_pointers) {

				}
				return
			}
			std::vector<std::string> get_report() {
				std::vector<std::string> lines;
				std::size_t map_size = 0,set_size = 0;
				//for(const auto& pair : m_map) {
				//	lines.emplace_back(CAT("[key: '",pair.first,"']: [value.size():'",pair.second.length(),"']"));
				//	map_size += pair.first.length() + pair.second.length();
				//}
				//for(const auto& element : m_keys) {
				//	set_size += element.length();
				//}
				//lines.emplace_back(CAT("[m_keys.size():'",set_size,"']"));
				return lines;
			}

		private:
			std::set<std::string> m_keys;
			std::vector<char*> m_pointers;
	};
};

#endif
