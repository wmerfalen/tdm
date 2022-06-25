# Mud Radio

# Code
- `mods/radio.cpp`
- `mods/orm/radio-station.*p`

# Admin Commands
## Help page
- `admin:radio`

## Create
- `admin:radio:create <station-name> <power>`
	- `power` is the number of rooms from the center of the station
		- dictates how far out the signal goes

## List
- `admin:radio:list`
	- lists all stations currently loaded into memory

## Delete
- `admin:radio:delete <station-name>`
	- `station-name` is the first matched station name based on what characters it starts with.

## Reload
- `admin:radio:reload`
	- Loads all radio stations from the database and into memory for use in the game immediately

## Transmit
- `admin:radio:transmit <station-name> <message>`
	- PRO TIP: use double quotes around the message to enable spaces
	- `station-name` is the first matched station name based on what characters it starts with.
		- example: `admin:radio:transmit n "hello"`
			- this will match any radio station that starts with the letter `n`
		- this field is not case sensitive

# Future plans:
- [ ] players must have a CB radio object
	- [ ] only players with a CB radio object can hear mud radio
