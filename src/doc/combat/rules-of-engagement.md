# Dictating preference of combat

# Ballistic mode
If you are wielding any weapon besides a sniper rifle and the target closes the distance
into same-room combat, you will continue to use your primary weapon in same room combat

# CQC mode
If you are wielding any weapon and the target closes the distance on you into same room
combat, then you will use hand to hand techniques.

# Auxiliary mode
If you are wielding any weapon that has an underbarrel attachment that can do same room damage,
then you will start using that underbarrel attachment on the same room opponent until
you run out of ammunition. You will then fall back to ballistic mode (if your primary is not
a sniper rifle), otherwise you will fall back to CQC mode.

# Secondary mode
If you are wielding any weapon and your target moves to same-room combat, you will start using
your secondary weapon.

# By default:
You start out in Ballistic mode.

# Setting your rules of engagement
Synytax:
```
set_rules_of_engagement <ballistic|cqc|auxiliary|secondary>
```
Example:
```
80H 145M 243V [7/7]> set_rules_of_engagement cqc
Rules of engagement set to CQC
[+] Done.

80H 145M 243V [7/7]>

```

# See your current rules of engagement
Simply use the 'skills' command
