#ifndef __MENTOC_MODS_STRING_HEADER__
#define __MENTOC_MODS_STRING_HEADER__

#include <strings.h>
#include <iostream>
#include <string>
#include <string_view>
#undef PQXX_TR1_HEADERS
#include <pqxx/pqxx>

#ifdef __MENTOC_USE_PQXX_RESULT__
	#define mentoc_pqxx_result_t pqxx::result::reference
#else
	#define mentoc_pqxx_result_t pqxx::row
#endif

namespace mods {
	struct string {
		string();
		string(const char* str);
		std::string_view view();
		std::string_view view() const;
		string(mentoc_pqxx_result_t);
		string(const string&);
		string(mentoc_pqxx_result_t::reference);
		~string();
		operator bool() const { return m_str.length(); }
		operator char*() const { return const_cast<char*>(c_str()); }
		explicit operator const char*() const { return m_str.length() == 0 ? nullptr : m_str.c_str(); }
		explicit operator std::string() const { return m_str; }
		string& operator=(const char* other);
		string& operator=(char* other);
		string& operator=(string other);
		string& operator=(const std::string & other);
		const char* operator*(){ return m_str.c_str(); }
		void assign_m(const mods::string & other);
		void assign(const std::string & other);
		void assign(nullptr_t a);
		void assign(mentoc_pqxx_result_t);
		void assign(mentoc_pqxx_result_t::reference);
		const char* c_str() const;
		std::string str() const;
		std::string str_or(std::string f) const;
		unsigned int length() const;
		void clear();
		char* ptr() {
			return m_cptr;
		}
		char** ptrptr(){
			m_allocate();
			return &m_cptr;
		}
		void concat(const std::string & str);
		void concat(int ch);
		private:
			void m_deallocate();
			void m_allocate();
			void m_realloc();
			std::string m_str;
			bool m_mallocd;
			char* m_cptr;
			std::size_t m_size;
	};
};
#endif

