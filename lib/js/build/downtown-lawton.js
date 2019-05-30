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
		'n',	//Los Gatos Mexican Food
		'rbuild title Los Gatos Mexican Food',
		'n',
		'rbuild title Los Gatos Kitchen',
		//TODO: place mob here [test with behaviour trees]
		//TODO: create breachable door to the south
		'n',	//Los Gatos Backdoor Entrance
		'rbuild title Los Gatos Mexican Food Back Entrance',
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

/**
 * The only argument to this function is the zone_index that corresponds
 * with the "Sixth Tower Chapel" zone.
 *
 * If you do not start this function when you're at the ideal starting point,
 * then it is likely to cause issues within the build system. I can't say
 * confidently whether or not it will corrupt the map, but it is definitely
 * undefined behaviour if this function isn't called at the right point.
 *
 * [2019-03-06] -> Run this function when you are at the room numbered 5
 * in the rnumlist output. 
 *
 */
function build_sixth_tower_chapel(zone_index){
	var top_left_anchor = {
		coordinates: [0,0],
		virtual_number: 600
	};
	/**
	 * Roadmap 
	 * --------
	 *  Any coordinates referenced below in the code is made by
	 *  using column, comma, row-- all surrounded by square brackets.
	 *  So the 'g' on the map below is at [1,0]
	 *
	 * 0  0  1  2  3  4  5  6  7  8  9  a  b  c  d  e
	 * 0-[ ][ ][g][ ][ ][ ][ ][ ][ ][ ][_____[x]_____]
	 * 1 =========[ ][ ][ ][ ][ ][ ][ ][ ][ ][ ][ ][ ]
	 * 2 ============[ ]======[ ][ ][ ][ ][ ][ ][ ][ ]
	 * 3 ============[ ]======[ ][ ][ ][  /``[x]```[x]``
	 * 4 =========[ ][ ][ ]======[ ][ ][ <>+
	 * 5 =========[ ][ ][ ]======[ ][ ][  \__[x]___[x]__
	 * 6 =========[ ][ ][ ]===[--------------------------
	 */

	var reused_west_entrance_road = ['rbuild description ',
			"The road stretches to the east and the further you make your ",
			"way through this narrowing road, the parking lot exposes itself ",
			"to you. It looks as if the parking spaces could suit any sized ",
			"vehicle. "
		].join('');

	/** Range: [3,0] to [9,0] */
	var range_3_1_to_e_1 = [
			"A fairly clean and well-maintained cement surface lies here in wait ",
			"to support the various vehicles that make their way through here once, ",
			"sometimes two to three times, per week. A fairly faint breeze is felt here, ",
			"as this part of the parking lot is in the open, save for the building to the east. "
		].join('')
	var range_copy = {
		'3_0_to_9_0': {
			title: 'North end parking lot',
			description: range_3_0_to_9_0,
			texture: ['CEMENT','OUTSIDE'],
			from: [3,0],
			to: [9,0]
		}
	};
	for(var i= 2; i < 9;i++){
		cmd('rbuild title "North end parking lot [' + i + ",0]"');
		cmd('rbuild description "' + range_3_0_to_9_0 + '"');
		cmd('rbuild texture CEMENT OUTSIDE');
		cmd('east');
	}
	cmd('south');
	/** second row, 3,0 -> e,1 [from the leftmost] */
	for(var i=9 ; i < 16;i++){
		cmd('rbuild title "North end parking lot [' + i + ",1]"');
		cmd('rbuild description "' + range_3_1_to_e_1 + '"');
		cmd('rbuild texture CEMENT OUTSIDE');
		cmd('east');
	}
	cmd('south');
	/** third row, 7,2 -> e,2 [from the east most] */
	var church_entrance_south = [

	].join('');
	for(var i=15;i > 6;i--){
		cmd('rbuild title "North end parking lot [' + i + ",2]"');
		switch(i){
			case 15:
			case 13:
				cmd('rbuild description "' + range_3_1_to_e_1 + ' ' + church_entrance_south + '"');
				break;
			default:
				cmd('rbuild description "' + range_3_1_to_e_1 + '"');
		}
		cmd('rbuild texture CEMENT OUTSIDE');
		cmd('west');
	}
	cmd('south');

	var chapel_to_the_right = [
		"To the immediate {yel}east{/yel} lies the front face of the chapel. ",
		"Towering high into the depths of the sky is the geometric shape that ",
		"has become synonymous with the chapel and it's members. "
	].join('');
	/** fourth row, 7,3 -> 9,3 [from the left most] */
	for(var i=7;i < 10;i--){
		cmd('rbuild title "North end parking lot [' + i + ",3]"');
		if(i == 9){
			cmd('rbuild description "' + range_3_1_to_e_1 + ' ' + chapel_to_the_right + '"');
		}else{
			cmd('rbuild description "' + range_3_1_to_e_1 + '"');
		}
		cmd('rbuild texture CEMENT OUTSIDE');
		cmd('east');
	}


	for(var i=15 ; i > 2;i--){
		cmd('rbuild title "North end parking lot [' + i + ",1]"');
		cmd('rbuild description "' + range_3_1_to_e_1 + '"');
		cmd('rbuild texture CEMENT OUTSIDE');
		cmd('west');
	}

	var grassy_description_west_parking_lot = [
		"The dampness of the grass below your feet is a welcoming sensation ",
		"in contrast to the stone-compacted nature of the parking spaces surrounding ",
		"this plot of land. `zone_sixth_chapel.east_congregation()`" 
	].join('');
	cmd('west');
	cmd('rbuild title "North end parking lot [2,1]');
	cmd('rbuild description "' + grassy_description_west_parking_lot + '"');
	cmd('rbuild texture GRASS OUTSIDE');


	/** Second row of rooms. */
	for(var i=3 ; i > 15;i--){
		cmd('rbuild title "North end parking lot [' + i + ",1]"');
		cmd('rbuild description "' + range_3_1_to_e_1 + '"');
		cmd('rbuild texture CEMENT OUTSIDE');
		cmd('east');
	}
	for(var i=15;i > -1;i--){
		cmd('west');
	}

	//var range_2_0_to_8_0_accumulated = [];
	//for(var irange_ctr = 0; irange_ctr < 7;irange_ctr++){
	//	range_2_0_to_8_0_accumulated.push(range_2_0_to_8_0);
	//}

	/** Range: [2,1] to [8,1] */
	var range_2_1_to_8_1 = [
		'rbuild title North End Parking lot',
		['rbuild description ',
			"A fairly clean and well-maintained cement surface lies here in wait ",
			"to support the various vehicles that make their way through here once, ",
			"sometimes two to three times, per week. A fairly faint breeze is felt here, ",
			"as this part of the parking lot is in the open, save for the building to the east. "
		].join(''),
		'e'
	];
	//var range_2_0_to_8_0_accumulated = [];
	//for(var irange_ctr = 0; irange_ctr < 7;irange_ctr++){
	//	range_2_0_to_8_0_accumulated.push(range_2_0_to_8_0);
	//}
	var commands = [
		['rbuild pave on 600 ',zone_index].join(''),

		/** co-ordinate [0,0] */
		'rbuild title West Entrance to Sixth Tower Chapel Parking Lot [0,0]',
		reused_west_entrance_road,
		'e',

		/** co-ordinate [1,0] */
		'rbuild title At The Gates: West Sixth Tower Chapel Parking Lot [1,0]',
		['rbuild description ',
			"Here lies the gate to the West entrance of the Sixth Tower ",
			"Chapel Parking Lot. The dogs just north of this gate are barking ",
			"ferociously indicating that, despite having a church in it's mists, ",
			"it could not stop the plight of victims of economic slavery."
		].join(''),

		'e',	//Entrance into the parking lot
		'rbuild title West Entrance to Sixth Tower Chapel Parking Lot [2,0]',
		['rbuild description ',
			"The road stretches to the east and the further you make your ",
			"way through this narrowing road, the parking lot exposes itself ",
			"to you. It looks as if the parking spaces could suit any sized ",
			"vehicle. Just faintly over the brim of the top of the gate can ",
			"be seen a crosshair symbolizing the eternal nature of the religion ",
			"which grips the foundation of this chapel. "
		].join(''),


	];
}

function build_sixth_tower_chapel_zone(){
	var output = cmd_exec('zbuild list');
	var zone_list = Duktape.dec('jx',output)['zones'];
	for(var i=0; i < zone_list.length;i++){
		if(zone_list[i].name == 'Sixth Tower Church'){
			send("{red}Warning:{/red} {yel}Sixth Tower Church{/yel} zone has already been created! Returning...");
			return;
		}
	}

	cmd('zbuild new 600 700 "Sixth Tower Chapel" 10 2');
	send('Built zone');
}
