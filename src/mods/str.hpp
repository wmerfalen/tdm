#ifndef __MENTOC_STRUTIL_HEADER__
#define  __MENTOC_STRUTIL_HEADER__

#include "../globals.hpp"

struct str {
	str() = delete;
	str(auto s) : m_str(CAT(s)) {}
	~str() = default;
	bool is(std::string_view d) {
		return m_str.compare(d.data()) == 0;
	}
	auto c_str() const {
		return m_str.c_str();
	}
	std::string m_str;

	static inline bool is(std::string_view s,std::string_view d) {
		return s.compare(d.data()) == 0;
	}
	static inline std::string alpha_only(std::string_view s) {
		std::string m;
		for(const auto ch : s) {
			if(isalpha(ch)) {
				m += ch;
			}
		}
		return m;
	}

	static inline std::string tolower(std::string_view _s) {
		std::string s = _s.data();
		std::transform(s.begin(), s.end(), s.begin(),
		[](unsigned char c) {
			return std::tolower(c);
		});
		return s;
	}

	static inline bool ends_with(std::string_view s, const std::string& hay) {
		auto f = hay.find(s);
		if(f == std::string::npos) {
			return false;
		}
		auto tmp = hay.substr(f);
		if(tmp.compare(s.data()) == 0) {
			return true;
		}
		return false;
	}
	static inline bool starts_with_lower(std::string_view s, const std::string& hay) {
		auto slower = tolower(s);
		auto hlower = tolower(hay);
		return hlower.find(slower) != std::string::npos;
	}
	static inline bool starts_with(std::string_view s, const std::string& hay) {
		return hay.find(s) != std::string::npos;
	}
	static inline std::string until(char c,const std::string& hay) {
		auto f = hay.find(c);
		if(f == std::string::npos) {
			return hay;
		}
		if(f+1 >= std::distance(hay.cbegin(),hay.cend())) {
			return hay;
		}
		return hay.substr(0,f);
	}
	static inline bool starts_with_any(std::vector<std::string>&& m,const std::string& hay) {
		for(const auto& str : m) {
			if(starts_with(str,hay)) {
				return true;
			}
		}
		return false;
	}
	static inline bool is_floatval(std::string_view s) {
		auto period = s.find_first_of(".") != std::string::npos;
		auto digit_count = 0;
		for(auto ch : s) {
			if(ch != '.' && isalpha(ch)) {
				return false;
			}
			if(isdigit(ch)) {
				++digit_count;
			}
		}
		return period && digit_count > 0;
	}
	static inline std::string chop_after_2nd_place(std::string_view s) {
		bool found_decimal = false;
		auto i = 0;
		std::string str;
		for(auto ch : s) {
			str += ch;
			if(ch == '.') {
				found_decimal = true;
				continue;
			}
			if(found_decimal && ++i == 2) {
				break;
			}
		}
		return str;
	}
	template <typename TStr>
	static inline TStr strip_color(TStr s) {
		return mods::colors::strip_color(s);
	}
	bool match_any(std::string_view src,std::vector<std::string> any_of_these,std::size_t max_ch);

	bool is_lower_match(std::string_view str1,std::string_view str2);
	bool match_any_lower(std::string_view src,std::vector<std::string> any_of_these,std::size_t max_ch);

	template <typename T>
	static inline bool nth_is_any(std::size_t n,const std::vector<T> vec_args,const std::vector<T>& list_string) {
		if(vec_args.size() <= n) {
			return false;
		}
		return std::any_of(list_string.cbegin(),list_string.cend(),[&](const auto & token) -> bool {
			return is_lower_match(token,vec_args[n]);
		});
	}
	template <typename T>
	static inline bool nth_is(std::size_t n,const std::vector<T> vec_args,std::string_view search) {
		if(vec_args.size() <= n) {
			return false;
		}
		return is_lower_match(search.data(),vec_args[n]);
	}
	using strmap_t = std::map<std::string,std::string>;
};

//template <typename TStr>
//static inline auto str(TStr s) {
//	return StrUtil(s);
//}

#endif
