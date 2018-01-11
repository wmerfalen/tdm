var player_uuid;

function report_test_pass(p_uuid,test_name){
	send_to_uuid(p_uuid,["[test]: ",test_name," passed!"].join(''));
}
function report_test_fail(p_uuid,test_name){
	send_to_uuid(p_uuid,["[test]: ",test_name," failed!"].join(''));
}
function test_new(){
	var output = cmd_exec(player_uuid,"mbuild new");
	if(output.match(/success/)){
		report_test_pass(player_uuid,"test_new");
	}else{
		report_test_fail(player_uuid,"test_new");
	}
}
