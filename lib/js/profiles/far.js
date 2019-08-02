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
}

cmd("+impl");
cmd("+god");
cmd("+build");
cmd("builder");
