function contract_31_0_introduction(person){
	var text = 'A high value target is roaming the area around the IRQTF base. Intelligence tells us that this individual is highly capable and seeks to infiltrate the premises. This individual is considered highly dangerous and should be eliminated immediately. INFOSEC has supplied us with information that leads us to believe that this individual is masquerading as a civilian or a member of IRQTF security personnel. Eliminate this target and report back to me immediately. ';
	send_to_char(person,text);
	db_seti(person + ":q31_deaths",0);
	db_set(person + ":contract_trigger","contract_31_0_step1('" + person + "');");
	mob_death_trigger("{character}","contract_31_0_mob_death_trigger");
}
function contract_31_0_step1(person){
	var room = in_room(person);
	if(room == 264){
		send_to_char(person,"{red}IRQTF radios in: {/red} INFOSEC just informed us that the HVT is in your vicinity. Use the 'search' command to find your target");
		db_set(person + ":contract_trigger","contract_31_0_step2('" + person + "');");
	}
}
function contract_31_0_abort(person){
	contract_abort(person,31,0);
}
function contract_31_0_step2(person){
	
}
function contract_31_0_progress(person){
	var ctr = parseInt(db_geti(person + ":q31_deaths"));
	if(ctr < 3){
		send_to_char(person,"You have eliminated {red}" + ctr + " out of 3{/red} possible HVTs");
	}
}
function contract_31_0_mob_death_trigger(person,victim,zone){
	if(victim == 'keeper peacekeeper'){
		db_seti(person + ":q31_deaths",parseInt(db_geti(person + ":q31_deaths")) + 1);
		var ctr = parseInt(db_geti(person + ":q31_deaths"));
		if(ctr == 3){
			send_to_char(person,"You have eliminated the HVT! Congratulations on your work Agent!\r\n");
			contract_complete(person,31,0);
		}
	}
}
contract_31_0_introduction("{character}");
