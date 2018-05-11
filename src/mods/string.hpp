#ifndef __MENTOC_MODS_string_HEADER__
#define __MENTOC_MODS_string_HEADER__

#include <strings.h>

namespace mods {
	struct string {
		string(){
			m_mallocd = false;
			m_cptr = nullptr;
		}
		~string(){
			if(m_mallocd){
				free(m_cptr);
			}
			m_mallocd = false;	
		};
		operator const char*() const { return m_str.length() == 0 ? nullptr : m_str.c_str(); }
		explicit operator std::string() const { return m_str; }
		string& operator=(string other){
			m_str = other.m_str;
			return *this;
		}
		string& operator=(const std::string & other){
			m_str = other;
			return *this;
		}
		void assign(const std::string & other){
			m_str = other;
			m_deallocate();
			m_allocate();
		}
		const char* c_str() const {
			return m_str.c_str();
		}
		unsigned int length() const {
			return m_str.length();
		}
		void clear(){
			m_str.clear();
		}
		char* ptr() {
			return m_cptr;
		}
		char** ptrptr(){
			m_allocate();
			return &m_cptr;
		}
		void concat(const std::string & str){
			m_str += str;
			m_deallocate();
			m_allocate();
		}
		void concat(int ch){
			m_str += ch;
			m_deallocate();
			m_allocate();
		}
		private:
			void m_deallocate(){
				if(m_mallocd){
					free(m_cptr);
					m_mallocd = false;
					m_size = 0;
				}
			}
			void m_allocate(){
				if(m_mallocd == false){
					m_cptr = static_cast<char*>(malloc(m_str.length() + 1));
					m_size = m_str.length();
					bcopy(m_str.c_str(),m_cptr,m_size);
					m_cptr[m_size] = '\0';
				}
			}
			std::string m_str;
			bool m_mallocd;
			char* m_cptr;
			std::size_t m_size;
	};
};
#endif

