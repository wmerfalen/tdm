cmd("+impl");
cmd("+god");
cmd("+build");
cmd("mbuild instantiate 0");
//mobile_activity();
//cmd("rbuild extra_desc new");
//send("done");
for(var i=0; i < 15;i++){
	cmd("rbuild extra_desc new");
}

function fex(){ 
	cmd("rbuild extra_desc list");
}
function del(i){
	cmd("rbuild extra_desc del " + i);
}
