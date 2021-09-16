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
