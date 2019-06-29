function report_test_pass(p_uuid,test_name){
	send(["[test]: ",test_name," passed!"].join(''));
}
function report_test_fail(p_uuid,test_name){
	send(["[test]: ",test_name," failed!"].join(''));
}

function run_test(command,test_name){
	var output = cmd_exec(command);
	if(output.match(/\[success\]/)){
		report_test_pass(test_name);
	}else{
		report_test_fail(test_name);
	}
}

