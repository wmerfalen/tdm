#ifndef __MENTOC_MODS_STRING_HEADER__
#define __MENTOC_MODS_STRING_HEADER__

#include <strings.h>
#include <iostream>
#include <string>
#define m_debug(a){;;} /* do{ std::cerr << "[m_debug]: " << a << "\n"; }while(0); */
namespace mods {
	struct string {
		string();
		string(const char* str);
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

