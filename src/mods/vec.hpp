#ifndef __MENTOC_UTILS_VEC_HEADER__
#define __MENTOC_UTILS_VEC_HEADER__

#include <vector>

namespace vec {
	template <typename T, typename... Args>
	static inline std::vector<T>& append(std::vector<T>& dst, Args... args) {
		for(const auto& arg : {
		            args ...
		        }) {
			dst.insert(dst.end(),arg.begin(),arg.end());
		}
		return dst;
	};
};

#endif
