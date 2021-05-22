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
	void load_all_scripted_sequencess() {
#if __MENTOC_USING_ORM_LAYER_FOR_SCRIPTED_SEQUENCES__
		auto& master_list = scripted_sequences_master_list();
		auto status = mods::orm::load_all_non_orm_scripted_sequencess(&master_list);
		if(std::get<0>(status)) {
			dbg_print(green_str("load_all_scripted_sequencess:") << "'" << std::get<1>(status) << "'");
		} else {
			dbg_print(red_str("load_all_scripted_sequencess:") << "'" << std::get<1>(status) << "'");
		}
#endif
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

	ACMD(do_scripted_sequences) {
		DO_HELP_WITH_ZERO("scripted_sequences");

		if(args()->first_is("list")) {
			player->pager_start();
			player->sendln("Listing...");
			for(const auto& con : scripted_sequences_master_list()) {
				player->sendln(CAT("{yel}[{/yel}",con->vnum,"{yel}]\r\n"));
			}
			player->sendln("Done listing.");
			player->pager_end();
			player->page(0);
			return;
		}
		int scripted_sequences_num = 0;
		if(args()->save_integer(1)->if_nth_has_either(0, {"join","leave"})) {
			scripted_sequences_num = args()->fetch_parsed_integer(1);

			if(scripted_sequences_num == -1) {
				*player << "{red}Invalid scripted_sequences number.{/red}\r\n";
				return;
			}
			/** find the scripted_sequences by vnum */
			auto opt_scripted_sequences = find_scripted_sequences(scripted_sequences_num);
			if(opt_scripted_sequences.has_value() == false) {
				player->send("{red}We could not find a scripted_sequences with that vnum{/red}\r\n");
				return;
			}
		}

	}
	namespace scripted_sequences_init {
		void init() {
			mods::interpreter::add_command("scripted_sequences", POS_RESTING, do_scripted_sequences, 0,0);
			load_all_scripted_sequencess();
		}
	};

};
