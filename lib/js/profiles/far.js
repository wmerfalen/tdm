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
	run(['rbuild flag:add PEACEFUL']);
	var hp = 65535;
	set_points('far','hp',hp);
	set_points('far','max_hp',hp);
	set_points('far','move',5000);
	set_points('far','max_move',5000);
	set_points('far','mana',5000);
	set_points('far','max_mana',5000);
	do_mini_gunner();
	//do_giveme_g36c();
	//do_distributed_load_test_mini_gunner();
	//run(['n','n','n']);
	do_mini_gunner_special(3,132);
	//do_chat();
}

main();
