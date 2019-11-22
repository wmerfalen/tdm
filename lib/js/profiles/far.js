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
var init = {
	/** TODO: it'd be *really* nice to have an NPC drill sergeant to say all this */
	phases: [
		{'title': 'Introduction',
			'description': ["Welcome to :[SIEGE MUD]:. Before you can take part in the ",
				"fun that is Siege MUD, you will have to run through a series of tests. ",
				"Complete these tests and you will be accepted into the world and placed ",
				"into the malestrom of counter-terrorist operations. The enemy knows no ",
				"limits. We must stay vigilant. Walk {grn}NORTH{/grn} to proceed. "].join(''),
			'textures': ['CEMENT','OUTSIDE']
		},
		{
			'title': 'Basic Movement',
			'description': ["Siege MUD is based off of the CircleMud codebase, however, ",
				"we have made lots of additions. The basic movements available to you are ",
				dirhl('up'), ',', dirhl('down'),',', dirhl('north'),',', 
				dirhl('south'),',',
				dirhl('east'),', and ',
				dirhl('west'),'. ', "To the {yel}west{/yel} of here is a {yel}weapons ",
				"stockpile{/yel}. Go west and follow the instructions there to complete ",
				"your first task. "].join(''),
				'textures': ['CEMENT','OUTSIDE'],
			'always': function(player){
			},
			'quest': function(){
			}
		},
	],
	pave_layout: function(){
		cmd('rbuild_sandbox new initiation 1');
		cmd('rbuild pave on 1 1');
		cmd('rbuild texture CEMENT OUTSIDE');
		td(0);
		cmd('north');
		cmd('north');
		td(1);
		cmd('north');
		cmd('north');
		td(2);
	}
};

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
	cmd("throw frag north 1");
}

function rbs_alpha(){
	cmd('rbuild_sandbox pave on "alpha"');
	cmd('n');
	cmd('n');
	cmd('n');
	cmd('e');
	cmd('e');
	cmd('n');
	cmd('n');
	cmd('n');
	cmd('e');
	cmd('n');
	cmd('w');
	cmd('n');
	cmd('n');
	cmd('u');
	cmd('u');
	cmd('s');
	cmd('e');
	cmd('e');
	cmd('n');
	cmd('n');
	cmd('e');
	cmd('w');
	cmd('s');
	cmd('s');
	cmd('w');
	cmd('w');

	cmd('rbuild_sandbox pave off');
	cmd('rbuild_sandbox save 0');
}

//rbs_alpha();
