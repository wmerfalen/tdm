#include "friendly-reminders.hpp"
#include "../replenish.hpp"
#include "../movement.hpp"
#include "../interpreter.hpp"
#include "../loops.hpp"
#include "../prefs.hpp"

#define __MENTOC_SHOW_MODS_PLAYERS_friendly_reminders_DEBUG_OUTPUT__
#ifdef __MENTOC_SHOW_MODS_PLAYERS_friendly_reminders_DEBUG_OUTPUT__
#define m_debug(a) std::cerr << "[mods::players::friendly_reminders][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#define m_crit(a) std::cerr << red_str("[CRITICAL]") << "mods::players::friendly_reminders][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#else
#define m_debug(a)
#define m_crit(a)
#endif


namespace mods::players::friendly_reminders {
	static std::vector<std::string> reminders;
	static std::map<std::string,std::size_t> player_reminder_index;
	void send() {
		m_debug("[stub]");
		mods::loops::foreach_player([&](auto player) -> bool {
			m_debug("noreminder value:" << PLAYER_GET("noreminder"));
			bool no_reminders = mods::prefs::get_player_pref(player,"noreminder");
			if(no_reminders || player->position() <= POS_STUNNED || player->fighting()) {
				return true; /** skip user as they requested reminders off */
			}

			auto& ref = player_reminder_index[player->name().c_str()];
			if(ref >= reminders.size()) {
				ref = 0;
			}
			player->sendln(CAT("{grn}Reminder:{/grn} {yel}",reminders[ref],"{/yel}"));
			ref++;
			return true;
		});
	}
	void load_from_db() {
		for(const auto& str : {
		            "You might have loot! Simply type: {blu}list_loot{/blu}",
		            "You can turn on or off the overhead map. To turn it on, type: {blu}automap{/blu}",
		            "Did you know you can turn these reminders off using: {blu}pref noreminder 1{/blu}?",
		        }) {
			reminders.emplace_back(str);
		}
	}
	void add_reminder(std::string_view msg) {

	}
	SUPERCMD(do_delete_reminder) {

	}
	SUPERCMD(do_add_reminder) {

	}
	SUPERCMD(do_list_reminder) {

	}
	void init() {
		ADD_BUILDER_COMMAND("list_reminder",do_list_reminder);
		ADD_BUILDER_COMMAND("add_reminder",do_add_reminder);
		ADD_BUILDER_COMMAND("delete_reminder",do_delete_reminder);
	}
};
