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
function snipe_fixture(){
cmd("mbuild instantiate 0");
cmd("obuild instantiate 2");
cmd("get xm");
cmd("builder");
cmd("yaml_example rifle");
cmd("yaml_import ls");
cmd("yaml_import rifle rifle.yml");
}

cmd("+impl");
cmd("+god");
cmd("+build");


function frag(){
	for(var i=0;i < 10;i++){
		cmd("giveme_frag");
	}
	cmd("hold frag");
	cmd("throw north 1");
}

function rbs_alpha(){
	cmd('rbuild_sandbox pave on "alpha"');
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
function do_camera(){
	cmd('yaml_import gadget camera.yml');
	cmd('get pi');
	cmd('hold pi');
	cmd('install pi north');
}
//yaml_example();
//yaml_import();
//do_sensor_nade();
//do_camera();

function mp5_shop(index){
	var cmds = ['sbuild attr ' + index + ' title "MP5 repo"',
		'sbuild attr ' + index + ' description "Feeling over burdened by money?"',
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
}
