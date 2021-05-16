# Description
CircleMudPlusPlus is CircleMUD with modern C++ improvements. Although 
based on the C code of CircleMUD, we strive to fully utilize modern C++ features and libraries.
Many parts of the CircleMUD core have been rewritten in C++. Performance
enhancements have been a major focus of this project. Many of the CircleMUD
structures were implemented as linked lists. These were converted to vectors
wherever possible. Rooms, zones, and objects have been converted and moved
over to a postgresql database. 

# Motivation
Purely a hobby project to strengthen and reinforce my C++ skills.
I do plan on launching and maintaining releases as long as my
current full-time job permits me the free time to do so. 

# Performance Improvements
* The CircleMUD reliance on files as storage is being widely avoided in this project. Instead, postgresql is 
being utilized to store and manage in-game assets.  
* Many structures that were formerly implemented as linked lists in the CircleMUD base code have been revamped and implemented as std::vector. A full rewrite would be necessary to revamp all linked lists, so unfortunately (for now atleast) there are still linked lists in the source code.

# Cool Features 
* duktape js has been embedded to allow for powerful scripting features. 
* A quests system has been built, but is in the early stages -- it has a long way to go. 
* Sniping mechanics are a main focal point. The motivation behind this is to provide a sniping mechanic for players
who don't mind kiting around a mob and avoiding melee. 

# Plans
* See src/mods/todo for a list of things I want to do with the codebase
* Mob movement is going to be revamped to challenge sniping players with the threat of melee damage. Currently, mobact is responsible for a lot of the mob actions, and this hasn't changed much from the original CircleMUD base.
* Weapons that can level up with you (CyberAssault-inspired).


# Compiling

You'll need g++-7 to be in your include path. Simply go to the src directory
and execute 'make'. Currently, compiling on Windows systems is not supported.

#Latest improvements
beta features:
- Deploying a test suite within src/scripts/test-suites/A-\*. Where '\*' is either a or b
- Behaviour tree for tracking down snipers

# Dependencies
* libpqxx
* duktape
* postgresql
