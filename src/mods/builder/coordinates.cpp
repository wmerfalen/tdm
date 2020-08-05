#include "rectangle-layouts.hpp"

extern void r_error(const player_ptr_t & player,std::string_view msg);
extern void r_success(const player_ptr_t & player,std::string_view msg);
extern void r_status(const player_ptr_t & player,std::string_view msg);

namespace mods::builder::rectangle_layouts {
	std::string_view explain_make(){
		constexpr static const char* explanation = "When you make a rectangle, there are 4 things that must be provided."
			"You must provide a width and length, which is obvious, but you must also provide us an X and Y offset."
			"The offset represents your position within the rectangle. Imagine we have a 3x3 rectangle (or square)."
			"If you specify 0,0 that would mean that you want the system to create a 3x3 rectangle where your current position "
			"is the top left corner of the square. The offset is counted from the top left for all rectangles. If you were "
			"to specify an X,Y offset of 1,1 then that would mean that your current position would be the exact middle of the "
			"square."
			"\r\n"
			"\r\n"
			" =========================================================================\r\n"
			" [Example]                                                                \r\n"
			" =========================================================================\r\n"
			" This is your position (p)                                                \r\n"
			"                                                                          \r\n"
			" [ ][ ][p]                                                                \r\n"
			"                                                                          \r\n"
			" rbuild make 3,3,0,0                                                      \r\n"
			"                                                                          \r\n"
			" Output:                                                                  \r\n"
			"                                                                          \r\n"
			" [ ][ ][p][ ][ ]                                                          \r\n"
			"       [ ][ ][ ]                                                          \r\n"
			"       [ ][ ][ ]                                                          \r\n"
			"                                                                          \r\n"
			" =========================================================================\r\n"
			" [Example 2: middle position]                                             \r\n"
			" =========================================================================\r\n"
			" This is your position (p)                                                \r\n"
			"                                                                          \r\n"
			" [ ][ ][p]                                                                \r\n"
			"                                                                          \r\n"
			" rbuild make 3,3,1,1                                                      \r\n"
			"                                                                          \r\n"
			" Output:                                                                  \r\n"
			"                                                                          \r\n"
			"    [ ][ ][ ]                                                             \r\n"
			" [ ][ ][p][ ]                                                             \r\n"
			"    [ ][ ][ ]                                                             \r\n"
			"                                                                          \r\n"
			" =========================================================================\r\n"
			" [Example 3: bottom right position]                                       \r\n"
			" =========================================================================\r\n"
			" This is your position (p)                                                \r\n"
			"                                                                          \r\n"
			" [ ][ ][p]                                                                \r\n"
			"                                                                          \r\n"
			" rbuild make 3,3,2,2                                                      \r\n"
			"                                                                          \r\n"
			" Output:                                                                  \r\n"
			"                                                                          \r\n"
			" [ ][ ][ ]                                                                \r\n"
			" [ ][ ][ ]                                                                \r\n"
			" [ ][ ][p]                                                                \r\n"
			"                                                                          \r\n"
			" =========================================================================\r\n"
			" [Example 4: rectangles]                                                  \r\n"
			" =========================================================================\r\n"
			" This is your position (p)                                                \r\n"
			"                                                                          \r\n"
			" [ ][ ][p]                                                                \r\n"
			"                                                                          \r\n"
			" rbuild make 2,6,0,1                                                      \r\n"
			"                                                                          \r\n"
			" Output:                                                                  \r\n"
			"       [ ][ ]                                                             \r\n"
			" [ ][ ][p][ ]                                                             \r\n"
			"       [ ][ ]                                                             \r\n"
			"       [ ][ ]                                                             \r\n"
			"       [ ][ ]                                                             \r\n"
			"       [ ][ ]                                                             \r\n"
			"                                                                          \r\n"
			;
		return explanation;
	}
	void send_rbuild_help(player_ptr_t& player){
		constexpr static const char* rbuild_menu = ""
			""
			""
			;
		player->send(rbuild_menu);
	}
	std::tuple<bool,str_vec_t> parse_dimensions(str_t input,dimensions_t* output){
		output->width = 0;
		output->height = 0;
		output->offset_x = 0;
		output->offset_y = 0;
		str_vec_t errors;
		auto args = EXPLODE(input,",");
		bool has_error = 0;
		if(args.size() != 4){
			errors.emplace_back(CAT({"We expected 4 comma separated values. Instead we got ",tostr(args.size()),", please correct this and try again."}));
			return {false, errors};
		}
		auto opt_width = mods::util::stoi<uint8_t>(args[0]);
		if(!opt_width.has_value()){
			errors.emplace_back("Invalid width.");
			has_error = 1;
		}else{
			output->width = opt_width.value();
		}
		auto opt_height = mods::util::stoi<uint8_t>(args[1]);
		if(!opt_height.has_value()){
			errors.emplace_back("Invalid height.");
			has_error = 1;
		}else{
			output->height = opt_height.value();
		}
		auto opt_offset_x = mods::util::stoi<uint8_t>(args[2]);
		if(!opt_offset_x.has_value()){
			errors.emplace_back("Invalid X offset.");
			has_error = 1;
		}else{
			output->offset_x = opt_offset_x.value();
		}
		auto opt_offset_y = mods::util::stoi<uint8_t>(args[3]);
		if(!opt_offset_y.has_value()){
			errors.emplace_back("Invalid Y offset.");
			has_error = 1;
		}else{
			output->offset_y = opt_offset_y.value();
		}
		return {has_error,errors};
	}
	bool handled_rbuild_command(player_ptr_t& player,str_t argument){
		auto vec_args = PARSE_ARGS();
		if(vec_args.size() == 0){
			return false;
		}
		bool is_make_cmd = vec_args[0].compare("make") == 0;
		if(is_make_cmd && vec_args.size() < 2){
			r_error(player,"Please specify the dimensions. For example: rbuild make 3,3,0,1\r\n");
			return true;
		}
		dimensions_t dim;
		auto status = parse_dimensions(vec_args[1],&dim);
		if(!std::get<0>(status)){
			for(auto & error : std::get<1>(status)){
				r_error(player,CAT({"Error parsing dimensions: '",error,"'"}));
				return true;
			}
		}
		return false;
	}
};

