#include "value-scaler.hpp"
#include "../interpreter.hpp"
#include "../super-users.hpp"
#include "requirements.hpp"

#ifdef __MENTOC_SHOW_MODS_FORGE_ENGINE_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::forge_engine::value_scaler]")  << MSG << "\n";
#else
#define m_debug(MSG) ;;
#endif

namespace mods::forge_engine {
	value_scaler::~value_scaler() {
		std::cout << "[value_scaler::~value_scaler]\n";
	}
	value_scaler::value_scaler(player_ptr_t& player) {
		calculate(player);
	}
	void value_scaler::calculate(player_ptr_t& player) {
		/** TODO need to balance this */
		level = (float)player->level();
		low = 1.0;
		high = level * 0.9;
		low_low = level / 3.5;
		stat_low = std::clamp(low_low, low,high);
		stat_high =  std::clamp(level,low+1,high+2);
		low_level = level / 3;
		high_level = level / 1.3;
		low_low_clamp = 1;
		low_high_clamp = 4 + level;
		high_low_clamp = 2;
		high_high_clamp = 5 + level;
		uint_low = std::clamp(low_level, low_low_clamp, low_high_clamp);
		uint_high = std::clamp(high_level, high_low_clamp, high_high_clamp);
		max_stats = level + 10;
	}
	void value_scaler::dump() {
		player->send("stat_low: %f\nstat_high: %f\nuint_low: %d\nuint_high: %d\nmax_stats: %d\n\n",stat_low,stat_high,uint_low,uint_high, max_stats);
	}
	std::string value_scaler::get_dump() {
		std::vector<char> buffer;
		buffer.resize(10240);
		std::fill(buffer.begin(),buffer.end(),0);
		snprintf(&buffer[0],10240,"stat_low: %f\nstat_high: %f\nuint_low: %d\nuint_high: %d\nmax_stats: %d\n\n",stat_low,stat_high,uint_low,uint_high, max_stats);
		return &buffer[0];
	}



	namespace value_scaler_static {
		/** admin utility function */
		ACMD(do_value_scaler) {
			ADMIN_REJECT();
			DO_HELP_WITH_ZERO("value_scaler");
			static constexpr const char* usage = "usage: value_scaler <level>...<level N>\r\n"
			                                     "example: value_scaler 1 10 30\r\n";
			auto vec_args = PARSE_ARGS();
			std::vector<std::string> screen;
			if(vec_args.size() < 1) {
				player->sendln(usage);
				return;
			}
			using namespace mods::forge_engine;
			auto saved_level = player->level();
			requirements_t requirements;
			for(uint8_t i = 0; i < vec_args.size(); ++i) {
				int level = mods::util::stoi(vec_args[i]).value_or(-1);
				if(level <= 0) {
					player->error(CAT("Invalid numeric value encountered at string: '",vec_args[i],"'\r\n"));
					continue;
				}
				player->level() = level;
				value_scaler scale(player);
				player->send("value_scale: %s\r\n",scale.get_dump().c_str());
			}
			player->level() = saved_level;
		}
		/** game init */
		void init() {
			mods::interpreter::add_command("value_scaler", POS_RESTING, do_value_scaler, LVL_BUILDER,0);
		}
	};
};
#undef m_debug
