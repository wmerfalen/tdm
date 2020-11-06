#include "deep-object-parser.hpp"

#define m_crit(a) std::cerr << "[deep_object_parser_t][parser_error][line:" << __LINE__ << "][file:'" << __FILE__ << "'][msg:'" << a << "']\n"
#define m_debug(a) std::cout << "[deep_object_parser_t][debug[line:" << __LINE__ << "][file:'" << __FILE__ << "'][msg:'" << a << "']\n"
#define m_debug_line_info() std::cerr << " substr:'" << line.substr(in_counter) << "'\n";
#define m_success(a) std::cout << "[deep_object_parser_t]" << green_str("[success]") << "[line:" << __LINE__ << "][file:'" << __FILE__ << "'][msg:'" << a << "']\n"
#define m_parse_error(a) std::cerr << "[deep_object_parser_t]" << red_str("[parse_error]") << "[line:" << __LINE__ << "][file:'" << __FILE__ << "'][msg:'" << a << "']\n"
namespace mods {
	const std::string example_grammar = "g36c.yml{sight:acog.yml,muzzle:compensator.yml,under_barrel:gm32grenadelauncher.yml}";
	using counter_t = uint16_t;
	std::tuple<bool,std::string,counter_t> expect_yaml_file(const std::string& line, counter_t in_counter) {
		counter_t saved = in_counter;
		bool file_parsed = 0;
		std::string file_name = "";
		std::string extension = "";
		while(line.length() > in_counter && !file_parsed){
			if(line[in_counter] == '-' || isalpha(line[in_counter]) || isdigit(line[in_counter])) {
				file_name += line[in_counter++];
				continue;
			}
			if(line[in_counter] == '.') {
				if(file_name.length() == 0){
					m_crit("We don't accept a yaml file with just an extension in line:'" << line << "', counter:" << in_counter);
					return {false,"",saved};
				}
				file_name += '.';
				in_counter++;
				if(line.length() <= in_counter + 2){
					m_crit("Expected position to have atleast 2 more characters. Found less than that. Parse error:'" << line << "', counter:" << in_counter);
					return {false,"",saved};
				}
				if(line[(in_counter)] == 'y' &&
					line[(in_counter) + 1] == 'm' &&
					line[(in_counter) + 2] == 'l'){
					in_counter = in_counter + 2;
					file_parsed = true;
					m_debug("Successfully parsed yaml file: '" << file_name << "yml, line:'" << line << "'");
					return {true,file_name + "yml",in_counter};
				}
				m_crit("found dot, but couldn't find proper .yml extension sequence in line:'" << line << "', counter:" << in_counter);
				return {false,"",saved};
			}
			++in_counter;
		}
		m_crit("reached end of line before successfully parsing full yaml file in line:'" << line << "', counter:" << in_counter);
		return {false,"",saved};
	}
	std::tuple<bool,std::string,counter_t> accept_eol(const std::string& line, counter_t in_counter) {
		if(line.length() == in_counter || line[in_counter] == '}'){
			return {true,"",in_counter};
		}
		return {false,"",in_counter};
	}
	std::tuple<bool,std::string,counter_t> accept_open_bracket(const std::string& line, counter_t in_counter) {
		m_debug_line_info();
		if(line.length() == in_counter || in_counter +1 >= line.length()){
			return {false,"",in_counter};
		}
		return {line[in_counter] == '{',"",in_counter};
	}
	/** 
	 *
	 * @param acceptable_slot_names
	 * @param line
	 * @param in_counter
	 *
	 * @return {parsed_okay,slot_name,counter};
	 */
	std::tuple<bool,std::string,counter_t> expect_slot_name(const std::vector<std::string>& acceptable_slot_names,const std::string& line, counter_t in_counter) {
		auto saved = in_counter;
		if(line.length() <= in_counter){
			m_crit("line ran out of space line:'" << line << "', counter:" << in_counter);
			return {false,"",in_counter};
		}
		auto s = line.substr(in_counter);
		std::cout << "line substr: '" << s << "'\n\n\n\n";
		for(auto & slot_option : acceptable_slot_names){
			std::cout << "line substr: '" << line.substr(in_counter) << "'\n";
			if(line.length() <= (in_counter + slot_option.length())){
				m_debug("out of bounds. not going to compare...");
				continue;
			}
			if(line.substr(in_counter,slot_option.length()).compare(slot_option) == 0){
				m_debug("Successfully parsed slot name: '" << slot_option );
				m_debug_line_info();
				return {true,slot_option,in_counter};
			}
		}
		m_crit("reached end of line before successfully parsing slot option in line:'" << line << "', counter:" << in_counter);
		return {false,"",saved};
	}
	std::tuple<bool,std::string,counter_t> accept_comma(const std::string& line, counter_t in_counter) {
		if(line.length() <= in_counter){
			m_crit("line ran out of space line:'" << line << "', counter:" << in_counter);
			return {false,"",in_counter};
		}
		if(line[in_counter] == ','){
			m_debug("Successfully parsed comma.");
			in_counter += 1;
			return {true,"",in_counter};
		}
		return {false,"",in_counter};
	}
	std::tuple<bool,std::string,counter_t> accept_closing_bracket(const std::string& line, counter_t in_counter) {
		counter_t saved = in_counter;
		bool file_parsed = 0;
		std::string file_name = "";
		std::string extension = "";
		while(line.length() > in_counter && !file_parsed){
			if(isalpha(line[in_counter]) || isdigit(line[in_counter])) {
				file_name += line[in_counter++];
				continue;
			}
			if(line[in_counter] == '.') {
				if(file_name.length() == 0){
					m_crit("We don't accept a yaml file with just an extension in line:'" << line << "', counter:" << in_counter);
					return {false,"",saved};
				}
				file_name += '.';
				in_counter++;
				if(line.length() <= in_counter + 2){
					m_crit("Expected position to have atleast 2 more characters. Found less than that. Parse error:'" << line << "', counter:" << in_counter);
					return {false,"",saved};
				}
				if(line[(in_counter)] == 'y' &&
					line[(in_counter) + 1] == 'm' &&
					line[(in_counter) + 2] == 'l'){
					in_counter = in_counter + 2;
					file_parsed = true;
					m_debug("Successfully parsed yaml file: '" << file_name << "yml, line:'" << line << "'");
					return {true,file_name + "yml",in_counter};
				}
				m_crit("found dot, but couldn't find proper .yml extension sequence in line:'" << line << "', counter:" << in_counter);
				return {false,"",saved};
			}
			++in_counter;
		}
		m_crit("reached end of line before successfully parsing full yaml file in line:'" << line << "', counter:" << in_counter);
		return {false,"",saved};
	}
	str_map_t deep_object_parser_t::extract_line_items(const std::string& line, const std::vector<std::string> & slot_names){
		str_map_t map;
		uint16_t counter = 0;
		std::string yaml_file;
		m_debug("parsing yaml file");
		auto t = expect_yaml_file(line,counter);
		if(!std::get<0>(t)){
			return map;
		}
		m_success("yaml file: " << std::get<1>(t));
		map["base"] = std::get<1>(t);
		counter += std::get<1>(t).length();
		auto tbracket = accept_open_bracket(line,counter);
		if(!std::get<0>(tbracket)){
			m_parse_error("didn't find bracket...");
			return map;
		}
		m_debug("found bracket...");
		++counter;

		bool eol = false;
		bool good = true;
		while(!eol && good){
			if(line.length() <= counter){
				eol = true;
				continue;
			}
			auto tslot = expect_slot_name(slot_names,line,counter);
			if(!std::get<0>(tslot)){
				m_parse_error("expected slot name, not parsed");
				good = false;
				continue;
			}
			std::string slot = std::get<1>(tslot);
			std::cout << "found slot name: '" << slot << "'\n";
			counter += slot.length();
			auto in_counter = counter;
			in_counter = counter;
			m_debug_line_info();

			if(line.length() <= counter || line.at(counter) != ':'){
				m_parse_error("expected colon");
				good = false;
				continue;
			}
			++counter;
			auto tyaml = expect_yaml_file(line,counter);
			if(!std::get<0>(tyaml)){
				good = false;
				continue;
			}
			m_success(green_str(std::get<1>(tyaml)) << " of type: '" << green_str(slot) << "'");
			m_success("yaml file: " << std::get<1>(tyaml));
			map[slot] = std::get<1>(tyaml);
			counter += std::get<1>(tyaml).length();
			in_counter = counter;
			m_debug_line_info();
			auto tcomma = accept_comma(line,counter);
			if(!std::get<0>(tcomma)){
				m_debug("no comma found");
				if(line.length() <= counter){
					m_debug("eol found. done parsing.");
					eol = true;
					continue;
				}
				if(line[counter] == '}'){
					m_debug("found closing bracket. done parsing.");
					eol = true;
					continue;
				}
			}
			++counter;
		}
		return map;
	}
};
