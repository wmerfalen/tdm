#include "scripted-step.hpp"
#include "scripted-sequences.hpp"
#include "interpreter.hpp"
#include "pfind.hpp"
#include "builder/encode.hpp"
#ifdef  __MENTOC_SHOW_CONTRACT_OUTPUT__
#define dbg_print(a) std::cerr << "[mods::scripted_sequencess][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#else
#define dbg_print(a)
#endif
namespace mods {
	/**
	 *
	 * Data storage
	 *
	 */
	std::deque<std::shared_ptr<scripted_sequences>>& scripted_sequences_master_list() {
		static std::deque<std::shared_ptr<scripted_sequences>> list;
		return list;
	}

	/**
	 *
	 * Data querying
	 *
	 */
	std::optional<std::shared_ptr<scripted_sequences>> find_scripted_sequences(const scripted_sequences_vnum_t& scripted_sequences_vnum) {
		for(const auto& c : scripted_sequences_master_list()) {
			if(c->vnum == scripted_sequences_vnum) {
				return c;
			}
		}
		return std::nullopt;
	}
	/**
	 * You should load all orm objects before calling this function
	 */
	std::tuple<int32_t,std::string> compile_scripted_data() {
		//
		return {0,"stub"};
	}

	namespace scripted_sequences_init {
		void init() {
		}
	};

};
