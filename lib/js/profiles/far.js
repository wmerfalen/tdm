
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
/*
cmd("mbuild instantiate 0");
cmd("obuild instantiate 2");
cmd("get xm");
*/
