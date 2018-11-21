#include "js-path.hpp"
#include "js-player-scaffolding.hpp"
//#include "quests.hpp"
namespace mods::js::player {
		static duk_ret_t require_js(duk_context *ctx) {
			/* First parameter is character name */
			auto fname = duk_to_string(ctx,0);
			std::string path = mods::js::JS_PATH;
			path += fname;
			duk_push_number(ctx,mods::js::load_library(ctx,path));
			return 1;	/* number of return values */
		}
};
