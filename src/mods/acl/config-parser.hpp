#ifndef __CIRCLEMUD_SRC_MODS_ACL_PARSER_HEADER__
#define __CIRCLEMUD_SRC_MODS_ACL_PARSER_HEADER__ 1

#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <map>
#include "radix_tree/radix_tree.hpp"

namespace mods {
    namespace acl {
        class FileParser {

            public:
                typedef struct { uint64_t rule_a; uint64_t rule_b; } rule;
                typedef radix_tree<std::string,rule> tree;
                typedef std::map<uint64_t,char*> command_map;
                static const int PARSE_FAIL;
                static const int FILE_CANNOT_OPEN;
                FileParser() : m_fp(nullptr),m_opened(false),m_symbol(NULL),m_file_offset(0){}
                ~FileParser();
                FileParser(const std::string &);
                void setFile(const char* f){m_file_name = f; }
                const char* getFile() const { return m_file_name.c_str(); }
                int read();
                int parse();
            private:
                int m_accept_regex(const std::string &,std::smatch&);
                int m_expect(char);
                int m_comment(void);
                void m_block(void);
                int m_class(void);
                int m_access_type(void);
                void m_access_start(void);
                void m_scope(void);
                void m_list_start(void);
                void m_string(void);
                void m_list_end(void);

                void m_nextline(void);
                void m_move_after(char);

                /* Commands and rules */
                command_map m_command_map;
                tree m_tree;

                /* File properties */
                std::string m_file_name;
                std::ifstream m_fp;
                int m_file_offset;
                std::string m_file_contents;
                bool m_opened;

                /* Purely utility functions for development */
                inline void util_print_until(char c){ int i=m_file_offset; while(m_file_contents[i] != c){ std::cout << m_file_contents[i++]; }}
        };

    };

};


#endif
