# Rifle Attachments System

## Background
A tactical counter terrorist game is nothing without the ability to customize the attachments of a weapon. Adding a scope, adding a muzzle brake, etc... All the hallmarks of modern video gaming, they contain loadout customizations. 

## The command
We should have a command namespace that would give the user access to all the necessary operations that can be performed on a weapon:

- [ ] `forge:add <attachment> <weapon>`
	- Adds an attachment to a weapon
	- Examples:
	- `forge:add scope g36c`
- [ ] `forge:remove <attachment> <weapon>`
	- Removes an attachment
	- Examples:
	- `forge:remove scope g36c`
- [ ] `forge:strip <weapon>`
	- Removes all attachments from a weapon and stores them in your inventory

## Refactoring list
We will need to refactor the codebase to allow for a smooth transition into this feature set.
- [ ] Rifle Attachments
	- [ ] Use this class to perform all CRUD operations on a weapon when any of the `forge:*` commands are called
	- [ ] Change the way rifle attachments are stored in the db so that *instances* of attachments and *instances* of rifles are saved/loaded to/from the DB respectively.
- [ ] Ranged Combat Totals
	- [ ] Anytime a weapon has it's attachments modified, the ranged combat totals need to be recalculated to account for the changes.
	- [ ] Verify that RCT's accurately reflect modifications made to a weapon.

## Code references
- For `forge:*` commands look in `src/mods/rifle-attachments.cpp`
