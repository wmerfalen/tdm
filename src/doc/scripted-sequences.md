# Builder documentation: Scripted sequences

A scripted sequence is a set of events and actions that builders can create in order to immerse the player in interactive content.

An example:

Our player is named "Zephyr"

- An MP Shotgunner stands guard here.
- Zephyr enters room
- Zephyr attacks MP Shotgunner
- MP Shotgunner is at the brink of death...
	- Scripted sequence starts after MP Shotgunner health reaches below 40% or less
		- An MP Shotgunner stops engaging you in the fight
		- An exasperated MP Shotgunner stops fighting you!
		- An MP Shotgunner says, "Hold on! Wait! Okay, I'll stop fighting! Please!... don't HURT me! I'll give you the info you need"
		- An MP Shotgunner pulls out a USB stick the size of your thumb. 
		- An MP Shotgunner gives you a LX45 USB thumb drive.
		- An MP Shotgunner drops [primary weapon]
		- An MP Shotgunner drops [secondary weapon]
		- An MP Shotgunner stands here with his hands up.

An interactive example:
- Prerequisites:
	- HV Ammo - object vnum: 405
	- TRITON Energy scientist - mob vnum: 505
	- HV Ammo Contract - contract vnum: 3
	- Orange Key FOB - object vnum: 303
	- TRITON Energy Building Second Floor Ballistics Lab 1A - room vnum 405
	- TRITON Energy Building Second Floor Lab 1A Armor closet - room vnum 406

- A TRITON Energy scientist stands here in a white lab coat.
- Zephyr enters the room
- Zephyr executes "talk Scientist"
- Dialogue prompt appears to Zephyr
---
 A TRITON Energy scientist stands here in his white lab coat tending to some very interesting and extremely complex looking devices. Completely immersed in whatever hs is working on, he barely notices your presence. Wearily he looks up at you.

 A TRITON Energy scientist says, "Oh, I did not see you there.. you must be here for the high velocity magazines..."
 A TRITON Energy scientist reaches into his pocket and uses the FOB around his neck to unlock the door to a cold locker to the north.
 A TRITON Energy scientist walks north.
 - Zephyr walks north
 [on event: Player enters room] (guard: has contract VNUM 3)
 	A TRITON Energy scientist hands Zephyr a briefcase.
	[publish event: GOAL_FIND_ITEM, object_vnum: 405]


# Scripted sequence requirements
 ```
 struct scripted_step {
 	uint16_t wait_ticks; /** how many ticks to wait until executing this */
	std::string dialogue; /** text to display */
	mob_vnum mob;
	obj_vnum obj;
	room_vnum room;
	std::string yaml;
	std::string type;
	std::vector<std::string> interpret;
	uint8_t quantity;
 };
	struct scripted_sequence {
		uint32_t vnum;
		std::vector<scripted_step> sequence;
	};
	```
	- `void register_scripted_sequence(uint32_t sequence_vnum, std::vector<js_object> sequence);`
	- `void register_contract_step_callback(contract_vnum_t, task_type_t, target_type_t, uint32_t task_vnum, uint32_t sequence_vnum);`
	- javascript napi:
		
		- ```
		register_scripted_sequence(
				1,	/** virtual number of this sequence */
				[
					{
						wait_ticks: 3,
						dialogue: " triton energy scientist dialoge ... ",
						mob: 505,
						type: 'send_to_char'
					},
					{
						wait_ticks: 25,
						type: 'interpret',
						mob: 505,
						interpret: [
							'hold FOB',
							'unlock door north',
							'open door north',
							'north',
						],
					},
				]
		);
		register_contract_step_callback(
			3,							/** contract vnum */
			'GOAL_FIND',		/** task type */
			'TARGET_ROOM',	/** task target */
			405, 						/** target vnum (room vnum in this case) */
			1								/** scripted sequence vnum */
		);
		register_scripted_sequence(
				2,	/** virtual number of this sequence */
				[
					{
						wait_ticks: 3,
						dialogue: "You'll find that these magazines pack quite the punch... (more dialogue)",
						mob: 505,
						type: 'send_to_char'
					},
					{
						type: 'mob_gives_yaml',
						mob: 505,
						yaml: "magazine/extended-ar-mag.yml",
						quantity: 4
					},
					{
						wait_ticks: 3,
						dialogue: "Take these magazines and bring them to Doctor Land. He can reverse engineer them and make you more copies if you bring him the right materials.",
						mob: 505,
						type: 'send_to_char'
					},
					{
						wait_ticks: 10,
						emote: 'rubs his temples and forces a smile...',
						mob: 505,
						type: 'emote',
					},
					{
						wait_ticks: 3,
						dialogue: "I must get back to my work unfortunately. Do let me know how everything works out. Send my regards to Doctor Land, if you will.",
						mob: 505,
						type: 'send_to_char',
					},
					{
						room: 405,
						type: 'char_to_room',
					},
					{
						mob: 505,
						room: 405,
						type: 'char_to_room',
					},
					{
						room: 405,
						direction: 'north',
						type: 'seal_room',
					},
				]
		);
		/**
			* Register callback when player follows scientist into the room to the north
			*/
		register_contract_step_callback(
			3,							/** contract vnum */
			'GOAL_FIND',		/** task type */
			'TARGET_ROOM',	/** task target */
			406, 						/** target vnum (room vnum in this case) */
			2								/** scripted sequence vnum */
		);
		```

# related functionality
	- need to verify 'unlock door' can honor the FOB that the scientist is holding
	- support for the following "type"s:
		- mob_gives_yaml
		- send_to_char
		- emote
		- seal_room
		- char_to_room
		- interpret
	- on contract step completion, publish an event
	- add listeners to contract step events
	- add listeners that trigger scripted sequences on events
		- listeners can have "guards" which will only execute if conditions are met
	- 
