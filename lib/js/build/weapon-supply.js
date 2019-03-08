function weapons_rack(index,item_number){
	cmd(["obuild flag ",index, " type_flags CONTAINER"].join(""));
	cmd(["obuild flag ",index, " value_0 0"].join(""));				//unused
	cmd(["obuild flag ",index, " value_1 0"].join(""));		//Number of damage dice
	cmd(["obuild flag ",index, " value_2 0"].join(""));		//size of damage dice
	cmd(["obuild flag ",index, " value_3 0"].join(""));		//size of damage dice
	cmd(["obuild ex ",index, " create 1"].join(""));
	cmd(["obuild ex ",index, " set 0 rack \"A weapons rack.\""].join(""));
	cmd(["obuild attr ",index, " item_number ",item_number].join(""));
	cmd(["obuild attr ",index, " name \"weapons rack\""].join(""));
	cmd(["obuild attr ",index, " description \"A weapons rack is attached to the wall here.\""].join(""));
	cmd(["obuild attr ",index, " short_description \"A weapons rack\""].join(""));
	cmd(["obuild attr ",index, " action_description \"A weapons rack\""].join(""));
}

