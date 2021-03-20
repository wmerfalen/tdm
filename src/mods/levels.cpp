#include "levels.hpp"
#include "help.hpp"
#include "super-users.hpp"
#include "interpreter.hpp"
#include "screen-searcher.hpp"
#include "players/util.hpp"
#include "classes/advance-levels.hpp"
#include "rand.hpp"
#include "players/db-load.hpp"

#ifdef __MENTOC_MODS_LEVELS_DEBUG__
#define mo_debug(A) std::cerr << "[mods::levels][debug]:" << A <<"\n";
#else
#define mo_debug(A)
#endif

extern std::string sanitize_key(std::string key);
extern void obj_to_obj(obj_ptr_t from_object, obj_ptr_t to_object);
extern int immort_level_ok;
namespace mods::levels {
	static constexpr uint32_t EXP_MAX = 10000000;
	int level_exp(int level) {
		if(level > LVL_IMPL || level < 0) {
			log("SYSERR: Requesting exp for invalid level %d!", level);
			return 0;
		}

		/*
		 * Gods have exp close to EXP_MAX.  This statement should never have to
		 * changed, regardless of how many mortal or immortal levels exist.
		 */
		if(level > LVL_IMMORT) {
			return EXP_MAX - ((LVL_IMPL - level) * 1000);
		}

		switch(level) {
			case  0:
				return 0;

			case  1:
				return 1;

			case  2:
				return 2500;

			case  3:
				return 5000;

			case  4:
				return 10000;

			case  5:
				return 20000;

			case  6:
				return 40000;

			case  7:
				return 60000;

			case  8:
				return 90000;

			case  9:
				return 135000;

			case 10:
				return 250000;

			case 11:
				return 375000;

			case 12:
				return 750000;

			case 13:
				return 1125000;

			case 14:
				return 1500000;

			case 15:
				return 1875000;

			case 16:
				return 2250000;

			case 17:
				return 2625000;

			case 18:
				return 3000000;

			case 19:
				return 3375000;

			case 20:
				return 3750000;

			case 21:
				return 4000000;

			case 22:
				return 4300000;

			case 23:
				return 4600000;

			case 24:
				return 4900000;

			case 25:
				return 5200000;

			case 26:
				return 5500000;

			case 27:
				return 5950000;

			case 28:
				return 6400000;

			case 29:
				return 6850000;

			case 30:
				return 7400000;

			/* add new levels here */
			default:
			case LVL_IMMORT:
				return 8000000;
		}


		/*
		 * This statement should never be reached if the exp tables in this function
		 * are set up properly.  If you see exp of 123456 then the tables above are
		 * incomplete -- so, complete them!
		 */
		log("SYSERR: XP tables not set up correctly in %s:%d",__FILE__,__LINE__);
		return -1;
	}
	int gain_exp(player_ptr_t& player,int gain) {
		if(IS_NPC(player->cd())) {
			return -1;
		}
		player->exp() += gain;
		bool is_altered = 0;
		int num_levels = 0;
		if(player->level() == 0) {
			player->level() = 1;
			advance_level(player);
		}
		while(player->level() < LVL_IMMORT - immort_level_ok &&
		        player->exp() >= mods::levels::level_exp(player->level() + 1)) {
			player->level() += 1;
			num_levels++;
			advance_level(player);
			is_altered = TRUE;
			if(is_altered) {
				mudlog(BRF, MAX(LVL_IMMORT, GET_INVIS_LEV(player->cd())), TRUE, "%s advanced %d level%s to level %d.",
				       player->name().c_str(), num_levels, num_levels == 1 ? "" : "s", player->level());
				if(num_levels == 1) {
					player->sendln(ADVANCE_LEVEL_MESSAGE());
				} else {
					player->send(ADVANCE_MULTIPLE_LEVELS_MESSAGE().c_str(),num_levels);
				}
				//set_title(ch, NULL);
			}
		}//end while
		std::cerr << green_str("gain_exp has your level as: ") << std::to_string(player->level()) << "\n";
		mods::players::db_load::save_from(player,mods::players::db_load::save_from_t::GAIN_EXP);
		return num_levels;
	}
#ifdef __MENTOC_USE_RANDOM_STAT_GAIN__
	int calculate_bonus_hp_gain(int dice,int sides,int greater_than,int player_level) {
		int amount = mods::rand::roll(1,6);
		if(greater_than < mods::rand::roll(dice,sides)) {
			amount = ADVANCE_LEVEL_HP_FLOAT_MULTIPLIER() * player_level;
		}
		return amount;
	}
	int calculate_bonus_move_gain(int dice,int sides,int greater_than,int player_level) {
		int amount = mods::rand::roll(1,6);
		if(greater_than < mods::rand::roll(dice,sides)) {
			amount += ADVANCE_LEVEL_HP_FLOAT_MULTIPLIER() * player_level;
		}
		return amount;
	}
#endif

	/**
	 * extra rewards for advancing
	 */
	void reward_player_for_advancing_levels(player_ptr_t& player) {
		player->sendln(ADVANCE_LEVEL_CONGRATS());

#ifdef __MENTOC_USE_RANDOM_STAT_GAIN__
		auto bonus_hp = calculate_bonus_hp_gain(
		                    ADVANCE_LEVEL_BONUS_HP_DICE(),
		                    ADVANCE_LEVEL_BONUS_HP_SIDES(),
		                    ADVANCE_LEVEL_BONUS_HP_THRESHOLD(),
		                    player->level()
		                );
		/** Random chance of additional move gain */
		auto bonus_move = calculate_bonus_move_gain(
		                      ADVANCE_LEVEL_BONUS_HP_DICE(),
		                      ADVANCE_LEVEL_BONUS_HP_SIDES(),
		                      ADVANCE_LEVEL_BONUS_HP_THRESHOLD(),
		                      player->level()
		                  );
#endif

	}


	void report_advances(std::array<float,STAT_INDEXES_SIZE> stats) {
		std::cerr <<
		          "hp:" << stats[STAT_HP] << "\n"\
		          << "strength:		" << stats[STAT_STRENGTH] << "\n"\
		          << "constitution:		" << stats[STAT_CONSTITUTION] << "\n"\
		          << "dexterity:		" << stats[STAT_DEXTERITY] << "\n"\
		          << "intelligence:		" << stats[STAT_INTELLIGENCE]<< "\n"\
		          << "wisdom:		" << stats[STAT_WISDOM] << "\n"\
		          << "electronics:		" << stats[STAT_ELECTRONICS] << "\n"\
		          << "chemistry:		" << stats[STAT_CHEMISTRY] << "\n"\
		          << "strategy:		" << stats[STAT_STRATEGY]<< "\n"\
		          << "marksmanship:		" << stats[STAT_MARKSMANSHIP] << "\n"\
		          << "sniping:		" << stats[STAT_SNIPING] << "\n"\
		          << "weapon_handling:		" << stats[STAT_WEAPON_HANDLING] << "\n"\
		          << "demolitions:		" << stats[STAT_DEMOLITIONS] << "\n"\
		          << "armor:		" << stats[STAT_ARMOR] << "\n"\
		          << "medical:		" << stats[STAT_MEDICAL] << "\n"\
		          << "charisma:		" << stats[STAT_CHARISMA] << "\n";
	}

	std::array<uint8_t,5> get_triads_by_class(player_class_t c) {
		std::array<uint8_t,5> m_triads;
		m_triads = {
			2, /** MELEE */
			2, /** WEAPONS */
			2, /** INTEL */
			2, /** SPEED */
			2  /** ARMOR */
		};
		return m_triads;
	}

	std::array<float,STAT_INDEXES_SIZE> calculate_based_on_triads(std::array<uint8_t,5> triads,int level) {

		std::array<float,STAT_INDEXES_SIZE> s = {0};
		float
		/** melee and armor related */
		hp = 0,	/** also speed related */

		/** melee related */
		strength = 0,
		constitution = 0,	/** also speed related */
		dexterity = 0,		/** also speed related */

		/** intel related */
		intelligence =0,
		wisdom = 0,
		electronics = 0,
		chemistry = 0,
		strategy = 0,

		/** weapons related */
		marksmanship = 0,
		sniping = 0,
		weapon_handling = 0,	/** also speed related */

		/** weapons and intel related */
		demolitions = 0,	/** armor triad value reduces explosive damage */

		/** armor related */
		armor = 0,

		/** armor and intel related */
		medical = 0,

		/** unclassified */
		charisma = 0
		           ;
		float FIRST_TIER = LEVELS_FIRST_TIER();
		float SECOND_TIER = FIRST_TIER * 0.75;
		float THIRD_TIER = FIRST_TIER * 0.50;
		float PASSIVE_TIER = FIRST_TIER * 0.25;
		float SPLIT_TIER = FIRST_TIER * 0.10;
		float SPLIT_TIER_HALF = SPLIT_TIER / 2;
		switch(triads[MELEE]) {
			case 1:
			case 2:
			case 3:
				hp += level * (FIRST_TIER * triads[MELEE]);
				strength += level * (FIRST_TIER * triads[MELEE]);
				dexterity += level * (FIRST_TIER * triads[MELEE]);
				constitution += level * (FIRST_TIER * triads[MELEE]);

				armor += level * (THIRD_TIER * triads[MELEE]);

				charisma += level * (PASSIVE_TIER * triads[INTEL]);
				medical += level * (PASSIVE_TIER * triads[INTEL]);
				break;
			default:
				std::cerr << red_str("generic_advance_hp encountered odd MELEE triad:") << triads[MELEE] << "\n";
				break;
		}
		switch(triads[WEAPONS]) {
			case 1:
			case 2:
			case 3:
				weapon_handling += level * (FIRST_TIER * triads[WEAPONS]);
				sniping += level * (FIRST_TIER * triads[WEAPONS]);
				marksmanship += level * (FIRST_TIER * triads[WEAPONS]);

				demolitions += level * (SPLIT_TIER * triads[WEAPONS]);

				armor += level * (PASSIVE_TIER * triads[MELEE]);
				break;
			default:
				std::cerr << red_str("generic_advance_hp encountered odd WEAPONS triad:") << triads[WEAPONS] << "\n";
				break;
		}
		switch(triads[INTEL]) {
			case 1:
			case 2:
			case 3:
				intelligence += level * (FIRST_TIER * triads[INTEL]);
				electronics += level * (FIRST_TIER * triads[INTEL]);
				chemistry += level * (FIRST_TIER * triads[INTEL]);
				strategy += level * (FIRST_TIER * triads[INTEL]);
				medical += level * (FIRST_TIER * triads[INTEL]);
				charisma += level * (FIRST_TIER * triads[INTEL]);

				wisdom += level * (SECOND_TIER * triads[INTEL]);

				demolitions += level * (SPLIT_TIER * triads[WEAPONS]);

				marksmanship += level * (PASSIVE_TIER * triads[INTEL]);
				break;
			default:
				std::cerr << red_str("generic_advance_hp encountered odd INTEL triad:") << triads[INTEL] << "\n";
				break;
		}
		switch(triads[SPEED]) {
			case 1:
			case 2:
			case 3:
				constitution += level * (FIRST_TIER * triads[SPEED]);
				dexterity += level * (FIRST_TIER * triads[SPEED]);

				weapon_handling += level * (PASSIVE_TIER * triads[SPEED]);
				break;
			default:
				std::cerr << red_str("generic_advance_hp encountered odd SPEED triad:") << triads[SPEED] << "\n";
				break;
		}
		switch(triads[ARMOR]) {
			case 1:
			case 2:
			case 3:
				hp += level * (FIRST_TIER * triads[ARMOR]);
				constitution += level * (FIRST_TIER * triads[ARMOR]);
				armor += level * (FIRST_TIER * triads[ARMOR]);

				demolitions += level * (SPLIT_TIER_HALF * triads[WEAPONS]);

				medical += level * (PASSIVE_TIER * triads[INTEL]);
				break;
			default:
				std::cerr << red_str("generic_advance_hp encountered odd ARMOR triad:") << triads[ARMOR] << "\n";
				break;
		}
		s[STAT_HP] = hp;
		s[STAT_STRENGTH] = strength;
		s[STAT_CONSTITUTION] = constitution;
		s[STAT_DEXTERITY] = dexterity;
		s[STAT_INTELLIGENCE] = intelligence;
		s[STAT_WISDOM] = wisdom;
		s[STAT_ELECTRONICS] = electronics;
		s[STAT_CHEMISTRY] = chemistry;
		s[STAT_STRATEGY] = strategy;
		s[STAT_MARKSMANSHIP] = marksmanship;
		s[STAT_SNIPING] = sniping;
		s[STAT_WEAPON_HANDLING] = weapon_handling;
		s[STAT_DEMOLITIONS] = demolitions;
		s[STAT_ARMOR] = armor;
		s[STAT_MEDICAL] = medical;
		s[STAT_CHARISMA] = charisma;
		return s;
	}


	void generic_advance(player_ptr_t& player) {
		auto triads = player->triads();
		auto s = calculate_based_on_triads(triads,player->level());
		report_advances(s);
		player->hp() = s[STAT_HP];
		player->strength() = s[STAT_STRENGTH];
		player->constitution() = s[STAT_CONSTITUTION];
		player->dexterity() = s[STAT_DEXTERITY];
		player->intelligence() = s[STAT_INTELLIGENCE];
		player->wisdom() = s[STAT_WISDOM];
		player->electronics() = s[STAT_ELECTRONICS];
		player->chemistry() = s[STAT_CHEMISTRY];
		player->strategy() = s[STAT_STRATEGY];
		player->marksmanship() = s[STAT_MARKSMANSHIP];
		player->sniping() = s[STAT_SNIPING];
		player->weapon_handling() = s[STAT_WEAPON_HANDLING];
		player->demolitions() = s[STAT_DEMOLITIONS];
		player->real_armor() = s[STAT_ARMOR];
		player->medical() = s[STAT_MEDICAL];
		player->charisma() = s[STAT_CHARISMA];
	}


	/**
	 * This is the ideal function we want to use as it makes use of
	 * the player's specific class pointer. i.e.: player->ghost()
	 * The hope is that once all classes have been fully implemented,
	 * we can phase out the generic_* functions completely.
	 */
	void advance_level(player_ptr_t& player) {
		generic_advance(player);
		switch(player->get_class()) {
			case GHOST:
				mods::classes::ghost_advance_level(player);
				break;
			default:
				std::cerr << red_str("[mods::levels::advance_level] unsupported class type: ") << player->get_class_string().str() << "\n";
				break;
		}
	}
	static constexpr uint8_t MAX_PLAYER_LEVEL = 30;
	std::string get_practice_dump() {
		std::string report = "[practice sessions per level]\r\n";
		for(uint8_t i = 1; i <= MAX_PLAYER_LEVEL; ++i) {
			report += CAT("Player level: ",i,": ", calculate_available_practice_sessions(i), "\r\n");
		}
		return report;
	}
	uint8_t calculate_available_practice_sessions(uint8_t level) {
		uint8_t sessions = 0;
		for(uint8_t current_level = 1; current_level <= level; ++current_level) {
			sessions += award_practice_sessions(current_level);
		}
		return sessions;
	}
	uint8_t award_practice_sessions(uint8_t level) {
		return 1;
	}
	int csv_export_report() {
		FILE* fp = fopen("levels.csv", "w+");
		if(!fp) {
			return -1;
		}
		std::string header = "player_class,level,HP,Strength,con,dex,intel,wis,electronics,chemistry,strategy,marksmanship,sniping,weapon_handling,demolitions,armor,medical,charisma\n";
		fwrite(header.c_str(),sizeof(char),header.length(),fp);
		for(unsigned player_level = 1; player_level <= LVL_IMMORT; ++player_level) {
			std::string player_class = "all";//mods::util::player_class_to_string(cl_type);
			auto triads = mods::levels::get_triads_by_class(player_class_t::GHOST);
			auto stats = mods::levels::calculate_based_on_triads(triads,player_level);
			std::string s;
			s = player_class + "," + std::to_string(player_level) + ",";
			fwrite(s.c_str(),sizeof(char),s.length(),fp);
			for(int i=0; i < stats.size(); ++i) {
				s = std::to_string(stats[i]);
				if(stats.size() > i+1) {
					s += ",";
				}
				fwrite(s.c_str(), sizeof(char),s.length(),fp);
			}
			fwrite("\n",1,1,fp);
		}
		fclose(fp);
		return 0;
	}

	/**
	 * admin debug command
	 */
	ACMD(do_award_exp_by_name) {
		ADMIN_REJECT();
		DO_HELP_WITH_ZERO("award_exp_by_name");
		/** code here */
		auto vec_args = PARSE_ARGS();
		static const char* usage = "usage: award_exp_by_name <player> <exp>\r\n";
		if(vec_args.size() < 2) {
			player->errorln(usage);
			SEND_HELP("award_exp_by_name");
			return;
		}

		auto found = mods::players::util::find_player_by_name(vec_args[0]);
		if(!found.has_value()) {
			player->sendln("Couldn't find a player by that name.");
			player->errorln(usage);
			return;
		}
		auto found_player = ptr_by_uuid(found.value());
		player->sendln("Setting...");
		auto opt = mods::util::stoi_optional<int>(vec_args[1]);
		if(!opt.has_value()) {
			player->errorln("Invalid integer encountered. Please specify a valid amount of experience points.");
			player->errorln(usage);
			return;
		}
		int used_to = found_player->exp();
		mods::levels::gain_exp(found_player,opt.value());
		player->send("Player '%s' used to have %d experience points and now has %d\r\n",found_player->name().c_str(),used_to,found_player->exp());
		player->sendln("Done.");
		ADMIN_DONE();
	}
	ACMD(do_csv_export_levels) {
		ADMIN_REJECT();
		DO_HELP_WITH_ZERO("csv_export_levels");
		auto status = csv_export_report();
		if(status == -1) {
			player->errorln("Unable to write to levels.csv");
			return;
		}
		player->admin_success("Successfully wrote to levels.csv.");
		ADMIN_DONE();
	}
	ACMD(do_practices) {
		player->sendln(get_practice_dump());
	}
	ACMD(do_exp) {
		player->send("{grn}You have %d experience points.{/grn}\r\n",player->exp());
	}


	void init() {
		mods::interpreter::add_command("award_exp_by_name", POS_RESTING, do_award_exp_by_name, LVL_BUILDER,0);
		mods::interpreter::add_command("csv_export_levels", POS_RESTING, do_csv_export_levels, LVL_BUILDER,0);
		mods::interpreter::add_command("exp", POS_RESTING, do_exp, 0,0);
		mods::interpreter::add_command("practices", POS_RESTING, do_practices, 0,0);
	}
};
#undef mo_debug
