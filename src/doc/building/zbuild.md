# Builder documentation: Building zones using the `zbuild` command

# random-item-spawn
	- syntax:
		`zbuild random-item-spawn <room_vnum> <max> <yaml>...[yaml-N]`
	- javascript helper:
		- `place_random_items(max,array_of_yaml_files)`
			- will grab the current room you are in
		- example: 
		```
			run([
				'rbuild pave continue',
				'e',
				place_random_items(3,['melee/crowbar.yml','melee/screwdriver.yml']),
				'rbuild pave off',
			])
		```


