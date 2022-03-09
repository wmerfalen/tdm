# Main goals
- is attracted/seeks out targets
- should have very loose mob roaming patterns
	- should roam anywhere

# When target is disoriented/inured/dead
	- should steal MP
	- should steal expensive items

# Reception by public
	- should try to steal from other NPC's
		- "good" aligned NPC's will kill on sight
		- neutral/evil NPC's ignore
			- except if thieves try to steal from them
	- should try to steal from PC's
		- if unsuccessful, should flee

# Behaviour tree pseudocode
```
bool should_steal_from(thief,victim){
	if(thief->dexterity() / victim->dexterity() > 0.9){
		return true;
	}
	if(thief->hp() > victim->hp()){
		return true;
	}
	return false;
}
bool theft_success(thief,victim){
	float good = thief->dexterity() / victim->dexterity();
	good -= victim->strength() / thief->strength();
}
sequence(){
	seek_victims(generic_thief gt){
		room_list = visible_player_list(gt->room())
		for(const auto & potential_victim : shuffle(room_list)){
			if(should_steal_from(gt,potential_victim)){
				gt->set_victim(potential_victim);
				return true;
			}
		}
		return false;
	},
	attempt_steal(generic_thief gt){
		/**
		 * if dexterity of thief is higher than target, 
		 * there should be a high chance of success.
		 * if target has high strength, then that should 
		 * lower the chance of success.
		 */
		auto chance = gt->player()->dexterity() / 
	}


```
