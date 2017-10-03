<?php
define("WORLD_DIR",realpath(dirname(__FILE__) .'/../../../../lib/world/wld/'));

chdir(WORLD_DIR);

$world_files = glob(WORLD_DIR . '/*.wld');

class Room {
	public $north;
	public $south;
	public $up;
	public $down;
	public $east;
	public $west;
	public $title;
	public $description;
};
$rooms = [];
$dir = [0 => 'north',
	1 => 'east',
	2 => 'south',
	3 => 'west',
	4 => 'up',
	5 => 'down'
];
foreach($world_files as $index => $file_name){
	$begin = true;
	$iteration = 0;
	foreach(file($file_name) as $line){
		++$iteration;
		$line = trim($line);
		$section .= $line;
		switch($iteration){
			case 1:
			preg_match("|^#([0-9]+)|",$line,$matches);
			$rooms[$matches[1]] = new Room();
			$current = $rooms[$matches[1]];
			continue;
			case 2:
				$current->title = $line;
				continue;
			case 3:
				continue;
			case 4:
				continue; //TODO: process bitvector
				break;
			default:
				if(preg_match("|^D([0-9]+)|",$line,$matches)){
					$current_dir = $dir[$matches[1]];
				}
				if($line === "S"){
					$iteration = 0;
					$section = '';
					break;
				}
				if(preg_match("|^([0-9]+) (\-*[0-9]+) ([0-9]+)|",$line,$matches)){
					if(strlen($current_dir) == 0){
						var_dump($section);
					}
					echo "Matched: $current_dir" . var_export($matches,1);
					$current_dir = '';
				}
				if(preg_match("|^D([0-9]+)|",$line,$matches)){
					$tilde_count = 0;
					$process_exits = false;
					//echo $line . PHP_EOL;
				}
				break;
		}
	}
}
