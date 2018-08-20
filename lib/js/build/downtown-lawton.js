function build_rooms(zone_index){
	var reused_1 = ['rbuild description ',
		"Make an entry into the 'Guinness Book of World Records' for the absolute ",
		"most disgusting street you've ever encountered. Trash is strewn ",
		"apathetically across every conceivable walkway area. The crew ",
		"responsible for cleaning this place hasn't visited in quite a ",
		"long time from the looks of it. Homeless drug addicts are lying ",
		"in their own self-contained cardboard hells."].join('');
	var reused_2 = ['rbuild description ',
		"This crudely carved out tunnel allows the average height male ",
		"to comfortably walk through. The purpose of this tunnel isn't ",
		"immediately obvious, but knowing the residents of Downtown Lawton, ",
		"it's likely connected to drugs. Blue canvases are draped across the ",
		"ground and grey canvases are laid over the padded walls. Why are ",
		"the walls padded?"
	].join('');
	var reused_3 = ['rbuild description ',
		"The canvas floor and walls abruptly end here. A light is hung here ",
		"providing just enough illumination to notice that there is a door ",
		"to the west of here that likely leads to the real reason this ",
		"underground system was built."
		].join('');
	var commands = [
		'recall',
		['rbuild pave on 121 ',zone_index].join(''),
		'n',	//Outside operations
		'rbuild title Outside The Operations Building',
		['rbuild description ',
		"The crisp air assaults every bit of your body ",
		"that is exposed to the elements. Huge industrial buildings obscure your ",
		"view of the mountainous backdrop surrounding this vast metropolis. City ",
		"buses will occasionally run by here. The street and sidewalk areas are ",
		"filthy. {red}Welcome to Lawton City{/red}"].join(''),
		'n',	//Downtown Lawton City
		'rbuild title Downtown Lawton City',
		reused_1,
		'e',	//Downtown Lawton City
		'rbuild title Downtown Lawton City',
		reused_1,
		'e',	//Francesco's Italian Food
		'rbuild title Francesco\'s Italian Food',
		['rbuild description ',
		"A refreshing change from the trashy cesspool that is Downtown Lawton. ",
		"The waiting area immediately greets you, but there is nobody there. ",
		"Air conditioning keeps the place either too hot or too cold-- there is ",
		"no middle ground. A waitress can be seen putting too much ",
		"salt in a dish to keep a certain customer from coming back. "].join(''),
		'w',
		'w',	//Reverand Avenue
		'rbuild title Downtown Lawton City',
		reused_1,
		'n',
		'rbuild title Reverand Avenue',
		['rbuild description ',
		"Of all the things that can be attributed to Downtown Lawton, faith ",
		"surely is not one of them. From the abandoned streets and overcrowded ",
		"walkways protrudes the infamous Sixth Tower Church. Worshippers gather ",
		"and pay heed to the all famous deity. "].join(''),
		'e',
		'rbuild title Sixth Tower Church',
		['rbuild description ',
		"Anyone would consider joining the Sixth Tower Church. That is, if the ",
		"cleanliness of a patron's soul matched the cleanliness of the inside of ",
		"this chapel. This area is largely empty which contributes to the calming ",
		"effect that the silence has on anything that steps into this chapel."
		].join(''),
		'w',	//Reverand Avenue
		'w',	//still on reverand avenue	[1]
		'rbuild title Reverand Avenue',
		['rbuild description ',
		"Of all the things that can be attributed to Downtown Lawton, faith ",
		"surely is not one of them. From the abandoned streets and overcrowded ",
		"walkways protrudes the infamous Sixth Tower Church. Worshippers gather ",
		"and pay heed to the all famous deity. "].join(''),
		'n',		//Armory
		'rbuild title Armor And Supplies',
		//TODO: instantiate shop here
		's',	//Reverand avenue [1]
		'w',	//reverand avenue [2]
		'rbuild title Reverand Avenue',
		['rbuild description ',
		"Of all the things that can be attributed to Downtown Lawton, faith ",
		"surely is not one of them. From the abandoned streets and overcrowded ",
		"walkways protrudes the infamous Sixth Tower Church. Worshippers gather ",
		"and pay heed to the all famous deity. "].join(''),
		'n',	//Los Putos Mexican Food
		'rbuild title Los Putos Mexican Food',
		'n',
		'rbuild title Los Putos Kitchen',
		//TODO: place mob here [test with behaviour trees]
		//TODO: create breachable door to the south
		'n',	//Los Putos Backdoor Entrance
		'rbuild title Los Putos Mexican Food Back Entrance',
		//TODO: place breachable bottom door here
		'd',
		'rbuild title Entrance to an underground tunnel',
		'w',
		'rbuild title Inside a tunnel',
		reused_2,
		'w',
		'rbuild title Inside a tunnel',
		reused_2,
		'w',
		'rbuild title Inside a tunnel',
		reused_2,
		'n',
		'rbuild title A dimly lit cave',
		reused_3,
		'w',
		'rbuild title A dimly lit cave',
		reused_3,
		'w',
		'rbuild title A dimly lit cave',
		reused_3,
		'w',
		'rbuild title Outside a fortified entrance',
		['rbuild description ',
		"It's not indestructible, but a normal breach charge ",
		"wouldn't cut through the thick armor that this door is fortified ",
		"with. A {red}thermite{/red} charge would cut through that door, ableit ",
		"very noisily."
		].join(''),
		//TODO: make a thermite breachable door to the west
		'rbuild pave off'
	];
	for(var i=0; i < commands.length;i++){
		cmd(commands[i]);
	}
}

function build(){
	var output = cmd_exec('zbuild list');
	var zone_list = Duktape.dec('jx',output)['zones'];
	for(var i=0; i < zone_list.length;i++){
		if(zone_list[i].name == 'Downtown Lawton'){
			return build_rooms(zone_list[i].id);
		}
	}

	cmd('zbuild new 121 200 "Downtown Lawton" 10 2');
	output = cmd_exec('zbuild list');
	zone_list = Duktape.dec('jx',output)['zones'];
	var zone_id = 0;
	for(var i=0;i < zone_list.length;i++){
		if(zone_list[i].name == 'Downtown Lawton'){
			return build_rooms(zone_list[i].id);
		}
	}
}
