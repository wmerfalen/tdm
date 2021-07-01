#ifndef __MENTOC_MODS_DB_REPORT_HEADER__
#define __MENTOC_MODS_DB_REPORT_HEADER__

#include <iostream>
#include <string_view>
#include "colors.hpp"
#include <memory>
#include <fstream>
#include "util.hpp"

#define REPORT_DB_ISSUE(MENTOC_MESSAGE,MENTOC_EXCEPTION_MESSAGE) report_db_issue(CAT("file:",__FILE__,",line:",__LINE__,"[message]->'",MENTOC_MESSAGE,"'"),MENTOC_EXCEPTION_MESSAGE)

namespace mods::db_report {
	static std::unique_ptr<std::fstream> log_file;
	static bool initialized = 0;
	static inline void close() {
		if(log_file) {
			log_file->flush();
			log_file->close();
			initialized = 0;
		}
	}
};
static inline void report_db_issue(std::string_view message,std::string_view exception_message) {
	using namespace mods::db_report;
	if(!initialized) {
		log_file = std::make_unique<std::fstream>("database-errors.log",std::ios::out | std::ios::app);
		initialized = 1;
	}
	(*log_file) << "[date:" << mods::util::time_string() << "]:" << message.data() << "|" << exception_message.data() << "\n";
	log_file->flush();
	std::cerr << mods::colors::red_str("[database issue]:") << "(message:'" << message.data() << "')(exception:'" << mods::colors::red_str(exception_message.data()) << "')\n";
}
#endif
