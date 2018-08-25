var player_uuid;

function report_test_pass(p_uuid,test_name){
	send_to_uuid(p_uuid,["[test]: ",test_name," passed!"].join(''));
}
function report_test_fail(p_uuid,test_name){
	send_to_uuid(p_uuid,["[test]: ",test_name," failed!"].join(''));
}

function run_test(command,test_name,p_uuid){
	var output = cmd_exec(p_uuid,command);
	if(output.match(/\[success\]/)){
		report_test_pass(p_uuid,test_name);
	}else{
		report_test_fail(p_uuid,test_name);
	}
}

function test_main(p_uuid){
	run_test("mbuild new","mbuild-new",p_uuid);
}

