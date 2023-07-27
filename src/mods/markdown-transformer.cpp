#include "markdown-transformer.hpp"
#include <map>
#include <string>

namespace mods::markdown_transformer {
	/**
	 * Grammar that we accept:

		- The hash tag:
			- up to 3 hash tag levels. No more
		- The emphasis (bold)
			- we only support 1 wildcard.
		- code tag
			- We accept inline code tags:
			hello, this is an `example` of this
			- We accept multi-line code tags:
			```
			hello,
			this is an example
			of this
			```
	*/
	using ctr_t = std::size_t;
	namespace rdp {
		enum ch_type {
			IDENT = '\t',	// "\t"
			NEWLINE = '\n',	// "\n"
			LINE_FEED = '\r', // "\r"
			HASHTAG = '#',	// "#"
			STAR = '*',		// "*"
			SPACE = ' ', // " "
			NUMBER = '\x01',
			CONTENT = '\x02',
			BACKTICK = '`',
		};
		static constexpr std::string_view content_chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ 1234567890,.:!@$%^&()-_=+[{]}\\|;\"',<.>/?";
		std::string f;
		std::string p;
		ctr_t i;
		bool syntax_okay;
		bool eof;
		std::string consume_until_newline();
		std::string to_string(ch_type t) {
			if(t == ch_type::NUMBER) {
				return "NUMBER";
			}
			if(t == ch_type::CONTENT) {
				return "CONTENT";
			}
			if(t == ch_type::IDENT) {
				return "IDENT";
			}
			if(t == ch_type::NEWLINE) {
				return "NEWLINE";
			}
			if(t == ch_type::LINE_FEED) {
				return "LINE_FEED";
			}
			if(t == ch_type::HASHTAG) {
				return "HASHTAG";
			}
			if(t == ch_type::STAR) {
				return "STAR";
			}
			if(t == ch_type::BACKTICK) {
				return "BACKTICK";
			}
			if(t == ch_type::SPACE) {
				return "SPACE";
			}
			return "?";
		}

		bool is_content() {
			for(const auto& ch : content_chars) {
				if(p[i] == ch) {
					return true;
				}
			}
			return false;
		}
		bool safe(ctr_t offset) {
			return p.length() > offset;
		}
		bool header_1();
		bool header_2();
		bool header_3();
		void nextsym();
		bool white_space();
		bool accept(ch_type type);
		bool expect(ch_type type);
		void header_line();
		bool code();
		bool inline_code();
		void content() {
			white_space();
			while(is_content()) {
				f += p[i];
				nextsym();
			}
		}
		void error_expected(ch_type t) {
			std::cerr << "SYNTAX_ERROR: expected: " << to_string(t) << "\n";
			std::cerr << "near: '";
			int start = 0;
			if(i - 10 > 0) {
				start = i - 10;
			} else {
				start = i;
			}
			for(int x= start; x < i + 10; x++) {
				if(safe(x) == false) {
					break;
				}
				std::cerr << p[x];
			}
			std::cerr << "'\n";
		}
		void nextsym() {
			++i;
			if(p.length() == i) {
				eof = true;
			}
		}
		bool accept_repeated(ch_type sym,ctr_t len) {
			for(ctr_t start = i; start < i + len; start++) {
				if(start >= p.length()) {
					return false;
				}
				if(p[start] != sym) {
					return false;
				}
			}
			return true;
		}
		bool accept(ch_type type) {
			if(type == ch_type::NUMBER) {
				if(isdigit(p[i])) {
					nextsym();
					return true;
				}
			}
			if(type == ch_type::CONTENT) {
				if(is_content()) {
					nextsym();
					return true;
				}
			}
			if(p[i] == type) {
				nextsym();
				return true;
			}
			return false;
		}
		bool expect(ch_type type) {
			if(accept(type)) {
				return true;
			}
			error_expected(type);
			return false;
		}
		std::string consume_until_newline() {
			std::string s;
			while(safe(i) && p[i] != '\n') {
				s += p[i];
				++i;
			}
			return s;
		}
		bool inline_code() {
			if(accept(BACKTICK)) {
				f += "{red}";
				while(p[i] != '`' && !eof) {
					f += p[i];
					nextsym();
				}
				if(p[i] == '`') {
					nextsym();
				}
				f += p[i];
				f += "{/red}";
				return true;
			}
			return false;
		}
		void bold() {
			if(accept(STAR)) {
				f += "{blu}";
				while(!accept(STAR)) {
					f += p[i];
					nextsym();
				}
				f += "{/blu}";
				return;
			}
		}
		bool white_space() {
			if(accept(IDENT)) {
				f += p[i];
				return white_space();
			}
			if(accept(SPACE)) {
				f += p[i];
				return white_space();
			}
			return false;
		}

		void header_line() {
			white_space();
			if(inline_code()) {
				f += "{yel}";
				return header_line();
			}
			if(accept(CONTENT)) {
				content();
				f += "{yel}";
				return header_line();
			}
			if(accept(STAR)) {
				f += p[i];
				bold();
				f += "{yel}";
				return header_line();
			}
		}
		bool header_1() {
			if(accept(HASHTAG)) {
				if(safe(i+1) && p[i+1] != HASHTAG) {
					f += "{yel}#";
					f += p[i];
					header_line();
					f += "{/yel}\r\n";
					return true;
				}
			}
			return false;
		}
		bool header_2() {
			if(accept_repeated(HASHTAG,2)) {
				if(safe(i+1) && p[i+1] != HASHTAG) {
					f += "{blu}#";
					f += p[i];
					header_line();
					f += "{/blu}\r\n";
					return true;
				}
			}
			return false;
		}
		bool header_3() {
			if(accept_repeated(HASHTAG,3)) {
				f += "{blu}#";
				f += p[i];
				header_line();
				f += "{/blu}\r\n";
				return true;
			}
			return false;
		}

		void line() {
			if(header_1()) {
				return line();
			}
			if(header_2()) {
				return line();
			}
			if(header_3()) {
				return line();
			}
			nextsym();
		}
		std::string parse() {
			i = 0;
			syntax_okay = true;
			while(!eof && syntax_okay) {
				line();
			}
			return f;
		}

	};
	std::string transform(std::string_view p) {
		rdp::p = p;
		return rdp::parse();
	}
};
