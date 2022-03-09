function clamp(orig, low, hi){
	if(orig < low){
		return low;
	}
	if(orig > hi){
		return hi;
	}
	return orig;
}
function calc(player_level){
	let level = player_level;
	let low = 1.0;
	let high = level * 0.9;
	let low_low = level / 3.5;
	let stat_low = clamp(low_low,low,high);
	let stat_high = clamp(level,low+1,high+2);
	let low_level = level / 3;
	let high_level = level / 1.3;
	let low_low_clamp = 1;
	let low_high_clamp = 4 + level;
	let high_low_clamp = 2;
	let high_high_clamp = 5 + level;
	let uint_low = clamp(low_level, low_low_clamp, low_high_clamp);
	let uint_high = clamp(high_level, high_low_clamp, high_high_clamp);
	let max_stats = level + 10;
	console.log({
		level,
		low,
		high,
		low_low,
		stat_low,
		stat_high,
		low_level,
		high_level,
		low_low_clamp,
		low_high_clamp,
		high_low_clamp,
		high_high_clamp,
		uint_low,
		uint_high,
		max_stats
	});
}
const { argv } = require('process');

calc(parseFloat(argv[2]));
