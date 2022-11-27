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

# `zone_data` table clean-up helpers
Due to the way `zone_data` is built, it's possible to have invalid commands in the table whether it be due to invalid mobs or invalid room vnums. The following commands will give you an SQL `delete` command to run manually against the `zone_data` table.

# Some background context
- When the mud is booted, it will store invalid zone commands in a list. This list is not touched unless you run `admin;zone:clear-invalid-commands` in which case it will be cleared. There currently isn't a way to repopulate that list during runtime directly. Though this list will be repopulated if any admin refreshes the entire zone content list via `admin:refresh-all`.

## get the SQL

- To get an sql `delete` command, run:
```
admin:zone:get-invalid-commands
```
- Example output:
```
delete from zone_data where id IN(1151,1167,1168,1169,1170,1171,1087,1088,1089,1090,1091,1092,1093,1094,1096,1607,1608,1609,1610,1611,1612,1613,1614,1615,1616,1617,1618,1718,1719,1720,1721,1722,1723,1724,1725,1726,1727,1728,1729,1730,1731,1732,1733,1734,1735,1736,1737,1738,1739,1740,1741,1742,1743,1744,1745,1746,1747,1748,1749,1750,1773,1774,1775,1776,1777,1778,1779,1780,1781,1782,1783,1784,1785);
```

- To clear the invalid zone command list mentioned in the "Some background context" section, run:
```
admin:zone:clear-invalid-commands
```


