function rbuild_insert(index){
	cmd(["rbuild attr ",index, " item_number ",next_room_number()].join(""));
	cmd(["rbuild save ",index].join(""));
}
function rbuild_update(index){
	cmd(["rbuild save ",index].join(""));
}
function room_js(){
	send("room_js included");
}
