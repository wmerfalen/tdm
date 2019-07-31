function builder_init(){
	require_js('build/mob.js');
	require_js('build/obj.js');
	require_js('build/room.js');
}

function next_object_number(){
	return cmd_exec('next_object_number');
}
function next_mob_number(){
	return cmd_exec('next_mob_number');
}
function next_room_number(){
	return cmd_exec('next_room_number');
}
