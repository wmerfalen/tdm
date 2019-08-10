# Prototype Phase
- Item has basic features
- Item describes the object and can be used in game

# Creation phase
- [CONDITIONAL]
	- CONDITION: Item placed in container via zone command
	- OR CONDITION: Item awarded by quest
		- Unique item number placed on item
		- Save in db

# [CONDITIONAL] Decoration phase
- CONDITION: Item belongs to user
	- Decorate item and load sub-structures (i.e.: weapon())

# Item is used
- FOREACH item below, save()
	- Item is attached
	- Item gets attachments
	- Item has properties modified
	- Item gets put in user's inventory

# [CONDITIONAL] User drops item
- Save room and/or container


