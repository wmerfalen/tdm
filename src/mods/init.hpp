#ifndef __MENTOC_MODS_INIT_HEADER__
#define __MENTOC_MODS_INIT_HEADER__

#define INIT(A) \
	namespace A { extern void init(); };
/**
 * Community edition
 */
INIT(mods::dialog_tree);
INIT(mods::drops);
INIT(mods::flee);
INIT(mods::suicide);
INIT(mods::builder::raid);
INIT(mods::builder::paybuild);
INIT(mods::builder::staticlootbuild);
INIT(mods::builder::ammobuild);
INIT(mods::memory);
INIT(mods::weapons::shotgun::dst7a_commands);
INIT(mods::armor::bc89_commands);
INIT(mods::armor::xtv89_commands);
INIT(mods::weapons::smg::vc88_commands);
INIT(mods::json);
INIT(mods::combat::commands);
INIT(mods::players::friendly_reminders);
INIT(mods::players::event_messages);
INIT(mods::prefs);
INIT(mods::players::banish);
INIT(mods::rand);
INIT(mods::mp);
INIT(mods::bleed);
INIT(mods::blind);
INIT(mods::melt);
INIT(builder);
INIT(mods::date_time);
INIT(mods::integral_objects);
INIT(mods::interpreter);
INIT(offensive);
INIT(informative);
INIT(mods::query_objects);
INIT(mods::target_practice);
INIT(mods::class_abilities);
INIT(mods::values);
INIT(mods::skills);
INIT(mods::rate_limiting);
INIT(mods::levels);
INIT(mods::orm::pba);
INIT(mods::classes);
INIT(mods::loot);
INIT(mods::forge_engine::value_scaler_static);
INIT(mods::weapons::damage_types);
INIT(mods::fluxkraft);
INIT(mods::fluxkraft::arcon_bar);
INIT(mods::builder::meqbuild);
INIT(mods::builder::hqbuild);
INIT(mods::contracts);
INIT(mods::players::messages);
INIT(mods::builder::conbuild);
INIT(mods::rifle_attachments);
INIT(mods::super_users);
INIT(mods::builder::bookmarks);
INIT(mods::builder);
INIT(mods::zone);
INIT(mods::mob_equipment);
INIT(mods::weapons::reload);
INIT(mods::help);
INIT(mods::mobact);
INIT(mods::scripted_step_init);
INIT(mods::scripted_sequences_init);
INIT(mods::builder::seqbuild);
INIT(mods::contract_step_callback_init);
INIT(mods::scripted_sequence_events);
INIT(mods::scripted_sequence_runner);
INIT(src::act::debug);
INIT(mods::projectile);
INIT(mods::melee::combat_order);
INIT(mods::melee::stance);
INIT(mods::melee::main);
INIT(mods::corpse);
INIT(mods::ban_system::commands);
INIT(mods::ensnare);
INIT(mods::admin_tools::stay);
INIT(mods::rifle_attachments::forge_commands);

/**
 * Grey area
 */
INIT(mods::combat_composer);

#ifdef __MENTOC_PLUGIN_CONTENT__
	#include "plugins/init.hpp"
#endif

#undef INIT


namespace mods::init {
	static inline void init() {
		mods::rand::init();
		mods::integral_objects::init();
		mods::query_objects::init();
		mods::date_time::init();
		mods::target_practice::init();
		//mods::class_abilities::init();
		//mods::class_abilities::forge::init();
		mods::values::init();
		mods::skills::init();
		mods::rate_limiting::init();
		mods::levels::init();
		mods::orm::pba::init();
		mods::classes::init();
		mods::loot::init();
		mods::forge_engine::value_scaler_static::init();
		mods::weapons::damage_types::init();
		mods::fluxkraft::init();
		mods::fluxkraft::arcon_bar::init();
		mods::builder::meqbuild::init();
		mods::builder::hqbuild::init();
		mods::builder::conbuild::init();
		mods::contracts::init();
		mods::players::messages::init();
		::offensive::init();
		::builder::init();
		mods::super_users::init();
		::informative::init();
		mods::builder::bookmarks::init();
		mods::builder::init();
		mods::zone::init();
		mods::mob_equipment::init();
		mods::weapons::reload::init();
		mods::help::init();
		mods::mobact::init();
		mods::scripted_step_init::init();
		mods::scripted_sequences_init::init();
		mods::builder::seqbuild::init();
		mods::builder::paybuild::init();
		mods::builder::staticlootbuild::init();
		mods::builder::ammobuild::init();
		mods::contract_step_callback_init::init();
		mods::scripted_sequence_events::init();
		mods::scripted_sequence_runner::init();
		src::act::debug::init();
		mods::players::banish::init();
		mods::projectile::init();
		mods::loot::init();
		mods::bleed::init();
		mods::blind::init();
		mods::melt::init();
		mods::prefs::init();
		mods::players::friendly_reminders::init();
		mods::players::event_messages::init();
		mods::mp::init();
		mods::melee::combat_order::init();
		mods::melee::stance::init();
		mods::melee::main::init();
		mods::corpse::init();
		mods::combat_composer::init();
		mods::combat::commands::init();
		mods::json::init();
		mods::ban_system::commands::init();
		mods::ensnare::init();
		mods::admin_tools::stay::init();
		mods::weapons::shotgun::dst7a_commands::init();
		mods::weapons::smg::vc88_commands::init();
		mods::armor::bc89_commands::init();
		mods::armor::xtv89_commands::init();
		mods::memory::init();
		mods::builder::raid::init();
		mods::flee::init();
		mods::suicide::init();
		mods::drops::init();
		mods::rifle_attachments::forge_commands::init();
		//mods::dialog_tree::init();
#ifdef __MENTOC_PLUGIN_CONTENT__
		mods::plugins::init::init();
#endif
	}
};
#endif
