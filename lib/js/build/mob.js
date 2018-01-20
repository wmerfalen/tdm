function lowbie_mob(player_uuid,index) {
	cmd(player_uuid,["mbuild attr ",index," mana ",150].join(""));
	cmd(player_uuid,["mbuild attr ",index," max_mana ",150].join(""));
	cmd(player_uuid,["mbuild attr ",index," hit ",150].join(""));
	cmd(player_uuid,["mbuild attr ",index," max_hit ",150].join(""));
	cmd(player_uuid,["mbuild attr ",index," move ",150].join(""));
	cmd(player_uuid,["mbuild attr ",index," max_move ",150].join(""));
	cmd(player_uuid,["mbuild attr ",index," damroll ",50].join(""));
	cmd(player_uuid,["mbuild attr ",index," gold ",1000].join(""));
	cmd(player_uuid,["mbuild attr ",index," level ",3].join(""));
	cmd(player_uuid,["mbuild attr ",index," weight ",10].join(""));
	cmd(player_uuid,["mbuild attr ",index," height ",3].join(""));
	cmd(player_uuid,["mbuild attr ",index," strength ",5].join(""));
	cmd(player_uuid,["mbuild attr ",index," strength_add ",5].join(""));
	cmd(player_uuid,["mbuild attr ",index," intelligence ",5].join(""));
	cmd(player_uuid,["mbuild attr ",index," dexterity ",5].join(""));
	cmd(player_uuid,["mbuild attr ",index," constitution ",5].join(""));
	cmd(player_uuid,["mbuild attr ",index," charisma ",5].join(""));
	cmd(player_uuid,["mbuild attr ",index," damnodice ",5].join(""));
	cmd(player_uuid,["mbuild attr ",index," damsizedice ",5].join(""));
	return 0; 
}

function lowbie_rat(player_uuid,index){
	cmd(player_uuid,["mbuild attr ",index, " name rat"].join(""));
	cmd(player_uuid,["mbuild attr ",index, ' description "A lowly fiendish creature just begging to be put out of it\'s misery. A faint glimmer of putrefaction can be seen leaving it\'s body with every malformed step it takes."'].join(""));
	cmd(player_uuid,["mbuild attr ",index, " short_description \"A fiendish rabies-infected rat\""].join(""));
	cmd(player_uuid,["mbuild attr ",index, " long_description \"A fiendish rabies-infected rat\""].join(""));
	cmd(player_uuid,["mbuild attr ",index, " sex MALE"].join(""));
	lowbie_mob(player_uuid,index);
	cmd(player_uuid,["mbuild save ",index].join(""));
	return 0;
}

function lowbie_security_guard(player_uuid,index){
	cmd(player_uuid,["mbuild attr ",index, " name sec security guard"].join(""));
	cmd(player_uuid,["mbuild attr ",index, " description \"An underpaid and under-achieving security guard stands awkwardly alert.\""].join(""));
	cmd(player_uuid,["mbuild attr ",index, " short_description \"A nervous looking security guard\""].join(""));
	cmd(player_uuid,["mbuild attr ",index, " long_description \"A nervous looking security guard\""].join(""));
	cmd(player_uuid,["mbuild attr ",index, " sex MALE"].join(""));
	lowbie_mob(player_uuid,index);
	cmd(player_uuid,["mbuild save ",index].join(""));
	return 0;
}
function scaled_mob(player_uuid,index,level){
	cmd(player_uuid,["mbuild attr ",index," mana ",150 * level].join(""));
	cmd(player_uuid,["mbuild attr ",index," max_mana ",150 * level].join(""));
	cmd(player_uuid,["mbuild attr ",index," hit ",150 * level].join(""));
	cmd(player_uuid,["mbuild attr ",index," max_hit ",150 * level].join(""));
	cmd(player_uuid,["mbuild attr ",index," move ",150 * level].join(""));
	cmd(player_uuid,["mbuild attr ",index," max_move ",150 * level].join(""));
	cmd(player_uuid,["mbuild attr ",index," damroll ",50 * level].join(""));
	cmd(player_uuid,["mbuild attr ",index," gold ",1000 * level].join(""));
	cmd(player_uuid,["mbuild attr ",index," level ",level].join(""));
	cmd(player_uuid,["mbuild attr ",index," weight ",10 * level].join(""));
	cmd(player_uuid,["mbuild attr ",index," height ",3 * level].join(""));
	cmd(player_uuid,["mbuild attr ",index," strength ",5 * level].join(""));
	cmd(player_uuid,["mbuild attr ",index," strength_add ",5 * level].join(""));
	cmd(player_uuid,["mbuild attr ",index," intelligence ",5 * level].join(""));
	cmd(player_uuid,["mbuild attr ",index," dexterity ",5 * level].join(""));
	cmd(player_uuid,["mbuild attr ",index," constitution ",5 * level].join(""));
	cmd(player_uuid,["mbuild attr ",index," charisma ",5 * level].join(""));
	cmd(player_uuid,["mbuild attr ",index," damnodice ",5 * level].join(""));
	cmd(player_uuid,["mbuild attr ",index," damsizedice ",5 * level].join(""));
	return 0;
}

function scaled_mp_guard(player_uuid,index){
	cmd(player_uuid,["mbuild attr ",index, " name mp guard"].join(""));
	cmd(player_uuid,["mbuild attr ",index, " description \"Brash and abrassive, this MP doesn't play any games. He is strict in appearance and demeanor.\""].join(""));
	cmd(player_uuid,["mbuild attr ",index, " short_description \"A confident looking MP\""].join(""));
	cmd(player_uuid,["mbuild attr ",index, " long_description \"A confident looking MP\""].join(""));
	cmd(player_uuid,["mbuild attr ",index, " sex MALE"].join(""));
	scaled_mob(player_uuid,index,5);
	cmd(player_uuid,["mbuild save ",index].join(""));
	return 0;
}


