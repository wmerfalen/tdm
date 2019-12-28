#ifndef __MENTOC_MODS_STRING_HEADER__
#define __MENTOC_MODS_STRING_HEADER__

#include <strings.h>
#include <iostream>
#include <string>
#include <string_view>
#undef PQXX_TR1_HEADERS
#include <pqxx/pqxx>
#define m_debug(a){;;} /* do{ std::cerr << "[m_debug]: " << a << "\n"; }while(0); */
namespace mods {
	struct string {
		string();
		string(const char* str);
		std::string_view view();
		std::string_view view() const;
		//string(const pqxx::tuple::reference & str);
		//string(const pqxx::result::reference & str);
#ifdef __MENTOC_USE_PQXX_RESULT__
		string(const pqxx::result::reference & str);
#else
		string(const pqxx::row::reference & str);
#endif
		string(const string&);
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
		void assign(const std::string & other);
		//void assign(const pqxx::tuple::reference & other);
#ifdef __MENTOC_USE_PQXX_RESULT__
		void assign(const pqxx::result::reference & other);
#else
		void assign(const pqxx::row::reference & other);
#endif
		const char* c_str() const;
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

