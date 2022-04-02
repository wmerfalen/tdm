#ifndef __MENTOC_CACHED_MAP_HEADER__
#define __MENTOC_CACHED_MAP_HEADER__
#include <iostream>
#include <map>
#include <set>

namespace mods {
	template <typename TKey,typename TValue>
	struct cached_map {
			/**
			 * Cached map will set the key once and never again.
			 * Repetitive setting results in a no-op.
			 */
			cached_map(std::map<TKey,TValue> in) :
				m_map(in) {}
			~cached_map() = default;
			cached_map() = default;

			bool has(const TKey& key) {
				return m_keys.contains(key);
			}
			void set_once(const TKey& key, const TValue& value) {
				if(has(key)) {
					return;
				}
				m_map[key] = value;
				m_keys.insert(key);
			}
			void unset(const TKey& key) {
				m_map.erase(key);
				m_keys.erase(key);
			}
			const auto& get(const TKey& key) {
				return m_map[key];
			}
			std::vector<std::string> get_report() {
				std::vector<std::string> lines;
				std::size_t map_size = 0,set_size = 0;
				for(const auto& pair : m_map) {
					lines.emplace_back(CAT("[key: '",pair.first,"']: [value.size():'",pair.second.length(),"']"));
					map_size += pair.first.length() + pair.second.length();
				}
				for(const auto& element : m_keys) {
					set_size += element.length();
				}
				lines.emplace_back(CAT("[m_keys.size():'",set_size,"']"));
				return lines;
			}

		private:
			std::set<TKey> m_keys;
			std::map<TKey,TValue> m_map;
	};
};

#endif
