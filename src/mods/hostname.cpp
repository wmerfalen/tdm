#include "hostname.hpp"
#include "filesystem.hpp"


namespace mods::host_name {
	static std::string host;
	void parse_hostname() {
		std::string guts,error;
		int ret = mods::filesystem::file_get_contents("/etc/hostname",guts,error);
		if(ret < 0) {
			host = "<?>";
		} else {
			host = guts;
		}
		std::string tmp;
		for(const auto& ch: host) {
			if(isalpha(ch) || isdigit(ch) || ch == '.' || ch == '-' || ch == '@') {
				tmp += ch;
			}
		}
		host = tmp;
	}
	const char* get() {
		if(host.length() == 0) {
			parse_hostname();
		}
		return host.c_str();
	}
};
