#ifndef __MENTOC_PLAYER_UTILS_HEADER__
#define  __MENTOC_PLAYER_UTILS_HEADER__

#include <iostream>
#include <string>

namespace mods::globals {
	extern std::map<std::string,player_ptr_t> player_name_map;
};
namespace mods {
	namespace players {
		namespace util {
#if 0
			typedef std::string sr_string_type;
			typedef std::array<std::vector<sr_string_type>,6> scan_results;
			template <typename ResultType>
			class search {
				public:
					search() = default;
					~search() = default;
					void set(const scan_results& s) {
						m_scan_results = s;
					}
					int get(const ResultType& _search,ResultType& _result {

						for(auto direction : {
						            NORTH,EAST,SOUTH,WEST,UP,DOWN
						        }) {
							if(auto ret = std::find(m_scan_results[direction].begin(),m_scan_results[direction].end(),_search)) {
								_result = ret;
								return 1;
							}
						}
						return nullptr;
					}
					protected:
					scan_results m_scan_results;
			};

#endif
			std::optional<uuid_t> find_player_by_name(std::string_view name){
				if(mods::globals::player_name_map.find(name.data()) != mods::globals::player_name_map.end()){
					auto & player = mods::globals::player_name_map[name.data()];
					if(!player->authenticated()){
						return std::nullopt;
					}
					return player->uuid();
				}
				return std::nullopt;
			}
		};
	};
};

#endif
