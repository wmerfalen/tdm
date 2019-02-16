#include "string.hpp"
#include <string.h>
//#define m_debug(a) ;; /* do{ std::cerr << "[m_debug]: " << a << "\n"; }while(0); */
namespace mods {
	string::string(const string& str){
		m_mallocd = false;
		m_cptr = nullptr;
		m_str = "";
		std::string other = str.c_str();
		if(other.length()){
			assign(other.c_str());
		}else{
			assign("");
		}
	}
	string::string(const pqxx::tuple::reference & str){
		m_mallocd = false;
		m_cptr = nullptr;
		m_str = "";
		std::string other = str.c_str();
		if(other.length()){
			assign(other.c_str());
		}else{
			assign("");
		}
	}
	string::string(){
		m_mallocd = false;
		m_cptr = nullptr;
		m_str = "";
		std::cerr << "mods::string::string constructor\n";
		assign(std::string(""));
	}
	string::string(const char* str){
		m_mallocd = false;
		m_cptr = nullptr;
		assign(str);
		//std::cerr << "mods::string::string(const char*) constructor\n";
	}
	string::~string(){
		if(m_mallocd){
			free(m_cptr);
		}
		m_mallocd = false;	
	};
	unsigned int string::length() const {
		if(!m_mallocd){
			return 0;
		}
		return m_str.length();
	}
	void string::clear(){
		///std::cerr <<"m_str: " << m_str.c_str() << "\n";
		assign("");
		/*
		m_str = "";
		if(m_mallocd){
			free(m_cptr);
		}
		m_cptr = nullptr;
		m_mallocd = false;
		*/
	}
	string& string::operator=(const char* other){
		assign(other);
		return *this;
	}
	string& string::operator=(char* other){
		if(other){
			assign(std::string(other));
		}else{
			assign("");
		}
		return *this;
	}
	string& string::operator=(string other){
		if(other.m_str.length()){
			assign(other.m_str);
		}else{
			assign("");
		}
		return *this;
	}
	string& string::operator=(const std::string & other){
		if(other.length()){
			m_str.assign(std::string(other.c_str()));
		}else{
			assign("");
		}
		return *this;
	}
	void string::assign(const pqxx::tuple::reference & other){
		assign(std::string(other.c_str()));
	}
	void string::assign(const std::string & other){
		m_debug("assign std::string\n");
		m_debug(other.c_str());
		m_debug("end of other.c_str()");
		//std::cerr << "strlen of other: " << strlen(other.c_str()) << "\n";
		if(other.length()){
			m_str.assign(std::string(other.c_str()));
		}else{
			m_str = "";
		}
		m_debug("after m_str assignment");
		m_realloc();
	}
	const char* string::c_str() const {
		//std::cerr << "returning c_str of m_str: << " << m_str.c_str() << "\n";
		if(m_str.length()){
			return m_str.c_str();
		}else{
			return "";
		}
	}
	void string::concat(const std::string & str){
		if(str.length()){
			m_str.append(str);
			m_realloc();
		}else{
			assign(str);
		}
	}
	void string::concat(int ch){
		m_str += std::to_string(ch);
		m_realloc();
	}
	void string::m_deallocate(){
		m_debug("m_deallocate entrance");
		if(m_mallocd){
			m_debug("mallocd, lets free it");
			free(m_cptr);
			m_mallocd = false;
			m_size = 0;
		}
	}
	void string::m_allocate(){
		if(m_mallocd == false){
			m_debug("inside m_allocate. running: static_cast");
			m_cptr = static_cast<char*>(malloc(m_str.length() + 1));
			m_debug("m_allocate: casted");
			m_size = m_str.length();
			bcopy(m_str.c_str(),m_cptr,m_size);
			m_cptr[m_size] = '\0';
			m_debug("m_allocate: terminated with null byte");
		}
	}
	void string::m_realloc(){
		m_debug("[REALLOC-begin]");
		m_deallocate();
		m_allocate();
		m_debug("[REALLOC-END]");
	}
};
