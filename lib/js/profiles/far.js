/**
 * ================================================================================
 * ================================================================================
 * || SECTION: 
 * ||         C O N S T A N T S 
 * ||
 * ================================================================================
 * ================================================================================
 * v-constants
 */
var scripted = {
	triton: {
		room: 143,
		room_north: 144,
		mob: 600,
		volunteer_mob: 601,
		volunteer_room: 147,
	},
	doctor_land: {
		mob: 603,
		room: 266, /** gear room */
	},
};
var crenshaw = {
	overpass_start: 'crenshaw-overpass-start',
	northern_shipping_area_entrance: 'crenshaw-northern-shipping-entrance',
};
var cofob = {
	west_atop_stairs: 'cofob-west-atop-stairs',
	secondfloor_center: 'cofob-secondfloor-center',
	center: 'cofobcenter',
	east_exit: 'eastexit',
	abbotstart: 'abbotstart',
	medhall: 'cofob-medhall',
	eng_commons: 'cofob-engineering-commons',
	gear_room: 'cofob-gear-room',
	armory_entrance: 'cofob-armory-entrance',
	mess_hall_upstairs: 'cofob-mess-hall-upstairs',
	armory_b2: 'cofob-armory-basement-2',
};
var waypoint = {
	rogue_mp: {
		mob: 407,
	},
	thief: {
		mob: 406,
	},
};
var grim_vnum = '101';
var contracts = {
	experimental_hv_mags: 1,
	weapons_cache_contract_vnum: 2,
	hv_mags_vnum: 3,
	gameplay: {
		gear_up: 4,
		ranged_attacks: 5,
		primary_secondary: 6,
		same_room_attacks: 7,
		weapon_stats_and_help: 8,
		breaching: 9,
		claymores: 10,
		grenades: 11,
		flash_bangs: 12,
		sensor_nades: 13,
		incendiary_nades: 14,
		smoke_nades: 15,
		night_vision: 16,
		thermal_vision: 17,
		cameras: 18,
		nvt_cameras: 19,
		drones: 20,
		hacking: 21,
	},
};
var mission_types = [
	"Recipe -> grab the various elements and bring them back to the contractee",
	"Eliminate -> kill target obj/mob",
	"Protect -> waves of enemies are on their way. Protect the obj/mob",
	"Explore -> the following area has a mob/obj. We need you to examine the area and report your findings",
	"Raid -> hostiles have barricade themselves in this building. We need you to break in and eliminate all threats",
	"Hostage -> a high value target is being held captive in this building. We need you to make contact",
].join(',');

/**
 * ================================================================================
 * ================================================================================
 * || SECTION: 
 * ||         O V E R P O W E R E D  G U N S
 * ||
 * ================================================================================
 * ================================================================================
 * v-overpowered-guns v-op-guns v-opguns
 */
var op_guns = {
	psg1: [
		{
			nick: 'dlance',
			name: 'DXC DemonLance XIV',
			theme: 'anti matter barrel. magazine adds radioactive+incendiary ammunition. the scope multiplies the base zoom level of the basic PSG-1 by 4 adding unprecedented range. while you cant attack in the same room with it, the underbarrel taser allows you to immobilize your enemies so that you can create distance and snipe from far off. the stock has a 3D printer interface that regenerates ammunition for your extended mag.',
			attachments: {
				barrel: 'anti-matter-barrel',	 /** created [2021-04-15] */
				magazine: 'depleted-uranium-magazine', /** created [2021-04-15] */
				sight: 'four-x-sight',	/** created [2021-04-15] */
				stock: 'printer-stock',	/** created [2021-04-15] */
				under_barrel: 'taser-underbarrel',	/** created [2021-04-15] */
			}
		},
		{
			nick: 'pathogen',
			name: 'TN-3 Pathogen',
			theme: 'highly corrosive long range sniper rifle with a close range radioactive shotgun underbarrel. ',
			attachments: {
				magazine: 'corrosive-sniper-magazine',
				under_barrel: 'fission-shotgun-underbarrel',
			}
		},
		{
			nick: 'hellfire',
			name: 'MK-X Hellfire VI',
			theme: 'extreme amount of incendiary damage but with a long reload time. the stock regenerates ammo',
			attachments: {
				barrel: 'hellfire-vi-barrel',
				magazine: 'hellfire-vi-magazine',
				sight: '4x-sniper-scope',
				stock: 'clover-regenerative-stock',
			}
		},
	],
	g36c: [
		{
			nick: 'chemlance',
			name: 'Chem-LANCE',
			theme: 'the caustic barrel guarantees your ammunition has high corrosive damage. the mag grants corrosive and explosive damage. the muzzle adds incendiary damage. visibility will never be a problem when you utilize the thermal or night vision modes of the sight. the frag underbarrel adds an element of surprise',
			attachments: {
				barrel: 'caustic-barrel',
				grip: 'stabilizer-grip',
				magazine: 'high-impact-corrosive-mag',
				muzzle: 'ignition-muzzle',
				sight: 'dual-thermal-nv-sight',
				stock: 'titanium-stock',
				under_barrel: 'frag-underbarrel',
			}
		},
	],
	mp5: [
		{
			nick: 'geiger',
			name: "Geiger's answer",
			theme: 'depleted uranium barrel poisons your ammunition with radioactive damage. the extended mag adds high velocity radioactive and explosive damage. the muzzle adds anti-matter damage. the control stock and grip reduce recoil and increase accuracy. the underbarrel is a corrosive spray variant',
			attachments: {
				barrel: 'depleted-uranium-barrel',
				grip: 'stabilizer-grip',
				magazine: 'hv-neutron-extended-mag',
				muzzle: 'anti-matter-muzzle',
				sight: 'reflex-sight',
				stock: 'control-stock',
				under_barrel: 'corrosive-spray-underbarrel',
			}
		},
		{
			nick: 'shrad',
			name: "Shrapnel decimator",
			theme: 'get the burst or automatic fire of an smg but with the added bonus of a barrel that causes explosive damage on top of your high velocity shrapnel ammunition. the heat dispatch muzzle adds incendiary damage. get the accuracy of the holographic sight which adds anti-matter damage. includes shotgun underbarrel with 6 shells',
			attachments: {
				barrel: 'dual-explosive-barrel',
				magazine: 'hv-shrapnel-discharge-mag',
				muzzle: 'heat-dispatch-muzzle',
				sight: 'holographic-anti-matter-sight',
				stock: 'control-stock',
				under_barrel: 'six-shell-shotgun-underbarrel',
			}
		},
	],
	sasg12: [
		{
			nick: 'alien',
			name: "Alienator",
			theme: 'smoke underbarrel, explosive regenerating shells, anti-matter barrel. the sight grants the user thermal and night vision for a powerful combination of blinding your enemies using the smoke grenade underbarrel and infiltrating the room unnoticed.',
			attachments: {
				magazine: '',
				stock: 'explosive-shell-printer-stock',
				strap: 'tactical-strap',
				under_barrel: 'smoke-underbarrel',
				sight: 'thermal-nv-sight',
			}
		},
	]
};
/**
 * ================================================================================
 * ================================================================================
 * || SECTION: 
 * ||         S C R I P T E D  S E Q U E N C E S 
 * ||
 * ================================================================================
 * ================================================================================
 * v-scripted-sequences
 */
function delete_scripted_sequence_by_vnum(vnum){
	var c = 'seqbuild ';
	vrun([
		c + 'delete ' + vnum
	]);
}
function delete_scripted_steps_by_sequence_vnum(vnum){
	var c = 'seqbuild ';
	vrun([
		c + 'delete-steps ' + vnum
	]);
}
function register_scripted_sequence(vnum,seq){
	var c = 'seqbuild ';
	vrun([
		c + 'new ' + vnum,
		c + 'save ' + vnum,
	]);
	var order = 0;
	for(var i =0; i < seq.length;i++){
		vrun([
			c + 'new-step ' + vnum
		]);
		for(var key in seq[i]){
			vrun([
				c + ' set-step-data ' + vnum + ' ' + i + ' s_' + key + ' ' + seq[i][key],
			]);
		}
		vrun([
			c + ' set-step-data ' + vnum + ' ' + i + [' s_order',order++].join(' '),
		]);
	}
	vrun([
		c + ' save-all-steps ' + vnum,
		c + ' load-steps ' + vnum,
		c + ' show-steps ' + vnum
	]);

}
function register_contract_step_callback(contract,type,target,vnum,seq_vnum){
	var c = 'seqbuild ';
	vrun([
		[c,'callback',contract,type,target,vnum,seq_vnum].join(' ')
	]);
}
function do_triton_scripted_content(){
	var greet_vnum  = 1;
	var give_mags_vnum = 2;
	var convnum = contracts.experimental_hv_mags; // contract vnum
	delete_scripted_sequence_by_vnum(greet_vnum);
	delete_scripted_steps_by_sequence_vnum(greet_vnum);
	delete_scripted_sequence_by_vnum(give_mags_vnum);
	delete_scripted_steps_by_sequence_vnum(give_mags_vnum);
	register_scripted_sequence(
			greet_vnum,	/** virtual number of this sequence */
			[
				{
					wait_ticks: 9,
					dialogue: '$n stands here in his white lab coat tending to some very interesting and extremely complex looking devices. Completely immersed in whatever hs is working on, he barely notices your presence. Wearily he looks up at you.',
					type: 'act',
					mob: scripted.triton.mob,
					room: scripted.triton.room,
				},
				{
					wait_ticks: 9,
					dialogue: 'Oh, I did not see you there.. ',
					type: 'dialogue',
					mob: scripted.triton.mob,
					room: scripted.triton.room,
				},
				{
					wait_ticks: 3,
					dialogue: 'You must be here for the high velocity magazines... Follow me...',
					type: 'dialogue',
					mob: scripted.triton.mob,
					room: scripted.triton.room,
				},
				{
					wait_ticks: 3,
					dialogue: '$n unlocks the door to the north',
					type: 'unlock_north',
					mob: scripted.triton.mob,
					room: scripted.triton.room,
				},
				{
					wait_ticks: 3,
					dialogue: '$n opens the door to the north',
					type: 'open_north',
					mob: scripted.triton.mob,
					room: scripted.triton.room,
				},
				{
					wait_ticks: 3,
					type: 'walk_north',
					mob: scripted.triton.mob,
					room: scripted.triton.room,
				}
			]
	);
	register_contract_step_callback(
		convnum,				/** contract vnum */
		'GOAL_FIND',		/** task type */
		'TARGET_ROOM',	/** task target */
		scripted.triton.room, 						/** target vnum (room vnum in this case) */
		greet_vnum								/** scripted sequence vnum */
	);
	register_scripted_sequence(
			give_mags_vnum,	/** virtual number of this sequence */
			[
				{
					wait_ticks: 3,
					dialogue: "You'll find that these magazines pack quite the punch...",
					mob: scripted.triton.mob,
					room: scripted.triton.room_north,
					type: 'say'
				},
				{
					wait_ticks: 3,
					type: 'mob_gives_yaml',
					mob: scripted.triton.mob,
					room: scripted.triton.room_north,
					yaml: 'attachment/hv-neutron-extended-mag.yml',
					quantity: 4
				},
				{
					wait_ticks: 3,
					dialogue: "Take these magazines and bring them to Doctor Land. He can reverse engineer them and make you more copies if you bring him the right materials.",
					mob: scripted.triton.mob,
					room: scripted.triton.room_north,
					type: 'say',
				},
				{
					wait_ticks: 10,
					dialogue: '$n rubs $s temples and forces a smile...',
					mob: scripted.triton.mob,
					room: scripted.triton.room_north,
					type: 'act',
				},
				{
					wait_ticks: 3,
					dialogue: '$n cleans $s glasses and sighs in exhaustion.',
					mob: scripted.triton.mob,
					room: scripted.triton.room_north,
					type: 'act',
				},
				{
					wait_ticks: 3,
					dialogue: "I must get back to my work unfortunately. Do let me know how everything works out. Send my regards to Doctor Land, if you will.",
					mob: scripted.triton.mob,
					room: scripted.triton.room_north,
					type: 'say',
				},
				{
					wait_ticks: 1,
					room: scripted.triton.room_north,
					type: 'seal_room_south',
				},
				{
					wait_ticks: 1,
					room: scripted.triton.room_north,
					type: 'force_south',
				},
			]
	);
	register_contract_step_callback(
		convnum,				/** contract vnum */
		'GOAL_FIND',		/** task type */
		'TARGET_ROOM',	/** task target */
		scripted.triton.room_north, 						/** target vnum (room vnum in this case) */
		give_mags_vnum								/** scripted sequence vnum */
	);
}
function do_seqbuild_test(){
	/**
	 * this demonstrates how to do use seqbuild
	 * but it is missing a few things.
	 */
	vrun([
		'seqbuild new 1',
		'contract_step_callback create 1 task target 4 1',
		'seqbuild new-step 1',
		'seqbuild set-step-data 1 0 s_mob 405',
		'seqbuild set-step-data 1 0 s_yaml rifle/psg1.yml',
		'seqbuild set-step-data 1 0 s_obj 22',
		'seqbuild set-step-data 1 0 s_quantity 33',
		'seqbuild set-step-data 1 0 s_order 1',
		'seqbuild set-step-data 1 0 s_dialogue heh, so whats up with all this stuff, hmm!?',
		'seqbuild set-step-data 1 0 s_type send_to_char',
		'seqbuild set-step-data 1 0 s_sequence_vnum 1',
		'seqbuild set-step-data 1 0 s_room 808',
		'seqbuild set-step-data 1 0 s_wait_ticks 10',
		'seqbuild new-step 1',
		'seqbuild set-step-data 1 1 s_mob 406',
		'seqbuild set-step-data 1 1 s_yaml rifle/p90.yml',
		'seqbuild set-step-data 1 1 s_obj 23',
		'seqbuild set-step-data 1 1 s_quantity 44',
		'seqbuild set-step-data 1 1 s_order 2',
		'seqbuild set-step-data 1 1 s_dialogue whats going on here?',
		'seqbuild set-step-data 1 1 s_type send_to_char',
		'seqbuild set-step-data 1 1 s_sequence_vnum 1',
		'seqbuild set-step-data 1 1 s_room 809',
		'seqbuild set-step-data 1 1 s_wait_ticks 15',
		'seqbuild new-step 1',
		'seqbuild set-step-data 1 2 s_mob 407',
		'seqbuild set-step-data 1 2 s_yaml rifle/mp5.yml',
		'seqbuild set-step-data 1 2 s_obj 24',
		'seqbuild set-step-data 1 2 s_quantity 55',
		'seqbuild set-step-data 1 2 s_order 3',
		'seqbuild set-step-data 1 2 s_dialogue here you go!',
		'seqbuild set-step-data 1 2 s_type send_to_char',
		'seqbuild set-step-data 1 2 s_sequence_vnum 1',
		'seqbuild set-step-data 1 2 s_room 810',
		'seqbuild set-step-data 1 2 s_wait_ticks 20',
		'seqbuild save-step 1 0 1 2',
		'seqbuild show-steps 1',
	]);
}

/**
 * ================================================================================
 * ================================================================================
 * ================================================================================
 * ================================================================================
 */















/**
 * ================================================================================
 * ================================================================================
 * || SECTION: 
 * ||         C O N T R A C T S
 * ||
 * ================================================================================
 * ================================================================================
 * v-contracts
 */
function build_hv_mag_contract(){
	return build_contract([
		{
			vnum: contracts.experimental_hv_mags,
			title: 'Experimental {yel}High Velocity{/yel} magazines.',
			description: [
				"Good evening, soldier. According to recent advances in our {yel}TRITON{/yel} sponsored laboratory a new kind of ",
				"ammunition is being built.\r\n\r\n",
				"The blueprint for this ammunition contains heavy amounts of radioactive isotopes from ",
				"recently spent Uranium. Find the scientist in the armory to east of {blu}COBALT{/blu} Main hallway. He will give you ",
				"the magazines he has made so far. Once you've acquired the magazines, I will give you further instructions over radio.\r\n",
				"{yel}Here are the contract instructions:{/yel}\r\n",
				"{yel}1) {grn}Find {blu}Doctor Land{/blu} {grn}in the Gear Room near the shooting range.{/grn}\r\n",
				"{yel}2) {grn}Find the TRITON Labs scientist. He will give you the magazines you need.\r\n",
				"{yel}3) {grn}Take the magazines to {blu}Doctor Land{/blu}.\r\n",
				"{yel}4) {grn}Talk to {blu}Doctor Land{/blu} on next steps...\r\n",
			].join(''),
			steps: [
				{
					task_type: 'GOAL_FIND',
					task_target: 'TARGET_ROOM',
					room_vnum: scripted.doctor_land.room,
					reward_xp: 850,
					reward_money: 1150,
					description: [
						"Find Doctor Land. Doctor Land is a ballistics expert and as such runs the majority of the new recruit shooting range\r\n",
						"activities. You can find him in the gear room.",
					].join(''),
				},
				{
					task_type: 'GOAL_FIND',
					task_target: 'TARGET_ROOM',
					room_vnum: scripted.triton.room,
					reward_xp: 850,
					reward_money: 1150,
					description: [
						"Locate the TRITON Labs research scientist. He can give you the magazines that Doctor Land requires.\r\n",
					].join(''),
				},
				{
					task_type: 'GOAL_FIND',
					task_target: 'TARGET_ROOM',
					room_vnum: scripted.triton.room_north,
					reward_xp: 850,
					reward_money: 5000,
					description: [
						"Follow the TRITON Labs research scientist north to get the magazines.\r\n"
					].join(''),
				},
				{
					task_type: 'GOAL_FIND',
					task_target: 'TARGET_MOB',
					mob_vnum: scripted.doctor_land.mob,
					reward_xp: 1150,
					reward_money: 1150,
					description: [
						"Go back to Doctor Land with the magazines.\r\n",
					].join(''),
				},
				{
					task_type: 'GOAL_GIVE',
					task_target: 'TARGET_MOB',
					mob_vnum: scripted.doctor_land.mob,
					reward_xp: 1150,
					reward_money: 1150,
					quota: 4,
					yaml: 'attachment/hv-neutron-extended-mag.yml',
					description: [
						"Give all 4 magazines to Doctor Land so that he can find a way to build more.\r\n",
					].join(''),
				},
			]
		}
	]);
}
function build_weapons_cache_contract(){
	return build_contract([
		{
			vnum: contracts.weapons_cache_contract_vnum,
			title: 'Find the missing weapon cache.',
			description: 'There is a weapons cache within the video store. The only problem is we dont know where. Figure out where the cache is at.',
			steps: [
				{
					task_type: 'GOAL_FIND',
					task_target: 'TARGET_ROOM',
					room_vnum: 143,
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
/**
 * Introduction to gameplay quests:
 * 1) gear up
 * 	- make your way to the training grounds to the north of here.
 * 	- grab the G36C from the rack
 * 	- grab ammunition for your rifle in the ammo crate
 * 	- make your way west to the shooting range
 * 2) ranged attacks
 *  - scan your surroundings
 * 	- use the snipe command to hit the targets to the south
 * 	- use the snipe command to hit the targets to the east
 * 	- use the snipe command to hit the targets to the west
 * 	- use the snipe command to hit the targets to the down
 * 	- use the snipe command to hit the targets to the up
 * 	- reload your rifle if you run out of ammunition
 * 3) primary and secondary
 * 	- grab a pistol from the rack
 * 	- grab ammunition for your pistol from the ammo crate
 * 	- make your way west to the shooting range
 * 4) ranged attacks
 * 	- go to the same room as the targets to the south
 * 	- hit the targets in the same room as you
 * 5) ranged weaponry help screen
 * 	- each weapon has a maximum and minimum range.
 * 	- each weapon has a set of stats
 * 	- to view this information, "examine" your weapon
 * 6) breaching doors
 * 	- there will come a time when you will need to get on the other side of a door
 * 	however the door is locked or sealed shut. In those cases you can use a breach
 * 	charge to blast your way in.
 * 	- grab a breach charge from the explosives crate
 * 	- <give instructions on how to breach>
 * 7) claymores
 * 	- sometimes you want to protect yourself from being flanked. an effective way
 * 	of doing this is to plant a claymore mine at the foot of a door.
 * 	- grab a claymore mine from the explosives crate
 * 	- plant the claymore to the south
 * 	- watch as corporal jennings triggers that claymore
 * 8) grenades
 * 	- grenades are an effective way of causing lots of damage to a remote target
 * 	- grab a couple grenades from the explosives crate
 * 	- arm and throw a grenade south to where corporal Jennings is at
 * 	- grenades almost always cause shrapnel. be careful
 * 9) flash bang grenades
 * 	- sometimes you will want to disorient your target. this may be an effective
 * 	way of surprising your target and thus allowing you to take them out while
 * 	they are still trying to figure out what's going on. 
 * 	- grab a flash bang grenade from the explosives crate
 * 	- throw the flashbang to the south where corporal Jennings is at
 * 	- notice the "disorient" messages
 * 	- snipe corporal jennings and notice the added bonus to your attack
 * 10) sensor grenades
 * 	- sensor grenades are an effective way of gathering intelligence about nearby targets.
 * 	- grab a sensor grenade from the explosives crate
 * 	- throw a sensor grenade to the south
 * 	- look at your surroundings and notice how your overhead map is updated
 * 11) incendiary grenades
 * 	- grenades are great but sometimes you want more damage. incendiary grenades
 * 	are a great way to damage the enemy and at the same time light the room on fire.
 * 	- grab an incendiary grenade from the explosives crate
 * 	- throw the grenade to the south where corporal Jennings is at
 * 	- go south. notice how the room is on fire!
 * 	- you will continue to take damage as long as the room is on fire
 * 12) smoke grenades
 * 13) night vision
 * 14) thermal vision
 * 15) cameras
 * 16) nv/thermal cameras
 * 17) drones
 * 18) hacking
 * 	- line up
 * 	- wires
 *
 */
/**
 * =======================================================
 * gameplay dynamics introduction: gear up
 * =======================================================
 */
var gameplay_intros = [
	{
		title: "Gear up",
		build: function(){
			var steps = [
				 ' make your way to the training grounds to the north of here.',
				 ' grab the G36C from the rack',
				 ' grab ammunition for your rifle in the ammo crate',
				 ' make your way west to the shooting range',
			];
			return build_contract([
				{
					vnum: contracts.gameplay.gear_up,
					title: 'Gear up, soldier!',
					description: 'Make your way to the {grn}Shooting Range Area Flacon{/grn} to the North of here. Take the Eastern Hallway and the shooting range will be to the {yel}WEST{/yel}.',
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
		}//end function
	},
	{
		title: "Ranged attacks",
		build: function(){
			var steps = [
			 'scan your surroundings',
			 'use the snipe command to hit the targets to the south',
			 'use the snipe command to hit the targets to the east',
			 'use the snipe command to hit the targets to the west',
			 'use the snipe command to hit the targets to the down',
			 'use the snipe command to hit the targets to the up',
			 'reload your rifle if you run out of ammunition',
			];
			return build_contract([
				{
					vnum: contracts.hv_mags_vnum,
					title: '',
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
		}//end function
	},
];//end gameplay_intros array

function build_contract(data){
	var order = 0;
	var run_me = [];
	for(var v in data){
		if(typeof data[v] === 'undefined' || typeof data[v].vnum === 'undefined'){
			send("Skipping record due to no vnum in contract data");
			continue;
		}
		var vnum = data[v].vnum;
		run_me.push('conbuild delete ' + vnum);
		run_me.push('conbuild new ' + vnum);
		run_me.push('conbuild title ' + vnum + ' ' + data[v].title);
		run_me.push('conbuild description ' + vnum + ' ' + data[v].description);
		order = 0;
		for(var i = 0; i < data[v].steps.length; i++){
			run_me.push('conbuild new-step ' + vnum);
			for(var k in data[v].steps[i]){
				run_me.push('conbuild set-step-data ' + vnum + ' ' + i + ' s_' + k + ' ' + data[v].steps[i][k]);
			}
			run_me.push('conbuild set-step-data ' + vnum + ' ' + i + ' s_order ' + order);
			run_me.push('conbuild save-step ' + vnum + ' ' + i);
			++order;
		}
		run_me.push('conbuild save ' + vnum);
		//run_me.push('conbuild load-steps ' + vnum);
	}
	//for(var i=0; i < run_me.length;i++){
	//	send(run_me[i]);
	//}
	run(run_me);
}

/**
 * ================================================================================
 * ================================================================================
 * ================================================================================
 * ================================================================================
 */



















function dlance(){
	ira('dlance');
}
function giveme_demonlance(){
	run([
		'instantiate_rifle_attachment ' + instantiate_op('psg1', 'DXC DemonLance XIV')
	]);
}
function list_op(){
	var page = [];
	for(var base in op_guns){
		page.push('base: "' + base + '"');
		for(var i =0; i < op_guns[base].length;i++){
			page.push('name: "' + op_guns[base][i].name + ' (ira("' + op_guns[base][i].nick + '"))');
		}
	}
	send(page.join("\r\n"));
}
function instantiate_op(base,name){
	for(var key in op_guns[base]){
		if(op_guns[base][key].name === name){
			var str = base + '.yml{';
			for(var nkey in op_guns[base][key].attachments){
				str += nkey + ':' + op_guns[base][key].attachments[nkey] + '.yml';
				str += ',';
			}
			str += '}';
			str = str.replace(/,\}$/,'}');
			return str;
		}
	}
	send("Okay, going to build string");
}
function ira(nick){
	for(var base in op_guns){
		for(var i=0; i < op_guns[base].length;i++){
			if(op_guns[base][i].nick === nick){
				run([
					'instantiate_rifle_attachment ' + instantiate_op(base,op_guns[base][i].name)
				]);
				send("instantiated one for you");
				return;
			}
		}
	}
	send('couldnt find anything by that name');
}

/**
 * ================================================================================
 * ================================================================================
 * ================================================================================
 * ================================================================================
 */

















/**
 * ================================================================================
 * ================================================================================
 * || SECTION: 
 * ||         F A C T O R Y  F U N C T I O N S
 * ||
 * ================================================================================
 * ================================================================================
 * v-factory
 */
function do_giveme_g36c(){
	cmd('yaml_import rifle g36c.yml');
}

/**
 * ================================================================================
 * ================================================================================
 * ================================================================================
 * ================================================================================
 */
















/**
 * ================================================================================
 * ================================================================================
 * || SECTION: 
 * ||         T E S T I N G  G R E N A D E S  A N D  G A D G E T S
 * ||
 * ================================================================================
 * ================================================================================
 * v-grenade-tests v-test-grenades v-gadgets
 */
function do_claymore_test(){
	run([
		'n','n','n',
		'w','w',
		'yaml_import explosive claymore-mine.yml',
		'install claymore east'
	]);
}

function do_flash_nade(){
cmd('giveme_flash');
cmd('hold grenade');
cmd('throw north 1');
}

function do_sensor_nade(){
cmd('giveme_sensor');
cmd('hold grenade');
cmd('throw north 1');
}
function do_camera(){
	cmd('yaml_import gadget camera.yml');
	cmd('get pi');
	cmd('hold pi');
	cmd('install pi north');
}
function do_nv_camera(){
	cmd('yaml_import gadget night-vision-camera.yml');
	cmd('get Night');
	cmd('hold Night');
	cmd('install Night north');
}
function do_thermal_camera(){
	cmd('yaml_import gadget thermal-camera.yml');
	//cmd('get Thermal');
	cmd('hold Thermal');
	cmd('install Thermal north');
}
function do_breach_charge(){
	cmd('yaml_import explosive breach-charge.yml');
	cmd('get breach');
	cmd('hold breach');
	cmd('breach north');
}
function do_thermite_breach_charge(){
	cmd('yaml_import explosive thermite-charge.yml');
	cmd('get Thermite');
	cmd('hold Thermite');
	cmd('thermite north');
}
function frag(){
	for(var i=0;i < 10;i++){
		cmd("giveme_frag");
	}
	cmd("hold frag");
	cmd("throw north 1");
}

/**
 * ================================================================================
 * ================================================================================
 * ================================================================================
 * ================================================================================
 */



















function dirhl(dir){
		return ['{wht}',dir[0],'{/wht}{grn}',dir.substr(1),'{/grn}'].join('');
}
function td(index){
		cmd('rbuild title ' + init.phases[index].title);
		cmd('rbuild description ' + init.phases[index].description);
}
function interactive_item(item){
	return '{blu}' + item + '{/blu}';
}
function action(a){
	return '{yel}' + a + '{/yel}';
}




function yaml_list(){
	return ['rifle','explosive','drone','attachment','armor','gadget'];
}

function yaml_example(){
	var t = yaml_list();
	for(var i=0;i < t.length;i++){
		cmd('yaml_example ' + t[i]);
	}
}

function yaml_import(){
	var t = yaml_list();
	for(var i=0; i < t.length;i++){
		send(t[i]);
		cmd('yaml_import ' + t[i] + ' ' + t[i] + '.yml');
	}
}


//yaml_example();
//yaml_import();
//do_sensor_nade();
//do_camera();


var funcs = {
	always: function(){
		cmd("+impl");
		cmd("+god");
		cmd("+build");
	},
	fixtures: {
		snipe_fixture: function(){
			cmd("mbuild instantiate 0");
			cmd("obuild instantiate 2");
			cmd("get xm");
			cmd("builder");
			cmd("yaml_example rifle");
			cmd("yaml_import ls");
			cmd("yaml_import rifle rifle.yml");
		},
		g36c: function(index){
			var vnum = 14;
			cmd(["obuild attr ",index, " weapon_type:add ASSAULT_RIFLE"].join(""));
			cmd(["obuild flag ",index, " wear_flags TAKE WIELD"].join(""));
			cmd(["obuild flag ",index, " type_flags WEAPON"].join(""));
			cmd(["obuild flag ",index, " value_0 0"].join(""));				//unused
			cmd(["obuild flag ",index, " value_1 ",25].join(""));		//Number of damage dice
			cmd(["obuild flag ",index, " value_2 ",5].join(""));		//size of damage dice
			cmd(["obuild flag ",index, " value_3 ",7].join(""));		//size of damage dice
			cmd(["obuild attr ",index, " item_number ",vnum].join(""));
			cmd(["obuild attr ",index, " name \"G36C Assault Rifle\""].join(""));
			cmd(["obuild attr ",index, " description \"A G36C Assault Rifle lies here.\""].join(""));
			cmd(["obuild attr ",index, " short_description \"A G36C Assault Rifle\""].join(""));
			cmd(["obuild attr ",index, " action_description \"A G36C Assault Rifle\""].join(""));
			//cmd(["obuild ex ",index, " create 1"].join(""));
			//cmd(["obuild ex ",index, " set 0 g36c \"A G36C\""].join(""));
			cmd('obuild obj_file ',index, ' "g36c.yml"');
		}
	},
	content: {
		vegas_junkyard: function(){
			cmd('rbuild_sandbox pave on "vegas_junkyard"');
			var narrow_hallway = {
				desc: [
					"The hallway in front of you aches with the relentlessness ",
					"of criminal empire. This area has doubled as an avenue for ",
					"criminal activity thanks to the property owner's ability ",
					"to look the other way for the right price. ",
					"There has been an insane amount of foot traffic here as ",
					"evidenced by the worn out condition of the ground. "
				],
				textures: [
					'CEMENT',
					'INSIDE',
					'TUNNEL'
				],
				flammable_chance: 0, /** out of 100 */
				channel_explosions: [
					{dir: 'n', amt: 50.0}, /** out of 100 */
					{dir: 's', amt: 50.0}, /** out of 100 */
				],
				wall_durability: 98, /** out of 100 */
				content_tags: [
					'contraband', 'tunnel','smuggling'
				],
				npc_weapon_drop: {
					rarity: ['common'],
					types: ['PISTOL'],
				}
			};
			var up_the_stairs = {
				desc: [
					"A worn out metal ladder is here leading up to the ground floor of this building. ",
					"Someone has spray painted graffiti all over the wall here, giving you the impression ",
					"that cartel members loiter here."
				],
				textures: [
					'CEMENT',
					'INSIDE',
					'LADDER',
				],
				flammable_chance: 0, /** out of 100 */
				channel_explosions: [
					{dir: 'u', amt: 50.0}, /** out of 100 */
					{dir: 'd', amt: 50.0}, /** out of 100 */
				],
				wall_durability: 98, /** out of 100 */
				content_tags: [
					'contraband', 'tunnel','smuggling',
					'graffiti'
				],
				mutable_objects: [
					{
						name: 'metal-ladder',
						mutations:[
							{type: 'heat',causes: 'burn-hands',counter: 'gloves'}
						]
					}
				],
				npc_wander: [
					{level: [1,20],group: 'cartel'}
				]
			};

			var ground_level_fork = {
				desc: [
					"The hallway in front of you stretches to the east and the west. ",
					"A ladder is here that leads down to a poorly lit tunnel. ",
					"To the south, you see the entrance to the junkyard's main area."
				].join(""),
				title: "A fork in the junkyard hallway",
				textures: [
					'CEMENT',
					'INSIDE'
				],
				flammable_chance: 0, /** out of 100 */
				channel_explosions: [
					{dir: 'w', amt: 50.0}, /** out of 100 */
					{dir: 'e', amt: 50.0}, /** out of 100 */
				],
				wall_durability: 98, /** out of 100 */
				content_tags: [
					'contraband', 'tunnel','smuggling'
				],
				npc_weapon_drop: {
					rarity: ['common'],
					types: ['PISTOL'],
				}
			};
			var ground_level_east = {
				desc: [
					"The hallway stretches to the east and the west. ",
					"To the west you can see large glass windows that peer ",
					"into the main area of the junkyard. "
				].join(""),
				textures: [
					'CEMENT',
					'INSIDE'
				],
				flammable_chance: 0, /** out of 100 */
				channel_explosions: [
					{dir: 'w', amt: 50.0}, /** out of 100 */
					{dir: 'e', amt: 50.0}, /** out of 100 */
				],
				wall_durability: 98, /** out of 100 */
				content_tags: [
					'contraband', 'tunnel','smuggling'
				],
				npc_weapon_drop: {
					rarity: ['common'],
					types: ['PISTOL'],
				}
			};
			var d = [
				narrow_hallway,
				up_the_stairs,
				ground_level_fork,
				ground_level_east,
				ground_level_east_exit,
				ground_level_east_outside
			];

			var cmds =[
				{dir:'n', d:0},
				{dir:'n', d:0},
				{dir:'n', d:0},
				{dir:'e', d:0},
				{dir:'e', d:0},
				{dir:'n', d:0},
				{dir:'n', d:0},
				{dir:'n', d:0},
				{dir:'e', d:0},
				{dir:'n', d:0},
				{dir:'w', d:0},
				{dir:'n', d:0},
				{dir:'n', d:0},
				{dir:'u', d:1},
				{dir:'u', d:1},
				{dir:'s', d:2},
				{dir:'e', d:3},
				{dir:'e', d:3},
				{dir:'n', d:3},
				{dir:'n', d:4, door: {dir: 'east', def: 'closed'}},
				{dir:'e', d:5},
				{dir:'w', d:2},
				{dir:'s', d:2},
				{dir:'s', d:2},
				{dir:'w', d:2},
				{dir:'w', d:2}
			];

			cmd('rbuild_sandbox pave off');
			cmd('rbuild_sandbox save 0');
		},
		smg_shop: function(index){
			var desc = ['If there ever was a mecca of guns and ammo, Grim\'s place would ',
				'definitely be a close contender right there behind the U.S. military itself. ',
				'An array of {grn}Sub Machine Guns{/grn} adorn both walls to the east and west ',
				'walls. Behind the counter you hear various automotive mechanic tools. Strange.. ',
				'this place definitely doesn\'t look like a mechanics shop. A worn out black leather ',
				'chair with it\'s stuffing bulging out is here rocking back n forth despite nobody ',
				'actually sitting in it.'
			];
			var cmds = ['sbuild attr ' + index + ' title "Grim\'s Guns and Ammo"',
				'sbuild attr ' + index + ' description "' + desc.join('') + '"',
				'sbuild attr ' + index + ' no_such_item1 "We don\'t carry that sort of thing here."',
				'sbuild attr ' + index + ' no_such_item2 "Look, we don\'t carry that..."',
				'sbuild attr ' + index + ' missing_cash1 "No money, no product. Simple as that."',
				'sbuild attr ' + index + ' missing_cash2 "The precursor to buying guns is that you have to have money upfront..."',
				'sbuild attr ' + index + ' do_not_buy "I don\'t work with those types of items."',
				'sbuild attr ' + index + ' message_buy "You got it. Here you go!"',
				'sbuild attr ' + index + ' message_sell "Nice. Maybe I can find some poor schmuck to sell this to..."',
			];
			for(var i=0; i < cmds.length;i++){
				cmd(cmds[i]);
			}
		},
		grim_shopkeeper: function(index){
			cmd(["mbuild attr ",index," virt ",grim_vnum].join(""));
			cmd(["mbuild attr ",index," mana ",15].join(""));
			cmd(["mbuild attr ",index," max_mana ",15].join(""));
			cmd(["mbuild attr ",index," hit ",70].join(""));
			cmd(["mbuild attr ",index," max_hit ",70].join(""));
			cmd(["mbuild attr ",index," move ",30].join(""));
			cmd(["mbuild attr ",index," max_move ",30].join(""));
			cmd(["mbuild attr ",index," damroll ",5].join(""));
			cmd(["mbuild attr ",index," gold ",25000].join(""));
			cmd(["mbuild attr ",index," level ",35].join(""));
			cmd(["mbuild attr ",index," weight ",10].join(""));
			cmd(["mbuild attr ",index," height ",4].join(""));
			cmd(["mbuild attr ",index," strength ",2].join(""));
			cmd(["mbuild attr ",index," strength_add ",2].join(""));
			cmd(["mbuild attr ",index," intelligence ",2].join(""));
			cmd(["mbuild attr ",index," dexterity ",2].join(""));
			cmd(["mbuild attr ",index," constitution ",2].join(""));
			cmd(["mbuild attr ",index," charisma ",2].join(""));
			cmd(["mbuild attr ",index," damnodice ",2].join(""));
			cmd(["mbuild attr ",index," damsizedice ",5].join(""));
			cmd(["mbuild attr ",index," name \"dallas gibson gib g grim gibbs\" "].join(""));
			cmd(["mbuild attr ",index," sex MALE "].join(""));
			cmd(["mbuild attr ",index," action SENTINEL ISNPC"].join(""));
			var short_desc = [
				'{gld}Dallas {red}\'Grim\'{/red} {gld}Gibson{/gld}'
			].join('');
			cmd(["mbuild attr ",index," short_description \"", short_desc, "\" "].join(""));
			var long_desc = [
				'{gld}Dallas {red}\'Grim\'{/red} {gld}Gibson the shopkeeper{/gld}'
			].join('');
			var description = [
				'Short of stature and sharp as an eagle, this man may look like ',
				'an under-powered elderly old man, but underneath that meek exterior ',
				'lies a fierce beast.'
			].join('');
			cmd(["mbuild attr ",index," long_description \"", long_desc, "\" "].join(""));
			cmd(["mbuild attr ",index," description \"", description, "\" "].join(""));
			cmd(["mbuild save ",index].join(""));
			return 0; 
		}
	},

	tests: {
		'triad_range_tests': function (){
			cmd('mbuild instantiate 0');
			cmd('l');
			cmd('obuild instantiate 1');
			cmd('get xm');
			cmd('wield xm');
			cmd('north');
			set_points('far','hp',999);
			cmd('snipe c s');
			cmd('snipe c s');
			cmd('s');
			cmd('s');
			cmd('k chef');
			cmd('one_punch');
		},
		'sensor_grenade_tests': function(){
			send('this test is incomplete... returning early');return;
			cmd('mbuild instantiate 0');
			cmd('l');
			do_nv_camera();
		}
	}
};
function th1(){
	//
	send('stub');
}
function do_fire(){
	cmd('room_fire on KINDLING');
}
function do_fueled_fire(){
	cmd('north');cmd('north');
	cmd('rbuild texture:add DRY GRASS');
	cmd('room_fire on');
}


/**
 * ================================================================================
 * ================================================================================
 * || SECTION: 
 * ||         R U N N I N G  C O M M A N D S
 * ||
 * ================================================================================
 * ================================================================================
 * v-run
 */
function repeat(count,commands){
	return {times: count, cmd: commands};
}
function run(commands){
	for(var i=0;i < commands.length;i++){
		if(commands[i] === false){
			return;
		}
		if(typeof commands[i] === 'object'){
			for(var k=0; k < commands[i].times; k++){
				for(var m=0; m < commands[i].cmd.length;m++){
					send('running: "' + commands[i].cmd[m] + '"');
					cmd(commands[i].cmd[m]);
				}
			}
			continue;
		}
		send('running: "' + commands[i] + '"');
		cmd(commands[i]);
	}
}
function vrun(commands){
	for(var i=0;i < commands.length;i++){
		send('running: {grn}' + commands[i] + '{/grn}');
		send('[--- command start ---]');
		send('[=====================]');
		cmd(commands[i]);
		send('[=====================]');
		send('[--- command end   ---]');
	}
}

/**
 * ================================================================================
 * ================================================================================
 * ================================================================================
 * ================================================================================
 */

















/**
 * ================================================================================
 * ================================================================================
 * || SECTION: 
 * ||         R E V I V E  A N D  R E P O R T I N G
 * ||
 * ================================================================================
 * ================================================================================
 * v-revive
 */
function do_revive_test(){
	run(['mbuild instantiate 0', 'set_npc_position 3 INCAP', 'revive chef' ]);
}
function do_send_report(){
	run(['mbuild instantiate 0', 'send_report']);
}

/**
 * ================================================================================
 * ================================================================================
 * ================================================================================
 * ================================================================================
 */


















/**
 * ================================================================================
 * ================================================================================
 * || SECTION: 
 * ||         M I N I G U N N E R S
 * ||
 * ================================================================================
 * ================================================================================
 * v-minigunners v-mg
 */
function do_mini_gunner(){
	run([
		'mbuild instantiate 0', 
		'send_report'
	]);
}
function do_load_test_mini_gunner(){
	for(var i=0; i < 5; i++){
		cmd('mbuild instantiate 0');
	}
}
function do_distributed_load_test_mini_gunner(){
	var inst = function(times){
		for(var i=0; i < times; i++){
			cmd('mbuild instantiate 0');
		}
	};
	run(['n','n','n']);
	inst(3);
	run(['e','e','e','n']);
	inst(2);
	run(['n','n','n','e','n']);
	inst(2);
}
function do_mini_gunner_special(index,room){
	var mgs = 'mbuild mini-gunner-sentinel ';
	run(['mbuild extended-type ' + index + ' MINI_GUNNER_SENTINEL',
		mgs + index + ' face-direction NORTH',
		mgs + index + ' room-vnum ' + room,
		mgs + index + ' save',
		'mbuild save ' + index
	]);
}

/**
 * ================================================================================
 * ================================================================================
 * ================================================================================
 * ================================================================================
 */

















function do_chat(){
	run(['chat lol']);
}
function do_get_noticed(){
	run(['n','n','e','e','w','w','s','s']);
}

function do_value_system_sanity_check(value){
	value_save('SANITY_CHECK',value);
	value_load('SANITY_CHECK');
	return value_sanity_check();
}


var escape_these = {
	mbuild: ['name',
		'short_description',
		'long_description',
		'description',
	],
	rbuild: [

	]
};

function conditional_escape(acmd,key,value){
	if(escape_these[acmd] && escape_these[acmd].indexOf(key) > -1){
		return "\"" + value + "\"";
	}
	return value;
}

/**
 * ================================================================================
 * ================================================================================
 * ================================================================================
 * ================================================================================
 */
































/**
 * ================================================================================
 * ================================================================================
 * || SECTION: 
 * ||         A U T O - F E E D  F U N C T I O N S
 * ||
 * ================================================================================
 * ================================================================================
 * v-auto-feed v-feed
 */
function clear_zone_data_entries(){
	cmd(['zbuild remove-all-zone-data-from-db']);
}
/** -!- Poster child for: create-or-update mob by vnum -!- */
function auto_feed_mob(obj){
	var index = exec('mbuild exists ' + obj.mbuild.attr.virt);
	if(index === '!'){
		index = exec('mbuild new');
	}
	for(var key in obj.mbuild.attr){
		cmd(["mbuild attr ",index," ",key,' ',conditional_escape("mbuild",key,obj.mbuild.attr[key])].join(''));
	}
	if(typeof obj.mbuild['extended-type'] !== 'undefined'){
		cmd(['mbuild extended-type ',index,' ',obj.mbuild['extended-type']].join(''));
	}
	if(typeof obj['meqbuild'] !== 'undefined'){
		cmd(['meqbuild delete ',obj.meqbuild.virtual_number].join(''));
		cmd(['meqbuild new ',obj.meqbuild.name,' ',obj.meqbuild.virtual_number].join(''));
		for(var key in obj.meqbuild.positions){
			cmd(["meqbuild set ",obj.meqbuild.virtual_number,' ',key,' ',obj.meqbuild.positions[key]].join(''));
		}
		cmd(['meqbuild save ',obj.meqbuild.virtual_number].join(''));
		cmd(['meqbuild map-assign ',obj.mbuild.attr.virt,' ',obj.meqbuild.virtual_number].join(''));
	}
	cmd(["mbuild save ",index].join(""));
	cmd('meqbuild reload-all');
	if(typeof obj['zbuild'] !== 'undefined'){
		cmd([
			'zbuild ',obj.zbuild.command,' ',
			obj.zbuild.zone_id,' ',
			obj.zbuild.mob_vnum,' ',
			obj.zbuild.room_vnum,' ',
			obj.zbuild.max,' ',
			obj.zbuild.if_flag,' ',
		].join(''));
	}
}
function auto_feed_object(obj){
	var index = exec('obuild exists ' + obj.obuild.attr.item_number);
	if(index === '!'){
		index = exec('obuild new ' + obj.obuild.attr.item_number);
	}
	for(var key in obj.mbuild.attr){
		cmd(["obuild attr ",index," ",key,' ',conditional_escape("obuild",key,obj.obuild.attr[key])].join(''));
	}
	cmd(["obuild save ",index].join(""));
}
/** -!- Poster child for: create-or-update mob by vnum by passing in object with several mob definitions -!- */
function auto_feed_mob_list(list){
	for(var name in list){
		auto_feed_mob(list[name]);
	}
}
function rbuild_pave_continue(commands){
	run(['rbuild pave continue']);
	for(var i = 0; i < commands.length;i++){
		if(commands[i].rbuild){
			for(var key in commands[i].rbuild){
				run([
					['rbuild ',key,' ', conditional_escape("rbuild",key,commands[i].rbuild[key])].join('')
				]);
			}
			continue;
		}
		run([commands[i]]);
	}
	var index = exec('rbuild pave off');
	send('To save the paved area, type: "rbuild save-paved ' + index + '"');
}

/**
 * ================================================================================
 * ================================================================================
 * ================================================================================
 * ================================================================================
 */















/**
 * ================================================================================
 * ================================================================================
 * || SECTION: 
 * ||         U . S .  V I D E O
 * ||					C O N T E N T
 * ||
 * ================================================================================
 * ================================================================================
 * v-us-video v-usvideo v-content
 */
function do_us_video(){
	/** -!- Poster child for: creating mobs programmatically -!- */
	var mobs = {
		bum: {
			mbuild: {
				attr: {
					virt: 401,
					mana: 25,
					max_mana: 25,
					hit: 580,
					max_hit: 580,
					move: 140,
					max_move: 240,
					damroll: 15,
					gold: 5000,
					level: 10,
					weight: 10,
					height: 4,
					strength: 15,
					strength_add: 13,
					intelligence: 12,
					dexterity: 14,
					constitution: 18,
					charisma: 1,
					damnodice: 14,
					damsizedice: 16,
					name: "A weak and twitchy homeless man",
					sex: "MALE",
					action: "ISNPC",
					short_description: 'A weak and twitchy homeless man.',
					long_description: 'A weak and twitchy homeless man.',
					description: 'A tired yet wide awake looking bum reaches his arm ' +
						'out to you for some spare change. '+
						'You have no idea what he will spend it on.'
				} /** end attr */
			} /** end mbuild */
		},
		hooker: {
			mbuild: {
				attr: {
					virt: 402,
					mana: 25,
					max_mana: 25,
					hit: 580,
					max_hit: 580,
					move: 140,
					max_move: 240,
					damroll: 15,
					gold: 5000,
					level: 10,
					weight: 10,
					height: 4,
					strength: 15,
					strength_add: 13,
					intelligence: 12,
					dexterity: 14,
					constitution: 18,
					charisma: 1,
					damnodice: 14,
					damsizedice: 16,
					name: "A diseased sex worker",
					sex: "FEMALE",
					action: "ISNPC",
					short_description: "A diseased sex worker",
					long_description: "A diseased sex worker",
					description: 'A sex worker stands here offering you cheap services.'
				} /** end attr */
			} /** end mbuild */
		},
		clerk: {
			mbuild: {
				attr: {
					virt: 403,
					mana: 25,
					max_mana: 25,
					hit: 580,
					max_hit: 580,
					move: 140,
					max_move: 240,
					damroll: 15,
					gold: 5000,
					level: 10,
					weight: 10,
					height: 4,
					strength: 15,
					strength_add: 13,
					intelligence: 12,
					dexterity: 14,
					constitution: 18,
					charisma: 1,
					damnodice: 14,
					damsizedice: 16,
					name: "A store clerk",
					sex: "MALE",
					action: "ISNPC",
					short_description: "A video rental clerk",
					long_description: "A video rental clerk",
					description: 'Bored, underpaid, and stoned.'
				} /** end attr */
			} /** end mbuild */
		},
		creepycustomer: {
			mbuild: {
				attr: {
					virt: 404,
					mana: 25,
					max_mana: 25,
					hit: 580,
					max_hit: 580,
					move: 140,
					max_move: 240,
					damroll: 15,
					gold: 5000,
					level: 10,
					weight: 10,
					height: 4,
					strength: 15,
					strength_add: 13,
					intelligence: 12,
					dexterity: 14,
					constitution: 18,
					charisma: 1,
					damnodice: 14,
					damsizedice: 16,
					name: "A store clerk",
					sex: "MALE",
					action: "ISNPC",
					short_description: "A video rental clerk",
					long_description: "A video rental clerk",
					description: 'Bored, underpaid, and stoned.'
				} /** end attr */
			}
		},
		lowly_security_guard: {
			mbuild: {
				attr: {
					virt: 405,
					mana: 45,
					max_mana: 45,
					hit: 880,
					max_hit: 880,
					move: 140,
					max_move: 240,
					damroll: 20,
					gold: 5000,
					level: 15,
					weight: 20,
					height: 5,
					strength: 25,
					strength_add: 23,
					intelligence: 22,
					dexterity: 24,
					constitution: 28,
					charisma: 10,
					damnodice: 24,
					damsizedice: 26,
					name: "A lowly security guard",
					sex: "MALE",
					action: "ISNPC HELPER AGGR_GOOD",
					short_description: "A lowly security guard",
					long_description: "A lowly security guard",
					description: 'A somewhat physically fit security guard. He looks armed.',
				}, /** end attr */
				'extended-type': 'LOWLY_SECURITY',
			},
				meqbuild: {
					name: 'lowlysec',
					virtual_number: 1,
					positions: {
						hold: 'gadget/cbradio.yml',
						wield: 'rifle/hk45.yml',
						head: 'armor/dsf-hat.yml',
						goggles: 'armor/sunglasses.yml',
						hands: 'armor/spiked-gloves.yml',
						body: 'armor/dsf-shirt.yml',
						legs: 'armor/black-dickies.yml',
						feet: 'armor/basic-boots.yml'
					}
				}
		},
	};
	auto_feed_mob_list(mobs);

	/** -!- Ideal way to create rooms programmatically -!- */
	var usvr_parking_lot = {
		rbuild: {
			title: 'Video rental parking lot',
			description: [
				'Black asphault, trash, and empty beer cans litter the mostly devoid ',
				'parking lot. White faded lines suggest where cars should be parked, ',
				'though it seems nobody pays attention to that.'
			].join(''),
			'sector-type': 'OUTSIDE_TRASHY_PARKING_LOT',
		}, /** end rbuild */
		mobs: [ 'bum','hooker' ]
	};
	var usvr_entrance = {
		rbuild: {
			title: 'U-Slurps video rental entrance',
			description: [
			'A sign with big red bold letters sputters on and off with backlit florescent white lights.',
			'A glass door with many stickers on the inside and outside advertise the latest deplorable ',
			'DVDs for rent, and oddly enough, religious propaganda.'
			].join(''),
			'sector-type': 'OUTSIDE_TRASHY_PARKING_LOT'
		},/** end rbuild */
		mobs: [ 'bum','hooker' ]
	};
	var usvr_entrance_newstand = {
		rbuild: {
			title: 'Newstand room',
			description: 
			'A variety of dirty magazines are stacked against the eastern wall. ' + 
			'To the north is the center isle. As you walk in, you notice the clerk ' +
			'stands behind a raised counter on the left.',
			'sector-type': 'OUTSIDE_TRASHY_PARKING_LOT',
		},/** end rbuild */
		mobs: [ 'creepycustomer' ]
	};
	var center_isle = {
		rbuild: {
			title: 'Center isle',
			description: 
			'A variety of magazines and videos line the isles to the east and west. ' +
			'To the north is a door with a sign that says {gld}Video Room{/gld}.',
			'sector-type': 'INDOOR_ROOF'
		},/** end rbuild */
		mobs: [ 'creepycustomer' ],
	};
	var checkout = {
		rbuild: {
			title: 'Checkout area',
			description: 
			'<TODO>',
			'sector-type': 'INDOOR_ROOF'
		},/** end rbuild */
		mobs: [ 'clerk' , 'lowly_security_guard']
	};

	rbuild_pave_continue([
		'n',
		usvr_parking_lot,
		'e',
		usvr_parking_lot,
		'w',
		'w',
		usvr_parking_lot,
		'e',
		'n',
		usvr_entrance,
		'n',
		center_isle,
		'n',
		center_isle,
		'n',
		center_isle,
		's','s',
		'w'
	]);

	/*
	run([
		'meqbuild new rsf-security 1',
		'meqbuild map-assign 405 1',
		'meqbuild map-assign 401 1',
		'meqbuild map-delete 401 1',
		'meqbuild map-list'
	]);
	*/
	send('To save the paved area, type: "rbuild save-paved 0"');
}
/**
 * ================================================================================
 * ================================================================================
 * ================================================================================
 * ================================================================================
 */
















/**
 * ================================================================================
 * ================================================================================
 * || SECTION: 
 * ||         P A R K W A Y  P L A Z A
 * ||					C O N T E N T
 * ||
 * ================================================================================
 * ================================================================================
 * v-parkway-plaza v-plaza v-content
 */
function do_parkway_plaza_area(){
	var north_entrance_description = [
		'To the south of you is the Northern entrance to Parkway Plaza ',
		'mall. The mall is indoors except for the parking lot which you ',
		'just came from to the north. A moderate amount of customers can ',
		'be seen entering and leaving this area.',
	].join('');
	var north_corridor = [
		'Stretching north and south is a long corridor that attaches to the ',
		'main area of the mall. To the east and west you see advertisements ',
		'on the walls.',
	].join('');
	var north_strip_A = [
		'Stretching north and south is a long corridor that attaches to the ',
		'main area of the mall. To the east and west you see advertisements ',
		'on the walls.',
	].join('');
	var sparse_wireless_entrance = [
		'{grn}Sparse Wireless{/grn} - {gld}2 Free Phones!{/gld}',
		'The entrance to Sparse Wireless is to the North.',
	].join('');
	var sparse_wireless_quadrant = [
		'{grn}Sparse Wireless{/grn} - {gld}2 Free Phones!{/gld}',
		'The entrance to Sparse Wireless is to the North.',
	].join('');
	var FILL_ME = '@FILL_ME@';
	run([
		'rbuild pave continue',
		's',
		'rbuild title Northern Entrance to Parkway Plaza',
		'rbuild description ' + north_entrance_description,
		's',
		'rbuild title Corridor to northern end of Parkway Plaza',
		'rbuild description ' + north_corridor,
		's',
		'rbuild title Corridor to northern end of Parkway Plaza',
		'rbuild description ' + north_corridor,
		'e',
		'rbuild title Northern Strip',
		'rbuild description ' + north_strip_A,
		'e',
		'rbuild title Northern Strip',
		'rbuild description ' + north_strip_A,
		'e',
		'rbuild title Northern Strip',
		'rbuild description ' + north_strip_A,
		'n',
		'rbuild title Sparse Wireless Store Entrance',
		'rbuild description ' + sparse_wireless_entrance,
		'n',
		'rbuild title Sparse Wireless',
		'rbuild description ' + sparse_wireless_quadrant,
		'w',
		'rbuild title Sparse Wireless Round Desk Area',
		'rbuild description ' + sparse_wireless_quadrant,
		'e',
		'n',
		'rbuild title Sparse Wireless',
		'rbuild description ' + sparse_wireless_quadrant,
		'n',
		'rbuild title Sparse Wireless - Backroom Entrance',
		'rbuild description ' + FILL_ME,
		'n',
		'rbuild title Sparse Wireless - Backroom',
		'rbuild description ' + FILL_ME,
		'e',
		'rbuild title Sparse Wireless - Backroom Bathroom Entrance',
		'rbuild description ' + FILL_ME,
		'e',
		'rbuild title Sparse Wireless - Backroom Bathroom',
		'rbuild description ' + FILL_ME,
		'w','w',
		'w',
		'rbuild title Sparse Wireless - Backroom Inventory Computer',
		'rbuild description ' + FILL_ME,
		'e',
		's',
		'rbuild pave off',
	]);
	send('To save the paved area, type: "rbuild save-paved 0"');
}








function mud_date(){
	send(get_month() + ' ' + get_day() + ' ' + get_iyear());
}
function set_first_tier(value){
	var key = 'LEVELS_FIRST_TIER';
	value_save(key,value);
	value_load(key);
}
/**
 * ================================================================================
 * ================================================================================
 * ================================================================================
 * ================================================================================
 */
















/**
 * ================================================================================
 * ================================================================================
 * || SECTION: 
 * ||         L I V E  T R A I N I N G
 * ||					C O N T E N T
 * ||
 * ================================================================================
 * ================================================================================
 * v-live-training v-content
 */
function title(t){
	return 'rbuild title ' + t;
}
function desc(d){
	return 'rbuild description ' + d;
}
function texture(list){
	return 'rbuild texture ' + list;
}
function mark(nick){
	return 'bookmark ' + nick + ' this';
}
function go(nick){
	return 'goto ' + nick;
}
function get_vnum(nick){
	return exec('print_vnum ' + nick);
}





function po(){
	run(['rbuild pave off']);
	return false;
}
function ignore(){}
/**
 * ================================================================================
 * ================================================================================
 * ================================================================================
 * ================================================================================
 */



















/**
 * ================================================================================
 * ================================================================================
 * || SECTION: 
 * ||         C O F O B
 * ||					C O N T E N T
 * ||
 * ================================================================================
 * ================================================================================
 * v-cofob v-content
 */

function do_medhall(){ /** NEEDS WORK */
	var medhall_start = [
		"The scent of sterility assaults your senses as you step into the hallway. ",
		"The tile floors, while immaculate in appearance, have more wear and tear than ",
		"even the cement floors just a few rooms in the other direction. To the east ",
		"is the patchbay, to the west is the surgical station for more serious maladies.",
	].join('');
	run([
		go(cofob.cofobcenter),
	]);

}
function should_run_shotgunner_zbuild_mob_command(){
	return false;
}
function do_waypoint_avenue_stalkers(){
	var mobs = {
		waypoint_avenue_thief: {
			mbuild: {
				attr: {
					virt: waypoint.thief.mob,
					mana: 45,
					max_mana: 45,
					hit: 150,
					max_hit: 150,
					move: 510,
					max_move: 510,
					damroll: 20,
					gold: 950,
					level: 15,
					weight: 5,
					height: 5,
					strength: 25,
					strength_add: 23,
					intelligence: 22,
					dexterity: 24,
					constitution: 28,
					charisma: 10,
					damnodice: 20,
					damsizedice: 16,
					name: "A suspicious looking car thief",
					sex: "MALE",
					action: "ISNPC HELPER AGGR_EVIL",
					short_description: "A suspicious looking car thief",
					long_description: "A suspicious looking car thief",
					description: "A car thief equipped with brass knuckles and a crowbar. He is obviously armed.",
				}, /** end attr */
				'extended-type': 'AGGRESSIVE_THIEF',
			},
			meqbuild: {
				name: 'waypoint-ave-car-thief',
				virtual_number: 2,
				positions: {
					wield: 'melee/crowbar.yml',
					hands: 'melee/brass-knuckles.yml',
					body: 'armor/basic-ballistic-vest.yml',
					head: 'armor/baklava.yml',
					legs: 'armor/dark-smithies-pants.yml',
					feet: 'armor/red-brug-sneakers.yml'
				}
			}
		},
		rogue_mp_shotgunner: {
			mbuild: {
				attr: {
					virt: waypoint.rogue_mp.mob,
					mana: 565,
					max_mana: 565,
					hit: 5550,
					max_hit: 5550,
					move: 510,
					max_move: 510,
					damroll: 30,
					gold: 150,
					level: 10,
					weight: 10,
					height: 6,
					strength: 35,
					strength_add: 33,
					intelligence: 32,
					dexterity: 34,
					constitution: 38,
					charisma: 10,
					damnodice: 15,
					damsizedice: 18,
					name: "A {red}Rogue{/red} Military Police shotgunner", 
					sex: "MALE",
					action: "ISNPC HELPER AGGR_EVIL",
					short_description: "A {red}Rogue{/red} Military Police shotgunner",
					long_description: "A {red}Rogue{/red} Military Police shotgunner",
					description: 'A fit military police shotgunner. He looks armed.',
				}, /** end attr */
				'extended-type': 'MP_SHOTGUNNER',
			},
			meqbuild: {
				name: 'mp-shotgunner',
				virtual_number: 3,
				positions: {
					wield: 'rifle/bf-39-shotgun.yml',
					hands: 'armor/mp-enforcer-gloves.yml',
					body: 'armor/basic-ballistic-vest.yml',
					legs: 'armor/mp-enforcer-pants.yml',
					feet: 'armor/basic-boots.yml'
				}
			}
		},
	};
	auto_feed_mob_list(mobs);
	/** mob roaming */

	var rogue_shotgunner_roam_data = [
		go(cofob.armory_b2),
		'mbuild roam:start rogue-mp-shotgunner ' + waypoint.rogue_mp.mob,
		's','s','s','s','s',
		'w','w',
		's','s',
		'w','w',
		'e','e',
		'e','e',
		'w','w',
		's','s',
		'w','w',
		'e','e',
		'e','e',
		'w','w',
		's','s',
		'w',
		'mbuild roam:stop rogue-mp-shotgunner',
		'mbuild roam:save rogue-mp-shotgunner',
	];
	run(rogue_shotgunner_roam_data);
}
function cobalt_military_police(){
	var mobs = {
		mp_enforcer: {
			mbuild: {
				attr: {
					virt: 500,
					mana: 45,
					max_mana: 45,
					hit: 250,
					max_hit: 250,
					move: 110,
					max_move: 110,
					damroll: 20,
					gold: 50,
					level: 10,
					weight: 10,
					height: 5,
					strength: 25,
					strength_add: 23,
					intelligence: 22,
					dexterity: 24,
					constitution: 28,
					charisma: 10,
					damnodice: 10,
					damsizedice: 6,
					name: "A Military Police enforcer", 
					sex: "MALE",
					action: "ISNPC HELPER AGGR_GOOD",
					short_description: "A Military Police enforcer",
					long_description: "A Military Police enforcer",
					description: 'A fit military police enforcer. He looks armed.',
				}, /** end attr */
				'extended-type': 'LOWLY_SECURITY',
			},
			meqbuild: {
				name: 'mp-enforcer',
				virtual_number: 4,
				positions: {
					wield: 'rifle/hk45.yml',
					hands: 'armor/mp-enforcer-gloves.yml',
					body: 'armor/basic-ballistic-vest.yml',
					legs: 'armor/mp-enforcer-pants.yml',
					feet: 'armor/basic-boots.yml'
				}
			},
			zbuild: {
				command: 'mob',
				zone_id: 0,
				mob_vnum: 500,
				room_vnum: 131,
				max: 10,
				if_flag: 0,
			}
		},
		mp_shotgunner: {
			mbuild: {
				attr: {
					virt: 501,
					mana: 565,
					max_mana: 565,
					hit: 5550,
					max_hit: 5550,
					move: 510,
					max_move: 510,
					damroll: 30,
					gold: 150,
					level: 10,
					weight: 10,
					height: 6,
					strength: 35,
					strength_add: 33,
					intelligence: 32,
					dexterity: 34,
					constitution: 38,
					charisma: 10,
					damnodice: 15,
					damsizedice: 18,
					name: "A Military Police shotgunner", 
					sex: "MALE",
					action: "ISNPC HELPER AGGR_GOOD",
					short_description: "A Military Police shotgunner",
					long_description: "A Military Police shotgunner",
					description: 'A fit military police shotgunner. He looks armed.',
				}, /** end attr */
				'extended-type': 'MP_SHOTGUNNER',
			},
			meqbuild: {
				name: 'mp-shotgunner',
				virtual_number: 5,
				positions: {
					wield: 'rifle/bf-39-shotgun.yml',
					hands: 'armor/mp-enforcer-gloves.yml',
					body: 'armor/basic-ballistic-vest.yml',
					legs: 'armor/mp-enforcer-pants.yml',
					feet: 'armor/basic-boots.yml'
				}
			},
			zbuild: {
				command: 'mob',
				zone_id: 0,
				mob_vnum: 501,
				room_vnum: 131,
				max: 10,
				if_flag: 0,
			}
		},
	};
	auto_feed_mob_list(mobs);
	/** mob roaming */

	var shotgunner_roam_data = [
		'recall',
		'mbuild roam:start mp-shotgunner 501',
		'n','n','e','w','w',
		'mbuild roam:stop mp-shotgunner',
		'mbuild roam:save mp-shotgunner',
	];
	run(shotgunner_roam_data);
	var enforcer_roam_data = [
		'recall',
		'mbuild roam:start mp-enforcer 500',
		'n','n','e','w',
		'w','w','w','w','w','w',
		'n', 'n', 'n', 'n', 
		'n', 'n', 'n', 'n',
		'n', 'n', 'n',
		'n', 'n', 'n', 'n',
		'e', 'e', 'e', 'e',
		'e', 'e', 'e', 'e',
		'e', 's', 's', 's',
		's', 's', 's', 's',
		's', 's', 's', 's',
		's', 's', 's', 's',
		'mbuild roam:stop mp-enforcer',
		'mbuild roam:save mp-enforcer',
	];
	run(enforcer_roam_data);
	
}
function do_ballistics_lab(){
	var mobs = {
		dr_land: {
			mbuild: {
				attr: {
					virt: scripted.doctor_land.mob,
					mana: 15,
					max_mana: 15,
					hit: 58580,
					max_hit: 58580,
					move: 140,
					max_move: 240,
					damroll: 215,
					gold: 505000,
					level: 40,
					weight: 10,
					height: 4,
					strength: 85,
					strength_add: 83,
					intelligence: 82,
					dexterity: 84,
					constitution: 88,
					charisma: 1,
					damnodice: 84,
					damsizedice: 86,
					name: "A Ballistics Expert",
					sex: "MALE",
					action: "ISNPC",
					short_description: "A Ballistics Expert",
					long_description: "A Ballistics Expert",
					description: "A Ballistics Expert",
				} /** end attr */
			}, /** end mbuild */
			meqbuild: {
				name: 'doctor-land',
				virtual_number: 6,
				positions: {
					hold: 'gadget/ballistics-lab-blue-key-fob.yml',
					primary: 'rifle/ump45-vulture-modded.yml',
					secondary: 'rifle/blackhawk-50cal-pistol.yml',
					hands: 'armor/forge-xm3-gloves.yml',
					body: 'armor/vulture-pk3-ballistic-vest.yml',
					legs: 'armor/heavy-armament-tactical-pants.yml',
					feet: 'armor/vulture-pk3-ballistic-boots.yml'
				}
			},
			zbuild: {
				command: 'mob',
				zone_id: 0,
				mob_vnum: scripted.doctor_land.mob,
				room_vnum: scripted.doctor_land.room,
				max: 1,
				if_flag: 0,
			}
		},// end scientist
	};//end mobs
	auto_feed_mob_list(mobs);
}
function do_triton_lab(){
	var mobs = {
		scientist: {
			mbuild: {
				attr: {
					virt: scripted.triton.mob,
					mana: 15,
					max_mana: 15,
					hit: 58580,
					max_hit: 58580,
					move: 140,
					max_move: 240,
					damroll: 215,
					gold: 505000,
					level: 40,
					weight: 10,
					height: 4,
					strength: 85,
					strength_add: 83,
					intelligence: 82,
					dexterity: 84,
					constitution: 88,
					charisma: 1,
					damnodice: 84,
					damsizedice: 86,
					name: "A {yel}TRITON{/yel} {blu}LABS{/blu} Scientist",
					sex: "MALE",
					action: "ISNPC",
					short_description: "A {yel}TRITON{/yel} {blu}LABS{/blu} Scientist",
					long_description: "A {yel}TRITON{/yel} {blu}LABS{/blu} Scientist",
					description: "A {yel}TRITON{/yel} {blu}LABS{/blu} Scientist",
				} /** end attr */
			}, /** end mbuild */
			meqbuild: {
				name: 'triton-scientist',
				virtual_number: 7,
				positions: {
					hold: 'gadget/triton-labs-yellow-key-fob.yml',
					goggles: 'armor/thick-glasses.yml',
					hands: 'armor/rubber-gloves.yml',
					body: 'armor/plain-white-lab-coat.yml',
					legs: 'armor/pale-brown-pants.yml',
					feet: 'armor/static-free-shoes.yml'
				}
			},
			zbuild: {
				command: 'mob',
				zone_id: 0,
				mob_vnum: scripted.triton.mob,
				room_vnum: scripted.triton.room,
				max: 1,
				if_flag: 0,
			}
		},// end scientist
		volunteerpatient: {
			mbuild: {
				attr: {
					virt: scripted.triton.volunteer_mob,
					mana: 25,
					max_mana: 25,
					hit: 880,
					max_hit: 880,
					move: 240,
					max_move: 240,
					damroll: 115,
					gold: 0,
					level: 15,
					weight: 10,
					height: 4,
					strength: 185,
					strength_add: 183,
					intelligence: 10,
					dexterity: 184,
					constitution: 188,
					charisma: 1,
					damnodice: 8,
					damsizedice: 90,
					name: "A volunteer patient",
					sex: "MALE",
					action: "ISNPC",
					short_description: "A volunteer patient",
					long_description: "A volunteer patient",
					description: "A volunteer patient stands here with dazed thousand mile stare.",
				} /** end attr */
			}, /** end mbuild */
			meqbuild: {
				name: 'triton-volunteer-patient',
				virtual_number: 8,
				positions: {
					wield: 'melee/improvised-shank.yml',
					body: 'armor/hospital-gown.yml',
					feet: 'armor/hospital-shoes.yml'
				}
			},
			zbuild: {
				command: 'mob',
				zone_id: 0,
				mob_vnum: scripted.triton.volunteer_mob,
				room_vnum: scripted.triton.volunteer_room,
				max: 8,
				if_flag: 0,
			}
		},// end volunteer patient
		surgeon: {
			mbuild: {
				attr: {
					virt: 602,
					mana: 815,
					max_mana: 815,
					hit: 8580,
					max_hit: 8580,
					move: 240,
					max_move: 240,
					damroll: 215,
					gold: 85000,
					level: 80,
					weight: 10,
					height: 4,
					strength: 185,
					strength_add: 183,
					intelligence: 182,
					dexterity: 184,
					constitution: 188,
					charisma: 85,
					damnodice: 184,
					damsizedice: 186,
					name: "A {yel}TRITON{/yel} {blu}LABS{/blu} Field Surgeon",
					sex: "FEMALE",
					action: "ISNPC",
					short_description: "A {yel}TRITON{/yel} {blu}LABS{/blu} Field Surgeon",
					long_description: "A {yel}TRITON{/yel} {blu}LABS{/blu} Field Surgeon",
					description: "A {yel}TRITON{/yel} {blu}LABS{/blu} Field Surgeon",
				} /** end attr */
			}, /** end mbuild */
			meqbuild: {
				name: 'triton-field-surgeon',
				virtual_number: 9,
				positions: {
					wield: 'melee/operating-scalpel.yml',
					goggles: 'armor/operating-goggles.yml',
					hands: 'armor/rubber-gloves.yml',
					body: 'armor/plain-white-lab-coat.yml',
					legs: 'armor/pale-brown-pants.yml',
					feet: 'armor/static-free-shoes.yml'
				}
			},
			zbuild: {
				command: 'mob',
				zone_id: 0,
				mob_vnum: 602,
				room_vnum: scripted.triton.room,
				max: 1,
				if_flag: 0,
			}
		},// end scientist
	};//end mobs
	auto_feed_mob_list(mobs);
	var patient_roam_data = [
		'recall',
		'n','n',
		'e','e',
		'n','n',
		'e','e','e',
		'mbuild roam:start volunteer-patient ' + scripted.triton.volunteer_mob,
		'e', 'd', 'e', 'e','d','e','e','d','e','e',
		'mbuild roam:stop volunteer-patient',
		'mbuild roam:save volunteer-patient',
	];
	run(patient_roam_data);
}
function do_live_training_exercise_FOB_1A(){
	var south_main_hallway = [
		'Pod 41A'
	].join('');
	run([ 'rbuild pave continue' ]);
	var draw_rectangle = [
		'rbuild title {blu}COBALT Air Force Base:{/blu} {grn}Live Training Exercise Station 1A{/grn}',
		'rbuild description ' + south_main_hallway,
		'rbuild sector-type INDOOR_CEMENT',
		'room_dark off'
	];
	var directions = ['e','n','w','s'];
	var ctr = 0, width = 5, height = 4, width_counter = 0, height_counter = 0;
	for(; ctr < 4; ctr++){
		var measurement = width;
		if(directions[ctr] === 'n' || directions[ctr] === 's'){
			measurement = height;
		}
		for(; measurement > 0; --measurement){
			run([directions[ctr]]);
			run(draw_rectangle);
		}
	}

}
function do_second_floor_engineering(){
	var far_north_engineering_hallway = [
		'The carpet beneath your feet is a drab grey color. The northern wall ',
		'is made up of mostly one-way glass panels that overlook the outer ',
		'courtyard below. To the east is a single door with advanced bio-metric ',
		'scanners that prevent unauthorized access. An iris scanner, voice recognition, ',
		'and a gait recognition camera all record your every move. ',
	].join('');
	var engineering_hallway = [
		"The doors on the eastern wall are all closed and most likely locked. ",
		"You see the commons area at the end of the hallway. You notice various ",
		"surveillance cameras strategically placed at the corners of the ceiling. ",
		/** TODO: add integral camera feeds here */
	].join('');
	var engineering_room_1a = [
		'The promise of coffee lingers in the air as the strictly regulated air conditioner ',
		'calmy spreads 68 degree air. A row of computers stretches to the north and south. ',
		'You can see a server room at the end of the long strip of computer screens all the ',
		'way at the north end of the room. ',
	].join('');
	var engineering_commons = [
		"A comfy looking couch and recliner welcome you to the engineering commons ",
		"area. Three television screens mounted on the west wall display various ",
		"graphs and statistics. A ping pong table is in the center of the room. ",
		"Two large refridgerators are humming quietly in the corner. The north wall ",
		"consists of large one-way tinted windows that overlook the training fields ",
		"below. To the east is the entrance to server room 1B.",
	].join('');
	var server_room_1a = [
		"It's noisy and loud in here. The rack mounted servers are fervently whirring ",
		'under the immense workloads. The ceiling is a grate with a large fan behind ',
		'it. A complex looking locking mechanism protects rack mounted servers here. ',
	].join('');
	var server_room_1b = [
		"Wall to wall rack mounted servers take up the entirety of this room. There ",
		"is almost nowhere to stand. A haphazard array of ethernet cables snake ",
		"chaotically to and from each server. Maintaining this must be a headache. ",
	].join('');
	var fm = '@FILL_ME@';

	run([
		repeat(4,['e']),
		repeat(4,['n',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Engineering hallway',
		'rbuild description ' + engineering_hallway,
		'rbuild sector-type INSIDE CARPET ROOFTOP',
		]),
		'n',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Commons area',
		'rbuild description ' + engineering_commons,
		'rbuild sector-type INSIDE CARPET ROOFTOP',
		mark(cofob.eng_commons),/** For the purpose of camera-feed */
		repeat(3,['s']),
		'e',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Server Room 1A',
		'rbuild description ' + server_room_1a,
		'rbuild sector-type INSIDE CARPET ROOFTOP',
		'w',
		repeat(3, ['n']),
		'e',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Server Room 1B',
		'rbuild description ' + server_room_1b,
		'rbuild sector-type INSIDE CARPET ROOFTOP',
		'w',
		repeat(5, ['s']),
		repeat(4, ['w']), /** return to center */
	]);
	return true;
}
function do_second_floor(){
	var eastern_hallway_middle = [
		'The hallway stretches north and south. The cement floor shows serious signs of wear and tear. ',
		'It seems whoever was in charge of cleaning this portion of the base did a very poor job, which ',
		'surprises you. A ventilation shaft calmly blows semi-cold air throughout this portion of the hallway. ',
		'{gld}A Bulletin Board{/gld} lists various contracts.'
	].join('');
	var fm = '@FILL_ME@';
	var eastern_hallway_middle = fm;
	var shooting_range_entrance_east = fm;
	var shooting_range_falcon = fm;
	var second_floor_hallway = fm;
	var north_hallway = fm;
	var north_hallway_stairs = fm;
	var second_floor_hallway = fm;
	var western_hallway = [
		"The reinforced metal walls are briefly interrupted by the occasional senior officer ",
		"door. As you make your way north and south, you notice that the entirety of the western ",
		"wall consists of metal walls with doors that lead to the underground portion of the base. ",
	].join('');

	var cofob_center_vnum = exec('print_vnum ' + cofob.center);
	run([
		/** start of mess hall */
		go(cofob.east_exit),
		repeat(3,[
			'n',
			'rbuild title {blu}C.O.F.O.B:{/blu} - Eastern Hallway',
			'rbuild description ' + eastern_hallway_middle,
			'rbuild sector-type INDOOR_CEMENT',
		]),
		mark(cofob.mess_hall_upstairs),
		repeat(2,['n',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Eastern Hallway',
		'rbuild description ' + eastern_hallway_middle,
		'rbuild sector-type INDOOR_CEMENT',
		]),
		repeat(2,['w',
		'rbuild title {blu}C.O.F.O.B:{/blu} - North Hallway',
		'rbuild description ' + north_hallway,
		'rbuild sector-type INDOOR_CEMENT',
		]),
		'w',
		'rbuild title {blu}C.O.F.O.B:{/blu} - North Hallway Stairs',
		'rbuild description ' + north_hallway_stairs,
		'rbuild sector-type INDOOR_CEMENT STAIRS',
		mark('cofob-stairs-A'),
		repeat(5,['w',
		'rbuild title {blu}C.O.F.O.B:{/blu} - North Hallway',
		'rbuild description ' + north_hallway,
		'rbuild sector-type INDOOR_CEMENT',
		]),
		repeat(14,['s',
			'rbuild title {blu}C.O.F.O.B [OUTER]:{/blu} - Western Hallway',
			'rbuild description ' + western_hallway,
			'rbuild texture INSIDE DRY ROOFTOP CEMENT',
		]),
		repeat(5,['e',
			'rbuild title {blu}C.O.F.O.B [OUTER]:{/blu} - South Hallway',
			'rbuild description ' + western_hallway,
			'rbuild texture INSIDE DRY ROOFTOP CEMENT',
		]),
		mark('cofob-bind-to-center'),
		'rbuild bind EAST ' + cofob_center_vnum,
		'e',
		go('cofob-stairs-A'),
		'n',
		'rbuild title {blu}C.O.F.O.B:{/blu} - North Hallway Stairs',
		'rbuild description ' + north_hallway_stairs,
		'rbuild sector-type INDOOR_CEMENT STAIRS',
		'u',
		'rbuild title {blu}C.O.F.O.B:{/blu} - North Hallway Stairs',
		'rbuild description ' + north_hallway_stairs,
		'rbuild sector-type INDOOR_CEMENT STAIRS',
		'n',
		'rbuild title {blu}C.O.F.O.B:{/blu} - North Hallway Stairs',
		'rbuild description ' + north_hallway_stairs,
		'rbuild sector-type INDOOR_CEMENT STAIRS',
		'u',
		'rbuild title {blu}C.O.F.O.B:{/blu} - North Hallway Stairs',
		'rbuild description ' + north_hallway_stairs,
		'rbuild sector-type INDOOR_CEMENT STAIRS',
		/** start of second floor */
		repeat(2,['n',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Second floor hallway',
		'rbuild description ' + second_floor_hallway,
		'rbuild sector-type INDOOR_CEMENT',
		]),
		mark(cofob.secondfloor_center),//'secondfloor-center'),
		repeat(5,['w',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Second floor hallway',
		'rbuild description ' + second_floor_hallway,
		'rbuild sector-type INDOOR_CEMENT',
		]),
		go(cofob.secondfloor_center),
		repeat(2,['n',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Second floor Foremast',
		'rbuild description ' + fm,
		'rbuild sector-type INSIDE CARPET ROOFTOP',
		]),
		'e',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Second floor Foremast',
		'rbuild description ' + fm,
		'rbuild sector-type INSIDE CARPET ROOFTOP',
		repeat(3,['n',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Second floor Foremast',
		'rbuild description ' + fm,
		'rbuild sector-type INSIDE CARPET ROOFTOP',
		]),
		repeat(3,['s']),
		'w',
		go(cofob.secondfloor_center),
		repeat(2,['n']),
		'w',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Second floor Foremast',
		'rbuild description ' + second_floor_hallway,
		'rbuild sector-type INDOOR_CEMENT CARPET ROOFTOP',
		repeat(3, ['n',
			'rbuild title {blu}C.O.F.O.B:{/blu} - Second floor Foremast',
		'rbuild description ' + second_floor_hallway,
		'rbuild sector-type INDOOR_CEMENT CARPET ROOFTOP',
		]),
		repeat(3, ['s']),
		'e',
		//repeat(2, ['s']), /** return to center */
		go(cofob.secondfloor_center),
		repeat(5,['e',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Second floor hallway',
		'rbuild description ' + second_floor_hallway,
		'rbuild sector-type INDOOR_CEMENT',
		]),
		//repeat(5,['w']), /** return to center */
		go(cofob.secondfloor_center),
	]);
	var bind_to_center_vnum = exec('print_vnum cofob-bind-to-center');
	run([
		go('cofob-bind-to-center'),
		'e',
		'rbuild bind WEST ' + bind_to_center_vnum,
		go(cofob.secondfloor_center),
	]);

	do_second_floor_engineering();
	return true;
}
function do_cobalt_spawn(){
	var FILL_ME = '@FILL_ME@';
	var spawn_entry = [
		'Description of spawn entry to come soon'
	].join('');
	var eastern_corps_drive = [
		FILL_ME
	].join('');
	var sniper_rifle_shop = [
		FILL_ME
	].join('');
	run([
		'rbuild pave continue',
		'n',
		'rbuild title {blu}COBALT Forward Operating Base{/blu} - General Area',
		'rbuild description ' + spawn_entry,
		'rbuild sector-type INDOOR_CEMENT',
		'e',
		'rbuild title {blu}COBALT Forward Operating Base{/blu} - Corps Drive',
		'rbuild description ' + eastern_corps_drive,
		'rbuild sector-type OUTDOOR_CEMENT',
		'e',
		'rbuild title {blu}COBALT Forward Operating Base{/blu} - Corps Drive',
		'rbuild description ' + eastern_corps_drive,
		'rbuild sector-type OUTDOOR_CEMENT',
		'n',
		'rbuild title {blu}COBALT Forward Operating Base{/blu} - Ranged Weapon Shop',
		'rbuild description ' + sniper_rifle_shop,
		'rbuild sector-type INDOOR_CEMENT',
	]);
}
function do_cofob_west_area_from(nickname_for_center){
	var western_hallway = [
		"The reinforced metal walls are briefly interrupted by the occasional senior officer ",
		"door. As you make your way north and south, you notice that the entirety of the western ",
		"wall consists of metal walls with doors that lead to the underground portion of the base. ",
	].join('');
	var atop_the_stairs= [
		"The floor deviates from the other rooms in that it is made up of a reinforced steel grating. ",
		"The choice for such a floor remains a mystery. The door to the west leads to the stairs that will ",
		"take you to the underground portion of the base.",
	].join('');
	var descending_the_stairs = [
		"fill me",
	].join('');
	run([
		go(cofob.center),
		repeat(6,['w']),
		go(cofob.center),
		repeat(6,['w']),
		'n',
		'rbuild title {blu}C.O.F.O.B [OUTER]:{/blu} - Atop the stairs',
		'rbuild description ' + atop_the_stairs,
		'rbuild texture INSIDE DRY ROOFTOP CEMENT',
		'n',
		'rbuild title {blu}C.O.F.O.B [OUTER]:{/blu} - Atop the stairs',
		'rbuild description ' + atop_the_stairs,
		'rbuild texture INSIDE DRY ROOFTOP CEMENT',
		mark(cofob.west_atop_stairs),//'west-atop-the-stairs'),
		'w',
		'rbuild title {blu}C.O.F.O.B [OUTER]:{/blu} - Descending the stairs',
		'rbuild description ' + descending_the_stairs,
		'rbuild texture INSIDE DRY ROOFTOP STAIRS',
		'd',
		'rbuild title {blu}C.O.F.O.B [OUTER]:{/blu} - Descending the stairs',
		'rbuild description ' + descending_the_stairs,
		'rbuild texture INSIDE DRY ROOFTOP STAIRS',

	]);
	return true;
}
function do_mess_hall(){
	var mess_hall_upstairs = [
		"Metal railings accompany the stairs leading down to the mess hall.",
	].join('');
	var mess_hall_serving_area = [
		"The tile floor is immaculate in presentation. Plain grey metal chairs attend each ",
		"table obediently in a neat formation of 2 chairs to each cardinal side.",
		"A few familiar dishes invite you to sit down and enjoy the hastily prepared meals. ",
		"The chefs can be seen in the kitchen looking down as they prepare the food. The remote ",
		"promise of coffee is enticing.",
	].join('');
	run([
		go(cofob.mess_hall_upstairs),
		'w',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Mess Hall stairs',
		'rbuild description ' + mess_hall_upstairs,
		'rbuild sector-type INDOOR CEMENT ROOFTOP',
		'd',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Mess Hall stairs',
		'rbuild description ' + mess_hall_upstairs,
		'rbuild sector-type INDOOR CEMENT ROOFTOP',
		repeat(2,['w',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Mess Hall stairs',
		'rbuild description ' + mess_hall_upstairs,
		'rbuild sector-type INDOOR CEMENT ROOFTOP',
		]),
		'd',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Mess Hall stairs',
		'rbuild description ' + mess_hall_upstairs,
		'rbuild sector-type INDOOR CEMENT ROOFTOP',
		repeat(2,['w',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Mess Hall stairs',
		'rbuild description ' + mess_hall_upstairs,
		'rbuild sector-type INDOOR CEMENT ROOFTOP',
		]),
		repeat(2,['s',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Mess Hall serving area',
		'rbuild description ' + mess_hall_serving_area,
		'rbuild sector-type INDOOR CEMENT ROOFTOP',
		]),
		repeat(2,['e',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Mess Hall serving area',
		'rbuild description ' + mess_hall_serving_area,
		'rbuild sector-type INDOOR CEMENT ROOFTOP',
		]),
		repeat(2,['s',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Mess Hall serving area',
		'rbuild description ' + mess_hall_serving_area,
		'rbuild sector-type INDOOR CEMENT ROOFTOP',
		]),
		repeat(2,['n']),
		repeat(2,['e',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Mess Hall serving area',
		'rbuild description ' + mess_hall_serving_area,
		'rbuild sector-type INDOOR CEMENT ROOFTOP',
		]),
		repeat(2,['s',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Mess Hall serving area',
		'rbuild description ' + mess_hall_serving_area,
		'rbuild sector-type INDOOR CEMENT ROOFTOP',
		]),
		repeat(2,['n']),
		repeat(2,['e',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Mess Hall serving area',
		'rbuild description ' + mess_hall_serving_area,
		'rbuild sector-type INDOOR CEMENT ROOFTOP',
		]),
		repeat(2,['s',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Mess Hall serving area',
		'rbuild description ' + mess_hall_serving_area,
		'rbuild sector-type INDOOR CEMENT ROOFTOP',
		]),
		repeat(2,['n']),
		go(cofob.mess_hall_upstairs),
	]);
	return true;
}
function do_cobalt_fob(){
	var south_main_hallway = [
		'COBALT Air Force base. Ground zero for basic training. All initiates must follow ',
		'rules and guidelines in your New Recruit handbook. Proceed {grn}North{/grn} recruit!',
	].join('');
	var south_hallway_east_west = [
		'The cement floor is a perfect complement to the reflection of the incandescent lights that ',
		'are installed into the the ceiling. {gld}To the East{/gld}, you will find the ',
		'Armory. {gld}To the West{/gld}, you will find the training grounds. Just around ',
		'the corner to the East and North a ways is the Weapons Locker. Feel free to get ',
		'acquainted with the layout, recruit.',
	].join('');
	var eastern_hallway_south_corner = [
		'The hallway reaches north and south from here. A reduced temperature is like the result ',
		'of the industrial grade internal air cooling system. It isn\'t much, but it beats the ',
		'outside desert climate. The promise of coffee entices you, but you can\'t tell which direction ',
		'it\'s coming from. ',
	].join('');
	var armory_entrance = [
		'A cool draft moves through the bottom crack of the door to the Armory Entrance to the East.',
		'It seems the quality of air drastically differs depending on the people in charge of each ',
		'department. The Sign above the door says in bold letters "Armory".',
	].join('');
	var armory = [
		'As you push through to the East, you notice a few recruits putting on standard ',
		'issue gear. They ignore you as you take a look around. To the East is the buy station ',
		'where you can make your purchases.'
	].join('');
	var armory_buy_station = [
		'You see an armor locker with standard issue equipment. Behind the counter is a {gld}list{/gld} of ',
		'all the various items for sell. You can spend {grn}MP{/grn} (Mission Points) here to upgrade your loadout. ',
		'To buy something, simply type {grn}"Buy ID"{/grn} where ID is the number next to the item you want in the ',
		'output of the list command.'
	].join('');
	var armory_sec_room = [
		'A musty room with several freshly smoked cigars laying inside a deep ash tray the size of your fist. ',
		'Someone was loading a Mossberg shotgun and haphazardly left it laying upon the couch as if it were a ',
		'visitor. The T.V. appears to still be warm. Whoever was here is likely coming back soon. You have a ',
		'feeling whoever was here will be back shortly.',
	].join('');
	var eastern_hallway_middle = [
		'The hallway stretches north and south. The cement floor shows serious signs of wear and tear. ',
		'It seems whoever was in charge of cleaning this portion of the base did a very poor job, which ',
		'surprises you. A ventilation shaft calmly blows semi-cold air throughout this portion of the hallway. ',
		'{gld}A Bulletin Board{/gld} lists various contracts.'
	].join('');
	var  armory_storage_room_north = [
		'Standard issue armor and defensive utilities line the walls; none of which you can take as they ',
		'are behind metal cages. There is, however, an Armor locker here with standard issue gear for ',
		'anyone to take. '
	].join('');
	var armory_storage_room_south = [
		'You enter the storage room and immediately notice the strong scent of sand, grime, and gasoline. ',
		'A few bits of ammunition are strewn across the floor haphazardly. The Armory personnell either ',
		'recently dug through the piles of ammo crates, or nobody bothered to clean this mess up. ',
		'There seems to be a computer terminal on the East wall.'
		/** TODO: put computer terminal on east wall */
	].join('');
	var eastern_hallway = [
		"The corridor is dimly lit by flourescent lights embedded into the metal ceiling. The corners ",
		"where the wall meets the ceiling look damp and dusty at the same time. The air is a cool contrast ",
		"to the outside desert air. You can't tell if it's day or night outside, but the low level of light ",
		"tricks your mind into thinking it's daytime all day."
	].join('');
	var eastern_exit = [
		'Before you is a sturdy metal door that prevents the outside elements from making their way ',
		'inside. The door simply says {yel}Eastern Exit{/yel}. The air is less cool as ',
		'the corridor leading to the east lacks the sufficient air flow. There are no ventilation ',
		'shafts leading to the east, but you can see through the tempered glass window on the door ',
		'that a few military police are gaurding the exit to the city outside. ',
	].join('');
	var eastern_dmz_corridor = [
		'Dirt stretches to the east towards a heavy containment door. Behind ',
		'that door must be where you can leave the building. The corridor is dimly ',
		'lit. The air is dry and smells of a combination of sweat and gasoline for some ',
		'reason.',
	].join('');
	var eastern_dmz_trap_door = [
		'Beneath the floor here is an entrance to an underground tunnel. The only problem ',
		"is that you can't seem to get it to move. To the east is the door to the outside world.",
		'A falcon engraving is above the door frame to the east.',
	].join('');
	var abott_market_south = [
		'Crushed and flattened by endless shells, the overpass that you just passed under creaks ',
		'eternally. Now and then you hear the crumble of asphault and be bend of metal rods. The ',
		'overpass connects the once thriving market place of Abott Market, but now all you see is ',
		'rubble and a shell of what used to be a thriving market.',
	].join('');
	var abott_market_abandoned_street_middle = [
		'A trio of burning cars have become part of the debris scattered along the street. ',
		'Way off to the north, you spot a working overpass. Deep long scars of blackened ',
		'concrete tell a tale of destruction. To the west lies a small set of apartments ',
		'once owned by the only property company to operate in this desolate portion of town. ',
		'A giant construction crane is resting in the middle of the street to the north.'
	].join('');
	var market_apt_east_entrance = [
		'An iron rod gate can be seen laying discarded near the entrance. In another time, ',
		'that gate would have been used to keep intruders out. Far off to the west is what ',
		"used to be the tenant parking lot. You'd be surprised to find any tenant using that ",
		'lot seeing as how low the income requirements were for this apartment complex.',
	].join('');
	var market_apt_building_3 = [
		'You see a two story apartment building with 8 units. Each apartment is a corner unit, ',
		'but neither home could possibly be more than a studio unit. A few of the doors are ',
		"closed, which you find peculiar. There can't possibly be anyone living there. Right?",
	].join('');
	var market_apt_building_2 = [
		'Building 2 is a two story building with 16 units. The stairs leading to the second ',
		'story are completely demolished. The top 4 units to the east are completely exposed ',
		'to the elements. You could make it upstairs but it would require some sort of rope. ',
	].join('');
	var market_apt_building_1 = [
		'Building 1 is a two story building with 8 units with the added addition of an exercise ',
		"room and what looks like the property manager's office. Both the exercise room and the office ",
		'look surprisingly intact. You notice lots of fresh footprints to and from building 1 ',
		'which disappear into the destruction of building 2. Despite the abandoned nature of ',
		'the complex as a whole, someone is still finding use for these units.',
	].join('');
	var market_apt_building_3_stairs = [
		'Before you lie the stairs that will help you reach the second floor of building 3. ',
		'The handrails are extremely hot to the touch due to baking here under the steady gaze ',
		'of the sun over your shoulder.',
	].join('');
	var market_apt_building_3_second_floor_hallway = [
		'A thin layer of dust and dirt cover the cement walkway here. You hear the crackle ',
		'of dirt and millions of pieces of broken glass under your feet as you make your ',
		'way about. Oddly enough, there are footprints here.',
	].join('');
	var market_apt_unit_301 = [
		'A mostly empty apartment. You notice what looks like sleeping bags in the corner ',
		'but that could just be trash. The air smells like cigarette smoke and dry desert ',
		'air. The light from outside illuminates trillions of dust particles floating in the ',
		'in perpetual motion.',
	].join('');
	var market_apt_unit_301_bathroom = [
		"The sink seems to still be intact and usable. How any water made its way up here ",
		'would be nothing short of miraculous. It does appear that the sink has been used ',
		'recently, which only affirms your suspicion that there are still inhabitants who ',
		'dwell here.'
	].join('');
	var shooting_range_gear_room = [
		"A huge weapons rack is attached to the wall here. The rack contains rifles, ",
		"pistols, ammunition, explosives, and breach charges.",
	].join('');

	var market_apt_unit_303 = market_apt_unit_301;
	var market_apt_unit_303_bathroom = market_apt_unit_301_bathroom;
	var market_apt_unit_303_bedroom = market_apt_unit_301_bedroom;


	var market_apt_unit_305 = market_apt_unit_301;
	var market_apt_unit_305_bathroom = market_apt_unit_301_bathroom;
	var market_apt_unit_305_bedroom = market_apt_unit_301_bedroom;

	var market_apt_unit_307 = market_apt_unit_301;
	var market_apt_unit_307_bathroom = market_apt_unit_301_bathroom;
	var market_apt_unit_307_bedroom = market_apt_unit_301_bedroom;

	var market_apt_unit_301_bedroom = [
		"A D.I.Y. home stove sits in the middle of the room. Aside from that, ",
		"there's nothing to look at. The carpet has been pulled up in places. ",
		"The walls are covered with massive holes and random burn marks.",
	].join('');
	var fm = '@FILL_ME@';
	var mess_hall_upstairs = [
		"Metal railings accompany the stairs leading down to the mess hall.",
	].join('');
	var mess_hall = [
		"The tile floor is immaculate in presentation. Plain grey metal chairs attend each ",
		"table obediently in a neat formation of 2 chairs to each cardinal side. To the south ",
		"is the cooking area where the chefs fill plates full of food.",
	].join('');
	var eastern_hallway_middle = fm;
	var shooting_range_entrance_east = fm;
	var shooting_range_falcon = fm;
	var second_floor_hallway = fm;
	var north_hallway = fm;
	var north_hallway_stairs = fm;


	var FILL_ME = '@FILL_ME@';
	run([
		'rbuild pave continue',
		'n',
		'rbuild title {blu}COBALT:{/blu} {grn}Forward Operating Base{/grn} - Main Hallway',
		'rbuild description ' + south_main_hallway,
		'rbuild sector-type INDOOR_CEMENT',
		'n',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Southern Hallway',
		'rbuild description ' + south_hallway_east_west,
		'rbuild sector-type INDOOR_CEMENT',
		mark(cofob.center),//'cofobcenter'),
		repeat(6,['w',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Southern Hallway',
		'rbuild description ' + south_hallway_east_west,
		'rbuild sector-type INDOOR_CEMENT',
		]),

		go(cofob.center),
		'e',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Southern Hallway',
		'rbuild description ' + south_hallway_east_west,
		'rbuild sector-type INDOOR_CEMENT',
		'e',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Southern Hallway',
		'rbuild description ' + south_hallway_east_west,
		'rbuild sector-type INDOOR_CEMENT',
		'n',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Southeast Corner',
		'rbuild description ' + eastern_hallway_south_corner,
		'rbuild sector-type INDOOR_CEMENT',
		'n',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Armory Entrance',
		'rbuild description ' + armory_entrance,
		'rbuild sector-type INDOOR_CEMENT',
		mark(cofob.armory_entrance),
		'e',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Armory',
		'rbuild description ' + armory,
		'rbuild sector-type INDOOR_CEMENT',
		'e',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Armory Buy Station',
		'rbuild description ' + armory_buy_station,
		'rbuild sector-type INDOOR_CEMENT',
		'n',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Armory Storage Room North',
		'rbuild description ' + armory_storage_room_north,
		'rbuild sector-type INDOOR_CEMENT',
		's','s',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Armory Storage Room South',
		'rbuild description ' + armory_storage_room_south,
		'rbuild sector-type INDOOR_CEMENT',
		'n',
		'e',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Armory - Secluded Room',
		'rbuild description ' + armory_sec_room,
		'rbuild sector-type INDOOR_CEMENT',
		'e',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Armory - Secluded Room - Weapons Cache',
		'rbuild description ' + fm,
		'rbuild sector-type INDOOR_CEMENT',
		'd',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - TOP',
		'rbuild description ' + fm,
		'rbuild sector-type INDOOR_CEMENT',
		'e',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - TOP',
		'rbuild description ' + fm,
		'rbuild sector-type INDOOR_CEMENT',
		'e',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - TOP',
		'rbuild description ' + fm,
		'rbuild sector-type INDOOR_CEMENT',
		'd',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 1',
		'rbuild description ' + fm,
		'rbuild sector-type INDOOR_CEMENT',
		'e',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 1',
		'rbuild description ' + fm,
		'rbuild sector-type INDOOR_CEMENT',
		'e',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 1',
		'rbuild description ' + fm,
		'rbuild sector-type INDOOR_CEMENT',
		'd',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 2',
		'rbuild description ' + fm,
		'rbuild sector-type INDOOR_CEMENT',
		'e',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 2',
		'rbuild description ' + fm,
		'rbuild sector-type INDOOR_CEMENT',
		'e',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 2',
		'rbuild description ' + fm,
		'rbuild sector-type INDOOR_CEMENT',
		mark(cofob.armory_b2),
		repeat(5,['s',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 2',
		'rbuild description ' + fm,
		'rbuild sector-type INDOOR_CEMENT',
		]),

		repeat(5,['w',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 2',
		'rbuild description ' + fm,
		'rbuild sector-type INDOOR_CEMENT',
		]),
		repeat(5,['n',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 2',
		'rbuild description ' + fm,
		'rbuild sector-type INDOOR_CEMENT',
		]),
		repeat(2,['s']),
		repeat(2,['e',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Basement 2 - Breach charges',
		'rbuild description ' + fm,
		'rbuild sector-type INDOOR_CEMENT',
		]),
		repeat(2,['w']),
		mark(cofob.armory_b2),
		repeat(5,['s']),
		repeat(2, ['w']),
		mark('cofob-armory-waypoint-avenue-exit'),
		'rbuild title {blu}C.O.F.O.B:{/blu} - Basement 2 - Waypoint Avenue North Exit',
		'rbuild description ' + fm,
		'rbuild sector-type INDOOR_CEMENT',
		repeat(2,['s',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Basement 2 - Car Garage',
		'rbuild description ' + fm,
		'rbuild sector-type OUTDOOR_CEMENT',
		]),
		repeat(2,['w',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Basement 2 - Car Garage - 1A',
		'rbuild description ' + fm,
		'rbuild sector-type OUTDOOR_CEMENT',
		]),
		repeat(2,['e']),
		repeat(2,['e',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Basement 2 - Car Garage - 1B',
		'rbuild description ' + fm,
		'rbuild sector-type OUTDOOR_CEMENT',
		]),
		repeat(2,['w']),
		repeat(2,['s',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Basement 2 - Car Garage',
		'rbuild description ' + fm,
		'rbuild sector-type OUTDOOR_CEMENT',
		]),
		repeat(2,['w',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Basement 2 - Car Garage - 2A',
		'rbuild description ' + fm,
		'rbuild sector-type OUTDOOR_CEMENT',
		]),
		repeat(2,['e']),
		repeat(2,['e',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Basement 2 - Car Garage - 2B',
		'rbuild description ' + fm,
		'rbuild sector-type OUTDOOR_CEMENT',
		]),
		repeat(2,['w']),
		repeat(2,['s',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Basement 2 - Car Garage',
		'rbuild description ' + fm,
		'rbuild sector-type OUTDOOR_CEMENT',
		]),
		'w',
		'rbuild title Waypoint North',
		'rbuild description ' + fm,
		'rbuild sector-type OUTDOOR_CEMENT',
		mark('waypoint-car-garage-north-1'),

		go(cofob.armory_entrance),
		/** back to hallway */
		repeat(6, ['n',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Eastern Hallway',
		'rbuild description ' + eastern_hallway_middle,
		'rbuild sector-type INDOOR_CEMENT',
		]),
		'n',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Eastern Exit',
		'rbuild description ' + eastern_exit,
		'rbuild sector-type INDOOR_CEMENT',
		mark(cofob.east_exit),//'eastexit'),
		/** end of eastern section */
	]);

	if(!do_second_floor()){
		return;
	}
	var range_locker = [
			'rifle/g36c.yml', 
			'explosive/breach-charge.yml',
			'explosive/claymore-mine.yml',
			'explosive/emp-grenade.yml',
			'explosive/frag-grenade.yml',
			'explosive/incendiary-grenade.yml',
			'explosive/sensor-grenade.yml',
			'explosive/smoke-grenade.yml',
			'explosive/flashbang-grenade.yml',
			'rifle/g36c.yml',
			'rifle/czp10.yml',
    	'attachment/ar-556mm-mag.yml',
    	'attachment/pistol-9mm-mag.yml',
	];
	var do_range_locker = function(){
		var locker_commands = [];
		locker_commands.push('install_weapon_locker');
		for(var i in range_locker){
			locker_commands.push(range_locker[i]);
		}
		locker_commands = locker_commands.join(' ');
		run([
			locker_commands
		]);
		var quota = [];
		for(var i in range_locker){
			quota.push('weapon_locker_quota ' + range_locker[i] + ' 10');
		}
		run(quota);
	};


	run([
		go(cofob.east_exit),//'eastexit'),
		'n',
		'n',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Eastern Hallway',
		'rbuild description ' + eastern_hallway_middle,
		'rbuild sector-type INDOOR_CEMENT',
		'n',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Eastern Hallway',
		'rbuild description ' + eastern_hallway_middle,
		'rbuild sector-type INDOOR_CEMENT',
		'n',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Eastern Hallway',
		'rbuild description ' + eastern_hallway_middle,
		'rbuild sector-type INDOOR_CEMENT',
		'e',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Eastern Hallway',
		'rbuild description ' + eastern_hallway_middle,
		'rbuild sector-type INDOOR_CEMENT',
		'e',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Gear room',
		'rbuild description ' + shooting_range_gear_room,
		'rbuild sector-type INDOOR CEMENT ROOFTOP',
		mark(cofob.gear_room),
		'n',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Shooting Range Area Falcon',
		'rbuild description ' + shooting_range_falcon,
		'rbuild sector-type INDOOR CEMENT ROOFTOP',
		'n',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Shooting Range Area Falcon',
		'rbuild description ' + shooting_range_falcon,
		'rbuild sector-type INDOOR CEMENT ROOFTOP',
		'n',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Shooting Range Area Falcon',
		'rbuild description ' + shooting_range_falcon,
		'rbuild sector-type INDOOR CEMENT ROOFTOP',
		's',
		'w',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Shooting Range Area Falcon',
		'rbuild description ' + shooting_range_falcon,
		'rbuild sector-type INDOOR CEMENT ROOFTOP',
		'e',
		'e',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Shooting Range Area Falcon',
		'rbuild description ' + shooting_range_falcon,
		'rbuild sector-type INDOOR CEMENT ROOFTOP',
		'w',
		'u',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Shooting Range Area Falcon',
		'rbuild description ' + shooting_range_falcon,
		'rbuild sector-type INDOOR CEMENT ROOFTOP',
		'd',
		'd',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Shooting Range Area Falcon',
		'rbuild description ' + shooting_range_falcon,
		'rbuild sector-type INDOOR CEMENT ROOFTOP',
		'u',

		go(cofob.east_exit),//'eastexit'),
		'e',
		'rbuild title {blu}C.O.F.O.B [OUTER]:{/blu} - Eastern DMZ corridor',
		'rbuild description ' + eastern_dmz_corridor,
		'rbuild texture OUTSIDE DIRT DRY DESERT AIR ROOFTOP TUNNEL FENCED', 
		'e',
		'rbuild title {blu}C.O.F.O.B [OUTER]:{/blu} - Eastern DMZ corridor',
		'rbuild description ' + eastern_dmz_corridor,
		'rbuild texture OUTSIDE DIRT DRY DESERT AIR ROOFTOP TUNNEL FENCED', 
		'e',
		'rbuild title {blu}C.O.F.O.B [OUTER]:{/blu} - Eastern DMZ corridor',
		'rbuild description ' + eastern_dmz_corridor,
		'rbuild texture OUTSIDE DIRT DRY DESERT AIR ROOFTOP TUNNEL FENCED', 
		'e',
		'rbuild title {blu}C.O.F.O.B [OUTER]:{/blu} - Fenced off DMZ corridor',
		'rbuild description ' + eastern_dmz_trap_door,
		'rbuild texture METAL_HATCH OUTSIDE DIRT DRY DESERT AIR ROOFTOP TUNNEL FENCED', 
		'e',
		'rbuild title Destroyed overpass - Abott Market South',
		'rbuild description ' + abott_market_south,
		'rbuild texture OUTSIDE DIRT DRY DESERT AIR RUBBLE STREET',
		mark(cofob.abbotstart),//'abbotstart'),
		//'n',
		//'rbuild title Destroyed overpass - Abott Market South',
		//'rbuild description ' + abott_market_south,
		//'rbuild texture OUTSIDE DIRT DRY DESERT AIR RUBBLE STREET',
		'e',
		'rbuild title Destroyed overpass - Abott Market South',
		'rbuild description ' + abott_market_south,
		'rbuild texture OUTSIDE DIRT DRY DESERT AIR RUBBLE STREET', 
		'n',
		'rbuild title Destroyed overpass - Abott Market South',
		'rbuild description ' + abott_market_south,
		'rbuild texture OUTSIDE DIRT DRY DESERT AIR RUBBLE STREET', 
		'n',
		'rbuild title Destroyed overpass - Abott Market South',
		'rbuild description ' + abott_market_south,
		'rbuild texture OUTSIDE DIRT DRY DESERT AIR RUBBLE STREET',
		repeat(9,['n',
		'rbuild title Destroyed overpass - Abott Market South',
		'rbuild description ' + abott_market_south,
		'rbuild texture OUTSIDE DIRT DRY DESERT AIR RUBBLE STREET',
		]),

		'w',
		'rbuild title Market Apartments - East entrance',
		'rbuild description ' + market_apt_east_entrance,
		'rbuild texture OUTSIDE DIRT DRY DESERT AIR',
		'w',
		'rbuild title Market Apartments - East entrance',
		'rbuild description ' + market_apt_east_entrance,
		'rbuild texture OUTSIDE DIRT DRY DESERT AIR', 


		'n',
		'rbuild title Market Apartments - Building Way',
		'rbuild description ' + market_apt_building_2,
		'rbuild texture OUTSIDE DIRT DRY DESERT AIR RUBBLE STREET', 
		'n',
		'rbuild title Market Apartments - Building Way',
		'rbuild description ' + market_apt_building_2,
		'rbuild texture OUTSIDE DIRT DRY DESERT AIR RUBBLE STREET', 
		'w',
		'rbuild title Market Apartments - Building Way',
		'rbuild description ' + market_apt_building_2,
		'rbuild texture OUTSIDE DIRT DRY DESERT AIR RUBBLE STREET', 
		'n',
		'rbuild title Market Apartments - Building 3',
		'rbuild description ' + market_apt_building_3,
		'rbuild texture OUTSIDE DIRT DRY DESERT AIR RUBBLE CEMENT', 
		'n',
		'rbuild title Market Apartments - Stairs',
		'rbuild description ' + market_apt_building_3_stairs,
		'rbuild texture OUTSIDE DIRT DRY DESERT AIR STAIRS',
		'u',
		'rbuild title Market Apartments - Stairs',
		'rbuild description ' + market_apt_building_3_stairs,
		'rbuild texture OUTSIDE DIRT DRY DESERT AIR STAIRS',
		'n',
		'rbuild title Market Apartments - Stairs',
		'rbuild description ' + market_apt_building_3_stairs,
		'rbuild texture OUTSIDE DIRT DRY DESERT AIR STAIRS',
		'u',
		'rbuild title Market Apartments - Building 3 Second floor hallway',
		'rbuild description ' + market_apt_building_3_second_floor_hallway,
		'rbuild texture OUTSIDE DIRT DRY DESERT AIR SHATTERED_GLASS',
		'n',
		'rbuild title Market Apartments - Building 3 Second floor hallway',
		'rbuild description ' + market_apt_building_3_second_floor_hallway,
		'rbuild texture OUTSIDE DIRT DRY DESERT AIR SHATTERED_GLASS',
		'w',
		'rbuild title Market Apartments - Building 3 Second floor hallway',
		'rbuild description ' + market_apt_building_3_second_floor_hallway,
		'rbuild texture OUTSIDE DIRT DRY DESERT AIR SHATTERED_GLASS',
		'w',
		'rbuild title Market Apartments - Building 3 Second floor hallway',
		'rbuild description ' + market_apt_building_3_second_floor_hallway,
		'rbuild texture OUTSIDE DIRT DRY DESERT AIR SHATTERED_GLASS',
		'w',
		'rbuild title Market Apartments - Building 3 Second floor hallway',
		'rbuild description ' + market_apt_building_3_second_floor_hallway,
		'rbuild texture OUTSIDE DIRT DRY DESERT AIR SHATTERED_GLASS',

		'n',
		'rbuild title Market Apartments - Building 3 - Unit 301',
		'rbuild description ' + market_apt_unit_301,
		'rbuild texture INSIDE TRASHY DRY DESERT AIR',
		'n',
		'rbuild title Market Apartments - Building 3 - Unit 301',
		'rbuild description ' + market_apt_unit_301,
		'rbuild texture INSIDE TRASHY DRY DESERT AIR',
		'n',
		'rbuild title Market Apartments - Building 3 - Unit 301 - Master Bedroom',
		'rbuild description ' + market_apt_unit_301_bedroom,
		'rbuild texture INSIDE TRASHY DRY DESERT AIR',
		's',
		'e',
		'rbuild title Market Apartments - Building 3 - Unit 301 - Bathroom',
		'rbuild description ' + market_apt_unit_301_bathroom,
		'rbuild texture INSIDE TRASHY DRY DESERT AIR',
		'w','s','s',

		'w',
		'rbuild title Market Apartments - Building 3 Second floor hallway',
		'rbuild description ' + market_apt_building_3_second_floor_hallway,
		'rbuild texture OUTSIDE DIRT DRY DESERT AIR SHATTERED_GLASS',
		'w',
		'rbuild title Market Apartments - Building 3 Second floor hallway',
		'rbuild description ' + market_apt_building_3_second_floor_hallway,
		'rbuild texture OUTSIDE DIRT DRY DESERT AIR SHATTERED_GLASS',
		'w',
		'rbuild title Market Apartments - Building 3 Second floor hallway',
		'rbuild description ' + market_apt_building_3_second_floor_hallway,
		'rbuild texture OUTSIDE DIRT DRY DESERT AIR SHATTERED_GLASS',

		'n',
		'rbuild title Market Apartments - Building 3 - Unit 303',
		'rbuild description ' + market_apt_unit_303,
		'rbuild texture INSIDE TRASHY DRY DESERT AIR',
		'n',
		'rbuild title Market Apartments - Building 3 - Unit 303',
		'rbuild description ' + market_apt_unit_303,
		'rbuild texture INSIDE TRASHY DRY DESERT AIR',
		'n',
		'rbuild title Market Apartments - Building 3 - Unit 303 - Master Bedroom',
		'rbuild description ' + market_apt_unit_303_bedroom,
		'rbuild texture INSIDE TRASHY DRY DESERT AIR',
		's',
		'e',
		'rbuild title Market Apartments - Building 3 - Unit 303 - Bathroom',
		'rbuild description ' + market_apt_unit_303_bathroom,
		'rbuild texture INSIDE TRASHY DRY DESERT AIR',
		'w','s','s',

		'e','e','e','e','e','e','e',
		'rbuild title Market Apartments - Building 3 Second floor hallway',
		'rbuild description ' + market_apt_building_3_second_floor_hallway,
		'rbuild texture OUTSIDE DIRT DRY DESERT AIR SHATTERED_GLASS',

		'e',
		'rbuild title Market Apartments - Building 3 Second floor hallway',
		'rbuild description ' + market_apt_building_3_second_floor_hallway,
		'rbuild texture OUTSIDE DIRT DRY DESERT AIR SHATTERED_GLASS',
		'n',
		'rbuild title Market Apartments - Building 3 - Unit 305',
		'rbuild description ' + market_apt_unit_305,
		'rbuild texture INSIDE TRASHY DRY DESERT AIR',
		'n',
		'rbuild title Market Apartments - Building 3 - Unit 305',
		'rbuild description ' + market_apt_unit_305,
		'rbuild texture INSIDE TRASHY DRY DESERT AIR',
		'n',
		'rbuild title Market Apartments - Building 3 - Unit 305 - Master Bedroom',
		'rbuild description ' + market_apt_unit_305_bedroom,
		'rbuild texture INSIDE TRASHY DRY DESERT AIR',
		's',
		'e',
		'rbuild title Market Apartments - Building 3 - Unit 305 - Bathroom',
		'rbuild description ' + market_apt_unit_305_bathroom,
		'rbuild texture INSIDE TRASHY DRY DESERT AIR',
		'w','s','s',



		'e',
		'e',
		'rbuild title Market Apartments - Building 3 Second floor hallway',
		'rbuild description ' + market_apt_building_3_second_floor_hallway,
		'rbuild texture OUTSIDE DIRT DRY DESERT AIR SHATTERED_GLASS',
		'e',
		'rbuild title Market Apartments - Building 3 Second floor hallway',
		'rbuild description ' + market_apt_building_3_second_floor_hallway,
		'rbuild texture OUTSIDE DIRT DRY DESERT AIR SHATTERED_GLASS',
		'n',
		'rbuild title Market Apartments - Building 3 - Unit 307',
		'rbuild description ' + market_apt_unit_307,
		'rbuild texture INSIDE TRASHY DRY DESERT AIR',
		'n',
		'rbuild title Market Apartments - Building 3 - Unit 307',
		'rbuild description ' + market_apt_unit_307,
		'rbuild texture INSIDE TRASHY DRY DESERT AIR',
		'n',
		'rbuild title Market Apartments - Building 3 - Unit 307 - Master Bedroom',
		'rbuild description ' + market_apt_unit_307_bedroom,
		'rbuild texture INSIDE TRASHY DRY DESERT AIR',
		's',
		'e',
		'rbuild title Market Apartments - Building 3 - Unit 307 - Bathroom',
		'rbuild description ' + market_apt_unit_307_bathroom,
		'rbuild texture INSIDE TRASHY DRY DESERT AIR',
		'w','s','s',
		'w','w','w','w',
	
		'w',
		's','d','s','d','s',

		's',
		'w',
		'rbuild title Market Apartments - Building Way',
		'rbuild description ' + market_apt_building_2,
		'rbuild texture OUTSIDE DIRT DRY DESERT AIR RUBBLE STREET', 
		'w',
		'rbuild title Market Apartments - Building Way',
		'rbuild description ' + market_apt_building_2,
		'rbuild texture OUTSIDE DIRT DRY DESERT AIR RUBBLE STREET', 
		'w',
		'rbuild title Market Apartments - Building Way',
		'rbuild description ' + market_apt_building_2,
		'rbuild texture OUTSIDE DIRT DRY DESERT AIR RUBBLE STREET', 
		'n',
		'rbuild title Market Apartments - Building 2',
		'rbuild description ' + market_apt_building_1,
		'rbuild texture OUTSIDE DIRT DRY DESERT AIR RUBBLE STREET', 
		's','e','e','e','e',
		's','s','e','e',

		'n',
		'rbuild title Abandoned Two way street - Abott Market',
		'rbuild description ' + abott_market_abandoned_street_middle,
		'rbuild texture OUTSIDE STREET DIRT DRY DESERT AIR STREET RUBBLE', 
		'n',
		'rbuild title Abandoned Two way street - Abott Market',
		'rbuild description ' + abott_market_abandoned_street_middle,
		'rbuild texture OUTSIDE STREET DIRT DRY DESERT AIR STREET RUBBLE', 
		'n',
		'rbuild title Abandoned Two way street - Abott Market',
		'rbuild description ' + abott_market_abandoned_street_middle,
		'rbuild texture OUTSIDE STREET DIRT DRY DESERT AIR STREET RUBBLE', 
		'n',
		'rbuild title Abandoned Two way street - Abott Market',
		'rbuild description ' + abott_market_abandoned_street_middle,
		'rbuild texture OUTSIDE STREET DIRT DRY DESERT AIR STREET RUBBLE', 
		'n',
		'rbuild title Abandoned Two way street - Abott Market',
		'rbuild description ' + abott_market_abandoned_street_middle,
		'rbuild texture OUTSIDE STREET DIRT DRY DESERT AIR STREET RUBBLE', 
	]);
	do_cofob_west_area_from('cofobcenter');
	//go(cofob.gear_room);
	//do_range_locker();
	
	do_mess_hall();
	run(['rbuild pave off']);
	//run([go('waypoint-car-garage-north-1')]);
}

/**
 * ================================================================================
 * ================================================================================
 * ================================================================================
 * ================================================================================
 */
















/**
 * ================================================================================
 * ================================================================================
 * || SECTION: 
 * ||         S H I P Y A R D
 * ||					C O N T E N T
 * ||
 * ================================================================================
 * ================================================================================
 * v-shipyward v-content
 */
var shipyard = {
	row_a: 'shipyard-row-a',
};
function do_shipyard_area(){
	var fm = '@FILL_ME@';

	var crenshaw_highway_overpass = [
		"A newly paved road stretching to the east and curving towards the ",
		"north. Speed limit signs are present but they aren't taken seriously ",
		"by the civilians. Admittedly, the only residents that use the ",
		"highways are the ones with disproportionate amounts of wealth. ",
		"You notice a few carcasses of dead animals that made the dire ",
		"decision to cross this hellish landscape."
	].join('');
	var crenshaw_highway_northbound = [
		"The ramp curves to the north. To the east you see a long stretch of highway ",
		"that disappears over the horizon. You see a shipyard far off to the northeast. ",
	].join('');
	var crenshaw_highway_94_east = [
		"Six lanes of traffic move for off to the east as far as the eye can see. ",
		"You see hills of varying heights hugging the highway's north and south sides. ",
		"There is sparse greenery along the freeway. "
	].join('');
	var shipyard_entrance = [
		"Shipping containers stacked 30 to 40 stories high blot out the sun's rays coming from the ",
		"east and even overhead during lunch time. Despite the time of day, artificial lighting is needed ",
		"everywhere. As you make your way deeper into the shipyard, ",
		"you notice several highly armed individuals patrolling the area. These individuals are ",
		"wearing masks and have extensive radio communication devices that are resistant to E.M.P.. ",
	].join('');
	var shipyard_shipment_row_a = [
		"All around you are red and blue shipping containers with varying amounts of rust and ",
		"outer wear. A few have severe indentations from mishaps. Oddly enough, containers with ",
		"giant abrasions are bent back into shape and reused. You notice what seems to be burn marks, ",
		"bullet holes, and dark splattered brownish red stains on a few containers. ",
	].join('');

	run([
		repeat(2,['n']),
		repeat(2,['e']),
		repeat(5,['n']),
		repeat(5,['e']),
		'n',
		'rbuild pave continue',
		'e',
		title('{blu}Crenshaw{/blu} Highway overpass'),
		desc(crenshaw_highway_overpass),
		texture('OUTSIDE HIGHWAY DRY DESERT AIR'),
		mark(crenshaw.overpass_start),

		repeat(3,['e',
			title('{blu}Crenshaw{/blu} Highway overpass - Ramp'),
			desc(crenshaw_highway_overpass),
			texture('OUTSIDE HIGHWAY DRY DESERT AIR'),
		]),
		repeat(3,['n',
			title('{blu}Crenshaw{/blu} Highway overpass - North'),
			desc(crenshaw_highway_northbound),
			texture('OUTSIDE HIGHWAY DRY DESERT AIR'),
		]),
		repeat(3,['e',
			title('{blu}Crenshaw{/blu} Highway 94 East'),
			desc(crenshaw_highway_94_east),
			texture('OUTSIDE HIGHWAY DRY DESERT AIR'),
		]),
		repeat(30,['e',
			title('{blu}Crenshaw{/blu} Highway 94 East'),
			desc(crenshaw_highway_overpass),
			texture('OUTSIDE HIGHWAY DRY DESERT AIR'),
		]),
		mark(crenshaw.northern_shipping_area_entrance),
		repeat(3,['n',
			title('{blu}Shipyard{/blu} Entrance'),
			desc(shipyard_entrance),
			texture('OUTSIDE DRY DESERT AIR'),
		]),
		mark(shipyard.row_a),
		repeat(2,['e',
			title('{blu}Shipyard{/blu} Shipment Row A'),
			desc(shipyard_shipment_row_a),
			texture('OUTSIDE HIGHWAY DRY DESERT AIR'),
		]),
		repeat(2,['n',
			title('{blu}Shipyard{/blu} Shipment Row A'),
			desc(shipyard_shipment_row_a),
			texture('OUTSIDE HIGHWAY DRY DESERT AIR'),
		]),
		repeat(2,['e',
			title('{blu}Shipyard{/blu} Shipment Row A'),
			desc(shipyard_shipment_row_a),
			texture('OUTSIDE HIGHWAY DRY DESERT AIR'),
		]),
		repeat(2,['w']),
		repeat(2,['s']),
		repeat(2,['w']),

		go(shipyard.row_a),

		repeat(2,['w',
			title('{blu}Shipyard{/blu} Shipment Row A'),
			desc(shipyard_shipment_row_a),
			texture('OUTSIDE HIGHWAY DRY DESERT AIR'),
		]),
		repeat(2,['n',
			title('{blu}Shipyard{/blu} Shipment Row A'),
			desc(shipyard_shipment_row_a),
			texture('OUTSIDE HIGHWAY DRY DESERT AIR'),
		]),
		repeat(2,['w',
			title('{blu}Shipyard{/blu} Shipment Row A'),
			desc(shipyard_shipment_row_a),
			texture('OUTSIDE HIGHWAY DRY DESERT AIR'),
		]),
		go(shipyard.row_a),
		go(crenshaw.northern_shipping_area_entrance),
		repeat(30,['e',
			title('{blu}Crenshaw{/blu} Highway 94 East'),
			desc(crenshaw_highway_overpass),
			texture('OUTSIDE HIGHWAY DRY DESERT AIR'),
		]),
		/** end of eastern section */
	]);

	run(['rbuild pave off']);
}
var vertical_slice = {
	mf13_hoodlum: function(index){
		/** TODO grunt work: convert to auto_feed_mob format */
		var equip = 'rifle/mac-10.yml{magazine:container/hv-mp-ammo.yml}';
		var mf13h_vnum = '4000';
			cmd(["mbuild attr ",index," virt ",mf13h_vnum].join(""));
			cmd(["mbuild attr ",index," mana ",25].join(""));
			cmd(["mbuild attr ",index," max_mana ",25].join(""));
			cmd(["mbuild attr ",index," hit ",580].join(""));
			cmd(["mbuild attr ",index," max_hit ",580].join(""));
			cmd(["mbuild attr ",index," move ",140].join(""));
			cmd(["mbuild attr ",index," max_move ",240].join(""));
			cmd(["mbuild attr ",index," damroll ",15].join(""));
			cmd(["mbuild attr ",index," gold ",5000].join(""));
			cmd(["mbuild attr ",index," level ",10].join(""));
			cmd(["mbuild attr ",index," weight ",10].join(""));
			cmd(["mbuild attr ",index," height ",4].join(""));
			cmd(["mbuild attr ",index," strength ",15].join(""));
			cmd(["mbuild attr ",index," strength_add ",13].join(""));
			cmd(["mbuild attr ",index," intelligence ",12].join(""));
			cmd(["mbuild attr ",index," dexterity ",14].join(""));
			cmd(["mbuild attr ",index," constitution ",18].join(""));
			cmd(["mbuild attr ",index," charisma ",1].join(""));
			cmd(["mbuild attr ",index," damnodice ",14].join(""));
			cmd(["mbuild attr ",index," damsizedice ",16].join(""));
			cmd(["mbuild attr ",index," name \"mf13 mf-13 hoodlum hood thug gangster\" "].join(""));
			cmd(["mbuild attr ",index," sex MALE "].join(""));
			cmd(["mbuild attr ",index," action AGGRESSIVE ISNPC"].join(""));
			var short_desc = [
				'{gld}MF-13 Hoodlum{/gld}'
			].join('');
			cmd(["mbuild attr ",index," short_description \"", short_desc, "\" "].join(""));
			var long_desc = [
				'{gld}MF-13 Hoodlum{/gld}'
			].join('');
			var description = [
				'Blue jeans, white tank top, and enough tattoos to intimidate any sane ',
				'citizen. The man standing before you is perpetually angry at anyone ',
				'who even shows the slightest bit of eye contact.',
			].join('');
			cmd(["mbuild attr ",index," long_description \"", long_desc, "\" "].join(""));
			cmd(["mbuild attr ",index," description \"", description, "\" "].join(""));
			cmd(["mbuild save ",index].join(""));
			return 0; 
		},
	mf13_hoodlum_initiate: function(index){
		/** TODO grunt work: convert to auto_feed_mob format */
		var equip = 'melee/sig-sauer.yml';
		var mf13h_vnum = '4001';
			cmd(["mbuild attr ",index," virt ",mf13h_vnum].join(""));
			cmd(["mbuild attr ",index," mana ",15].join(""));
			cmd(["mbuild attr ",index," max_mana ",15].join(""));
			cmd(["mbuild attr ",index," hit ",280].join(""));
			cmd(["mbuild attr ",index," max_hit ",280].join(""));
			cmd(["mbuild attr ",index," move ",40].join(""));
			cmd(["mbuild attr ",index," max_move ",40].join(""));
			cmd(["mbuild attr ",index," damroll ",5].join(""));
			cmd(["mbuild attr ",index," gold ",5000].join(""));
			cmd(["mbuild attr ",index," level ",10].join(""));
			cmd(["mbuild attr ",index," weight ",10].join(""));
			cmd(["mbuild attr ",index," height ",4].join(""));
			cmd(["mbuild attr ",index," strength ",5].join(""));
			cmd(["mbuild attr ",index," strength_add ",3].join(""));
			cmd(["mbuild attr ",index," intelligence ",2].join(""));
			cmd(["mbuild attr ",index," dexterity ",4].join(""));
			cmd(["mbuild attr ",index," constitution ",8].join(""));
			cmd(["mbuild attr ",index," charisma ",1].join(""));
			cmd(["mbuild attr ",index," damnodice ",4].join(""));
			cmd(["mbuild attr ",index," damsizedice ",6].join(""));
			cmd(["mbuild attr ",index," name \"initiate hooded\" "].join(""));
			cmd(["mbuild attr ",index," sex MALE "].join(""));
			cmd(["mbuild attr ",index," action AGGRESSIVE ISNPC"].join(""));
			var short_desc = [
				'{gld}MF-13 Hooded Initiate{/gld}'
			].join('');
			cmd(["mbuild attr ",index," short_description \"", short_desc, "\" "].join(""));
			var long_desc = [
				'{gld}MF-13 Hooded Initiate{/gld}'
			].join('');
			var description = [
				'The danger of the individual you are viewing is amplified in the context ',
				'that this thug needs to impress the likes of the higher ranked thugs. ',
				'The man standing before is smoking PCP.',
			].join('');
			cmd(["mbuild attr ",index," long_description \"", long_desc, "\" "].join(""));
			cmd(["mbuild attr ",index," description \"", description, "\" "].join(""));
			cmd(["mbuild save ",index].join(""));
			return 0; 
		},
		harmless_citizen: function(index){
		/** TODO grunt work: convert to auto_feed_mob format */
		var harmless_vnum = '4002';
			cmd(["mbuild attr ",index," virt ",harmless_vnum].join(""));
			cmd(["mbuild attr ",index," mana ",5].join(""));
			cmd(["mbuild attr ",index," max_mana ",5].join(""));
			cmd(["mbuild attr ",index," hit ",40].join(""));
			cmd(["mbuild attr ",index," max_hit ",40].join(""));
			cmd(["mbuild attr ",index," move ",10].join(""));
			cmd(["mbuild attr ",index," max_move ",10].join(""));
			cmd(["mbuild attr ",index," damroll ",2].join(""));
			cmd(["mbuild attr ",index," gold ",10].join(""));
			cmd(["mbuild attr ",index," level ",3].join(""));
			cmd(["mbuild attr ",index," weight ",10].join(""));
			cmd(["mbuild attr ",index," height ",4].join(""));
			cmd(["mbuild attr ",index," strength ",2].join(""));
			cmd(["mbuild attr ",index," strength_add ",1].join(""));
			cmd(["mbuild attr ",index," intelligence ",2].join(""));
			cmd(["mbuild attr ",index," dexterity ",1].join(""));
			cmd(["mbuild attr ",index," constitution ",2].join(""));
			cmd(["mbuild attr ",index," charisma ",1].join(""));
			cmd(["mbuild attr ",index," damnodice ",1].join(""));
			cmd(["mbuild attr ",index," damsizedice ",1].join(""));
			cmd(["mbuild attr ",index," name \"harmless citizen\" "].join(""));
			cmd(["mbuild attr ",index," sex FEMALE "].join(""));
			cmd(["mbuild attr ",index," action WIMPY ISNPC"].join(""));
			var short_desc = [
				'Harmless citizen'
			].join('');
			cmd(["mbuild attr ",index," short_description \"", short_desc, "\" "].join(""));
			var long_desc = [
				'Harmless citizen'
			].join('');
			var description = [
				'An old and decrepit citizen is here trying to mind her own business...'
			].join('');
			cmd(["mbuild attr ",index," long_description \"", long_desc, "\" "].join(""));
			cmd(["mbuild attr ",index," description \"", description, "\" "].join(""));
			cmd(["mbuild save ",index].join(""));
			return 0; 
		},
		la_posta_chef: function(index){
		var la_posta_chef_vnum = '4003';
		/** TODO grunt work: convert to auto_feed_mob format */
			cmd(["mbuild attr ",index," virt ",la_posta_chef_vnum].join(""));
			cmd(["mbuild attr ",index," mana ",8].join(""));
			cmd(["mbuild attr ",index," max_mana ",8].join(""));
			cmd(["mbuild attr ",index," hit ",110].join(""));
			cmd(["mbuild attr ",index," max_hit ",110].join(""));
			cmd(["mbuild attr ",index," move ",15].join(""));
			cmd(["mbuild attr ",index," max_move ",15].join(""));
			cmd(["mbuild attr ",index," damroll ",4].join(""));
			cmd(["mbuild attr ",index," gold ",20].join(""));
			cmd(["mbuild attr ",index," level ",5].join(""));
			cmd(["mbuild attr ",index," weight ",10].join(""));
			cmd(["mbuild attr ",index," height ",4].join(""));
			cmd(["mbuild attr ",index," strength ",4].join(""));
			cmd(["mbuild attr ",index," strength_add ",9].join(""));
			cmd(["mbuild attr ",index," intelligence ",9].join(""));
			cmd(["mbuild attr ",index," dexterity ",9].join(""));
			cmd(["mbuild attr ",index," constitution ",9].join(""));
			cmd(["mbuild attr ",index," charisma ",1].join(""));
			cmd(["mbuild attr ",index," damnodice ",2].join(""));
			cmd(["mbuild attr ",index," damsizedice ",5].join(""));
			cmd(["mbuild attr ",index," name \"la posta chef\" "].join(""));
			cmd(["mbuild attr ",index," sex MALE "].join(""));
			cmd(["mbuild attr ",index," action STAY_ZONE HELPER AGGR_GOOD ISNPC"].join(""));
			var short_desc = [
				'La Posta Chef',
			].join('');
			cmd(["mbuild attr ",index," short_description \"", short_desc, "\" "].join(""));
			var long_desc = [
				'La Posta Chef',
			].join('');
			var description = [
				'A middle-aged man is standing here preparing food using a dull knife. ',
				'He just wants to finish his shift and get home to his wife and children.'
			].join('');
			cmd(["mbuild attr ",index," long_description \"", long_desc, "\" "].join(""));
			cmd(["mbuild attr ",index," description \"", description, "\" "].join(""));
			cmd(["mbuild save ",index].join(""));
			return 0; 
		},
	toughened_teenager: function(index){
		/** TODO grunt work: convert to auto_feed_mob format */
		var toughened_teen_vnum = '4004';
			cmd(["mbuild attr ",index," virt ",toughened_teen_vnum].join(""));
			cmd(["mbuild attr ",index," mana ",25].join(""));
			cmd(["mbuild attr ",index," max_mana ",25].join(""));
			cmd(["mbuild attr ",index," hit ",280].join(""));
			cmd(["mbuild attr ",index," max_hit ",280].join(""));
			cmd(["mbuild attr ",index," move ",100].join(""));
			cmd(["mbuild attr ",index," max_move ",100].join(""));
			cmd(["mbuild attr ",index," damroll ",10].join(""));
			cmd(["mbuild attr ",index," gold ",50].join(""));
			cmd(["mbuild attr ",index," level ",6].join(""));
			cmd(["mbuild attr ",index," weight ",10].join(""));
			cmd(["mbuild attr ",index," height ",4].join(""));
			cmd(["mbuild attr ",index," strength ",10].join(""));
			cmd(["mbuild attr ",index," strength_add ",10].join(""));
			cmd(["mbuild attr ",index," intelligence ",27].join(""));
			cmd(["mbuild attr ",index," dexterity ",24].join(""));
			cmd(["mbuild attr ",index," constitution ",20].join(""));
			cmd(["mbuild attr ",index," charisma ",10].join(""));
			cmd(["mbuild attr ",index," damnodice ",10].join(""));
			cmd(["mbuild attr ",index," damsizedice ",6].join(""));
			cmd(["mbuild attr ",index," name \"toughened inner city teen\" "].join(""));
			cmd(["mbuild attr ",index," sex MALE "].join(""));
			cmd(["mbuild attr ",index," action AGGRESSIVE ISNPC"].join(""));
			var short_desc = [
				'Toughened inner-city teenager',
			].join('');
			cmd(["mbuild attr ",index," short_description \"", short_desc, "\" "].join(""));
			var long_desc = [
				'Toughened inner-city teenager',
			].join('');
			var description = [
				'Cast out by the system that failed him, this young kid has so much potential ',
				'just so long as he stays out of trouble (an impossible task given his current ',
				'environment). Behind that tough facade is a kid who wants to overcome this city.',
			].join('');
			cmd(["mbuild attr ",index," long_description \"", long_desc, "\" "].join(""));
			cmd(["mbuild attr ",index," description \"", description, "\" "].join(""));
			cmd(["mbuild save ",index].join(""));
			return 0; 
		},
		bootstrap_mobs: function(){
			send('creating mf13 hoodlum...');
			vertical_slice.mf13_hoodlum(5);
			send('done');
			send('creating mf13 initiate...');
			vertical_slice.mf13_hoodlum_initiate(6);
			send('done');
			send('creating harmless citizen...');
			vertical_slice.harmless_citizen(7);
			send('done');
			send('creating la posta chef...');
			vertical_slice.la_posta_chef(8);
			send('done');
			send('creating toughened teen...');
			vertical_slice.toughened_teenager(9);
			send('done');
		},
};//end vertical_slice
/**
 * ================================================================================
 * ================================================================================
 * ================================================================================
 * ================================================================================
 */


















/**
 * ================================================================================
 * ================================================================================
 * || SECTION: 
 * ||         R I F L E  A T T A C H M E N T S 
 * ||					C O N T E N T
 * ||
 * ================================================================================
 * ================================================================================
 */
function do_rifle_attachment_tests(){
	cmd('room_dark off');
	cmd('instantiate_rifle_attachment rifle/g36c.yml{barrel:silencer.yml,grip:hexon-grip.yml,magazine:extended-ar-mag.yml,muzzle:flash-hider.yml,sight:reflex-sight.yml,stock:titanium-stock.yml,strap:tactical-strap.yml,under_barrel:frag-underbarrel.yml}');
	cmd('examine g36c');
	cmd('give g36c sniper');
}
/**
 * ================================================================================
 * ================================================================================
 * ================================================================================
 * ================================================================================
 */

















/**
 * ================================================================================
 * ================================================================================
 * || SECTION: 
 * ||         P R O C E D U R A L  G E N E R A T I O N
 * ||					C O N T E N T
 * ||
 * ================================================================================
 * ================================================================================
 * v-content v-procgen v-procedural-generation
 */
function do_random_area(){
	var start = -400;
	cmd('recall');
	set_points('far','move',10000);
	cmd(['generate_zone 200 ',start,' -400 -400'].join(''));
	cmd('recall');
	set_points('far','move',10000);
	start = -500;
	cmd(['generate_zone 200 ',start,' -400 -400'].join(''));
	cmd('recall');
	set_points('far','move',10000);
	start = -600;
	cmd(['generate_zone 200 -400 ',start,' -400'].join(''));
}
/**
 * ================================================================================
 * ================================================================================
 * ================================================================================
 * ================================================================================
 */














function phase1(){
	do_cobalt_fob();
	send([
		"When you're ready, do a {grn}rbuild save-paved 0{/grn} to save the rooms you just paved\r\n",
		"I've placed all the above instructions in a helper function:\r\n",
		"Simply call: {grn}js phase2();{/grn}\r\n"
	].join(''));
}
function phase2(){
	run([
		'rbuild save-paved 0'
	]);
	clear_zone_data_entries();
	do_triton_lab();
	do_ballistics_lab();
	build_hv_mag_contract();
	do_triton_scripted_content();
	do_waypoint_avenue_stalkers();
	cobalt_military_police();
}

function main(){
	funcs.always();
}

main();
