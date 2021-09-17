# GHOST class playtesting
Currently level 5

# Issues with target being out of range
There appears to be some sort of issue when sniping targets which should be within sniping range
I am using the PSG-1 to snipe targets in the fountain and euclid/market area. It does not appear
that this bug is consistent, and upon considering the dynamics at play, I think what is going
on is that the sensor grenade dynamics are affecting the distance to a target. The message I'm getting
is "target is out of range". There is some code that (when you use a sensor grenade) the target
becomes closer or further away even if they are physically closer/further. Sounds stupid, but this
may be the issue.


# Recommended dice count/sides for level 1
Recommended strength of melee weapons for level 1 is most likely 1d3 (the level 1 ghost class has 10hp)

# Recommended dice count/sides for level 5
The 'thug' mob is balanced well now that it uses a flimsy-knife.yml instead of the sickening-blade.yml.
The sickening blade could do 3d40 which often resulted in the thug mob completely obliterating the
level 1-5 ghost class (ghost level 5 only has 100hp).

# Recommended experience for level 1-5 mobs
The magic number that seems to work well is 250. However, it does seem that this can get fairly grindy.
In order to get to level 5, you'll need 20k experience


# 2021-09-16 CHANGES
- GHOST players at level 1 now have the following stats:
	- 66 HP
	- 173 mana points
	- 165 move points

- This is the better starting out values because the newbie level mobs should be a little tougher.
Setting the thug mob (vnum 112) to have less hp than 300-400 would mean that one hit kills them
from a stock PSG-1 

# 2021-09-17 CURRENT LEVEL PROGRESSION
`level 		HP,				Mana			Move	
1,				87,				158,			265,	
2,				174,			317,			531,	
3,				261,			475,			797,	
4,				348,			634,			1063,	
5,				435,			792,			1328,	
6,				522,			951,			1594,	
7,				609,			1109,			1860,	
8,				696,			1268,			2126,	
9,				783,			1426,			2392,	
10,				870,			1585,			2657,	
11,				957,			1743,			2923,	
12,				1044,			1902,			3189,	
13,				1131,			2060,			3455,	
14,				1218,			2219,			3720,	
15,				1305,			2377,			3986,	
16,				1392,			2536,			4252,	
17,				1479,			2695,			4518,	
18,				1566,			2853,			4784,	
19,				1653,			3012,			5049,	
20,				1740,			3170,			5315,	
21,				1827,			3329,			5581,	
22,				1914,			3487,			5847,	
23,				2001,			3646,			6112,	
24,				2088,			3804,			6378,	
25,				2175,			3963,			6644,	
26,				2262,			4121,			6910,	
27,				2349,			4280,			7176,	
28,				2436,			4438,			7441,	
29,				2523,			4597,			7707,	
30,				2610,			4755,			7973,	
31,				2697,			4914,			8239,	
