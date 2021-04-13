#ifndef __MENTOC_MODS_INIT_HEADER__
#define __MENTOC_MODS_INIT_HEADER__

#define INIT(A) \
	namespace A { extern void init(); };
INIT(mods::rand);
INIT(builder);
INIT(mods::date_time);
INIT(mods::integral_objects);
INIT(mods::interpreter);
INIT(offensive);
INIT(informative);
INIT(mods::query_objects);
INIT(mods::target_practice);
INIT(mods::class_abilities);
INIT(mods::class_abilities::forge);
INIT(mods::class_abilities::sniper);
INIT(mods::class_abilities::ghost);
INIT(mods::class_abilities::engineer);
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
INIT(demolitions);
INIT(mods::rifle_attachments);
INIT(mods::super_users);
INIT(mods::builder::bookmarks);
INIT(mods::builder);
INIT(mods::zone);
INIT(mods::mob_equipment);
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
		mods::class_abilities::sniper::init();
		mods::class_abilities::ghost::init();
		mods::class_abilities::engineer::init();
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
		::demolitions::init();
		mods::rifle_attachments::init();
		mods::super_users::init();
		::informative::init();
		mods::builder::bookmarks::init();
		mods::builder::init();
		mods::zone::init();
		mods::mob_equipment::init();
	}
};
#endif
