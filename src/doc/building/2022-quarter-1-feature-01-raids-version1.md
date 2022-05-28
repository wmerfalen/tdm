# Dynamic mobs for the purpose of raiding
## Javascript functions
I have created a few functions in far.js that allow you to dynamically create an NPC with scaled
stats. For example, `level_10_mp_shotgunner()` demonstrates how I generate a mob dynamically 
and place it in the targetted rooms.

## Refreshing mob, zone, and objects during RUNTIME!!!

It is now possible to build mobs, zones, and objects and have those changes reloaded from
the database and into memory. 

The magic behind all this is through the function: `mods::zone::refresh_mobs_and_zones()`

The interface to this function is demonstrated below:

```
js level_10_mp_shotgunner();
admin:queue:refresh
```

This will run the level 10 mp shotgunner demo function and then request that the system
perform a refresh of mobs and zones. 

If you're asking yourself why we can't just call `admin:refresh-all`, then you're right
to speculate. `admin:refresh-all` works fine if you're manually entering it in at the prompt.
But for whatever reason, it's not possible to have that run inside a javascript file.

For example, let's say i put this in my startup javascript file:

```
/* Is always called when i login */
function main(){
	level_10_mp_shotgunner();
	run([
		'admin:refresh-all',
	]);
}
```

The `level_10_mp_shotgunner()` function will run, but the mud will not properly complete
the refresh.

So, in other words: use `admin:queue:refresh` inside of a javascript file, and use 
`admin:refresh-all` when manually running commands.

## `admin:refresh-all`
Refreshes all mob, zone, and object data. Must be a BUILDER to run this.
- Works if you're entering it in manually
- Doesn't currently work if you're running this in a js file

## `admin:queue:refresh`
The same as `admin:refresh-all`, but instead it is a deferred process.
- Works if you're entering it in manually
- Works from a js file
- This is not immediate. Currently, this mechanism depends on `mods::zone::refresh_tick_resolution()`'s return value
- This controlled by comm.cpp. See: heartbeat function
