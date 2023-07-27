#include "markdown-transformer.hpp"
#include <map>
#include <string>

namespace mods::markdown_transformer {
	using ctr_t = std::size_t;
	std::string code_color = "{red}";
	std::string code_end = "{/red}";
	bool safe(std::string_view page,const ctr_t& offset) {
		return offset < page.length();
	}
	char at(std::string_view page,const ctr_t& offset) {
		return page[offset];
	}
	std::string till_tilde(std::string_view p, ctr_t* offset);
	std::string till_newline(std::string_view p, ctr_t* offset) {
		std::string captured;
		for(; p.length() > *offset && p[*offset] != '\n'; ++(*offset)) {
			if(p[*offset] == '`') {
				++(*offset);
				captured += code_color;
				captured += till_tilde(p,offset);
				captured += code_end;
				++(*offset);
				continue;
			}
			captured += p[*offset];
		}
		return captured;
	}
	std::string till_tilde(std::string_view p, ctr_t* offset) {
		std::string captured;
		for(; p.length() > *offset && p[*offset] != '`'; ++(*offset)) {
			captured += p[*offset];
		}
		return captured;
	}

	std::string transform(std::string_view p) {
		std::string f;
		bool new_line = true;
		for(ctr_t i=0; i < p.length(); i++) {
			if(new_line) {
				if(p[i] == '#' && safe(p,i+1) && at(p,i+1) == ' ') {
					/**
					 * Single '#' found.
					 */
					f += "{yel}";
					f += till_newline(p,&i);
					f += "{/yel}\r\n";
					new_line = true;
				}
				if(p[i] == '#' && safe(p,i+1) && at(p,i+1) == '#') {
					/**
					 * Single '#' found.
					 */
					f += "{blu}";
					f += till_newline(p,&i);
					f += "{/blu}\r\n";
					new_line = true;
				} else if(p[i] == '`' && safe(p,i+1) && at(p,i+1) != '`') {
					/**
					 * User wants single line code embed
					 */
					++i;
					f += "{grn}";
					f += till_tilde(p,&i);
					++i;
					f += "{/grn}";
					f += till_newline(p,&i);
					f += "\r\n";
					new_line = true;
				} else if(p[i] == '`' && safe(p,i+2) && at(p,i+1) == '`' && at(p,i+2) == '`') {
					/**
					 * User wants multi line code embed
					 */
					till_newline(p,&i);
					do {
						f += "{red}";
						f += till_newline(p,&i);
						f += "{/red}\r\n";
						++i;
					} while(safe(p,i) && p[i] != '`' && safe(p,i+2) && at(p,i+1) != '`' && at(p,i+2) != '`');
					till_newline(p,&i);
					new_line = true;
				} else {
					f += till_newline(p,&i);
					f += "\r\n";
					new_line = true;
				}
			} else {
				f += p[i];
				new_line = p[i] == '\n';
			}
		}
		return f;
	}
};
