function build_sniper(index){
	cmd('obuild attr ' + index + ' short_description "a deadly XM109 sniper rifle"');
	cmd('obuild flag ' + index + ' type_flags WEAPON');
	cmd('obuild flag ' + index + ' wear_flags TAKE WIELD');
	cmd('obuild attr ' + index + ' weapon_type:add SNIPE');
	cmd('obuild save ' + index);
}
function test1(){
	cmd("+impl");
	cmd("+god");
	cmd("+build");
	build_sniper(4);
	cmd('mbuild instantiate 0');
}
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
function training_course_drone_assisted_sniping(){
	var rooms = {
		'courtyard' : {
				'title': 'Outer training courtyard - D.M.Z 0x4155',
				'description': [
					"You stand in a square shaped courtyard amongs other new recruits.",
					"You can smell the reek of sweat and dirt everywhere you go. {yel}Corporal ",
					"{yel}James Taggart{/yel} stands before you: a menacing authoritative presence.",
					"There is a heavy steel door to the {grn}North{/grn}.",
				].join("\r\n"),
				'objects': [
					'training-chooser'
				],
				'npcs': [
					'corp-james-taggart'
				]
			},
	};
//		/** MCMAP Weapon Disarming */
//		[
//			"Learn to disarm your opponent.",
//			"(10% chance of taking primary weapon from enemy)",
//			/** TODO */
//		].join(''),
//		/** Chemical Enhancement Engineering */
//		[
//			/** TODO */
//			"Learn to craft your own synthetic anabolic steroids to increase damage output.",
//			"Learn to fuse multiple substances and gain the benefits of a plethora of enhancements.",
//			"Learn to utilize medicinal herbs found in the wild to heal and patch wounds.",
//			"Craft substances to reduce the negative effects of performance enhancing drugs."
//		].join(''),
//		/** Ranged and CQB Transitioning */
//		[
//			/** TODO */
//			"Ranged and CQB transitioning"
//		].join('')
//		/** Recon Gadgetry - Area Scanning */
//		[
//			/** TODO */
//			"Recon Gadgetry"
//		].join('')
//		/** Recon Gadgetry - Autonomous Drone Scanning */
//		[
//			/** TODO */
//			"Recon Gadgetry - Autonomous Drone Scanning"
//		].join('')
	var course_install_commands = [
		'install_computer_choice "Training Computer"',
		'add_computer_choice 1 "Drone assisted Sniping Course" "start_course"',
		'add_computer_choice 2 "Demolitions Course" "start_course"',
		'add_computer_choice 3 "Tactical Gadgetry Course" "start_course"',
		'add_computer_choice 4 "Marine Corps Martial Arts Program - {grn}Striking Clinic{/grn}" "start_course"',
		'add_computer_choice 5 "Marine Corps Martial Arts Program - {grn}Judo Trips and Throws{/grn}" "start_course"',
		'add_computer_choice 6 "Marine Corps Martial Arts Program - {grn}Submission Grappling{/grn}" "start_course"',
		//TODO 'add_computer_choice 7 "Marine Corps Martial Arts Program - {grn}Weapon Disarming{/grn}" "start_course"',
		//TODO 'add_computer_choice 8 "Chemical Enhancement Engineering - {grn}{/grn}" "start_course"',
		//TODO 'add_computer_choice 9 "Ranged and C.Q.B. Transitioning" "start_course"',
		//TODO 'add_computer_choice 10 "Recon Gadgetry - {grn}Area Scanning{/grn}" "start_course"',
		//TODO 'add_computer_choice 11 "Recon Gadgetry - {grn}Autonomous Drone Scanning{/grn}" "start_course"',
		];
	var course_text = [
		[
			"Drone assisted sniping HOW TO:\r\n",
			"==============================\r\n",
			"Step 1) Deploy your drone.\r\n",
			"run the command: ", action('deploy drone'), "\r\n",
			"You will now see the world through the eyes of your drone.\r\n",
			"\r\n",
			"Step 2) Fly or ground travel to the target room.\r\n",
			"run any direction commands to get to the room where your target enemy is. (north, east, south, etc)...\r\n",
			"\r\n",
			"Step 3) Target your enemy.\r\n",
			"run the command: ", action('target ENEMY'), " where 'ENEMY' is the name of the mob.\r\n",
			"\r\n",
			"Step 4) Snipe your target.\r\n",
			"type this exactly to snipe your enemy: ", action('snipe target'), ". If you try to use \r\n",
			"any other phrase other than target, the command will not work.\r\n",
			"\r\n",
		].join(''),
		[
			"Demolitions HOW TO: Section: GRENADES\r\n",
			"=====================================\r\n",
			"Step 1) Hold your grenade\r\n",
			"run the command: ", action('hold grenade'), "\r\n",
			"\r\n",
			"Step 2) Throw grenade.\r\n",
			"run the command: ", action('throw grenade DISTANCE'), "\r\n",
			"{grn}replace the phrase DISTANCE with the number of rooms you would like to throw.{/grn}\r\n",
			"\r\n",
			"\r\n",

			"Demolitions HOW TO: Section: C4\r\n",
			"===========================================\r\n",
			"Step 1) Attach the C4 to an object, door, person, or vehicle.\r\n",
			"run the command: ", action('attach c4 ITEM'), "\r\n",
			"{grn}replace the phrase ITEM with the target object, person, vehicle, or direction.{/grn}\r\n",
			"\r\n",
			"Step 2) Detonate the C4.\r\n",
			"run the command: ", action('detonate'), "\r\n",
			"\r\n",
			"\r\n",

			"Demolitions HOW TO: Section: Claymore Mines\r\n",
			"===========================================\r\n",
			"Step 1) Install the claymore mine at the specified door.\r\n",
			"run the command: ", action('install claymore DIRECTION'), "\r\n",
			"{grn}replace the phrase DISTANCE with the direction of the door you would like to install the claymore mine on.{/grn}\r\n",
			"\r\n",
		].join(''),
	];

	run(['rbuild pave continue', 'w', 'w',
		['rbuild title "',rooms.courtyard.title,'"'].join(''),
		['rbuild description "',rooms.courtyard.description,'"'].join(''),
	]);
}
//var init = {
//	/** TODO: it'd be *really* nice to have an NPC drill sergeant to say all this */
//	phases: [
//		{'title': 'Introduction',
//			'description': ["Welcome to :[SIEGE MUD]:. Before you can take part in the ",
//				"fun that is Siege MUD, you will have to run through a series of tests. ",
//				"Complete these tests and you will be accepted into the world and placed ",
//				"into the malestrom of counter-terrorist operations. The enemy knows no ",
//				"limits. We must stay vigilant. Walk {grn}NORTH{/grn} to proceed. "].join(''),
//			'textures': ['CEMENT','OUTSIDE']
//		},
//		{
//			'title': 'Basic Movement',
//			'description': ["Siege MUD is based off of the CircleMud codebase, however, ",
//				"we have made lots of additions. The basic movements available to you are ",
//				dirhl('up'), ',', dirhl('down'),',', dirhl('north'),',', 
//				dirhl('south'),',',
//				dirhl('east'),', and ',
//				dirhl('west'),'. ', "To the {yel}west{/yel} of here is a {yel}weapons ",
//				"stockpile{/yel}. Go west and follow the instructions there to complete ",
//				"your first task. "].join(''),
//				'textures': ['CEMENT','OUTSIDE'],
//			'always': function(player){
//			},
//			'quest': function(){
//			}
//		},
//	],
//	pave_layout: function(){
//		cmd('rbuild_sandbox new initiation 1');
//		cmd('rbuild pave on 1 1');
//		cmd('rbuild texture CEMENT OUTSIDE');
//		td(0);
//		cmd('north');
//		cmd('north');
//		td(1);
//		cmd('north');
//		cmd('north');
//		td(2);
//	}
//};
//

function frag(){
	for(var i=0;i < 10;i++){
		cmd("giveme_frag");
	}
	cmd("hold frag");
	cmd("throw north 1");
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
function do_sensor_nade_with_range_modifier(){
	cmd('n');
	cmd('n');
	cmd('n');
	cmd('mbuild instantiate 0');
	cmd('s');
	cmd('s');
	cmd('s');
	set_points('far','level',26);
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
//yaml_example();
//yaml_import();
//do_sensor_nade();
//do_camera();

var grim_vnum = '101';

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
function do_null_weapon_test(){
	cmd('n');
	cmd('n');
	cmd('n');
	cmd('mbuild instantiate 0');
	cmd('s');
	cmd('s');
	cmd('snipe l north');
}
function do_rifle_weapon_test(){
	cmd('n');
	cmd('n');
	cmd('n');
	cmd('mbuild instantiate 0');
	cmd('s');
	cmd('s');
	cmd('yaml_import rifle psg1.yml');
	cmd('wield psg');
	cmd('s');
	cmd('snipe l north');
}
function do_flush_feed_test(){
	cmd('yaml_import rifle g36c.yml');
	cmd('wield g36c');
	cmd('yaml_import rifle p90.yml');
	cmd('flush_player');
	cmd('feed_player');
}
function run(commands){
	for(var i=0;i < commands.length;i++){
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
function do_revive_test(){
	run(['mbuild instantiate 0', 'set_npc_position 3 INCAP', 'revive chef' ]);
}
function do_send_report(){
	run(['mbuild instantiate 0', 'send_report']);
}
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
function do_chat(){
	run(['chat lol']);
}
function do_giveme_g36c(){
	cmd('yaml_import rifle g36c.yml');
}
function do_get_noticed(){
	run(['n','n','e','e','w','w','s','s']);
}
function do_value_system_sanity_check(value){
	value_save('SANITY_CHECK',value);
	value_load('SANITY_CHECK');
	return value_sanity_check();
}
function do_server_room_area(){
	var server_room_a155_description = [
		"The air is surprisingly cool given the amount of server modules ",
		"pumping heated air into the vicinity. Dozens of rack modules are ",
		"working feverishly. Fans built into the ceiling and floor are the ",
		"only way this room could possibly not be over one hundred degrees ",
		"farenheit. There are atleast a couple hundred ethernet cables running ",
		"chaotic routes into and out of each rack module. It's dizzying and complex. ",
		"You're just happy that you're not the maniac who has to maintain ",
		"the order here."
	].join('');
	var server_room_row_zero_middle = [
		"A break in the monotony of server rack modules, but only for the ",
		"inclusion of a series of desks stretching east and west. ",
		"Despite the empty chairs, each computer's CPU can be heard ",
		"running at maximum capacity. Whoever runs these machines knows ",
		"exactly what they're doing. A few desks here have a fresh cup of coffee, ",
		"meaning someone was just here minutes ago..."
	].join('');
	var server_room_row_zero_west_desk_0 = [
		"The name on this desk says 'P. Manning'. Patrick Manning: the ",
		"one in charge of gateway operations. Most recently, Patrick was ",
		"seen tending a few of the rack modules in the 0xA155 server room. ",
		"A few stickers decorate the outside of his P.C. case. A few ",
		"Tengu demons can be seen on the case. ",
		"{yel}The display says 'Login:'{/yel} with a blinking square.",
	].join('');
	var server_room_row_zero_west_desk_1 = [
		"The name on this desk says 'E. Hatchet'. Edward Hatchet: the ",
		"grunt that made it to operations through hard work and determination. ",
		"Ed made a name for himself by having a solid work ethic, and a ",
		"talent for reading people. Having survived several deployments to ",
		"Iraq and Afghanistan, Ed is a valuable asset with loads of experience.",
		"{yel}The display says 'Login:'{/yel} with a blinking square."
	].join('');
	var server_room_row_zero_west_desk_2 = [
		"The desk name plate has nothing on it. The desk doesn't appear to ",
		"belong to anyone. As with all desks, this computer has a login screen. ",
		"Usually, with a constantly evolving personnel lineup, terminals are ",
		"typically occupied by new recruits until they are shifted to other ",
		"responsibilities. It's uncommon to have a recruit who doesn't move ",
		"his or her desk after a few months on-duty. ",
		"{yel}The display says 'Login:'{/yel} with a blinking square."
	].join('');
	var server_room_row_zero_east_desk_0 = [
		"The desk name plate has nothing on it. The desk doesn't appear to ",
		"belong to anyone. As with all desks, this computer has a login screen. ",
		"Usually, with a constantly evolving personnel lineup, terminals are ",
		"typically occupied by new recruits until they are shifted to other ",
		"responsibilities. It's uncommon to have a recruit who doesn't move ",
		"his or her desk after a few months on-duty. ",
		"{yel}The display says 'Login:'{/yel} with a blinking square."
	].join('');
	var server_room_row_zero_east_desk_1 = [
		"A spent casing is laying on the desk, which is your first red flag. ",
		"The name plate says 'I. Briggs'. The infamous Ian Briggs, an incredibly ",
		"gifted and controversial hacker. But you didn't hear that from us. ",
		"An upside down star is seen on the display, along with a red and black text that says {red}'Baphomet OS'{/red}.",
		"Ian is known to run his own custom operating systems. Perhaps that's what ",
		"he's named this build."
	].join('');
	var server_room_row_zero_east_desk_2 = [
		"The desk name plate has nothing on it. The desk doesn't appear to ",
		"belong to anyone. As with all desks, this computer has a login screen. ",
		"Usually, with a constantly evolving personnel lineup, terminals are ",
		"typically occupied by new recruits until they are shifted to other ",
		"responsibilities. It's uncommon to have a recruit who doesn't move ",
		"his or her desk after a few months on-duty. ",
		"{yel}The display says 'Login:'{/yel} with a blinking square."
	].join('');
	var server_room_south_row_one = [
		'A vacuum sealed room rudely greets you with stuffy ears. Had it been ',
		'any less of a surprise to your senses, it may have been tolerable. This room ',
		'feels like you are sitting in a pressurized airplane cabin. The only escape ',
		'is either north or south in the respective door.'
	];
	var server_room_south_row_one_west = [
		''
	];
	var server_room_south_entrance = [
		"A heavily fortified yet elegant looking door stands between the ",
		"outside world and the server room you see before you. There are ",
		"hidden biometric scanners and infrared cameras focused on your ",
		"every move. You don't know this because you can see these devices. ",
		"You know this because the higher ups have told you. You wouldn't ",
		"try anything stupid, now, would you?"
	].join('');
 	var commons_hallway_server_room_bio_metrics_scanning = [
		"Two incredibly thick pieces of glass on the east and west walls ",
		"supply the overpowered sense of being watched. The claustrophobic ",
		"fear of having your every move scrutinized. Anxiety tickles your spine, ",
		"a cold sweat emerges on your forehead. As you walk through, a lean and ",
		"subtle mechanical process of thousands of motors move various infrared ",
		"cameras. Gait recognition software makes it nearly impossible for you to ",
		"masquerade as anybody else."
	].join('');
	var commons_hallway_0 = [
		"Ambient noise is one thing that sets the tone of this room apart from ",
		"the rest of the internals building. A more lax and energetic feel is ",
		"projected through mounted speakers. Automated recon reports can be seen ",
		"along the hallway monitors adorning the walls to the north. You edge by ",
		"preocupied personnel on their way to their designated workstations."
	].join('');
	var commons_hallway_n1 = commons_hallway_0;
	var commons_hallway_n2 = commons_hallway_0;
	var commons_hallway_n3 = commons_hallway_0;
	var commons_restroom_male = [
		'This bathroom has been scrubbed clean of nearly any visible bacteria. ',
		'You are quite impressed with how sterile this bathroom is.',
		'The tiled floor is immaculately clean. The walls are a drab hue of ',
		'egg shell yellow. A sign reads "Employees must wash hands before returning to work".',
		'There is a sink with a slightly open faucet causing water to drip with almost ',
		'metronome accuracy.'
	].join('');
	var commons_restroom_female = [
		'This bathroom has been scrubbed clean of nearly any visible bacteria. ',
		'You are quite impressed with how sterile this bathroom is.',
		'The tiled floor is immaculately clean. The walls are a drab hue of ',
		'egg shell yellow. A sign reads "Employees must wash hands before returning to work".',
		'There is a sink with a slightly open faucet causing water to drip with almost ',
		'metronome accuracy.'
	].join('');
	var commons_elevator_unit_a = [
		'"N-Level Clearance", reads the sign above this elevator entrance. ',
		'A keycard slot is installed just below the circular buttons used to ',
		'control your destination.'
	].join('');
	var elevator_unit_floor_11 = [
		'"General Lobby", reads the sign above this elevator entrance. ',
		'You can hear the hustle and bustle of the general population ',
		'scurrying around. The promise of coffee entices your senses. ',
	].join('');

	run([
		'rbuild pave continue',
		's',
		'rbuild title Operations Level 1 - Server Room 0xA155',
		'rbuild description ' + server_room_a155_description,
		's',
		'rbuild title Server Room 0xA155 - Row Zero',
		'rbuild description ' + server_room_row_zero_middle,
		'w',
		'rbuild title Server Room 0XA155 - Row Zero - Desk Zero West',
		'rbuild description ' + server_room_row_zero_west_desk_0,
		'w',
		'rbuild title Server Room 0XA155 - Row Zero - Desk One West',
		'rbuild description ' + server_room_row_zero_west_desk_1,
		'w',
		'rbuild title Server Room 0XA155 - Row Zero - Desk Two West',
		'rbuild description ' + server_room_row_zero_west_desk_2,
		'e','e','e',
		'e',
		'rbuild title Server Room 0XA155 - Row Zero - Desk Zero East',
		'rbuild description ' + server_room_row_zero_east_desk_0,
		'e',
		'rbuild title Server Room 0XA155 - Row Zero - Desk One East',
		'rbuild description ' + server_room_row_zero_east_desk_1,
		'e',
		'rbuild title Server Room 0XA155 - Row Zero - Desk Two East',
		'rbuild description ' + server_room_row_zero_east_desk_2,
		'w','w','w',
		's', 
		'rbuild title Server Room 0xA155 - Row One - Containment',
		'rbuild description ' + server_room_south_row_one,
		's', 
		'rbuild title Server Room 0xA155 - Row One - Containment',
		'rbuild description ' + server_room_south_row_one,
		'w',
		'rbuild title Server Room 0xA155 - Row One West',
		'rbuild description ' + server_room_south_row_one_west,
		'w',
		'rbuild title Server Room 0xA155 - Row One West',
		'rbuild description ' + server_room_south_row_one_west,

		'rbuild title Server Room South Entrance',
		'rbuild description ' + server_room_south_entrance,
		's',
		'rbuild title Commons Hallway - Server Room Bio Metrics Scanning',
		'rbuild description ' + commons_hallway_server_room_bio_metrics_scanning,
		's',
		'rbuild title Commons Hallway [0]',
		'rbuild description ' + commons_hallway_0,
		's',
		'rbuild title Commons Hallway [-1]',
		'rbuild description ' + commons_hallway_n1,
		's',
		'rbuild title Commons Hallway [-2]',
		'rbuild description ' + commons_hallway_n2,
		's',
		'rbuild title Commons Hallway [-3]',
		'rbuild description ' + commons_hallway_n3,
		'w',
		'rbuild title Commons Restroom - MALE',
		'rbuild description ' + commons_restroom_male,
		'e',
		'e',
		'rbuild title Commons Restroom - FEMALE',
		'rbuild description ' + commons_restroom_female,
		'w',
		's',
		'rbuild title Commons Elevator Unit Floor 10',
		'rbuild description ' + commons_elevator_unit_a,
		'rbuild sector-type INDOOR_ELEVATOR',
		'u',
		'rbuild title Elevator Unit Floor 11',
		'rbuild description ' + elevator_unit_floor_11,
		'rbuild sector-type INDOOR_ELEVATOR',
		'rbuild pave off',
	]);
	send('To save the paved area, type: "rbuild save-paved 0"');
}

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
		'all the various items for sell. You can spend {grn}MP{/grn} (Mission Points) here to upgrade you loadout. ',
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
		'w',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Southern Hallway',
		'rbuild description ' + south_hallway_east_west,
		'rbuild sector-type INDOOR_CEMENT',
		'w',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Southern Hallway',
		'rbuild description ' + south_hallway_east_west,
		'rbuild sector-type INDOOR_CEMENT',
		'w',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Southern Hallway',
		'rbuild description ' + south_hallway_east_west,
		'rbuild sector-type INDOOR_CEMENT',
		'w',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Southern Hallway',
		'rbuild description ' + south_hallway_east_west,
		'rbuild sector-type INDOOR_CEMENT',

		'e','e','e','e', /* backtrack */
		'e',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Southern Hallway',
		'rbuild description ' + south_hallway_east_west,
		'rbuild sector-type INDOOR_CEMENT',
		'e',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Southern Hallway',
		'rbuild description ' + south_hallway_east_west,
		'rbuild sector-type INDOOR_CEMENT',
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
		'w','w','w', /* backtracking */
		'n',
		'rbuild title {blu}C.O.F.O.B:{/blu} - Eastern Hallway',
		'rbuild description ' + eastern_hallway_middle,
		'rbuild sector-type INDOOR_CEMENT',
	]);
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
var vertical_slice = {
	mf13_hoodlum: function(index){
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

function main(){
	funcs.always();
	//do_sensor_nade();
	//do_sensor_nade_with_range_modifier();
	//do_nv_camera();
	//send('when done installing, call: js t1()');
	//do_thermal_camera();
	//send('when done installing, call: js th1();');
	//do_fire();
	//do_null_weapon_test();
	//do_rifle_weapon_test();
	//do_flush_feed_test();
	//do_revive_test();
	//do_send_report();
	//do_fueled_fire();
	//run(['rbuild flag:add PEACEFUL']);
	//var hp = 65535;
	//set_points('far','hp',hp);
	//set_points('far','max_hp',hp);
	//set_points('far','move',5000);
	//set_points('far','max_move',5000);
	//set_points('far','mana',5000);
	//set_points('far','max_mana',5000);

	//set_points('zim','hp',hp);
	//set_points('zim','max_hp',hp);
	//set_points('zim','move',5000);
	//set_points('zim','max_move',5000);
	//set_points('zim','mana',5000);
	//set_points('zim','max_mana',5000);
	//set_attacker('zim','far');
	//instigate('zim','far');
	//do_mini_gunner();
	//do_giveme_g36c();
	//do_distributed_load_test_mini_gunner();
	//run(['n','n','n']);
	//do_mini_gunner_special(3,132);
	//do_get_noticed();
	//do_chat();
	//send('this should be the phrase "foobar1234!": ' + '"' + do_value_system_sanity_check('foobar1234!') + '"');
	//run(['get_super_user_list']);
	//run(['sbuild info']);
	//vrun(['hack', 'rotate_right', 'rotate_left','next_row','rotate_left','reset']);
	//vrun(['hack', 'rotate_right', 'rotate_left','next_row','rotate_left']);
	//vrun(['yaml_example explosive', 'yaml_example container', 'yaml_import container weapon-locker.yml']);
	//vrun(['install_weapon_locker rifle/g36c.yml explosive/frag.yml']);
	//vrun(['builder_help registration']);
	////vrun(['list_objects']);
	////vrun(['drone start']);
	//vrun(['laugh', 'eat']);
	//vrun(['n','l']);
	//vrun(['skills']);
	/*
	vrun([
		'set_rate_limit PLAYER_SAVE 1 60',
		'get_rate_limit_types PLAYER',
		'get_rate_limit_types',
		'list_rate_limits',
		'list_rate_limits PLAYER',
	]);
	*/
	//vrun([
	//	'csv_export_levels'
	//]);
	//vrun([
	//	//'award_exp_by_name ghost 2500'
	//	'save_my_pba'
	//]);
	//send('to run the server room area generator, type: js do_server_room_area(); :)');
	//do_server_room_area();
	//do_parkway_plaza_area();
	//do_cobalt_fob();
	//do_live_training_exercise_FOB_1A();
	//cmd('reward rifle 1 5 10 15 20 25');
	//cmd('reward armor 1 5 10 15 20 25');
	//cmd('inc_damage 50 100');
	//cmd('practices');
	//vertical_slice.bootstrap_mobs();
	send('to generate random area: js do_random_area();');
	do_random_area();
}

main();
