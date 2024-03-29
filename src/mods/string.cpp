#include "string.hpp"
#include <string.h>
#include <pqxx/pqxx>
#define m_debug(a) ;; //do{  std::cerr << "[m_debug]: " << a << "\n"; }while(0);
namespace mods {
	/**
	 * How might we implement a mods::string class that would not duplicate a string
	 * if that string has already been allocated? Additionally, any new instantiations
	 * of mods::string that are a duplicate of an already existing string should instead
	 * have their internal pointer pointed to that existing string?
	 *
	 * There is also a catch: We only want to point to an existing string if the calling
	 * code explicitly told us that it's okay to point to an existing string. For this to
	 * happen, let's allow for there to be a function called allow_duplicates(bool b). If the
	 * user instantiates this class with the following new constructor, let's allow for
	 * the second parameter to be passed to allow_duplicates(bool):
	 * string::string(const string& str, bool allow_duplicates)
	 */
	string::string(const string& str) {
		allow_duplicates(true);
		m_debug("string:;string const string& copy");
		m_mallocd = false;
		m_cptr = nullptr;
		m_str = "";
		std::string other = str.c_str();
		if(other.length()) {
			assign(other.c_str());
		} else {
			assign("");
		}
	}
	std::string_view string::view() {
		m_debug("grabbing view");
		return m_str;
	}
	std::string_view string::view() const {
		m_debug("grabbing view const version");
		return m_str;
	}
	string::string(mentoc_pqxx_result_t::reference other) {
		allow_duplicates(true);
		m_debug("str reference other");
		assign(other.c_str());
	}
	string::string() {
		allow_duplicates(true);
		m_debug("str no arg");
		m_mallocd = false;
		m_cptr = nullptr;
		m_str = "";
		//std::cerr << "mods::string::string constructor\n";
		assign(std::string(""));
	}
	string::string(mentoc_pqxx_result_t str) {
		allow_duplicates(true);
		m_debug("str pqxx result");
		m_mallocd = false;
		m_cptr = nullptr;
		assign(str);
	}
	string::string(const char* str) {
		allow_duplicates(true);
		m_debug("str const char*");
		m_mallocd = false;
		m_cptr = nullptr;
		assign(str);
		//std::cerr << "mods::string::string(const char*) constructor\n";
	}
	string::~string() {
		m_deallocate();
		m_str.clear();
	};
	unsigned int string::length() const {
		if(!m_mallocd) {
			return 0;
		}
		return m_str.length();
	}
	void string::clear() {
		m_debug("str clear");
		///std::cerr <<"m_str: " << m_str.c_str() << "\n";
		m_deallocate();
		m_str.clear();
		/*
		m_str = "";
		if(m_mallocd){
			free(m_cptr);
		}
		m_cptr = nullptr;
		m_mallocd = false;
		*/
	}
	string& string::operator=(const char* other) {
		allow_duplicates(true);
		m_debug("str op equals const char* other");
		assign(other);
		return *this;
	}
	string& string::operator=(char* other) {
		allow_duplicates(true);
		m_debug("str op equals other");
		if(other) {
			assign(std::string(other));
		} else {
			assign("");
		}
		return *this;
	}
	string& string::operator=(string other) {
		allow_duplicates(true);
		m_debug("str op equals string other");
		if(other.m_str.length()) {
			assign(other.m_str);
		} else {
			assign("");
		}
		return *this;
	}
	string& string::operator=(const std::string& other) {
		allow_duplicates(true);
		m_debug("str op equals const std::string & other");
		if(other.length()) {
			m_str.assign(std::string(other.c_str()));
		} else {
			assign("");
		}
		return *this;
	}
	void string::assign(mentoc_pqxx_result_t other) {
		allow_duplicates(true);
		m_debug("str op equals pqxx result t other");
		assign(other[0].c_str());
	}
	void string::assign(nullptr_t a) {
		allow_duplicates(true);
		m_debug("assign (nullptr_t)");
		assign(std::string(""));
	}
	void string::assign(mentoc_pqxx_result_t::reference str) {
		allow_duplicates(true);
		m_debug("assign (pqxx::row::reference)");
		assign(pqxx::to_string(str).c_str());
	}
	void string::assign_m(const mods::string& other) {
		allow_duplicates(other.m_allow_duplicates);
		m_debug("assign std::string\n");
		m_debug(other.c_str());
		m_debug("end of other.c_str()");
		//std::cerr << "strlen of other: " << strlen(other.c_str()) << "\n";
		if(other.length()) {
			m_str.assign(other.c_str());
		} else {
			m_str = "";
		}
		m_debug("after m_str assignment");
		m_realloc();
	}
	void string::assign(const std::string& other) {
		allow_duplicates(true);
		m_debug("assign std::string\n");
		m_debug(other.c_str());
		m_debug("end of other.c_str()");
		//std::cerr << "strlen of other: " << strlen(other.c_str()) << "\n";
		if(other.length()) {
			m_str.assign(other.c_str());
		} else {
			m_str = "";
		}
		m_debug("after m_str assignment");
		m_realloc();
	}
	const char* string::c_str() const {
		m_debug("str c_str() const");
		m_debug("returning c_str of m_str: '" << m_str.c_str() << "'");
		if(m_str.length()) {
			return m_str.c_str();
		} else {
			return "";
		}
	}
	std::string string::str() const {
		m_debug("str str() const");
		if(m_str.length()) {
			return m_str;
		} else {
			return "";
		}
	}
	std::string string::str_or(std::string f) const {
		m_debug("str str_or(" << f << ") const");
		if(m_str.length()) {
			return m_str;
		}
		return f;
	}

	void string::concat(const std::string& str) {
		m_debug("concat: '" << m_str << "' += '" << str << "'");
		if(str.length()) {
			m_str.append(str);
			m_realloc();
		} else {
			assign(str);
		}
	}
	void string::concat(int ch) {
		m_str += std::to_string(ch);
		m_realloc();
	}
	void string::m_deallocate() {
		m_debug("m_deallocate entrance");
		if(m_mallocd) {
			m_debug("mallocd, lets free it");
			free(m_cptr);
			m_mallocd = false;
			m_size = 0;
		}
	}
	void string::m_allocate() {
		if(m_mallocd == false) {
			m_debug("inside m_allocate. running: static_cast");
			m_cptr = static_cast<char*>(malloc(m_str.length() + 1));
			m_debug("m_allocate: casted");
			m_size = m_str.length();
			bcopy(m_str.c_str(),m_cptr,m_size);
			m_cptr[m_size] = '\0';
			m_debug("m_allocate: terminated with null byte");
			m_debug("m_cptr: " << m_cptr);
			m_mallocd = true;
		}
	}
	void string::m_realloc() {
		m_debug("[REALLOC-begin]");
		m_deallocate();
		m_allocate();
		m_debug("[REALLOC-END]");
	}
	void string::allow_duplicates(bool allow) {
		m_allow_duplicates = allow;
	}
	bool string::allows_duplicates() const {
		return m_allow_duplicates;
	}
};
