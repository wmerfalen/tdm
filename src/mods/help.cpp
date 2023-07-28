#include "help.hpp"
#include <map>
#include <string>
#include "player.hpp"
#include "interpreter-include.hpp"
#include "filesystem.hpp"
#define __MENTOC_SHOW_UTIL_MAP_DUMP_OUTPUT__
#include "util-map.hpp"
#include "boot-flags.hpp"
#include "pc-ability.hpp"
#include <sys/types.h>
#include <dirent.h>
#include "markdown-transformer.hpp"

#ifdef m_debug
	#undef m_debug
#endif

#define m_debug(A) std::cerr << "[DEBUG][mods::help]: " << A << "\n"

namespace mods::help {
	void send_help(const std::vector<std::string>& vec_args,player_ptr_t& player);
	bool is_exact_handled_help_match(std::string_view in_topic,player_ptr_t& player);
	bool is_class_help_match(const std::string& in_topic,player_ptr_t& player);
	void send_class_header(player_ptr_t& player,std::string_view class_name);
	void send_class_footer(player_ptr_t& player,std::string_view class_name);
	std::string send_topic(std::string_view topic);
	void builder_page(std::vector<std::string>& screen);
	void fetch_builder_help(std::vector<std::string>& screen);
	void send_player_class_help(player_ptr_t& player);
	void send_help_topics(player_ptr_t& player);
	void send_marine_help_menu(player_ptr_t& player);
	void send_breacher_help_menu(player_ptr_t& player) {
		static std::vector<std::string> topics = {
			"Special Ability - To use your explosive shot, simply move toward a closed door or surface twice. The first time you "
			" move toward the door/surface, it will tell you to move that direction once more to explode it.\r\n"
		};
		send_class_header(player,"BREACHER");
		for(auto topic : topics) {
			player->sendln(send_topic(topic));
		}
		send_class_footer(player,"BREACHER");
	}
	std::vector<std::string> dirs = {
		"breacher",
		"contagion",
		"ghost",
		"marine",
	};
	static std::vector<std::string> generic_pages;
	static std::vector<std::string> marine_pages;
	static std::vector<std::string> contagion_pages;
	static std::vector<std::string> breacher_pages;
	static std::vector<std::string> ghost_pages;
	void send_player_class_help(player_ptr_t& player) {
		player->pager_start();
		switch(player->get_class()) {
			case player_class_t::CLASS_MARINE:
				send_marine_help_menu(player);
				break;
			case player_class_t::CLASS_GHOST:
				send_ghost_help_menu(player);
				break;
			case player_class_t::CLASS_CONTAGION:
				send_contagion_help_menu(player);
				break;
			case player_class_t::CLASS_BREACHER:
				send_breacher_help_menu(player);
				break;
			default:
				break;
		}
		send_help_topics(player);
		player->pager_end();
		player->page(0);
	}
	void send_help_topics(player_ptr_t& player) {

	}
	void send_builder_help(player_ptr_t& player) {
		std::vector<std::string> builder_help;
		builder_page(builder_help);
		player->pager_start();
		for(const auto& line : builder_help) {
			player->sendln(line);
		}
		player->pager_end();
		player->page(0);
	}

	std::string send_topic(std::string_view topic) {
		std::string line = topic.data();
		line += " - help ";
		for(auto ch : topic) {
			if(isspace(ch)) {
				line += "_";
				continue;
			}
			line += tolower(ch);
		}
		return line;
	}
	void fetch_mortal_help(std::vector<std::string>& screen) {
		std::string guts,error;
		mods::filesystem::file_get_contents("../lib/ACMD.list", guts,error);
		std::string current = "";
		for(auto ch : guts) {
			if(ch == '\n') {
				screen.emplace_back(current);
				current.clear();
				continue;
			}
			current += ch;
		}
	}
	void builder_page(std::vector<std::string>& screen) {
		static const std::vector<std::string> builder_screen = {
			"{yel} ----------------------------------------------------------------------{/yel}",
			"{yel} --                      -:[ Cameras ]:-                               {/yel}",
			"{yel} ----------------------------------------------------------------------{/yel}",
			"{gld}giveme_camera{/gld} -- {grn}give me deployable camera [feature-debug]{/grn}",
			"{gld}giveme_night_vision_camera{/gld} -- {grn}give me a night vision camera{/grn}",
			"{gld}giveme_thermal_camera{/gld} -- {grn}give me a thermal camera {/grn}",
			"",
			"{yel} ----------------------------------------------------------------------{/yel}",
			"{yel} --                      -:[ Grenades ]:-                              {/yel}",
			"{yel} ----------------------------------------------------------------------{/yel}",
			"{gld}giveme_flash{/gld} -- {grn}give me frag grenades [feature-debug]{/grn}",
			"{gld}giveme_frag{/gld} -- {grn}give me frag grenades [feature-debug]{/grn}",
			"{gld}giveme_inc{/gld} -- {grn}give me frag grenades [feature-debug]{/grn}",
			"{gld}giveme_sensor{/gld} -- {grn}give me sensor grenades [feature-debug]{/grn}",
			"{gld}giveme_smoke{/gld} -- {grn}give me frag grenades [feature-debug]{/grn}",
			"{gld}givemegold{/gld} -- {grn}give me gold [feature-debug]{/grn}",
			"",
			"{yel} ----------------------------------------------------------------------{/yel}",
			"{yel} --                      -:[ Building ]:-                              {/yel}",
			"{yel} ----------------------------------------------------------------------{/yel}",
			//"{gld}cambuild{/gld} -- {grn}create surveilance cameras that change their feed [builder-utils]{/grn}",
			"{gld}list_minigame{/gld} -- {grn}lists minigames currently installed in the room [builder-utils]{/grn}",
			"{gld}install_minigame{/gld} -- {grn}installs a minigame in the room [builder-utils]{/grn}",
			"{gld}uninstall_minigame{/gld} -- {grn}uninstalls a minigame in the room [builder-utils]{/grn}",
			"{gld}mbuild{/gld} -- {grn}mob builder [builder-utils]{/grn}",
			"{gld}obuild{/gld} -- {grn}object builder [builder-utils]{/grn}",
			"{gld}sbuild{/gld} -- {grn}shop builder [builder-utils]{/grn}",
			"{gld}rbuild{/gld} -- {grn}room builder [builder-utils]{/grn}",
			"{gld}rbuild_sandbox{/gld} -- {grn}room builder sandbox utility [builder-utils]{/grn}",
			"{gld}rnumlist{/gld} -- {grn}list rooms [feature-debug]{/grn}",
			"{gld}rnumtele{/gld} -- {grn}teleport to a room [feature-debug]{/grn}",
			"{gld}room_dark{/gld} -- {grn}usage: room_dark on, room_dark off [builder-utils][admin-utils]{/grn}",
			"{gld}room_fire{/gld} -- {grn}usage: room_fire <on|off> [level] [builder-utils][admin-utils]{/grn}",
			"{gld}room_list{/gld} -- {grn}lists rooms [builder-utils][admin-utils]{/grn}",
			"{gld}yaml_example{/gld} -- {grn}type 'yaml_example list' for a list of yaml files{/grn}",
			"{gld}yaml_import{/gld} -- {grn}type 'yaml_import <filename>' to import a yaml file{/grn}",
			"{gld}yaml_log{/gld} -- {grn}get the yaml log to find out which files failed to import{/grn}",
			"{gld}yaml_log_save{/gld} -- {grn}write the yaml log to disk{/grn}",
			"{gld}yaml_log_clear{/gld} -- {grn}clear the yaml log without flushing to disk{/grn}",
			"{gld}zbuild{/gld} -- {grn}zone builder [builder-utils]{/grn}",
			"",
			"{yel} ----------------------------------------------------------------------{/yel}",
			"{yel} --                    -:[ Ammunition ]:-                              {/yel}",
			"{yel} ----------------------------------------------------------------------{/yel}",
			"{gld}ammo{/gld} -- {grn}give yourself ammo [feature-debug]{/grn}",
			"{gld}empty_clip{/gld} -- {grn}empty the clip in your primary weapon [feature-debug]{/grn}",
			"{gld}set_ammo{/gld} -- {grn}set ammo of currently wielded weapon{/grn}",
			"",
			"{yel} ----------------------------------------------------------------------{/yel}",
			"{yel} --                    -:[ Damage debugging ]:-                        {/yel}",
			"{yel} ----------------------------------------------------------------------{/yel}",
			"{gld}affect_me{/gld} -- {grn}give yourself an affect <BLIND|DISORIENT|POISON> [feature-debug]{/grn}",
			"{gld}heal{/gld} -- {grn}heal yourself [feature-debug][staging-feature][class-medic]{/grn}",
			"{gld}one_punch{/gld} -- {grn}immediately kill your currently fighting char with one punch{/grn}",
			"{gld}point_update{/gld} -- {grn}manually call point update [feature-debug]{/grn}",
			"{gld}set_position{/gld} -- {grn}set position to one of the POS_* constants [feature-debug]{/grn}",
			"",
			"{yel} ----------------------------------------------------------------------{/yel}",
			"{yel} --                    -:[ Debugging ]:-                               {/yel}",
			"{yel} ----------------------------------------------------------------------{/yel}",
			"{gld}flush_player{/gld} -- {grn}calls the default flush_player method [feature-debug][staging-feature][builder-utils]{/grn}",
			"{gld}feed_player{/gld} -- {grn}calls the default feed_player method [feature-debug][staging-feature][builder-utils]{/grn}",
			"{gld}flush_holding{/gld} -- {grn}flush the item you are holding to the db [feature-debug][staging-feature][builder-utils]{/grn}",
			"{gld}get_ticks_per_minute{/gld} -- {grn}get ticks per minute [feature-debug]{/grn}",
			"{gld}histfile{/gld} -- {grn}start recording all commands. stop with 'histfile stop' [builder-utils][feature-debug]{/grn}",
			"{gld}idle{/gld} -- {grn}force your character into idle state [feature-debug]{/grn}",
			"{gld}js{/gld} -- {grn}Run javascript [feature-debug][admin-utils]{/grn}",
			"{gld}js_help{/gld} -- {grn}Show useful js commands [builder-utils][admin-utils]{/grn}",
			"{gld}jstest{/gld} -- {grn}run a javascript test [builder-utils][admin-tools]{/grn}",
			"{gld}newjs{/gld} -- {grn}create a new javascript context [admin-tools][feature-debug]{/grn}",
			"{gld}pref{/gld} -- {grn}preferences utility [staging-feature]{/grn}",
			"{gld}room_list_uuid{/gld} -- {grn}list uuids and names of everyone in the same room [builder-utils]{/grn}",
			"{gld}show_tics{/gld} -- {grn}toggle tics[builder-utils]{/grn}",
			"{yel} ----------------------------------------------------------------------{/yel}",
			"{yel} --                    -:[ Character Generation ]:-                    {/yel}",
			"{yel} ----------------------------------------------------------------------{/yel}",
			"{gld}disable_registration{/gld} -- {grn}yup[builder-utils]{/grn}",
			"{gld}enable_registration{/gld} -- {grn}yup[builder-utils]{/grn}",
			"{gld}registration_status{/gld} -- {grn}prints allowed or not allowed.[builder-utils]{/grn}",
			"{yel} ----------------------------------------------------------------------{/yel}",
			"{yel} --                    -:[ Values System ]:-                           {/yel}",
			"{yel} ----------------------------------------------------------------------{/yel}",
			"{gld}revert_value_to_default{/gld} -- {grn}revert a value back to factory default{/grn}",
			"{gld}list_values{/gld} -- {grn}list all available values{/grn}",
			"{gld}list_value{/gld} -- {grn}alias of list_values{/grn}",
			"{gld}set_value{/gld} -- {grn}set value to specific value{/grn}",
			"{gld}get_value{/gld} -- {grn}display a specific value{/grn}",
			"{yel} ----------------------------------------------------------------------{/yel}",
			"{yel} --                    -:[ Admin Commands ]:-                          {/yel}",
			"{yel} ----------------------------------------------------------------------{/yel}",
			"{gld}admin:disable_registration{/gld} -- {grn}disable new player registration[builder-utils]{/grn}",
			"{gld}admin:banish{/gld} -- {grn}force a player out of your room(not permanent)[debugging-players][bad-players]{/grn}",
			"{gld}admin:pull{/gld} -- {grn}transfer a player to your room (not permanent)[debugging-players]{/grn}",
			"{gld}admin:radio{/gld} -- {grn}manages mud radio [admin-utils] [builder-utils]{/grn}",
			"{gld}admin:weapon-locker:help{/gld} -- {grn}{/grn}",
			"{gld}admin:armor-locker:help{/gld} -- {grn}{/grn}",
			"{gld}admin:ammo-locker:help{/gld} -- {grn}{/grn}",

			"{yel} ----------------------------------------------------------------------{/yel}",
			"{yel} --                    -:[ Punishing Players ]:-                       {/yel}",
			"{yel} ----------------------------------------------------------------------{/yel}",
			"{gld}admin:ban{/gld} -- {grn}ban a player[bad-players]{/grn}",
			"{gld}admin:mute{/gld} -- {grn}mute a player[bad-players]{/grn}",
			"{gld}admin:unban{/gld} -- {grn}unban a player[bad-players]{/grn}",
			"{gld}admin:unmute{/gld} -- {grn}un-mute a player[bad-players]{/grn}",
		};
		for(const auto& line : builder_screen) {
			screen.emplace_back(line);
		}
	}
	void fetch_builder_help(std::vector<std::string>& screen) {
		builder_page(screen);
		std::string guts,error;
		for(const auto& file : {
		        "../lib/SUPERCMD.list","../lib/ADMINCMD.list"
		    }) {
			guts.clear();
			error.clear();
			mods::filesystem::file_get_contents(file, guts,error);
			std::string current = "";
			for(auto ch : guts) {
				if(ch == '\n') {
					screen.emplace_back(current);
					current.clear();
					continue;
				}
				current += ch;
			}
		}
	}

	bool matches_many(const std::string& items,std::string_view from) {
		std::vector<std::string> tokens;
		std::string token;
		std::istringstream tokenStream(items);
		while(std::getline(tokenStream, token, ',')) {
			if(token.compare(from.data()) == 0) {
				return true;
			}
		}
		return false;
	}
	auto find_page_for(auto topic) {
		const auto table = "help_pages";
		try {
			auto up_txn = txn();
			sql_compositor comp(table,&up_txn);
			auto room_sql = comp.select("hp_content")
			    .from(table)
			    .where("hp_section","LIKE",CAT("%",topic.data(),"%"))
			    .sql();
#ifdef __DEBUG_HELP_PAGES_SQL__
			m_debug(room_sql.data() << "\n";
#endif
			    return mods::pq::exec(up_txn,room_sql.data());
		} catch(std::exception& e) {
			return mods::pq::result();
		}
	}
	std::string sanitize_path(std::string_view p) {
		std::string f;
		for(const auto& ch : p) {
			if(isalpha(ch) || isdigit(ch) || ch == '/' || ch == '_') {
				f += ch;
			}
			if(f.length() > 24) {
				return f;
			}
		}
		return f;
	}
	std::string sanitize_topic(std::string_view p) {
		std::string f;
		for(const auto& ch : p) {
			if(ch == ':') {
				f.clear();
				continue;
			}
			if(isalpha(ch) || isdigit(ch) || ch == '_') {
				f += ch;
			}
			if(f.length() > 24) {
				return f;
			}
		}
		return f;
	}
	std::string get_help_file(std::string_view category,std::string_view topic) {
		std::string path = std::string(MENTOC_CURRENT_WORKING_DIR);
		path += "help/";
		if(category.compare("generic") == 0) {
			path += sanitize_topic(topic.data());
			path += ".md";
		} else {
			path += sanitize_path(category.data());
			path += "/";
			path += sanitize_topic(topic.data());
			path += ".md";
		}
		m_debug("[debug] get_help_file() file: '" << path << "'");
		std::string guts,error;
		int ret = mods::filesystem::file_get_contents(path,guts,error);
		if(ret <= 0) {
			if(ret < 0) {
				m_debug("[ERROR] get_help_file(): '" << error << "'");
			}
			return "";
		}
		return mods::markdown_transformer::transform(guts);
	}

	bool handled_help(std::string_view in_topic,const std::vector<std::string>& pages,player_ptr_t& player,std::string_view category) {
		std::string topic = in_topic.data();
		for(const auto& page_name : pages) {
			m_debug("debug: page_name: '" << page_name << "'");
			if(topic.compare(page_name.c_str()) == 0) {
				player->sendln(get_help_file(category,topic));
				return true;
			}
		}
		return false;
	}
	void send_marine_help_menu(player_ptr_t& player) {
		player->pager_start();
		send_class_header(player,"MARINE");
		for(const auto& topic : marine_pages) {
			player->sendln(send_topic(topic));
		}
		send_class_footer(player,"MARINE");
		player->pager_end();
		player->page(0);
	}
	bool is_class_help_match(const std::string& in_topic,player_ptr_t& player) {
		if(mods::util::is_lower_match(in_topic,"marine")) {
			send_marine_help_menu(player);
			return true;
		}
		if(mods::util::is_lower_match(in_topic,"contagion")) {
			send_contagion_help_menu(player);
			return true;
		}
		if(mods::util::is_lower_match(in_topic,"ghost")) {
			send_ghost_help_menu(player);
			return true;
		}
		if(mods::util::is_lower_match(in_topic,"breacher")) {
			send_breacher_help_menu(player);
			return true;
		}
		return false;
	}
	bool is_exact_handled_help_match(std::string_view topic,player_ptr_t& player) {
		if(handled_help(topic,generic_pages,player,"generic")) {
			return true;
		}
		if(handled_help(topic,marine_pages,player,"marine")) {
			return true;
		}
		if(handled_help(topic,breacher_pages,player,"breacher")) {
			return true;
		}
		if(handled_help(topic,contagion_pages,player,"contagion")) {
			return true;
		}
		if(handled_help(topic,ghost_pages,player,"ghost")) {
			return true;
		}
		return false;
	}

	bool should_continue(std::string_view from,std::string_view argument, std::shared_ptr<mods::player>& player,bool zero_is_help) {
		//bool show = false;
		//auto vec_args = PARSE_ARGS();
		//if(vec_args.size() == 0 && zero_is_help) {
		//	show = true;
		//}
		m_debug("should_continue: from:'" << from << "', argument: '" << argument << "'");
		if(is_exact_handled_help_match(argument,player)) {
			return false;
		}
		return true;
	}


	void send_class_header(player_ptr_t& player,std::string_view class_name) {
		uint8_t len = class_name.length();
		uint8_t bar_count = len + 2 + strlen(" class menu =");
		std::string header = "";
		for(unsigned i=0; i < bar_count; ++i) {
			header += "=";
		}
		header += CAT("\r\n= ",class_name.data()," class menu =\r\n");
		for(unsigned i=0; i < bar_count; ++i) {
			header += "=";
		}
		player->sendln(header);
	}


	void send_class_footer(player_ptr_t& player,std::string_view class_name) {
		uint8_t len = class_name.length();
		uint8_t bar_count = len + 2 + strlen(" class menu =");
		std::string header = "";
		for(unsigned i=0; i < bar_count; ++i) {
			header += "=";
		}
		player->sendln(header);
	}

	void send_contagion_help_menu(player_ptr_t& player) {
		player->pager_start();
		send_class_header(player,"CONTAGION");
		for(const auto& topic : contagion_pages) {
			player->sendln(send_topic(topic));
		}
		send_class_footer(player,"CONTAGION");
		player->pager_end();
		player->page(0);
	}
	void send_ghost_help_menu(player_ptr_t& player) {
		player->pager_start();
		send_class_header(player,"GHOST");
		for(const auto& topic : ghost_pages) {
			player->sendln(send_topic(topic));
		}
		send_class_footer(player,"GHOST");
		player->pager_end();
		player->page(0);
	}
	ACMD(do_help) {
		if(IS_NPC(ch)) {
			return;
		}
		auto vec_args = PARSE_ARGS();
		send_help(vec_args,player);
	}
	bool send_help(std::string_view topic,player_ptr_t& player) {
		if(is_exact_handled_help_match(topic,player)) {
			return false;
		}
		return true;

	}
	void send_help(const std::vector<std::string>& vec_args,player_ptr_t& player) {
		std::vector<std::string> screen;
		static constexpr std::string_view search_help_msg = "\r\n{grn}help:search{/grn} can be use to search every command in the mud. Try it out!\r\n";

		if(vec_args.size()) {
			if(is_exact_handled_help_match(vec_args[0],player)) {
				return;
			}
			if(is_class_help_match(vec_args[0],player)) {
				return;
			}
			if(vec_args[0].compare("builder") == 0 && (
			        player->implementor_mode() || player->builder_mode()
			    )) {
				send_builder_help(player);
				return ;
			}
		}
		if(vec_args.size() == 0) {
			send_player_class_help(player);
			return;
		}

		if(vec_args.size() == 0 || vec_args[0].compare("index") == 0) {
			player->sendln(search_help_msg.data());
		}

	}
	ACMD(do_search_help) {
		auto vec_args = PARSE_ARGS();
		if(!argshave()->size_gt(0)->passed()) {
			player->sendln("Usage: help:search <phrase>");
			return;
		}
		bool is_builder = false;
		static bool mortal_initialized = false;
		static bool builder_initialized = false;
		static std::vector<std::string> mortal_screen, builder_screen;
		if(!mortal_initialized) {
			mods::help::fetch_mortal_help(mortal_screen);
			mortal_initialized = true;
		}
		if(player->implementor_mode() || player->builder_mode()) {
			is_builder = true;
			if(!builder_initialized) {
				mods::help::fetch_builder_help(builder_screen);
				builder_initialized = true;
			}
		}
		player->pager_start();
		player->sendln("Searching every command...");
		mods::search_screen<player_ptr_t>(player,mortal_screen,vec_args,64);
		if(is_builder) {
			mods::search_screen<player_ptr_t>(player,builder_screen,vec_args,64);
		}
		player->pager_end();
		player->page(0);
		return;
	}
	std::map<std::string,std::string> get_help_pages() {
		return {};
	}

	void place_in_sql() {
		auto delete_first = boot_flag("--import-help-pages-delete-first");
		auto import_pages = boot_flag("--import-help-pages");
		if(!import_pages) {
			log("boot flag for help pages isn't present...");
			return;
		} else {
			log("boot flag for help pages is there");
		}
		if(delete_first) {
			log("Preparing help pages. Deleting existing data...");
			try {
				auto txn2 = txn();
				auto sql = sql_compositor("help_pages",&txn2)
				    .del()
				    .from("help_pages")
				    .sql();
				mods::pq::exec(txn2,sql);
				mods::pq::commit(txn2);
			} catch(std::exception& e) {
				REPORT_DB_ISSUE("failed",e.what());
				std::cerr << std::string("failed: ") + e.what() << "\n";
			}
			log("Existing help pages deleted.");
		}
		auto place = [&](auto section, auto cmd, auto content) {
			try {
				auto txn2 = txn();
				auto sql = sql_compositor("help_pages",&txn2)
				    .insert()
				    .into("help_pages")
				.values({
					{"hp_section", section},
					{"hp_cmd", cmd},
					{"hp_content", content},
				})
				.sql();
				mods::pq::exec(txn2,sql);
				mods::pq::commit(txn2);
			} catch(std::exception& e) {
				REPORT_DB_ISSUE("failed",e.what());
				std::cerr << std::string("failed: ") + e.what() << "\n";
			}
		};
		log("Importing help pages...");
		for(const auto& pair : get_help_pages()) {
			const std::string f = pair.first;
			place(f,f,pair.second);
		}
		log("Done importing help pages.");
	}
	std::string remove_md_extension(std::string s) {
		std::string f;
		for(std::size_t i=0; i < s.length(); i++) {
			if(s[i] == '.' && i + 3 == s.length() && s[i+1] == 'm' && s[i+2] == 'd') {
				return f;
			}
			f += s[i];
		}
		return f;
	}
	void read_flat_help_files() {
		std::string help_dir = MENTOC_CURRENT_WORKING_DIR;
		help_dir += "help/";
		for(const auto& p_class : dirs) {
			std::string glob = help_dir + p_class;
			m_debug("glob: '" << glob << "'");
			DIR * fp = opendir(glob.c_str());
			if(fp == nullptr) {
				m_debug("[ERROR]: cannot open glob'd directory: '" << glob << "'");
				continue;
			}
			while(dirent * entry = readdir(fp)) {
				if(entry->d_type == DT_UNKNOWN || entry->d_type == DT_DIR || entry->d_type == DT_LNK) {
					m_debug("[DEBUG] skipping '" << entry->d_name << "'");
					continue;
				}
				if(!mods::filesystem::has_extension(entry->d_name,"md")) {
					continue;
				}
				if(entry->d_type == DT_REG) {
					std::string path = glob + "/";
					path += entry->d_name;
					std::string page_name = p_class + std::string(":") + remove_md_extension(entry->d_name);
					m_debug("[DEBUG] page name: '" << page_name << "'");
					if(p_class.compare("breacher") == 0) {
						breacher_pages.emplace_back(page_name);
					} else if(p_class.compare("marine") == 0) {
						marine_pages.emplace_back(page_name);
					} else if(p_class.compare("contagion") == 0) {
						contagion_pages.emplace_back(page_name);
					} else if(p_class.compare("ghost") == 0) {
						ghost_pages.emplace_back(page_name);
					} else {
						m_debug("WARNING: didn't match a page index for: '" << p_class << "'");
					}
				}
			}
			closedir(fp);
		}
	}
	void read_global_flat_help_files() {
		std::string help_dir = MENTOC_CURRENT_WORKING_DIR;
		help_dir += "help/";
		DIR * fp = opendir(help_dir.c_str());
		if(fp == nullptr) {
			return;
		}
		while(dirent * entry = readdir(fp)) {
			if(entry->d_type == DT_UNKNOWN || entry->d_type == DT_DIR || entry->d_type == DT_LNK) {
				continue;
			}
			if(!mods::filesystem::has_extension(entry->d_name,"md")) {
				continue;
			}
			if(entry->d_type == DT_REG) {
				std::string path = help_dir;
				path += entry->d_name;
				std::string page_name = remove_md_extension(entry->d_name);
				m_debug("[DEBUG] page name: '" << page_name << "'");
				generic_pages.emplace_back(page_name);
			}
		}
		closedir(fp);
	}
	void clear_flat_help_files() {
		breacher_pages.clear();
		marine_pages.clear();
		contagion_pages.clear();
		ghost_pages.clear();
	}
	void clear_global_flat_help_files() {
		generic_pages.clear();
	}

	SUPERCMD(do_help_refresh) {
		ADMIN_REJECT();
		clear_flat_help_files();
		read_flat_help_files();
		clear_global_flat_help_files();
		read_global_flat_help_files();
		ADMIN_DONE();
	}
	SUPERCMD(do_builder_help) {
		ADMIN_REJECT();
		send_builder_help(player);
		ADMIN_DONE();
	}
	SUPERCMD(do_help_dump) {
		ADMIN_REJECT();
		for(const auto& section : {
		        marine_pages,
		        contagion_pages,
		    }) {
			for(const auto& pg : section) {
				player->sendln(pg);
			}
		}
		ADMIN_DONE();
	}

	void init() {
		place_in_sql();
		mods::interpreter::add_command("builder_help", POS_RESTING, do_builder_help, LVL_BUILDER,0);
		mods::interpreter::add_command("help", POS_RESTING, do_help, 0,0);
		mods::interpreter::add_command("help:search", POS_RESTING, do_search_help, 0,0);
		mods::interpreter::add_command("admin:help:refresh", POS_RESTING, do_help_refresh, LVL_BUILDER,0);
		mods::interpreter::add_command("admin:help:dump", POS_RESTING, do_help_dump, LVL_BUILDER,0);
		read_flat_help_files();
		read_global_flat_help_files();
	}
};
