# Description
bnull-mud is CircleMUD with modern C++ improvements. bnull-mud, although 
based on the C code of CircleMUD, uses modern C++ features and libraries.
Many parts of the CircleMUD core have been rewritten in C++. Performance
enhancements have been a major focus of this project. Many of the CircleMUD
structures were implemented as linked lists. These were converted to vectors
wherever possible. Rooms, zones, and objects have been converted and moved
over to a postgresql database. 

# Motivation
bnull-mud is purely a hobby project to strengthen and reinforce my C++ skills.
I do plan on launching and maintaining bnull-mud full-time as long as my
current full-time job permits me the free time to do so. bnull-mud is my
first serious personal project that I have poured all my free time into. 

# Performance Improvements
* The CircleMUD reliance on files as storage is being widely avoided in this project. Instead, lmdb and postgresql are 
being utilized to store and manage in-game assets. 
* lmdb is integrated to provide a fast key/value storage system. 
* Many structures that were formerly implemented as linked lists in the CircleMUD base code have been revamped and implemented as std::vector. A full rewrite would be necessary to revamp all linked lists, so unfortunately (for now atleast) there are still linked lists in the source code.

# Cool Features 
* duktape js has been embedded to allow for powerful scripting features. The js scripts can access lmdb, making for a very powerful and robust scripting layer to assist building tasks.
* A quests system has been built, but is in the early stages -- it has a long way to go. 
* Sniping mechanics are a main focal point. The motivation behind this is to provide a sniping mechanic for players
who don't mind kiting around a mob and avoiding melee. 

# Plans
* See src/mods/todo for a list of things I want to do with the codebase
* Mob movement is going to be revamped to challenge sniping players with the threat of melee damage. Currently, mobact is responsible for a lot of the mob actions, and this hasn't changed much from the original CircleMUD base.
* Weapons that can level up with you (CyberAssault-inspired).

# Things I learned
I have reinvented the wheel in a few places merely because I was unaware
that the CircleMUD base already implemented certain features. The ACL 
config parser is a huge blemish (see src/mods/acl). The motivation for making
a Recursive Descent Parser for the task of ACL configurations was purely 
academic. I later realized that CircleMUD already had a rudimentary, and quite 
simple, ACL system already. This wasn't necessarily a waste of time on my part
since I learned how to create a Recursive Descent Parser, but for the potential
coders who inherit this codebase, it is likely not to be re-used (unfortunately).

# Compiling

You'll need g++-7 to be in your include path. Simply go to the src directory
and execute 'make'. Currently, compiling on Windows systems is not supported.

# Theme
The theme for this mud will be heavily dependant upon whomever I hire as builders.
I build most of the rooms and assets only because I have no builders currently.
I am motivated by twitchy PvP mechanics from the MUD Aliens Vs Predator. I am also
heavily influenced by CyberAssault (shout out to Grifter). Rainbow Six Siege is
also a heavy influence on the mechanics I pour into bnull-mud. This can be seen
in elements such as Thermite Breach Charges, Mute devices, and a lot of the weaponry
that is currently in sql form. 

# Roadmap
I don't have any concrete launch dates for bnull-mud, but I do know that I will
have everything ready before 2019. This README is currently written on the beginning
of January 2018. A full year of development should see many improvements and a stable
codebase to launch. 

# Dependencies
* libpqxx
* GSL
* lmdb
* duktape
* postgresql
* php7.0 (not really used right now)

# Special Thanks
* Grifter - CyberAssault Game Design
* Obfuscate - Of IRC (real mentocful eh? @Obfuscate)
* FearItself - Aliens Vs Predator MUD (coder/implementor). He may not remember me, but I remember the impact his MUD had on me, and for that I am grateful.
* Gahan - CyberAssault (coder/implementor)
* Xorg - CyberAssault (player)
* The folks at LMDB - For making an awesomely fast key/value store
* The geniuses behind the latest C++17 stuff. 
