var zone_sixth_chapel = {
	east_congregation: function(){

		if(get_day() === 'sunday'){
			if(get_ihour() < 10){
				return [
					"In a few hours, the congregation of the Sixth Tower Chapel will make their ",
					"way in to the towering monolith of a building that you see to the East. The ",
					"sun's rays form godlike patterns behind the struture and cast a long shadow ",
					"over the breadth of the parking lot. "
				].join('');
			}
			/** From noon to 8pm, show this description */
			if(get_ihour() >= 12 && get_ihour() <= 20){
				return [
					"The lot is empty save for a few pieces of trash and today's pamphlets ",
					"being dragged and strewn by the wind. "
				].join('');
			}
			if(get_ihour() >= 20 && get_ihour() <= 24){
				if(get_moon_phase() === 'full-moon'){
					return [
						"The moon lights up the parking lot with a hazey foreboding presence."
					].join('');
				}
				return [
					"The night looms over the parking lot."
				].join('');
			}
			switch(get_ihour()){
				case 10:
				return [
					"To the east, you can see a gathering of mostly good people.",
					"Their faith a measure of their willingness to accept a higher power. ",
					"Grown men and women make their way into the chapel and take a seat before ",
					"the service begins. A few kids are misbehaving due to a lack of proper parenting ",
					"skills. "
				].join('');
					break;
				case 11:
					return [
						"To the east, you see the congregation making their way out ",
						"of the chapel. Children release their subdued energy that ",
						"had been supressed during the holy worship. "
					].join('');
					break;
			}

		}else{
			if(get_ihour() >= 0 && get_ihour() <= 10){
				return [
					"The lot is empty."
				].join('');
			}
			if(get_ihour() >= 10 && get_ihour() <= 11){
			return [
				"The lot is empty save for a few militant perishioners who take religion ",
				"much more seriously than the more casual attendees. It's odd for this lot ",
				"to have human beings present on a day besides Sunday. The directed and ",
				"close-minded school sessions are about to commence, though you wouldn't ",
				"know that by the jaded look of the chilren forced to take part in this ",
				"educated militia."
			].join('');
			}
			if(get_ihour() == 11){
				return [
					"A few perishioners are loading into their cars to make the drive home."
				].join('');
			}
			if(get_ihour() >= 12 && get_ihour() <= 8){
					return ["The lot is empty save for a few pieces of trash and today's pamphlets ",
					"being dragged and strewn by the wind. "
					].join('');
			}
		}
	}

};
