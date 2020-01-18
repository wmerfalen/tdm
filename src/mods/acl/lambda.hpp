#ifndef __CIRCLEMUD_SRC_MODS_ACL_LAMBDA_HEADER__
#define __CIRCLEMUD_SRC_MODS_ACL_LAMBDA_HEADER__ 1

#include <iostream>
#include <string>
#include <functional>
#include "../../structs.h"
#include "../../globals.hpp"
namespace mods {
    namespace acl {
        class Lambda {
            public:
				Lambda()= default;
				Lambda(std::function<void (char_data*,char*,int,int)> func,const char* file,const char* command_name) : 
					m_func(func),m_file(file),m_command_name(command_name) {}
				void operator() (char_data *ch, char *argument, int cmd, int subcmd){
					if(action_allowed(ch,m_command_name.c_str(),m_file.c_str(),cmd,argument,subcmd)) return m_func(ch,argument,cmd,subcmd);
					else 
					return;
				}
            protected:
				bool action_allowed(struct ::char_data *ch,const char* command_name,const char* file_name,int cmd,const char* arg,int subcmd){
					return mods::globals::acl_allowed(ch,command_name,file_name,cmd,arg,subcmd);
				};
			private:
				std::function<void (char_data*,char*,int,int)> m_func;
				std::string m_file;
				std::string m_command_name;
        };

    };

};


#endif
