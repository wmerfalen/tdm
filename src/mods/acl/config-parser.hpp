#ifndef __CIRCLEMUD_SRC_MODS_ACL_PARSER_HEADER__
#define __CIRCLEMUD_SRC_MODS_ACL_PARSER_HEADER__ 1

#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <regex>
#include <tuple>
#include <map>
#include <algorithm>
#include "radix_tree/radix_tree.hpp"
#include "color.hpp"
#include "../util-map.hpp"
#include <cstdarg>  //for variadic function m_expect_chain(int ...)

#define dbg(f)  std::cout << Color::fg::green <<\
    "[" << Color::bg::blue << "debug: ]" << Color::bg::def << \
     Color::fg::blue << f << "\n" << Color::fg::def;
#define dm(m,prefix) for(auto i: m){ std::cout << prefix << i << "\n"; }
#define SKIP_WHITESPACE() \
    while(isspace(m_at())){\
        if(m_at() == '\n'){ m_line_number++; }\
        m_increment_file_offset(1);\
        if(m_get_file_offset() >= m_file_contents.length()){ break; }\
    }


namespace mods {
    namespace acl {
        class FileParser {

            public:
                /****************/
                /* Public types */
                /****************/
                /*
                 * The following examples are based off of a situation where we parse this example conf:
                 *
                 * -playable
                 *      allow -> 
                 *          files : [
                 *              act.wizard.c
                 *              act.offensive.c
                 *          ]
                 *      deny ->
                 *          files : [
                 *              main.c
                 *              foobar.c
                 *          ]
                 *
                 * We will assign a value to m_command_map as such:
                 *
                 * m_command_map["act.wizard.c"] = 1;
                 * m_command_map["act.offensive.c"] = 2; 
                 * m_command_map["main.c"] = 3;
                 * m_command_map["foobar.c"] = 4;
                 *
                 * rule playable;
                 * playable[m_command_map["act.wizard.c"]] = 1; //Allow act.wizard.c 
                 * playable[m_command_map["act.offensive.c"]] = 1; //Allow act.offensive.c 
                 * playable[m_command_map["main.c"]] = 0; //Deny main.c 
                 * playable[m_command_map["foobar.c"]] = 0; //Deny foobar.c 
                 *
                 * m_tree["playable"] = playable;
                 *
                 * ==================
                 * = Finding a rule =
                 * ==================
                 * bool FileParser::allowed(const char* role,const char* file_or_command){
                 *  return m_tree[role][m_command_map[file_or_command]];
                 * }
                 *
                 *
                 */
                typedef std::vector<bool>  rule;
                typedef radix_tree<std::string,rule> tree;
                typedef std::string cm_key;
                typedef uint64_t cm_value;
                typedef std::map<cm_key,cm_value> command_map;
                typedef uint64_t int_type;
                enum PARSE_TYPE { E_CLASS, E_EXTENDED_CLASS, E_COMMENT, E_BLOCK,    \
                    E_ACCESS_TYPE, E_ACCESS_START, E_SCOPE, E_LIST_START, E_STRING, \
                    E_ALLOW, E_DENY,\
                    E_LIST_END, E_COLON, E_EXTENDS, E_ARROW, E_FILES, E_COMMANDS,   \
                    E_DEFAULT\
                };
                enum ITEM_TYPE { ITEM_TYPE_FILES, ITEM_TYPE_COMMANDS };
                /* Constants */
                static const int PARSE_FAIL;
                static const int FILE_CANNOT_OPEN;
                static const int FILE_EOF;
                FileParser() : m_command_ctr(0),m_fp(nullptr),m_opened(false),m_file_offset(0),\
                               m_tentative_file_offset(0),\
                               m_line_number(1),m_dont_advance_file_offset(false){}
                ~FileParser();
                FileParser(const std::string &);
                void setFile(const char* f){m_file_name = f; }
                const char* getFile() const { return m_file_name.c_str(); }
                int read();
                int parse();
                inline void dump_tree(void){
                    for(auto leaf: m_tree){
                        //
                        blu_text(leaf.first << " ");
                        blu_text(leaf.second.size());
                        for(unsigned i=0;i < leaf.second.size();i++){
                            std::cout << "[command: " << \
                                ::mods::util::maps::keyval_first<cm_key,cm_value>(m_command_map,i,"none found") << "]: ";
                            if(leaf.second[i]){
                                grn_text("allow");
                            }else{
                                red_text("deny");
                            }
                            std::cout << "\n";
                        }
                        for(auto c: m_command_map){
                            blu_text("cmap: " << c.first << "-> " << c.second);
                        }
                    }
                };
            private:
                int m_accept_regex(const std::string &,std::vector<std::string>&);
                int m_accept_regex(const std::string &);
                int m_move_after_regex(const char*);
                int m_expect(PARSE_TYPE);
                int m_expect_chain(int,...);
                template <typename T>
                int m_expect_chain(const T &,std::vector<int>&);
                template <typename  T>
                T m_expect(PARSE_TYPE);
                int m_comment(void);
                int m_block(void);
                int m_access_rules(void);
                int m_arrow(void);
                int m_class(void);
                int m_extended_class(void);
                int m_default(void);
                std::tuple<int,bool> m_access_type(void);
                int m_colon(void);
                int m_extends(void);
                void m_access_start(void);
                int m_files(void);
                int m_commands(void);
                void m_scope(void);
                int m_list_start(void);
                int m_list_end(void);
                void m_string(void);

                int m_store_items(const std::string&);
                int m_store_extends(std::string&);

                std::vector<std::string> m_parse_list(void);


                /* Commands and rules */
                command_map m_command_map;
                tree m_tree;
                std::string m_current_class;
                std::string m_current_extended_class;
                cm_value m_command_ctr;
                PARSE_TYPE m_current_access_type;
                inline void m_register_commands(const std::vector<std::string>& vec,PARSE_TYPE type){
                    //TODO:
                    //Maybe in the future we will have 2 different structures. One for files
                    //and one for commands. In which case, `type` will play a role and decide
                    //which of those structures to register the command into
                    dbg("Parse type{{{" << type << "}}}");
                    for(auto& s:vec){
                        dbg("Finding string: " << s);
                        std::string cleaned = "";
                        for(auto sc:s){
                            if(sc == ']'){
                                continue;
                            }else{
                                cleaned += sc;
                            }
                        }
                        if(cleaned.length()){
                            if(m_command_map.find(cleaned.c_str()) == m_command_map.end()){
                                //We need to insert this into the command map
                                dbg("Didnt find string, inserting...");
                                m_command_map.insert(std::pair<cm_key,cm_value>(cleaned.c_str(),++m_command_ctr));
                            }else{
                                dbg("Found string: `" << cleaned.c_str() << "`. Skipping insert...");
                            }
                        }
                    }
                };
                inline void m_save_base_class(const std::vector<std::string>& vec){
                    //TODO: generate rules based on m_current_access_type being either allow or deny

                };

                inline void m_save_extended_class(const std::vector<std::string>& vec){
                    //TODO: generate rules based on m_current_access_type, m_current_extended_class, and m_current_class

                };
                command_map m_d_map;

                /* File properties */
                std::string m_file_name;
                std::ifstream m_fp;
                std::string m_file_contents;
                bool m_opened;
                size_t m_file_offset;
                size_t m_tentative_file_offset;
                int_type m_line_number;
                bool m_dont_advance_file_offset;

                inline bool m_out_of_bounds(void){ return m_get_file_offset() >= m_file_contents.length(); }
                void m_nextline(void);
                char m_next_char(void);
                int m_move_after(char);
                size_t m_get_file_offset(void);
                size_t m_increment_file_offset(size_t);
                inline void m_set_tentative_file_offset(size_t i){ m_tentative_file_offset = i; }
                inline void m_toggle_file_offset_advance(bool b){
                    m_dont_advance_file_offset = b;
                    if(b){
                        m_set_tentative_file_offset(0);
                    }else{
                        m_file_offset += m_tentative_file_offset;
                    }
                }

                /* Status and reporting functions */
                void m_report_line(const char* s){ printf(s,m_line_number + 1); }

                /* Utility functions */
                std::string m_enum_to_string(int);
                inline char m_at(size_t offset){ 
                    try{ return m_file_contents.at(offset); }
                    catch(std::out_of_range const& e){ return '\0'; } 
                }
                inline char m_at(void){ if(m_out_of_bounds()){ return '\0'; } return m_at(m_get_file_offset()); }
                inline std::string m_substr(void){
                    if(m_at() == '\0' || m_out_of_bounds()){
                        return "";
                    }
                    return m_file_contents.substr(m_get_file_offset());
                }
                inline void m_advance(size_t i){ if(m_out_of_bounds()){ return; } m_increment_file_offset(i); }
                /* Purely utility functions for development */
                inline void util_print_until(char c){ 
                    if(m_out_of_bounds()){ return; }
                    int i=m_get_file_offset(); 
                    while(m_at(i) != c){ std::cout << m_at(i++); }
                }
                inline void m20(){ if(m_out_of_bounds()){ dbg("m20: not printing"); return; }
                    std::cout << "[line:" << m_line_number << "]: ";
                    std::cout << m_file_contents.substr(m_get_file_offset(),20) << "\n"; }

        };

    };

};


#endif
