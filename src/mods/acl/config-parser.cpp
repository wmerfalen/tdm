#include "config-parser.hpp"
#include <algorithm>

template <typename T>
void remove_duplicates(std::vector<T>& vec)
{
      std::sort(vec.begin(), vec.end());
      vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
}

namespace mods {
    namespace acl {
        /*
         * File: 
         * #####################
         * # this is a comment #
         * #####################
         *
         * -playable
         *      deny ->
         *          files: [
         *              act.wizard.c
         *              main.c
         *              foo.c
         *           ]
         *           commands: [
         *              shutdown
         *           ]
         *      allow ->
         *          files: [
         *              act.offensive.c
         *          ]
         *
         * -player
         *      allow : extends playable -> 
         *      deny : extends playable ->
         *          
         */

        const int FileParser::PARSE_FAIL = -1;
        const int FileParser::FILE_CANNOT_OPEN = -2;
        const int FileParser::FILE_EOF = -3;

        static std::map<const char*,unsigned int> recursion;
        static std::stack<const char*> recursive_stack;
        inline void dbg_recursion_register(const char* str){
            recursive_stack.push(str);
        }
        inline void dbg_recursion(const char* str){
            if(recursive_stack.empty()){return;}
            if(strcmp(recursive_stack.top(),str) == 0){
                if(recursion.find(str) != recursion.end()){
                    recursion[str] = 1;
                }else{
                    recursion[str]++;
                }
                dbg("[[[recursion:::" << str << ":::{{COUNT}}-->" << recursion[str]);
            }
        }

        /* Destructor */
        FileParser::~FileParser(){
            if(m_opened){
                m_fp.close();
            }
        }

        int FileParser::read(){
            m_file_offset = 0;
            if(!m_opened){
                m_fp.open(m_file_name);
                m_opened = m_fp.is_open();
            }
            if(!m_opened){
                return FileParser::FILE_CANNOT_OPEN;
            }
            while(!m_fp.eof() && m_fp.good()){
                std::array<char,1024> buf;
                buf.fill(0);
                m_fp.getline(buf.data(),1024,'\n');
                m_file_contents += buf.data();
                m_file_contents += '\n';
            }
            return m_file_contents.length();
        }

        template <>
        std::tuple<int,bool> FileParser::m_expect<std::tuple<int,bool>>(PARSE_TYPE enum_id){
            return m_access_type();
        }

        int FileParser::parse(){
            m_block();
            return FileParser::PARSE_FAIL;
        }

        int FileParser::m_store_extends(std::string & klass_name){

            dbg("Storing extends...");
            return 0;
        }

        int FileParser::m_block(void){
            if(m_out_of_bounds()){ return 0;}
            if(m_comment()){
                dbg("m_block recursion call [found comment]");
                return m_block();
            }
            if(!m_expect(E_CLASS)){
                m_report_line("Expected class on line %d\n"); 
                return -1;
            }
            while(m_access_rules() > 0){}
            return m_block();
        }

        int FileParser::m_access_rules(void){
            if(m_out_of_bounds()){ return 0;}
            dbg_recursion("m_access_rules");
            m20();
            dbg(m_get_file_offset());
            auto t_expect = m_expect<std::tuple<int,bool>>(E_ACCESS_TYPE);
            if(!std::get<0>(t_expect)){
                m_report_line("Expected access type on line %d\n");
                return -1;
            }
            if(std::get<1>(t_expect)){
                m_current_access_type = E_ALLOW;
                dbg("Allow rule detected");
            }else{
                m_current_access_type = E_DENY;
                dbg("Deny rule detected");
            }
            dbg("Before expected colon");
            m20();
            //TODO: Make sure this is storing rules
            if(m_expect(E_COLON)){
                dbg("Colon found haha");
                if(m_expect(E_EXTENDS)){
                    m_toggle_file_offset_advance(false);
                    if(m_expect_chain(3,E_EXTENDED_CLASS,E_ARROW,E_DEFAULT)){
                        //Proceed to the next access type
                        m_toggle_file_offset_advance(true);
                        dbg("Toggle file offset advance");
                        m20();
                        dbg("Here comes the recursion");
                        dbg_recursion_register("m_access_rules");
                        return m_access_rules();
                    }
                }else{
                    m_report_line("Expected `extends` after `:` on line %d\n");
                    return -1;
                }
                if(m_expect(E_CLASS)){
                    dbg("Found extended class");
                    return 1;
                }else{
                    m_report_line("Expected a `class` name after `:` on line %d\n");
                    return -1;
                }
            }

            if(m_expect(E_ARROW)){
                /*
                 * An access rule can be blank (i.e.: it doesn't define files or commands).
                 * This can be useful if the user decides that the rule decides to extend 
                 * another rule. It may be the intention of the user to simply inherit (extend)
                 * another rule.
                 */
                dbg("Arrow detected");
                m20();
                /*
                 * The purpose of this do...while loop is to allow the user to specify
                 * commands or files in any order and as many times as they want to
                 */
                int com_result = 0, file_result = 0;
                do{
                    file_result = m_expect(E_FILES);
                    if(file_result){
                        if(m_expect_chain(2,E_COLON,E_LIST_START)){
                            auto list = m_parse_list();
                            dm(list,"List item(file): ");
                            m_register_commands(list,E_FILES);
                        }else{
                            m_report_line("[files] Invalid syntax on line %d\n");
                            return -1;
                        }
                    }
                    com_result = m_expect(E_COMMANDS);
                    if(com_result){
                        if(m_expect_chain(2,E_COLON,E_LIST_START)){
                            auto list = m_parse_list();
                            dm(list,"List item(command): ");
                            m_register_commands(list,E_COMMANDS);
                        }else{
                            m_report_line("[commands] Invalid syntax on line %d\n");
                            return -1;
                        }
                    }
                }while(com_result > 0 || file_result > 0);
                dbg("Outside of while com/file");
                return 1;
            }else{
                m_report_line("Expected `->` on line %d\n");
                return -1;
            }
            return 0;
        }

        template <typename T>
        int FileParser::m_expect_chain(const T &expect_list, std::vector<int> &in_return_values){
            int ctr =0;
            for(typename T::const_iterator i = expect_list.begin();
                    i != expect_list.end();++i){
                auto return_value = m_expect((PARSE_TYPE)(*i));
                in_return_values.push_back(return_value);
                if(return_value){
                    ctr++;
                }
            } 
            return ctr;
        }

        int FileParser::m_expect_chain(int count, ...){
            va_list args;
            va_start(args,count);
            int ctr = 0;
            for(int i=0;i < count; i++){
                int expect_enum_id = va_arg(args,int);
                if(!m_expect((PARSE_TYPE)expect_enum_id)){
                    dbg("report line error: " << expect_enum_id);
                    m_report_line(
                            std::string(
                             (
                              std::string("<chain>Expected `") + m_enum_to_string(expect_enum_id) 
                             ) + "` on line %d\n"
                            ).c_str()
                    );
                    return 0;
                }
                ctr++;
            } 
            va_end(args);
            return ctr;
        }

        std::string FileParser::m_enum_to_string(int enum_id){
            /*
             * enum PARSE_TYPE { E_CLASS, E_COMMENT, E_BLOCK, E_ACCESS_TYPE,       \
             *                     E_ACCESS_START, E_SCOPE, E_LIST_START, E_STRING, E_LIST_END,    \
             *                     E_COLON, E_EXTENDS, E_ARROW, E_FILES, E_COMMANDS };
             */

            switch(enum_id){
                case E_CLASS:
                    return "-class";
                case E_EXTENDED_CLASS:
                    return "class";
                case E_DEFAULT:
                    return "default";
                case E_COMMENT:
                    return "#";
                case E_BLOCK:
                    return "block";
                case E_ACCESS_TYPE:
                    return "allow|deny";
                case E_COLON:
                    return ":";
                case E_EXTENDS:
                    return "extends";
                case E_ARROW:
                    return "->";
                case E_FILES:
                    return "files";
                case E_COMMANDS:
                    return "commands";
                case E_LIST_START:
                    return "[";
                case E_LIST_END:
                    return "]";
                default:
                    return "<unknown>";
            }
        }


        int FileParser::m_colon(void){
            SKIP_WHITESPACE();
            if(m_at() == ':'){
                m_advance(1);
                return 1;
            }
            return 0;
        }

        int FileParser::m_move_after(char c){
            SKIP_WHITESPACE();
            while(m_get_file_offset() < m_file_contents.length()){
                if(m_at() == '\n'){
                    m_line_number++;
                }
                if(m_at() == c){
                    m_increment_file_offset(1);
                    return 1;
                }
                m_increment_file_offset(1);
            }
            return 0;
        }

        int FileParser::m_expect(PARSE_TYPE enum_id){
            SKIP_WHITESPACE();
            return m_expect<int>(enum_id);
        }

        int FileParser::m_default(void){
            SKIP_WHITESPACE();
            if(m_accept_regex("default")){
                m_increment_file_offset(strlen("default"));
                return 1;
            }
            return 0;
        }

        template <typename T>
        T FileParser::m_expect(PARSE_TYPE enum_id){
            SKIP_WHITESPACE();
            switch(enum_id){
                case E_CLASS:
                    return m_class();
                case E_EXTENDED_CLASS:
                    return m_extended_class();
                case E_DEFAULT:
                    return m_default();
                case E_EXTENDS:
                    return m_extends();
                case E_ARROW:
                    return m_arrow();
                case E_COLON:
                    return m_colon();
                case E_LIST_START:
                    return m_list_start();
                case E_LIST_END:
                    return m_list_end();
                case E_FILES:
                    return m_files();
                case E_COMMANDS:
                    return m_commands();
                default:
                    std::cerr << "Unhandled expect: " << enum_id << "\n";
                    break;
            }
            return 0;
        }

        int FileParser::m_list_start(void){
            if(m_at() == '['){
               m_move_after('[');
               return 1; 
            }
            return 0;
        }

        int FileParser::m_list_end(void){
            if(m_at() == ']'){
                m_move_after(']');
                return 1;
            }
            return 0;
        }

        int FileParser::m_extends(void){
            SKIP_WHITESPACE();
            if(m_accept_regex("extends")){
                m_increment_file_offset(strlen("extends"));
                return 1;
            }
            return 0;
        }

        int FileParser::m_files(void){
            if(m_accept_regex("files")){
                m_increment_file_offset(strlen("files"));
                return 1;
            }
            return 0;
        }

        int FileParser::m_commands(void){
            if(m_accept_regex("commands")){
                m_increment_file_offset(strlen("commands"));
                return 1;
            }
            return 0;
        }

        int FileParser::m_comment(void){
            dbg("Checking for comment");
            m20();
            SKIP_WHITESPACE();
            dbg("Skipped whitespace (in comment)");
            m20();
            if(m_at() == '#'){
                dbg("Comment found");
                m_move_after('\n');
                return 1;
            }
            return 0;
        }

        int FileParser::m_arrow(void){
            int ret = 0;
            if(m_at() == '-' && 
                    m_at(m_get_file_offset() +1) == '>'){
                m_increment_file_offset(2);
                ret = 1;
            }
            return ret;
        }

        int FileParser::m_store_items(const std::string & klass_name){
            return 0;
        }

        std::vector<std::string> FileParser::m_parse_list(){
            std::vector<std::string> list;
            std::vector<std::string> list_matches;
            do{
                std::string current_item = "";
                SKIP_WHITESPACE(); 
                /* Grab all non-whitespace characters */
                auto count = m_accept_regex("^([^[:space:]]+)",list_matches);
                for(auto i = 0; i < count; i++){
                    list.push_back(list_matches[i]);
                    dbg("List found{{{" << list_matches[i] << "}}}");
                }
                if(list.back().find("]") != std::string::npos || m_next_char() == ']'){
                    m_nextline();
                    break;
                }
                m_nextline();
            }while(m_get_file_offset() < m_file_contents.length());
            dm(list,"List: ");
            return list;
        }

        char FileParser::m_next_char(){
            SKIP_WHITESPACE();
            return m_at();
        }

        int FileParser::m_class(){
            std::vector<std::string> matches;
            if(m_accept_regex("^-([[:lower:]]+)",matches)){
                dbg("matched class: " << matches[0]);
                rule r;
                m_tree[matches[0]] = r;
                m_current_class = matches[0];
                m_increment_file_offset(matches[0].length() + 2);
                return 1;
            }
            return 0;
        }

        int FileParser::m_extended_class(){
            std::vector<std::string> matches;
            SKIP_WHITESPACE();
            if(m_accept_regex("^([[:lower:]]+)",matches)){
                dbg("matched [extended] class: " << matches[0]);
                //find if the class is already declared. 
                if(m_tree.find(matches[0]) != m_tree.end()){
                    //copy all the rules from the previous class into the current class
                    dbg("Saving extended class");
                    m_current_extended_class = matches[0];
                    m_increment_file_offset(matches[0].length() + 1);
                    m_tree[m_current_extended_class] = m_tree[matches[0]];
                    return 1;
                }else{
                    m_report_line("Cannot extend from a forward declaration on line %d\n");
                    return 0;
                }
            }
            return 0;
        }

        void FileParser::m_nextline(void){
            while(m_at() != '\n'){m_increment_file_offset(1); }
            m_line_number++;
        }

        std::tuple<int,bool> FileParser::m_access_type(void){
            m20();
            SKIP_WHITESPACE();
            m20();
            std::tuple<int,bool> ret;
            std::vector<std::string> matches;
            if(m_accept_regex("(allow)",matches)){
                dbg("Allow rule detected");
                m_advance(strlen("allow"));
                return std::make_tuple(1,true);
            }
            if(m_accept_regex("(deny)",matches)){
                    dbg("Deny rule detected");
                    m_advance(strlen("deny"));
                    return std::make_tuple(1,false);
            }
            return std::make_tuple(0,false);
        }


        int FileParser::m_accept_regex(const std::string & regex_string,std::vector<std::string> & holder){
            SKIP_WHITESPACE();
            dbg("Skipped white space in m_accept_regex");
            using namespace std::regex_constants;
            std::string target_string = m_substr();
            if(!target_string.length()){
                return 0;
            }
            m20();
            std::smatch our_matches;
            std::regex_search(target_string,our_matches,
                    std::regex(regex_string),
                    match_any | match_not_null | match_continuous);
            //dbg("Regex search: " <<  rs);
            dbg("our_matches.size(): " << our_matches.size());
            for(unsigned i = 1; i < our_matches.size();++i){
                holder.emplace_back(our_matches[i]);
            }
            return holder.size();
        }

        int FileParser::m_accept_regex(const std::string & regex_string){
            SKIP_WHITESPACE();
            using namespace std::regex_constants;
            std::string target_string = m_substr();
            if(!target_string.length()){
                return 0;
            }
            m20();
            bool b = std::regex_search(target_string,std::regex(regex_string),
                    match_any | match_not_null | match_continuous);
            dbg("Regex search: " <<  b);
            return (int)b;
        }

        int FileParser::m_move_after_regex(const char* regex_string){
            return 0;
        }

        size_t FileParser::m_increment_file_offset(size_t i){
            if(m_dont_advance_file_offset){
                m_tentative_file_offset += i;
                return m_file_offset + m_tentative_file_offset;
            }else{
                m_file_offset += i;
                return m_file_offset;
            }
        }

        size_t FileParser::m_get_file_offset(){
            return m_increment_file_offset(0);
        }

    };

};


