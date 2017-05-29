#include "config-parser.hpp"

#define SKIP_WHITESPACE() if(isspace(m_file_contents[m_file_offset])){\
    while(++m_file_offset < m_file_contents.length() && \
            isspace(m_file_contents[m_file_offset]))\
    {\
        std::cout << "is space @" << m_file_offset << "\n";\
    }\
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

        int FileParser::parse(){
            m_block();
            return FileParser::PARSE_FAIL;
        }

        void FileParser::m_block(void){
            if(m_comment()){
                std::cout << "Found comment: ";
                util_print_until('\n');
                std::cout << "\n";
                m_nextline();
                m_block();
                return;
            }
            if(m_class()){
                if(m_access_type()){


                }
            }

        }

        int FileParser::m_comment(void){
            SKIP_WHITESPACE();
            std::cout << "checking if comment\n";
            std::cout << "m_fc: " << m_file_contents[m_file_offset] << "\n";
            if(m_file_contents[m_file_offset] == '#'){
                return 1;
            }
            return 0;
        }

        void FileParser::m_move_after(char c){
            while(m_file_offset < m_file_contents.length() && m_file_contents[m_file_offset++] != c){};
            m_file_offset++;
        }

        int FileParser::m_class(){
            std::smatch matches;
            if(m_accept_regex("\\-{1}([a-z0-9A-Z]+)",matches)){
                std::cout << "-" << matches[1] << "\n";
            }
        }

        void FileParser::m_nextline(void){
            while(m_file_contents[m_file_offset++] != '\n'){}
        }

        int FileParser::m_access_type(void){
            return 1;
        }

        int FileParser::m_accept_regex(const std::string & regex_string,std::smatch & matches){
            SKIP_WHITESPACE();
            std::regex e(regex_string);
            std::string s = m_file_contents.substr(m_file_offset);
            std::cout << "file contents: " << s << "\n";
            return std::regex_search(s,matches,e) - 1;
        }

    };

};


