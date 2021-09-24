# Techniques

# Legend
	- *DEP_FREE* means:
		Depending on previous attack, this attack may automatically be executed at the same time as previous attack
	- *FLOWS_FROM* means: 
		If a previous attack is one of a select few techniques, this technique has the chance to be a *DEP_FREE* technique.
	- *STARTS_FLOW* means:
		If technique is the first in a combat order, it can cause the next techniques to flow together

## Available attributes
	skill_measurement_t str;
	skill_measurement_t str_add;
	skill_measurement_t intel;
	skill_measurement_t wis;
	skill_measurement_t dex;
	skill_measurement_t con;
	skill_measurement_t cha;
	skill_measurement_t electronics;
	skill_measurement_t armor;
	skill_measurement_t marksmanship;
	skill_measurement_t sniping;
	skill_measurement_t demolitions;
	skill_measurement_t chemistry;
	skill_measurement_t weapon_handling;
	skill_measurement_t strategy;
	skill_measurement_t medical;

# Upper body techniques
## Jab
	- Uses Dexterity
	- Higher constitution does 10% chance of stunning from technique

## Cross
	- Uses Strength 
	- Higher constitution does 20% chance of breaking nose causing bleed

## Uppercut L/R
	- Uses Str
	- Higher constitution does 20% chance of knockout

## Elbow L/R
	- *DEP_FREE*
	- Uses Dex
	- Higher Dex causes 20% chance to cut open target's face causing bleed


# Transitional techniques
## Clinch
	- Description: Grab hold and control the target's head and neck using both hands
	- Opens up techniques:
		- Elbow L/R
		- Knee to Head L/R
		- Knee to stomach L/R
		- Hip toss
		- Drop Seoinage
		- Inner leg trip L/R


# Judo throws

## Hip toss
	- Uses melee attribute
	- Higher strength attribute increases chance of success

## Drop Seoinage
	- Uses melee attribute
	- Higher strength attribute increases chance of success

## Inner leg trip L/R
	- Same as Single leg L/R except:
		- 10% chance for target to be knocked unconscious
	
## Ippon Seoinage
	- Same as Double leg except:
		- 30% chance for target to be knocked unconscious
		- 20% chance for target to break kneck causing POS_INCAP
		- 50% chance to throw target to another room

# Kicking techniques
## Teep kicks L/R
	- Uses Dex, Str
	- *DEP_FREE*
	- Speed attribute affects chance to double up
	- Higher melee attack may cause target to fall down

## Front kick L/R
	- Uses Dex, Str
	- *DEP_FREE*
	- *FLOWS_FROM* Jab (L), Cross, (R), Front Kick (R)
	- *STARTS_FLOW* Front Kick (R), Elbow (L), Uppercut(R)

## Oblique L/R
	- Uses constitution
	- Higher Strength causes 15% chance to break target leg

## Knee to Head L/R
	- Uses dex, strength, Con
	- *DEP_FREE*
	- *FLOWS_FROM* Clinch, Elbow, Knee
	- *STARTS_FLOW* Knee, Elbow, Hip Toss
	- *STARTS_FLOW* Knee, Inner leg trip

## Knee to Body L/R
	- Uses dex, str, con
	- *DEP_FREE*
	- *FLOWS_FROM* Clinch or Elbow
	- *STARTS_FLOW* Knee, Ippon Seoinage

# Disarming techniques
## Knife disarm
	- Difficult to land, but when landed, can remove weapon from attacker
	- Uses Speed, Dex, and Weapon Handling attribute
		- On slower enemies, has high chance of success
		- Uses the practice skill system in that the more you use it the better you get at it
	- Brainstorm: Might make this only a passive technique

## Pistol disarm
	- Uses Speed, Dex, and Weapon handling attribute
	- Brainstorm: Might make this only a passive technique



# Bladed weapon techniques
## Slash
	- Uses Melee attribute
	- Higher speed attribute decrease chance of target blocking the attack

## Stab
	- Uses Melee attribute
	- Higher speed attribute decrease chance of target blocking the attack


# Wrestling techniques
## Double leg
	- Uses dexterity
	- Higher strength increases chance of takedown
	- Higher strength increases ticks grounded

## Single leg L/R
	- Uses Dex
	- If target leg damage, 50% chance of takedown success
	- Higher speed attribute increases chance of takedown success
