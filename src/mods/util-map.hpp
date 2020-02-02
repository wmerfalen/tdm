#ifndef __MENTOC_UTIL_MAP_HEADER__
#define  __MENTOC_UTIL_MAP_HEADER__

#include <iostream>
#include <map>
#include <unordered_map>
#include <string>
#include <vector>

namespace mods {
	namespace util {
		namespace maps {
			template <typename TMapType,typename TKeyType>
			static inline void remove_if_exists(TMapType* in_map,TKeyType key){
				auto it = in_map->find(key);
				if(it != in_map->end()){
					std::cerr << "removing map entry: " << key << "\n";
					in_map->erase(it);
				}
			}
			template <typename TKey,typename TVal>
			static inline void dump(std::map<TKey,TVal> m){
				for(auto & [k,v]: m){
					std::cout << "[" << k << "]->'" << v << "'\n";
				}
			}
			template <typename Tkey,typename Tval>
			Tkey keyval_first(std::map<Tkey,Tval>& m,const Tval value,Tkey default_value) {
				for(auto& i:m) {
					if(i.second == value) {
						return i.first;
					}
				}

				return default_value;
			}
			template <typename Tkey,typename Tval>
			Tval max(std::map<Tkey,Tval>& m) {
				Tval max_value = 0;

				for(auto& i:m) {
					if(i.second > max_value) {
						max_value = i.second;
					}
				}

				return max_value;
			}

		};
	};

};

#endif
