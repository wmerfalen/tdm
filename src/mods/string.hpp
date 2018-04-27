#ifndef __MENTOC_MODS_string_HEADER__
#define __MENTOC_MODS_string_HEADER__

namespace mods {
	struct string {
		string() = default;
		~string() = default;
		operator char*() const { return m_str.length() == 0 ? nullptr : m_str.c_str(); }
		explicit operator std::string() const { return m_str; }
		string& operator=(string other){
			m_str = other.m_str;
			return *this;
		}
		string& operator=(const std::string & other){
			m_str = other;
			return *this;
		}
		const char* c_str() const {
			return m_str.c_str();
		}
		private:
		std::string m_str;
	};
};
#endif

