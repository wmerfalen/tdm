#ifndef __MENTOC_PLAYER_UTILS_HEADER__
#define  __MENTOC_PLAYER_UTILS_HEADER__

#include <iostream>
#include <string>

namespace mods {
	namespace player {
		namespace util {
			typedef std::array<std::vector<std::string>,6> scan_results;
			template <typename ResultType>
			class search {
				public:
					search() = delete;
					search(const scan_results& s) : m_scan_results(s) {

					}
					ResultType get(const ResultType& _search) {
						for(auto direction : {
						            NORTH,EAST,SOUTH,WEST,UP,DOWN
						        }) {
							auto temp = m_scan_results[direction];

							if(auto ret = std::find(temp.begin(),temp.end(),_search)) {
								return ret;
							}
						}
						return reinterpret_cast<ResultType>(nullptr);
					}
				protected:
					scan_results m_scan_results;
			};

			std::optional<uuid_t> find_player_by_name(std::string_view name){
				return std::nullopt;
			}
		};
	};
};

#endif
