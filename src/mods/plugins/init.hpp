#ifndef __MENTOC_MODS_PLUGINS_INIT_HEADER__
#define __MENTOC_MODS_PLUGINS_INIT_HEADER__

#ifndef INIT
#define INIT(A) \
	namespace A { extern void init(); };
#endif
/**
 * TDMud content
 */
INIT(mods::class_abilities::breacher);
INIT(mods::class_abilities::marine);
INIT(mods::class_abilities::ghost);
INIT(mods::class_abilities::contagion);
INIT(demolitions);
INIT(mods::boosters::adrenaline_shot_init);
INIT(mods::mobs::defiler_init);

namespace mods::plugins::init {
	static inline void init() {
		mods::class_abilities::ghost::init();
		mods::class_abilities::contagion::init();
		mods::class_abilities::marine::init();
		mods::class_abilities::breacher::init();
		mods::rifle_attachments::init();
		::demolitions::init();
		mods::boosters::adrenaline_shot_init::init();
		mods::mobs::defiler_init::init();
	}
};
#endif
