#include "levels.hpp"
#include "help.hpp"
#include "super-users.hpp"
#include "interpreter.hpp"
#include "screen-searcher.hpp"
#include "players/util.hpp"
#include "classes/advance-levels.hpp"
#include "rand.hpp"

#ifdef __MENTOC_MODS_LEVELS_DEBUG__
#define mo_debug(A) std::cerr << "[mods::levels][debug]:" << A <<"\n";
#else
#define mo_debug(A)
#endif

extern std::string sanitize_key(std::string key);
extern void obj_to_obj(obj_ptr_t from_object, obj_ptr_t to_object);
namespace mods::levels {
	enum triad : uint8_t {
		MELEE = 0,
		WEAPONS,
		INTEL,
		SPEED,
		ARMOR
	};
	void gain_exp(player_ptr_t player,int gain){
		if(IS_NPC(player->cd())){
			return;
		}
		/** TODO fill me in */
		player->send("[stub] file:%s line:%d\r\n",__FILE__,__LINE__);
	}
#ifdef __MENTOC_USE_RANDOM_STAT_GAIN__
	int calculate_bonus_hp_gain(int dice,int sides,int greater_than,int player_level){
		int amount = mods::rand::roll(1,6);
		if(greater_than < mods::rand::roll(dice,sides)){
			 amount = ADVANCE_LEVEL_HP_FLOAT_MULTIPLIER() * player_level;
		}
		return amount;
	}
	int calculate_bonus_move_gain(int dice,int sides,int greater_than,int player_level){
		int amount = mods::rand::roll(1,6);
		if(greater_than < mods::rand::roll(dice,sides)){
			 amount += ADVANCE_LEVEL_HP_FLOAT_MULTIPLIER() * player_level;
		}
		return amount;
	}
#endif

	/**
	 * extra rewards for advancing
	 */
	void reward_player_for_advancing_levels(player_ptr_t& player){
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


	void report_advances(std::array<float,STAT_INDEXES_SIZE> stats){
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

	std::array<uint8_t,5> get_triads_by_class(player_class_t c){
		std::array<uint8_t,5> m_triads;
			switch(c){
				case GHOST:
					m_triads = {
						GHOST_CLASS_TRIADS_MELEE(),
						GHOST_CLASS_TRIADS_WEAPONS(),
						GHOST_CLASS_TRIADS_INTEL(),
						GHOST_CLASS_TRIADS_SPEED(),
						GHOST_CLASS_TRIADS_ARMOR()
					};
					break;
				case MARKSMAN:
					m_triads = {
						MARKSMAN_CLASS_TRIADS_MELEE(),
						MARKSMAN_CLASS_TRIADS_WEAPONS(),
						MARKSMAN_CLASS_TRIADS_INTEL(),
						MARKSMAN_CLASS_TRIADS_SPEED(),
						MARKSMAN_CLASS_TRIADS_ARMOR()
					};
					break;
				case BANDIT:
					m_triads = {
						BANDIT_CLASS_TRIADS_MELEE(),
						BANDIT_CLASS_TRIADS_WEAPONS(),
						BANDIT_CLASS_TRIADS_INTEL(),
						BANDIT_CLASS_TRIADS_SPEED(),
						BANDIT_CLASS_TRIADS_ARMOR()
					};
					break;
				case BUTCHER:
					m_triads = {
						BUTCHER_CLASS_TRIADS_MELEE(),
						BUTCHER_CLASS_TRIADS_WEAPONS(),
						BUTCHER_CLASS_TRIADS_INTEL(),
						BUTCHER_CLASS_TRIADS_SPEED(),
						BUTCHER_CLASS_TRIADS_ARMOR()
					};
					break;
				case STRIKER:
					m_triads = {
						STRIKER_CLASS_TRIADS_MELEE(),
						STRIKER_CLASS_TRIADS_WEAPONS(),
						STRIKER_CLASS_TRIADS_INTEL(),
						STRIKER_CLASS_TRIADS_SPEED(),
						STRIKER_CLASS_TRIADS_ARMOR()
					};
					break;
				case OBSTRUCTOR:
					m_triads = {
						OBSTRUCTOR_CLASS_TRIADS_MELEE(),
						OBSTRUCTOR_CLASS_TRIADS_WEAPONS(),
						OBSTRUCTOR_CLASS_TRIADS_INTEL(),
						OBSTRUCTOR_CLASS_TRIADS_SPEED(),
						OBSTRUCTOR_CLASS_TRIADS_ARMOR()
					};
					break;
				case MALADY:
					m_triads = {
						MALADY_CLASS_TRIADS_MELEE(),
						MALADY_CLASS_TRIADS_WEAPONS(),
						MALADY_CLASS_TRIADS_INTEL(),
						MALADY_CLASS_TRIADS_SPEED(),
						MALADY_CLASS_TRIADS_ARMOR()
					};
					break;
				case PYREXIA:
					m_triads = {
						PYREXIA_CLASS_TRIADS_MELEE(),
						PYREXIA_CLASS_TRIADS_WEAPONS(),
						PYREXIA_CLASS_TRIADS_INTEL(),
						PYREXIA_CLASS_TRIADS_SPEED(),
						PYREXIA_CLASS_TRIADS_ARMOR()
					};
					break;
				case DEALER:
					m_triads = {
						DEALER_CLASS_TRIADS_MELEE(),
						DEALER_CLASS_TRIADS_WEAPONS(),
						DEALER_CLASS_TRIADS_INTEL(),
						DEALER_CLASS_TRIADS_SPEED(),
						DEALER_CLASS_TRIADS_ARMOR()
					};
					break;
				case FORGE:
					m_triads = {
						FORGE_CLASS_TRIADS_MELEE(),
						FORGE_CLASS_TRIADS_WEAPONS(),
						FORGE_CLASS_TRIADS_INTEL(),
						FORGE_CLASS_TRIADS_SPEED(),
						FORGE_CLASS_TRIADS_ARMOR()
					};
					break;
				case SYNDROME:
					m_triads = {
						SYNDROME_CLASS_TRIADS_MELEE(),
						SYNDROME_CLASS_TRIADS_WEAPONS(),
						SYNDROME_CLASS_TRIADS_INTEL(),
						SYNDROME_CLASS_TRIADS_SPEED(),
						SYNDROME_CLASS_TRIADS_ARMOR()
					};
					break;
				case MACHINIST:
					m_triads = {
						MACHINIST_CLASS_TRIADS_MELEE(),
						MACHINIST_CLASS_TRIADS_WEAPONS(),
						MACHINIST_CLASS_TRIADS_INTEL(),
						MACHINIST_CLASS_TRIADS_SPEED(),
						MACHINIST_CLASS_TRIADS_ARMOR()
					};
					break;
				default:
					std::cerr << red_str("[mods::player] UNKNOWN CLASS TYPE. Setting triads to 1!\n");
					m_triads = {
						1,1,1,1,1
					};
					break;
			}
			return m_triads;
	}

	std::array<float,STAT_INDEXES_SIZE> calculate_based_on_triads(std::array<uint8_t,5> triads,int level){

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
		static constexpr float FIRST_TIER = 0.60;
		static constexpr float SECOND_TIER = 0.44;
		static constexpr float THIRD_TIER = 0.22;
		static constexpr float PASSIVE_TIER = 0.12;
		static constexpr float SPLIT_TIER = 0.30;
		static constexpr float SPLIT_TIER_HALF = SPLIT_TIER / 2;
		switch(triads[MELEE]){
			case 1:
			case 2:
			case 3:
				hp += level * (FIRST_TIER * triads[MELEE]);
				strength += level * (FIRST_TIER * triads[MELEE]);
				dexterity += level * (FIRST_TIER * triads[MELEE]);
				constitution += level * (FIRST_TIER * triads[MELEE]);

				armor += level * (THIRD_TIER * triads[MELEE]);
				break;
			default:
				std::cerr << red_str("generic_advance_hp encountered odd MELEE triad:") << triads[MELEE] << "\n";
				break;
		}
		switch(triads[WEAPONS]){
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
		switch(triads[INTEL]){
			case 1:
			case 2:
			case 3:
				intelligence += level * (FIRST_TIER * triads[INTEL]);
				electronics += level * (FIRST_TIER * triads[INTEL]);
				chemistry += level * (FIRST_TIER * triads[INTEL]);

				demolitions += level * (SPLIT_TIER * triads[WEAPONS]);

				wisdom += level * (SECOND_TIER * triads[INTEL]);

				marksmanship += level * (PASSIVE_TIER * triads[INTEL]);
				break;
			default:
				std::cerr << red_str("generic_advance_hp encountered odd INTEL triad:") << triads[INTEL] << "\n";
				break;
		}
		switch(triads[SPEED]){
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
		switch(triads[ARMOR]){
			case 1:
			case 2:
			case 3:
				hp += level * (FIRST_TIER * triads[ARMOR]);
				constitution += level * (FIRST_TIER * triads[ARMOR]);
				armor += level * (FIRST_TIER * triads[ARMOR]);

				demolitions += level * (SPLIT_TIER_HALF * triads[WEAPONS]);
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


	void generic_advance(player_ptr_t& player){
		auto triads = player->triads();
		auto s = calculate_based_on_triads(triads,player->level());
		report_advances(s);
		player->hp() += s[STAT_HP];
		player->strength() += s[STAT_STRENGTH];
		player->constitution() += s[STAT_CONSTITUTION];
		player->dexterity() += s[STAT_DEXTERITY];
		player->intelligence() += s[STAT_INTELLIGENCE];
		player->wisdom() += s[STAT_WISDOM];
		player->electronics() += s[STAT_ELECTRONICS];
		player->chemistry() += s[STAT_CHEMISTRY];
		player->strategy() += s[STAT_STRATEGY];
		player->marksmanship() += s[STAT_MARKSMANSHIP];
		player->sniping() += s[STAT_SNIPING];
		player->weapon_handling() += s[STAT_WEAPON_HANDLING];
		player->demolitions() += s[STAT_DEMOLITIONS];
		player->real_armor() += s[STAT_ARMOR];
		player->medical() += s[STAT_MEDICAL];
		player->charisma() += s[STAT_CHARISMA];
	}

	
	/**
	 * This is the ideal function we want to use as it makes use of 
	 * the player's specific class pointer. i.e.: player->ghost()
	 * The hope is that once all classes have been fully implemented, 
	 * we can phase out the generic_* functions completely.
	 */
	void advance_level(player_ptr_t& player){
		generic_advance(player);
		switch(player->get_class()){
			case GHOST:
				mods::classes::ghost_advance_level(player);
				break;
			case PYREXIA:
				mods::classes::pyrexia_advance_level(player);
				break;
			default:
				std::cerr << red_str("[mods::levels::advance_level] unsupported class type: ") << player->get_class_string().str() << "\n";
				break;
		}
	}
	int award_exp(player_ptr_t& player,int exp){
		player->exp() += exp;
		return 0;
	}

	/**
	 * admin debug command
	 */
ACMD(do_award_exp_by_name){
	ADMIN_REJECT();
	DO_HELP_WITH_ZERO("award_exp_by_name");
	/** code here */
	auto vec_args = PARSE_ARGS();
	if(vec_args.size() < 2){
		SEND_HELP("award_exp_by_name");
		return;
	}

	auto found = mods::players::util::find_player_by_name(vec_args[0]);
	if(!found.has_value()){
		player->sendln("Couldn't find a player by that name.");
		return;
	}
	auto found_player = ptr_by_uuid(found.value());
	player->sendln("Setting...");
	auto opt = mods::util::stoi_optional<int>(vec_args[1]);
	if(!opt.has_value()){
		player->errorln("Invalid integer encountered. Please specify a valid amount of experience points.");
		return;
	}
	int used_to = found_player->exp();
	mods::levels::award_exp(found_player,opt.value());
	player->send("Player '%s' used to have %d experience points and now has %d\r\n",found_player->name().c_str(),used_to,found_player->exp());
	player->sendln("Done.");
	ADMIN_DONE();
}

	void init(){
		mods::interpreter::add_command("award_exp_by_name", POS_RESTING, do_award_exp_by_name, LVL_BUILDER,0);
	}
};
#undef mo_debug
