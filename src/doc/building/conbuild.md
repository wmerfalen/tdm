# Builder documentation: Building Contracts [updated: 05/14/2021]

Contract is the phrase we use for "quests". A contract is a quest with several steps to fulfill. 
Listed below are the commands and an extended walkthrough on how to build a contract.

# Creating a new contract
		- `conbuild new <vnum-of-contract>`
		- `conbuild new 405`
			- This will create a new contract identified by the virtual number 405. 

# Setting the title of the contract
	- `conbuild title <vnum> text goes here`
	- Example: 
		`conbuild title 405 Find and eliminate H.V.T. at shipping container distribution center.`

# Setting the description of the contract
	- `conbuild description <vnum> text goes here`
	- Example: 
		`conbuild description 405 Intelligence reports are giving us reports that...`

# Listing steps per contract
 	- `conbuild show-steps <vnum>`
	- Example: 
		`conbuild show-steps 405`

# Setting step data
	- `conbuild set-step-data <vnum> <step-id> <field> <value>`
	- Example: 
		`conbuild set-step-data 405 0 <field> <value>`
	- Fields:
		`s_task_type`: can be one of:
			- `GOAL_FIND`
			- `GOAL_DESTROY`
			- `GOAL_RETRIEVE`
			- `GOAL_QUOTA`
			- `GOAL_KILL`
			- `GOAL_GAIN_ENTRY`
			- `GOAL_PROTECT`
			- `GOAL_TALK_TO`
			- `GOAL_INSTALL`
			- `GOAL_CUSTOM_EVENT`
		`s_task_target`: can be one of:
			- `TARGET_MOB`
			- `TARGET_ITEM`
			- `TARGET_ROOM`
			- `TARGET_DOOR`
		`s_contract_vnum`: contract vnum
		`s_description`: text
		`s_mob_vnum`: 
		`s_room_vnum`: 
		`s_quota`: number to kill/find/etc
		`s_is_optional`: if step is optional
		`s_order`: for ordering the steps. lower values are ordered before higher values
		`s_object_yaml`: yaml path to an extended type in the `lib/objects` directory. supports deep objects
		`s_reward_1`: freeform text. interpretted by the contract. supports deep objects
		`s_reward_2`: "" ""
		`s_reward_3`: "" ""
		`s_reward_4`: "" ""
		`s_reward_5`: "" ""
		`s_reward_6`: "" ""
		`s_reward_7`: "" ""
		`s_reward_8`: "" ""
		`s_reward_9`: "" ""
		`s_reward_10`: "" ""
		`s_reward_money`: number of units to reward on completion
		`s_reward_xp`: number of exp points to reward

# Listing column names for step data
		- `conbuild columns`

# Saving steps
		- `conbuild save-step <vnum> <step-id>...[step-id-N]`
		- Example:
			- `conbuild save-step 405 0 2 3 5 6 1`
			- this will save multiple steps for the 405 contract	

# Deleting steps
		- `conbuild delete-step <vnum> <step-id>...[step-id-N]`
		- Same syntax as save except this just deletes those steps

# Using the `custom_event` task type
		- by using `GOAL_CUSTOM_EVENT` as a task type, you essentially allow for custom
		handling of any arbitrary event. This has to be handled in player-contract-instance.cpp
		for it to have any meaning. The default mode is for it to be a quota task. If the custom
		event is called enough times to fulfill the quota, the contract moves forward. 
		- Currently, the only custom event that is delegated to another .cpp file
		is the `CEV_HUMAN_REMAINS_FOUND` event for the Defiler contract.
			- see `mods/contract-defiler.hpp` and the accompanied `.cpp` file

# An extended example
	```
		conbuild new 1,
		conbuild 1 title Find the missing weapon cache.
		conbuild 1 description There is a weapons cache within the video store. The only problem is we dont know where. Figure out where the cache is at.
		conbuild new-step 1
		conbuild set-step-data 1 0 s_task_type GOAL_GAIN_ENTRY
		conbuild set-step-data 1 0 s_task_target TARGET_ROOM
		conbuild set-step-data 1 0 s_room_vnum 405
		conbuild set-step-data 1 0 s_reward_xp 250
		conbuild save-step 1 0
		conbuild new-step 1
		conbuild set-step-data 1 1 s_task_type GOAL_KILL
		conbuild set-step-data 1 1 s_task_target TARGET_MOB
		conbuild set-step-data 1 1 s_room_vnum 101
		conbuild set-step-data 1 1 s_reward_xp 850
		conbuild set-step-data 1 1 s_reward_money 5000
		conbuild set-step-data 1 1 s_reward_1 #yaml|explosive/frag-grenade.yml
		conbuild save-step 1 1
		conbuild show-steps 1
	```

# Using javascript objects
	- It's possible to create contracts by simply passing in javascript objects to the `build_contract(data);` javascript function
	- Example:
	```
		function build_weapons_cache_contract(){
			return build_contract([
				{
					vnum: 2,
					title: 'Find the missing weapon cache.',
					description: 'There is a weapons cache within the video store. The only problem is we dont know where. Figure out where the cache is at.',
					steps: [
						{
							task_type: 'GOAL_FIND',
							task_target: 'TARGET_ROOM',
							room_vnum: 145,
							reward_xp: 250,
							reward_money: 150,
							reward_1: '#yaml|explosive/frag-grenade.yml',
						},
						{
							task_type: 'GOAL_KILL',
							task_target: 'TARGET_MOB',
							mob_vnum: 501,	/** mp shotgunner */
							reward_xp: 250,
							reward_money: 5000,
							reward_1: '#yaml|explosive/frag-grenade.yml',
							reward_2:  '#deep|rifle/g36c.yml{barrel:silencer.yml,grip:hexon-grip.yml,magazine:extended-ar-mag.yml,muzzle:flash-hider.yml,sight:reflex-sight.yml,stock:titanium-stock.yml,strap:tactical-strap.yml,under_barrel:frag-underbarrel.yml}',
						},
					]
				}
			]);
		}
	```

