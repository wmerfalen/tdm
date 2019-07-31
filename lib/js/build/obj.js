function shotgun_weapon_template(index,level){
	cmd(["obuild attr ",index, " weapon_type:add SHOTGUN"].join(""));
	cmd(["obuild flag ",index, " wear_flags TAKE WIELD"].join(""));
	cmd(["obuild flag ",index, " type_flags WEAPON"].join(""));
	cmd(["obuild flag ",index, " value_0 0"].join(""));				//unused
	cmd(["obuild flag ",index, " value_1 ",35 * level].join(""));		//Number of damage dice
	cmd(["obuild flag ",index, " value_2 ",5 * level].join(""));		//size of damage dice
	cmd(["obuild flag ",index, " value_3 ",7].join(""));		//size of damage dice
}
//TODO: remove first parameter from all these calls where uuid is being used
function sniper_rifle_weapon_template(index,level){
	cmd(["obuild attr ",index, " weapon_type:add SNIPE"].join(""));
	cmd(["obuild flag ",index, " wear_flags TAKE WIELD"].join(""));
	cmd(["obuild flag ",index, " type_flags WEAPON"].join(""));
	cmd(["obuild flag ",index, " value_0 0"].join(""));				//unused
	cmd(["obuild flag ",index, " value_1 ",1 * level].join(""));	//Number of damage dice
	cmd(["obuild flag ",index, " value_2 ",50 * level].join(""));	//size of damage dice
	cmd(["obuild flag ",index, " value_3 ",7].join(""));		//size of damage dice
}
function smg_weapon_template(index,level){
	cmd(["obuild attr ",index, " weapon_type:add SMG"].join(""));
	cmd(["obuild flag ",index, " wear_flags TAKE WIELD"].join(""));
	cmd(["obuild flag ",index, " type_flags WEAPON"].join(""));
	cmd(["obuild flag ",index, " value_0 0"].join(""));				//unused
	cmd(["obuild flag ",index, " value_1 ",25 * level].join(""));		//Number of damage dice
	cmd(["obuild flag ",index, " value_2 ",5 * level].join(""));		//size of damage dice
	cmd(["obuild flag ",index, " value_3 ",7].join(""));		//size of damage dice
}
function grenade_weapon_template(index,level){
	cmd(["obuild attr ",index, " weapon_type:add GRENADE"].join(""));
	cmd(["obuild flag ",index, " wear_flags TAKE WIELD"].join(""));
	cmd(["obuild flag ",index, " type_flags WEAPON"].join(""));
	cmd(["obuild flag ",index, " value_0 0"].join(""));				//unused
	cmd(["obuild flag ",index, " value_1 ",3 * level].join(""));		//Number of damage dice
	cmd(["obuild flag ",index, " value_2 ",5 * level].join(""));		//size of damage dice
	cmd(["obuild flag ",index, " value_3 ",7].join(""));		//size of damage dice
}
function level_1_p90(index){
	var level = 1;
	smg_weapon_template(index,level);
	cmd(["obuild attr ",index, " item_number 91"].join(""));
	cmd(["obuild attr ",index, " name \"p90 smg\""].join(""));
	cmd(["obuild attr ",index, " description \"A P90 lies here.\""].join(""));
	cmd(["obuild attr ",index, " short_description \"A P90\""].join(""));
	cmd(["obuild attr ",index, " action_description \"A P90\""].join(""));
	cmd(["obuild ex ",index, " create 1"].join(""));
	cmd(["obuild ex ",index, " set 0 p90 \"A P90\""].join(""));
}

function level_1_xm109(index){
	var level = 1;
	sniper_rifle_weapon_template(index,level);
	cmd(["obuild attr ",index, " item_number 92"].join(""));
	cmd(["obuild attr ",index, " name \"xm109 xm snipe sniper rifle\""].join(""));
	cmd(["obuild attr ",index, " description \"An XM109 lies here.\""].join(""));
	cmd(["obuild attr ",index, " short_description \"An XM109\""].join(""));
	cmd(["obuild attr ",index, " action_description \"An XM109\""].join(""));
	cmd(["obuild ex ",index, " create 1"].join(""));
	cmd(["obuild ex ",index, " set 0 xm109 \"An XM109\""].join(""));
}
function level_5_p90(index){
	var level = 5;
	smg_weapon_template(index,level);
	cmd(["obuild attr ",index, " item_number 93"].join(""));
	cmd(["obuild attr ",index, " name \"p90 smg\""].join(""));
	cmd(["obuild attr ",index, " description \"A devastating P90 lies here.\""].join(""));
	cmd(["obuild attr ",index, " short_description \"A devastating P90\""].join(""));
	cmd(["obuild attr ",index, " action_description \"A devastating P90\""].join(""));
	cmd(["obuild ex ",index, " create 1"].join(""));
	cmd(["obuild ex ",index, " set 0 p90 \"A P90\""].join(""));
}

function xm109(index,level){
	sniper_rifle_weapon_template(index,level);
	cmd(["obuild attr ",index, " item_number 94"].join(""));
	cmd(["obuild attr ",index, " name \"xm109 xm snipe sniper rifle\""].join(""));
	cmd(["obuild attr ",index, " description \"A devastating XM109 lies here.\""].join(""));
	cmd(["obuild attr ",index, " short_description \"A devastating XM109\""].join(""));
	cmd(["obuild attr ",index, " action_description \"A devastating XM109\""].join(""));
	cmd(["obuild ex ",index, " create 1"].join(""));
	cmd(["obuild ex ",index, " set 0 xm109 \"A devastating XM109\""].join(""));
}
	
function give_me_sniper(){
cmd("obuild instantiate 2");
cmd("get xm");
cmd("wield xm");
cmd("ammo");
}
	

/**
 * ATTACHMENTS
 */

function scope_template(index,level){
	cmd(["obuild attr ",index, " weapon_type:add SNIPE"].join(""));
	cmd(["obuild attr ",index, " weapon_type:add SNIPE"].join(""));
	cmd(["obuild flag ",index, " wear_flags TAKE WIELD"].join(""));
	cmd(["obuild flag ",index, " type_flags WEAPON"].join(""));
	cmd(["obuild flag ",index, " value_0 0"].join(""));				//unused
	cmd(["obuild flag ",index, " value_1 ",1 * level].join(""));	//Number of damage dice
	cmd(["obuild flag ",index, " value_2 ",50 * level].join(""));	//size of damage dice
	cmd(["obuild flag ",index, " value_3 ",7].join(""));		//size of damage dice
}
function obuild_insert(index){
	cmd(["obuild attr ",index, " item_number ",next_object_number()].join(""));
	cmd(["obuild save ",index].join(""));
}
function obuild_update(index){
	cmd(["obuild save ",index].join(""));
}

function obj_js(){
	send("obj_js included");
}

