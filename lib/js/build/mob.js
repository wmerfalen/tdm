function mbuild_insert(index){
	cmd(["mbuild attr ",index, " item_number ",next_mob_number()].join(""));
	cmd(["mbuild save ",index].join(""));
}
function mbuild_update(index){
	cmd(["mbuild save ",index].join(""));
}
function lowbie_mob(index) {
	cmd(["mbuild attr ",index," mana ",50].join(""));
	cmd(["mbuild attr ",index," max_mana ",50].join(""));
	cmd(["mbuild attr ",index," hit ",20].join(""));
	cmd(["mbuild attr ",index," max_hit ",20].join(""));
	cmd(["mbuild attr ",index," move ",50].join(""));
	cmd(["mbuild attr ",index," max_move ",50].join(""));
	cmd(["mbuild attr ",index," damroll ",5].join(""));
	cmd(["mbuild attr ",index," gold ",200].join(""));
	cmd(["mbuild attr ",index," level ",1].join(""));
	cmd(["mbuild attr ",index," weight ",10].join(""));
	cmd(["mbuild attr ",index," height ",3].join(""));
	cmd(["mbuild attr ",index," strength ",2].join(""));
	cmd(["mbuild attr ",index," strength_add ",2].join(""));
	cmd(["mbuild attr ",index," intelligence ",2].join(""));
	cmd(["mbuild attr ",index," dexterity ",2].join(""));
	cmd(["mbuild attr ",index," constitution ",2].join(""));
	cmd(["mbuild attr ",index," charisma ",2].join(""));
	cmd(["mbuild attr ",index," damnodice ",2].join(""));
	cmd(["mbuild attr ",index," damsizedice ",5].join(""));
	return 0; 
}

function lowbie_rat(index){
	cmd(["mbuild attr ",index, " name rat"].join(""));
	cmd(["mbuild attr ",index, ' description "A lowly fiendish creature just begging to be put out of it\'s misery. A faint glimmer of putrefaction can be seen leaving it\'s body with every malformed step it takes."'].join(""));
	cmd(["mbuild attr ",index, " short_description \"A fiendish rabies-infected rat\""].join(""));
	cmd(["mbuild attr ",index, " long_description \"A fiendish rabies-infected rat\""].join(""));
	cmd(["mbuild attr ",index, " sex MALE"].join(""));
	lowbie_mob(index);
	cmd(["mbuild save ",index].join(""));
	return 0;
}

function lowbie_security_guard(index){
	cmd(["mbuild attr ",index, " name sec security guard"].join(""));
	cmd(["mbuild attr ",index, " description \"An underpaid and under-achieving security guard stands awkwardly alert.\""].join(""));
	cmd(["mbuild attr ",index, " short_description \"A nervous looking security guard\""].join(""));
	cmd(["mbuild attr ",index, " long_description \"A nervous looking security guard\""].join(""));
	cmd(["mbuild attr ",index, " sex MALE"].join(""));
	lowbie_mob(index);
	cmd(["mbuild save ",index].join(""));
	return 0;
}
function scaled_mob(index,level){
	cmd(["mbuild attr ",index," mana ",50 * level].join(""));
	cmd(["mbuild attr ",index," max_mana ",50 * level].join(""));
	cmd(["mbuild attr ",index," hit ",50 * level].join(""));
	cmd(["mbuild attr ",index," max_hit ",50 * level].join(""));
	cmd(["mbuild attr ",index," move ",50 * level].join(""));
	cmd(["mbuild attr ",index," max_move ",50 * level].join(""));
	cmd(["mbuild attr ",index," damroll ",50 * level].join(""));
	cmd(["mbuild attr ",index," gold ",1000 * level].join(""));
	cmd(["mbuild attr ",index," level ",level].join(""));
	cmd(["mbuild attr ",index," weight ",10 * level].join(""));
	cmd(["mbuild attr ",index," height ",3 * level].join(""));
	cmd(["mbuild attr ",index," strength ",5 * level].join(""));
	cmd(["mbuild attr ",index," strength_add ",5 * level].join(""));
	cmd(["mbuild attr ",index," intelligence ",5 * level].join(""));
	cmd(["mbuild attr ",index," dexterity ",5 * level].join(""));
	cmd(["mbuild attr ",index," constitution ",5 * level].join(""));
	cmd(["mbuild attr ",index," charisma ",5 * level].join(""));
	cmd(["mbuild attr ",index," damnodice ",5 * level].join(""));
	cmd(["mbuild attr ",index," damsizedice ",5 * level].join(""));
	return 0;
}

function chef(index){
	cmd(["mbuild attr ",index, " name chef los "].join(""));
	cmd(["mbuild attr ",index, " description ",
		"\"This particular employee looks like he just got out of ",
		"a federal penitentiary. He's most likely hiding some weapon ",
		"in one of the many compartments that should be used for ",
		"storing utensils and food paraphernalia.\""
	].join(""));
	cmd(["mbuild attr ",index, " short_description \"A pissed looking Los  employee\""].join(""));
	cmd(["mbuild attr ",index, " long_description \"A pissed looking Los  employee\""].join(""));
	cmd(["mbuild attr ",index, " sex MALE"].join(""));
	scaled_mob(index,5);
	cmd(["mbuild save ",index].join(""));
	return 0;
}

function level_5_mp_guard(index){
	cmd(["mbuild attr ",index, " name mp guard"].join(""));
	cmd(["mbuild attr ",index, " description \"Brash and abrassive, this MP doesn't play any games. He is strict in appearance and demeanor.\""].join(""));
	cmd(["mbuild attr ",index, " short_description \"A confident looking MP\""].join(""));
	cmd(["mbuild attr ",index, " long_description \"A confident looking MP\""].join(""));
	cmd(["mbuild attr ",index, " sex MALE"].join(""));
	scaled_mob(index,5);
	cmd(["mbuild save ",index].join(""));
	return 0;
}

function level_10_mp_guard(index){
	cmd(["mbuild attr ",index, " name sentinel mp guard"].join(""));
	cmd(["mbuild attr ",index, " description \"An MP with enough firepower and muscle to hold down a constant opposing force for an extended period of time.\""].join(""));
	cmd(["mbuild attr ",index, " short_description \"A Sentinel MP\""].join(""));
	cmd(["mbuild attr ",index, " long_description \"A Sentinel MP\""].join(""));
	cmd(["mbuild attr ",index, " sex MALE"].join(""));
	scaled_mob(index,10);
	cmd(["mbuild save ",index].join(""));
	return 0;
}

function behaviour_trees_fixture(){
	var cmds = ['mbuild instantiate 0', 'obuild instantiate 2'];
	for(var i=0; i < cmds.length; i++){
		cmd(cmds[i]);
	}
}
function mob_js(){
	send("mob_js included");
}
