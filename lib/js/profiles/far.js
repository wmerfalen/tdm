/**
 * ================================================================================
 * ================================================================================
 * || SECTION:
 * ||         C O N S T A N T S
 * ||
 * ================================================================================
 * ================================================================================
 * v-constants
 */
var MAX_INT32 = 2147483647;
var MAX_UINT16 = 65535;
var rooms = {
	triton_labs_room: 143,
	triton_labs_room_north: 144,
};
var scripted = {
  triton: {
    room: rooms.triton_labs_room,
    room_north: rooms.triton_labs_room,
    mob: 600,
    volunteer_mob: 601,
    volunteer_room: 147,
		scientist: {
			mob: 600,
			room: rooms.triton_labs_room,
		},
  },
  doctor_land: {
    mob: 603,
    room: 266 /** gear room */,
  },
	magnus_ortem: {
		mob: 604,
		room: rooms.triton_labs_room,
	},
	allied_foods: {
		slaughter_isle_entrance: 653,
	},
	defiler_sequences: {
		deliver_biometric_scanner: 26,
	},
};
var crenshaw = {
  overpass_start: "crenshaw-overpass-start",
  northern_shipping_area_entrance: "crenshaw-northern-shipping-entrance",
};
var cofob = {
  west_atop_stairs: "cofob-west-atop-stairs",
  secondfloor_center: "cofob-secondfloor-center",
  center: "cofobcenter",
  east_exit: "eastexit",
  abbotstart: "abbotstart",
  medhall: "cofob-medhall",
  eng_commons: "cofob-engineering-commons",
  gear_room: "cofob-gear-room",
  armory_entrance: "cofob-armory-entrance",
  mess_hall_upstairs: "cofob-mess-hall-upstairs",
  armory_b2: "cofob-armory-basement-2",
};
var garage_thief_spawn = [393, 394, 395, 396, 398, 400, 399, 401, 402];
var petty_thief_spawn = [316, 317, 322, 323, 324, 325, 306, 308];
var vile_kidnapper_spawn = [
  290, 291, 292, 294, 295, 331, 298, 303, 310, 317, 324,
];
var vile_kidnapper_zone_spawn_max = 2;
var chaotic_meth_addict_spawn = [407, 409, 413, 282, 284, 286, 292, 293];
var chaotic_meth_addict_spawn_max = 1;
var shoplift_spawn = [
	589, 592,600, 603,611,614,622,625,633,
];
var crackhead_spawn = [];

var waypoint = {
  rogue_mp: {
    mob: 407,
  },
  thief: {
    mob: 406,
  },
};
var grim_vnum = "101";
var contracts = {
  experimental_hv_mags: 1,
  weapons_cache_contract_vnum: 2,
  hv_mags_vnum: 3,
  gameplay: {
    gear_up: 4,
    ranged_attacks: 5,
    primary_secondary: 6,
    same_room_attacks: 7,
    weapon_stats_and_help: 8,
    breaching: 9,
    claymores: 10,
    grenades: 11,
    flash_bangs: 12,
    sensor_nades: 13,
    incendiary_nades: 14,
    smoke_nades: 15,
    night_vision: 16,
    thermal_vision: 17,
    cameras: 18,
    nvt_cameras: 19,
    drones: 20,
    hacking: 21,
  },
	raven: {
		eliminate: {
			drone: 22,
			defiler: 23,
		},
	},
	defiler_contracts: {
		fetch_biometric_scanner: 24,
  	deliver_biometric_scanner: 25,
	},
	hellfire_claymore: 26,
	atrocity_mp5_contract: 27,
};
	var psitech = {
		start: 'psi-tech-start',
		sniper_post_west: 'psi-tech-sniper-post-west',
		sniper_post_east: 'psi-tech-sniper-post-east',
		sentry_gun_A: 'psi-tech-sentry-gun-a',
		sentry_gun_B: 'psi-tech-sentry-gun-b',
		drop_off_start: 'psi-tech-drop-off-start',
		south_door: 'psi-tech-south-door',
		guard_A: 'psi-tech-guard-a',
		guard_B: 'psi-tech-guard-b',
		atrium: 'psi-tech-atrium',
		atrium_2: 'psi-tech-atrium-2',
		east_gate: 'psi-tech-east-gate',
		west_gate: 'psi-tech-west-gate',
		hatch: 'psi-tech-hatch',
		demo: {
			entrance: 'psi-tech-demo-entrance',
		},
	};
var mission_types = [
  "Recipe -> grab the various elements and bring them back to the contractee",
  "Eliminate -> kill target obj/mob",
  "Protect -> waves of enemies are on their way. Protect the obj/mob",
  "Explore -> the following area has a mob/obj. We need you to examine the area and report your findings",
  "Raid -> hostiles have barricade themselves in this building. We need you to break in and eliminate all threats",
  "Hostage -> a high value target is being held captive in this building. We need you to make contact",
].join(",");
/**
 * ================================================================================
 * ================================================================================
 * || SECTION:
 * ||         U T I L I T Y  F U N C T I O N S
 * ||
 * ================================================================================
 * ================================================================================
 * v-utils
 */
function is_array(item){
	return Array.isArray(item);
}

/**
 * ================================================================================
 * ================================================================================
 * || SECTION:
 * ||         O V E R P O W E R E D  G U N S
 * ||
 * ================================================================================
 * ================================================================================
 * v-overpowered-guns v-op-guns v-opguns
 */
var op_guns = {
  psg1: [
    {
      nick: "dlance",
      name: "DXC DemonLance XIV",
      theme:
        "anti matter barrel. magazine adds radioactive+incendiary ammunition. the scope multiplies the base zoom level of the basic PSG-1 by 4 adding unprecedented range. while you cant attack in the same room with it, the underbarrel taser allows you to immobilize your enemies so that you can create distance and snipe from far off. the stock has a 3D printer interface that regenerates ammunition for your extended mag.",
      attachments: {
        barrel: "anti-matter-barrel" /** created [2021-04-15] */,
        magazine: "depleted-uranium-magazine" /** created [2021-04-15] */,
        sight: "four-x-sight" /** created [2021-04-15] */,
        stock: "printer-stock" /** created [2021-04-15] */,
        under_barrel: "taser-underbarrel" /** created [2021-04-15] */,
      },
    },
    {
      nick: "pathogen",
      name: "TN-3 Pathogen",
      theme:
        "highly corrosive long range sniper rifle with a close range radioactive shotgun underbarrel. ",
      attachments: {
        magazine: "corrosive-sniper-magazine" /** created 2021-09-10 */,
        under_barrel: "fission-shotgun-underbarrel" /** created 2021-09-10 */,
      },
    },
    {
      nick: "hellfire",
      name: "MK-X Hellfire VI",
      theme:
        "extreme amount of incendiary damage but with a long reload time. the stock regenerates ammo. Unfortunately " +
        "due to the nature of the regeneration mechanism, each bullet carries a high amount of radioactive material.",
      attachments: {
        barrel: "hellfire-vi-barrel" /** created 2021-09-10 */,
        magazine: "hellfire-vi-magazine" /** created 2021-09-10 */,
        sight: "9x-sniper-scope" /** created 2021-09-10 */,
        stock: "clover-regenerative-stock" /** created 2021-09-10 */,
      },
    },
  ],
  g36c: [
    {
      nick: "chemlance",
      name: "Chem-LANCE",
      theme:
        "the caustic barrel guarantees your ammunition has high corrosive damage. the mag grants corrosive and explosive damage. the muzzle adds incendiary damage. visibility will never be a problem when you utilize the thermal or night vision modes of the sight. the frag underbarrel adds an element of surprise",
      attachments: {
        barrel: "caustic-barrel" /** created 2021-09-10 */,
        grip: "stabilizer-grip" /** created 2021-09-10 */,
        magazine: "high-impact-corrosive-mag" /** created 2021-09-10 */,
        muzzle: "ignition-muzzle" /** created 2021-09-10 */,
        sight: "dual-thermal-nv-sight" /** created 2021-09-10 */,
        stock: "titanium-stock" /** created 2021-09-10 */,
        under_barrel: "frag-underbarrel" /** created 2021-09-10 */,
      },
    },
  ],
  mp5: [
    {
      nick: "geiger",
      name: "Geiger's answer",
      theme:
        "depleted uranium barrel poisons your ammunition with radioactive damage. the extended mag adds high velocity radioactive and explosive damage. the muzzle adds anti-matter damage. the control stock and grip reduce recoil and increase accuracy. the underbarrel is a corrosive spray variant",
      attachments: {
        barrel: "depleted-uranium-barrel" /** created 2021-09-10 */,
        grip: "stabilizer-grip" /** created 2021-09-10 */,
        magazine: "hv-neutron-extended-mag" /** created 2021-09-10 */,
        muzzle: "anti-matter-muzzle" /** created 2021-09-10 */,
        sight: "reflex-sight" /** created 2021-09-10 */,
        stock: "control-stock" /** created 2021-09-10 */,
        under_barrel: "corrosive-spray-underbarrel" /** created 2021-09-10 */,
      },
    },
    {
      nick: "shrad",
      name: "Shrapnel decimator",
      theme:
        "get the burst or automatic fire of an smg but with the added bonus of a barrel that causes explosive damage on top of your high velocity shrapnel ammunition. the heat dispatch muzzle adds incendiary damage. get the accuracy of the holographic sight which adds anti-matter damage. includes shotgun underbarrel with 6 shells",
      attachments: {
        barrel: "dual-explosive-barrel" /** created 2021-09-10 */,
        magazine: "hv-shrapnel-discharge-mag" /** created 2021-09-10 */,
        muzzle: "heat-dispatch-muzzle" /** created 2021-09-10 */,
        sight: "holographic-anti-matter-sight" /** created 2021-09-10 */,
        stock: "control-stock" /** created 2021-09-10 */,
        under_barrel: "six-shell-shotgun-underbarrel" /** created 2021-09-10 */,
      },
    },
  ],
  sasg12: [
    {
      nick: "alien",
      name: "Alienator",
      theme:
        "smoke underbarrel, explosive regenerating shells, anti-matter barrel. the sight grants the user thermal and night vision for a powerful combination of blinding your enemies using the smoke grenade underbarrel and infiltrating the room unnoticed.",
      attachments: {
        magazine: "",
        barrel: "anti-matter-barrel" /** created 2021-09-10 */,
        stock: "explosive-shell-printer-stock" /** created 2021-09-10 */,
        strap: "tactical-strap" /** created 2021-09-10 */,
        under_barrel: "paralyzing-smoke-underbarrel" /** created 2021-09-10 */,
        sight: "dual-thermal-nv-sight" /** created 2021-09-10 */,
      },
    },
  ],
};
/**
 * ================================================================================
 * ================================================================================
 * || SECTION:
 * ||         S C R I P T E D  S E Q U E N C E S
 * ||
 * ================================================================================
 * ================================================================================
 * v-scripted-sequences
 */
function delete_scripted_sequence_by_vnum(vnum) {
  var c = "seqbuild ";
  vrun([c + "delete " + vnum]);
}
function delete_scripted_steps_by_sequence_vnum(vnum) {
  var c = "seqbuild ";
  vrun([c + "delete-steps " + vnum]);
}
function register_scripted_sequence(vnum, seq) {
  var c = "seqbuild ";
  vrun([c + "new " + vnum]);
  var order = 0;
  for (var i = 0; i < seq.length; i++) {
    vrun([c + "new-step " + vnum]);
    for (var key in seq[i]) {
      vrun([
        c +
          " set-step-data " +
          vnum +
          " " +
          i +
          " s_" +
          key +
          " " +
          seq[i][key],
      ]);
    }
    vrun([
      c + " set-step-data " + vnum + " " + i + [" s_order", order++].join(" "),
    ]);
  }
  vrun([
    c + " save-all-steps " + vnum,
    c + " load-steps " + vnum,
    c + " show-steps " + vnum,
  ]);
}
function register_contract_step_callback(
  contract,
  type,
  target,
  vnum,
  seq_vnum
) {
  var c = "seqbuild ";
  vrun([[c, "callback", contract, type, target, vnum, seq_vnum].join(" ")]);
}
function do_triton_scripted_content() {
  var greet_vnum = 1;
  var give_mags_vnum = 2;
  var convnum = contracts.experimental_hv_mags; // contract vnum
  delete_scripted_sequence_by_vnum(greet_vnum);
  delete_scripted_steps_by_sequence_vnum(greet_vnum);
  delete_scripted_sequence_by_vnum(give_mags_vnum);
  delete_scripted_steps_by_sequence_vnum(give_mags_vnum);
  register_scripted_sequence(
    greet_vnum /** virtual number of this sequence */,
    [
      {
        wait_ticks: 9,
        dialogue:
          "$n stands here in his white lab coat tending to some very interesting and extremely complex looking devices. Completely immersed in whatever hs is working on, he barely notices your presence. Wearily he looks up at you.",
        type: "act",
        mob: scripted.triton.mob,
        room: scripted.triton.room,
      },
      {
        wait_ticks: 9,
        dialogue: "Oh, I did not see you there.. ",
        type: "dialogue",
        mob: scripted.triton.mob,
        room: scripted.triton.room,
      },
      {
        wait_ticks: 3,
        dialogue:
          "You must be here for the high velocity magazines... Follow me...",
        type: "dialogue",
        mob: scripted.triton.mob,
        room: scripted.triton.room,
      },
      {
        wait_ticks: 3,
        dialogue: "$n unlocks the door to the north",
        type: "unlock_north",
        mob: scripted.triton.mob,
        room: scripted.triton.room,
      },
      {
        wait_ticks: 3,
        dialogue: "$n opens the door to the north",
        type: "open_north",
        mob: scripted.triton.mob,
        room: scripted.triton.room,
      },
      {
        wait_ticks: 3,
        type: "walk_north",
        mob: scripted.triton.mob,
        room: scripted.triton.room,
      },
    ]
  );
  register_contract_step_callback(
    convnum /** contract vnum */,
    "GOAL_FIND" /** task type */,
    "TARGET_ROOM" /** task target */,
    scripted.triton.room /** target vnum (room vnum in this case) */,
    greet_vnum /** scripted sequence vnum */
  );
  register_scripted_sequence(
    give_mags_vnum /** virtual number of this sequence */,
    [
      {
        wait_ticks: 3,
        dialogue: "You'll find that these magazines pack quite the punch...",
        mob: scripted.triton.mob,
        room: scripted.triton.room_north,
        type: "say",
      },
      {
        wait_ticks: 3,
        type: "mob_gives_yaml",
        mob: scripted.triton.mob,
        room: scripted.triton.room_north,
        yaml: "attachment/hv-neutron-extended-mag.yml",
        quantity: 4,
      },
      {
        wait_ticks: 3,
        dialogue:
          "Take these magazines and bring them to Doctor Land. He can reverse engineer them and make you more copies if you bring him the right materials.",
        mob: scripted.triton.mob,
        room: scripted.triton.room_north,
        type: "say",
      },
      {
        wait_ticks: 10,
        dialogue: "$n rubs $s temples and forces a smile...",
        mob: scripted.triton.mob,
        room: scripted.triton.room_north,
        type: "act",
      },
      {
        wait_ticks: 3,
        dialogue: "$n cleans $s glasses and sighs in exhaustion.",
        mob: scripted.triton.mob,
        room: scripted.triton.room_north,
        type: "act",
      },
      {
        wait_ticks: 3,
        dialogue:
          "I must get back to my work unfortunately. Do let me know how everything works out. Send my regards to Doctor Land, if you will.",
        mob: scripted.triton.mob,
        room: scripted.triton.room_north,
        type: "say",
      },
      {
        wait_ticks: 1,
        room: scripted.triton.room_north,
        type: "seal_room_south",
      },
      {
        wait_ticks: 1,
        room: scripted.triton.room_north,
        type: "force_south",
      },
    ]
  );
  register_contract_step_callback(
    convnum /** contract vnum */,
    "GOAL_FIND" /** task type */,
    "TARGET_ROOM" /** task target */,
    scripted.triton.room_north /** target vnum (room vnum in this case) */,
    give_mags_vnum /** scripted sequence vnum */
  );
}
function do_seqbuild_test() {
  /**
   * this demonstrates how to do use seqbuild
   * but it is missing a few things.
   */
  vrun([
    "seqbuild new 1",
    "contract_step_callback create 1 task target 4 1",
    "seqbuild new-step 1",
    "seqbuild set-step-data 1 0 s_mob 405",
    "seqbuild set-step-data 1 0 s_yaml rifle/psg1.yml",
    "seqbuild set-step-data 1 0 s_obj 22",
    "seqbuild set-step-data 1 0 s_quantity 33",
    "seqbuild set-step-data 1 0 s_order 1",
    "seqbuild set-step-data 1 0 s_dialogue heh, so whats up with all this stuff, hmm!?",
    "seqbuild set-step-data 1 0 s_type send_to_char",
    "seqbuild set-step-data 1 0 s_sequence_vnum 1",
    "seqbuild set-step-data 1 0 s_room 808",
    "seqbuild set-step-data 1 0 s_wait_ticks 10",
    "seqbuild new-step 1",
    "seqbuild set-step-data 1 1 s_mob 406",
    "seqbuild set-step-data 1 1 s_yaml rifle/p90.yml",
    "seqbuild set-step-data 1 1 s_obj 23",
    "seqbuild set-step-data 1 1 s_quantity 44",
    "seqbuild set-step-data 1 1 s_order 2",
    "seqbuild set-step-data 1 1 s_dialogue whats going on here?",
    "seqbuild set-step-data 1 1 s_type send_to_char",
    "seqbuild set-step-data 1 1 s_sequence_vnum 1",
    "seqbuild set-step-data 1 1 s_room 809",
    "seqbuild set-step-data 1 1 s_wait_ticks 15",
    "seqbuild new-step 1",
    "seqbuild set-step-data 1 2 s_mob 407",
    "seqbuild set-step-data 1 2 s_yaml rifle/mp5.yml",
    "seqbuild set-step-data 1 2 s_obj 24",
    "seqbuild set-step-data 1 2 s_quantity 55",
    "seqbuild set-step-data 1 2 s_order 3",
    "seqbuild set-step-data 1 2 s_dialogue here you go!",
    "seqbuild set-step-data 1 2 s_type send_to_char",
    "seqbuild set-step-data 1 2 s_sequence_vnum 1",
    "seqbuild set-step-data 1 2 s_room 810",
    "seqbuild set-step-data 1 2 s_wait_ticks 20",
    "seqbuild save-step 1 0 1 2",
    "seqbuild show-steps 1",
  ]);
}

/**
 * ================================================================================
 * ================================================================================
 * ================================================================================
 * ================================================================================
 */

/**
 * ================================================================================
 * ================================================================================
 * || SECTION:
 * ||         B O S S E S
 * ||
 * ================================================================================
 * ================================================================================
 * v-bosses v-npc
 */
function build_defiler_boss() {
  var defiler_vnum = 666;
  var vnums = [defiler_vnum];
  var defiler_spawn = [get_vnum("defiler-spawn")];
  clear_mob_zone_commands_for(vnums);
  var mobs = {
    defiler: {
      mbuild: {
        attr: {
          exp: 8050,
          virt: defiler_vnum,
          level: 20,
          mana: 830,
          max_mana: 830,
          hit: 3820,
          max_hit: 3820,
          move: 160,
          max_move: 160,
          damroll: 12,
          gold: 42080,
          weight: 10,
          height: 5,
          strength: 22,
          strength_add: 13,
          intelligence: 12,
          dexterity: 14,
          constitution: 14,
          electronics: 12,
          chemistry: 10,
          strategy: 18,
          marksmanship: 13,
          sniping: 12,
          weapon_handling: 11,
          demolitions: 11,
          armor: 20,
          medical: 1,
          charisma: 1,
          damnodice: 16,
          damsizedice: 16,
          name: "{red}DEFILER{/red}",
          sex: "MALE",
          action: "ISNPC",
          short_description: "{red}DEFILER{/red}",
          long_description: "{red}DEFILER{/red}",
          description: "{red}DEFILER{/red}",
        } /** end attr */,
        "extended-type": "DEFILER",
        "roam-pattern": "Butcher",
      },
      meqbuild: {
        name: "defiler",
        positions: {
					//secondary: "melee/shock-naginata.yml",
          wield: "rifle/defiler-scarh.yml",
          eyes: "armor/night-vision-goggles.yml",
          body: "armor/falcon-ballistic-vest.yml",
          hands: "armor/xm-scorpio-tactical-gloves.yml",
          legs: "armor/mp-enforcer-pants.yml",
          feet: "armor/xm50-ultralight-boots.yml",
        },
      },
      zbuild: {
        command: "mob",
        zone_id: 0,
        mob_vnum: defiler_vnum,
        room_vnum: defiler_spawn,
        max: 1,
        if_flag: 0,
      },
    },
  };
  auto_feed_mob_list(mobs);
}
function build_tengu_scout() {
	var description = [
		"Kojima Corp is a well-funded military force with very high profile ",
		"clients supplying it's income. The most important part of maintaining ",
		"a strategic edge against outside threats is to have a comprehensive plan ",
		"for deep intelligence gathering. The Tengu Scout is part of the fast ",
		"moving ellusive recon team. ",
	].join('');
  var tengu_vnum = 713;
  var vnums = [tengu_vnum];
  var tengu_spawn = kojima.get_spawns('scout');
  clear_mob_zone_commands_for(vnums);
  var mobs = {
    defiler: {
      mbuild: {
        attr: {
          exp: 8050,
          virt: tengu_vnum,
          level: 25,
          mana: 230,
          max_mana: 230,
          hit: 1720,
          max_hit: 1720,
          move: 560,
          max_move: 560,
          damroll: 12,
          gold: 4080,
          weight: 5,
          height: 5,
          strength: 8,
          strength_add: 0,
          intelligence: 49,
          dexterity: 54,
          constitution: 20,
          electronics: 28,
          chemistry: 0,
          strategy: 30,
          marksmanship: 20,
          sniping: 15,
          weapon_handling: 20,
          demolitions: 0,
          armor: 8,
          medical: 0,
          charisma: 0,
          damnodice: 26,
          damsizedice: 13,
          name: "Tengu Scout",
          sex: "MALE",
          action: "ISNPC",
          short_description: "A Tengu Scout",
          long_description: description,
          description: description,
        } /** end attr */,
        //TODO: maybe?"extended-type": "TENGU-TROOPER",
        "extended-type": "ORTHOS_AGENT",
        "roam-pattern": "Kojima",
      },
      meqbuild: {
        name: "tengu-lancer",
        positions: {
					//secondary: "melee/shock-naginata.yml",
          wield: "rifle/tengu-radon-saiga.yml",
          body: "armor/tengu-lightweight-ballistic-vest.yml",
          hands: "armor/xm-scorpio-tactical-gloves.yml",
          legs: "armor/mp-enforcer-pants.yml",
          feet: "armor/xm50-ultralight-boots.yml",
        },
      },
      zbuild: {
        command: "mob",
        zone_id: 0,
        mob_vnum: tengu_vnum,
        room_vnum: tengu_spawn,
        max: 1,
        if_flag: 0,
      },
    },
  };
  auto_feed_mob_list(mobs);
}
function build_tengu_lancer() {
	var description = [
		"The large shoulder armor and upper body shielding give the ",
		"Lancer an appearance not unlike the heavily padded athletes ",
		"of most high contact sports. The legs of the Lancer are not ",
		"armored except for some black shin guards and stealth-optimized ",
		"footwear. The lancer is 6 feet tall and extremely agile. ",
	].join('');

  var tengu_vnum = 714;
  var vnums = [tengu_vnum];
  var tengu_spawn = kojima.get_spawns('lancer');
  clear_mob_zone_commands_for(vnums);
  var mobs = {
    defiler: {
      mbuild: {
        attr: {
          exp: 8050,
          virt: tengu_vnum,
          level: 28,
          mana: 930,
          max_mana: 930,
          hit: 4820,
          max_hit: 4820,
          move: 160,
          max_move: 160,
          damroll: 12,
          gold: 42080,
          weight: 10,
          height: 5,
          strength: 11,
          strength_add: 0,
          intelligence: 29,
          dexterity: 34,
          constitution: 10,
          electronics: 8,
          chemistry: 0,
          strategy: 0,
          marksmanship: 33,
          sniping: 32,
          weapon_handling: 31,
          demolitions: 0,
          armor: 9,
          medical: 0,
          charisma: 0,
          damnodice: 36,
          damsizedice: 20,
          name: "Tengu Lancer",
          sex: "MALE",
          action: "ISNPC",
          short_description: "A Tengu Lancer",
          long_description: description,
          description: description,
        } /** end attr */,
        //TODO: maybe? "extended-type": "TENGU-LANCER",
        "extended-type": "ORTHOS_AGENT",
        "roam-pattern": "Kojima",
      },
      meqbuild: {
        name: "tengu-lancer",
        positions: {
					//secondary: "melee/shock-naginata.yml",
          wield: "rifle/tengu-vulkan-ax13.yml",
          eyes: "armor/tengu-spectrum-ax13-goggles.yml",
          body: "armor/falcon-ballistic-vest.yml",
    			shoulders_l: "armor/titan-shoulder-pads.yml",
    			shoulders_r: "armor/titan-shoulder-pads.yml",
          hands: "armor/xm-scorpio-tactical-gloves.yml",
          legs: "armor/tengu-shin-guards.yml",
          feet: "armor/tengu-stealth-footwear.yml",
        },
      },
      zbuild: {
        command: "mob",
        zone_id: 0,
        mob_vnum: tengu_vnum,
        room_vnum: tengu_spawn,
        max: 1,
        if_flag: 0,
      },
    },
  };
  auto_feed_mob_list(mobs);
}
function build_tengu_trooper() {
	var description = [
		"Optimized for assault, the Tengu Trooper is the epitome of ",
		"brute force. While not being the most strategic unit, the Trooper ",
		"makes up for this with fast attack speed and resilience. ",
		"The Trooper's lightweight ballistic protection sacrifices ",
		"damage absorption for speed.",
	].join('');
  var tengu_vnum = 715;
  var vnums = [tengu_vnum];
  var tengu_spawn = kojima.get_spawns('trooper');
  clear_mob_zone_commands_for(vnums);
  var mobs = {
    defiler: {
      mbuild: {
        attr: {
          exp: 8050,
          virt: tengu_vnum,
          level: 20,
          mana: 830,
          max_mana: 830,
          hit: 3820,
          max_hit: 3820,
          move: 160,
          max_move: 160,
          damroll: 12,
          gold: 42080,
          weight: 10,
          height: 5,
          strength: 11,
          strength_add: 0,
          intelligence: 9,
          dexterity: 34,
          constitution: 10,
          electronics: 8,
          chemistry: 0,
          strategy: 0,
          marksmanship: 23,
          sniping: 22,
          weapon_handling: 21,
          demolitions: 0,
          armor: 9,
          medical: 0,
          charisma: 0,
          damnodice: 16,
          damsizedice: 16,
          name: "Tengu Trooper",
          sex: "MALE",
          action: "ISNPC",
          short_description: "A Tengu Trooper",
          long_description: description,
          description: description,
        } /** end attr */,
        //TODO: maybe?"extended-type": "TENGU-TROOPER",
        "extended-type": "ORTHOS_AGENT",
        "roam-pattern": "Kojima",
      },
      meqbuild: {
        name: "tengu-lancer",
        positions: {
					//secondary: "melee/shock-naginata.yml",
          wield: "rifle/tengu-radon-saiga.yml",
          body: "armor/tengu-lightweight-ballistic-vest.yml",
          hands: "armor/xm-scorpio-tactical-gloves.yml",
          legs: "armor/mp-enforcer-pants.yml",
          feet: "armor/xm50-ultralight-boots.yml",
        },
      },
      zbuild: {
        command: "mob",
        zone_id: 0,
        mob_vnum: tengu_vnum,
        room_vnum: tengu_spawn,
        max: 1,
        if_flag: 0,
      },
    },
  };
  auto_feed_mob_list(mobs);
}
function build_tengu_eviscerator() {
	var description = [
		"Few of the militaristic units deployed by Kojima Corp actually ",
		"fit the classical idea of a 'soldier'. The Tengu Eviscerator ",
		"hardly qualifies as a living breathing human. The technology ",
		"fused into the very biology of the Eviscerator borders of ",
		"extreme cybermancy and archaic magic. As such, the Eviscerator ",
		"is usually only used where a high amount of security is needed. ",
	].join('');
  var tengu_vnum = 716;
  var vnums = [tengu_vnum];
  var tengu_spawn = kojima.get_spawns('eviscerator');
  clear_mob_zone_commands_for(vnums);
  var mobs = {
    defiler: {
      mbuild: {
        attr: {
          exp: 9050,
          virt: tengu_vnum,
          level: 25,
          mana: 930,
          max_mana: 930,
          hit: 5820,
          max_hit: 5820,
          move: 360,
          max_move: 360,
          damroll: 32,
          gold: 42080,
          weight: 10,
          height: 5,
          strength: 11,
          strength_add: 9,
          intelligence: 29,
          dexterity: 34,
          constitution: 30,
          electronics: 0,
          chemistry: 0,
          strategy: 23,
          marksmanship: 23,
          sniping: 22,
          weapon_handling: 21,
          demolitions: 30,
          armor: 29,
          medical: 0,
          charisma: 0,
          damnodice: 20,
          damsizedice: 30,
          name: "Tengu Eviscerator",
          sex: "MALE",
          action: "ISNPC",
          short_description: "A Tengu Eviscerator",
          long_description: description,
          description: description,
        } /** end attr */,
        //TODO: maybe?"extended-type": "TENGU-TROOPER",
        "extended-type": "ORTHOS_AGENT",
        "roam-pattern": "Kojima",
      },
      meqbuild: {
        name: "tengu-eviscerator",
        positions: {
					//secondary: "melee/shock-naginata.yml",
          wield: "rifle/tengu-vulkan-ax13.yml",
          eyes: "armor/tengu-spectrum-ax13-goggles.yml",
          body: "armor/falcon-ballistic-vest.yml",
    			shoulders_l: "armor/titan-shoulder-pads.yml",
    			shoulders_r: "armor/titan-shoulder-pads.yml",
          hands: "armor/xm-scorpio-tactical-gloves.yml",
          legs: "armor/tengu-shin-guards.yml",
          feet: "armor/xm50-ultralight-boots.yml",
        },
      },
      zbuild: {
        command: "mob",
        zone_id: 0,
        mob_vnum: tengu_vnum,
        room_vnum: tengu_spawn,
        max: 1,
        if_flag: 0,
      },
    },
  };
  auto_feed_mob_list(mobs);
}

function build_defiler_boss_contract() {
  var defiler_vnum = 666;
  var vnums = [defiler_vnum];
  var defiler_spawn = [get_vnum("defiler-spawn")];
  clear_mob_zone_commands_for(vnums);
	/** 
	 * Defiler contract
	 */
  var result = build_contract([
    {
      vnum: contracts.defiler_contracts.fetch_biometric_scanner,
      title: "Get the BIOMETRIC scanner from Doctor Land",
      description: [
				"We need you to go to the TRITON Lab and ask Doctor Land for the ",
				"BIOMETRIC scanner. He should be expecting you.",
        "{yel}Here are the contract instructions:{/yel}",
        "{yel}1) {grn}Find {blu}Doctor Land{/blu} {grn}in the TRITON Lab{/grn}",
        "{yel}2) {grn}{yel}ask{/yel} {blu}Doctor Land{/blu}{grn} for the BIOMETRIC scanner{/grn}",
      ].join("\r\n"),
      steps: [
        {
          task_type: "GOAL_FIND",
          task_target: "TARGET_ROOM",
          room_vnum: scripted.doctor_land.room,
          reward_xp: 120,
          reward_money: 0,
          description: [
						"You've got the scanner? Great! Now head to Allied Foods marketplace. It is northeast from here.",
						"You will want to look for the {yel}Slaughter Isle{/yel}. Use your BIOMETRIC scanner to ",
						"check the meat products for the presence of {red}HUMAN REMAINS{/red}",
						"\r\n",
						"Once you've found positive results we will radio you with your next objective.",
          ].join("\r\n"),
        },
        {
          task_type: "GOAL_CUSTOM_EVENT",
          task_target: "TARGET_ITEM",
          room_vnum: scripted.allied_foods.slaughter_isle_entrance,
          reward_xp: 1100,
          reward_money: 560,
					/** Normally, we'd use quota: 4, but that is handled by C++ */
          description: [
            "Locate the slaughter isle to begin testing the foods using your biometric scanner.",
						"We need atleast 4 positive bioscan results.",
						"{blu}Hint: type {yel}bioscan <target>{/yel}"
          ].join(""),
        },
        {
          task_type: "GOAL_FIND",
          task_target: "TARGET_ROOM",
          room_vnum: scripted.triton.room,
          reward_xp: 850,
          reward_money: 500,
          description: [
            "Take the biometric scanner back to Doctor Magnus Ortem. We will need to decide ",
						"next steps."
          ].join(""),
        },
        {
          task_type: "GOAL_FIND",
          task_target: "TARGET_MOB",
          mob_vnum: scripted.doctor_land.mob,
          reward_xp: 50,
          reward_money: 50,
          description: ["Go back to Doctor Land with the biometric scanner."].join(""),
        },
        {
          task_type: "GOAL_GIVE",
          task_target: "TARGET_MOB",
          mob_vnum: scripted.doctor_land.mob,
          reward_xp: 1150,
          reward_money: 1150,
          description: [
            "Give the biometric scanner to Doctor Land.",
          ].join(""),
        },
      ],
    },
  ]);

	/** defiler scripted content */
	var deliver_seq_vnum = scripted.defiler_sequences.deliver_biometric_scanner;
  delete_scripted_sequence_by_vnum(deliver_seq_vnum);
  delete_scripted_steps_by_sequence_vnum(deliver_seq_vnum);

  register_scripted_sequence(
    deliver_seq_vnum, /** virtual number of this sequence */
    [
      {
        wait_ticks: 9,
        dialogue:
          "$n looks up from a messy desk full of detailed readouts..",
        type: "act",
        mob: scripted.triton.mob,
        room: scripted.triton.room,
      },
      {
        wait_ticks: 9,
        dialogue: "The scanner? Give it here...",
        type: "dialogue",
        mob: scripted.triton.mob,
        room: scripted.triton.room,
      },
      {
        wait_ticks: 3,
        dialogue:
          "$n attaches a long orange cable to the bottom of the biometric scanner",
        type: "act",
        mob: scripted.triton.mob,
        room: scripted.triton.room,
      },
      {
        wait_ticks: 3,
        dialogue: "Yes... these are definitely human remains. Though I do see an advanced type of mutation occurring. " +
				"It's almost as if this sample were taken off of a living breathing human. I don't understand how this sort of " +
				"thing could be possible. It defies all scientific explanation.",
        type: "dialogue",
        mob: scripted.triton.mob,
        room: scripted.triton.room,
      },
      {
        wait_ticks: 3,
        dialogue: "$n types a few commands into a computer terminal.",
        type: "act",
        mob: scripted.triton.mob,
        room: scripted.triton.room,
      },
      {
        wait_ticks: 3,
        dialogue: "I will get these results to my international colleagues for further study. " + 
				"I will give Corporal Crawford the results of my studies. That is all for now...",
        type: "dialogue",
        mob: scripted.triton.mob,
        room: scripted.triton.room,
      },
      {
        wait_ticks: 3,
        dialogue: "$n rubs his temples in an effort to quell a severe migraine...",
        type: "act",
        mob: scripted.triton.mob,
        room: scripted.triton.room,
      },
    ]
  );
  register_contract_step_callback(
    contracts.defiler_contracts.fetch_biometric_scanner, /** contract vnum */
    "GOAL_GIVE", /** type */
    "TARGET_MOB", /** target */
    scripted.triton.room, 	/** room vnum */
    deliver_seq_vnum /** scripted sequence vnum */
  );
}


/**
 * ================================================================================
 * ================================================================================
 * || SECTION:
 * ||         P R I V A T E  C O N T R A C T O R S
 * ||
 * ================================================================================
 * ================================================================================
 * v-private-contactors
 */
function build_raven_contracting() {
  var entrance = [
		'RAVEN contracting is a private contracting firm with deep ties to ',
		'international entities. Their clients are officials within the intelligence ',
		'community. Every operation is off the books and covered up. The room is ',
		'well-lit with a cool breeze coming in from the central air conditioning. ',
		"The insignia for RAVEN contracting is on the wall to the west: an upside ",
		"down triangle with a horizontal line crossing through it. A single black ",
		"feather is at the bottom of the upside down triangle.",
  ].join("");
	var consulting_room = [
		"A small confined room with wooden paneling and very little as far as ",
		"aesthetics. There is a wooden desk with a computer screen facing away from you ",
		"and a man sitting behind the desk in a large office chair. {yel}Lane Crawford{yel} ",
		"regards you with a stern cold gaze as you enter the room.",
	].join('');


	set_title_prefix("{blu}RAVEN Contracting{/blu}");
  run([
    "recall",
    repeat(2, ["n"]),
    repeat(4, ["w"]),
    "rbuild pave continue",
		'n',
    mark("raven-contracting-entrance"),
    title("Entrance"),
    desc(entrance),
    texture("INSIDE ROOFTOP CARPET"),

    "n",
		mark("lane-crawford-spawn"),
    title("Consulting room"),
    desc(consulting_room),
    texture("INSIDE ROOFTOP CARPET"),
    "n",

		'rbuild pave off',
		'rbuild save-paved 0',
  ]);
	clear_title_prefix();

	/**
	 * Lane Crawford NPC
	 */
  var lane_crawford_vnum = 108;
  var vnums = [lane_crawford_vnum];
  var lane_crawford_spawn = [get_vnum("lane-crawford-spawn")];
  clear_mob_zone_commands_for(vnums);
  var mobs = {
    defiler: {
      mbuild: {
        attr: {
          exp: 0,
          virt: lane_crawford_vnum,
          level: 250,
          mana: MAX_INT32,
          max_mana: MAX_INT32,
          hit: MAX_INT32,
          max_hit: MAX_INT32,
          move: MAX_INT32,
          max_move: MAX_INT32,
          damroll: MAX_INT32,
          gold: 0,
          weight: 10,
          height: 5,
          strength: MAX_UINT16,
          strength_add: MAX_UINT16,
          intelligence: MAX_UINT16,
          dexterity: MAX_UINT16,
          constitution: MAX_UINT16,
          electronics: MAX_UINT16,
          chemistry: MAX_UINT16,
          strategy: MAX_UINT16,
          marksmanship: MAX_UINT16,
          sniping: MAX_UINT16,
          weapon_handling: MAX_UINT16,
          demolitions: MAX_UINT16,
          armor: MAX_UINT16,
          medical: MAX_UINT16,
          charisma: MAX_UINT16,
          damnodice: MAX_UINT16,
          damsizedice: MAX_UINT16,
          name: "Lane Crawford",
          sex: "MALE",
          action: "ISNPC",
          short_description: "Lane Crawford",
          long_description: "Lane Crawford",
          description: "Lane Crawford",
        } /** end attr */,
        "extended-type": "PRIVATE_CONTRACTOR",
      },
      meqbuild: {
        name: "lane-crawford",
        positions: {
					secondary: "rifle/desert-eagle.yml",
          body: "armor/falcon-ballistic-vest.yml",
          legs: "armor/mp-enforcer-pants.yml",
          feet: "armor/xm50-ultralight-boots.yml",
        },
      },
      zbuild: {
        command: "mob",
        zone_id: 0,
        mob_vnum: lane_crawford_vnum,
        room_vnum: lane_crawford_spawn,
        max: 1,
        if_flag: 0,
      },
    },
  };
  auto_feed_mob_list(mobs);

}

/**
 * ================================================================================
 * ================================================================================
 * || SECTION:
 * ||         C O N T R A C T S
 * ||
 * ================================================================================
 * ================================================================================
 * v-contracts
 */
function build_hv_mag_contract() {
  return build_contract([
    {
      vnum: contracts.experimental_hv_mags,
      title: "Experimental {yel}High Velocity{/yel} magazines.",
      description: [
        "Good evening, soldier. According to recent advances in our {yel}TRITON{/yel} sponsored laboratory a new kind of ",
        "ammunition is being built.\r\n\r\n",
        "The blueprint for this ammunition contains heavy amounts of radioactive isotopes from ",
        "recently spent Uranium. Find the scientist in the armory to east of {blu}COBALT{/blu} Main hallway. He will give you ",
        "the magazines he has made so far. Once you've acquired the magazines, I will give you further instructions over radio.\r\n",
        "{yel}Here are the contract instructions:{/yel}\r\n",
        "{yel}1) {grn}Find {blu}Doctor Land{/blu} {grn}in the Gear Room near the shooting range.{/grn}\r\n",
        "{yel}2) {grn}Find the TRITON Labs scientist. He will give you the magazines you need.\r\n",
        "{yel}3) {grn}Take the magazines to {blu}Doctor Land{/blu}.\r\n",
        "{yel}4) {grn}Talk to {blu}Doctor Land{/blu} on next steps...\r\n",
      ].join(""),
      steps: [
        {
          task_type: "GOAL_FIND",
          task_target: "TARGET_ROOM",
          room_vnum: scripted.doctor_land.room,
          reward_xp: 850,
          reward_money: 1150,
          description: [
            "Find Doctor Land. Doctor Land is a ballistics expert and as such runs the majority of the new recruit shooting range\r\n",
            "activities. You can find him in the gear room.",
          ].join(""),
        },
        {
          task_type: "GOAL_FIND",
          task_target: "TARGET_ROOM",
          room_vnum: scripted.triton.room,
          reward_xp: 850,
          reward_money: 1150,
          description: [
            "Locate the TRITON Labs research scientist. He can give you the magazines that Doctor Land requires.\r\n",
          ].join(""),
        },
        {
          task_type: "GOAL_FIND",
          task_target: "TARGET_ROOM",
          room_vnum: scripted.triton.room_north,
          reward_xp: 850,
          reward_money: 5000,
          description: [
            "Follow the TRITON Labs research scientist north to get the magazines.\r\n",
          ].join(""),
        },
        {
          task_type: "GOAL_FIND",
          task_target: "TARGET_MOB",
          mob_vnum: scripted.doctor_land.mob,
          reward_xp: 1150,
          reward_money: 1150,
          description: ["Go back to Doctor Land with the magazines.\r\n"].join(
            ""
          ),
        },
        {
          task_type: "GOAL_GIVE",
          task_target: "TARGET_MOB",
          mob_vnum: scripted.doctor_land.mob,
          reward_xp: 1150,
          reward_money: 1150,
          quota: 4,
          yaml: "attachment/hv-neutron-extended-mag.yml",
          description: [
            "Give all 4 magazines to Doctor Land so that he can find a way to build more.\r\n",
          ].join(""),
        },
      ],
    },
  ]);
}
function build_weapons_cache_contract() {
  return build_contract([
    {
      vnum: contracts.weapons_cache_contract_vnum,
      title: "Find the missing weapon cache.",
      description:
        "There is a weapons cache within the video store. The only problem is we dont know where. Figure out where the cache is at.",
      steps: [
        {
          task_type: "GOAL_FIND",
          task_target: "TARGET_ROOM",
          room_vnum: 143,
          reward_xp: 250,
          reward_money: 150,
          reward_1: "#yaml|explosive/frag-grenade.yml",
        },
        {
          task_type: "GOAL_KILL",
          task_target: "TARGET_MOB",
          mob_vnum: 501 /** mp shotgunner */,
          reward_xp: 250,
          reward_money: 5000,
          reward_1: "#yaml|explosive/frag-grenade.yml",
          reward_2:
            "#deep|rifle/g36c.yml{barrel:silencer.yml,grip:hexon-grip.yml,magazine:extended-ar-mag.yml,muzzle:flash-hider.yml,sight:reflex-sight.yml,stock:titanium-stock.yml,strap:tactical-strap.yml,under_barrel:frag-underbarrel.yml}",
        },
      ],
    },
  ]);
}
/**
 * Introduction to gameplay quests:
 * 1) gear up
 * 	- make your way to the training grounds to the north of here.
 * 	- grab the G36C from the rack
 * 	- grab ammunition for your rifle in the ammo crate
 * 	- make your way west to the shooting range
 * 2) ranged attacks
 *  - scan your surroundings
 * 	- use the snipe command to hit the targets to the south
 * 	- use the snipe command to hit the targets to the east
 * 	- use the snipe command to hit the targets to the west
 * 	- use the snipe command to hit the targets to the down
 * 	- use the snipe command to hit the targets to the up
 * 	- reload your rifle if you run out of ammunition
 * 3) primary and secondary
 * 	- grab a pistol from the rack
 * 	- grab ammunition for your pistol from the ammo crate
 * 	- make your way west to the shooting range
 * 4) ranged attacks
 * 	- go to the same room as the targets to the south
 * 	- hit the targets in the same room as you
 * 5) ranged weaponry help screen
 * 	- each weapon has a maximum and minimum range.
 * 	- each weapon has a set of stats
 * 	- to view this information, "examine" your weapon
 * 6) breaching doors
 * 	- there will come a time when you will need to get on the other side of a door
 * 	however the door is locked or sealed shut. In those cases you can use a breach
 * 	charge to blast your way in.
 * 	- grab a breach charge from the explosives crate
 * 	- <give instructions on how to breach>
 * 7) claymores
 * 	- sometimes you want to protect yourself from being flanked. an effective way
 * 	of doing this is to plant a claymore mine at the foot of a door.
 * 	- grab a claymore mine from the explosives crate
 * 	- plant the claymore to the south
 * 	- watch as corporal jennings triggers that claymore
 * 8) grenades
 * 	- grenades are an effective way of causing lots of damage to a remote target
 * 	- grab a couple grenades from the explosives crate
 * 	- arm and throw a grenade south to where corporal Jennings is at
 * 	- grenades almost always cause shrapnel. be careful
 * 9) flash bang grenades
 * 	- sometimes you will want to disorient your target. this may be an effective
 * 	way of surprising your target and thus allowing you to take them out while
 * 	they are still trying to figure out what's going on.
 * 	- grab a flash bang grenade from the explosives crate
 * 	- throw the flashbang to the south where corporal Jennings is at
 * 	- notice the "disorient" messages
 * 	- snipe corporal jennings and notice the added bonus to your attack
 * 10) sensor grenades
 * 	- sensor grenades are an effective way of gathering intelligence about nearby targets.
 * 	- grab a sensor grenade from the explosives crate
 * 	- throw a sensor grenade to the south
 * 	- look at your surroundings and notice how your overhead map is updated
 * 11) incendiary grenades
 * 	- grenades are great but sometimes you want more damage. incendiary grenades
 * 	are a great way to damage the enemy and at the same time light the room on fire.
 * 	- grab an incendiary grenade from the explosives crate
 * 	- throw the grenade to the south where corporal Jennings is at
 * 	- go south. notice how the room is on fire!
 * 	- you will continue to take damage as long as the room is on fire
 * 12) smoke grenades
 * 13) night vision
 * 14) thermal vision
 * 15) cameras
 * 16) nv/thermal cameras
 * 17) drones
 * 18) hacking
 * 	- line up
 * 	- wires
 *
 */
/**
 * =======================================================
 * gameplay dynamics introduction: gear up
 * =======================================================
 */
var gameplay_intros = [
  {
    title: "Gear up",
    build: function () {
      var steps = [
        " make your way to the training grounds to the north of here.",
        " grab the G36C from the rack",
        " grab ammunition for your rifle in the ammo crate",
        " make your way west to the shooting range",
      ];
      return build_contract([
        {
          vnum: contracts.gameplay.gear_up,
          title: "Gear up, soldier!",
          description:
            "Make your way to the {grn}Shooting Range Area Flacon{/grn} to the North of here. Take the Eastern Hallway and the shooting range will be to the {yel}WEST{/yel}.",
          steps: [
            {
              task_type: "GOAL_FIND",
              task_target: "TARGET_ROOM",
              room_vnum: 145,
              reward_xp: 250,
              reward_money: 150,
              reward_1: "#yaml|explosive/frag-grenade.yml",
            },
            {
              task_type: "GOAL_KILL",
              task_target: "TARGET_MOB",
              mob_vnum: 501 /** mp shotgunner */,
              reward_xp: 250,
              reward_money: 5000,
              reward_1: "#yaml|explosive/frag-grenade.yml",
              reward_2:
                "#deep|rifle/g36c.yml{barrel:silencer.yml,grip:hexon-grip.yml,magazine:extended-ar-mag.yml,muzzle:flash-hider.yml,sight:reflex-sight.yml,stock:titanium-stock.yml,strap:tactical-strap.yml,under_barrel:frag-underbarrel.yml}",
            },
          ],
        },
      ]);
    }, //end function
  },
  {
    title: "Ranged attacks",
    build: function () {
      var steps = [
        "scan your surroundings",
        "use the snipe command to hit the targets to the south",
        "use the snipe command to hit the targets to the east",
        "use the snipe command to hit the targets to the west",
        "use the snipe command to hit the targets to the down",
        "use the snipe command to hit the targets to the up",
        "reload your rifle if you run out of ammunition",
      ];
      return build_contract([
        {
          vnum: contracts.hv_mags_vnum,
          title: "",
          description:
            "There is a weapons cache within the video store. The only problem is we dont know where. Figure out where the cache is at.",
          steps: [
            {
              task_type: "GOAL_FIND",
              task_target: "TARGET_ROOM",
              room_vnum: 145,
              reward_xp: 250,
              reward_money: 150,
              reward_1: "#yaml|explosive/frag-grenade.yml",
            },
            {
              task_type: "GOAL_KILL",
              task_target: "TARGET_MOB",
              mob_vnum: 501 /** mp shotgunner */,
              reward_xp: 250,
              reward_money: 5000,
              reward_1: "#yaml|explosive/frag-grenade.yml",
              reward_2:
                "#deep|rifle/g36c.yml{barrel:silencer.yml,grip:hexon-grip.yml,magazine:extended-ar-mag.yml,muzzle:flash-hider.yml,sight:reflex-sight.yml,stock:titanium-stock.yml,strap:tactical-strap.yml,under_barrel:frag-underbarrel.yml}",
            },
          ],
        },
      ]);
    }, //end function
  },
]; //end gameplay_intros array

function build_contract(data) {
  var order = 0;
  var run_me = [];
  for (var v in data) {
    if (typeof data[v] === "undefined" || typeof data[v].vnum === "undefined") {
      send("Skipping record due to no vnum in contract data");
      continue;
    }
    var vnum = data[v].vnum;
    run_me.push("conbuild delete " + vnum);
    run_me.push("conbuild new " + vnum);
    run_me.push("conbuild title " + vnum + " " + data[v].title);
    run_me.push("conbuild description " + vnum + " " + data[v].description);
    order = 0;
    for (var i = 0; i < data[v].steps.length; i++) {
      run_me.push("conbuild new-step " + vnum);
      for (var k in data[v].steps[i]) {
				var step_value = data[v].steps[i][k];
				if(is_array(step_value)){
					step_value = step_value.join(',');
				}
        run_me.push(
          "conbuild set-step-data " +
            vnum +
            " " +
            i +
            " s_" +
            k +
            " " +
            step_value
        );
      }
      run_me.push(
        "conbuild set-step-data " + vnum + " " + i + " s_order " + order
      );
      run_me.push("conbuild save-step " + vnum + " " + i);
      ++order;
    }
    run_me.push("conbuild save " + vnum);
    run_me.push('conbuild load-steps ' + vnum);
  }
  //for(var i=0; i < run_me.length;i++){
  //	send(run_me[i]);
  //}
  run(run_me);
}

/**
 * ================================================================================
 * ================================================================================
 * ================================================================================
 * ================================================================================
 */

function dlance(level) {
  ira("dlance", level);
}
function giveme_demonlance() {
  run([
    "instantiate_rifle_attachment " +
      instantiate_op("psg1", "DXC DemonLance XIV"),
  ]);
}
function list_op() {
  var page = [];
  for (var base in op_guns) {
    page.push('base: "' + base + '"');
    for (var i = 0; i < op_guns[base].length; i++) {
      page.push(
        'name: "' +
          op_guns[base][i].name +
          ' (ira("' +
          op_guns[base][i].nick +
          '",level))'
      );
    }
  }
  send(page.join("\r\n"));
}
function instantiate_op(base, name, level) {
  for (var key in op_guns[base]) {
    if (op_guns[base][key].name === name) {
      var str = base + ".yml{";
      for (var nkey in op_guns[base][key].attachments) {
        str += nkey + ":" + op_guns[base][key].attachments[nkey] + ".yml";
        str += ",";
      }
      str += "}";
      str = str.replace(/,\}$/, "}");
      str += "#level:" + level;
      send("str: " + str);
      return str;
    }
  }
  send("Okay, going to build string");
}
function ira(nick, level) {
  for (var base in op_guns) {
    for (var i = 0; i < op_guns[base].length; i++) {
      if (op_guns[base][i].nick === nick) {
        run([
          "instantiate_rifle_attachment " +
            instantiate_op(base, op_guns[base][i].name, level),
        ]);
        send("instantiated one for you");
        return;
      }
    }
  }
  send("couldnt find anything by that name");
}

/**
 * ================================================================================
 * ================================================================================
 * ================================================================================
 * ================================================================================
 */

/**
 * ================================================================================
 * ================================================================================
 * || SECTION:
 * ||         F A C T O R Y  F U N C T I O N S
 * ||
 * ================================================================================
 * ================================================================================
 * v-factory
 */
function do_giveme_g36c() {
  cmd("yaml_import rifle g36c.yml");
}

/**
 * ================================================================================
 * ================================================================================
 * ================================================================================
 * ================================================================================
 */

/**
 * ================================================================================
 * ================================================================================
 * || SECTION:
 * ||         T E S T I N G  G R E N A D E S  A N D  G A D G E T S
 * ||
 * ================================================================================
 * ================================================================================
 * v-grenade-tests v-test-grenades v-gadgets
 */
function do_claymore_test() {
  run([
    "n",
    "n",
    "n",
    "w",
    "w",
    "yaml_import explosive claymore-mine.yml",
    "install claymore east",
  ]);
}

function do_flash_nade() {
  cmd("giveme_flash");
  cmd("hold grenade");
  cmd("throw north 1");
}

function do_sensor_nade() {
  cmd("giveme_sensor");
  cmd("hold grenade");
  cmd("throw north 1");
}
function do_camera() {
  cmd("yaml_import gadget camera.yml");
  cmd("get pi");
  cmd("hold pi");
  cmd("install pi north");
}
function do_nv_camera() {
  cmd("yaml_import gadget night-vision-camera.yml");
  cmd("get Night");
  cmd("hold Night");
  cmd("install Night north");
}
function do_thermal_camera() {
  cmd("yaml_import gadget thermal-camera.yml");
  //cmd('get Thermal');
  cmd("hold Thermal");
  cmd("install Thermal north");
}
function do_breach_charge() {
  cmd("yaml_import explosive breach-charge.yml");
  cmd("get breach");
  cmd("hold breach");
  cmd("breach north");
}
function do_thermite_breach_charge() {
  cmd("yaml_import explosive thermite-charge.yml");
  cmd("get Thermite");
  cmd("hold Thermite");
  cmd("thermite north");
}
function frag() {
  for (var i = 0; i < 10; i++) {
    cmd("giveme_frag");
  }
  cmd("hold frag");
  cmd("throw north 1");
}

/**
 * ================================================================================
 * ================================================================================
 * ================================================================================
 * ================================================================================
 */

function dirhl(dir) {
  return ["{wht}", dir[0], "{/wht}{grn}", dir.substr(1), "{/grn}"].join("");
}
function td(index) {
  cmd("rbuild title " + init.phases[index].title);
  cmd("rbuild description " + init.phases[index].description);
}
function interactive_item(item) {
  return "{blu}" + item + "{/blu}";
}
function action(a) {
  return "{yel}" + a + "{/yel}";
}

function yaml_list() {
  return ["rifle", "explosive", "drone", "attachment", "armor", "gadget"];
}

function yaml_example() {
  var t = yaml_list();
  for (var i = 0; i < t.length; i++) {
    cmd("yaml_example " + t[i]);
  }
}

function yaml_import() {
  var t = yaml_list();
  for (var i = 0; i < t.length; i++) {
    send(t[i]);
    cmd("yaml_import " + t[i] + " " + t[i] + ".yml");
  }
}

//yaml_example();
//yaml_import();
//do_sensor_nade();
//do_camera();

var funcs = {
  always: function () {
    cmd("+impl");
    cmd("+god");
    cmd("+build");
  },
  fixtures: {
    snipe_fixture: function () {
      cmd("mbuild instantiate 0");
      cmd("obuild instantiate 2");
      cmd("get xm");
      cmd("builder");
      cmd("yaml_example rifle");
      cmd("yaml_import ls");
      cmd("yaml_import rifle rifle.yml");
    },
    g36c: function (index) {
      var vnum = 14;
      cmd(["obuild attr ", index, " weapon_type:add ASSAULT_RIFLE"].join(""));
      cmd(["obuild flag ", index, " wear_flags TAKE WIELD"].join(""));
      cmd(["obuild flag ", index, " type_flags WEAPON"].join(""));
      cmd(["obuild flag ", index, " value_0 0"].join("")); //unused
      cmd(["obuild flag ", index, " value_1 ", 25].join("")); //Number of damage dice
      cmd(["obuild flag ", index, " value_2 ", 5].join("")); //size of damage dice
      cmd(["obuild flag ", index, " value_3 ", 7].join("")); //size of damage dice
      cmd(["obuild attr ", index, " item_number ", vnum].join(""));
      cmd(["obuild attr ", index, ' name "G36C Assault Rifle"'].join(""));
      cmd(
        [
          "obuild attr ",
          index,
          ' description "A G36C Assault Rifle lies here."',
        ].join("")
      );
      cmd(
        [
          "obuild attr ",
          index,
          ' short_description "A G36C Assault Rifle"',
        ].join("")
      );
      cmd(
        [
          "obuild attr ",
          index,
          ' action_description "A G36C Assault Rifle"',
        ].join("")
      );
      //cmd(["obuild ex ",index, " create 1"].join(""));
      //cmd(["obuild ex ",index, " set 0 g36c \"A G36C\""].join(""));
      cmd("obuild obj_file ", index, ' "g36c.yml"');
    },
  },
  content: {
    vegas_junkyard: function () {
      cmd('rbuild_sandbox pave on "vegas_junkyard"');
      var narrow_hallway = {
        desc: [
          "The hallway in front of you aches with the relentlessness ",
          "of criminal empire. This area has doubled as an avenue for ",
          "criminal activity thanks to the property owner's ability ",
          "to look the other way for the right price. ",
          "There has been an insane amount of foot traffic here as ",
          "evidenced by the worn out condition of the ground. ",
        ],
        textures: ["CEMENT", "INSIDE", "TUNNEL"],
        flammable_chance: 0 /** out of 100 */,
        channel_explosions: [
          { dir: "n", amt: 50.0 } /** out of 100 */,
          { dir: "s", amt: 50.0 } /** out of 100 */,
        ],
        wall_durability: 98 /** out of 100 */,
        content_tags: ["contraband", "tunnel", "smuggling"],
        npc_weapon_drop: {
          rarity: ["common"],
          types: ["PISTOL"],
        },
      };
      var up_the_stairs = {
        desc: [
          "A worn out metal ladder is here leading up to the ground floor of this building. ",
          "Someone has spray painted graffiti all over the wall here, giving you the impression ",
          "that cartel members loiter here.",
        ],
        textures: ["CEMENT", "INSIDE", "LADDER"],
        flammable_chance: 0 /** out of 100 */,
        channel_explosions: [
          { dir: "u", amt: 50.0 } /** out of 100 */,
          { dir: "d", amt: 50.0 } /** out of 100 */,
        ],
        wall_durability: 98 /** out of 100 */,
        content_tags: ["contraband", "tunnel", "smuggling", "graffiti"],
        mutable_objects: [
          {
            name: "metal-ladder",
            mutations: [
              { type: "heat", causes: "burn-hands", counter: "gloves" },
            ],
          },
        ],
        npc_wander: [{ level: [1, 20], group: "cartel" }],
      };

      var ground_level_fork = {
        desc: [
          "The hallway in front of you stretches to the east and the west. ",
          "A ladder is here that leads down to a poorly lit tunnel. ",
          "To the south, you see the entrance to the junkyard's main area.",
        ].join(""),
        title: "A fork in the junkyard hallway",
        textures: ["CEMENT", "INSIDE"],
        flammable_chance: 0 /** out of 100 */,
        channel_explosions: [
          { dir: "w", amt: 50.0 } /** out of 100 */,
          { dir: "e", amt: 50.0 } /** out of 100 */,
        ],
        wall_durability: 98 /** out of 100 */,
        content_tags: ["contraband", "tunnel", "smuggling"],
        npc_weapon_drop: {
          rarity: ["common"],
          types: ["PISTOL"],
        },
      };
      var ground_level_east = {
        desc: [
          "The hallway stretches to the east and the west. ",
          "To the west you can see large glass windows that peer ",
          "into the main area of the junkyard. ",
        ].join(""),
        textures: ["CEMENT", "INSIDE"],
        flammable_chance: 0 /** out of 100 */,
        channel_explosions: [
          { dir: "w", amt: 50.0 } /** out of 100 */,
          { dir: "e", amt: 50.0 } /** out of 100 */,
        ],
        wall_durability: 98 /** out of 100 */,
        content_tags: ["contraband", "tunnel", "smuggling"],
        npc_weapon_drop: {
          rarity: ["common"],
          types: ["PISTOL"],
        },
      };
      var d = [
        narrow_hallway,
        up_the_stairs,
        ground_level_fork,
        ground_level_east,
        ground_level_east_exit,
        ground_level_east_outside,
      ];

      var cmds = [
        { dir: "n", d: 0 },
        { dir: "n", d: 0 },
        { dir: "n", d: 0 },
        { dir: "e", d: 0 },
        { dir: "e", d: 0 },
        { dir: "n", d: 0 },
        { dir: "n", d: 0 },
        { dir: "n", d: 0 },
        { dir: "e", d: 0 },
        { dir: "n", d: 0 },
        { dir: "w", d: 0 },
        { dir: "n", d: 0 },
        { dir: "n", d: 0 },
        { dir: "u", d: 1 },
        { dir: "u", d: 1 },
        { dir: "s", d: 2 },
        { dir: "e", d: 3 },
        { dir: "e", d: 3 },
        { dir: "n", d: 3 },
        { dir: "n", d: 4, door: { dir: "east", def: "closed" } },
        { dir: "e", d: 5 },
        { dir: "w", d: 2 },
        { dir: "s", d: 2 },
        { dir: "s", d: 2 },
        { dir: "w", d: 2 },
        { dir: "w", d: 2 },
      ];

      cmd("rbuild_sandbox pave off");
      cmd("rbuild_sandbox save 0");
    },
    smg_shop: function (index) {
      var desc = [
        "If there ever was a mecca of guns and ammo, Grim's place would ",
        "definitely be a close contender right there behind the U.S. military itself. ",
        "An array of {grn}Sub Machine Guns{/grn} adorn both walls to the east and west ",
        "walls. Behind the counter you hear various automotive mechanic tools. Strange.. ",
        "this place definitely doesn't look like a mechanics shop. A worn out black leather ",
        "chair with it's stuffing bulging out is here rocking back n forth despite nobody ",
        "actually sitting in it.",
      ];
      var cmds = [
        "sbuild attr " + index + ' title "Grim\'s Guns and Ammo"',
        "sbuild attr " + index + ' description "' + desc.join("") + '"',
        "sbuild attr " +
          index +
          ' no_such_item1 "We don\'t carry that sort of thing here."',
        "sbuild attr " +
          index +
          ' no_such_item2 "Look, we don\'t carry that..."',
        "sbuild attr " +
          index +
          ' missing_cash1 "No money, no product. Simple as that."',
        "sbuild attr " +
          index +
          ' missing_cash2 "The precursor to buying guns is that you have to have money upfront..."',
        "sbuild attr " +
          index +
          ' do_not_buy "I don\'t work with those types of items."',
        "sbuild attr " + index + ' message_buy "You got it. Here you go!"',
        "sbuild attr " +
          index +
          ' message_sell "Nice. Maybe I can find some poor schmuck to sell this to..."',
      ];
      for (var i = 0; i < cmds.length; i++) {
        cmd(cmds[i]);
      }
    },
    grim_shopkeeper: function (index) {
      cmd(["mbuild attr ", index, " virt ", grim_vnum].join(""));
      cmd(["mbuild attr ", index, " mana ", 15].join(""));
      cmd(["mbuild attr ", index, " max_mana ", 15].join(""));
      cmd(["mbuild attr ", index, " hit ", 70].join(""));
      cmd(["mbuild attr ", index, " max_hit ", 70].join(""));
      cmd(["mbuild attr ", index, " move ", 30].join(""));
      cmd(["mbuild attr ", index, " max_move ", 30].join(""));
      cmd(["mbuild attr ", index, " damroll ", 5].join(""));
      cmd(["mbuild attr ", index, " gold ", 25000].join(""));
      cmd(["mbuild attr ", index, " level ", 35].join(""));
      cmd(["mbuild attr ", index, " weight ", 10].join(""));
      cmd(["mbuild attr ", index, " height ", 4].join(""));
      cmd(["mbuild attr ", index, " strength ", 2].join(""));
      cmd(["mbuild attr ", index, " strength_add ", 2].join(""));
      cmd(["mbuild attr ", index, " intelligence ", 2].join(""));
      cmd(["mbuild attr ", index, " dexterity ", 2].join(""));
      cmd(["mbuild attr ", index, " constitution ", 2].join(""));
      cmd(["mbuild attr ", index, " charisma ", 2].join(""));
      cmd(["mbuild attr ", index, " damnodice ", 2].join(""));
      cmd(["mbuild attr ", index, " damsizedice ", 5].join(""));
      cmd(
        ["mbuild attr ", index, ' name "dallas gibson gib g grim gibbs" '].join(
          ""
        )
      );
      cmd(["mbuild attr ", index, " sex MALE "].join(""));
      cmd(["mbuild attr ", index, " action SENTINEL ISNPC"].join(""));
      var short_desc = ["{gld}Dallas {red}'Grim'{/red} {gld}Gibson{/gld}"].join(
        ""
      );
      cmd(
        ["mbuild attr ", index, ' short_description "', short_desc, '" '].join(
          ""
        )
      );
      var long_desc = [
        "{gld}Dallas {red}'Grim'{/red} {gld}Gibson the shopkeeper{/gld}",
      ].join("");
      var description = [
        "Short of stature and sharp as an eagle, this man may look like ",
        "an under-powered elderly old man, but underneath that meek exterior ",
        "lies a fierce beast.",
      ].join("");
      cmd(
        ["mbuild attr ", index, ' long_description "', long_desc, '" '].join("")
      );
      cmd(
        ["mbuild attr ", index, ' description "', description, '" '].join("")
      );
      cmd(["mbuild save ", index].join(""));
      return 0;
    },
  },

  tests: {
    triad_range_tests: function () {
      cmd("mbuild instantiate 0");
      cmd("l");
      cmd("obuild instantiate 1");
      cmd("get xm");
      cmd("wield xm");
      cmd("north");
      set_points("far", "hp", 999);
      cmd("snipe c s");
      cmd("snipe c s");
      cmd("s");
      cmd("s");
      cmd("k chef");
      cmd("one_punch");
    },
    sensor_grenade_tests: function () {
      send("this test is incomplete... returning early");
      return;
      cmd("mbuild instantiate 0");
      cmd("l");
      do_nv_camera();
    },
  },
};
function th1() {
  //
  send("stub");
}
function do_fire() {
  cmd("room_fire on KINDLING");
}
function do_fueled_fire() {
  cmd("north");
  cmd("north");
  cmd("rbuild texture:add DRY GRASS");
  cmd("room_fire on");
}

/**
 * ================================================================================
 * ================================================================================
 * || SECTION:
 * ||         R U N N I N G  C O M M A N D S
 * ||
 * ================================================================================
 * ================================================================================
 * v-run
 */
function repeat(count, commands) {
  return { times: count, cmd: commands };
}
/**
 * example: drive('2e4s3w');
 */
function drive(str){
	var numeric = '';
	for(var i=0; i < str.length; i++){
		if(str[i].match(/[0-9]{1}/)){
			numeric += str[i];
			continue;
		}
		if(str[i].match(/[neswud]{1}/)){
			var parsed_numeric = parseInt(numeric,10);
			for(var k=0; k < parsed_numeric; k++){
				cmd(str[i]);
			}
			parsed_numeric = null;
			numeric = '';
			continue;
		}
	}
	return { times: 0, cmd: 'l' };
}
function run(commands) {
  for (var i = 0; i < commands.length; i++) {
    if (commands[i] === false) {
      return;
    }
    if (typeof commands[i] === "object") {
      for (var k = 0; k < commands[i].times; k++) {
        for (var m = 0; m < commands[i].cmd.length; m++) {
          send('running: "' + commands[i].cmd[m] + '"');
          cmd(commands[i].cmd[m]);
        }
      }
      continue;
    }
    send('running: "' + commands[i] + '"');
    cmd(commands[i]);
  }
}
function vrun(commands) {
  for (var i = 0; i < commands.length; i++) {
    send("running: {grn}" + commands[i] + "{/grn}");
    send("[--- command start ---]");
    send("[=====================]");
    cmd(commands[i]);
    send("[=====================]");
    send("[--- command end   ---]");
  }
}

/** Calls admin:queue:refresh */
function refresh(){
	run([
		'admin:queue:refresh',
	]);
}

/**
 * ================================================================================
 * ================================================================================
 * ================================================================================
 * ================================================================================
 */

/**
 * ================================================================================
 * ================================================================================
 * || SECTION:
 * ||         R E V I V E  A N D  R E P O R T I N G
 * ||
 * ================================================================================
 * ================================================================================
 * v-revive
 */
function do_revive_test() {
  run(["mbuild instantiate 0", "set_npc_position 3 INCAP", "revive chef"]);
}
function do_send_report() {
  run(["mbuild instantiate 0", "send_report"]);
}

/**
 * ================================================================================
 * ================================================================================
 * ================================================================================
 * ================================================================================
 */

/**
 * ================================================================================
 * ================================================================================
 * || SECTION:
 * ||         M I N I G U N N E R S
 * ||
 * ================================================================================
 * ================================================================================
 * v-minigunners v-mg
 */
function do_mini_gunner() {
  run(["mbuild instantiate 0", "send_report"]);
}
function do_load_test_mini_gunner() {
  for (var i = 0; i < 5; i++) {
    cmd("mbuild instantiate 0");
  }
}
function do_distributed_load_test_mini_gunner() {
  var inst = function (times) {
    for (var i = 0; i < times; i++) {
      cmd("mbuild instantiate 0");
    }
  };
  run(["n", "n", "n"]);
  inst(3);
  run(["e", "e", "e", "n"]);
  inst(2);
  run(["n", "n", "n", "e", "n"]);
  inst(2);
}
function do_mini_gunner_special(index, room) {
  var mgs = "mbuild mini-gunner-sentinel ";
  run([
    "mbuild extended-type " + index + " MINI_GUNNER_SENTINEL",
    mgs + index + " face-direction NORTH",
    mgs + index + " room-vnum " + room,
    mgs + index + " save",
    "mbuild save " + index,
  ]);
}

/**
 * ================================================================================
 * ================================================================================
 * ================================================================================
 * ================================================================================
 */

function do_chat() {
  run(["chat lol"]);
}
function do_get_noticed() {
  run(["n", "n", "e", "e", "w", "w", "s", "s"]);
}

function do_value_system_sanity_check(value) {
  value_save("SANITY_CHECK", value);
  value_load("SANITY_CHECK");
  return value_sanity_check();
}

var escape_these = {
  mbuild: ["name", "short_description", "long_description", "description"],
  rbuild: [],
};

function conditional_escape(acmd, key, value) {
  if (escape_these[acmd] && escape_these[acmd].indexOf(key) > -1) {
    return '"' + value + '"';
  }
  return value;
}

/**
 * ================================================================================
 * ================================================================================
 * ================================================================================
 * ================================================================================
 */

/**
 * ================================================================================
 * ================================================================================
 * || SECTION:
 * ||         A U T O - F E E D  F U N C T I O N S
 * ||
 * ================================================================================
 * ================================================================================
 * v-auto-feed v-feed
 */
function clear_mob_zone_commands_for(vnums) {
  for (var i = 0; i < vnums.length; i++) {
    cmd(["zbuild delete-by-mob-vnum " + vnums[i]]);
  }
}
function clear_zone_data_entries() {
  cmd(["zbuild remove-all-zone-data-from-db"]);
}
function auto_feed_shop(obj) {
  /*
		        " {red}sbuild{/red} {grn}help{/grn}\r\n" <<
		        "  |--> this help menu\r\n" <<
		        "  {grn}|____[example]{/grn}\r\n" <<
		        "  |:: {red}sbuild{/red} help\r\n" <<
		        "  |:: (this help menu will show up)\r\n" <<
		        " {red}sbuild{/red} {grn}new{/grn}\r\n" <<
		        " {red}sbuild{/red} {grn}list{/grn}\r\n" <<
		        " {red}sbuild{/red} {grn}attr{/grn} <shop_id> <attr> <value>\r\n" <<
		        "  {gld}|:: -:[attributes]:-{/gld}\r\n" <<
		        "  {gld}|:: db_id{/gld}\r\n" <<
		        "  {gld}|:: vnum{/gld}\r\n" <<
		        "  {gld}|:: title{/gld}\r\n" <<
		        "  {gld}|:: description{/gld}\r\n" <<
		        "  {gld}|:: profit_buy{/gld}\r\n" <<
		        "  {gld}|:: profit_sell{/gld}\r\n" <<
		        "  {gld}|:: type{/gld}\r\n" <<
		        "  {gld}|:: no_such_item1{/gld}\r\n" <<
		        "  {gld}|:: no_such_item2{/gld}\r\n" <<
		        "  {gld}|:: missing_cash1{/gld}\r\n" <<
		        "  {gld}|:: missing_cash2{/gld}\r\n" <<
		        "  {gld}|:: do_not_buy{/gld}\r\n" <<
		        "  {gld}|:: message_buy{/gld}\r\n" <<
		        "  {gld}|:: message_sell{/gld}\r\n" <<
		        "  {gld}|:: temper1{/gld}\r\n" <<
		        "  {gld}|:: bitvector{/gld}\r\n" <<
		        "  {gld}|:: keeper{/gld}\r\n" <<
		        "  {gld}|:: with_who{/gld}\r\n" <<
		        "  {gld}|:: open1{/gld}\r\n" <<
		        "  {gld}|:: open22r\n" <<
		        "  {gld}|:: close1{/gld}\r\n" <<
		        "  {gld}|:: close2{/gld}\r\n" <<
		        "  {gld}|:: bankAccount{/gld}\r\n" <<
		        "  {gld}|:: lastsort{/gld}\r\n" <<
		        " {red}sbuild{/red} {grn}save{/grn} <shop_id>\r\n" <<
		        " {red}sbuild{/red} {grn}show{/grn} <shop_id>\r\n" <<
		        " {red}sbuild{/red} {grn}install{/grn} <shop_id>\r\n" <<
		        "  |--> install shop in the room you're standing in\r\n" <<
		        "  {grn}|____[example]{/grn}\r\n" <<
		        "  |:: {red}sbuild{/red} install 2\r\n" <<
		        "  |:: (install shop 2 in this room)\r\n" <<
		        " {red}sbuild{/red} {grn}uninstall{/grn} <shop_id>\r\n" <<
		        "  |--> uninstall shop in the room you're standing in\r\n" <<
		        "  {grn}|____[example]{/grn}\r\n" <<
		        "  |:: {red}sbuild{/red} uninstall 2\r\n" <<
		        "  |:: (uninstall shop 2 in this room)\r\n" <<
		        " {red}sbuild{/red} {grn}add-object{/grn} <shop_id> <object_id>\r\n" <<
		        "  |--> add object_id to shop_id\r\n" <<
		        "  {grn}|____[example]{/grn}\r\n" <<
		        "  |:: {red}sbuild{/red} add-object 2 3\r\n" <<
		        "  |:: (add object 3 into shop number 2)\r\n" <<
		        "  |:: (for a list of objects, type obuild)\r\n" <<
		        " {red}sbuild{/red} {grn}del-object{/grn} <shop_id> <object_id>\r\n" <<
		        "  |--> delete object_id from shop_id\r\n" <<
		        "  {grn}|____[example]{/grn}\r\n" <<
		        "  |:: {red}sbuild{/red} del-object 2 3\r\n" <<
		        "  |:: (removes object 3 from shop number 2)\r\n" <<
		        "  |:: (for a list of objects, type obuild)\r\n" <<
		        " {red}sbuild{/red} {grn}info{/grn}\r\n" <<
		        "  |--> prints information about the shop installed in this room (if any).\r\n" <<
		        "  {grn}|____[example]{/grn}\r\n" <<
		        "  |:: {red}sbuild{/red} info\r\n" <<
		        "  |:: (prints info (if any))\r\n" <<
						*/
}
/** -!- Poster child for: create-or-update mob by vnum -!- */
function auto_feed_mob(obj) {
  var index = exec("mbuild exists " + obj.mbuild.attr.virt);
  if (index === "!") {
    index = exec("mbuild new");
  }
  for (var key in obj.mbuild.attr) {
    cmd(
      [
        "mbuild attr ",
        index,
        " ",
        key,
        " ",
        conditional_escape("mbuild", key, obj.mbuild.attr[key]),
      ].join("")
    );
  }
  if (typeof obj.mbuild["extended-type"] !== "undefined") {
    cmd(
      ["mbuild extended-type ", index, " ", obj.mbuild["extended-type"]].join(
        ""
      )
    );
  }
	cmd([
		"mbuild action:clear ",index
	].join('')
	);
  if (typeof obj.mbuild.attr["action"] !== "undefined") {
		var has_space = obj.mbuild.attr["action"].indexOf(' ');
		var actions = obj.mbuild.attr["action"].split(' ');
		if(has_space){
			for(var i=0; i < actions.length;i++){
				cmd(
					["mbuild action:add ",index," ",actions[i]].join('')
				);
			}
		}else{
			cmd(["mbuild action:add ",index," ",obj.mbuild["action"]].join(''));
		}
	}
  if (typeof obj.mbuild["roam-pattern"] !== "undefined") {
    cmd(
      ["mbuild roam-pattern ", index, " ", obj.mbuild["roam-pattern"]].join("")
    );
  }
  cmd(["mbuild save ", index].join(""));
  if (typeof obj["meqbuild"] !== "undefined") {
    run([
			["meqbuild delete ", obj.mbuild.attr.virt].join(""),
		]);
    run([
      [
        "meqbuild new ",
        obj.meqbuild.name,
        " ",
				obj.mbuild.attr.virt,
      ].join("")
		]);
    for (var key in obj.meqbuild.positions) {
      run([
        [
          "meqbuild set ",
					obj.mbuild.attr.virt,
          " ",
          key,
          " ",
          obj.meqbuild.positions[key],
        ].join("")
			]);
    }
    run([
			["meqbuild save ", obj.mbuild.attr.virt].join(""),
		]);
    run([
      [
        "meqbuild map-assign ",
        obj.mbuild.attr.virt,
        " ",
				obj.mbuild.attr.virt,
      ].join("")
		]);
  }
  run([
		"meqbuild reload-all",
	]);
  if (typeof obj["zbuild"] !== "undefined") {
    run([
			["zbuild delete-by-mob-vnum ", obj.zbuild.mob_vnum].join(""),
		]);
    if (Array.isArray(obj["zbuild"]["room_vnum"])) {
      for (var zbuild_i = 0; zbuild_i < obj["zbuild"]["room_vnum"].length; zbuild_i++) {
        run([
          [
            "zbuild ",
            obj.zbuild.command,
            " ",
            obj.zbuild.zone_id,
            " ",
            obj.zbuild.mob_vnum,
            " ",
            obj.zbuild.room_vnum[zbuild_i],
            " ",
            obj.zbuild.max,
            " ",
            obj.zbuild.if_flag,
          ].join("")
				]);
      }
    } else {
      run([
        [
          "zbuild ",
          obj.zbuild.command,
          " ",
          obj.zbuild.zone_id,
          " ",
          obj.zbuild.mob_vnum,
          " ",
          obj.zbuild.room_vnum,
          " ",
          obj.zbuild.max,
          " ",
          obj.zbuild.if_flag,
        ].join("")
			]);
    }
  }
}
function auto_feed_object(obj) {
  var index = exec("obuild exists " + obj.obuild.attr.item_number);
  if (index === "!") {
    index = exec("obuild new " + obj.obuild.attr.item_number);
  }
  for (var key in obj.mbuild.attr) {
    cmd(
      [
        "obuild attr ",
        index,
        " ",
        key,
        " ",
        conditional_escape("obuild", key, obj.obuild.attr[key]),
      ].join("")
    );
  }
  cmd(["obuild save ", index].join(""));
}
/** -!- Poster child for: create-or-update mob by vnum by passing in object with several mob definitions -!- */
function auto_feed_mob_list(list) {
  for (var name in list) {
    auto_feed_mob(list[name]);
  }
}
function rbuild_pave_continue(commands) {
  run(["rbuild pave continue"]);
  for (var i = 0; i < commands.length; i++) {
    if (commands[i].rbuild) {
      for (var key in commands[i].rbuild) {
        run([
          [
            "rbuild ",
            key,
            " ",
            conditional_escape("rbuild", key, commands[i].rbuild[key]),
          ].join(""),
        ]);
      }
      continue;
    }
    run([commands[i]]);
  }
  var index = exec("rbuild pave off");
  send('To save the paved area, type: "rbuild save-paved ' + index + '"');
}

/**
 * ================================================================================
 * ================================================================================
 * ================================================================================
 * ================================================================================
 */

/**
 * ================================================================================
 * ================================================================================
 * || SECTION:
 * ||         U . S .  V I D E O
 * ||					C O N T E N T
 * ||
 * ================================================================================
 * ================================================================================
 * v-us-video v-usvideo v-content
 */
function do_us_video() {
  /** -!- Poster child for: creating mobs programmatically -!- */
  var mobs = {
    bum: {
      mbuild: {
        attr: {
          exp: 50,
          virt: 401,
          mana: 25,
          max_mana: 25,
          hit: 580,
          max_hit: 580,
          move: 140,
          max_move: 240,
          damroll: 15,
          gold: 5000,
          level: 10,
          weight: 10,
          height: 4,
          strength: 15,
          strength_add: 13,
          intelligence: 12,
          dexterity: 14,
          constitution: 18,
          charisma: 1,
          damnodice: 14,
          damsizedice: 16,
          name: "A weak and twitchy homeless man",
          sex: "MALE",
          action: "ISNPC",
          short_description: "A weak and twitchy homeless man.",
          long_description: "A weak and twitchy homeless man.",
          description:
            "A tired yet wide awake looking bum reaches his arm " +
            "out to you for some spare change. " +
            "You have no idea what he will spend it on.",
        } /** end attr */,
      } /** end mbuild */,
    },
    hooker: {
      mbuild: {
        attr: {
          exp: 50,
          virt: 402,
          mana: 25,
          max_mana: 25,
          hit: 580,
          max_hit: 580,
          move: 140,
          max_move: 240,
          damroll: 15,
          gold: 5000,
          level: 10,
          weight: 10,
          height: 4,
          strength: 15,
          strength_add: 13,
          intelligence: 12,
          dexterity: 14,
          constitution: 18,
          charisma: 1,
          damnodice: 14,
          damsizedice: 16,
          name: "A diseased sex worker",
          sex: "FEMALE",
          action: "ISNPC",
          short_description: "A diseased sex worker",
          long_description: "A diseased sex worker",
          description: "A sex worker stands here offering you cheap services.",
        } /** end attr */,
      } /** end mbuild */,
    },
    clerk: {
      mbuild: {
        attr: {
          exp: 50,
          virt: 403,
          mana: 25,
          max_mana: 25,
          hit: 580,
          max_hit: 580,
          move: 140,
          max_move: 240,
          damroll: 15,
          gold: 5000,
          level: 10,
          weight: 10,
          height: 4,
          strength: 15,
          strength_add: 13,
          intelligence: 12,
          dexterity: 14,
          constitution: 18,
          charisma: 1,
          damnodice: 14,
          damsizedice: 16,
          name: "A store clerk",
          sex: "MALE",
          action: "ISNPC",
          short_description: "A video rental clerk",
          long_description: "A video rental clerk",
          description: "Bored, underpaid, and stoned.",
        } /** end attr */,
      } /** end mbuild */,
    },
    creepycustomer: {
      mbuild: {
        attr: {
          exp: 50,
          virt: 404,
          mana: 25,
          max_mana: 25,
          hit: 580,
          max_hit: 580,
          move: 140,
          max_move: 240,
          damroll: 15,
          gold: 5000,
          level: 10,
          weight: 10,
          height: 4,
          strength: 15,
          strength_add: 13,
          intelligence: 12,
          dexterity: 14,
          constitution: 18,
          charisma: 1,
          damnodice: 14,
          damsizedice: 16,
          name: "A store clerk",
          sex: "MALE",
          action: "ISNPC",
          short_description: "A video rental clerk",
          long_description: "A video rental clerk",
          description: "Bored, underpaid, and stoned.",
        } /** end attr */,
      },
    },
    lowly_security_guard: {
      mbuild: {
        attr: {
          exp: 150,
          virt: 405,
          mana: 45,
          max_mana: 45,
          hit: 880,
          max_hit: 880,
          move: 140,
          max_move: 240,
          damroll: 20,
          gold: 5000,
          level: 15,
          weight: 20,
          height: 5,
          strength: 25,
          strength_add: 23,
          intelligence: 22,
          dexterity: 24,
          constitution: 28,
          charisma: 10,
          damnodice: 24,
          damsizedice: 26,
          name: "A lowly security guard",
          sex: "MALE",
          action: "ISNPC HELPER AGGR_GOOD",
          short_description: "A lowly security guard",
          long_description: "A lowly security guard",
          description:
            "A somewhat physically fit security guard. He looks armed.",
        } /** end attr */,
        "extended-type": "LOWLY_SECURITY",
      },
      meqbuild: {
        name: "lowlysec",
        positions: {
          hold: "gadget/cbradio.yml",
          wield: "rifle/hk45.yml",
          head: "armor/dsf-hat.yml",
          goggles: "armor/sunglasses.yml",
          hands: "armor/spiked-gloves.yml",
          body: "armor/dsf-shirt.yml",
          legs: "armor/black-dickies.yml",
          feet: "armor/basic-boots.yml",
        },
      },
    },
  };
  auto_feed_mob_list(mobs);

  /** -!- Ideal way to create rooms programmatically -!- */
  var usvr_parking_lot = {
    rbuild: {
      title: "Video rental parking lot",
      description: [
        "Black asphault, trash, and empty beer cans litter the mostly devoid ",
        "parking lot. White faded lines suggest where cars should be parked, ",
        "though it seems nobody pays attention to that.",
      ].join(""),
      "sector-type": "OUTSIDE_TRASHY_PARKING_LOT",
    } /** end rbuild */,
    mobs: ["bum", "hooker"],
  };
  var usvr_entrance = {
    rbuild: {
      title: "U-Slurps video rental entrance",
      description: [
        "A sign with big red bold letters sputters on and off with backlit florescent white lights.",
        "A glass door with many stickers on the inside and outside advertise the latest deplorable ",
        "DVDs for rent, and oddly enough, religious propaganda.",
      ].join(""),
      "sector-type": "OUTSIDE_TRASHY_PARKING_LOT",
    } /** end rbuild */,
    mobs: ["bum", "hooker"],
  };
  var usvr_entrance_newstand = {
    rbuild: {
      title: "Newstand room",
      description:
        "A variety of dirty magazines are stacked against the eastern wall. " +
        "To the north is the center isle. As you walk in, you notice the clerk " +
        "stands behind a raised counter on the left.",
      "sector-type": "OUTSIDE_TRASHY_PARKING_LOT",
    } /** end rbuild */,
    mobs: ["creepycustomer"],
  };
  var center_isle = {
    rbuild: {
      title: "Center isle",
      description:
        "A variety of magazines and videos line the isles to the east and west. " +
        "To the north is a door with a sign that says {gld}Video Room{/gld}.",
      "sector-type": "INDOOR_ROOF",
    } /** end rbuild */,
    mobs: ["creepycustomer"],
  };
  var checkout = {
    rbuild: {
      title: "Checkout area",
      description: "<TODO>",
      "sector-type": "INDOOR_ROOF",
    } /** end rbuild */,
    mobs: ["clerk", "lowly_security_guard"],
  };

  rbuild_pave_continue([
    "n",
    usvr_parking_lot,
    "e",
    usvr_parking_lot,
    "w",
    "w",
    usvr_parking_lot,
    "e",
    "n",
    usvr_entrance,
    "n",
    center_isle,
    "n",
    center_isle,
    "n",
    center_isle,
    "s",
    "s",
    "w",
  ]);

  /*
	run([
		'meqbuild new rsf-security 1',
		'meqbuild map-assign 405 1',
		'meqbuild map-assign 401 1',
		'meqbuild map-delete 401 1',
		'meqbuild map-list'
	]);
	*/
  send('To save the paved area, type: "rbuild save-paved 0"');
}
/**
 * ================================================================================
 * ================================================================================
 * ================================================================================
 * ================================================================================
 */

/**
 * ================================================================================
 * ================================================================================
 * || SECTION:
 * ||         P A R K W A Y  P L A Z A
 * ||					C O N T E N T
 * ||
 * ================================================================================
 * ================================================================================
 * v-parkway-plaza v-plaza v-content
 */
function do_parkway_plaza_area() {
  var north_entrance_description = [
    "To the south of you is the Northern entrance to Parkway Plaza ",
    "mall. The mall is indoors except for the parking lot which you ",
    "just came from to the north. A moderate amount of customers can ",
    "be seen entering and leaving this area.",
  ].join("");
  var north_corridor = [
    "Stretching north and south is a long corridor that attaches to the ",
    "main area of the mall. To the east and west you see advertisements ",
    "on the walls.",
  ].join("");
  var north_strip_A = [
    "Stretching north and south is a long corridor that attaches to the ",
    "main area of the mall. To the east and west you see advertisements ",
    "on the walls.",
  ].join("");
  var sparse_wireless_entrance = [
    "{grn}Sparse Wireless{/grn} - {gld}2 Free Phones!{/gld}",
    "The entrance to Sparse Wireless is to the North.",
  ].join("");
  var sparse_wireless_quadrant = [
    "{grn}Sparse Wireless{/grn} - {gld}2 Free Phones!{/gld}",
    "The entrance to Sparse Wireless is to the North.",
  ].join("");
  var FILL_ME = "@FILL_ME@";
  run([
    "rbuild pave continue",
    "s",
    "rbuild title Northern Entrance to Parkway Plaza",
    "rbuild description " + north_entrance_description,
    "s",
    "rbuild title Corridor to northern end of Parkway Plaza",
    "rbuild description " + north_corridor,
    "s",
    "rbuild title Corridor to northern end of Parkway Plaza",
    "rbuild description " + north_corridor,
    "e",
    "rbuild title Northern Strip",
    "rbuild description " + north_strip_A,
    "e",
    "rbuild title Northern Strip",
    "rbuild description " + north_strip_A,
    "e",
    "rbuild title Northern Strip",
    "rbuild description " + north_strip_A,
    "n",
    "rbuild title Sparse Wireless Store Entrance",
    "rbuild description " + sparse_wireless_entrance,
    "n",
    "rbuild title Sparse Wireless",
    "rbuild description " + sparse_wireless_quadrant,
    "w",
    "rbuild title Sparse Wireless Round Desk Area",
    "rbuild description " + sparse_wireless_quadrant,
    "e",
    "n",
    "rbuild title Sparse Wireless",
    "rbuild description " + sparse_wireless_quadrant,
    "n",
    "rbuild title Sparse Wireless - Backroom Entrance",
    "rbuild description " + FILL_ME,
    "n",
    "rbuild title Sparse Wireless - Backroom",
    "rbuild description " + FILL_ME,
    "e",
    "rbuild title Sparse Wireless - Backroom Bathroom Entrance",
    "rbuild description " + FILL_ME,
    "e",
    "rbuild title Sparse Wireless - Backroom Bathroom",
    "rbuild description " + FILL_ME,
    "w",
    "w",
    "w",
    "rbuild title Sparse Wireless - Backroom Inventory Computer",
    "rbuild description " + FILL_ME,
    "e",
    "s",
    "rbuild pave off",
  ]);
  send('To save the paved area, type: "rbuild save-paved 0"');
}

function mud_date() {
  send(get_month() + " " + get_day() + " " + get_iyear());
}
function set_first_tier(value) {
  var key = "LEVELS_FIRST_TIER";
  value_save(key, value);
  value_load(key);
}
/**
 * ================================================================================
 * ================================================================================
 * ================================================================================
 * ================================================================================
 */

/**
 * ================================================================================
 * ================================================================================
 * || SECTION:
 * ||         L I V E  T R A I N I N G
 * ||					C O N T E N T
 * ||
 * ================================================================================
 * ================================================================================
 * v-live-training v-content
 */
function closed_door(direction) {
  return "rbuild exit:add " + direction + " BREACHABLE CLOSED ISDOOR";
}
function place_random_items(max, yaml_list) {
  return "zbuild random-item-spawn " + max + " " + yaml_list.join(" ");
  //" {grn}zbuild{/grn} {red}random-item-spawn <room_vnum> <max> <yaml>...[yaml-N]{/red}\r\n" <<
}
function automatic_door(direction) {
  return (
    "rbuild exit:add " + direction + " AUTOMATIC BREACHABLE CLOSED ISDOOR GLASS"
  );
}
function hidden_thermite(direction) {
  return (
    "rbuild exit:add " +
    direction +
    " BREACHABLE PICKPROOF CLOSED REINFORCED HIDDEN ISDOOR"
  );
}
var title_prefix = '';
function title(t) {
	if(title_prefix.length){
  	return "rbuild title " + title_prefix + ' - ' + t;
	}
	return 'rbuild title ' + t;
}
function set_title_prefix(p){
	title_prefix = p;
}
function clear_title_prefix(){
	set_title_prefix('');
}
function desc(d) {
  return "rbuild description " + d;
}
function texture(list) {
  return "rbuild texture:add " + list;
}
function save(){
	return "rbuild save";
}
function mark(nick) {
  return "bookmark " + nick + " this";
}
function tie(dir, nick) {
  return "rbuild bind-mark " + dir + " " + nick;
}
function go(nick) {
  return "goto " + nick;
}
function get_vnum(nick) {
  return exec("print_vnum " + nick);
}
function dirgen(direction, general_description) {
  return "rbuild dopt " + direction + " gen " + general_description;
}
function dirkeyword(direction, keyword) {
  return "rbuild dopt " + direction + " keyword " + keyword;
}
function dirkey(direction, key) {
  return "rbuild dopt " + direction + " key " + key;
}
function dirto(direction, to_room) {
  return "rbuild dopt " + direction + " to_room " + to_room;
}
function dirtovnum(direction, room_vnum) {
  return "rbuild dopt " + direction + " to_vnum " + room_vnum;
}
function dirtomark(direction, bookmark) {
  return "rbuild dopt " + direction + " to_mark " + bookmark;
}
function generate_rectangle(height, width) {
  var x = width,
    y = 0;
  for (y = 0; y < height; y++) {
    run([
      repeat(1, [
        "n",
        title("foobar " + x + "x" + y),
        desc("foobar " + x + "x" + y),
        mark("foobar-" + x + "x" + y),
      ]),
    ]);
  }
  --y;
  for (x = width - 1; x > 0; x--) {
    run([
      repeat(1, [
        "w",
        title("foobar " + x + "x" + y),
        desc("foobar " + x + "x" + y),
        mark("foobar-" + x + "x" + y),
      ]),
    ]);
  }
  ++x;
  --y;
  for (m = 0; m < height - 1; m++, y--) {
    run([
      repeat(1, [
        "s",
        title("foobar " + x + "x" + y),
        desc("foobar " + x + "x" + y),
        mark("foobar-" + x + "x" + y),
      ]),
    ]);
  }
  ++y;
  ++x;
  for (; x <= width - 1; x++) {
    run([
      repeat(1, [
        "e",
        title("foobar " + x + "x" + y),
        desc("foobar " + x + "x" + y),
        mark("foobar-" + x + "x" + y),
      ]),
    ]);
    for (var yy = 1; yy < height - 1; yy++) {
      run([
        repeat(1, [
          "n",
          title("foobar " + x + "x" + yy),
          desc("foobar " + x + "x" + yy),
          mark("foobar-" + x + "x" + yy),
          tie("west", ["foobar-", x - 1, "x", yy].join("")),
        ]),
      ]);
      if (x == width - 1) {
        run([tie("east", ["foobar-", x + 1, "x", yy].join(""))]);
      }
    }
    run([tie("north", ["foobar-", x, "x", yy].join(""))]);
    run([repeat(height - 2, ["s"])]);
  }
  run([tie("east", ["foobar-", x, "x", 0].join(""))]);
}

function generate_rectangle_to_the_west(prefix, height, width, starting_y) {
  var x = width,
    y = starting_y;
  run([mark(prefix + "-starting-point")]);
  for (; y < height; y++) {
    run([
      repeat(1, [
        "n",
        title(prefix + x + "x" + y),
        desc(prefix + x + "x" + y),
        mark(prefix + "-" + x + "x" + y),
      ]),
    ]);
  }
  --y;
  for (x = width - 1; x > 0; x--) {
    run([
      repeat(1, [
        "w",
        title(prefix + x + "x" + y),
        desc(prefix + x + "x" + y),
        mark(prefix + "-" + x + "x" + y),
      ]),
    ]);
  }
  ++x;
  --y;
  for (m = 0; m < height - 1; m++, y--) {
    run([
      repeat(1, [
        "s",
        title(prefix + x + "x" + y),
        desc(prefix + x + "x" + y),
        mark(prefix + "-" + x + "x" + y),
      ]),
    ]);
  }
  ++y;
  ++x;
  for (; x <= width - 1; x++) {
    run([
      repeat(1, [
        "e",
        title(prefix + x + "x" + y),
        desc(prefix + x + "x" + y),
        mark(prefix + "-" + x + "x" + y),
      ]),
    ]);
    for (var yy = 1; yy < height - 1; yy++) {
      run([
        repeat(1, [
          "n",
          title(prefix + x + "x" + yy),
          desc(prefix + x + "x" + yy),
          mark(prefix + "-" + x + "x" + yy),
          tie("west", [prefix + "-", x - 1, "x", yy].join("")),
        ]),
      ]);
      if (x == width - 1) {
        run([tie("east", [prefix + "-", x + 1, "x", yy].join(""))]);
      }
    }
    run([tie("north", [prefix + "-", x, "x", yy].join(""))]);
    run([repeat(height - 2, ["s"])]);
  }
  run([
    go([prefix + "-", width, "x", starting_y].join("")),
    "s",
    tie("west", [prefix + "-", width - 1, "x", starting_y - 1].join("")),
    "w",
    tie("east", [prefix + "-starting-point"].join("")),
    go(prefix + "-starting-point"),
  ]);
  x = width;
  for (y = starting_y - 2; y >= 0; y--) {
    run([
      repeat(1, [
        "s",
        title(prefix + x + "x" + y),
        desc(prefix + x + "x" + y),
        mark(prefix + "-" + x + "x" + y),
        tie("west", [prefix + "-", x - 1, "x", y].join("")),
      ]),
    ]);
  }
}

function po() {
  run(["rbuild pave off"]);
  return false;
}
function ignore() {}
/**
 * ================================================================================
 * ================================================================================
 * ================================================================================
 * ================================================================================
 */

/**
 * ================================================================================
 * ================================================================================
 * || SECTION:
 * ||         C O F O B
 * ||					C O N T E N T
 * ||
 * ================================================================================
 * ================================================================================
 * v-cofob v-content
 */

function do_medhall() {
  /** NEEDS WORK */
  var medhall_start = [
    "The scent of sterility assaults your senses as you step into the hallway. ",
    "The tile floors, while immaculate in appearance, have more wear and tear than ",
    "even the cement floors just a few rooms in the other direction. To the east ",
    "is the patchbay, to the west is the surgical station for more serious maladies.",
  ].join("");
  run([go(cofob.cofobcenter)]);
}
function should_run_shotgunner_zbuild_mob_command() {
  return false;
}
function do_waypoint_avenue_stalkers() {
  var mobs = {
    waypoint_avenue_thief: {
      mbuild: {
        attr: {
          exp: 150,
          virt: waypoint.thief.mob,
          mana: 45,
          max_mana: 45,
          hit: 150,
          max_hit: 150,
          move: 510,
          max_move: 510,
          damroll: 20,
          gold: 950,
          level: 15,
          weight: 5,
          height: 5,
          strength: 25,
          strength_add: 23,
          intelligence: 22,
          dexterity: 24,
          constitution: 28,
          charisma: 10,
          damnodice: 20,
          damsizedice: 16,
          name: "A suspicious looking car thief",
          sex: "MALE",
          action: "ISNPC HELPER AGGR_EVIL",
          short_description: "A suspicious looking car thief",
          long_description: "A suspicious looking car thief",
          description:
            "A car thief equipped with brass knuckles and a crowbar. He is obviously armed.",
        } /** end attr */,
        "extended-type": "AGGRESSIVE_THIEF",
      },
      meqbuild: {
        name: "waypoint-ave-car-thief",
        positions: {
          wield: "melee/crowbar.yml",
          hands: "melee/brass-knuckles.yml",
          body: "armor/basic-ballistic-vest.yml",
          head: "armor/baklava.yml",
          legs: "armor/dark-smithies-pants.yml",
          feet: "armor/red-brug-sneakers.yml",
        },
      },
    },
    rogue_mp_shotgunner: {
      mbuild: {
        attr: {
          exp: 250,
          virt: waypoint.rogue_mp.mob,
          mana: 565,
          max_mana: 565,
          hit: 5550,
          max_hit: 5550,
          move: 510,
          max_move: 510,
          damroll: 30,
          gold: 150,
          level: 25,
          weight: 10,
          height: 6,
          strength: 35,
          strength_add: 33,
          intelligence: 32,
          dexterity: 34,
          constitution: 38,
          charisma: 10,
          damnodice: 15,
          damsizedice: 18,
          name: "A {red}Rogue{/red} Military Police shotgunner",
          sex: "MALE",
          action: "ISNPC HELPER AGGR_EVIL",
          short_description: "A {red}Rogue{/red} Military Police shotgunner",
          long_description: "A {red}Rogue{/red} Military Police shotgunner",
          description: "A fit military police shotgunner. He looks armed.",
        } /** end attr */,
        "extended-type": "MP_SHOTGUNNER",
      },
      meqbuild: {
        name: "rogue-mp-shotgunner",
        positions: {
          wield: "rifle/bf-39-shotgun.yml",
          hands: "armor/mp-enforcer-gloves.yml",
          body: "armor/basic-ballistic-vest.yml",
          legs: "armor/mp-enforcer-pants.yml",
          feet: "armor/basic-boots.yml",
        },
      },
    },
  };
  auto_feed_mob_list(mobs);
  /** mob roaming */

  var rogue_shotgunner_roam_data = [
    go(cofob.armory_b2),
    "mbuild roam:start rogue-mp-shotgunner " + waypoint.rogue_mp.mob,
    "s",
    "s",
    "s",
    "s",
    "s",
    "w",
    "w",
    "s",
    "s",
    "w",
    "w",
    "e",
    "e",
    "e",
    "e",
    "w",
    "w",
    "s",
    "s",
    "w",
    "w",
    "e",
    "e",
    "e",
    "e",
    "w",
    "w",
    "s",
    "s",
    "w",
    "mbuild roam:stop rogue-mp-shotgunner",
    "mbuild roam:save rogue-mp-shotgunner",
  ];
  run(rogue_shotgunner_roam_data);
}
function garage_thieves() {
  var mobs = {
    garage_thief: {
      mbuild: {
        attr: {
          exp: 250,
          virt: 100,
          mana: 45,
          max_mana: 45,
          hit: 350,
          max_hit: 350,
          move: 610,
          max_move: 610,
          damroll: 20,
          gold: 750,
          level: 10,
          weight: 10,
          height: 5,
          strength: 18,
          strength_add: 13,
          intelligence: 2,
          dexterity: 34,
          constitution: 18,
          electronics: 15,
          chemistry: 0,
          strategy: 0,
          marksmanship: 15,
          sniping: 8,
          weapon_handling: 18,
          demolitions: 0,
          armor: 5,
          medical: 0,
          charisma: 0,
          damnodice: 30,
          damsizedice: 6,
          name: "A rugged car thief",
          sex: "MALE",
          action: "ISNPC HELPER AGGR_GOOD",
          short_description: "A rugged car thief",
          long_description: "A rugged car thief",
          description: "A car thief stalking the area. He is armed.",
        } /** end attr */,
        "extended-type": "CAR_THIEF",
      },
      meqbuild: {
        name: "car-thief",
        positions: {
          wield: "melee/crowbar.yml",
          head: "armor/baklava.yml",
          hands: "armor/mp-enforcer-gloves.yml",
          body: "armor/basic-ballistic-vest.yml",
          legs: "armor/blue-jeans.yml",
          feet: "armor/plat-basketball-shoes.yml",
        },
      },
      zbuild: {
        command: "mob",
        zone_id: 0,
        mob_vnum: 100,
        room_vnum: garage_thief_spawn,
        max: 2,
        if_flag: 0,
      },
    },
    petty_thief: {
      mbuild: {
        attr: {
          exp: 250,
          virt: 101,
          mana: 15,
          max_mana: 15,
          hit: 100,
          max_hit: 100,
          move: 110,
          max_move: 110,
          damroll: 10,
          gold: 750,
          level: 5,
          weight: 4,
          height: 5,
          strength: 4,
          strength_add: 4,
          intelligence: 2,
          dexterity: 4,
          constitution: 4,
          electronics: 5,
          chemistry: 0,
          strategy: 0,
          marksmanship: 5,
          sniping: 2,
          weapon_handling: 4,
          demolitions: 0,
          armor: 4,
          medical: 0,
          charisma: 0,
          damnodice: 2,
          damsizedice: 6,
          name: "A petty thief",
          sex: "MALE",
          action: "ISNPC HELPER AGGR_EVIL",
          short_description: "A petty thief",
          long_description: "A petty thief",
          description: "A petty thief is stalking the area.",
        } /** end attr */,
        "extended-type": "CAR_THIEF",
      },
      meqbuild: {
        name: "petty-thief",
        positions: {
          wield: "melee/cheap-crowbar.yml",
          legs: "armor/blue-jeans.yml",
          feet: "armor/plat-basketball-shoes.yml",
        },
      },
      zbuild: {
        command: "mob",
        zone_id: 0,
        mob_vnum: 101,
        room_vnum: petty_thief_spawn,
        max: 5,
        if_flag: 0,
      },
    },
    vile_kidnapper: {
      mbuild: {
        attr: {
          exp: 250,
          virt: 102,
          mana: 25,
          max_mana: 25,
          hit: 1450,
          max_hit: 1450,
          move: 510,
          max_move: 510,
          damroll: 20,
          gold: 1123,
          level: 8,
          weight: 4,
          height: 5,
          strength: 9,
          strength_add: 6,
          intelligence: 2,
          dexterity: 7,
          constitution: 10,
          electronics: 0,
          chemistry: 0,
          strategy: 0,
          marksmanship: 8,
          sniping: 3,
          weapon_handling: 5,
          demolitions: 0,
          armor: 10,
          medical: 0,
          charisma: 0,
          damnodice: 10,
          damsizedice: 25,
          name: "a kidnapper",
          sex: "MALE",
          action: "ISNPC HELPER AGGR_EVIL",
          short_description: "a kidnapper",
          long_description: "a kidnapper",
          description: "a kidnapper is stalking the area.",
        } /** end attr */,
        "extended-type": "GENERIC_THIEF",
        targets: "DARK_ROOM CHURCH PARKS DRUGS WEAPONS UTILITY_STORES",
      },
      meqbuild: {
        name: "kidnapper",
        positions: {
          head: "armor/baklava.yml",
          hands: "armor/brown-leather-gloves.yml",
          body: "armor/leather-trenchcoat.yml",
					//Generic thief extended mobs don't work well with
					//melee weapons. until that's figured out, use an mp5
          //wield: "melee/sickening-knife.yml",
					wield: "rifle/mp5.yml",
          legs: "armor/black-jeans.yml",
          feet: "armor/atom-fade-shoes.yml",
          goggle: "armor/stunner-shades.yml",
          neck_1: "armor/dark-throne-necklace.yml",
        },
      },
      zbuild: {
        command: "mob",
        zone_id: 0,
        mob_vnum: 102,
        room_vnum: vile_kidnapper_spawn,
        max: vile_kidnapper_zone_spawn_max,
        if_flag: 0,
      },
    },
    chaotic_meth_addict: {
      mbuild: {
        attr: {
          exp: 250,
          virt: 103,
          mana: 25,
          max_mana: 25,
          hit: 650,
          max_hit: 650,
          move: 310,
          max_move: 310,
          damroll: 30,
          gold: 4123,
          level: 13,
          weight: 4,
          height: 5,
          strength: 13,
          strength_add: 6,
          intelligence: 2,
          dexterity: 13,
          constitution: 15,
          electronics: 0,
          chemistry: 0,
          strategy: 0,
          marksmanship: 1,
          sniping: 1,
          weapon_handling: 8,
          demolitions: 2,
          armor: 4,
          medical: 9,
          charisma: 0,
          damnodice: 8,
          damsizedice: 25,
          name: "a chaotic meth addict",
          sex: "MALE",
          action: "ISNPC HELPER AGGR_EVIL",
          short_description: "a chaotic meth addict",
          long_description: "a chaotic meth addict",
          description:
            "a chaotic meth addict is manically patrolling the area.",
        } /** end attr */,
        "extended-type": "CHAOTIC_METH_ADDICT",
        targets: "CHURCH PARKS DRUGS WEAPONS UTILITY_STORES",
      },
      meqbuild: {
        name: "chaotic-meth-addict",
        positions: {
          //head: 'armor/baklava.yml',
          //hands: 'armor/brown-leather-gloves.yml',
          body: "armor/leather-trenchcoat.yml",
          wield: "melee/sickening-knife.yml",
          legs: "armor/black-jeans.yml",
          feet: "armor/atom-fade-shoes.yml",
          //goggle: 'armor/stunner-shades.yml',
          //neck_1: 'armor/dark-throne-necklace.yml',
        },
      },
      zbuild: {
        command: "mob",
        zone_id: 0,
        mob_vnum: 103,
        room_vnum: chaotic_meth_addict_spawn,
        max: chaotic_meth_addict_spawn_max,
        if_flag: 0,
      },
    },
    shoplifter: {
      mbuild: {
        attr: {
          exp: 250,
          virt: 104,
          mana: 15,
          max_mana: 15,
          hit: 10,
          max_hit: 10,
          move: 60,
          max_move: 60,
          damroll: 1,
          gold: 10,
          level: 1,
          weight: 4,
          height: 5,
          strength: 1,
          strength_add: 1,
          intelligence: 1,
          dexterity: 1,
          constitution: 1,
          electronics: 1,
          chemistry: 0,
          strategy: 0,
          marksmanship: 1,
          sniping: 0,
          weapon_handling: 1,
          demolitions: 0,
          armor: 0,
          medical: 0,
          charisma: 0,
          damnodice: 1,
          damsizedice: 3,
          name: "A shoplifter",
          sex: "MALE",
          action: "ISNPC HELPER AGGR_EVIL",
          short_description: "A shoplifter",
          long_description: "A shoplifter",
          description: "A shoplifter is stalking the area.",
        } /** end attr */,
        "extended-type": "GENERIC_THIEF",
        targets: "CLOTHES ELECTRONICS",
      },
      meqbuild: {
        name: "shoplifter",
        positions: {
          legs: "armor/blue-jeans.yml",
          head: "armor/baklava.yml",
          hands: "armor/brown-leather-gloves.yml",
          body: "armor/leather-trenchcoat.yml",
					//Generic thief extended mobs don't work well with
					//melee weapons. until that's figured out, use an mp5
          //wield: "melee/sickening-knife.yml",
					wield: "rifle/mp5.yml",
          feet: "armor/atom-fade-shoes.yml",
        },
      },
      zbuild: {
        command: "mob",
        zone_id: 0,
        mob_vnum: 104,
        room_vnum: shoplift_spawn,
        max: 5,
        if_flag: 0,
      },
    },
    crackhead: {
      mbuild: {
        attr: {
          exp: 250,
          virt: 105,
          mana: 15,
          max_mana: 15,
          hit: 35,
          max_hit: 35,
          move: 60,
          max_move: 60,
          damroll: 1,
          gold: 10,
          level: 2,
          weight: 4,
          height: 5,
          strength: 1,
          strength_add: 1,
          intelligence: 1,
          dexterity: 3,
          constitution: 0,
          electronics: 0,
          chemistry: 0,
          strategy: 0,
          marksmanship: 0,
          sniping: 0,
          weapon_handling: 1,
          demolitions: 0,
          armor: 0,
          medical: 0,
          charisma: 0,
          damnodice: 2,
          damsizedice: 6,
          name: "A crackhead",
          sex: "MALE",
          action: "ISNPC HELPER AGGR_EVIL",
          short_description: "A crackhead",
          long_description: "A crackhead",
          description: "A crackhead is stalking the area.",
        } /** end attr */,
        "extended-type": "AIMLESS_WANDERER",
        targets: "DRUGS CARS BUS_STOP EMPTY_ROOM",
      },
      meqbuild: {
        name: "crackhead",
        positions: {
          legs: "armor/blue-jeans.yml",
          feet: "armor/plat-basketball-shoes.yml",
        },
      },
      zbuild: {
        command: "mob",
        zone_id: 0,
        mob_vnum: 105,
        room_vnum: crackhead_spawn,
        max: 8,
        if_flag: 0,
      },
    },
  };
  auto_feed_mob_list(mobs);
  /** mob roaming */

  var shotgunner_roam_data = [
    "recall",
    "rnumtele 393",
    "mbuild roam:start car-thief 100",
    "w",
    "e",
    "e",
    "e",
    "e",
    "w",
    "w",
    "s",
    "s",
    "w",
    "w",
    "e",
    "e",
    "e",
    "e",
    "w",
    "w",
    "s",
    "s",
    "w",
    "w",
    "mbuild roam:stop car-thief",
    "mbuild roam:save car-thief",
  ];
  run(shotgunner_roam_data);

  var kidnapper_roaming_data = [
    "recall",
    "rnumtele 290",
    "mbuild roam:start kidnapper 102",
    "w",
    "w",
    "n",
    "n",
    "w",
    "w",
    "w",
    "w",
    "e",
    "e",
    "e",
    "n",
    "n",
    "u",
    "n",
    "u",
    "n",
    "e",
    "e",
    "e",
    "e",
    "e",
    "w",
    "w",
    "w",
    "w",
    "w",
    "w",
    "mbuild roam:stop kidnapper",
    "mbuild roam:save kidnapper",
  ];
  run(kidnapper_roaming_data);
}
function dynamic_garage_thief(obj){
	var level = obj.level;
	var vnum = obj.vnum;
	var count = obj.count;
	var roam_pattern = obj.roam_pattern;
	var rooms = obj.spawn;
	var orig = 10;
  var mobs = {
    garage_thief: {
      mbuild: {
        attr: {
					scalable: 1,
          exp: 250 / orig * level,
          virt: vnum,
          mana: 45 / orig * level,
          max_mana: 45 / orig * level,
          hit: 350 / orig * level,
          max_hit: 350 / orig * level,
          move: 610 / orig * level,
          max_move: 610 / orig * level,
          damroll: 20 / orig * level,
          gold: 750 / orig * level,
          level: level,
          weight: 10,
          height: 5,
          strength: 18 / orig * level,
          strength_add: 13 / orig * level,
          intelligence: 2 / orig * level,
          dexterity: 34 / orig * level,
          constitution: 18 / orig * level,
          electronics: 15 / orig * level,
          chemistry: 2 / orig * level,
          strategy: 2 / orig * level,
          marksmanship: 15 / orig * level,
          sniping: 8 / orig * level,
          weapon_handling: 18 / orig * level,
          demolitions: 2 / orig * level,
          armor: 5 / orig * level,
          medical: 2 / orig * level,
          charisma: 2 / orig * level,
          damnodice: 30 / orig * level,
          damsizedice: 6 / orig * level,
          name: "A rugged car thief",
          sex: "MALE",
          action: "ISNPC HELPER AGGR_GOOD",
          short_description: "A rugged car thief",
          long_description: "A rugged car thief",
          description: "A car thief stalking the area. He is armed.",
        } /** end attr */,
        "extended-type": "CAR_THIEF",
      },
      meqbuild: {
        name: "car-thief-dynamic-" + level + "-" + vnum,
        positions: {
          wield: "melee/crowbar.yml",
          head: "armor/baklava.yml",
          hands: "armor/mp-enforcer-gloves.yml",
          body: "armor/basic-ballistic-vest.yml",
          legs: "armor/blue-jeans.yml",
          feet: "armor/plat-basketball-shoes.yml",
        },
      },
      zbuild: {
        command: "mob",
        zone_id: 0,
        mob_vnum: vnum,
        room_vnum: rooms,
        max: count,
        if_flag: 0,
      },
    },
	};
	auto_feed_mob_list(mobs);
}
function dynamic_petty_thief(obj){
	var level = obj.level;
	var vnum = obj.vnum;
	var count = obj.count;
	var roam_pattern = obj.roam_pattern;
	var rooms = obj.spawn;
	var orig = 15;
	var mobs = {
    petty_thief: {
      mbuild: {
        attr: {
					scalable: 1,
          exp: 250 / orig * level,
          virt: vnum,
          mana: 15 / orig * level,
          max_mana: 15 / orig * level,
          hit: 100 / orig * level,
          max_hit: 100 / orig * level,
          move: 110 / orig * level,
          max_move: 110 / orig * level,
          damroll: 10 / orig * level,
          gold: 750 / orig * level,
          level: level,
          weight: 4 / orig * level,
          height: 5 / orig * level,
          strength: 4 / orig * level,
          strength_add: 4 / orig * level,
          intelligence: 2 / orig * level,
          dexterity: 4 / orig * level,
          constitution: 4 / orig * level,
          electronics: 5 / orig * level,
          chemistry: 0 / orig * level,
          strategy: 0 / orig * level,
          marksmanship: 5 / orig * level,
          sniping: 2 / orig * level,
          weapon_handling: 4 / orig * level,
          demolitions: 0 / orig * level,
          armor: 4 / orig * level,
          medical: 0 / orig * level,
          charisma: 0 / orig * level,
          damnodice: 2 / orig * level,
          damsizedice: 6 / orig * level,
          name: "A petty thief",
          sex: "MALE",
          action: "ISNPC HELPER AGGR_EVIL",
          short_description: "A petty thief",
          long_description: "A petty thief",
          description: "A petty thief is stalking the area.",
        } /** end attr */,
        "extended-type": "CAR_THIEF",
      },
      meqbuild: {
        name: "petty-thief-dynamic-level-" + level + "-" + vnum,
        positions: {
          wield: "melee/cheap-crowbar.yml",
          legs: "armor/blue-jeans.yml",
          feet: "armor/plat-basketball-shoes.yml",
        },
      },
      zbuild: {
        command: "mob",
        zone_id: 0,
        mob_vnum: vnum,
        room_vnum: rooms,
        max: count,
        if_flag: 0,
      },
    },
	};
	auto_feed_mob_list(mobs);
}
function dynamic_vile_kidnapper(obj){
	var level = obj.level;
	var vnum = obj.vnum;
	var count = obj.count;
	var roam_pattern = obj.roam_pattern;
	var rooms = obj.spawn;
	var orig = 15;
	var mobs = {
    vile_kidnapper: {
      mbuild: {
        attr: {
					scalable: 1,
          exp: 250 / orig * level,
          virt: vnum,
          mana: 25 / orig * level,
          max_mana: 25 / orig * level,
          hit: 1450 / orig * level,
          max_hit: 1450 / orig * level,
          move: 510 / orig * level,
          max_move: 510 / orig * level,
          damroll: 20 / orig * level,
          gold: 1123 / orig * level,
          level: level,
          weight: 4,
          height: 5,
          strength: 9 / orig * level,
          strength_add: 6 / orig * level,
          intelligence: 2 / orig * level,
          dexterity: 7 / orig * level,
          constitution: 10 / orig * level,
          electronics: 0 / orig * level,
          chemistry: 0 / orig * level,
          strategy: 0 / orig * level,
          marksmanship: 8 / orig * level,
          sniping: 3 / orig * level,
          weapon_handling: 5 / orig * level,
          demolitions: 0 / orig * level,
          armor: 10 / orig * level,
          medical: 0 / orig * level,
          charisma: 0 / orig * level,
          damnodice: 10 / orig * level,
          damsizedice: 25 / orig * level,
          name: "a kidnapper",
          sex: "MALE",
          action: "ISNPC HELPER AGGR_EVIL",
          short_description: "a kidnapper",
          long_description: "a kidnapper",
          description: "a kidnapper is stalking the area.",
        } /** end attr */,
        "extended-type": "GENERIC_THIEF",
        targets: "DARK_ROOM CHURCH PARKS DRUGS WEAPONS UTILITY_STORES",
      },
      meqbuild: {
        name: "kidnapper-dynamic-" + level + "-" + vnum,
        positions: {
          head: "armor/baklava.yml",
          hands: "armor/brown-leather-gloves.yml",
          body: "armor/leather-trenchcoat.yml",
					//Generic thief extended mobs don't work well with
					//melee weapons. until that's figured out, use an mp5
          //wield: "melee/sickening-knife.yml",
					wield: "rifle/mp5.yml",
          legs: "armor/black-jeans.yml",
          feet: "armor/atom-fade-shoes.yml",
          goggle: "armor/stunner-shades.yml",
          neck_1: "armor/dark-throne-necklace.yml",
        },
      },
      zbuild: {
        command: "mob",
        zone_id: 0,
        mob_vnum: vnum,
        room_vnum: rooms,
        max: count,
        if_flag: 0,
      },
    },
	};
	auto_feed_mob_list(mobs);
}
function dynamic_chaotic_meth_addict(obj){
	var level = obj.level;
	var vnum = obj.vnum;
	var count = obj.count;
	var roam_pattern = obj.roam_pattern;
	var rooms = obj.spawn;
	var orig = 15;
	var mobs = {
    chaotic_meth_addict: {
      mbuild: {
        attr: {
					scalable: 1,
          exp: 250 / orig * level,
          virt: vnum,
          mana: 25 / orig * level,
          max_mana: 25 / orig * level,
          hit: 650 / orig * level,
          max_hit: 650 / orig * level,
          move: 310 / orig * level,
          max_move: 310 / orig * level,
          damroll: 30 / orig * level,
          gold: 4123 / orig * level,
          level: level,
          weight: 4 / orig * level,
          height: 5 / orig * level,
          strength: 13 / orig * level,
          strength_add: 6 / orig * level,
          intelligence: 2 / orig * level,
          dexterity: 13 / orig * level,
          constitution: 15 / orig * level,
          electronics: 0 / orig * level,
          chemistry: 0 / orig * level,
          strategy: 0 / orig * level,
          marksmanship: 1 / orig * level,
          sniping: 1 / orig * level,
          weapon_handling: 8 / orig * level,
          demolitions: 2 / orig * level,
          armor: 4 / orig * level,
          medical: 9 / orig * level,
          charisma: 0 / orig * level,
          damnodice: 8 / orig * level,
          damsizedice: 25 / orig * level,
          name: "a chaotic meth addict",
          sex: "MALE",
          action: "ISNPC HELPER AGGR_EVIL",
          short_description: "a chaotic meth addict",
          long_description: "a chaotic meth addict",
          description:
            "a chaotic meth addict is manically patrolling the area.",
        } /** end attr */,
        "extended-type": "CHAOTIC_METH_ADDICT",
        targets: "CHURCH PARKS DRUGS WEAPONS UTILITY_STORES",
      },
      meqbuild: {
        name: "chaotic-meth-addict-dynamic-" + level + "-" + vnum,
        positions: {
          //head: 'armor/baklava.yml',
          //hands: 'armor/brown-leather-gloves.yml',
          body: "armor/leather-trenchcoat.yml",
          wield: "melee/sickening-knife.yml",
          legs: "armor/black-jeans.yml",
          feet: "armor/atom-fade-shoes.yml",
          //goggle: 'armor/stunner-shades.yml',
          //neck_1: 'armor/dark-throne-necklace.yml',
        },
      },
      zbuild: {
        command: "mob",
        zone_id: 0,
        mob_vnum: vnum,
        room_vnum: rooms,
        max: count,
        if_flag: 0,
      },
    },
	};
	auto_feed_mob_list(mobs);
}
function dynamic_shoplifter(obj){
	var level = obj.level;
	var vnum = obj.vnum;
	var count = obj.count;
	var roam_pattern = obj.roam_pattern;
	var rooms = obj.spawn;
	var orig = 15;
	var mobs = {
    shoplifter: {
      mbuild: {
        attr: {
					scalable: 1,
          exp: 250 / orig * level,
          virt: vnum,
          mana: 15 / orig * level,
          max_mana: 15 / orig * level,
          hit: 10 / orig * level,
          max_hit: 10 / orig * level,
          move: 60 / orig * level,
          max_move: 60 / orig * level,
          damroll: 1 / orig * level,
          gold: 10 / orig * level,
          level: level,
          weight: 4,
          height: 5,
          strength: 1 / orig * level,
          strength_add: 1 / orig * level,
          intelligence: 1 / orig * level,
          dexterity: 1 / orig * level,
          constitution: 1 / orig * level,
          electronics: 1 / orig * level,
          chemistry: 0 / orig * level,
          strategy: 0 / orig * level,
          marksmanship: 1 / orig * level,
          sniping: 0 / orig * level,
          weapon_handling: 1 / orig * level,
          demolitions: 0 / orig * level,
          armor: 0 / orig * level,
          medical: 0 / orig * level,
          charisma: 0 / orig * level,
          damnodice: 1 / orig * level,
          damsizedice: 3 / orig * level,
          name: "A shoplifter",
          sex: "MALE",
          action: "ISNPC HELPER AGGR_EVIL",
          short_description: "A shoplifter",
          long_description: "A shoplifter",
          description: "A shoplifter is stalking the area.",
        } /** end attr */,
        "extended-type": "GENERIC_THIEF",
        targets: "CLOTHES ELECTRONICS",
      },
      meqbuild: {
        name: "shoplifter-dynamic-" + level + "-" + vnum,
        positions: {
          legs: "armor/blue-jeans.yml",
          head: "armor/baklava.yml",
          hands: "armor/brown-leather-gloves.yml",
          body: "armor/leather-trenchcoat.yml",
					//Generic thief extended mobs don't work well with
					//melee weapons. until that's figured out, use an mp5
          //wield: "melee/sickening-knife.yml",
					wield: "rifle/mp5.yml",
          feet: "armor/atom-fade-shoes.yml",
        },
      },
      zbuild: {
        command: "mob",
        zone_id: 0,
        mob_vnum: vnum,
        room_vnum: rooms,
        max: count,
        if_flag: 0,
      },
    },
	};
	auto_feed_mob_list(mobs);
}
function dynamic_crackhead(obj){
	var level = obj.level;
	var vnum = obj.vnum;
	var count = obj.count;
	var roam_pattern = obj.roam_pattern;
	var rooms = obj.spawn;
	var orig = 15;
	var mobs = {
    crackhead: {
      mbuild: {
        attr: {
					scalable: 1,
          exp: 250 / orig * level,
          virt: vnum,
          mana: 15 / orig * level,
          max_mana: 15 / orig * level,
          hit: 35 / orig * level,
          max_hit: 35 / orig * level,
          move: 60 / orig * level,
          max_move: 60 / orig * level,
          damroll: 1 / orig * level,
          gold: 10 / orig * level,
          level: level,
          weight: 4 / orig * level,
          height: 5 / orig * level,
          strength: 1 / orig * level,
          strength_add: 1 / orig * level,
          intelligence: 1 / orig * level,
          dexterity: 3 / orig * level,
          constitution: 0 / orig * level,
          electronics: 0 / orig * level,
          chemistry: 0 / orig * level,
          strategy: 0 / orig * level,
          marksmanship: 0 / orig * level,
          sniping: 0 / orig * level,
          weapon_handling: 1 / orig * level,
          demolitions: 0 / orig * level,
          armor: 0 / orig * level,
          medical: 0 / orig * level,
          charisma: 0 / orig * level,
          damnodice: 2 / orig * level,
          damsizedice: 6 / orig * level,
          name: "A crackhead",
          sex: "MALE",
          action: "ISNPC HELPER AGGR_EVIL",
          short_description: "A crackhead",
          long_description: "A crackhead",
          description: "A crackhead is stalking the area.",
        } /** end attr */,
        targets: "DRUGS CARS BUS_STOP EMPTY_ROOM",
        "extended-type": "GENERIC_THIEF",
        "roam-pattern": roam_pattern,
      },
      meqbuild: {
        name: "crackhead-dynamic-" + level + "-" + vnum,
        positions: {
          legs: "armor/blue-jeans.yml",
          feet: "armor/plat-basketball-shoes.yml",
        },
      },
      zbuild: {
        command: "mob",
        zone_id: 0,
        mob_vnum: vnum,
        room_vnum: rooms,
        max: count,
        if_flag: 0,
      },
    },
	};
	auto_feed_mob_list(mobs);
}


function dynamic_aggressive_thief(obj){
	var level = obj.level;
	var vnum = obj.vnum;
	var count = obj.count;
	var roam_pattern = obj.roam_pattern;
	var rooms = obj.spawn;
	var orig = 15;
  var mobs = {
    waypoint_avenue_thief: {
      mbuild: {
        attr: {
					scalable: 1,
          exp: 150 / orig * level,
          virt: vnum,
          mana: 45 / orig * level,
          max_mana: 45 / orig * level,
          hit: 150 / orig * level,
          max_hit: 150 / orig * level,
          move: 510 / orig * level,
          max_move: 510 / orig * level,
          damroll: 20 / orig * level,
          gold: 950 / orig * level,
          level: level,
          weight: 5 / orig * level,
          height: 5 / orig * level,
          strength: 25 / orig * level,
          strength_add: 23 / orig * level,
          intelligence: 22 / orig * level,
          dexterity: 24 / orig * level,
          constitution: 28 / orig * level,
          charisma: 10 / orig * level,
          damnodice: 20 / orig * level,
          damsizedice: 16 / orig * level,
          name: "A suspicious looking car thief",
          sex: "MALE",
          action: "ISNPC HELPER AGGR_EVIL",
          short_description: "A suspicious looking car thief",
          long_description: "A suspicious looking car thief",
          description:
            "A car thief equipped with brass knuckles and a crowbar.",
        } /** end attr */,
        targets: "WEAPONS STREETS WATER FIRE",
        "extended-type": "GENERIC_THIEF",
        "roam-pattern": roam_pattern,
      },
      meqbuild: {
        name: "waypoint-ave-car-thief-level-dynamic-" + level + "-" + vnum,
        positions: {
          wield: "melee/crowbar.yml",
          hands: "melee/brass-knuckles.yml",
          body: "armor/basic-ballistic-vest.yml",
          head: "armor/baklava.yml",
          legs: "armor/dark-smithies-pants.yml",
          feet: "armor/red-brug-sneakers.yml",
        },
      },
      zbuild: {
        command: "mob",
        zone_id: 0,
        mob_vnum: vnum,
        room_vnum: rooms,
        max: count,
        if_flag: 0,
      },
    },
  };
  auto_feed_mob_list(mobs);
}

function dynamic_rogue_mp_shotgunner(obj){
	var level = obj.level;
	var vnum = obj.vnum;
	var count = obj.count;
	var roam_pattern = obj.roam_pattern;
	var rooms = obj.spawn;
	var orig = 25;
  var mobs = {
    rogue_mp_shotgunner: {
      mbuild: {
        attr: {
					scalable: 1,
          exp: 250 / orig * level,
          virt: vnum,
          mana: 565 / orig * level,
          max_mana: 565 / orig * level,
          hit: 5550 / orig * level,
          max_hit: 5550 / orig * level,
          move: 510 / orig * level,
          max_move: 510 / orig * level,
          damroll: 30 / orig * level,
          gold: 150 / orig * level,
          level: level,
          weight: 10,
          height: 6,
          strength: 35 / orig * level,
          strength_add: 33 / orig * level,
          intelligence: 32 / orig * level,
          dexterity: 34 / orig * level,
          constitution: 38 / orig * level,
          charisma: 10 / orig * level,
          damnodice: 15 / orig * level,
          damsizedice: 18 / orig * level,
          name: "A {red}Rogue{/red} Military Police shotgunner",
          sex: "MALE",
          action: "ISNPC HELPER AGGR_EVIL",
          short_description: "A {red}Rogue{/red} Military Police shotgunner",
          long_description: "A {red}Rogue{/red} Military Police shotgunner",
          description: "A fit military police shotgunner. He looks armed.",
        } /** end attr */,
        "extended-type": "MP_SHOTGUNNER",
      },
      meqbuild: {
        name: "rogue-mp-shotgunner-dynamic-" + level + "-" + vnum,
        positions: {
          wield: "rifle/bf-39-shotgun.yml",
          hands: "armor/mp-enforcer-gloves.yml",
          body: "armor/basic-ballistic-vest.yml",
          legs: "armor/mp-enforcer-pants.yml",
          feet: "armor/basic-boots.yml",
        },
      },
      zbuild: {
        command: "mob",
        zone_id: 0,
        mob_vnum: vnum,
        room_vnum: rooms,
        max: count,
        if_flag: 0,
      },
    },
  };
  auto_feed_mob_list(mobs);
}
function dynamic_defiler(obj){
	var level = obj.level;
	var vnum = obj.vnum;
	var count = obj.count;
	var roam_pattern = obj.roam_pattern;
	var rooms = obj.spawn;
	var orig = 20;
  var mobs = {
    defiler: {
      mbuild: {
        attr: {
					scalable: 1,
          exp: 8050 / orig * level,
          virt: vnum,
          level: level,
          mana: 830 / orig * level,
          max_mana: 830 / orig * level,
          hit: 3820 / orig * level,
          max_hit: 3820 / orig * level,
          move: 160 / orig * level,
          max_move: 160 / orig * level,
          damroll: 12 / orig * level,
          gold: 42080 / orig * level,
          weight: 10,
          height: 5,
          strength: 22 / orig * level,
          strength_add: 13 / orig * level,
          intelligence: 12 / orig * level,
          dexterity: 14 / orig * level,
          constitution: 14 / orig * level,
          electronics: 12 / orig * level,
          chemistry: 10 / orig * level,
          strategy: 18 / orig * level,
          marksmanship: 13 / orig * level,
          sniping: 12 / orig * level,
          weapon_handling: 11 / orig * level,
          demolitions: 11 / orig * level,
          armor: 20 / orig * level,
          medical: 1 / orig * level,
          charisma: 1 / orig * level,
          damnodice: 16 / orig * level,
          damsizedice: 16 / orig * level,
          name: "{red}DEFILER{/red}",
          sex: "MALE",
          action: "ISNPC",
          short_description: "{red}DEFILER{/red}",
          long_description: "{red}DEFILER{/red}",
          description: "{red}DEFILER{/red}",
        } /** end attr */,
        "extended-type": "DEFILER",
        "roam-pattern": "Butcher",
      },
      meqbuild: {
        name: "defiler-dynamic-" + level + "-" + vnum,
        positions: {
          wield: "rifle/defiler-scarh.yml",
          eyes: "armor/night-vision-goggles.yml",
          body: "armor/falcon-ballistic-vest.yml",
          hands: "armor/xm-scorpio-tactical-gloves.yml",
          legs: "armor/mp-enforcer-pants.yml",
          feet: "armor/xm50-ultralight-boots.yml",
        },
      },
      zbuild: {
        command: "mob",
        zone_id: 0,
        mob_vnum: vnum,
        room_vnum: rooms,
        max: count,
        if_flag: 0,
      },
    },
  };
  auto_feed_mob_list(mobs);
}

function dynamic_mp_shotgunner(obj){
	var level = obj.level;
	var vnum = obj.vnum;
	var count = obj.count;
	var roam_pattern = obj.roam_pattern;
	var rooms = obj.spawn;
  var mobs = {
    mp_shotgunner: {
      mbuild: {
        attr: {
					scalable: 1,
          exp: 7050,
          virt: obj.vnum,
          mana: 56.5 * level,
          max_mana: 56.5 * level,
          hit: 555 * level,
          max_hit: 555 * level,
          move: 51 * level,
          max_move: 51 * level,
          damroll: 3 * level,
          gold: 15 * level,
          level: level,
          weight: 10,
          height: 6,
          strength: 3.5 * level,
          strength_add: 3.3 * level,
          intelligence: 3.2 * level,
          dexterity: 3.4 * level,
          constitution: 3.8 * level,
          electronics: level,
          chemistry: level,
          strategy: level,
          marksmanship: level,
          sniping: level,
          weapon_handling: level,
          demolitions: level,
          armor: level,
          medical: level,
          charisma: level,
          damnodice: 1.5 * level,
          damsizedice: 1.8 * level,
          name: "A Military Police shotgunner",
          sex: "MALE",
          action: "ISNPC HELPER AGGR_GOOD",
          short_description: "A Military Police shotgunner",
          long_description: "A Military Police shotgunner",
          description: "A fit military police shotgunner. He looks armed.",
        } /** end attr */,
        "extended-type": "MP_SHOTGUNNER",
				"roam-pattern": obj.roam_pattern,
      },
      meqbuild: {
        name: "mp-shotgunner-dynamic-" + level + "-" + vnum,
        positions: {
          wield: "rifle/sasg12.yml",
          hands: "armor/mp-enforcer-gloves.yml",
          body: "armor/basic-ballistic-vest.yml",
          legs: "armor/mp-enforcer-pants.yml",
          feet: "armor/basic-boots.yml",
        },
      },
      zbuild: {
        command: "mob",
        zone_id: 0,
        mob_vnum: obj.vnum,
        room_vnum: obj.rooms,
        max: obj.count,
        if_flag: 0,
      },
    },
  };
	auto_feed_mob_list(mobs);
}
function dynamic_mp_enforcer(obj) {
	var level = obj.level;
	var vnum = obj.vnum;
	var count = obj.count;
	var roam_pattern = obj.roam_pattern;
	var rooms = obj.spawn;
  var mobs = {
    mp_enforcer: {
      mbuild: {
        attr: {
					scalable: 1,
          exp: 705 * level,
          virt: vnum,
          mana: 4.5 * level,
          max_mana: 4.5 * level,
          hit: 2.5 * level,
          max_hit: 2.5 * level,
          move: 11 * level,
          max_move: 11 * level,
          damroll: 2 * level,
          gold: 5 * level,
          level: level,
          weight: 10,
          height: 5,
          strength: 2.5 * level,
          strength_add: 2.3 * level,
          intelligence: 2.2 * level,
          dexterity: 2.4 * level,
          constitution: 2.8 * level,
          electronics: level,
          chemistry: level,
          strategy: level,
          marksmanship: level,
          sniping: level,
          weapon_handling: level,
          demolitions: level,
          armor: level,
          medical: level,
          charisma: level,
          damnodice: level,
          damsizedice: level,
          name: "A Military Police enforcer",
          sex: "MALE",
          action: "ISNPC HELPER AGGR_GOOD",
          short_description: "A Military Police enforcer",
          long_description: "A Military Police enforcer",
          description: "A fit military police enforcer. He looks armed.",
        } /** end attr */,
        "extended-type": "LOWLY_SECURITY",
        "roam-pattern": roam_pattern,
      },
      meqbuild: {
        name: "mp-enforcer-dynamic-" + level + "-" + vnum,
        positions: {
          wield: "rifle/hk45.yml",
          hands: "armor/mp-enforcer-gloves.yml",
          body: "armor/basic-ballistic-vest.yml",
          legs: "armor/mp-enforcer-pants.yml",
          feet: "armor/basic-boots.yml",
        },
      },
      zbuild: {
        command: "mob",
        zone_id: 0,
        mob_vnum: vnum,
        room_vnum: rooms,
        max: count,
        if_flag: 0,
      },
    },
	};

  auto_feed_mob_list(mobs);
}

function dynamic_ops_shield_shotgunner(obj) {
	var level = obj.level;
	var vnum = obj.vnum;
	var count = obj.count;
	var roam_pattern = obj.roam_pattern;
	var rooms = obj.spawn;
	var olevel = 40;
  var mobs = {
    ops_shield_shotgunner: {
      mbuild: {
        attr: {
					scalable: 1,
          exp: 20050 / olevel * level,
          virt: vnum,
          mana: 1565 / olevel * level,
          max_mana: 1565 / olevel * level,
          hit: 109550 / olevel * level,
          max_hit: 109550 / olevel * level,
          move: 810 / olevel * level,
          max_move: 810 / olevel * level,
          damroll: 340 / olevel * level,
          gold: 109150 / olevel * level,
          level: level,
          weight: 10,
          height: 6,
          strength: 355 / olevel * level,
          strength_add: 333 / olevel * level,
          intelligence: 132 / olevel * level,
          dexterity: 334 / olevel * level,
          constitution: 338 / olevel * level,
          electronics: 10 / olevel * level,
          chemistry: 10 / olevel * level,
          strategy: 10 / olevel * level,
          marksmanship: 10 / olevel * level,
          sniping: 10 / olevel * level,
          weapon_handling: 10 / olevel * level,
          demolitions: 10 / olevel * level,
          armor: 10 / olevel * level,
          medical: 10 / olevel * level,
          charisma: 10 / olevel * level,
          damnodice: 115 / olevel * level,
          damsizedice: 48 / olevel * level,
          name: "An Ops Shield shotgunner",
          sex: "MALE",
          action: "ISNPC HELPER AGGR_GOOD",
          short_description: "An Ops Shield shotgunner",
          long_description: "An Ops Shield shotgunner",
          description: "An Ops Shield shotgunner",
        } /** end attr */,
        "extended-type": "MP_SHOTGUNNER",
      },
      meqbuild: {
        name: "ops-shield-shotgunner-dynamic-" + level + "-" + vnum,
        positions: {
          /** TODO: deck the halls with OP weapons and armor */
          wield: "rifle/bf-39-shotgun.yml",
          hands: "armor/mp-enforcer-gloves.yml",
          body: "armor/basic-ballistic-vest.yml",
          legs: "armor/mp-enforcer-pants.yml",
          feet: "armor/basic-boots.yml",
        },
      },
      zbuild: {
        command: "mob",
        zone_id: 0,
        mob_vnum: vnum,
        room_vnum: rooms,
        max: count,
        if_flag: 0,
      },
    },
	};

  auto_feed_mob_list(mobs);
}
function cobalt_military_police() {
  var mobs = {
    mp_enforcer: {
      mbuild: {
        attr: {
          exp: 7050,
          virt: 500,
          mana: 45,
          max_mana: 45,
          hit: 250,
          max_hit: 250,
          move: 110,
          max_move: 110,
          damroll: 20,
          gold: 50,
          level: 10,
          weight: 10,
          height: 5,
          strength: 25,
          strength_add: 23,
          intelligence: 22,
          dexterity: 24,
          constitution: 28,
          electronics: 10,
          chemistry: 10,
          strategy: 10,
          marksmanship: 10,
          sniping: 10,
          weapon_handling: 10,
          demolitions: 10,
          armor: 10,
          medical: 10,
          charisma: 10,
          damnodice: 10,
          damsizedice: 6,
          name: "A Military Police enforcer",
          sex: "MALE",
          action: "ISNPC HELPER AGGR_GOOD",
          short_description: "A Military Police enforcer",
          long_description: "A Military Police enforcer",
          description: "A fit military police enforcer. He looks armed.",
        } /** end attr */,
        "extended-type": "LOWLY_SECURITY",
      },
      meqbuild: {
        name: "mp-enforcer",
        positions: {
          wield: "rifle/hk45.yml",
          hands: "armor/mp-enforcer-gloves.yml",
          body: "armor/basic-ballistic-vest.yml",
          legs: "armor/mp-enforcer-pants.yml",
          feet: "armor/basic-boots.yml",
        },
      },
      zbuild: {
        command: "mob",
        zone_id: 0,
        mob_vnum: 500,
        room_vnum: 131,
        max: 3,
        if_flag: 0,
      },
    },
    mp_shotgunner: {
      mbuild: {
        attr: {
          exp: 7050,
          virt: 501,
          mana: 565,
          max_mana: 565,
          hit: 5550,
          max_hit: 5550,
          move: 510,
          max_move: 510,
          damroll: 30,
          gold: 150,
          level: 40,
          weight: 10,
          height: 6,
          strength: 35,
          strength_add: 33,
          intelligence: 32,
          dexterity: 34,
          constitution: 38,
          electronics: 10,
          chemistry: 10,
          strategy: 10,
          marksmanship: 10,
          sniping: 10,
          weapon_handling: 10,
          demolitions: 10,
          armor: 10,
          medical: 10,
          charisma: 10,
          damnodice: 15,
          damsizedice: 18,
          name: "A Military Police shotgunner",
          sex: "MALE",
          action: "ISNPC HELPER AGGR_GOOD",
          short_description: "A Military Police shotgunner",
          long_description: "A Military Police shotgunner",
          description: "A fit military police shotgunner. He looks armed.",
        } /** end attr */,
        "extended-type": "MP_SHOTGUNNER",
      },
      meqbuild: {
        name: "mp-shotgunner",
        positions: {
          wield: "rifle/bf-39-shotgun.yml",
          hands: "armor/mp-enforcer-gloves.yml",
          body: "armor/basic-ballistic-vest.yml",
          legs: "armor/mp-enforcer-pants.yml",
          feet: "armor/basic-boots.yml",
        },
      },
      zbuild: {
        command: "mob",
        zone_id: 0,
        mob_vnum: 501,
        room_vnum: 131,
        max: 3,
        if_flag: 0,
      },
    },
    ops_shield_shotgunner: {
      mbuild: {
        attr: {
          exp: 20050,
          virt: 502,
          mana: 1565,
          max_mana: 1565,
          hit: 109550,
          max_hit: 109550,
          move: 810,
          max_move: 810,
          damroll: 340,
          gold: 109150,
          level: 40,
          weight: 10,
          height: 6,
          strength: 355,
          strength_add: 333,
          intelligence: 132,
          dexterity: 334,
          constitution: 338,
          electronics: 10,
          chemistry: 10,
          strategy: 10,
          marksmanship: 10,
          sniping: 10,
          weapon_handling: 10,
          demolitions: 10,
          armor: 10,
          medical: 10,
          charisma: 10,
          damnodice: 115,
          damsizedice: 48,
          name: "An Ops Shield shotgunner",
          sex: "MALE",
          action: "ISNPC HELPER AGGR_GOOD",
          short_description: "An Ops Shield shotgunner",
          long_description: "An Ops Shield shotgunner",
          description: "An Ops Shield shotgunner",
        } /** end attr */,
        "extended-type": "MP_SHOTGUNNER",
      },
      meqbuild: {
        name: "ops-shield-shotgunner",
        positions: {
          /** TODO: deck the halls with OP weapons and armor */
          wield: "rifle/bf-39-shotgun.yml",
          hands: "armor/mp-enforcer-gloves.yml",
          body: "armor/basic-ballistic-vest.yml",
          legs: "armor/mp-enforcer-pants.yml",
          feet: "armor/basic-boots.yml",
        },
      },
      zbuild: {
        command: "mob",
        zone_id: 0,
        mob_vnum: 502,
        room_vnum: [212, 207, 204, 340, 199, 279, 139],
        max: 2,
        if_flag: 0,
      },
    },
  };
  auto_feed_mob_list(mobs);
  /** mob roaming */

  var shotgunner_roam_data = [
    "recall",
    "mbuild roam:start mp-shotgunner 501",
    "n",
    "n",
    "e",
    "w",
    "w",
    "mbuild roam:stop mp-shotgunner",
    "mbuild roam:save mp-shotgunner",
  ];
  run(shotgunner_roam_data);
  var enforcer_roam_data = [
    "recall",
    "mbuild roam:start mp-enforcer 500",
    "n",
    "n",
    "e",
    "w",
    "w",
    "w",
    "w",
    "w",
    "w",
    "w",
    "n",
    "n",
    "n",
    "n",
    "n",
    "n",
    "n",
    "n",
    "n",
    "n",
    "n",
    "n",
    "n",
    "n",
    "n",
    "e",
    "e",
    "e",
    "e",
    "e",
    "e",
    "e",
    "e",
    "e",
    "s",
    "s",
    "s",
    "s",
    "s",
    "s",
    "s",
    "s",
    "s",
    "s",
    "s",
    "s",
    "s",
    "s",
    "s",
    "mbuild roam:stop mp-enforcer",
    "mbuild roam:save mp-enforcer",
  ];
  run(enforcer_roam_data);
}
function do_ballistics_lab() {
  var mobs = {
    dr_land: {
      mbuild: {
        attr: {
          exp: 30500,
          virt: scripted.doctor_land.mob,
          mana: 15,
          max_mana: 15,
          hit: 58580,
          max_hit: 58580,
          move: 140,
          max_move: 240,
          damroll: 215,
          gold: 505000,
          level: 40,
          weight: 10,
          height: 4,
          strength: 85,
          strength_add: 83,
          intelligence: 82,
          dexterity: 84,
          constitution: 88,
          electronics: 10,
          chemistry: 10,
          strategy: 10,
          marksmanship: 10,
          sniping: 10,
          weapon_handling: 10,
          demolitions: 10,
          armor: 10,
          medical: 10,
          charisma: 1,
          damnodice: 84,
          damsizedice: 86,
          name: "A Ballistics Expert",
          sex: "MALE",
          action: "ISNPC",
          short_description: "A Ballistics Expert",
          long_description: "A Ballistics Expert",
          description: "A Ballistics Expert",
        } /** end attr */,
      } /** end mbuild */,
      meqbuild: {
        name: "doctor-land",
        positions: {
          hold: "gadget/ballistics-lab-blue-key-fob.yml",
          primary: "rifle/ump45-vulture-modded.yml",
          secondary: "rifle/blackhawk-50cal-pistol.yml",
          hands: "armor/forge-xm3-gloves.yml",
          body: "armor/vulture-pk3-ballistic-vest.yml",
          legs: "armor/heavy-armament-tactical-pants.yml",
          feet: "armor/vulture-pk3-ballistic-boots.yml",
        },
      },
      zbuild: {
        command: "mob",
        zone_id: 0,
        mob_vnum: scripted.doctor_land.mob,
        room_vnum: scripted.doctor_land.room,
        max: 1,
        if_flag: 0,
      },
    }, // end scientist
  }; //end mobs
  auto_feed_mob_list(mobs);
}
function do_triton_lab() {
  var mobs = {
    scientist: {
      mbuild: {
        attr: {
          exp: 30500,
          virt: scripted.triton.mob,
          mana: 15,
          max_mana: 15,
          hit: 58580,
          max_hit: 58580,
          move: 140,
          max_move: 240,
          damroll: 215,
          gold: 505000,
          level: 40,
          weight: 10,
          height: 4,
          strength: 85,
          strength_add: 83,
          intelligence: 82,
          dexterity: 84,
          constitution: 88,
          electronics: 10,
          chemistry: 10,
          strategy: 10,
          marksmanship: 10,
          sniping: 10,
          weapon_handling: 10,
          demolitions: 10,
          armor: 10,
          medical: 10,
          charisma: 1,
          damnodice: 84,
          damsizedice: 86,
          name: "A {yel}TRITON{/yel} {blu}LABS{/blu} Scientist",
          sex: "MALE",
          action: "ISNPC",
          short_description: "A {yel}TRITON{/yel} {blu}LABS{/blu} Scientist",
          long_description: "A {yel}TRITON{/yel} {blu}LABS{/blu} Scientist",
          description: "A {yel}TRITON{/yel} {blu}LABS{/blu} Scientist",
        } /** end attr */,
      } /** end mbuild */,
      meqbuild: {
        name: "triton-scientist",
        positions: {
          hold: "gadget/triton-labs-yellow-key-fob.yml",
          goggles: "armor/thick-glasses.yml",
          hands: "armor/rubber-gloves.yml",
          body: "armor/plain-white-lab-coat.yml",
          legs: "armor/pale-brown-pants.yml",
          feet: "armor/static-free-shoes.yml",
        },
      },
      zbuild: {
        command: "mob",
        zone_id: 0,
        mob_vnum: scripted.triton.mob,
        room_vnum: scripted.triton.room,
        max: 1,
        if_flag: 0,
      },
    }, // end scientist
    volunteerpatient: {
      mbuild: {
        attr: {
          exp: 3500,
          virt: scripted.triton.volunteer_mob,
          mana: 25,
          max_mana: 25,
          hit: 880,
          max_hit: 880,
          move: 240,
          max_move: 240,
          damroll: 115,
          gold: 0,
          level: 15,
          weight: 10,
          height: 4,
          strength: 185,
          strength_add: 183,
          intelligence: 10,
          dexterity: 184,
          constitution: 188,
          electronics: 10,
          chemistry: 10,
          strategy: 10,
          marksmanship: 10,
          sniping: 10,
          weapon_handling: 10,
          demolitions: 10,
          armor: 10,
          medical: 10,
          charisma: 1,
          damnodice: 8,
          damsizedice: 90,
          name: "A volunteer patient",
          sex: "MALE",
          action: "ISNPC",
          short_description: "A volunteer patient",
          long_description: "A volunteer patient",
          description:
            "A volunteer patient stands here with dazed thousand mile stare.",
        } /** end attr */,
      } /** end mbuild */,
      meqbuild: {
        name: "triton-volunteer-patient",
        positions: {
          wield: "melee/improvised-shank.yml",
          body: "armor/hospital-gown.yml",
          feet: "armor/hospital-shoes.yml",
        },
      },
      zbuild: {
        command: "mob",
        zone_id: 0,
        mob_vnum: scripted.triton.volunteer_mob,
        room_vnum: scripted.triton.volunteer_room,
        max: 8,
        if_flag: 0,
      },
    }, // end volunteer patient
    surgeon: {
      mbuild: {
        attr: {
          exp: 8500,
          virt: 602,
          mana: 815,
          max_mana: 815,
          hit: 8580,
          max_hit: 8580,
          move: 240,
          max_move: 240,
          damroll: 215,
          gold: 85000,
          level: 80,
          weight: 10,
          height: 4,
          strength: 185,
          strength_add: 183,
          intelligence: 182,
          dexterity: 184,
          constitution: 188,
          electronics: 10,
          chemistry: 10,
          strategy: 10,
          marksmanship: 10,
          sniping: 10,
          weapon_handling: 10,
          demolitions: 10,
          armor: 10,
          medical: 10,
          charisma: 85,
          damnodice: 184,
          damsizedice: 186,
          name: "A {yel}TRITON{/yel} {blu}LABS{/blu} Field Surgeon",
          sex: "FEMALE",
          action: "ISNPC",
          short_description:
            "A {yel}TRITON{/yel} {blu}LABS{/blu} Field Surgeon",
          long_description: "A {yel}TRITON{/yel} {blu}LABS{/blu} Field Surgeon",
          description: "A {yel}TRITON{/yel} {blu}LABS{/blu} Field Surgeon",
        } /** end attr */,
      } /** end mbuild */,
      meqbuild: {
        name: "triton-field-surgeon",
        positions: {
          wield: "melee/operating-scalpel.yml",
          goggles: "armor/operating-goggles.yml",
          hands: "armor/rubber-gloves.yml",
          body: "armor/plain-white-lab-coat.yml",
          legs: "armor/pale-brown-pants.yml",
          feet: "armor/static-free-shoes.yml",
        },
      },
      zbuild: {
        command: "mob",
        zone_id: 0,
        mob_vnum: 602,
        room_vnum: scripted.triton.room,
        max: 1,
        if_flag: 0,
      },
    }, // end scientist
  }; //end mobs
  auto_feed_mob_list(mobs);
  var patient_roam_data = [
    "recall",
    "n",
    "n",
    "e",
    "e",
    "n",
    "n",
    "e",
    "e",
    "e",
    "mbuild roam:start volunteer-patient " + scripted.triton.volunteer_mob,
    "e",
    "d",
    "e",
    "e",
    "d",
    "e",
    "e",
    "d",
    "e",
    "e",
    "mbuild roam:stop volunteer-patient",
    "mbuild roam:save volunteer-patient",
  ];
  run(patient_roam_data);
}
function do_live_training_exercise_FOB_1A() {
  var south_main_hallway = ["Pod 41A"].join("");
  run(["rbuild pave continue"]);
  var draw_rectangle = [
    "rbuild title {blu}COBALT Air Force Base:{/blu} {grn}Live Training Exercise Station 1A{/grn}",
    "rbuild description " + south_main_hallway,
    "rbuild sector-type INDOOR_CEMENT",
    "room_dark off",
  ];
  var directions = ["e", "n", "w", "s"];
  var ctr = 0,
    width = 5,
    height = 4,
    width_counter = 0,
    height_counter = 0;
  for (; ctr < 4; ctr++) {
    var measurement = width;
    if (directions[ctr] === "n" || directions[ctr] === "s") {
      measurement = height;
    }
    for (; measurement > 0; --measurement) {
      run([directions[ctr]]);
      run(draw_rectangle);
    }
  }
}
function do_second_floor_engineering() {
  var far_north_engineering_hallway = [
    "The carpet beneath your feet is a drab grey color. The northern wall ",
    "is made up of mostly one-way glass panels that overlook the outer ",
    "courtyard below. To the east is a single door with advanced bio-metric ",
    "scanners that prevent unauthorized access. An iris scanner, voice recognition, ",
    "and a gait recognition camera all record your every move. ",
  ].join("");
  var engineering_hallway = [
    "The doors on the eastern wall are all closed and most likely locked. ",
    "You see the commons area at the end of the hallway. You notice various ",
    "surveillance cameras strategically placed at the corners of the ceiling. ",
    /** TODO: add integral camera feeds here */
  ].join("");
  var engineering_room_1a = [
    "The promise of coffee lingers in the air as the strictly regulated air conditioner ",
    "calmy spreads 68 degree air. A row of computers stretches to the north and south. ",
    "You can see a server room at the end of the long strip of computer screens all the ",
    "way at the north end of the room. ",
  ].join("");
  var engineering_commons = [
    "A comfy looking couch and recliner welcome you to the engineering commons ",
    "area. Three television screens mounted on the west wall display various ",
    "graphs and statistics. A ping pong table is in the center of the room. ",
    "Two large refridgerators are humming quietly in the corner. The north wall ",
    "consists of large one-way tinted windows that overlook the training fields ",
    "below. To the east is the entrance to server room 1B.",
  ].join("");
  var server_room_1a = [
    "It's noisy and loud in here. The rack mounted servers are fervently whirring ",
    "under the immense workloads. The ceiling is a grate with a large fan behind ",
    "it. A complex looking locking mechanism protects rack mounted servers here. ",
  ].join("");
  var server_room_1b = [
    "Wall to wall rack mounted servers take up the entirety of this room. There ",
    "is almost nowhere to stand. A haphazard array of ethernet cables snake ",
    "chaotically to and from each server. Maintaining this must be a headache. ",
  ].join("");
  var fm = "@FILL_ME@";

  run([
    repeat(4, ["e"]),
    repeat(4, [
      "n",
      "rbuild title {blu}C.O.F.O.B:{/blu} - Engineering hallway",
      "rbuild description " + engineering_hallway,
      "rbuild sector-type INSIDE CARPET ROOFTOP",
    ]),
    "n",
    "rbuild title {blu}C.O.F.O.B:{/blu} - Commons area",
    "rbuild description " + engineering_commons,
    "rbuild sector-type INSIDE CARPET ROOFTOP",
    mark(cofob.eng_commons) /** For the purpose of camera-feed */,
    repeat(3, ["s"]),
    "e",
    "rbuild title {blu}C.O.F.O.B:{/blu} - Server Room 1A",
    "rbuild description " + server_room_1a,
    "rbuild sector-type INSIDE CARPET ROOFTOP",
    "w",
    repeat(3, ["n"]),
    "e",
    "rbuild title {blu}C.O.F.O.B:{/blu} - Server Room 1B",
    "rbuild description " + server_room_1b,
    "rbuild sector-type INSIDE CARPET ROOFTOP",
    "w",
    repeat(5, ["s"]),
    repeat(4, ["w"]) /** return to center */,
  ]);
  return true;
}
function do_second_floor() {
  var eastern_hallway_middle = [
    "The hallway stretches north and south. The cement floor shows serious signs of wear and tear. ",
    "It seems whoever was in charge of cleaning this portion of the base did a very poor job, which ",
    "surprises you. A ventilation shaft calmly blows semi-cold air throughout this portion of the hallway. ",
    "{gld}A Bulletin Board{/gld} lists various contracts.",
  ].join("");
  var fm = "@FILL_ME@";
  var eastern_hallway_middle = fm;
  var shooting_range_entrance_east = fm;
  var shooting_range_falcon = fm;
  var second_floor_hallway = fm;
  var north_hallway = fm;
  var north_hallway_stairs = fm;
  var second_floor_hallway = fm;
  var western_hallway = [
    "The reinforced metal walls are briefly interrupted by the occasional senior officer ",
    "door. As you make your way north and south, you notice that the entirety of the western ",
    "wall consists of metal walls with doors that lead to the underground portion of the base. ",
  ].join("");

  var cofob_center_vnum = exec("print_vnum " + cofob.center);
  run([
    /** start of mess hall */
    go(cofob.east_exit),
    repeat(3, [
      "n",
      "rbuild title {blu}C.O.F.O.B:{/blu} - Eastern Hallway",
      "rbuild description " + eastern_hallway_middle,
      "rbuild sector-type INDOOR_CEMENT",
    ]),
    mark(cofob.mess_hall_upstairs),
    repeat(2, [
      "n",
      "rbuild title {blu}C.O.F.O.B:{/blu} - Eastern Hallway",
      "rbuild description " + eastern_hallway_middle,
      "rbuild sector-type INDOOR_CEMENT",
    ]),
    repeat(2, [
      "w",
      "rbuild title {blu}C.O.F.O.B:{/blu} - North Hallway",
      "rbuild description " + north_hallway,
      "rbuild sector-type INDOOR_CEMENT",
    ]),
    "w",
    "rbuild title {blu}C.O.F.O.B:{/blu} - North Hallway Stairs",
    "rbuild description " + north_hallway_stairs,
    "rbuild sector-type INDOOR_CEMENT STAIRS",
    mark("cofob-stairs-A"),
    repeat(5, [
      "w",
      "rbuild title {blu}C.O.F.O.B:{/blu} - North Hallway",
      "rbuild description " + north_hallway,
      "rbuild sector-type INDOOR_CEMENT",
    ]),
    repeat(14, [
      "s",
      "rbuild title {blu}C.O.F.O.B [OUTER]:{/blu} - Western Hallway",
      "rbuild description " + western_hallway,
      "rbuild texture INSIDE DRY ROOFTOP CEMENT",
    ]),
    repeat(5, [
      "e",
      "rbuild title {blu}C.O.F.O.B [OUTER]:{/blu} - South Hallway",
      "rbuild description " + western_hallway,
      "rbuild texture INSIDE DRY ROOFTOP CEMENT",
    ]),
    mark("cofob-bind-to-center"),
    "rbuild bind EAST " + cofob_center_vnum,
    "e",
    go("cofob-stairs-A"),
    "n",
    "rbuild title {blu}C.O.F.O.B:{/blu} - North Hallway Stairs",
    "rbuild description " + north_hallway_stairs,
    "rbuild sector-type INDOOR_CEMENT STAIRS",
    "u",
    "rbuild title {blu}C.O.F.O.B:{/blu} - North Hallway Stairs",
    "rbuild description " + north_hallway_stairs,
    "rbuild sector-type INDOOR_CEMENT STAIRS",
    "n",
    "rbuild title {blu}C.O.F.O.B:{/blu} - North Hallway Stairs",
    "rbuild description " + north_hallway_stairs,
    "rbuild sector-type INDOOR_CEMENT STAIRS",
    "u",
    "rbuild title {blu}C.O.F.O.B:{/blu} - North Hallway Stairs",
    "rbuild description " + north_hallway_stairs,
    "rbuild sector-type INDOOR_CEMENT STAIRS",
    /** start of second floor */
    repeat(2, [
      "n",
      "rbuild title {blu}C.O.F.O.B:{/blu} - Second floor hallway",
      "rbuild description " + second_floor_hallway,
      "rbuild sector-type INDOOR_CEMENT",
    ]),
    mark(cofob.secondfloor_center), //'secondfloor-center'),
    repeat(5, [
      "w",
      "rbuild title {blu}C.O.F.O.B:{/blu} - Second floor hallway",
      "rbuild description " + second_floor_hallway,
      "rbuild sector-type INDOOR_CEMENT",
    ]),
    go(cofob.secondfloor_center),
    repeat(2, [
      "n",
      "rbuild title {blu}C.O.F.O.B:{/blu} - Second floor Foremast",
      "rbuild description " + fm,
      "rbuild sector-type INSIDE CARPET ROOFTOP",
    ]),
    "e",
    "rbuild title {blu}C.O.F.O.B:{/blu} - Second floor Foremast",
    "rbuild description " + fm,
    "rbuild sector-type INSIDE CARPET ROOFTOP",
    repeat(3, [
      "n",
      "rbuild title {blu}C.O.F.O.B:{/blu} - Second floor Foremast",
      "rbuild description " + fm,
      "rbuild sector-type INSIDE CARPET ROOFTOP",
    ]),
    repeat(3, ["s"]),
    "w",
    go(cofob.secondfloor_center),
    repeat(2, ["n"]),
    "w",
    "rbuild title {blu}C.O.F.O.B:{/blu} - Second floor Foremast",
    "rbuild description " + second_floor_hallway,
    "rbuild sector-type INDOOR_CEMENT CARPET ROOFTOP",
    repeat(3, [
      "n",
      "rbuild title {blu}C.O.F.O.B:{/blu} - Second floor Foremast",
      "rbuild description " + second_floor_hallway,
      "rbuild sector-type INDOOR_CEMENT CARPET ROOFTOP",
    ]),
    repeat(3, ["s"]),
    "e",
    //repeat(2, ['s']), /** return to center */
    go(cofob.secondfloor_center),
    repeat(5, [
      "e",
      "rbuild title {blu}C.O.F.O.B:{/blu} - Second floor hallway",
      "rbuild description " + second_floor_hallway,
      "rbuild sector-type INDOOR_CEMENT",
    ]),
    //repeat(5,['w']), /** return to center */
    go(cofob.secondfloor_center),
  ]);
  var bind_to_center_vnum = exec("print_vnum cofob-bind-to-center");
  run([
    go("cofob-bind-to-center"),
    "e",
    "rbuild bind WEST " + bind_to_center_vnum,
    go(cofob.secondfloor_center),
  ]);

  do_second_floor_engineering();
  return true;
}
function do_cobalt_spawn() {
  var FILL_ME = "@FILL_ME@";
  var spawn_entry = ["Description of spawn entry to come soon"].join("");
  var eastern_corps_drive = [FILL_ME].join("");
  var sniper_rifle_shop = [FILL_ME].join("");
  run([
    "rbuild pave continue",
    "n",
    "rbuild title {blu}COBALT Forward Operating Base{/blu} - General Area",
    "rbuild description " + spawn_entry,
    "rbuild sector-type INDOOR_CEMENT",
    "e",
    "rbuild title {blu}COBALT Forward Operating Base{/blu} - Corps Drive",
    "rbuild description " + eastern_corps_drive,
    "rbuild sector-type OUTDOOR_CEMENT",
    "e",
    "rbuild title {blu}COBALT Forward Operating Base{/blu} - Corps Drive",
    "rbuild description " + eastern_corps_drive,
    "rbuild sector-type OUTDOOR_CEMENT",
    "n",
    "rbuild title {blu}COBALT Forward Operating Base{/blu} - Ranged Weapon Shop",
    "rbuild description " + sniper_rifle_shop,
    "rbuild sector-type INDOOR_CEMENT",
  ]);
}
function do_cofob_west_area_from(nickname_for_center) {
  var western_hallway = [
    "The reinforced metal walls are briefly interrupted by the occasional senior officer ",
    "door. As you make your way north and south, you notice that the entirety of the western ",
    "wall consists of metal walls with doors that lead to the underground portion of the base. ",
  ].join("");
  var atop_the_stairs = [
    "The floor deviates from the other rooms in that it is made up of a reinforced steel grating. ",
    "The choice for such a floor remains a mystery. The door to the west leads to the stairs that will ",
    "take you to the underground portion of the base.",
  ].join("");
  var descending_the_stairs = ["fill me"].join("");
  run([
    go(cofob.center),
    repeat(6, ["w"]),
    go(cofob.center),
    repeat(6, ["w"]),
    "n",
    "rbuild title {blu}C.O.F.O.B [OUTER]:{/blu} - Atop the stairs",
    "rbuild description " + atop_the_stairs,
    "rbuild texture INSIDE DRY ROOFTOP CEMENT",
    "n",
    "rbuild title {blu}C.O.F.O.B [OUTER]:{/blu} - Atop the stairs",
    "rbuild description " + atop_the_stairs,
    "rbuild texture INSIDE DRY ROOFTOP CEMENT",
    mark(cofob.west_atop_stairs), //'west-atop-the-stairs'),
    "w",
    "rbuild title {blu}C.O.F.O.B [OUTER]:{/blu} - Descending the stairs",
    "rbuild description " + descending_the_stairs,
    "rbuild texture INSIDE DRY ROOFTOP STAIRS",
    "d",
    "rbuild title {blu}C.O.F.O.B [OUTER]:{/blu} - Descending the stairs",
    "rbuild description " + descending_the_stairs,
    "rbuild texture INSIDE DRY ROOFTOP STAIRS",
  ]);
  return true;
}
function do_mess_hall() {
  var mess_hall_upstairs = [
    "Metal railings accompany the stairs leading down to the mess hall.",
  ].join("");
  var mess_hall_serving_area = [
    "The tile floor is immaculate in presentation. Plain grey metal chairs attend each ",
    "table obediently in a neat formation of 2 chairs to each cardinal side.",
    "A few familiar dishes invite you to sit down and enjoy the hastily prepared meals. ",
    "The chefs can be seen in the kitchen looking down as they prepare the food. The remote ",
    "promise of coffee is enticing.",
  ].join("");
  run([
    go(cofob.mess_hall_upstairs),
    "w",
    "rbuild title {blu}C.O.F.O.B:{/blu} - Mess Hall stairs",
    "rbuild description " + mess_hall_upstairs,
    "rbuild sector-type INDOOR CEMENT ROOFTOP",
    "d",
    "rbuild title {blu}C.O.F.O.B:{/blu} - Mess Hall stairs",
    "rbuild description " + mess_hall_upstairs,
    "rbuild sector-type INDOOR CEMENT ROOFTOP",
    repeat(2, [
      "w",
      "rbuild title {blu}C.O.F.O.B:{/blu} - Mess Hall stairs",
      "rbuild description " + mess_hall_upstairs,
      "rbuild sector-type INDOOR CEMENT ROOFTOP",
    ]),
    "d",
    "rbuild title {blu}C.O.F.O.B:{/blu} - Mess Hall stairs",
    "rbuild description " + mess_hall_upstairs,
    "rbuild sector-type INDOOR CEMENT ROOFTOP",
    repeat(2, [
      "w",
      "rbuild title {blu}C.O.F.O.B:{/blu} - Mess Hall stairs",
      "rbuild description " + mess_hall_upstairs,
      "rbuild sector-type INDOOR CEMENT ROOFTOP",
    ]),
    repeat(2, [
      "s",
      "rbuild title {blu}C.O.F.O.B:{/blu} - Mess Hall serving area",
      "rbuild description " + mess_hall_serving_area,
      "rbuild sector-type INDOOR CEMENT ROOFTOP",
    ]),
    repeat(2, [
      "e",
      "rbuild title {blu}C.O.F.O.B:{/blu} - Mess Hall serving area",
      "rbuild description " + mess_hall_serving_area,
      "rbuild sector-type INDOOR CEMENT ROOFTOP",
    ]),
    repeat(2, [
      "s",
      "rbuild title {blu}C.O.F.O.B:{/blu} - Mess Hall serving area",
      "rbuild description " + mess_hall_serving_area,
      "rbuild sector-type INDOOR CEMENT ROOFTOP",
    ]),
    repeat(2, ["n"]),
    repeat(2, [
      "e",
      "rbuild title {blu}C.O.F.O.B:{/blu} - Mess Hall serving area",
      "rbuild description " + mess_hall_serving_area,
      "rbuild sector-type INDOOR CEMENT ROOFTOP",
    ]),
    repeat(2, [
      "s",
      "rbuild title {blu}C.O.F.O.B:{/blu} - Mess Hall serving area",
      "rbuild description " + mess_hall_serving_area,
      "rbuild sector-type INDOOR CEMENT ROOFTOP",
    ]),
    repeat(2, ["n"]),
    repeat(2, [
      "e",
      "rbuild title {blu}C.O.F.O.B:{/blu} - Mess Hall serving area",
      "rbuild description " + mess_hall_serving_area,
      "rbuild sector-type INDOOR CEMENT ROOFTOP",
    ]),
    repeat(2, [
      "s",
      "rbuild title {blu}C.O.F.O.B:{/blu} - Mess Hall serving area",
      "rbuild description " + mess_hall_serving_area,
      "rbuild sector-type INDOOR CEMENT ROOFTOP",
    ]),
    repeat(2, ["n"]),
    go(cofob.mess_hall_upstairs),
  ]);
  return true;
}
function create_ammunition_locker() {
  var p = "consumable/";
  var ammo = [
    "consumable/sg3-ar-ammunition.yml",
    "consumable/sg3-lmg-ammunition.yml",
    "consumable/sg3-mp-ammunition.yml",
    "consumable/sg3-pistol-ammunition.yml",
    "consumable/sg3-shotgun-ammunition.yml",
    "consumable/sg3-smg-ammunition.yml",
    "consumable/sg3-sniper-ammunition.yml",
  ];

  var locker_commands = [];
  locker_commands.push("install_ammo_locker");
  for (var i = 0; i < ammo.length; i++) {
    locker_commands.push(ammo[i]);
  }
  locker_commands = locker_commands.join(" ");
  run(["uninstall_ammo_locker", locker_commands]);
}
function create_tier_1_weapons_locker() {
  var weapons = [
    "rifle/552-commando.yml",
    "rifle/aug-a3.yml",
    "rifle/augpara.yml",
    "rifle/czp10.yml",
    "rifle/desert-eagle.yml",
    "rifle/famas.yml",
    "rifle/fmg9.yml",
    "rifle/g36c.yml",
    "rifle/glock.yml",
    "rifle/hk21.yml",
    "rifle/hk45.yml",
    "rifle/l96aw.yml",
    "rifle/m16a4.yml",
    "rifle/m3.yml",
    "rifle/m4.yml",
    "rifle/magnum-revolver.yml",
    "rifle/mk46.yml",
    "rifle/mp5.yml",
    "rifle/mp9.yml",
    "rifle/p90.yml",
    "rifle/ppk.yml",
    "rifle/psg1.yml",
    "rifle/saiga12.yml",
    "rifle/sasg12.yml",
    "rifle/scarh.yml",
    "rifle/tar21.yml",
    "rifle/ump45.yml",
    "rifle/uzi.yml",
    "rifle/xm109.yml",
  ];

  var locker_commands = [];
  locker_commands.push("install_weapon_locker");
  for (var i = 0; i < weapons.length; i++) {
    locker_commands.push(weapons[i]);
  }
  locker_commands = locker_commands.join(" ");
  run(["uninstall_weapon_locker", locker_commands]);
}

function create_tier_1_armor_locker() {
  var weapons = [
    "armor/basic-ballistic-vest.yml",
    "armor/basic-boots.yml",
    "armor/raven-ultralight-backpack.yml",
    "armor/titan-elbow-guards.yml",
    "armor/titan-gauntlets.yml",
    "armor/titan-shin-guards.yml",
    "armor/titan-shoulder-pads.yml",
    "armor/xm-scorpio-belt.yml",
    "armor/xm-scorpio-tactical-gloves.yml",
  ];

  var locker_commands = [];
  locker_commands.push("install_armor_locker");
  for (var i = 0; i < weapons.length; i++) {
    locker_commands.push(weapons[i]);
  }
  locker_commands = locker_commands.join(" ");
  run(["uninstall_armor_locker", locker_commands]);
}
function place_cars_in_garage() {
  var commands = [
    "rnumtele 393",
    "zbuild yaml this #yaml|vehicle/p3-hunchbak.yml 402 1 0",
    "zbuild yaml this #yaml|vehicle/p3-offroad-mx3.yml 400 1 0",
    "zbuild yaml this #yaml|vehicle/prime-town-suv.yml 399 1 0",
    "zbuild yaml this #yaml|vehicle/lxr-sunrise.yml 395 1 0",
    "zbuild yaml this #yaml|vehicle/lxr-sport.yml 393 1 0",
    "zbuild yaml this #yaml|vehicle/lxr-sport.yml 394 1 0",
  ];
  run(commands);
}
function do_cobalt_fob() {
  var south_main_hallway = [
    "COBALT Air Force base. Ground zero for basic training. All initiates must follow ",
    "rules and guidelines in your New Recruit handbook. Proceed {grn}North{/grn} recruit!",
  ].join("");
  var south_hallway_east_west = [
    "The cement floor is a perfect complement to the reflection of the incandescent lights that ",
    "are installed into the the ceiling. {gld}To the East{/gld}, you will find the ",
    "Armory. {gld}To the West{/gld}, you will find the training grounds. Just around ",
    "the corner to the East and North a ways is the Weapons Locker. Feel free to get ",
    "acquainted with the layout, recruit.",
  ].join("");
  var eastern_hallway_south_corner = [
    "The hallway reaches north and south from here. A reduced temperature is like the result ",
    "of the industrial grade internal air cooling system. It isn't much, but it beats the ",
    "outside desert climate. The promise of coffee entices you, but you can't tell which direction ",
    "it's coming from. ",
  ].join("");
  var armory_entrance = [
    "A cool draft moves through the bottom crack of the door to the Armory Entrance to the East.",
    "It seems the quality of air drastically differs depending on the people in charge of each ",
    'department. The Sign above the door says in bold letters "Armory".',
  ].join("");
  var armory = [
    "As you push through to the East, you notice a few recruits putting on standard ",
    "issue gear. They ignore you as you take a look around. To the East is the buy station ",
    "where you can make your purchases.",
  ].join("");
  var armory_buy_station = [
    "You see an armor locker with standard issue equipment. Behind the counter is a {gld}list{/gld} of ",
    "all the various items for sell. You can spend {grn}MP{/grn} (Mission Points) here to upgrade your loadout. ",
    'To buy something, simply type {grn}"Buy ID"{/grn} where ID is the number next to the item you want in the ',
    "output of the list command.",
  ].join("");
  var armory_sec_room = [
    "A musty room with several freshly smoked cigars laying inside a deep ash tray the size of your fist. ",
    "Someone was loading a Mossberg shotgun and haphazardly left it laying upon the couch as if it were a ",
    "visitor. The T.V. appears to still be warm. Whoever was here is likely coming back soon. You have a ",
    "feeling whoever was here will be back shortly.",
  ].join("");
  var eastern_hallway_middle = [
    "The hallway stretches north and south. The cement floor shows serious signs of wear and tear. ",
    "It seems whoever was in charge of cleaning this portion of the base did a very poor job, which ",
    "surprises you. A ventilation shaft calmly blows semi-cold air throughout this portion of the hallway. ",
    "{gld}A Bulletin Board{/gld} lists various contracts.",
  ].join("");
  var armory_storage_room_north = [
    "Standard issue armor and defensive utilities line the walls; none of which you can take as they ",
    "are behind metal cages. There is, however, an Armor locker here with standard issue gear for ",
    "anyone to take. ",
  ].join("");
  var armory_storage_room_south = [
    "You enter the storage room and immediately notice the strong scent of sand, grime, and gasoline. ",
    "A few bits of ammunition are strewn across the floor haphazardly. The Armory personnell either ",
    "recently dug through the piles of ammo crates, or nobody bothered to clean this mess up. ",
    "There seems to be a computer terminal on the East wall.",
    /** TODO: put computer terminal on east wall */
  ].join("");
  var eastern_hallway = [
    "The corridor is dimly lit by flourescent lights embedded into the metal ceiling. The corners ",
    "where the wall meets the ceiling look damp and dusty at the same time. The air is a cool contrast ",
    "to the outside desert air. You can't tell if it's day or night outside, but the low level of light ",
    "tricks your mind into thinking it's daytime all day.",
  ].join("");
  var eastern_exit = [
    "Before you is a sturdy metal door that prevents the outside elements from making their way ",
    "inside. The door simply says {yel}Eastern Exit{/yel}. The air is less cool as ",
    "the corridor leading to the east lacks the sufficient air flow. There are no ventilation ",
    "shafts leading to the east, but you can see through the tempered glass window on the door ",
    "that a few military police are gaurding the exit to the city outside. ",
  ].join("");
  var eastern_dmz_corridor = [
    "Dirt stretches to the east towards a heavy containment door. Behind ",
    "that door must be where you can leave the building. The corridor is dimly ",
    "lit. The air is dry and smells of a combination of sweat and gasoline for some ",
    "reason.",
  ].join("");
  var eastern_dmz_trap_door = [
    "Beneath the floor here is an entrance to an underground tunnel. The only problem ",
    "is that you can't seem to get it to move. To the east is the door to the outside world.",
    "A falcon engraving is above the door frame to the east.",
  ].join("");
  var abott_market_south = [
    "Crushed and flattened by endless shells, the overpass that you just passed under creaks ",
    "eternally. Now and then you hear the crumble of asphault and be bend of metal rods. The ",
    "overpass connects the once thriving market place of Abott Market, but now all you see is ",
    "rubble and a shell of what used to be a thriving market.",
  ].join("");
  var abott_market_abandoned_street_middle = [
    "A trio of burning cars have become part of the debris scattered along the street. ",
    "Way off to the north, you spot a working overpass. Deep long scars of blackened ",
    "concrete tell a tale of destruction. To the west lies a small set of apartments ",
    "once owned by the only property company to operate in this desolate portion of town. ",
    "A giant construction crane is resting in the middle of the street to the north.",
  ].join("");
  var market_apt_east_entrance = [
    "An iron rod gate can be seen laying discarded near the entrance. In another time, ",
    "that gate would have been used to keep intruders out. Far off to the west is what ",
    "used to be the tenant parking lot. You'd be surprised to find any tenant using that ",
    "lot seeing as how low the income requirements were for this apartment complex.",
  ].join("");
  var market_apt_building_3 = [
    "You see a two story apartment building with 8 units. Each apartment is a corner unit, ",
    "but neither home could possibly be more than a studio unit. A few of the doors are ",
    "closed, which you find peculiar. There can't possibly be anyone living there. Right?",
  ].join("");
  var market_apt_building_2 = [
    "Building 2 is a two story building with 16 units. The stairs leading to the second ",
    "story are completely demolished. The top 4 units to the east are completely exposed ",
    "to the elements. You could make it upstairs but it would require some sort of rope. ",
  ].join("");
  var market_apt_building_1 = [
    "Building 1 is a two story building with 8 units with the added addition of an exercise ",
    "room and what looks like the property manager's office. Both the exercise room and the office ",
    "look surprisingly intact. You notice lots of fresh footprints to and from building 1 ",
    "which disappear into the destruction of building 2. Despite the abandoned nature of ",
    "the complex as a whole, someone is still finding use for these units.",
  ].join("");
  var market_apt_building_3_stairs = [
    "Before you lie the stairs that will help you reach the second floor of building 3. ",
    "The handrails are extremely hot to the touch due to baking here under the steady gaze ",
    "of the sun over your shoulder.",
  ].join("");
  var market_apt_building_3_second_floor_hallway = [
    "A thin layer of dust and dirt cover the cement walkway here. You hear the crackle ",
    "of dirt and millions of pieces of broken glass under your feet as you make your ",
    "way about. Oddly enough, there are footprints here.",
  ].join("");
  var market_apt_unit_301 = [
    "A mostly empty apartment. You notice what looks like sleeping bags in the corner ",
    "but that could just be trash. The air smells like cigarette smoke and dry desert ",
    "air. The light from outside illuminates trillions of dust particles floating in the ",
    "in perpetual motion.",
  ].join("");
  var market_apt_unit_301_bathroom = [
    "The sink seems to still be intact and usable. How any water made its way up here ",
    "would be nothing short of miraculous. It does appear that the sink has been used ",
    "recently, which only affirms your suspicion that there are still inhabitants who ",
    "dwell here.",
  ].join("");
  var shooting_range_gear_room = [
    "A huge weapons rack is attached to the wall here. The rack contains rifles, ",
    "pistols, ammunition, explosives, and breach charges.",
  ].join("");

  var market_apt_unit_303 = market_apt_unit_301;
  var market_apt_unit_303_bathroom = market_apt_unit_301_bathroom;
  var market_apt_unit_303_bedroom = market_apt_unit_301_bedroom;

  var market_apt_unit_305 = market_apt_unit_301;
  var market_apt_unit_305_bathroom = market_apt_unit_301_bathroom;
  var market_apt_unit_305_bedroom = market_apt_unit_301_bedroom;

  var market_apt_unit_307 = market_apt_unit_301;
  var market_apt_unit_307_bathroom = market_apt_unit_301_bathroom;
  var market_apt_unit_307_bedroom = market_apt_unit_301_bedroom;

  var market_apt_unit_301_bedroom = [
    "A D.I.Y. home stove sits in the middle of the room. Aside from that, ",
    "there's nothing to look at. The carpet has been pulled up in places. ",
    "The walls are covered with massive holes and random burn marks.",
  ].join("");
  var fm = "@FILL_ME@";
  var mess_hall_upstairs = [
    "Metal railings accompany the stairs leading down to the mess hall.",
  ].join("");
  var mess_hall = [
    "The tile floor is immaculate in presentation. Plain grey metal chairs attend each ",
    "table obediently in a neat formation of 2 chairs to each cardinal side. To the south ",
    "is the cooking area where the chefs fill plates full of food.",
  ].join("");
  var eastern_hallway_middle = fm;
  var shooting_range_entrance_east = fm;
  var shooting_range_falcon = fm;
  var second_floor_hallway = fm;
  var north_hallway = fm;
  var north_hallway_stairs = fm;

  var FILL_ME = "@FILL_ME@";
  run([
    "rbuild pave on",
    "n",
    "rbuild title {blu}COBALT:{/blu} {grn}Forward Operating Base{/grn} - Main Hallway",
    "rbuild description " + south_main_hallway,
    "rbuild sector-type INDOOR_CEMENT",
    "n",
    "rbuild title {blu}C.O.F.O.B:{/blu} - Southern Hallway",
    "rbuild description " + south_hallway_east_west,
    "rbuild sector-type INDOOR_CEMENT",
    mark(cofob.center),
    repeat(6, [
      "w",
      "rbuild title {blu}C.O.F.O.B:{/blu} - Southern Hallway",
      "rbuild description " + south_hallway_east_west,
      "rbuild sector-type INDOOR_CEMENT",
    ]),

    go(cofob.center),
    "e",
    "rbuild title {blu}C.O.F.O.B:{/blu} - Southern Hallway",
    "rbuild description " + south_hallway_east_west,
    "rbuild sector-type INDOOR_CEMENT",
    "e",
    "rbuild title {blu}C.O.F.O.B:{/blu} - Southern Hallway",
    "rbuild description " + south_hallway_east_west,
    "rbuild sector-type INDOOR_CEMENT",
    "n",
    "rbuild title {blu}C.O.F.O.B:{/blu} - Southeast Corner",
    "rbuild description " + eastern_hallway_south_corner,
    "rbuild sector-type INDOOR_CEMENT",
    "n",
    "rbuild title {blu}C.O.F.O.B:{/blu} - Armory Entrance",
    "rbuild description " + armory_entrance,
    "rbuild sector-type INDOOR_CEMENT",
    mark(cofob.armory_entrance),
    "e",
    "rbuild title {blu}C.O.F.O.B:{/blu} - Armory",
    "rbuild description " + armory,
    "rbuild sector-type INDOOR_CEMENT",
    "e",
    "rbuild title {blu}C.O.F.O.B:{/blu} - Armory Buy Station",
    "rbuild description " + armory_buy_station,
    "rbuild sector-type INDOOR_CEMENT",
    "n",
    "rbuild title {blu}C.O.F.O.B:{/blu} - Armory Storage Room North",
    "rbuild description " + armory_storage_room_north,
    "rbuild sector-type INDOOR_CEMENT",
    "s",
    "s",
    "rbuild title {blu}C.O.F.O.B:{/blu} - Armory Storage Room South",
    "rbuild description " + armory_storage_room_south,
    "rbuild sector-type INDOOR_CEMENT",
    "n",
    "e",
    "rbuild title {blu}C.O.F.O.B:{/blu} - Armory - Secluded Room",
    "rbuild description " + armory_sec_room,
    "rbuild sector-type INDOOR_CEMENT",
    "e",
    "rbuild title {blu}C.O.F.O.B:{/blu} - Armory - Secluded Room - Weapons Cache",
    "rbuild description " + fm,
    "rbuild sector-type INDOOR_CEMENT",
    "d",
    "rbuild title {blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - TOP",
    "rbuild description " + fm,
    "rbuild sector-type INDOOR_CEMENT",
    "e",
    "rbuild title {blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - TOP",
    "rbuild description " + fm,
    "rbuild sector-type INDOOR_CEMENT",
    "e",
    "rbuild title {blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - TOP",
    "rbuild description " + fm,
    "rbuild sector-type INDOOR_CEMENT",
    "d",
    "rbuild title {blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 1",
    "rbuild description " + fm,
    "rbuild sector-type INDOOR_CEMENT",
    "e",
    "rbuild title {blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 1",
    "rbuild description " + fm,
    "rbuild sector-type INDOOR_CEMENT",
    "e",
    "rbuild title {blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 1",
    "rbuild description " + fm,
    "rbuild sector-type INDOOR_CEMENT",
    "d",
    "rbuild title {blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 2",
    "rbuild description " + fm,
    "rbuild sector-type INDOOR_CEMENT",
    "e",
    "rbuild title {blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 2",
    "rbuild description " + fm,
    "rbuild sector-type INDOOR_CEMENT",
    "e",
    "rbuild title {blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 2",
    "rbuild description " + fm,
    "rbuild sector-type INDOOR_CEMENT",
    mark(cofob.armory_b2),
    repeat(5, [
      "s",
      "rbuild title {blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 2",
      "rbuild description " + fm,
      "rbuild sector-type INDOOR_CEMENT",
    ]),

    repeat(5, [
      "w",
      "rbuild title {blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 2",
      "rbuild description " + fm,
      "rbuild sector-type INDOOR_CEMENT",
    ]),
    repeat(5, [
      "n",
      "rbuild title {blu}C.O.F.O.B:{/blu} - Weapons Cache stairs - Basement 2",
      "rbuild description " + fm,
      "rbuild sector-type INDOOR_CEMENT",
    ]),
    repeat(2, ["s"]),
    repeat(2, [
      "e",
      "rbuild title {blu}C.O.F.O.B:{/blu} - Basement 2 - Breach charges",
      "rbuild description " + fm,
      "rbuild sector-type INDOOR_CEMENT",
    ]),
    repeat(2, ["w"]),
    mark(cofob.armory_b2),
    repeat(5, ["s"]),
    repeat(2, ["w"]),
    mark("cofob-armory-waypoint-avenue-exit"),
    "rbuild title {blu}C.O.F.O.B:{/blu} - Basement 2 - Waypoint Avenue North Exit",
    "rbuild description " + fm,
    "rbuild sector-type INDOOR_CEMENT",
    repeat(2, [
      "s",
      "rbuild title {blu}C.O.F.O.B:{/blu} - Basement 2 - Car Garage",
      "rbuild description " + fm,
      "rbuild sector-type OUTDOOR_CEMENT",
    ]),
    "w",
    "rbuild title {blu}C.O.F.O.B:{/blu} - Basement 2 - Car Garage - 1A",
    "rbuild description " + fm,
    "rbuild sector-type OUTDOOR_CEMENT",
    //'rbuild vnum ' + garage['1a-1'].vnum,
    "w",
    "rbuild title {blu}C.O.F.O.B:{/blu} - Basement 2 - Car Garage - 1A",
    "rbuild description " + fm,
    "rbuild sector-type OUTDOOR_CEMENT",
    //'rbuild vnum ' + garage['1a-2'].vnum,
    repeat(2, ["e"]),
    "e",
    "rbuild title {blu}C.O.F.O.B:{/blu} - Basement 2 - Car Garage - 1B",
    "rbuild description " + fm,
    "rbuild sector-type OUTDOOR_CEMENT",
    //'rbuild vnum ' + garage['1b-1']['vnum'],
    "e",
    "rbuild title {blu}C.O.F.O.B:{/blu} - Basement 2 - Car Garage - 1B",
    "rbuild description " + fm,
    "rbuild sector-type OUTDOOR_CEMENT",
    //'rbuild vnum ' + garage['1b-2']['vnum'],
    repeat(2, ["w"]),
    repeat(2, [
      "s",
      "rbuild title {blu}C.O.F.O.B:{/blu} - Basement 2 - Car Garage",
      "rbuild description " + fm,
      "rbuild sector-type OUTDOOR_CEMENT",
    ]),
    repeat(2, [
      "w",
      "rbuild title {blu}C.O.F.O.B:{/blu} - Basement 2 - Car Garage - 2A",
      "rbuild description " + fm,
      "rbuild sector-type OUTDOOR_CEMENT",
    ]),
    repeat(2, ["e"]),
    repeat(2, [
      "e",
      "rbuild title {blu}C.O.F.O.B:{/blu} - Basement 2 - Car Garage - 2B",
      "rbuild description " + fm,
      "rbuild sector-type OUTDOOR_CEMENT",
    ]),
    repeat(2, ["w"]),
    repeat(2, [
      "s",
      "rbuild title {blu}C.O.F.O.B:{/blu} - Basement 2 - Car Garage",
      "rbuild description " + fm,
      "rbuild sector-type OUTDOOR_CEMENT",
    ]),
    "w",
    "rbuild title Waypoint North",
    "rbuild description " + fm,
    "rbuild sector-type OUTDOOR_CEMENT",
    mark("waypoint-car-garage-north-1"),

    go(cofob.armory_entrance),
    /** back to hallway */
    repeat(6, [
      "n",
      "rbuild title {blu}C.O.F.O.B:{/blu} - Eastern Hallway",
      "rbuild description " + eastern_hallway_middle,
      "rbuild sector-type INDOOR_CEMENT",
    ]),
    "n",
    "rbuild title {blu}C.O.F.O.B:{/blu} - Eastern Exit",
    "rbuild description " + eastern_exit,
    "rbuild sector-type INDOOR_CEMENT",
    mark(cofob.east_exit),
    /** end of eastern section */
  ]);

  if (!do_second_floor()) {
    return;
  }
  var range_locker = [
    "rifle/g36c.yml",
    "explosive/breach-charge.yml",
    "explosive/claymore-mine.yml",
    "explosive/emp-grenade.yml",
    "explosive/frag-grenade.yml",
    "explosive/incendiary-grenade.yml",
    "explosive/sensor-grenade.yml",
    "explosive/smoke-grenade.yml",
    "explosive/flashbang-grenade.yml",
    "rifle/g36c.yml",
    "rifle/czp10.yml",
    "attachment/ar-556mm-mag.yml",
    "attachment/pistol-9mm-mag.yml",
  ];
  var do_range_locker = function () {
    var locker_commands = [];
    locker_commands.push("install_weapon_locker");
    for (var i in range_locker) {
      locker_commands.push(range_locker[i]);
    }
    locker_commands = locker_commands.join(" ");
    run([locker_commands]);
    var quota = [];
    for (var i in range_locker) {
      quota.push("weapon_locker_quota " + range_locker[i] + " 10");
    }
    run(quota);
  };

  run([
    go(cofob.east_exit), //'eastexit'),
    "n",
    "n",
    "rbuild title {blu}C.O.F.O.B:{/blu} - Eastern Hallway",
    "rbuild description " + eastern_hallway_middle,
    "rbuild sector-type INDOOR_CEMENT",
    "n",
    "rbuild title {blu}C.O.F.O.B:{/blu} - Eastern Hallway",
    "rbuild description " + eastern_hallway_middle,
    "rbuild sector-type INDOOR_CEMENT",
    "n",
    "rbuild title {blu}C.O.F.O.B:{/blu} - Eastern Hallway",
    "rbuild description " + eastern_hallway_middle,
    "rbuild sector-type INDOOR_CEMENT",
    "e",
    "rbuild title {blu}C.O.F.O.B:{/blu} - Eastern Hallway",
    "rbuild description " + eastern_hallway_middle,
    "rbuild sector-type INDOOR_CEMENT",
    "e",
    "rbuild title {blu}C.O.F.O.B:{/blu} - Gear room",
    "rbuild description " + shooting_range_gear_room,
    "rbuild sector-type INDOOR CEMENT ROOFTOP",
    mark(cofob.gear_room),
    "n",
    "rbuild title {blu}C.O.F.O.B:{/blu} - Shooting Range Area Falcon",
    "rbuild description " + shooting_range_falcon,
    "rbuild sector-type INDOOR CEMENT ROOFTOP",
    "n",
    "rbuild title {blu}C.O.F.O.B:{/blu} - Shooting Range Area Falcon",
    "rbuild description " + shooting_range_falcon,
    "rbuild sector-type INDOOR CEMENT ROOFTOP",
    "n",
    "rbuild title {blu}C.O.F.O.B:{/blu} - Shooting Range Area Falcon",
    "rbuild description " + shooting_range_falcon,
    "rbuild sector-type INDOOR CEMENT ROOFTOP",
    "s",
    "w",
    "rbuild title {blu}C.O.F.O.B:{/blu} - Shooting Range Area Falcon",
    "rbuild description " + shooting_range_falcon,
    "rbuild sector-type INDOOR CEMENT ROOFTOP",
    "e",
    "e",
    "rbuild title {blu}C.O.F.O.B:{/blu} - Shooting Range Area Falcon",
    "rbuild description " + shooting_range_falcon,
    "rbuild sector-type INDOOR CEMENT ROOFTOP",
    "w",
    "u",
    "rbuild title {blu}C.O.F.O.B:{/blu} - Shooting Range Area Falcon",
    "rbuild description " + shooting_range_falcon,
    "rbuild sector-type INDOOR CEMENT ROOFTOP",
    "d",
    "d",
    "rbuild title {blu}C.O.F.O.B:{/blu} - Shooting Range Area Falcon",
    "rbuild description " + shooting_range_falcon,
    "rbuild sector-type INDOOR CEMENT ROOFTOP",
    "u",

    go(cofob.east_exit), //'eastexit'),
    "e",
    "rbuild title {blu}C.O.F.O.B [OUTER]:{/blu} - Eastern DMZ corridor",
    "rbuild description " + eastern_dmz_corridor,
    "rbuild texture OUTSIDE DIRT DRY DESERT AIR ROOFTOP TUNNEL FENCED",
    "e",
    "rbuild title {blu}C.O.F.O.B [OUTER]:{/blu} - Eastern DMZ corridor",
    "rbuild description " + eastern_dmz_corridor,
    "rbuild texture OUTSIDE DIRT DRY DESERT AIR ROOFTOP TUNNEL FENCED",
    "e",
    "rbuild title {blu}C.O.F.O.B [OUTER]:{/blu} - Eastern DMZ corridor",
    "rbuild description " + eastern_dmz_corridor,
    "rbuild texture OUTSIDE DIRT DRY DESERT AIR ROOFTOP TUNNEL FENCED",
    "e",
    "rbuild title {blu}C.O.F.O.B [OUTER]:{/blu} - Fenced off DMZ corridor",
    "rbuild description " + eastern_dmz_trap_door,
    "rbuild texture METAL_HATCH OUTSIDE DIRT DRY DESERT AIR ROOFTOP TUNNEL FENCED",
    "e",
    "rbuild title Destroyed overpass - Abott Market South",
    "rbuild description " + abott_market_south,
    "rbuild texture OUTSIDE DIRT DRY DESERT AIR RUBBLE STREET",
    mark(cofob.abbotstart), //'abbotstart'),
    //'n',
    //'rbuild title Destroyed overpass - Abott Market South',
    //'rbuild description ' + abott_market_south,
    //'rbuild texture OUTSIDE DIRT DRY DESERT AIR RUBBLE STREET',
    "e",
    "rbuild title Destroyed overpass - Abott Market South",
    "rbuild description " + abott_market_south,
    "rbuild texture OUTSIDE DIRT DRY DESERT AIR RUBBLE STREET",
    "n",
    "rbuild title Destroyed overpass - Abott Market South",
    "rbuild description " + abott_market_south,
    "rbuild texture OUTSIDE DIRT DRY DESERT AIR RUBBLE STREET",
    "n",
    "rbuild title Destroyed overpass - Abott Market South",
    "rbuild description " + abott_market_south,
    "rbuild texture OUTSIDE DIRT DRY DESERT AIR RUBBLE STREET",
    repeat(9, [
      "n",
      "rbuild title Destroyed overpass - Abott Market South",
      "rbuild description " + abott_market_south,
      "rbuild texture OUTSIDE DIRT DRY DESERT AIR RUBBLE STREET",
    ]),

    "w",
    "rbuild title Market Apartments - East entrance",
    "rbuild description " + market_apt_east_entrance,
    "rbuild texture OUTSIDE DIRT DRY DESERT AIR",
    "w",
    "rbuild title Market Apartments - East entrance",
    "rbuild description " + market_apt_east_entrance,
    "rbuild texture OUTSIDE DIRT DRY DESERT AIR",

    "n",
    "rbuild title Market Apartments - Building Way",
    "rbuild description " + market_apt_building_2,
    "rbuild texture OUTSIDE DIRT DRY DESERT AIR RUBBLE STREET",
    "n",
    "rbuild title Market Apartments - Building Way",
    "rbuild description " + market_apt_building_2,
    "rbuild texture OUTSIDE DIRT DRY DESERT AIR RUBBLE STREET",
    "w",
    "rbuild title Market Apartments - Building Way",
    "rbuild description " + market_apt_building_2,
    "rbuild texture OUTSIDE DIRT DRY DESERT AIR RUBBLE STREET",
    "n",
    "rbuild title Market Apartments - Building 3",
    "rbuild description " + market_apt_building_3,
    "rbuild texture OUTSIDE DIRT DRY DESERT AIR RUBBLE CEMENT",
    "n",
    "rbuild title Market Apartments - Stairs",
    "rbuild description " + market_apt_building_3_stairs,
    "rbuild texture OUTSIDE DIRT DRY DESERT AIR STAIRS",
    "u",
    "rbuild title Market Apartments - Stairs",
    "rbuild description " + market_apt_building_3_stairs,
    "rbuild texture OUTSIDE DIRT DRY DESERT AIR STAIRS",
    "n",
    "rbuild title Market Apartments - Stairs",
    "rbuild description " + market_apt_building_3_stairs,
    "rbuild texture OUTSIDE DIRT DRY DESERT AIR STAIRS",
    "u",
    "rbuild title Market Apartments - Building 3 Second floor hallway",
    "rbuild description " + market_apt_building_3_second_floor_hallway,
    "rbuild texture OUTSIDE DIRT DRY DESERT AIR SHATTERED_GLASS",
    "n",
    "rbuild title Market Apartments - Building 3 Second floor hallway",
    "rbuild description " + market_apt_building_3_second_floor_hallway,
    "rbuild texture OUTSIDE DIRT DRY DESERT AIR SHATTERED_GLASS",
    "w",
    "rbuild title Market Apartments - Building 3 Second floor hallway",
    "rbuild description " + market_apt_building_3_second_floor_hallway,
    "rbuild texture OUTSIDE DIRT DRY DESERT AIR SHATTERED_GLASS",
    "w",
    "rbuild title Market Apartments - Building 3 Second floor hallway",
    "rbuild description " + market_apt_building_3_second_floor_hallway,
    "rbuild texture OUTSIDE DIRT DRY DESERT AIR SHATTERED_GLASS",
    "w",
    "rbuild title Market Apartments - Building 3 Second floor hallway",
    "rbuild description " + market_apt_building_3_second_floor_hallway,
    "rbuild texture OUTSIDE DIRT DRY DESERT AIR SHATTERED_GLASS",

    "n",
    "rbuild title Market Apartments - Building 3 - Unit 301",
    "rbuild description " + market_apt_unit_301,
    "rbuild texture INSIDE TRASHY DRY DESERT AIR",
    "n",
    "rbuild title Market Apartments - Building 3 - Unit 301",
    "rbuild description " + market_apt_unit_301,
    "rbuild texture INSIDE TRASHY DRY DESERT AIR",
    "n",
    "rbuild title Market Apartments - Building 3 - Unit 301 - Master Bedroom",
    "rbuild description " + market_apt_unit_301_bedroom,
    "rbuild texture INSIDE TRASHY DRY DESERT AIR",
    "s",
    "e",
    "rbuild title Market Apartments - Building 3 - Unit 301 - Bathroom",
    "rbuild description " + market_apt_unit_301_bathroom,
    "rbuild texture INSIDE TRASHY DRY DESERT AIR",
    "w",
    "s",
    "s",

    "w",
    "rbuild title Market Apartments - Building 3 Second floor hallway",
    "rbuild description " + market_apt_building_3_second_floor_hallway,
    "rbuild texture OUTSIDE DIRT DRY DESERT AIR SHATTERED_GLASS",
    "w",
    "rbuild title Market Apartments - Building 3 Second floor hallway",
    "rbuild description " + market_apt_building_3_second_floor_hallway,
    "rbuild texture OUTSIDE DIRT DRY DESERT AIR SHATTERED_GLASS",
    "w",
    "rbuild title Market Apartments - Building 3 Second floor hallway",
    "rbuild description " + market_apt_building_3_second_floor_hallway,
    "rbuild texture OUTSIDE DIRT DRY DESERT AIR SHATTERED_GLASS",

    "n",
    "rbuild title Market Apartments - Building 3 - Unit 303",
    "rbuild description " + market_apt_unit_303,
    "rbuild texture INSIDE TRASHY DRY DESERT AIR",
    "n",
    "rbuild title Market Apartments - Building 3 - Unit 303",
    "rbuild description " + market_apt_unit_303,
    "rbuild texture INSIDE TRASHY DRY DESERT AIR",
    "n",
    "rbuild title Market Apartments - Building 3 - Unit 303 - Master Bedroom",
    "rbuild description " + market_apt_unit_303_bedroom,
    "rbuild texture INSIDE TRASHY DRY DESERT AIR",
    "s",
    "e",
    "rbuild title Market Apartments - Building 3 - Unit 303 - Bathroom",
    "rbuild description " + market_apt_unit_303_bathroom,
    "rbuild texture INSIDE TRASHY DRY DESERT AIR",
    "w",
    "s",
    "s",

    "e",
    "e",
    "e",
    "e",
    "e",
    "e",
    "e",
    "rbuild title Market Apartments - Building 3 Second floor hallway",
    "rbuild description " + market_apt_building_3_second_floor_hallway,
    "rbuild texture OUTSIDE DIRT DRY DESERT AIR SHATTERED_GLASS",

    "e",
    "rbuild title Market Apartments - Building 3 Second floor hallway",
    "rbuild description " + market_apt_building_3_second_floor_hallway,
    "rbuild texture OUTSIDE DIRT DRY DESERT AIR SHATTERED_GLASS",
    "n",
    "rbuild title Market Apartments - Building 3 - Unit 305",
    "rbuild description " + market_apt_unit_305,
    "rbuild texture INSIDE TRASHY DRY DESERT AIR",
    "n",
    "rbuild title Market Apartments - Building 3 - Unit 305",
    "rbuild description " + market_apt_unit_305,
    "rbuild texture INSIDE TRASHY DRY DESERT AIR",
    "n",
    "rbuild title Market Apartments - Building 3 - Unit 305 - Master Bedroom",
    "rbuild description " + market_apt_unit_305_bedroom,
    "rbuild texture INSIDE TRASHY DRY DESERT AIR",
    "s",
    "e",
    "rbuild title Market Apartments - Building 3 - Unit 305 - Bathroom",
    "rbuild description " + market_apt_unit_305_bathroom,
    "rbuild texture INSIDE TRASHY DRY DESERT AIR",
    "w",
    "s",
    "s",

    "e",
    "e",
    "rbuild title Market Apartments - Building 3 Second floor hallway",
    "rbuild description " + market_apt_building_3_second_floor_hallway,
    "rbuild texture OUTSIDE DIRT DRY DESERT AIR SHATTERED_GLASS",
    "e",
    "rbuild title Market Apartments - Building 3 Second floor hallway",
    "rbuild description " + market_apt_building_3_second_floor_hallway,
    "rbuild texture OUTSIDE DIRT DRY DESERT AIR SHATTERED_GLASS",
    "n",
    "rbuild title Market Apartments - Building 3 - Unit 307",
    "rbuild description " + market_apt_unit_307,
    "rbuild texture INSIDE TRASHY DRY DESERT AIR",
    "n",
    "rbuild title Market Apartments - Building 3 - Unit 307",
    "rbuild description " + market_apt_unit_307,
    "rbuild texture INSIDE TRASHY DRY DESERT AIR",
    "n",
    "rbuild title Market Apartments - Building 3 - Unit 307 - Master Bedroom",
    "rbuild description " + market_apt_unit_307_bedroom,
    "rbuild texture INSIDE TRASHY DRY DESERT AIR",
    "s",
    "e",
    "rbuild title Market Apartments - Building 3 - Unit 307 - Bathroom",
    "rbuild description " + market_apt_unit_307_bathroom,
    "rbuild texture INSIDE TRASHY DRY DESERT AIR",
    "w",
    "s",
    "s",
    "w",
    "w",
    "w",
    "w",

    "w",
    "s",
    "d",
    "s",
    "d",
    "s",

    "s",
    "w",
    "rbuild title Market Apartments - Building Way",
    "rbuild description " + market_apt_building_2,
    "rbuild texture OUTSIDE DIRT DRY DESERT AIR RUBBLE STREET",
    "w",
    "rbuild title Market Apartments - Building Way",
    "rbuild description " + market_apt_building_2,
    "rbuild texture OUTSIDE DIRT DRY DESERT AIR RUBBLE STREET",
    "w",
    "rbuild title Market Apartments - Building Way",
    "rbuild description " + market_apt_building_2,
    "rbuild texture OUTSIDE DIRT DRY DESERT AIR RUBBLE STREET",
    "n",
    "rbuild title Market Apartments - Building 2",
    "rbuild description " + market_apt_building_1,
    "rbuild texture OUTSIDE DIRT DRY DESERT AIR RUBBLE STREET",
    "s",
    "e",
    "e",
    "e",
    "e",
    "s",
    "s",
    "e",
    "e",

    "n",
    "rbuild title Abandoned Two way street - Abott Market",
    "rbuild description " + abott_market_abandoned_street_middle,
    "rbuild texture OUTSIDE STREET DIRT DRY DESERT AIR STREET RUBBLE",
    "n",
    "rbuild title Abandoned Two way street - Abott Market",
    "rbuild description " + abott_market_abandoned_street_middle,
    "rbuild texture OUTSIDE STREET DIRT DRY DESERT AIR STREET RUBBLE",
    "n",
    "rbuild title Abandoned Two way street - Abott Market",
    "rbuild description " + abott_market_abandoned_street_middle,
    "rbuild texture OUTSIDE STREET DIRT DRY DESERT AIR STREET RUBBLE",
    "n",
    "rbuild title Abandoned Two way street - Abott Market",
    "rbuild description " + abott_market_abandoned_street_middle,
    "rbuild texture OUTSIDE STREET DIRT DRY DESERT AIR STREET RUBBLE",
    "n",
    "rbuild title Abandoned Two way street - Abott Market",
    "rbuild description " + abott_market_abandoned_street_middle,
    "rbuild texture OUTSIDE STREET DIRT DRY DESERT AIR STREET RUBBLE",
  ]);
  do_cofob_west_area_from("cofobcenter");
  //go(cofob.gear_room);
  //do_range_locker();

  do_mess_hall();
  run(["rbuild pave off"]);
  //run([go('waypoint-car-garage-north-1')]);
}

function do_psi_tech_building(){
  var fm = "@FILL_ME@";
  var west_courtyard_entrance = [
		"The entrance to {blu}Psi-Tech{/blu} is a stark contrast to the poverty of ",
		"the surrounding area. The outside of the building is a bland grey-blue mix. ",
		"Each walkway has stairs and a futuristic looking angular railing. Each corner ",
		"of the paved sidewalk looks like a sleek cutout from a futuristic jet.",
		"The walkway curves down towards the main entrance. You notice a few cameras watching your movement.",
	].join('');
  var east_courtyard = [
		"This side of the {blu}Psi-Tech{/blu} building has a peculiar lack of windows and ",
		"entrances. A soft humming sound gives you the impression that a power generator ",
		"is nearby, though you cannot possibly imagine where.",
	].join('');
  var vip_dropoff = [
		"This area contains many high end vehicles fit for the high value targets that use them.",
		"This place is heavily guarded and monitored at all times.",
	].join('');
	var outer_txt = "OUTSIDE GLASS_WINDOWS SIDEWALK NARROW_EAST_WEST";
	var south_entrance = [
		"Large marble pillars 6 feet in diameter flank you on both sides. Every ten feet a new ",
		"row of pillars. The entrance to {blu}Psi-Tech{/blu} is heavily guarded. An array of ",
		"cameras at the end of the atrium monitor your movements.",
	].join('');
  var atrium = [
		"You find yourself in a perfectly air conditioned atrium with very high ceilings.",
		"Classical music is being played through a pair of hidden speakers somewhere above ",
		"you. The obsidian colored marble floor is smooth yet slip-resistant.",
	].join('');
  //var cofob_center_vnum = exec("print_vnum " + cofob.center);
  run([
		"recall",
		"n",
		"n",
		"e",
		"e",
		repeat(9, ["n"]),
		repeat(6, ["e"]),
		repeat(15,["n"]),
		
		"rbuild pave continue",
    /** start of mess hall */
		mark(psitech.start),
    repeat(2, [
      "e",
      title("{blu}Psi-Tech H.Q.{/blu} - Outside - West courtyard entrance"),
      desc(west_courtyard_entrance),
			texture(outer_txt),
      "rbuild sector-type OUTDOOR_CEMENT",
    ]),
    repeat(2, [
      "n",
      title("{blu}Psi-Tech H.Q.{/blu} - Outside - West courtyard"),
      desc(west_courtyard_entrance),
			texture(outer_txt),
      "rbuild sector-type OUTDOOR_CEMENT",
    ]),
		mark(psitech.sniper_post_west),
    repeat(3, [
      "e",
      title("{blu}Psi-Tech H.Q.{/blu} - Outside - Main courtyard"),
      desc(west_courtyard_entrance),
			texture(outer_txt),
      "rbuild sector-type OUTDOOR_CEMENT",
    ]),
		mark(psitech.sentry_gun_A),

		go(psitech.sentry_gun_A),
		"n",
		title("{blu}Psi-Tech H.Q.{/blu} - Outside - South entrance"),
		desc(south_entrance),
		texture("OUTSIDE DRY"),
		"rbuild sector-type OUTDOOR_CEMENT",

		mark(psitech.guard_A),

		repeat(2, ["e",
			title("{blu}Psi-Tech H.Q.{/blu} - Outside - South entrance"),
			desc(south_entrance),
			texture("OUTSIDE DRY"),
			"rbuild sector-type OUTDOOR_CEMENT",
		]),
		mark(psitech.guard_B),

		"w",
		mark(psitech.south_door),
		"e",
		"s",
      title("{blu}Psi-Tech H.Q.{/blu} - Outside - Main courtyard"),
      desc(west_courtyard_entrance),
			texture(outer_txt),
      "rbuild sector-type OUTDOOR_CEMENT",
		mark(psitech.sentry_gun_B),
    repeat(5, [
      "e",
      title("{blu}Psi-Tech H.Q.{/blu} - Outside - Main courtyard"),
      desc(west_courtyard_entrance),
			texture(outer_txt),
      "rbuild sector-type OUTDOOR_CEMENT",
    ]),
		mark(psitech.sniper_post_east),
    repeat(4, [
      "n",
      title("{blu}Psi-Tech H.Q.{/blu} - Outside - East courtyard"),
      desc(east_courtyard),
			texture("OUTSIDE DRY SIDEWALK NARROW_NORTH_SOUTH"),
      "rbuild sector-type OUTDOOR_CEMENT",
    ]),
      "e",
      title("{blu}Psi-Tech H.Q.{/blu} - Outside - East courtyard"),
      desc(east_courtyard),
			texture("OUTSIDE DRY SIDEWALK NARROW_NORTH_SOUTH"),
      "rbuild sector-type OUTDOOR_CEMENT",
      "n",
      title("{blu}Psi-Tech H.Q.{/blu} - Outside - East courtyard"),
      desc(east_courtyard),
			texture("OUTSIDE DRY SIDEWALK NARROW_NORTH_SOUTH"),
      "rbuild sector-type OUTDOOR_CEMENT",
		go(psitech.sniper_post_east),
		repeat(2,["n"]),
		mark(psitech.drop_off_start),
    repeat(3, [
      "e",
      title("{blu}Psi-Tech H.Q.{/blu} - Outside - VIP Drop Off"),
      desc(vip_dropoff),
			texture("OUTSIDE DRY STREET PARKING_LOT PARKING_STALL"),
      "rbuild sector-type OUTDOOR_CEMENT",
			/** TODO random spawn mp shotgunners here */
    ]),
      "n",
      title("{blu}Psi-Tech H.Q.{/blu} - Outside - VIP Drop Off"),
      desc(vip_dropoff),
			texture("OUTSIDE DRY STREET PARKING_LOT PARKING_STALL"),
      "rbuild sector-type OUTDOOR_CEMENT",
      "e",
      title("{blu}Psi-Tech H.Q.{/blu} - Outside - VIP Drop Off"),
      desc(vip_dropoff),
			texture("OUTSIDE DRY STREET PARKING_LOT PARKING_STALL"),
      "rbuild sector-type OUTDOOR_CEMENT",
		go(psitech.sniper_post_east),
		"n","n",
		"e",
		mark('psi-tech-car-1'),
		"e",
		mark('psi-tech-car-2'),
		"e",
		mark('psi-tech-car-3'),
		"n",
		mark('psi-tech-car-4'),
		"e",
		mark('psi-tech-car-5'),

		/** Let's start building the indoor section */
		go(psitech.south_door),
    repeat(5, [
      "n",
      title("{blu}Psi-Tech H.Q.{/blu} - Inside - Atrium"),
      desc(atrium),
			texture("INSIDE TILE "),
      "rbuild sector-type INDOOR_METAL_WALLS",
			/** TODO random spawn mp shotgunners here */
    ]),
		go(psitech.south_door),
		"n",
		mark(psitech.atrium),
		repeat(2,["n"]),
		mark(psitech.atrium_2),
		go(psitech.south_door),
		repeat(5,["n"]),
		"s",
    repeat(4, [
      "w",
      title("{blu}Psi-Tech H.Q.{/blu} - Inside - Atrium"),
      desc(atrium),
			texture("INSIDE TILE "),
      "rbuild sector-type INDOOR_METAL_WALLS",
			/** TODO random spawn mp shotgunners here */
    ]),
		go(psitech.south_door),
		repeat(4,["n"]),
		repeat(2,["e"]),
		mark(psitech.hatch),
		texture("INSIDE TILE METAL_HATCH"),
    dirgen(
      "d",
      "A large metal hatch lays beneath you. An unfamiliar contraption keeps it locked."
    ),
    dirkeyword("d", "metal hatch"),
    dirkey("d", 4),
    dirtovnum("d", 128),	/** TODO: change this to the underground tunnel */
    repeat(2, [
      "e",
      title("{blu}Psi-Tech H.Q.{/blu} - Inside - Atrium"),
      desc(atrium),
			texture("INSIDE TILE "),
      "rbuild sector-type INDOOR_METAL_WALLS",
			/** TODO random spawn mp shotgunners here */
    ]),
		'rbuild pave off',
		'rbuild save-paved 0',
  ]);
  var commands = [
    "zbuild yaml this #yaml|vehicle/p3-hunchbak.yml " + exec('print_vnum psi-tech-car-1') + " 1 0",
    "zbuild yaml this #yaml|vehicle/p3-offroad-mx3.yml " + exec('print_vnum psi-tech-car-2') + " 1 0",
    "zbuild yaml this #yaml|vehicle/prime-town-suv.yml " + exec('print_vnum psi-tech-car-3') + " 1 0",
    "zbuild yaml this #yaml|vehicle/lxr-sunrise.yml " + exec('print_vnum psi-tech-car-4') + " 1 0",
    "zbuild yaml this #yaml|vehicle/lxr-sport.yml " + exec('print_vnum psi-tech-car-5') + " 1 0",
  ];
  run(commands);
	// start orthos mob list

  return true;
}
function do_psi_tech_building_demolitions(){
  var fm = "@FILL_ME@";
  var demo_entrance = [
		"The walls actively hum as you enter the room. The metal floor below your feet ",
		"has multiple layers of steel stacked on top of each other. Each layer has a ",
		"pattern of intricately drilled holes. The room is lit from above your head ",
		"using LED lights. The room smells like chemical cleaners and burned plastics. ",
		"A {blu}camera{/blu} watches your every move.",
	].join('');
	var demo_txt = "INSIDE METAL_WALL REINFORCED_WALLS METAL_FLOORS";
	var protected_hallway = [
		"To each side of you are reinforced one way mirrors that can withstand an incredibly ",
		"high amount of ballistic damage. A {blu}gait-recognition camera{/blu} on the west ",
		"wall analyzes each step you take. An infrared and UV {red]camera{/red} on the east ",
		"wall moves as you move.",
	].join('');
  run([
		"recall",
		go(psitech.hatch),
		drive("2e"),
		
		"rbuild pave continue",
		"u",
    title("{blu}Psi-Tech - Demolitions{/blu} - Demolitions entrance"),
    desc(demo_entrance),
		mark(psitech.demo.entrance),
		texture(demo_txt),

		/**
		 * TODO: left off here 
		 */

		'rbuild pave off','rbuild save-paved 0',
  ]);
  var commands = [
    "zbuild yaml this #yaml|vehicle/p3-hunchbak.yml " + exec('print_vnum psi-tech-car-1') + " 1 0",
    "zbuild yaml this #yaml|vehicle/p3-offroad-mx3.yml " + exec('print_vnum psi-tech-car-2') + " 1 0",
    "zbuild yaml this #yaml|vehicle/prime-town-suv.yml " + exec('print_vnum psi-tech-car-3') + " 1 0",
    "zbuild yaml this #yaml|vehicle/lxr-sunrise.yml " + exec('print_vnum psi-tech-car-4') + " 1 0",
    "zbuild yaml this #yaml|vehicle/lxr-sport.yml " + exec('print_vnum psi-tech-car-5') + " 1 0",
  ];
  run(commands);
	// start orthos mob list

  return true;
}
function orthos_guards(){
  var mobs = {
    orthos_guard: {
      mbuild: {
        attr: {
          exp: 8950,
          virt: 503,
          mana: 245,
          max_mana: 245,
          hit: 5850,
          max_hit: 5850,
          move: 110,
          max_move: 110,
          damroll: 30,
          gold: 250,
          level: 20,
          weight: 10,
          height: 5,
          strength: 55,
          strength_add: 23,
          intelligence: 52,
          dexterity: 54,
          constitution: 58,
          electronics: 20,
          chemistry: 20,
          strategy: 20,
          marksmanship: 20,
          sniping: 20,
          weapon_handling: 20,
          demolitions: 20,
          armor: 20,
          medical: 20,
          charisma: 20,
          damnodice: 20,
          damsizedice: 16,
          name: "An Orthos guard",
          sex: "MALE",
          action: "ISNPC HELPER AGGR_GOOD",
          short_description: "An Orthos guard",
          long_description: "An Orthos guard",
          description: "An Orthos guard regards you indifferently.",
        } /** end attr */,
        targets: "PLAYERS",
        "extended-type": "ORTHOS_AGENT",
        "roam-pattern": "Psi-Tech",
      },
      meqbuild: {
        name: "orthos-guard",
        positions: {
          wield: "rifle/orthos-aug-a3.yml",
          hands: "armor/mp-enforcer-gloves.yml",
          body: "armor/vulture-ballistic-vest.yml",
          legs: "armor/titan-shin-guards.yml",
          feet: "armor/xm50-ultralight-boots.yml",
        },
      },
      zbuild: {
        command: "mob",
        zone_id: 0,
        mob_vnum: 503,
        room_vnum: [
					exec('print_vnum ' + psitech.guard_A),
        	exec('print_vnum ' + psitech.guard_B),
				],
        max: 1,
        if_flag: 0,
      },
    },
    orthos_sniper: {
      mbuild: {
        attr: {
          exp: 9050,
          virt: 504,
          mana: 865,
          max_mana: 865,
          hit: 6550,
          max_hit: 6550,
          move: 510,
          max_move: 510,
          damroll: 30,
          gold: 950,
          level: 20,
          weight: 10,
          height: 6,
          strength: 35,
          strength_add: 33,
          intelligence: 32,
          dexterity: 34,
          constitution: 38,
          electronics: 80,
          chemistry: 10,
          strategy: 10,
          marksmanship: 110,
          sniping: 90,
          weapon_handling: 90,
          demolitions: 10,
          armor: 10,
          medical: 10,
          charisma: 10,
          damnodice: 25,
          damsizedice: 28,
          name: "An Orthos sniper",
          sex: "MALE",
          action: "ISNPC HELPER AGGR_GOOD",
          short_description: "An Orthos sniper",
          long_description: "An Orthos sniper",
          description: "An Orthos sniper lies here... waiting..",
        } /** end attr */,
        targets: "PLAYERS",
        "extended-type": "ORTHOS_AGENT",
        "roam-pattern": "Psi-Tech",
      },
      meqbuild: {
        name: "orthos-sniper",
        positions: {
          wield: "rifle/orthos-l96aw.yml",
          hands: "armor/mp-enforcer-gloves.yml",
          body: "armor/vulture-ballistic-vest.yml",
          legs: "armor/titan-shin-guards.yml",
          feet: "armor/xm50-ultralight-boots.yml",
        },
      },
      zbuild: {
        command: "mob",
        zone_id: 0,
        mob_vnum: 504,
        room_vnum: [
					exec('print_vnum ' + psitech.sniper_post_west),
        	exec('print_vnum ' + psitech.sniper_post_east),
				],
        max: 2,
        if_flag: 0,
      },
    },
    orthos_menace: {
      mbuild: {
        attr: {
          exp: 5050,
          virt: 505,
          mana: 1565,
          max_mana: 1565,
          hit: 8050,
          max_hit: 8550,
          move: 910,
          max_move: 910,
          damroll: 340,
          gold: 2150,
          level: 20,
          weight: 10,
          height: 6,
          strength: 55,
          strength_add: 33,
          intelligence: 32,
          dexterity: 34,
          constitution: 38,
          electronics: 80,
          chemistry: 0,
          strategy: 20,
          marksmanship: 40,
          sniping: 30,
          weapon_handling: 40,
          demolitions: 80,
          armor: 50,
          medical: 0,
          charisma: 0,
          damnodice: 115,
          damsizedice: 48,
          name: "An Orthos menace",
          sex: "MALE",
          action: "ISNPC HELPER AGGR_GOOD",
          short_description: "An Orthos menace",
          long_description: "An Orthos menace",
          description: "An Orthos menace",
        } /** end attr */,
        targets: "PLAYERS",
        "extended-type": "ORTHOS_AGENT",
        "roam-pattern": "Psi-Tech",
      },
      meqbuild: {
        name: "orthos-menace",
        positions: {
          /** TODO: deck the halls with OP weapons and armor */
          wield: "rifle/orthos-scarh.yml",
          hands: "armor/mp-enforcer-gloves.yml",
          body: "armor/raven-ballistic-vest.yml",
          legs: "armor/viper-leg-guards.yml",
          feet: "armor/xm50-ultralight-boots.yml",
        },
      },
      zbuild: {
        command: "mob",
        zone_id: 0,
        mob_vnum: 505,
        room_vnum: [
					exec('print_vnum ' + psitech.atrium),
					exec('print_vnum ' + psitech.atrium_2),
					exec('print_vnum ' + psitech.east_gate),
					exec('print_vnum ' + psitech.west_gate),
				],
        max: 1,
        if_flag: 0,
      },
    },
  };
  auto_feed_mob_list(mobs);
}// end orthos mob list

/**
 * ================================================================================
 * ================================================================================
 * ================================================================================
 * ================================================================================
 */

/**
 * ================================================================================
 * ================================================================================
 * || SECTION:
 * ||         S H I P Y A R D
 * ||					C O N T E N T
 * ||
 * ================================================================================
 * ================================================================================
 * v-shipyward v-content
 */
var shipyard = {
  row_a: "shipyard-row-a",
};
function do_shipyard_area() {
  var fm = "@FILL_ME@";

  var crenshaw_highway_overpass = [
    "A newly paved road stretching to the east and curving towards the ",
    "north. Speed limit signs are present but they aren't taken seriously ",
    "by the civilians. Admittedly, the only residents that use the ",
    "highways are the ones with disproportionate amounts of wealth. ",
    "You notice a few carcasses of dead animals that made the dire ",
    "decision to cross this hellish landscape.",
  ].join("");
  var crenshaw_highway_northbound = [
    "The ramp curves to the north. To the east you see a long stretch of highway ",
    "that disappears over the horizon. You see a shipyard far off to the northeast. ",
  ].join("");
  var crenshaw_highway_94_east = [
    "Six lanes of traffic move for off to the east as far as the eye can see. ",
    "You see hills of varying heights hugging the highway's north and south sides. ",
    "There is sparse greenery along the freeway. ",
  ].join("");
  var shipyard_entrance = [
    "Shipping containers stacked 30 to 40 stories high blot out the sun's rays coming from the ",
    "east and even overhead during lunch time. Despite the time of day, artificial lighting is needed ",
    "everywhere. As you make your way deeper into the shipyard, ",
    "you notice several highly armed individuals patrolling the area. These individuals are ",
    "wearing masks and have extensive radio communication devices that are resistant to E.M.P.. ",
  ].join("");
  var shipyard_shipment_row_a = [
    "All around you are red and blue shipping containers with varying amounts of rust and ",
    "outer wear. A few have severe indentations from mishaps. Oddly enough, containers with ",
    "giant abrasions are bent back into shape and reused. You notice what seems to be burn marks, ",
    "bullet holes, and dark splattered brownish red stains on a few containers. ",
  ].join("");

  run([
    repeat(2, ["n"]),
    repeat(2, ["e"]),
    repeat(9, ["n"]),
    repeat(6, ["e"]),
    "n",
    "rbuild pave continue",
    "e",
    title("{blu}Crenshaw{/blu} Highway overpass"),
    desc(crenshaw_highway_overpass),
    texture("OUTSIDE HIGHWAY DRY DESERT AIR"),
    mark(crenshaw.overpass_start),

    repeat(3, [
      "e",
      title("{blu}Crenshaw{/blu} Highway overpass - Ramp"),
      desc(crenshaw_highway_overpass),
      texture("OUTSIDE HIGHWAY DRY DESERT AIR"),
    ]),
    repeat(3, [
      "n",
      title("{blu}Crenshaw{/blu} Highway overpass - North"),
      desc(crenshaw_highway_northbound),
      texture("OUTSIDE HIGHWAY DRY DESERT AIR"),
    ]),
    repeat(3, [
      "e",
      title("{blu}Crenshaw{/blu} Highway 94 East"),
      desc(crenshaw_highway_94_east),
      texture("OUTSIDE HIGHWAY DRY DESERT AIR"),
    ]),
    repeat(30, [
      "e",
      title("{blu}Crenshaw{/blu} Highway 94 East"),
      desc(crenshaw_highway_overpass),
      texture("OUTSIDE HIGHWAY DRY DESERT AIR"),
    ]),
    mark(crenshaw.northern_shipping_area_entrance),
    repeat(3, [
      "n",
      title("{blu}Shipyard{/blu} Entrance"),
      desc(shipyard_entrance),
      texture("OUTSIDE DRY DESERT AIR"),
    ]),
    mark(shipyard.row_a),
    repeat(2, [
      "e",
      title("{blu}Shipyard{/blu} Shipment Row A"),
      desc(shipyard_shipment_row_a),
      texture("OUTSIDE HIGHWAY DRY DESERT AIR"),
    ]),
    repeat(2, [
      "n",
      title("{blu}Shipyard{/blu} Shipment Row A"),
      desc(shipyard_shipment_row_a),
      texture("OUTSIDE HIGHWAY DRY DESERT AIR"),
    ]),
    repeat(2, [
      "e",
      title("{blu}Shipyard{/blu} Shipment Row A"),
      desc(shipyard_shipment_row_a),
      texture("OUTSIDE HIGHWAY DRY DESERT AIR"),
    ]),
    repeat(2, ["w"]),
    repeat(2, ["s"]),
    repeat(2, ["w"]),

    go(shipyard.row_a),

    repeat(2, [
      "w",
      title("{blu}Shipyard{/blu} Shipment Row A"),
      desc(shipyard_shipment_row_a),
      texture("OUTSIDE HIGHWAY DRY DESERT AIR"),
    ]),
    repeat(2, [
      "n",
      title("{blu}Shipyard{/blu} Shipment Row A"),
      desc(shipyard_shipment_row_a),
      texture("OUTSIDE HIGHWAY DRY DESERT AIR"),
    ]),
    repeat(2, [
      "w",
      title("{blu}Shipyard{/blu} Shipment Row A"),
      desc(shipyard_shipment_row_a),
      texture("OUTSIDE HIGHWAY DRY DESERT AIR"),
    ]),
    go(shipyard.row_a),
    go(crenshaw.northern_shipping_area_entrance),
    repeat(30, [
      "e",
      title("{blu}Crenshaw{/blu} Highway 94 East"),
      desc(crenshaw_highway_overpass),
      texture("OUTSIDE HIGHWAY DRY DESERT AIR"),
    ]),
    /** end of eastern section */
  ]);

  run(["rbuild pave off"]);
}
var campo = {
  kenwood_drive_intersection: "kenwood-drive-intersection",
  kenwood_drive_campo_road: "kenwood-drive-campo-road",
  conrad_drive_intersection: "campo-conrad-drive-intersection",
  back_alley_church_entrance: "campo-back-alley-church-entrance",
  saint_vale_nw_parking_lot: "saint-vale-nw-parking-lot",
  vale_west_entrance: "saint-vale-west-entrance",
  vale_rear_east: "saint-vale-rear-east",
  vale_altar: "saint-vale-altar",
};
function do_abbot_market_east() {
  var allied_food_market_alcohol_isle = [
    "Both sides of the isle contain huge amounts of ",
    "name brand alcoholic beverages. You notice that there aren't ",
    "any beers. The only types of alcohol are high strength variants ",
    "like vodka, rum, and whiskey. An {red}explosion{/red} in this ",
    "isle would be {grn}fatal{/grn}.",
  ].join("");
  var allied_food_market_dairy = [
    "The isle is dimly lit but that doesn't affect your vision of the ",
    "contents of this isle. Every product here is in a white container. ",
    "Milks, butters, cream cheese.. all sorts of kinds of almond milks.. ",
    "At the end of the isle you notice an end cap displaying a high grade ",
    "vokda. How appropriate.",
  ].join("");
  var allied_food_market_shower_isle = ["A shower isle"].join("");
  var allied_food_market_pharmacy = ["A Pharmacy"].join("");
  var allied_food_market = [
    "A large green and white sign looms above your head. {grn}Allied Foods{/grn} ",
    "is the main food source for the local community. It's central location is ",
    "within walking distance for most of the residents.",
  ].join("");
  var allied_food_market_slaughter_isle = [
    "The frigid air of a freezer section assaults you into a new state of wakefulness. ",
    "The white tiled floor has turned to an uneven amount of pink and black due to a ",
    "large amount of foot traffic here. Soot and tracked-in dirt color the floor in uneven ",
    "patterns that get heavier the further south you go.",
  ].join("");
  var carts = [
    "Four rows of black shopping carts are off to the left. The automatic door in front of ",
    "you gives you a nice gust of cool air-conditioned air.",
  ].join("");
  var random_item_spawns = [
    "melee/crowbar.yml",
    "melee/improvised-shank.yml",
    "melee/screwdriver.yml",
  ];
  run([
    "recall",
		drive('2n2e9n6e10n'),
    "rbuild pave continue",
    mark("abbot-market-east-divergence"),
    "e",
    title("{grn}Abbot Market East{/grn} - Allied Foods - Entrance"),
    desc(allied_food_market),
    texture("OUTSIDE DRY DESERT AIR"),
		save(),
    "u",
    title("{grn}Abbot Market East{/grn} - Allied Foods - Entrance"),
    desc(allied_food_market),
    texture("OUTSIDE DRY DESERT AIR"),
		save(),
    repeat(4,
			["e",
				title("{grn}Abbot Market East{/grn} - Allied Foods - Entrance"),
				desc(allied_food_market),
				texture("OUTSIDE DRY DESERT AIR"),
			]
		),
    "w",
    automatic_door("e"),
    "open keyword east",
    "e",
    "e",
    title("{grn}Abbot Market East{/grn} - Allied Foods - Carts"),
    desc(carts),
    texture("INSIDE GLASS_WINDOWS"),
		save(),
    place_random_items(1, random_item_spawns),
    "w",
    automatic_door("e"),
    "open keyword east",
    "e",
    repeat(3, [
      "e",
      title("{grn}Abbot Market East{/grn} - Allied Foods - Pharmacy"),
      desc(allied_food_market_pharmacy),
      texture("INSIDE"),
      place_random_items(1, random_item_spawns),
    ]),
    mark("allied-foods-pharmacy-bend"),
    repeat(3, [
      "n",
      title("{grn}Abbot Market East{/grn} - Allied Foods - Pharmacy"),
      desc(allied_food_market_pharmacy),
      texture("INSIDE"),
		save(),
      place_random_items(1, random_item_spawns),
    ]),
    go("allied-foods-pharmacy-bend"),
    repeat(8, [
      "e",
      title("{grn}Abbot Market East{/grn} - Allied Foods - Shower Isle"),
      desc(allied_food_market_shower_isle),
      texture("INSIDE"),
		save(),
      place_random_items(1, random_item_spawns),
    ]),
    mark("allied-foods-shower-end"),
    repeat(3, [
      "s",
      title("{grn}Abbot Market East{/grn} - Allied Foods - Dairy"),
      desc(allied_food_market_dairy),
      texture("INSIDE TILE"),
		save(),
      place_random_items(1, random_item_spawns),
    ]),
    mark("allied-foods-dairy-end"),
    repeat(8, [
      "w",
      title("{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle"),
      desc(allied_food_market_alcohol_isle),
      texture("INSIDE TILE VOLATILE GLASS_CONTENTS"),
		save(),
      place_random_items(1, random_item_spawns),
    ]),
    mark("allied-foods-alcohol-end"),
    repeat(3, [
      "s",
      title("{grn}Abbot Market East{/grn} - Allied Foods - Dairy"),
      desc(allied_food_market_dairy),
      texture("INSIDE TILE"),
		save(),
      place_random_items(1, random_item_spawns),
    ]),
    mark("allied-foods-dairy-end"),
    repeat(8, [
      "e",
      title("{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle 2"),
      desc(allied_food_market_alcohol_isle),
      texture("INSIDE TILE VOLATILE GLASS_CONTENTS"),
		save(),
      place_random_items(1, random_item_spawns),
    ]),
    mark("allied-foods-alcohol-end2"),
    repeat(3, [
      "s",
      title("{grn}Abbot Market East{/grn} - Allied Foods - Dairy"),
      desc(allied_food_market_dairy),
      texture("INSIDE TILE"),
		save(),
      place_random_items(1, random_item_spawns),
    ]),
    mark("allied-foods-dairy-end2"),
    repeat(8, [
      "w",
      title("{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle 3"),
      desc(allied_food_market_alcohol_isle),
      texture("INSIDE TILE VOLATILE GLASS_CONTENTS"),
		save(),
      place_random_items(1, random_item_spawns),
    ]),
    mark("allied-foods-alcohol-end3"),
    repeat(3, [
      "s",
      title("{grn}Abbot Market East{/grn} - Allied Foods - Dairy"),
      desc(allied_food_market_dairy),
      texture("INSIDE TILE"),
		save(),
      place_random_items(1, random_item_spawns),
    ]),
    mark("allied-foods-dairy-end3"),
    repeat(8, [
      "e",
      title("{grn}Abbot Market East{/grn} - Allied Foods - Alcohol Isle 4"),
      desc(allied_food_market_alcohol_isle),
      texture("INSIDE TILE VOLATILE GLASS_CONTENTS"),
		save(),
      place_random_items(1, random_item_spawns),
    ]),
    repeat(3, [
      "s",
      title("{grn}Abbot Market East{/grn} - Allied Foods - Dairy"),
      desc(allied_food_market_dairy),
      texture("INSIDE TILE"),
		save(),
      place_random_items(1, random_item_spawns),
    ]),
    mark("allied-foods-dairy-end4"),

    repeat(17, [
      "s",
      title("{grn}Abbot Market East{/grn} - Allied Foods - Slaughter Isle"),
      desc(allied_food_market_slaughter_isle),
      texture("INSIDE TILE FREEZING"),
		save(),
    ]),

    mark("defiler-isle"),
    repeat(4, [
      "e",
      title("{grn}Abbot Market East{/grn} - Allied Foods - Butcher"),
      desc(allied_food_market_slaughter_isle),
      texture("INSIDE TILE FREEZING"),
		save(),
    ]),
    mark("defiler-corner-1"),
    repeat(4, [
      "s",
      title("{grn}Abbot Market East{/grn} - Allied Foods - Butcher"),
      desc(allied_food_market_slaughter_isle),
      texture("INSIDE TILE FREEZING"),
		save(),
    ]),
    mark("defiler-east-1"),
    repeat(3, [
      "w",
      title("{grn}Abbot Market East{/grn} - Allied Foods - Butcher"),
      desc(allied_food_market_slaughter_isle),
      texture("INSIDE TILE FREEZING"),
		save(),
    ]),
    mark("defiler-corner-2"),
    repeat(3, [
      "n",
      title("{grn}Abbot Market East{/grn} - Allied Foods - Butcher"),
      desc(allied_food_market_slaughter_isle),
      texture("INSIDE TILE FREEZING"),
		save(),
    ]),
    mark("defiler-east-tie"),
    go("defiler-isle"),
    repeat(4, [
      "w",
      title("{grn}Abbot Market East{/grn} - Allied Foods - Butcher"),
      desc(allied_food_market_slaughter_isle),
      texture("INSIDE TILE FREEZING"),
		save(),
    ]),
    mark("defiler-corner-3"),
    repeat(4, [
      "s",
      title("{grn}Abbot Market East{/grn} - Allied Foods - Butcher"),
      desc(allied_food_market_slaughter_isle),
      texture("INSIDE TILE FREEZING"),
		save(),
    ]),
    mark("defiler-west-1"),
    repeat(3, [
      "e",
      title("{grn}Abbot Market East{/grn} - Allied Foods - Butcher"),
      desc(allied_food_market_slaughter_isle),
      texture("INSIDE TILE FREEZING"),
		save(),
    ]),
    mark("defiler-corner-4"),
    repeat(3, [
      "n",
      title("{grn}Abbot Market East{/grn} - Allied Foods - Butcher"),
      desc(allied_food_market_slaughter_isle),
      texture("INSIDE TILE FREEZING"),
		save(),
    ]),
    mark("defiler-corner-5"),
    "e",
    title("{grn}Abbot Market East{/grn} - Allied Foods - Butcher"),
    desc(allied_food_market_slaughter_isle),
    texture("INSIDE TILE FREEZING"),
		save(),
    tie("n", "defiler-isle"),
    tie("e", "defiler-east-tie"),
    mark("defiler-spawn"),
		'rbuild pave off','rbuild save-paved 0',
  ]);
  //generate_rectangle_to_the_west('{grn}Abbot Market East{/grn} - Allied Foods - Butcher',5,3,3)
  //return

  var retail_associate_vnum = 109;
  var shoplifter_vnum = 110;
  var shoplifter_spawn = [
    get_vnum("allied-foods-pharmacy-bend"),
    get_vnum("allied-foods-shower-end"),
    get_vnum("allied-foods-alcohol-end"),
    get_vnum("allied-foods-alcohol-end2"),
    get_vnum("allied-foods-alcohol-end3"),
    get_vnum("allied-foods-dairy-end"),
    get_vnum("allied-foods-dairy-end2"),
    get_vnum("allied-foods-dairy-end3"),
    get_vnum("allied-foods-dairy-end4"),
  ];
  var retail_associate_spawn = shoplifter_spawn;

  var vnums = [retail_associate_vnum];
  clear_mob_zone_commands_for(vnums);
  var mobs = {
    retail_associate: {
      mbuild: {
        attr: {
          exp: 580,
          virt: retail_associate_vnum,
          level: 3,
          mana: 50,
          max_mana: 50,
          hit: 120,
          max_hit: 120,
          move: 60,
          max_move: 60,
          damroll: 2,
          gold: 85,
          weight: 10,
          height: 5,
          strength: 15,
          strength_add: 1,
          intelligence: 2,
          dexterity: 4,
          constitution: 4,
          electronics: 2,
          chemistry: 0,
          strategy: 2,
          marksmanship: 1,
          sniping: 1,
          weapon_handling: 1,
          demolitions: 1,
          armor: 1,
          medical: 1,
          charisma: 4,
          damnodice: 2,
          damsizedice: 5,
          name: "A retail associate",
          sex: "MALE",
          action: "ISNPC HELPER",
          short_description: "A retail associate",
          long_description: "A retail associate",
          description: "A retail associate tirelessly stocks shelves...",
        } /** end attr */,
        "extended-type": "MELEE_COMBATANT",
        "roam-pattern": "Allied Foods",
      },
      meqbuild: {
        name: "retail-associate",
        positions: {
          wield: "melee/retractable-knife.yml",
          body: "armor/allied-foods-shirt.yml",
          legs: "armor/black-jeans.yml",
          feet: "armor/plain-black-shoes.yml",
        },
      },
      zbuild: {
        command: "mob",
        zone_id: 0,
        mob_vnum: retail_associate_vnum,
        room_vnum: retail_associate_spawn,
        max: 16,
        if_flag: 0,
      },
    },
    shoplifter: {
      mbuild: {
        attr: {
          exp: 580,
          virt: shoplifter_vnum,
          level: 6,
          mana: 100,
          max_mana: 100,
          hit: 240,
          max_hit: 240,
          move: 128,
          max_move: 128,
          damroll: 5,
          gold: 150,
          weight: 10,
          height: 5,
          strength: 30,
          strength_add: 1,
          intelligence: 5,
          dexterity: 8,
          constitution: 8,
          electronics: 5,
          chemistry: 0,
          strategy: 5,
          marksmanship: 2,
          sniping: 2,
          weapon_handling: 3,
          demolitions: 3,
          armor: 3,
          medical: 2,
          charisma: 8,
          damnodice: 5,
          damsizedice: 10,
          name: "A shoplifter",
          sex: "MALE",
          action: "ISNPC HELPER AGGR_EVIL",
          short_description: "A shoplifter",
          long_description: "A shoplifter",
          description: "A shoplifter looks around nervously...",
        } /** end attr */,
        "extended-type": "SHOPLIFTER",
        "roam-pattern": "Allied Foods",
      },
      meqbuild: {
        name: "shoplifter",
        positions: {
          wield: "rifle/mp5.yml",
          eyes: "armor/sunner-shades.yml",
          body: "armor/blue-hoodie.yml",
          legs: "armor/dusty-jeans.yml",
          feet: "armor/sloth-martins.yml",
        },
      },
      zbuild: {
        command: "mob",
        zone_id: 0,
        mob_vnum: shoplifter_vnum,
        room_vnum: shoplifter_spawn,
        max: 16,
        if_flag: 0,
      },
    },
  };
  auto_feed_mob_list(mobs);
}
function do_metropolitan_area() {
  var fm = "@FILL_ME@";
  var metro_bornald_road = [
    "The road to the heart of Metro City is a bustling hive of busy upper city ",
    "residents. While the majority of the people who work in Downtown Metro are ",
    "mid to upper class citizens, a large disproportionate chunk of the people ",
    "who gather here are drugged out degenerates. Several homeless families ",
    "make camp right in the middle of the sidewalk: their life for all to judge. ",
  ].join("");

  var metro_fountain = [
    "A tribute to the Henley Corporation, an elaborately crafted fountain was erected in the early stages of ",
    "development of Downtwon Metro City. The water is cleaned several times before it's recycled and spat back ",
    "out. The fountain has a rectangular cement wall that keeps the water inside. There are elaborately chiseled ",
    "patterns at the bottom of the pool and on the inner walls.",
  ].join("");
  var metro_inside_the_fountain = [
    "You are up to your knees in water. All of your equipment is starting to soak. That soggy feeling is creeping ",
    "into your clothes and you feel less agile.",
  ].join("");

  var metro_haven_str_sidewalk = [
    "Along the paved sidewalks of Haven street are all the hallmarks of a busy metropolis. Except from the fact ",
    "that the homeless denizens outnumber the working class citizens. The sidewalk paints a grim picture of the ",
    "grip that drugs have on those who are poverty-striken. A testament to their suffering in the form of ",
    "man made tents and makeshift sleeping bags.",
  ].join("");
  var metro_hartford_bank_entrance = [
    "A delightful awning above your head provides sanctuary from the blazing heat of the sun. Two automatic ",
    "doors to the north welcome you to Hartford bank.",
  ].join("");
  var metro_hartford_bank_atrium = [
    "Hartford bank, like most of the businesses in downtown metro, is owned and operated by the Hexos Cartel. ",
    "The lavishly decorated atrium is a testament to the massive amounts of income that is pumped through ",
    "thousands of fake accounts in a sophisticated money laundering mechanism.",
  ].join("");
  var metro_hartford_bank_atrium_center = [
    "An elaborately carved marble statue depicting a god from other worlds is here. At the base of the statue ",
    "are four pyramids, each for the sigil they represent. The words 'Memento Mori' is carved in large letters ",
    "along the base of the marble block holding everything else up.",
  ].join("");
  var metro_hartford_bank_tellers = [
    "To the east and west are bank tellers at their respective kiosks. The marble floor has a prestine aura ",
    "to it. The distant promise of coffee hangs in the air.",
  ].join("");
  var metro_hartford_bank_teller = [
    "You stand at a teller kiosk. There is a atm card scanner on the desk in front of you ",
    "On the opposite side of the desk is a large monitor that is angled away from you. ",
  ].join("");
  var textures = {
    fountain: "OUTSIDE DAMP",
    inside_fountain: "OUTSIDE DAMP WATER SHALLOW_WATER",
    haven_sidewalk: "OUTSIDE CEMENT SIDEWALK",
    hartford_bank_atrium: "INSIDE CARPET ATRIUM GLASS_WINDOWS WOODEN_WALLS",
    hartford_bank: "INSIDE CARPET ROOFTOP WOODEN_WALLS",
  };
  run([
    "recall",
    repeat(2, ["n"]),
    repeat(2, ["e"]),
    repeat(2, ["n"]),
    repeat(4, ["e"]),
    "d",
    repeat(2, ["e"]),
    "d",
    repeat(2, ["e"]),
    "d",
    repeat(2, ["e"]),
    repeat(5, ["s"]),
    repeat(5, ["w"]),
    repeat(2, ["s"]),
    repeat(2, ["w"]),
    repeat(6, ["s"]),
    "w",
    "rbuild pave continue",

    repeat(3, [
      "w",
      title("{blu}Metro{/blu} Bornald Road"),
      desc(metro_bornald_road),
      texture("OUTSIDE CEMENT DRY TRASHY STREET AIR"),
    ]),
    mark("bornald-entry"),
  ]);
  generate_rectangle_to_the_west("metrofountain", 5, 3, 3);
  run([
    repeat(2, [
      "n",
      title("{blu}Metro{/blu} Henley Fountain"),
      desc(metro_fountain),
      texture(textures.fountain),
    ]),
    repeat(2, [
      "n",
      title("{blu}Metro{/blu} Henley Fountain"),
      desc(metro_fountain),
      texture(textures.fountain),
    ]),
    repeat(2, [
      "w",
      title("{blu}Metro{/blu} Henley Fountain"),
      desc(metro_fountain),
      texture(textures.fountain),
    ]),
    repeat(4, [
      "s",
      title("{blu}Metro{/blu} Henley Fountain"),
      desc(metro_fountain),
      texture(textures.fountain),
    ]),
    repeat(2, [
      "e",
      title("{blu}Metro{/blu} Henley Fountain"),
      desc(metro_fountain),
      texture(textures.fountain),
    ]),
    repeat(2, [
      "n",
      title("{blu}Metro{/blu} Henley Fountain"),
      desc(metro_fountain),
      texture(textures.fountain),
    ]),
    repeat(1, [
      "w",
      title("{blu}Metro{/blu} Inside the Henley Fountain"),
      desc(metro_inside_the_fountain),
      texture(textures.inside_fountain),
    ]),
    repeat(1, [
      "s",
      title("{blu}Metro{/blu} Inside the Henley Fountain"),
      desc(metro_inside_the_fountain),
      texture(textures.inside_fountain),
    ]),
    repeat(2, [
      "n",
      title("{blu}Metro{/blu} Inside the Henley Fountain"),
      desc(metro_inside_the_fountain),
      texture(textures.inside_fountain),
    ]),
    go("metrofountain-starting-point"),
    "w",
    dirgen(
      "d",
      "A large metal hatch lays beneath you. An unfamiliar contraption keeps it locked."
    ),
    dirkeyword("d", "metal hatch"),
    dirkey("d", 3),
    dirtovnum("d", 128),
    "w",
    "n",
    "w",
    repeat(5, [
      "w",
      title("{blu}Metro{/blu} Haven Street sidewalk"),
      desc(metro_haven_str_sidewalk),
      texture(textures.haven_sidewalk),
    ]),
    repeat(4, ["e"]),
    repeat(2, [
      "n",
      title("{blu}Metro{/blu} Hartford Bank entrance"),
      desc(metro_hartford_bank_entrance),
      texture(textures.haven_sidewalk),
    ]),
    repeat(2, [
      "n",
      title("{blu}Hartford Bank{/blu} - Atrium"),
      desc(metro_hartford_bank_atrium),
      texture(textures.hartford_bank_atrium),
    ]),
    repeat(2, [
      "e",
      title("{blu}Hartford Bank{/blu} - Atrium"),
      desc(metro_hartford_bank_atrium),
      texture(textures.hartford_bank_atrium),
    ]),
    repeat(2, ["w"]),
    repeat(2, [
      "w",
      title("{blu}Hartford Bank{/blu} - Atrium"),
      desc(metro_hartford_bank_atrium),
      texture(textures.hartford_bank_atrium),
    ]),
    repeat(2, [
      "n",
      title("{blu}Hartford Bank{/blu} - Atrium"),
      desc(metro_hartford_bank_atrium),
      texture(textures.hartford_bank_atrium),
    ]),
    mark("hartford-atrium-west"),
    repeat(2, ["s"]),
    repeat(2, ["e"]),
    repeat(2, [
      "n",
      title("{blu}Hartford Bank{/blu} - Atrium"),
      desc(metro_hartford_bank_atrium),
      texture(textures.hartford_bank_atrium),
    ]),
    mark("hartford-atrium-center"),
    repeat(2, ["s"]),
    repeat(2, [
      "e",
      title("{blu}Hartford Bank{/blu} - Atrium"),
      desc(metro_hartford_bank_atrium),
      texture(textures.hartford_bank_atrium),
    ]),
    repeat(2, [
      "n",
      title("{blu}Hartford Bank{/blu} - Atrium"),
      desc(metro_hartford_bank_atrium),
      texture(textures.hartford_bank_atrium),
    ]),
    mark("hartford-atrium-east"),
    "w",
    title("{blu}Hartford Bank{/blu} - Atrium"),
    desc(metro_hartford_bank_atrium),
    texture(textures.hartford_bank_atrium),
    tie("w", "hartford-atrium-center"),
    "w",
    "w",
    title("{blu}Hartford Bank{/blu} - Atrium"),
    desc(metro_hartford_bank_atrium),
    texture(textures.hartford_bank_atrium),
    tie("w", "hartford-atrium-west"),
    "e",
    repeat(6, [
      "n",
      title("{blu}Hartford Bank{/blu} - Tellers"),
      desc(metro_hartford_bank_tellers),
      texture(textures.hartford_bank_atrium),
    ]),
    repeat(4, ["s"]),
    "w",
    title("{blu}Hartford Bank{/blu} - Teller A"),
    desc(metro_hartford_bank_teller),
    texture(textures.hartford_bank_atrium),
    mark("hartford-teller-a"),
    "e",
    repeat(2, ["n"]),
    "w",
    title("{blu}Hartford Bank{/blu} - Teller B"),
    desc(metro_hartford_bank_teller),
    texture(textures.hartford_bank_atrium),
    mark("hartford-teller-b"),
    "e",
    "s",
    "e",
    title("{blu}Hartford Bank{/blu} - Teller C"),
    desc(metro_hartford_bank_teller),
    texture(textures.hartford_bank_atrium),
    mark("hartford-teller-c"),
    "w",
    repeat(2, ["n"]),
    "e",
    title("{blu}Hartford Bank{/blu} - Teller D"),
    desc(metro_hartford_bank_teller),
    texture(textures.hartford_bank_atrium),
    mark("hartford-teller-d"),
    "w",
    "n",
  ]);

  run(['rbuild pave off','rbuild save-paved 0']);
  var armed_sec_vnum = 106;
  var armed_sec_spawn = [
    get_vnum("hartford-teller-a"),
    get_vnum("hartford-teller-b"),
    get_vnum("hartford-teller-c"),
    get_vnum("hartford-teller-d"),
  ];
  var bank_teller_vnum = 107;
  var bank_teller_spawn = armed_sec_spawn;
  var vnums = [armed_sec_vnum, bank_teller_vnum];
  clear_mob_zone_commands_for(vnums);
  var mobs = {
    armed_security: {
      mbuild: {
        attr: {
          exp: 85580,
          virt: armed_sec_vnum,
          mana: 245,
          max_mana: 245,
          hit: 950,
          max_hit: 950,
          move: 610,
          max_move: 610,
          damroll: 60,
          gold: 2150,
          level: 20,
          weight: 10,
          height: 5,
          strength: 28,
          strength_add: 23,
          intelligence: 25,
          dexterity: 54,
          constitution: 28,
          electronics: 25,
          chemistry: 10,
          strategy: 25,
          marksmanship: 35,
          sniping: 20,
          weapon_handling: 38,
          demolitions: 30,
          armor: 30,
          medical: 20,
          charisma: 0,
          damnodice: 90,
          damsizedice: 18,
          name: "An armed security guard",
          sex: "MALE",
          action: "ISNPC HELPER AGGR_GOOD",
          short_description: "An armed security guard",
          long_description: "An armed security guard",
          description: "An armed security guard watches you closely.",
        } /** end attr */,
        "extended-type": "ARMED_GUARD",
      },
      meqbuild: {
        name: "armed-guard",
        positions: {
          wield: "rifle/mp5.yml",
          head: "armor/baklava.yml",
          eyes: "armor/sunner-shades.yml",
          hands: "armor/forge-xm3-gloves.yml",
          body: "armor/falcon-ballistic-vest.yml",
          legs: "armor/mp-enforcer-pants.yml",
          feet: "armor/xm607-vulture-boots.yml",
          elbow_l: "armor/titan-elbow-guards.yml",
          elbow_r: "armor/titan-elbow-guards.yml",
          shoulders_l: "armor/titan-shoulder-pads.yml",
          shoulders_r: "armor/titan-shoulder-pads.yml",
        },
      },
      zbuild: {
        command: "mob",
        zone_id: 0,
        mob_vnum: armed_sec_vnum,
        room_vnum: armed_sec_spawn,
        max: 1,
        if_flag: 0,
      },
    },
    bank_teller: {
      mbuild: {
        attr: {
          exp: 580,
          virt: bank_teller_vnum,
          mana: 15,
          max_mana: 15,
          hit: 15,
          max_hit: 15,
          move: 20,
          max_move: 20,
          damroll: 1,
          gold: 450,
          level: 2,
          weight: 4,
          height: 5,
          strength: 1,
          strength_add: 1,
          intelligence: 1,
          dexterity: 3,
          constitution: 0,
          electronics: 0,
          chemistry: 0,
          strategy: 0,
          marksmanship: 0,
          sniping: 0,
          weapon_handling: 1,
          demolitions: 0,
          armor: 0,
          medical: 0,
          charisma: 30,
          damnodice: 1,
          damsizedice: 3,
          name: "A bank teller",
          sex: "FEMALE",
          action: "ISNPC VULNERABLE",
          short_description: "A bank teller",
          long_description: "A bank teller",
          description: "A bank teller is obediently serving you",
        } /** end attr */,
        "extended-type": "SENTINEL_SERVICER",
      },
      meqbuild: {
        name: "teller",
        positions: {
          eyes: "armor/thick-glasses.yml",
          legs: "armor/leggings.yml",
          feet: "armor/dress-shoes.yml",
        },
      },
      zbuild: {
        command: "mob",
        zone_id: 0,
        mob_vnum: bank_teller_vnum,
        room_vnum: bank_teller_spawn,
        max: 1,
        if_flag: 0,
      },
    },
  };
  auto_feed_mob_list(mobs);
}
function do_metro_bank_west_alley() {
  var west_alley = [
    "If you hadn't known there was a bank just to the east of here, you'd think you were in the ",
    "worst time of dystopian nightmare. The ground is littered with used needles, broken glass, and human feces. ",
    "The very thought of sleeping here makes you cringe, yet you see that many homeless people call ",
    "this home by evidence of the makeshift shelters made of cardboard and newspaper fragments.",
  ].join("");
  var west_alley_textures =
    "OUTSIDE CEMENT SIDEWALK SHATTERED_GLASS STREET GHETTO ALLEY FENCED RUBBLE";
  var west_valley_vault = [
    "You find yourself in a heavily fortified bank vault. A scene that few people have the privilege of ",
    "witnessing. Heavy bags of money line the walls. Giant stacks of money are organized into a neat ",
    "pile on several tables in the middle of the room.",
  ].join("");
  var west_vault_textures = "INSIDE CARPET METAL_WALL";
  var west_alley_hidden_wall = "hartford-bank-west-valley-hidden-wall";
  run([
    "recall",
    repeat(2, ["n"]),
    repeat(2, ["e"]),
    repeat(2, ["n"]),
    repeat(4, ["e"]),
    "d",
    repeat(2, ["e"]),
    "d",
    repeat(2, ["e"]),
    "d",
    repeat(2, ["e"]),
    repeat(5, ["s"]),
    repeat(5, ["w"]),
    repeat(2, ["s"]),
    repeat(2, ["w"]),
    repeat(6, ["s"]),
    repeat(6, ["w"]),
    "n",
    repeat(5, ["w"]),
    mark("bank-west-alley-start"),
    "rbuild pave continue",
    repeat(12, [
      "n",
      title("{blu}Hartford Bank West Alley{/blu}"),
      desc(west_alley),
      texture(west_alley_textures),
    ]),
    mark(west_alley_hidden_wall),
    repeat(2, [
      "e",
      title("{blu}Hartford Bank Vault{/blu}"),
      desc(west_valley_vault),
      texture(west_vault_textures),
    ]),
    repeat(2, ["w"]),
    hidden_thermite("e"),
		'rbuild pave off','rbuild save-paved 0',
  ]);
}
function do_level_2_drones() {
  var thompson_avenue = [
    "Thompson Avenue is home to a recent spurt in development that started with the redesign ",
    "and rebranding of the Market Street area. With new leadership and a focus on invigorating ",
    "and supporting the working class, Thompson Avenue transformed from a lightly populated ",
    "industrial town, to a thriving metropolitan suburb. ",
  ].join("");
  var trolley_station = [
    "Thanks to hundreds of thousands of dollars in federal and state funding, the Trolley ",
    "station on Market Street is a reliable source of dependable transportation for the ",
    "surrounding middle class population. At some point, an influx of the viral pathogen ",
    "known collectively as TR-9B caused a variety of changes to the occupants in ways ",
    "that nobody could ever fathom. The stress that the pathogen put on the nation ended ",
    "up causing the working class cititzens to be impacted most.",
  ].join("");
  trolley_station_textures = "OUTSIDE STREET SIDEWALK";
  thompson_textures = "OUTSIDE STREET SIDEWALK";
  run([
    "recall",
    repeat(2, ["n"]),
    repeat(2, ["e"]),
    repeat(2, ["n"]),
    repeat(4, ["e"]),
    "d",
    repeat(2, ["e"]),
    "d",
    repeat(2, ["e"]),
    "d",
    repeat(2, ["e"]),
    repeat(5, ["s"]),
    repeat(5, ["w"]),
    repeat(2, ["s"]),
    repeat(2, ["w"]),
    repeat(6, ["s"]),
    repeat(5, ["w"]),
    repeat(2, ["s"]),
    "rbuild pave continue",
    "s",
    title("{blu}Market Street{/blu} - Thompson Avenue"),
    desc(thompson_avenue),
    texture(thompson_textures),
    mark("thompson-north-1"),
    repeat(4, [
      "s",
      title("{blu}Market Street{/blu} - Thompson Avenue"),
      desc(thompson_avenue),
      texture(thompson_textures),
    ]),
    mark("thompson-north-2"),
    repeat(3, [
      "w",
      title("{blu}Market Street{/blu} - Thompson Avenue"),
      desc(thompson_avenue),
      texture(thompson_textures),
    ]),
    mark("thompson-north-3"),
    repeat(4, [
      "s",
      title("{blu}Market Street{/blu} - Thompson Avenue"),
      desc(thompson_avenue),
      texture(thompson_textures),
    ]),
    mark("thompson-north-4"),
    repeat(3, [
      "w",
      title("{blu}Market Street{/blu} - Thompson Avenue"),
      desc(thompson_avenue),
      texture(thompson_textures),
    ]),
    mark("thompson-north-5"),
    repeat(5, [
      "s",
      title("{blu}Market Street{/blu} - Trolley Station"),
      desc(trolley_station),
      texture(trolley_station_textures),
    ]),
    mark("trolley-section-1"),
    repeat(5, [
      "w",
      title("{blu}Market Street{/blu} - Trolley Station"),
      desc(trolley_station),
      texture(trolley_station_textures),
    ]),
    mark("trolley-section-2"),
    repeat(5, [
      "s",
      title("{blu}Market Street{/blu} - Trolley Station"),
      desc(trolley_station),
      texture(trolley_station_textures),
    ]),
    mark("trolley-section-3"),
    repeat(5, [
      "w",
      title("{blu}Market Street{/blu} - Trolley Station"),
      desc(trolley_station),
      texture(trolley_station_textures),
    ]),
    mark("trolley-section-4"),
		'rbuild pave off','rbuild save-paved 0',
  ]);
}
function do_level_2_drones_east() {
  var thompson_avenue = [
    "Thompson Avenue is home to a recent spurt in development that started with the redesign ",
    "and rebranding of the Market Street area. With new leadership and a focus on invigorating ",
    "and supporting the working class, Thompson Avenue transformed from a lightly populated ",
    "industrial town, to a thriving metropolitan suburb. ",
  ].join("");
  var trolley_station = [
    "Thanks to hundreds of thousands of dollars in federal and state funding, the Trolley ",
    "station on Market Street is a reliable source of dependable transportation for the ",
    "surrounding middle class population. At some point, an influx of the viral pathogen ",
    "known collectively as TR-9B caused a variety of changes to the occupants in ways ",
    "that nobody could ever fathom. The stress that the pathogen put on the nation ended ",
    "up causing the working class cititzens to be impacted most.",
  ].join("");
  trolley_station_textures = "OUTSIDE STREET GHETTO GANG_INFESTED SIDEWALK";
  thompson_textures = "OUTSIDE STREET GHETTO GANG_INFESTED SIDEWALK";
  run([
    "recall",
    repeat(2, ["n"]),
    repeat(2, ["e"]),
    repeat(2, ["n"]),
    repeat(4, ["e"]),
    "d",
    repeat(2, ["e"]),
    "d",
    repeat(2, ["e"]),
    "d",
    repeat(2, ["e"]),
    repeat(5, ["s"]),
    repeat(5, ["w"]),
    repeat(2, ["s"]),
    repeat(2, ["w"]),
    repeat(6, ["s"]),
    repeat(5, ["w"]),
    repeat(7, ["s"]),
    "rbuild pave continue",
    "s",
    title("{blu}Market Street{/blu} - Thompson Avenue - EAST"),
    desc(thompson_avenue),
    texture(thompson_textures),
    mark("thompson-east-1"),
    repeat(3, [
      "s",
      title("{blu}Market Street{/blu} - Thompson Avenue - EAST"),
      desc(thompson_avenue),
      texture(thompson_textures),
    ]),
    mark("thompson-east-2"),
    repeat(3, [
      "s",
      title("{blu}Market Street{/blu} - Thompson Avenue - EAST"),
      desc(thompson_avenue),
      texture(thompson_textures),
    ]),
    mark("thompson-east-3"),
    repeat(3, [
      "s",
      title("{blu}Market Street{/blu} - Thompson Avenue - EAST"),
      desc(thompson_avenue),
      texture(thompson_textures),
    ]),
    mark("thompson-east-4"),
    repeat(3, [
      "s",
      title("{blu}Market Street{/blu} - Thompson Avenue - EAST"),
      desc(thompson_avenue),
      texture(thompson_textures),
    ]),
    mark("thompson-east-5"),
    repeat(3, [
      "s",
      title("{blu}Market Street{/blu} - Thompson Avenue - EAST"),
      desc(thompson_avenue),
      texture(thompson_textures),
    ]),
    mark("thompson-east-6"),
    repeat(3, [
      "s",
      title("{blu}Market Street{/blu} - Thompson Avenue - EAST"),
      desc(thompson_avenue),
      texture(thompson_textures),
    ]),
    mark("thompson-east-7"),
    repeat(3, [
      "s",
      title("{blu}Market Street{/blu} - Thompson Avenue - EAST"),
      desc(thompson_avenue),
      texture(thompson_textures),
    ]),
    mark("thompson-east-8"),
    repeat(3, [
      "s",
      title("{blu}Market Street{/blu} - Thompson Avenue - EAST"),
      desc(thompson_avenue),
      texture(thompson_textures),
    ]),
    mark("thompson-east-9"),

    repeat(3, [
      "s",
      title("{blu}Market Street{/blu} - Thompson Avenue - EAST"),
      desc(thompson_avenue),
      texture(thompson_textures),
    ]),
    mark("thompson-east-10"),

    repeat(3, [
      "s",
      title("{blu}Market Street{/blu} - Thompson Avenue - EAST"),
      desc(thompson_avenue),
      texture(thompson_textures),
    ]),
    mark("thompson-east-11"),
		'rbuild pave off','rbuild save-paved 0',
  ]);
}
function do_vale_church_area_mobs() {
  var fm = "@FILL_ME@";

  run([
    repeat(2, ["n"]),
    repeat(2, ["e"]),
    repeat(9, ["n"]),
    repeat(6, ["e"]),
    "n",
    repeat(4, ["e"]),
    repeat(3, ["n"]),
    repeat(10, ["e"]),
	]);

}

function mark_fountain_area(){
	var start = 1;
	var end = 5;

	run([
		'recall',
		repeat(2,'n'),
		repeat(2,'e'),
		repeat(2,'n'),
		repeat(4,'e'),
		'd',
		repeat(2,'e'),
		'd',
		repeat(2,'e'),
		'd',
		repeat(2,'e'),
		repeat(5,'s'),
		repeat(5,'w'),
		repeat(2,'s'),
		repeat(2,'w'),
		repeat(6,'s'),
		repeat(5,'w'),
		mark('fountain-1'),
		'rbuild save',
		'n',
		mark('fountain-2'),
		'rbuild save',
		'n',
		mark('fountain-3'),
		'rbuild save',
		repeat(3,'s'),
		mark('fountain-4'),
		'rbuild save',
		's',
		mark('fountain-5'),
		'rbuild save',
	]);
}

function goto_fountain_area(){
	var start = 1;
	var end = 5;

	run([
		'recall',
		repeat(2,'n'),
		repeat(2,'e'),
		repeat(2,'n'),
		repeat(4,'e'),
		'd',
		repeat(2,'e'),
		'd',
		repeat(2,'e'),
		'd',
		repeat(2,'e'),
		repeat(5,'s'),
		repeat(5,'w'),
		repeat(2,'s'),
		repeat(2,'w'),
		repeat(6,'s'),
		repeat(5,'w'),
	]);
}
function get_fountain_area_vnums(){
	return [
		get_vnum('fountain-1'),
		get_vnum('fountain-2'),
		get_vnum('fountain-3'),
		get_vnum('fountain-4'),
		get_vnum('fountain-5'),
	];
}
function do_fountain_drone_mobs() {
	mark_fountain_area();
	var infected_drone_spawn = get_fountain_area_vnums();
  var infected_drone_vnum = 114;
  var adapted_drone_vnum = 115;
  var adapted_drone_spawn = infected_drone_spawn;
  var vnums = [infected_drone_vnum, adapted_drone_vnum];
  clear_mob_zone_commands_for(vnums);

  var mobs = {
    infected_drone: {
      mbuild: {
        attr: {
          exp: 100,
          virt: infected_drone_vnum,
          mana: 100,
          max_mana: 100,
          hit: 493,
          max_hit: 493,
          move: 82,
          max_move: 82,
          damroll: 10,
          gold: 95,
          level: 2,
          weight: 10,
          height: 5,
          strength: 2,
          strength_add: 2,
          intelligence: 2,
          dexterity: 4,
          constitution: 2,
          electronics: 2,
          chemistry: 1,
          strategy: 2,
          marksmanship: 3,
          sniping: 2,
          weapon_handling: 3,
          demolitions: 3,
          armor: 3,
          medical: 2,
          charisma: 5,
          damnodice: 1,
          damsizedice: 8,
          name: "An infected drone",
          sex: "MALE",
          action: "ISNPC",
          short_description: "An infected drone",
          long_description: "An infected drone",
          description: "An infected drone lumbers hugrily towards you...",
        } /** end attr */,
        targets: "WEAPONS STREETS WATER",
        "extended-type": "GENERIC_THIEF",
        "roam-pattern": "Henley Fountain",
      },
      meqbuild: {
        name: "fountain-drone-infected-drone-lvl-2",
        positions: {
          body: "armor/leather-trenchcoat.yml",
          //wield: "melee/flimsy-knife.yml",
					wield: "rifle/czp10.yml",
          legs: "armor/black-jeans.yml",
          feet: "armor/atom-fade-shoes.yml",
        },
      },
      zbuild: {
        command: "mob",
        zone_id: 0,
        mob_vnum: infected_drone_vnum,
        room_vnum: infected_drone_spawn,
        max: 10,
        if_flag: 0,
      },
    },
    adapted_drone: {
      mbuild: {
        attr: {
          exp: 100,
          virt: adapted_drone_vnum,
          mana: 100,
          max_mana: 100,
          hit: 350,
          max_hit: 350,
          move: 82,
          max_move: 82,
          damroll: 18,
          gold: 88,
          level: 3,
          weight: 10,
          height: 5,
          strength: 4,
          strength_add: 5,
          intelligence: 4,
          dexterity: 4,
          constitution: 4,
          electronics: 4,
          chemistry: 0,
          strategy: 3,
          marksmanship: 5,
          sniping: 4,
          weapon_handling: 8,
          demolitions: 7,
          armor: 5,
          medical: 4,
          charisma: 5,
          damnodice: 2,
          damsizedice: 12,
          name: "An adapted drone",
          sex: "MALE",
          action: "ISNPC",
          short_description: "An adapted drone ",
          long_description: "An adapted drone ",
          description: "An adapted drone looks for something to infect...",
        } /** end attr */,
        targets: "WEAPONS STREETS WATER FIRE",
        "extended-type": "GENERIC_THIEF",
        "roam-pattern": "Henley Fountain",
      },
      meqbuild: {
        name: "fountain-drone-hustler-lvl-3",
        positions: {
          body: "armor/leather-trenchcoat.yml",
          wield: "rifle/czp10.yml",
          legs: "armor/black-jeans.yml",
          feet: "armor/atom-fade-shoes.yml",
        },
      },
      zbuild: {
        command: "mob",
        zone_id: 0,
        mob_vnum: adapted_drone_vnum,
        room_vnum: adapted_drone_spawn,
        max: 10,
        if_flag: 0,
      },
    },
  };
  auto_feed_mob_list(mobs);
}

function do_drone_mobs() {
  var infected_drone_vnum = 112;
  var adapted_drone_vnum = 113;
  var infected_drone_spawn = [
    get_vnum("thompson-north-1"),
    get_vnum("thompson-north-2"),
    get_vnum("thompson-north-3"),
    get_vnum("thompson-north-4"),
    get_vnum("thompson-north-5"),
    get_vnum("trolley-section-1"),
    get_vnum("trolley-section-2"),
    get_vnum("trolley-section-3"),
    get_vnum("trolley-section-4"),
    get_vnum("thompson-east-1"),
    get_vnum("thompson-east-2"),
    get_vnum("thompson-east-3"),
    get_vnum("thompson-east-4"),
    get_vnum("thompson-east-5"),
    get_vnum("thompson-east-6"),
    get_vnum("thompson-east-7"),
    get_vnum("thompson-east-8"),
    get_vnum("thompson-east-9"),
    get_vnum("thompson-east-10"),
    get_vnum("thompson-east-11"),
  ];
  var adapted_drone_spawn = infected_drone_spawn;
  var vnums = [infected_drone_vnum, adapted_drone_vnum];
  clear_mob_zone_commands_for(vnums);
  var mobs = {
    infected_drone: {
      mbuild: {
        attr: {
          exp: 100,
          virt: infected_drone_vnum,
          mana: 100,
          max_mana: 100,
          hit: 493,
          max_hit: 493,
          move: 82,
          max_move: 82,
          damroll: 10,
          gold: 95,
          level: 2,
          weight: 10,
          height: 5,
          strength: 2,
          strength_add: 2,
          intelligence: 2,
          dexterity: 4,
          constitution: 2,
          electronics: 2,
          chemistry: 1,
          strategy: 2,
          marksmanship: 3,
          sniping: 2,
          weapon_handling: 3,
          demolitions: 3,
          armor: 3,
          medical: 2,
          charisma: 5,
          damnodice: 1,
          damsizedice: 8,
          name: "An infected drone",
          sex: "MALE",
          action: "ISNPC",
          short_description: "An infected drone",
          long_description: "An infected drone",
          description: "An infected drone lumbers hugrily towards you...",
        } /** end attr */,
        targets: "WEAPONS STREETS WATER",
        "extended-type": "GENERIC_THIEF",
        "roam-pattern": "Market Street",
      },
      meqbuild: {
        name: "infected-drone-lvl-2",
        positions: {
          body: "armor/leather-trenchcoat.yml",
          //wield: "melee/flimsy-knife.yml",
					wield: "rifle/czp10.yml",
          legs: "armor/black-jeans.yml",
          feet: "armor/atom-fade-shoes.yml",
        },
      },
      zbuild: {
        command: "mob",
        zone_id: 0,
        mob_vnum: infected_drone_vnum,
        room_vnum: infected_drone_spawn,
        max: 5,
        if_flag: 0,
      },
    },
    adapted_drone: {
      mbuild: {
        attr: {
          exp: 100,
          virt: adapted_drone_vnum,
          mana: 100,
          max_mana: 100,
          hit: 350,
          max_hit: 350,
          move: 82,
          max_move: 82,
          damroll: 18,
          gold: 88,
          level: 3,
          weight: 10,
          height: 5,
          strength: 4,
          strength_add: 5,
          intelligence: 4,
          dexterity: 4,
          constitution: 4,
          electronics: 4,
          chemistry: 0,
          strategy: 3,
          marksmanship: 5,
          sniping: 4,
          weapon_handling: 8,
          demolitions: 7,
          armor: 5,
          medical: 4,
          charisma: 5,
          damnodice: 2,
          damsizedice: 12,
          name: "An adapted drone",
          sex: "MALE",
          action: "ISNPC",
          short_description: "An adapted drone ",
          long_description: "An adapted drone ",
          description: "An adapted drone looks for something to infect...",
        } /** end attr */,
        targets: "WEAPONS STREETS WATER FIRE",
        "extended-type": "GENERIC_THIEF",
        "roam-pattern": "Market Street",
      },
      meqbuild: {
        name: "hustler-lvl-3",
        positions: {
          body: "armor/leather-trenchcoat.yml",
          wield: "rifle/czp10.yml",
          legs: "armor/black-jeans.yml",
          feet: "armor/atom-fade-shoes.yml",
        },
      },
      zbuild: {
        command: "mob",
        zone_id: 0,
        mob_vnum: adapted_drone_vnum,
        room_vnum: adapted_drone_spawn,
        max: 5,
        if_flag: 0,
      },
    },
  };
  auto_feed_mob_list(mobs);
}
function do_vale_church_area() {
  var fm = "@FILL_ME@";

  var campo_road_exit = "Exit 10B to Campo Road.";
  var campo_road_exit_a = [
    "A bright orange sign on the right hand shoulder says {yel}END ROAD WORK{/yel}. You see a ",
    "diamond shaped yellow sign with a black arrow pointing upwards and a {red}red{/red} octagon.",
    "The road slopes downwards and on the road you see 'STOP AHEAD' written in white painted on the ",
    "ground. A narrow shoulder on your right is guarded by a grey metal and wooden railing.",
  ].join("");
  var kenwood_dr_inter = [
    "You can take a left or right and you would be on Kenwood Drive. Go straight ahead and you can get back ",
    "on Highway 94 East.",
  ].join("");
  var campo_road = [
    "Campo Road is a bland introduction to the hot and boring town of Spruce Valley. ",
    "The roads are decorated with a Mexican food restaurant every block or so. A few ",
    "signs advertise the presence of the {red}Saint Vale Church{/red}.",
  ].join("");
  var conrad_drive = [
    "Nearly every building has the same Aztec style rooftops. The same construction company ",
    "that worked on every building on Campo Road also worked on the apartment buildings here. ",
    "That same company was shutdown due to activities related to money laundering. As low income ",
    "as this neighborhood is, there are no signs of an obvious wealth inequality here. The residents ",
    "here are like every other community of hard working parents trying to provide for their family.",
  ].join("");

  var vale_back_alley_entrance = [
    "An extremely narrow alleyway leads to the Vale Church parking lot. A wall made of concrete and ",
    "topped with a high iron fence is to the North. Directly to the south are cars jam packed next to ",
    "each other in a feeble attempt to add more parking spaces to the apartments directly south of here. ",
  ].join("");

  var vale_parking_lot = [
    "A huge parking lot for a giant congregation. There are administration and community buildings to ",
    "the north where people often hold weddings and other communal gatherings. To the south you spot a ",
    "building where the staff go when services are over. It seems to be heavily fortified, which seems ",
    "odd, given that this is a place of worship. The Church itself is to the East and it's main bell ",
    "tower casts a looming shadow over the rest of the building and parking lot.",
  ].join("");
  var kenwood_dr_north = fm;
  var vale_west_inside = [
    "Sanctuary. A well ventilated and air conditioned cathedral. The west entrance of Saint Vale ",
    "Church lets a little bit of outside light leak inside as the door closes automatically behind you. ",
    "The smell of incense and blown out candles is hard to ignore, yet it's a very pleasant and calming ",
    "feeling. Each of the fifty rows of seating is 100 feet across. Whoever built this Church had significant ",
    "amounts of money, possibly even ludicrous amounts. ",
  ].join("");
  var vale_bathroom = [
    "The walls are painted a soothing orange and white color and the ceiling fan generates a constant ",
    "white noise hum that lulls you into a meditative state.",
  ].join("");
  var vale_statues = [
    "A statue of a woman holding her hands out while maintaining a look of pain and persecution on her ",
    "face. The pain and misery of her plight beg you to deposit money into the metal donation box. ",
  ].join("");
  var vale_rear_hall = [
    "The wall to the south is made of a repurposed sort of brick layering that makes it almost look ",
    "as if it was a design decision that came long after the architecture of the church was already ",
    "decided. Something seems very off about this wall and you almost feel like you are being watched ",
    "here. A door leading to the bathroom hallway is to the south of here. You can take a seat in ",
    "one of the many rows of seating to the north of here.",
  ].join("");
  var vale_bathroom_hall = [
    "Oddly enough, this hallway is not air conditioned. The male bathroom is just to the west and the ",
    "female bathroom is to the east.",
  ].join("");

  var vale_center_isle = [
    "An elaborate purple and yellow pattern decorates the isle which leads to the altar. ",
    "The corners of each isle have divine sigils engraved into the dark stain of the wood.",
  ].join("");
  var vale_isle = [
    "A dark colored wooden bench is here. There is a retractable padded kneeling device below ",
    "the hanging bookshelf of psalms and prayers. ",
  ].join("");
  var vale_altar = [
    "A table is here with a decorative cloth draped over it. On each side of the table are ",
    "tall stands that hold long white candles. There is a stand with a microphone embedded ",
    "into the wood.",
  ].join("");

  run([
		'recall',
    repeat(2, ["n"]),
    repeat(2, ["e"]),
    repeat(9, ["n"]),
    repeat(6, ["e"]),
    repeat(16, ["n"]),
    "rbuild pave continue",

    repeat(6, [
      "e",
      title("{blu}La Mesa{/blu} Campo Road Exit"),
      desc(campo_road_exit),
      texture("OUTSIDE ONRAMP HIGHWAY DRY DESERT AIR"),
    ]),
    repeat(2, [
      "s",
      title("{blu}La Mesa{/blu} Campo Road Exit"),
      desc(campo_road_exit),
      texture("OUTSIDE ONRAMP HIGHWAY DRY DESERT AIR"),
    ]),
    repeat(1, [
      "d",
      title("{blu}La Mesa{/blu} Campo Road Exit"),
      desc(campo_road_exit),
      texture("OUTSIDE ONRAMP HIGHWAY DRY DESERT AIR"),
    ]),
    repeat(1, [
      "e",
      title("{blu}La Mesa{/blu} Campo Road Exit"),
      desc(campo_road_exit),
      texture("OUTSIDE ONRAMP HIGHWAY DRY DESERT AIR"),
    ]),

    repeat(6, [
      "e",
      title("{blu}La Mesa{/blu} Exit 10B On Ramp"),
      desc(campo_road_exit_a),
      texture("OUTSIDE ONRAMP HIGHWAY DRY DESERT AIR"),
    ]),

    "e",
    title("{blu}La Mesa{/blu} Kenwood Drive Intersection"),
    desc(kenwood_dr_inter),
    texture("OUTSIDE ONRAMP DRY DESERT AIR") /** TODO: add my texture */,
    mark(campo.kenwood_drive_intersection),

    repeat(8, [
      "n",
      title("{blu}La Mesa{/blu} Kenwood Drive North"),
      desc(kenwood_dr_north),
      texture("OUTSIDE DRY DESERT AIR"),
    ]),
    mark(campo.kenwood_drive_campo_road),

    repeat(4, [
      "e",
      title("{blu}La Mesa{/blu} Campo Road 9600"),
      desc(campo_road),
      texture(
        "OUTSIDE GHETTO GANG_INFESTED DRY DESERT AIR"
      ) /** TODO: add my texture */,
    ]),
    mark(campo.conrad_drive_intersection),

    repeat(25, [
      "n",
      title("{blu}La Mesa{/blu} Conrad Road"),
      desc(conrad_drive),
      texture("OUTSIDE GHETTO GANG_INFESTED DRY DESERT AIR"),
    ]),
    mark(campo.back_alley_church_entrance),

    repeat(3, [
      "e",
      title("{blu}La Mesa{/blu} Vale church back alley entrance"),
      desc(vale_back_alley_entrance),
      texture(
        "OUTSIDE NARROW_EAST_WEST ALLEY GHETTO GANG_INFESTED DRY DESERT AIR"
      ),
    ]),

    mark(campo.saint_vale_nw_parking_lot),

    repeat(12, [
      "e",
      title("{blu}Saint Vale Church{/blu} Parking Lot"),
      desc(vale_parking_lot),
      texture("OUTSIDE PARKING_LOT GHETTO GANG_INFESTED DRY DESERT AIR"),
    ]),
    repeat(8, ["w"]),
    repeat(4, [
      "s",
      title("{blu}Saint Vale Church{/blu} Parking Lot - Row A"),
      desc(vale_parking_lot),
      texture("OUTSIDE PARKING_STALL GHETTO GANG_INFESTED DRY DESERT AIR"),
    ]),
    repeat(4, ["n"]),
    repeat(2, ["e"]),
    repeat(4, [
      "s",
      title("{blu}Saint Vale Church{/blu} Parking Lot - Row B"),
      desc(vale_parking_lot),
      texture("OUTSIDE PARKING_STALL GHETTO GANG_INFESTED DRY DESERT AIR"),
    ]),
    repeat(4, ["n"]),
    repeat(2, ["e"]),
    repeat(4, [
      "s",
      title("{blu}Saint Vale Church{/blu} Parking Lot - Row B"),
      desc(vale_parking_lot),
      texture("OUTSIDE PARKING_STALL GHETTO GANG_INFESTED DRY DESERT AIR"),
    ]),
    repeat(4, ["n"]),
    repeat(2, ["e"]),
    repeat(4, [
      "s",
      title("{blu}Saint Vale Church{/blu} Parking Lot - Row C"),
      desc(vale_parking_lot),
      texture("OUTSIDE PARKING_STALL GHETTO GANG_INFESTED DRY DESERT AIR"),
    ]),
    repeat(4, ["n"]),
    repeat(2, ["e"]),
    mark(campo.vale_west_entrance),
    repeat(6, [
      "s",
      title("{blu}Saint Vale Church{/blu} Inside"),
      desc(vale_west_inside),
      texture("INSIDE DRY DESERT CARPET GLASS_WINDOWS ROOFTOP WOODEN_WALLS"),
    ]),
    repeat(4, [
      "e",
      title("{blu}Saint Vale Church{/blu} Inside - Rear Hall"),
      desc(vale_rear_hall),
      texture("INSIDE DRY DESERT CARPET GLASS_WINDOWS ROOFTOP WOODEN_WALLS"),
    ]),
    mark(campo.vale_rear_east),
    repeat(2, ["w"]),
    repeat(2, [
      "s",
      title("{blu}Saint Vale Church{/blu} Inside - Bathroom Hall"),
      desc(vale_bathroom_hall),
      texture("INSIDE DRY DESERT CARPET ROOFTOP WOODEN_WALLS"),
    ]),
    repeat(2, [
      "e",
      title("{blu}Saint Vale Church{/blu} Inside - Male Bathroom"),
      desc(vale_bathroom),
      texture("INSIDE DRY DESERT ROOFTOP WOODEN_WALLS"),
    ]),
    repeat(2, ["w"]),
    repeat(2, [
      "w",
      title("{blu}Saint Vale Church{/blu} Inside - Female Bathroom"),
      desc(vale_bathroom),
      texture("INSIDE DRY DESERT ROOFTOP WOODEN_WALLS"),
    ]),
    go(campo.vale_rear_east),

    repeat(4, [
      "e",
      title("{blu}Saint Vale Church{/blu} Inside - Rear Hall"),
      desc(vale_rear_hall),
      texture("INSIDE DRY DESERT CARPET GLASS_WINDOWS ROOFTOP WOODEN_WALLS"),
    ]),

    go(campo.vale_rear_east),
    repeat(2, ["e"]),
    repeat(9, [
      "n",
      title("{blu}Saint Vale Church{/blu} Inside - Center Isle"),
      desc(vale_center_isle),
      texture("INSIDE DRY DESERT CARPET ROOFTOP WOODEN_WALLS"),
    ]),
    mark(campo.vale_altar),

    repeat(6, ["s"]),

    repeat(2, [
      "w",
      title("{blu}Saint Vale Church{/blu} Inside - Isle"),
      desc(vale_isle),
      texture("INSIDE DRY DESERT CARPET ROOFTOP WOODEN_WALLS"),
    ]),
    repeat(2, ["e"]),
    repeat(2, [
      "e",
      title("{blu}Saint Vale Church{/blu} Inside - Isle"),
      desc(vale_isle),
      texture("INSIDE DRY DESERT CARPET ROOFTOP WOODEN_WALLS"),
    ]),
    repeat(2, ["w"]),

    repeat(2, ["n"]),
    repeat(2, [
      "w",
      title("{blu}Saint Vale Church{/blu} Inside - Isle"),
      desc(vale_isle),
      texture("INSIDE DRY DESERT CARPET ROOFTOP WOODEN_WALLS"),
    ]),
    repeat(2, ["e"]),
    repeat(2, [
      "e",
      title("{blu}Saint Vale Church{/blu} Inside - Isle"),
      desc(vale_isle),
      texture("INSIDE DRY DESERT CARPET ROOFTOP WOODEN_WALLS"),
    ]),
    repeat(2, ["w"]),

    repeat(2, ["n"]),
    repeat(2, [
      "w",
      title("{blu}Saint Vale Church{/blu} Inside - Isle"),
      desc(vale_isle),
      texture("INSIDE DRY DESERT CARPET ROOFTOP WOODEN_WALLS"),
    ]),
    repeat(2, ["e"]),
    repeat(2, [
      "e",
      title("{blu}Saint Vale Church{/blu} Inside - Isle"),
      desc(vale_isle),
      texture("INSIDE DRY DESERT CARPET ROOFTOP WOODEN_WALLS"),
    ]),
    repeat(2, ["w"]),

    go(campo.vale_altar),

    /** altar */
    repeat(2, [
      "e",
      title("{blu}Saint Vale Church{/blu} Inside - Altar"),
      desc(vale_altar),
      texture("INSIDE DRY DESERT CARPET ROOFTOP WOODEN_WALLS"),
    ]),
    repeat(2, ["w"]),
    repeat(1, [
      "w",
      title("{blu}Saint Vale Church{/blu} Inside - Altar"),
      desc(vale_altar),
      texture("INSIDE DRY DESERT CARPET ROOFTOP WOODEN_WALLS"),
    ]),
    repeat(1, [
      "n",
      title("{blu}Saint Vale Church{/blu} Inside - Altar"),
      desc(vale_altar),
      texture("INSIDE DRY DESERT CARPET ROOFTOP WOODEN_WALLS"),
    ]),
    repeat(1, [
      "w",
      title("{blu}Saint Vale Church{/blu} Inside - Altar"),
      desc(vale_altar),
      texture("INSIDE DRY DESERT CARPET ROOFTOP WOODEN_WALLS"),
    ]),
    "e",
    "s",
    "e",
    repeat(2, ["e"]),
    repeat(1, [
      "n",
      title("{blu}Saint Vale Church{/blu} Inside - Altar"),
      desc(vale_altar),
      texture("INSIDE DRY DESERT CARPET ROOFTOP WOODEN_WALLS"),
    ]),
    repeat(1, [
      "e",
      title("{blu}Saint Vale Church{/blu} Inside - Altar"),
      desc(vale_altar),
      texture("INSIDE DRY DESERT CARPET ROOFTOP WOODEN_WALLS"),
    ]),
    repeat(1, [
      "n",
      title("{blu}Saint Vale Church{/blu} Inside - Altar"),
      desc(vale_altar),
      texture("INSIDE DRY DESERT CARPET ROOFTOP WOODEN_WALLS"),
    ]),
  ]);

  run(["rbuild pave off"]);
}
var vertical_slice = {
  mf13_hoodlum: function (index) {
    /** TODO grunt work: convert to auto_feed_mob format */
    var equip = "rifle/mac-10.yml{magazine:container/hv-mp-ammo.yml}";
    var mf13h_vnum = "4000";
    cmd(["mbuild attr ", index, " virt ", mf13h_vnum].join(""));
    cmd(["mbuild attr ", index, " mana ", 25].join(""));
    cmd(["mbuild attr ", index, " max_mana ", 25].join(""));
    cmd(["mbuild attr ", index, " hit ", 580].join(""));
    cmd(["mbuild attr ", index, " max_hit ", 580].join(""));
    cmd(["mbuild attr ", index, " move ", 140].join(""));
    cmd(["mbuild attr ", index, " max_move ", 240].join(""));
    cmd(["mbuild attr ", index, " damroll ", 15].join(""));
    cmd(["mbuild attr ", index, " gold ", 5000].join(""));
    cmd(["mbuild attr ", index, " level ", 10].join(""));
    cmd(["mbuild attr ", index, " weight ", 10].join(""));
    cmd(["mbuild attr ", index, " height ", 4].join(""));
    cmd(["mbuild attr ", index, " strength ", 15].join(""));
    cmd(["mbuild attr ", index, " strength_add ", 13].join(""));
    cmd(["mbuild attr ", index, " intelligence ", 12].join(""));
    cmd(["mbuild attr ", index, " dexterity ", 14].join(""));
    cmd(["mbuild attr ", index, " constitution ", 18].join(""));
    cmd(["mbuild attr ", index, " charisma ", 1].join(""));
    cmd(["mbuild attr ", index, " damnodice ", 14].join(""));
    cmd(["mbuild attr ", index, " damsizedice ", 16].join(""));
    cmd(
      [
        "mbuild attr ",
        index,
        ' name "mf13 mf-13 hoodlum hood thug gangster" ',
      ].join("")
    );
    cmd(["mbuild attr ", index, " sex MALE "].join(""));
    cmd(["mbuild attr ", index, " action AGGRESSIVE ISNPC"].join(""));
    var short_desc = ["{gld}MF-13 Hoodlum{/gld}"].join("");
    cmd(
      ["mbuild attr ", index, ' short_description "', short_desc, '" '].join("")
    );
    var long_desc = ["{gld}MF-13 Hoodlum{/gld}"].join("");
    var description = [
      "Blue jeans, white tank top, and enough tattoos to intimidate any sane ",
      "citizen. The man standing before you is perpetually angry at anyone ",
      "who even shows the slightest bit of eye contact.",
    ].join("");
    cmd(
      ["mbuild attr ", index, ' long_description "', long_desc, '" '].join("")
    );
    cmd(["mbuild attr ", index, ' description "', description, '" '].join(""));
    cmd(["mbuild save ", index].join(""));
    return 0;
  },
  mf13_hoodlum_initiate: function (index) {
    /** TODO grunt work: convert to auto_feed_mob format */
    var equip = "melee/sig-sauer.yml";
    var mf13h_vnum = "4001";
    cmd(["mbuild attr ", index, " virt ", mf13h_vnum].join(""));
    cmd(["mbuild attr ", index, " mana ", 15].join(""));
    cmd(["mbuild attr ", index, " max_mana ", 15].join(""));
    cmd(["mbuild attr ", index, " hit ", 280].join(""));
    cmd(["mbuild attr ", index, " max_hit ", 280].join(""));
    cmd(["mbuild attr ", index, " move ", 40].join(""));
    cmd(["mbuild attr ", index, " max_move ", 40].join(""));
    cmd(["mbuild attr ", index, " damroll ", 5].join(""));
    cmd(["mbuild attr ", index, " gold ", 5000].join(""));
    cmd(["mbuild attr ", index, " level ", 10].join(""));
    cmd(["mbuild attr ", index, " weight ", 10].join(""));
    cmd(["mbuild attr ", index, " height ", 4].join(""));
    cmd(["mbuild attr ", index, " strength ", 5].join(""));
    cmd(["mbuild attr ", index, " strength_add ", 3].join(""));
    cmd(["mbuild attr ", index, " intelligence ", 2].join(""));
    cmd(["mbuild attr ", index, " dexterity ", 4].join(""));
    cmd(["mbuild attr ", index, " constitution ", 8].join(""));
    cmd(["mbuild attr ", index, " charisma ", 1].join(""));
    cmd(["mbuild attr ", index, " damnodice ", 4].join(""));
    cmd(["mbuild attr ", index, " damsizedice ", 6].join(""));
    cmd(["mbuild attr ", index, ' name "initiate hooded" '].join(""));
    cmd(["mbuild attr ", index, " sex MALE "].join(""));
    cmd(["mbuild attr ", index, " action AGGRESSIVE ISNPC"].join(""));
    var short_desc = ["{gld}MF-13 Hooded Initiate{/gld}"].join("");
    cmd(
      ["mbuild attr ", index, ' short_description "', short_desc, '" '].join("")
    );
    var long_desc = ["{gld}MF-13 Hooded Initiate{/gld}"].join("");
    var description = [
      "The danger of the individual you are viewing is amplified in the context ",
      "that this thug needs to impress the likes of the higher ranked thugs. ",
      "The man standing before is smoking PCP.",
    ].join("");
    cmd(
      ["mbuild attr ", index, ' long_description "', long_desc, '" '].join("")
    );
    cmd(["mbuild attr ", index, ' description "', description, '" '].join(""));
    cmd(["mbuild save ", index].join(""));
    return 0;
  },
  harmless_citizen: function (index) {
    /** TODO grunt work: convert to auto_feed_mob format */
    var harmless_vnum = "4002";
    cmd(["mbuild attr ", index, " virt ", harmless_vnum].join(""));
    cmd(["mbuild attr ", index, " mana ", 5].join(""));
    cmd(["mbuild attr ", index, " max_mana ", 5].join(""));
    cmd(["mbuild attr ", index, " hit ", 40].join(""));
    cmd(["mbuild attr ", index, " max_hit ", 40].join(""));
    cmd(["mbuild attr ", index, " move ", 10].join(""));
    cmd(["mbuild attr ", index, " max_move ", 10].join(""));
    cmd(["mbuild attr ", index, " damroll ", 2].join(""));
    cmd(["mbuild attr ", index, " gold ", 10].join(""));
    cmd(["mbuild attr ", index, " level ", 3].join(""));
    cmd(["mbuild attr ", index, " weight ", 10].join(""));
    cmd(["mbuild attr ", index, " height ", 4].join(""));
    cmd(["mbuild attr ", index, " strength ", 2].join(""));
    cmd(["mbuild attr ", index, " strength_add ", 1].join(""));
    cmd(["mbuild attr ", index, " intelligence ", 2].join(""));
    cmd(["mbuild attr ", index, " dexterity ", 1].join(""));
    cmd(["mbuild attr ", index, " constitution ", 2].join(""));
    cmd(["mbuild attr ", index, " charisma ", 1].join(""));
    cmd(["mbuild attr ", index, " damnodice ", 1].join(""));
    cmd(["mbuild attr ", index, " damsizedice ", 1].join(""));
    cmd(["mbuild attr ", index, ' name "harmless citizen" '].join(""));
    cmd(["mbuild attr ", index, " sex FEMALE "].join(""));
    cmd(["mbuild attr ", index, " action WIMPY ISNPC"].join(""));
    var short_desc = ["Harmless citizen"].join("");
    cmd(
      ["mbuild attr ", index, ' short_description "', short_desc, '" '].join("")
    );
    var long_desc = ["Harmless citizen"].join("");
    var description = [
      "An old and decrepit citizen is here trying to mind her own business...",
    ].join("");
    cmd(
      ["mbuild attr ", index, ' long_description "', long_desc, '" '].join("")
    );
    cmd(["mbuild attr ", index, ' description "', description, '" '].join(""));
    cmd(["mbuild save ", index].join(""));
    return 0;
  },
  la_posta_chef: function (index) {
    var la_posta_chef_vnum = "4003";
    /** TODO grunt work: convert to auto_feed_mob format */
    cmd(["mbuild attr ", index, " virt ", la_posta_chef_vnum].join(""));
    cmd(["mbuild attr ", index, " mana ", 8].join(""));
    cmd(["mbuild attr ", index, " max_mana ", 8].join(""));
    cmd(["mbuild attr ", index, " hit ", 110].join(""));
    cmd(["mbuild attr ", index, " max_hit ", 110].join(""));
    cmd(["mbuild attr ", index, " move ", 15].join(""));
    cmd(["mbuild attr ", index, " max_move ", 15].join(""));
    cmd(["mbuild attr ", index, " damroll ", 4].join(""));
    cmd(["mbuild attr ", index, " gold ", 20].join(""));
    cmd(["mbuild attr ", index, " level ", 5].join(""));
    cmd(["mbuild attr ", index, " weight ", 10].join(""));
    cmd(["mbuild attr ", index, " height ", 4].join(""));
    cmd(["mbuild attr ", index, " strength ", 4].join(""));
    cmd(["mbuild attr ", index, " strength_add ", 9].join(""));
    cmd(["mbuild attr ", index, " intelligence ", 9].join(""));
    cmd(["mbuild attr ", index, " dexterity ", 9].join(""));
    cmd(["mbuild attr ", index, " constitution ", 9].join(""));
    cmd(["mbuild attr ", index, " charisma ", 1].join(""));
    cmd(["mbuild attr ", index, " damnodice ", 2].join(""));
    cmd(["mbuild attr ", index, " damsizedice ", 5].join(""));
    cmd(["mbuild attr ", index, ' name "la posta chef" '].join(""));
    cmd(["mbuild attr ", index, " sex MALE "].join(""));
    cmd(
      ["mbuild attr ", index, " action STAY_ZONE HELPER AGGR_GOOD ISNPC"].join(
        ""
      )
    );
    var short_desc = ["La Posta Chef"].join("");
    cmd(
      ["mbuild attr ", index, ' short_description "', short_desc, '" '].join("")
    );
    var long_desc = ["La Posta Chef"].join("");
    var description = [
      "A middle-aged man is standing here preparing food using a dull knife. ",
      "He just wants to finish his shift and get home to his wife and children.",
    ].join("");
    cmd(
      ["mbuild attr ", index, ' long_description "', long_desc, '" '].join("")
    );
    cmd(["mbuild attr ", index, ' description "', description, '" '].join(""));
    cmd(["mbuild save ", index].join(""));
    return 0;
  },
  toughened_teenager: function (index) {
    /** TODO grunt work: convert to auto_feed_mob format */
    var toughened_teen_vnum = "4004";
    cmd(["mbuild attr ", index, " virt ", toughened_teen_vnum].join(""));
    cmd(["mbuild attr ", index, " mana ", 25].join(""));
    cmd(["mbuild attr ", index, " max_mana ", 25].join(""));
    cmd(["mbuild attr ", index, " hit ", 280].join(""));
    cmd(["mbuild attr ", index, " max_hit ", 280].join(""));
    cmd(["mbuild attr ", index, " move ", 100].join(""));
    cmd(["mbuild attr ", index, " max_move ", 100].join(""));
    cmd(["mbuild attr ", index, " damroll ", 10].join(""));
    cmd(["mbuild attr ", index, " gold ", 50].join(""));
    cmd(["mbuild attr ", index, " level ", 6].join(""));
    cmd(["mbuild attr ", index, " weight ", 10].join(""));
    cmd(["mbuild attr ", index, " height ", 4].join(""));
    cmd(["mbuild attr ", index, " strength ", 10].join(""));
    cmd(["mbuild attr ", index, " strength_add ", 10].join(""));
    cmd(["mbuild attr ", index, " intelligence ", 27].join(""));
    cmd(["mbuild attr ", index, " dexterity ", 24].join(""));
    cmd(["mbuild attr ", index, " constitution ", 20].join(""));
    cmd(["mbuild attr ", index, " charisma ", 10].join(""));
    cmd(["mbuild attr ", index, " damnodice ", 10].join(""));
    cmd(["mbuild attr ", index, " damsizedice ", 6].join(""));
    cmd(["mbuild attr ", index, ' name "toughened inner city teen" '].join(""));
    cmd(["mbuild attr ", index, " sex MALE "].join(""));
    cmd(["mbuild attr ", index, " action AGGRESSIVE ISNPC"].join(""));
    var short_desc = ["Toughened inner-city teenager"].join("");
    cmd(
      ["mbuild attr ", index, ' short_description "', short_desc, '" '].join("")
    );
    var long_desc = ["Toughened inner-city teenager"].join("");
    var description = [
      "Cast out by the system that failed him, this young kid has so much potential ",
      "just so long as he stays out of trouble (an impossible task given his current ",
      "environment). Behind that tough facade is a kid who wants to overcome this city.",
    ].join("");
    cmd(
      ["mbuild attr ", index, ' long_description "', long_desc, '" '].join("")
    );
    cmd(["mbuild attr ", index, ' description "', description, '" '].join(""));
    cmd(["mbuild save ", index].join(""));
    return 0;
  },
  bootstrap_mobs: function () {
    send("creating mf13 hoodlum...");
    vertical_slice.mf13_hoodlum(5);
    send("done");
    send("creating mf13 initiate...");
    vertical_slice.mf13_hoodlum_initiate(6);
    send("done");
    send("creating harmless citizen...");
    vertical_slice.harmless_citizen(7);
    send("done");
    send("creating la posta chef...");
    vertical_slice.la_posta_chef(8);
    send("done");
    send("creating toughened teen...");
    vertical_slice.toughened_teenager(9);
    send("done");
  },
}; //end vertical_slice
/**
 * ================================================================================
 * ================================================================================
 * ================================================================================
 * ================================================================================
 */

/**
 * ================================================================================
 * ================================================================================
 * || SECTION:
 * ||         R I F L E  A T T A C H M E N T S
 * ||					C O N T E N T
 * ||
 * ================================================================================
 * ================================================================================
 */
function do_rifle_attachment_tests() {
  cmd("room_dark off");
  cmd(
    "instantiate_rifle_attachment rifle/g36c.yml{barrel:silencer.yml,grip:hexon-grip.yml,magazine:extended-ar-mag.yml,muzzle:flash-hider.yml,sight:reflex-sight.yml,stock:titanium-stock.yml,strap:tactical-strap.yml,under_barrel:frag-underbarrel.yml}"
  );
  cmd("examine g36c");
  cmd("give g36c sniper");
}
/**
 * ================================================================================
 * ================================================================================
 * ================================================================================
 * ================================================================================
 */

/**
 * ================================================================================
 * ================================================================================
 * || SECTION:
 * ||         P R O C E D U R A L  G E N E R A T I O N
 * ||					C O N T E N T
 * ||
 * ================================================================================
 * ================================================================================
 * v-content v-procgen v-procedural-generation
 */
function do_random_area() {
  var start = -400;
  cmd("recall");
  set_points("far", "move", 10000);
  cmd(["generate_zone 200 ", start, " -400 -400"].join(""));
  cmd("recall");
  set_points("far", "move", 10000);
  start = -500;
  cmd(["generate_zone 200 ", start, " -400 -400"].join(""));
  cmd("recall");
  set_points("far", "move", 10000);
  start = -600;
  cmd(["generate_zone 200 -400 ", start, " -400"].join(""));
}
/**
 * ================================================================================
 * ================================================================================
 * ================================================================================
 * ================================================================================
 */

function phase1() {
  clear_zone_data_entries();
  do_cobalt_fob();
  send(
    [
      "When you're ready, do a {grn}rbuild save-paved 0{/grn} to save the rooms you just paved\r\n",
      "I've placed all the above instructions in a helper function:\r\n",
      "Simply call: {grn}js phase2();{/grn}\r\n",
    ].join("")
  );
	run(['rbuild save-paved 0', 'recall']);
}
function do_lockers(){
  run(["recall"]);
  create_tier_1_armor_locker();
  create_ammunition_locker();
  create_tier_1_weapons_locker();
}
function phase2() {
  run(["recall"]);
	do_lockers();

  //run(["recall"]);
  //do_triton_lab();
  //run(["recall"]);
  //do_ballistics_lab();
  //run(["recall"]);
  //build_hv_mag_contract();
  //run(["recall"]);
  //do_triton_scripted_content();
  //run(["recall"]);
  //do_waypoint_avenue_stalkers();
  //run(["recall"]);
  //cobalt_military_police();
  //run(["recall"]);
  place_cars_in_garage();
  run(["recall"]);
  garage_thieves();
  run(["recall"]);
  do_shipyard_area();
  run(["recall"]);

	/** These two areas conflict with whats int
	 * beta.sql
	 */
  //run(["recall"]);
  //do_abbot_market_east();
  //run(["recall"]);
  //do_vale_church_area();
  //run(["recall"]);
	
	/**
	 * THese areas are alreaady in the beta.sql
	 */
  run(["recall"]);
  do_metropolitan_area();
  run(["recall"]);
  do_metro_bank_west_alley();
  run(["recall"]);
  do_level_2_drones();
  run(["recall"]);
  do_level_2_drones_east();
  run(["recall"]);
  do_drone_mobs();
  run(["recall"]);
	build_defiler_boss();
  run(["recall"]);
	build_defiler_boss_contract();
  run(["recall"]);
	orthos_guards();
	run(['recall']);
	do_psi_tech_building();
	run(['recall']);
}
function phase3(){
	fixup_content();
	hydro_proc_extremists();
}
function instigate_snipe() {
  run([
    //'yaml_import melee shock-naginata.yml',
    "yaml_import rifle mp5.yml",
    "i",
    "wield mp5",
    "n",
    "n",
    "w",
    "snipe shotgunner e",
    //'k enforcer'
  ]);
}
function echo_all_sql_commands() {
  run(["set_value ECHO_ALL_SQL_COMMANDS 0"]);
}

function f() {
  for (var i = 0; i < 50; i++) {
    cmd("attach_shotgun_underbarrel");
  }
  run(["n", "n", "n", "w", "k thief"]);
}
function frag_test() {
  run(["n", "n", "n", "w", "w", "w", "w"]);
}
function corrosive_claymore_test() {
  run(["build_corrosive_claymore", "n", "n", "install corrosive east"]);
}
function content() {
  /**
   * Please note: this content is in beta.sql
   */
	//We no longer have to call phase1 because that's already in beta.sql
  //phase1();
  phase2();
	phase3();

}
function defiler_testing(){
	run([
		'vnumtele 6',
		'admin:pull sniper',
	]);
}
function frag_test(){
	for(var i=0; i < 20;i++){
		cmd('yaml_import explosive frag-grenade.yml');
	}
	run([
		'vnumtele 595',
	]);
	for(var i=0; i < 5;i++){
		run([
			'rbuild texture:add VOLATILE',
			'w',
		]);
	}
	for(var i=0; i < 5;i++){
		run([
			'e',
		]);
	}
	run([
		'hold grenade',
		'throw west 1',
		'w',
		'w',
	]);
}

function save(){
	return 'rbuild save';
}
function create_cofob_eq_inventory_mobs(room_nick){
  var inventory_specialist_vnum = 112;
	var inventory_grunt_vnum = 113;
  var inventory_specialist_spawn = [
    get_vnum(room_nick),
  ];
  var vnums = [inventory_specialist_vnum,inventory_grunt_vnum];
  clear_mob_zone_commands_for(vnums);
  var mobs = {
    inventory_specialist: {
      mbuild: {
        attr: {
          exp: 1,
          virt: inventory_specialist_vnum,
          mana: 98100,
          max_mana: 98100,
          hit: 98493,
          max_hit: 98493,
          move: 9882,
          max_move: 9882,
          damroll: 950,
          gold: 0,
          level: 200,
          weight: 10,
          height: 5,
          strength: 99,
          strength_add: 500,
          intelligence: 500,
          dexterity: 500,
          constitution: 500,
          electronics: 500,
          chemistry: 500,
          strategy: 500,
          marksmanship: 500,
          sniping: 500,
          weapon_handling: 500,
          demolitions: 500,
          armor: 500,
          medical: 500,
          charisma: 500,
          damnodice: 950,
          damsizedice: 950,
          name: "An inventory specialist",
          sex: "FEMALE",
          action: "ISNPC NODROP",
          short_description: "An inventory specialist",
          long_description: "An inventory specialist",
          description: "An inventory specialist takes a calculating look at the list of items on today\'s list.",
        } /** end attr */,
        "extended-type": "GENERIC_THIEF",
        "roam-pattern": "Inventory",
      },
      meqbuild: {
        name: "cofob-inventory-specialist",
        positions: {
          body: "armor/basic-ballistic-vest.yml",
					wield: "rifle/defiler-scarh.yml",
          hands: "armor/brown-leather-gloves.yml",
          legs: "armor/mp-enforcer-pants.yml",
          feet: "armor/xm50-ultralight-boots.yml",
          elbow_l: "armor/titan-elbow-guards.yml",
          elbow_r: "armor/titan-elbow-guards.yml",
          shoulders_l: "armor/titan-shoulder-pads.yml",
          shoulders_r: "armor/titan-shoulder-pads.yml",
        },
      },
      zbuild: {
        command: "mob",
        zone_id: 0,
        mob_vnum: inventory_specialist_vnum,
        room_vnum: inventory_specialist_spawn,
        max: 1,
        if_flag: 0,
      },
    },
    inventory_grunt: {
      mbuild: {
        attr: {
          exp: 1,
          virt: inventory_grunt_vnum,
          mana: 98100,
          max_mana: 98100,
          hit: 98493,
          max_hit: 98493,
          move: 9882,
          max_move: 9882,
          damroll: 950,
          gold: 0,
          level: 200,
          weight: 10,
          height: 5,
          strength: 99,
          strength_add: 500,
          intelligence: 500,
          dexterity: 500,
          constitution: 500,
          electronics: 500,
          chemistry: 500,
          strategy: 500,
          marksmanship: 500,
          sniping: 500,
          weapon_handling: 500,
          demolitions: 500,
          armor: 500,
          medical: 500,
          charisma: 500,
          damnodice: 950,
          damsizedice: 950,
          name: "An inventory grunt",
          sex: "MALE",
          action: "ISNPC NODROP",
          short_description: "An inventory grunt",
          long_description: "An inventory grunt",
          description: "An inventory grunt is here breaking a sweat. He hardly notices you.",
        } /** end attr */,
        "extended-type": "GENERIC_THIEF",
        "roam-pattern": "Inventory",
      },
      meqbuild: {
        name: "cofob-inventory-grunt",
        positions: {
          body: "armor/basic-ballistic-vest.yml",
					wield: "rifle/defiler-scarh.yml",
          hands: "armor/brown-leather-gloves.yml",
          legs: "armor/mp-enforcer-pants.yml",
          feet: "armor/xm50-ultralight-boots.yml",
          elbow_l: "armor/titan-elbow-guards.yml",
          elbow_r: "armor/titan-elbow-guards.yml",
          shoulders_l: "armor/titan-shoulder-pads.yml",
          shoulders_r: "armor/titan-shoulder-pads.yml",
        },
      },
      zbuild: {
        command: "mob",
        zone_id: 0,
        mob_vnum: inventory_grunt_vnum,
        room_vnum: inventory_specialist_spawn,
        max: 3,
        if_flag: 0,
      },
    },
  };
  auto_feed_mob_list(mobs);
}
function create_maintenance_mobs(){
	run([
		'recall',
		repeat(3,'s'),
	]);
	var maintenance_guard_vnum = 667;
	var maintenance_guard_rooms = [];

	var maintenance_grunt_vnum = 668;
	var maintenance_grunt_rooms = [];

  var orthos_spawn_sentinel_vnum = 669;
  var orthos_sentinel_spawn_vnums = [];
	for(var i=0; i < 10; i++){
		run([
			'w',
			mark('maintenance-grunt-roaming-' + i),
			'rbuild save',
		]);
		
		maintenance_grunt_rooms.push(get_vnum('maintenance-grunt-roaming-' + i));
	}
	run([
		'recall',
		repeat(3,'s'),

	]);

	for(var i=0; i < maintenance_grunt_rooms.length;i++){
		maintenance_guard_rooms.push(maintenance_grunt_rooms[i]);
	}
	for(var i=0; i < 7;i++){
		run([
			's',
			mark('maintenance_guard-roaming-' + i),
			'rbuild save',
		]);
		maintenance_guard_rooms.push(get_vnum('maintenance_guard-roaming-' + i));
	}


  clear_mob_zone_commands_for([maintenance_guard_vnum,maintenance_grunt_vnum,orthos_spawn_sentinel_vnum]);

	run([
		'recall',
		's',
		mark('orthos-spawn-sentinel-1'),
		save(),
	]);
	orthos_sentinel_spawn_vnums = [get_vnum('orthos-spawn-sentinel-1')];

  var mobs = {
    orthos_spawn_sentinel: {
      mbuild: {
        attr: {
          exp: 0,
          virt: orthos_spawn_sentinel_vnum,
          level: 85,
          mana: 9080,
          max_mana: 9080,
          hit: 985000,
          max_hit: 985000,
          move: 1500,
          max_move: 1500,
          damroll: 230,
          gold: 0,
          weight: 1,
          height: 5,
          strength: 900,
          strength_add: 91,
          intelligence: 902,
          dexterity: 944,
          constitution: 944,
          electronics: 942,
          chemistry: 944,
          strategy: 944,
          marksmanship: 944,
          sniping: 944,
          weapon_handling: 1000,
          demolitions: 1000,
          armor: 1000,
          medical: 1000,
          charisma: 0,
          damnodice: 950,
          damsizedice: 800,
          name: "Orthos sentinel",
          sex: "MALE",
          action: "ISNPC HELPER AGGR_GOOD NODROP",
          short_description: "Orthos sentinel",
          long_description: "Orthos sentinel",
          description: "Orthos sentinel",
        } /** end attr */,
        "extended-type": "ORTHOS_SPAWN_SENTINEL",
      },
      meqbuild: {
        name: "orthos-sentinel-spawn",
        positions: {
          wield: "rifle/orthos-sentinel-scarh.yml",
          hands: "armor/forge-xm3-gloves.yml",
          legs: "armor/mp-enforcer-pants.yml",
          feet: "armor/xm50-ultralight-boots.yml",
          elbow_l: "armor/titan-elbow-guards.yml",
          elbow_r: "armor/titan-elbow-guards.yml",
          shoulders_l: "armor/titan-shoulder-pads.yml",
          shoulders_r: "armor/titan-shoulder-pads.yml",
        },
      },
      zbuild: {
        command: "mob",
        zone_id: 0,
        mob_vnum: orthos_spawn_sentinel_vnum,
        room_vnum: orthos_sentinel_spawn_vnums,
        max: 6,
        if_flag: 0,
      },
    },
  };
  auto_feed_mob_list(mobs);
}

var extremist_vnum = 670

var extremist_grunt_vnum = 671
var extremist_grunt_rooms = [];

function clear_hydro_proc_extremists_zone_commands(){
  clear_mob_zone_commands_for([extremist_vnum,extremist_grunt_vnum]);
}


/**
 * What can we parameterize for the purpose of dynamic raid content?
 * - level
 * - count
 * - roam pattern
 * - rooms
 * - armor sets
 * - weapons
 * - extended-type
 */

var configurations = {
	GENERIC_THIEF: {
		wield_types: [
			'sniper','ar','smg','lmg','pistol',
		],
	},
	MP_SHOTGUNNER: {
		wield_types: [
			'shotgun',
		],
	},
	DEFILER: {
		wield_types: [
			'ar','smg','lmg','sniper',
		],
	},
	LOWLY_SECURITY: {
		wield_types: [
			'ar','smg','lmg','sniper',
		],
	},
	CAR_THIEF: {
		wield_types: [
			'melee',
		],
	},
	CHAOTIC_METH_ADDICT: {
		wield_types: [
			'melee',
		],
	},
	SHOPLIFTER: {
		wield_types: [
			'smg','ar','lmg','sniper',
		],
	},
	ORTHOS_AGENT: {
		wield_types: [
			'ar','lmg','sniper',
		],
	},
	MELEE_COMBATANT: {
		wield_types: [
			'melee',
		],
	},
};

function dynamic_extremist(obj){
	var level = obj.level;
	var vnum = obj.vnum;
	var count = obj.count;
	var roam_pattern = obj.roam_pattern;
	var rooms = obj.spawn;

  var mobs = {
    extremist: {
      mbuild: {
        attr: {
					scalable: 1,
          exp: 50 * level,
          virt: vnum,
          mana: 50 * level,
          max_mana: 50 * level,
          hit: 245 * level,
          max_hit: 245 * level,
          move: 42 * level,
          max_move: 42 * level,
          damroll: 5 * level,
          gold: 45 * level,
          level: level,
          weight: 10,
          height: 5,
          strength: 1 * level,
          strength_add: 1 * level,
          intelligence: 1 * level,
          dexterity: 2 * level,
          constitution: 1 * level,
          electronics: 1 * level,
          chemistry: 1 * level,
          strategy: 1 * level,
          marksmanship: 2 * level,
          sniping: 1 * level,
          weapon_handling: 1 * level,
          demolitions: 1 * level,
          armor: 1 * level,
          medical: 1 * level,
          charisma: 2 * level,
          damnodice: 1 * level,
          damsizedice: 4 * level,
          name: "An extremist",
          sex: "MALE",
          action: "ISNPC",
          short_description: "An extremist",
          long_description: "An extremist",
          description: "An extremist spitefully makes eye contact with you.",
        } /** end attr */,
        targets: "WEAPONS STREETS OIL WATER FIRE GASOLINE",
        "extended-type": "GENERIC_THIEF",
        "roam-pattern": roam_pattern, 
      },
      meqbuild: {
        name: "hydro-proc-extremists-dynamic-" + level + "-" + vnum,
        positions: {
          body: "armor/leather-trenchcoat.yml",
					wield: "rifle/tar21.yml",
          legs: "armor/pale-brown-pants.yml",
          feet: "armor/plain-black-shoes.yml",
        },
      },
      zbuild: {
        command: "mob",
        zone_id: 0,
        mob_vnum: vnum,
        room_vnum: rooms,
        max: count,
        if_flag: 0,
      },
    },
  };
  auto_feed_mob_list(mobs);
}

function hydro_proc_extremists(){
	var extremist_rooms = [];
	run([
		'recall',
		repeat(3,'s'),
		repeat(2,'w'),
		repeat(5,'d'),
	]);
	for(var i=0; i < 5; i++){
		run([
			's',
			mark('hydro-proc-extremists-roaming-' + i),
			'rbuild save',
		]);
		extremist_rooms.push(get_vnum('hydro-proc-extremists-roaming-' + i));
	}

	dynamic_extremist({
		vnum: 670,
		level: 5,
		count: 10,
		roam_pattern: "Hydro-Processing",
		spawn: extremist_rooms,
	});
}
/**
 * ================================================================================
 * ================================================================================
 * || SECTION:
 * ||         F I X E S  FOR EXISTING/MISSING CONTENT
 * ||
 * ================================================================================
 * ================================================================================
 * v-fixes v-content
 */
function fixup_content(){
	var southern_hallway = [
		'The southern hallway of the COFOB aligns exactly with a series ',
		'of huge sewer pipes. Several years ago, a recruit went AWOL and ',
		'hijacked a tank. He drove it through the southern hallway and other parts ',
		'of the FOB. As a result, the sewage drains burst open and flooded the entire ',
		'first level of the FOB. It is why the common nickname for the southern hallway ',
		'is "el bano".',
	].join('');
	var southern_hallway_single_door = [
			'There is a single door here leading to the inner hallway of the FOB. From there, you can ',
			'make your way to the different areas like the TRITON Laboratory, the kitchen, the engineering ',
			'level, and the exit to the outlying city surrounding the FOB.',
	].join('');
   var east_hallway = [
		 'Many complaints have been lodged against the higher ups about the lack ',
		 'of cleanliness of the eastern hallway. An odd dampness is prevalent throughout. ',
		 'The temperature outside somehow affects this part of the FOB more than others. ',
		 'During sand storms, the eastern hallway turns to a sandy damp mess.',
	].join('');
  var east_hallway_triton_entrance = [
		'A sign above the door to the East says "{yel}TRITON LABS{/yel}" in big bold ',
		'lettering. The placement of the laboratory is an odd one, as it is one of the ',
		'only ways to reach the civilian portion of the FOB via the East.',
	].join('');
	var triton_decon = [
		'In order for the lab to maintain a clean and sterile atmosphere, a decontamination ',
		'area was installed. That is the room you find yourself in. A station not unlike ',
		'the body scanners you find at an airport sprays you up and down with a cool mist ',
		'of chemicals. A vacuum below your feet sucks wind and your ears pop as various ',
		'hidden but audible devices clamor to eliminate any hazardous bacteria.',
	].join('');
	var triton_res = [
		'An array of server racks blocks the center of the room from where you are standing.',
		'TRITON Labs is a sub-contractor of the incredibly murky organization known as ',
		'{blu}ORTHOS CORP{/blu}. The lab specializes in HIGHLY experimental ammunition ',
		'which blurs the line between ethical and immoral conduct. TRITON Labs is believed ',
		'to have developed an extreme pathogen with the power to wipe out an entire city ',
		'block with just a few drops of a highly secretive chemical agent.',
	].join('');
  var triton_exp = [
		'There used to be a time when clients would pay for highly specialized medical products.',
		'During the apex of that booming underground market, scientists were paid top dollar to ',
		'secure the targetted product under any means necessary. The TRTION labs human experimentation ',
		'sector is located somewhere beneath the FOB, though some say it was forever closed off ',
		'forever. The leftover and forgotten human experiments reside in this room indefinitely. ',
		'To have survived those experiments is a greater hell than to have perished.',
	].join('');
	var triton_ammo = [
		'TRITON labs has been home to the most advanced bleeding edge experimental ammunition ',
		'on the planet. Racks of hermetically sealed boxes with TRITON\'s menacing logo reach ',
		'almost to the full height of the extended ceiling. There is a single computer terminal ',
		'here asking for biometric credentials. A camera adjusts it\'s focus and magnification ',
		'as you move.',
	].join('');
	var cofob_eq = [
		'Three huge lockers are attached to the wall, each with their own category of equipment. ',
		'The most appalling detail is the haphazard array of bloodied medical equipment ',
		'sitting on a nearby desk.',
	].join('');
	var north_hallway = [
		'The north end of the FOB has always been a much nicer and cleaner place than the rest of the base. ',
		'Primarily due to the frequency of senior officer suites. The north hallway will also bring you to the ',
		'second floor where a large majority of the engineering work is done.',
	].join('');
	var stair_case = [
		'A remarkably plain staircase is here. The second floor of the FOB is home to the {yel}Engineering{/yel} ',
		'team. Already you can tell that the air quality is much better upstairs.',
	].join('');
	var going_up = [
		'The walls are a drab shade of grey and the stairs are an odd combination of carpet and metal. ',
	].join('');
	var eng_atop = [
		'The computing industry has progressed to the point where every individual has their own ',
		'computer in the palm of their hand. But those advances dont\'t come without a cost. ',
		'The engineering level of the FOB focusses on the invisible threat of state actors ',
		'and the growing need for tighter security. ',
	].join('');
	var eng_infra = [
		'It\'s almost comical how clean the engineering level of the FOB is compared to the lower levels. ',
		'The carpet and walls are in a prestine -- no, IMMACULATE state. You almost feel like you should ',
		'take off your shoes. The climate is controlled by a different air-conditioning system than the ',
		'rest of the base. The room is cool and devoid of any outside noise.',
	].join('');
	var eng_threat = [
		'There are a couple high profile incidents where law enforcement were locked out of the personal ',
		'devices of a deceased criminal. A hard lesson in cryptography was learned that day. As a result, ',
		'funding instead went into massive amounts of global surveillance and threat tracking. The advent ',
		'of Artificial Intelligence gave another possible avenue: automated tracking. The {red}Threat Surveillance{/red} ',
		'sector of the FOB is the most advanced and funded aspect of every FOB to date.',
	].join('');
	var exit_a1f = [
		'Prior to the investments of outside sources (driven by the income TRITON has generated), the FOB ',
		'had primarily one exit to the outside world: Exit A1F. Or, affectionately nicknamed: Albatross. ',
		'The metal enclosure is barely wide enough for one person with full kit to pass through... a rite of ',
		'passage, if you will. To the south, you see a singular double door labeled: "EXIT A1F".',
	].join('');
	var south_exit_a1f = [
		'To the south, you can eventually leave the FOB premises. The opposite direction will bring you back to ',
		'the southern end of the FOB. The dirt and hellish heat of the desert greets you like a slap in the face. ',
	].join('');
	var pharm = [
		'Prior to the installation of the FOB, the residents of Abbot City had no source of cheap and subsidized ',
		'pharmaceuticals. The FOB created jobs and opportunities that would never be possible prior to it\'s existence. ',
		'Ironically, the FOB also brought more danger and resentment from the extremist sect of the local religion. ',
	].join('');
	var mort_way = [
		'Rubble and sand mix together to form a harsh surface area. It\'s hard to believe that this was once ',
		'a thriving metropolitan roadway. The street signs were supposed to display something other than \'Morticians Way\', ',
		'but every sign has a hard to remove sticker saying otherwise. ',
	].join('');
	run([
		'recall',
    "rbuild pave continue",
    mark("cofob-spawn-point-a"),
    title("C.O.F.O.B - Welcome to hell"),
    desc([
			'You find yourself in a humid, climate-controlled room with a heavy industrial fan ',
			'above you swirling all the dust around. Three very large lockers are firmly bolted ',
			'against the wall here. Each locker containing armor, weapons, and ammunition.',
		].join('')),
    texture("INSIDE ROOFTOP CEMENT DAMP"),
		save(),

    "n",
    title("C.O.F.O.B - Hallway entrance"),
    desc(southern_hallway_single_door),
    texture("INSIDE ROOFTOP CEMENT DAMP"),
		save(),

    "n",
    title("{blu}C.O.F.O.B{/blu} - Southern Hallway"),
    desc(southern_hallway),
		save(),

		repeat(2, [
			"e",
			title("{blu}C.O.F.O.B{/blu} - Southern Hallway"),
			desc(southern_hallway),
			save(),
		]),

		repeat(2,'w'),

    repeat(6,["w",
			title("{blu}C.O.F.O.B{/blu} - Southern Hallway"),
			desc(southern_hallway),
			save(),
		]),

		'recall',
		repeat(2,'n'),
		repeat(2,'e'),

    title("{blu}C.O.F.O.B{/blu} - East Hallway"),
    desc(east_hallway),
		save(),

		'n',
    title("{blu}C.O.F.O.B{/blu} - East Hallway"),
    desc(east_hallway),
		save(),

		'n',
    title("{blu}C.O.F.O.B{/blu} - East Hallway - TRITON Entrance"),
    desc(east_hallway_triton_entrance),
		save(),

		'e',
    title("{blu}C.O.F.O.B{/blu} - TRITON - Decontamination"),
    desc(triton_decon),
		save(),

		'e',
    title("{blu}C.O.F.O.B{/blu} - TRITON - Research"),
    desc(triton_res),
		save(),

		'e',
    title("{blu}C.O.F.O.B{/blu} - TRITON - Research"),
    desc(triton_res),
		save(),

		'e',
    title("{blu}C.O.F.O.B{/blu} - TRITON - Human Experiments"),
    desc(triton_exp),
		save(),

		repeat(2,'w'),

		'n',
    title('{blu}C.O.F.O.B{/blu} - TRITON - Experimental Ammunition'),
    desc(triton_ammo),
		save(),

		repeat(2,'s'),
    title('{blu}C.O.F.O.B{/blu} - TRITON - Experimental Ammunition'),
    desc(triton_ammo),
		save(),

		'recall',
    title('{blu}C.O.F.O.B{/blu} - Equipment'),
    desc(cofob_eq),
		save(),

		'recall',
		repeat(2,'n'),

		repeat(2,'w'),
		/** NEW ROOM */
		'n',
    title('{blu}C.O.F.O.B{/blu} - Equipment Inventory'),
    desc(cofob_eq),
		'rbuild flag:add PEACEFUL INDOORS DMZ NOTRACK',
    texture("INSIDE ROOFTOP METAL_WALL TILE"),
		'rbuild exit:add south CLOSED ISDOOR',
		'rbuild dopt south keyword door',
    mark("cofob-equipment-inventory"),
		save(),

		'recall',
		repeat(2,'n'),
		repeat(2,'e'),
		repeat(2,'n'),

		repeat(6,[
			'n',
			title("{blu}C.O.F.O.B{/blu} - East Hallway"),
			desc(east_hallway),
			save(),
		]),
		'n',
		repeat(4,[
			'n',
			title("{blu}C.O.F.O.B{/blu} - East Hallway"),
			desc(east_hallway),
			save(),
		]),
		'n',
		repeat(2,[
			'w',
			title("{blu}C.O.F.O.B{/blu} - North Hallway"),
			desc(north_hallway),
			save(),
		]),
		'w',
		title("{blu}C.O.F.O.B{/blu} - Staircase"),
		desc(stair_case),
		save(),

		'n',
		title("{blu}C.O.F.O.B{/blu} - Up the Staircase"),
		desc(going_up),
		save(),

		'u',
		title("{blu}C.O.F.O.B{/blu} - Up the Staircase"),
		desc(going_up),
		save(),

		'n',
		title("{blu}C.O.F.O.B{/blu} - Up the Staircase"),
		desc(going_up),
		save(),

		'u',
		title("{blu}C.O.F.O.B{/blu} - Engineering - Atop the stairs"),
		desc(eng_atop),
		save(),

		repeat(2,['n',
			title("{blu}C.O.F.O.B{/blu} - Engineering - Atop the stairs"),
			desc(eng_atop),
			save(),
		]),
		repeat(5,['e',
			title("{blu}C.O.F.O.B{/blu} - Engineering - Infrastructure Hallway"),
			desc(eng_infra),
			save(),
		]),
		repeat(5, 'w'),
		repeat(5,['w',
			title("{blu}C.O.F.O.B{/blu} - Engineering - Threat Surveillance"),
			desc(eng_threat),
			save(),
		]),

		'recall',
		repeat(2,'n'),
		repeat(2,'e'),
		repeat(14,'n'),

		mark('cofob-northeast-corner'),
		title("{blu}C.O.F.O.B{/blu} - North Hallway"),
		desc(north_hallway),
		save(),

		repeat(3,'w'),
		repeat(5,[
			'w',
			title("{blu}C.O.F.O.B{/blu} - North Hallway"),
			desc(north_hallway),
			save(),
		]),
	]);

	var maint = [
		'You make your way through the southern maintenance hallway. Your feet clank on the reinforced ',
		'steel grating, except when you make contact with the solid metal portions which glue each ',
		'segment together. There are heavily enforced windows about the size of a dinner plate every ',
		'10 feet. The red and orange stains of sand and soot stain the windows. ',
	].join('');

	run([

		/** NEW CONTENT */
		/** Southern exit A1F */
		'recall',
		'rbuild pave continue',
		repeat(10,[
			's',
			title("{blu}C.O.F.O.B{/blu} - Maintenance"),
			desc(maint),
    	texture("INSIDE ROOFTOP METAL_WALL METAL_FLOORS"),
			save(),
		]),

		'rbuild pave off','rbuild save-paved 0',
  ]);

	/** NEW CONTENT */
	create_cofob_eq_inventory_mobs("cofob-equipment-inventory");

	var hydro_proc = [
		'For as long as you can see (east and west), a blue, black, and green tubing the size of a small ',
		'dog are pinned to the bottom north and south corners. It is noisey in here as whatever is inside that ',
		'tubing is being fiercely pushed through.',
	].join('');
	var hydro_serv = [
		'A large proportion of the room consists of a locked down computer terminal with old hardware. But it doesn\'t ',
		'seem to be just old hardware: everything about it is built to withstand several attempts at trying to access ',
		'the hardware inside. Things like the hard drives or the device in charge of the biometric interface. ',
		'It\'s been said that this terminal can survive several grenade blasts. As of yet, nobody has tried to compromise ',
		'the  machine, but never say never.',
	].join('');
	var hydro_ladder = [
		'The ladder is reinforced with large protroduing bolts on both sides. There are some points that need to be re-welded ',
		'as the connections to the sturdiness of the wall seems to have been severed somehow. You notice that the ladder should ',
		'wiped down due to the massive amount of grease, oil, and somehow blood. ',
	].join('');
	run([
		'recall',
		'rbuild pave continue',
		repeat(3,'s'),
		repeat(10,['w',
			title("{blu}On Base{/blu} - Hydro-Processing"),
			desc(hydro_proc),
    	texture("INSIDE METAL_WALL METAL_FLOORS SEWER DAMP METAL_HATCH TUNNEL SHALLOW_WATER NARROW_EAST_WEST"),
			save(),
		]),
		'recall',
		repeat(3,'s'),
		repeat(2,'w'),
		repeat(5,['d',
			title("{blu}On Base{/blu} - Hydro-Processing"),
			desc(hydro_ladder),
    	texture("INSIDE METAL_WALL METAL_FLOORS SEWER DAMP METAL_HATCH TUNNEL SHALLOW_WATER NARROW_EAST_WEST"),
			save(),
		]),
		repeat(5,['s',
			title("{blu}On Base{/blu} - Hydro-Processing"),
			desc(hydro_proc),
    	texture("INSIDE METAL_WALL METAL_FLOORS NARROW_NORTH_SOUTH"),
			save(),
		]),
		repeat(2,['w',
			title("{blu}On Base{/blu} - Hydro-Processing"),
			desc(hydro_proc),
    	texture("INSIDE METAL_WALL METAL_FLOORS SEWER DAMP METAL_HATCH TUNNEL SHALLOW_WATER NARROW_EAST_WEST"),
			save(),
		]),
		mark('hydro-proc-start-point'),
		save(),
		repeat(8,['w',
			title("{blu}On Base{/blu} - Hydro-Processing"),
			desc(hydro_proc),
    	texture("INSIDE METAL_WALL METAL_FLOORS SEWER DAMP METAL_HATCH TUNNEL SHALLOW_WATER NARROW_EAST_WEST"),
			save(),
		]),
		'n',
			title("{blu}On Base{/blu} - Hydro-Processing - Terminal"),
			desc(hydro_serv),
    	texture("INSIDE METAL_WALL METAL_FLOORS NARROW_NORTH_SOUTH SERVER_ROOM"),
			save(),
		's',
		's',
			title("{blu}On Base{/blu} - Hydro-Processing - Terminal"),
			desc(hydro_serv),
    	texture("INSIDE METAL_WALL METAL_FLOORS NARROW_NORTH_SOUTH SERVER_ROOM"),
			save(),
		'n',
		repeat(4,['w',
			title("{blu}On Base{/blu} - Hydro-Processing"),
			desc(hydro_proc),
    	texture("INSIDE METAL_WALL METAL_FLOORS SEWER DAMP METAL_HATCH TUNNEL SHALLOW_WATER NARROW_EAST_WEST"),
			save(),
		]),
		repeat(2,'e'),
		'n',
			title("{blu}On Base{/blu} - Hydro-Processing - Terminal"),
			desc(hydro_serv),
    	texture("INSIDE METAL_WALL METAL_FLOORS NARROW_NORTH_SOUTH SERVER_ROOM"),
			save(),
		's',
		's',
			title("{blu}On Base{/blu} - Hydro-Processing - Terminal"),
			desc(hydro_serv),
    	texture("INSIDE METAL_WALL METAL_FLOORS NARROW_NORTH_SOUTH SERVER_ROOM"),
			save(),
		'n',
		repeat(2,'w'),
		repeat(2,['n',
			title("{blu}On Base{/blu} - Hydro-Processing"),
			desc(hydro_proc),
    	texture("INSIDE METAL_WALL METAL_FLOORS NARROW_NORTH_SOUTH"),
			save(),
		]),
		repeat(3,['w',
			title("{blu}On Base{/blu} - Hydro-Processing"),
			desc(hydro_proc),
    	texture("INSIDE METAL_WALL METAL_FLOORS NARROW_EAST_WEST"),
			//mark('hydro-proc-top-left'),
			save(),
		]),
		'rbuild pave off',
		'rbuild save-paved 0',
	]);

	run([
		'recall',
		repeat(2,'n'),
		repeat(2,'e'),
		repeat(9,'n'),
		repeat(6,'e'),
		repeat(10,'n'),
		repeat(2,'e'),
		'rbuild pave continue',
		repeat(3,['e',
			title("{grn}Abbot Market East{/grn} - Allied Foods - Pharmacy"),
			desc(pharm),
    	texture("INSIDE CARPET GLASS_WINDOWS ROOFTOP"),
			save(),
		]),
		repeat(3,['n',
			title("{grn}Abbot Market East{/grn} - Allied Foods - Pharmacy"),
			desc(pharm),
    	texture("INSIDE CARPET GLASS_WINDOWS ROOFTOP"),
			save(),
		]),
		repeat(3,'s'),


		'rbuild pave off',
		'rbuild save-paved 0',
	]);

	/** NEW CONTENT */
	create_cofob_eq_inventory_mobs("cofob-equipment-inventory");
}
function cmm(){
	create_maintenance_mobs();
	do_fountain_drone_mobs();
}

function fixup(){
	fixup_content();
	create_maintenance_mobs();
	do_fountain_drone_mobs();
}

function level_10_mp_shotgunner(){
	run([
		'recall',
		repeat(2,'n'),
		repeat(2,'e'),
		repeat(12,'n'),
		'w',
		'd',
		repeat(2,'w'),
		'd',
		repeat(2,'w'),
		repeat(2,'s'),
	]);
	var r = [];
	for(var i=0; i < 6;++i){
		r.push(exec('rbuild vnum'));
		run([
			'e',
		]);
	}

	var vnum = exec('mbuild giveme:int:next_vnum');
	dynamic_mp_shotgunner({
		vnum: vnum,
		level: 10,
		count: 5,
		rooms: r,
		roam_pattern: "Mess Hall",
	});
	refresh();
}

/**
 * The core of this function is to just call all the dynamic mob generator functions.
 * Their level will be set as level 10. 
 */
function run_level_10_base_mobs(){
	run([
		'recall',
		repeat(2,'n'),
		repeat(2,'e'),
		repeat(12,'n'),
		'w',
		'd',
		repeat(2,'w'),
		'd',
		repeat(2,'w'),
		repeat(2,'s'),
	]);
	var r = [];
	for(var i=0; i < 6;++i){
		r.push(exec('rbuild vnum'));
		run([
			'e',
		]);
	}


	var obj = {};
	var vnum = 0;
	var funcs = [dynamic_aggressive_thief,
		//dynamic_chaotic_meth_addict,
		//dynamic_crackhead,
		//dynamic_defiler,
		//dynamic_extremist,
		//dynamic_garage_thief,
		//dynamic_mp_enforcer,
		//dynamic_mp_shotgunner,

		//dynamic_ops_shield_shotgunner,
		//dynamic_petty_thief,
		//dynamic_rogue_mp_shotgunner,
		//dynamic_shoplifter,
		//dynamic_vile_kidnapper,
	];

	for(var i=0; i < funcs.length;i++){
		vnum = exec('mbuild giveme:int:next_vnum');
		obj = {
			vnum: vnum,
			level: 10,
			count: 1,
			spawn: r,
			roam_pattern: "Mess Hall",
		};
		funcs[i](obj);
	}
}

function build_hellfire_claymore_contract() {
	var steps = [
		'acquire 5 frag grenades and 5 claymore mines (you can get these items from the infected drones near henley fountain)',
		'bring the grenades and mines to the triton labs scientist',
		'the scientist will give you a blueprint which you need to bring to the inventory specialist',
	];
  return build_contract([
    {
      vnum: contracts.hellfire_claymore,
      title: "{red}Hellfire Claymore Mines{/red}",
      description: [
				"There are some high level scientists working on a prototype claymore mine that does ",
				"{grn}MASSIVE{/grn} incendiary damage. We're interested in acquiring some of these mines for the purpose ",
				"of reverse engineering.\r\n",
				"\r\n",
        "{yel}Here are the contract instructions:{/yel}\r\n",
        "{yel}1) {grn}Acquire {blu}5 frag grenades and 5 claymore mines{/blu}{grn}.{/grn}\r\n",
        "{yel}2) {grn}Find the TRITON Labs scientist.{/grn}\r\n",
        "{yel}3) {grn}Make sure to say the phrase {blu}hellfire{/blu} when you find the TRITON Labs scientist.{/grn}\r\n",
        "{yel}4) {grn}When the scientist acknowledges you, give him the 5 frag grenades then the 5 claymore mines IN THAT ORDER.{/grn}\r\n",
        "{yel}5) {grn}The TRITON Labs scientist will give you 1 hellfire claymore mine for each 5 frags and 5 mines you give him.{/grn}\r\n",
      ].join(""),
      steps: [
				{
          task_type: "GOAL_GIVE",
          task_target: "TARGET_MOB",
          mob_vnum: scripted.triton.scientist.mob,
          reward_xp: 1150,
          reward_money: 1150,
          quota: 5,
          yaml: "explosive/frag-grenade.yml",
          description: [
            "Give 5 frag grenades to the TRITON Labs Scientist\r\n",
          ].join(""),
        },
				{
          task_type: "GOAL_GIVE",
          task_target: "TARGET_MOB",
          mob_vnum: scripted.triton.scientist.mob,
          reward_xp: 1150,
          reward_money: 1150,
          quota: 5,
          yaml: "explosive/claymore-mine.yml",
          description: [
            "Give 5 claymore mines to the TRITON Labs Scientist\r\n",
          ].join(""),
        }
      ],
    },
  ]);
  var greet_vnum = 3;
  var convnum = contracts.hellfire_claymore; // contract vnum
  delete_scripted_sequence_by_vnum(greet_vnum);
  delete_scripted_steps_by_sequence_vnum(greet_vnum);

  register_scripted_sequence(
    greet_vnum /** virtual number of this sequence */,
    [
      {
        wait_ticks: 3,
        dialogue: "Great, give me 5 frag grenades and 5 claymore mines and I can give you a {grn}Hellfire Claymore Mine{/grn}\r\n",
        type: "dialogue",
        mob: scripted.triton.mob,
        room: scripted.triton.room,
			},
    ]
  );
  register_contract_step_callback(
    convnum /** contract vnum */,
    "GOAL_GIVE" /** task type */,
    "TARGET_MOB" /** task target */,
    scripted.triton.room /** target vnum (room vnum in this case) */,
    greet_vnum /** scripted sequence vnum */
  );
  register_scripted_sequence(
    give_mags_vnum /** virtual number of this sequence */,
    [
      {
        wait_ticks: 3,
        dialogue: "You'll find that these magazines pack quite the punch...",
        mob: scripted.triton.mob,
        room: scripted.triton.room_north,
        type: "say",
      },
      {
        wait_ticks: 3,
        type: "mob_gives_yaml",
        mob: scripted.triton.mob,
        room: scripted.triton.room_north,
        yaml: "attachment/hv-neutron-extended-mag.yml",
        quantity: 4,
      },
      {
        wait_ticks: 3,
        dialogue:
          "Take these magazines and bring them to Doctor Land. He can reverse engineer them and make you more copies if you bring him the right materials.",
        mob: scripted.triton.mob,
        room: scripted.triton.room_north,
        type: "say",
      },
      {
        wait_ticks: 10,
        dialogue: "$n rubs $s temples and forces a smile...",
        mob: scripted.triton.mob,
        room: scripted.triton.room_north,
        type: "act",
      },
      {
        wait_ticks: 3,
        dialogue: "$n cleans $s glasses and sighs in exhaustion.",
        mob: scripted.triton.mob,
        room: scripted.triton.room_north,
        type: "act",
      },
      {
        wait_ticks: 3,
        dialogue:
          "I must get back to my work unfortunately. Do let me know how everything works out. Send my regards to Doctor Land, if you will.",
        mob: scripted.triton.mob,
        room: scripted.triton.room_north,
        type: "say",
      },
      {
        wait_ticks: 1,
        room: scripted.triton.room_north,
        type: "seal_room_south",
      },
      {
        wait_ticks: 1,
        room: scripted.triton.room_north,
        type: "force_south",
      },
    ]
  );
  register_contract_step_callback(
    convnum /** contract vnum */,
    "GOAL_FIND" /** task type */,
    "TARGET_ROOM" /** task target */,
    scripted.triton.room_north /** target vnum (room vnum in this case) */,
    give_mags_vnum /** scripted sequence vnum */
  );
}

var kojima = {
	get_spawns: function(kind){
		var spawns = [];
		for(var i=0; i < kojima.created[kind].length;i++){
			spawns.push( get_vnum(kojima.created[kind][i]));
		}
		return spawns;
	},
	north_start: {
		nick: 'kojima-north-start',
	},
	room_state: {
		scout: 0,
		lancer: 0,
		trooper: 0,
		eviscerator: 0,
	},
	scout_prefix: 'kojima-spawn-scout',
	lancer_prefix: 'kojima-spawn-lancer',
	trooper_prefix: 'kojima-spawn-trooper',
	eviscerator_prefix: 'kojima-spawn-eviscerator',
	created: {
		scout: [],
		lancer: [],
		trooper: [],
		eviscerator: [],
	},
	mark_scout_room: function(){
		kojima.room_state.scout += 1;
		var x = [kojima.scout_prefix,kojima.room_state.scout].join('-');
		kojima.created.scout.push(x);
		return mark(x);
	},
	mark_lancer_room: function(){
		kojima.room_state.lancer += 1;
		var x = [kojima.lancer_prefix,kojima.room_state.lancer].join('-');
		kojima.created.lancer.push(x);
		return mark(x);
	},
	mark_trooper_room: function(){
		kojima.room_state.trooper += 1;
		var x = [kojima.trooper_prefix,kojima.room_state.trooper].join('-');
		kojima.created.trooper.push(x);
		return mark(x);
	},
	mark_eviscerator_room: function(){
		kojima.room_state.eviscerator += 1;
		var x = [kojima.eviscerator_prefix,kojima.room_state.eviscerator].join('-');
		kojima.created.eviscerator.push(x);
		return mark(x);
	},
	north_entrance: {
		'west': 'kojima-north-entrance-west',
		'east': 'kojima-north-entrance-east',
	},
	west_entrance: {
		'north': 'kojima-west-entrance-north',
		'south': 'kojima-west-entrance-south',
	},
	mark_north_entrance: function(which){
		return mark(kojima.north_entrance[which]);
	},
	mark_west_entrance: function(which){
		return mark(kojima.west_entrance[which]);
	},
	junction: {
		'A': 'kojima-junction-A',
		'B': 'kojima-junction-B',
		'C': 'kojima-junction-C-INSIDE',
	},
	mark_junction: function(which){
		return mark(kojima.junction[which]);
	},
	zone_to: {
		'x1': 'kojima-zone-to-x1',
		'x2': 'kojima-zone-to-x2',
		'x3': 'kojima-zone-to-x3',
		'x4': 'kojima-zone-to-x4',
		'x5': 'kojima-zone-to-x5',
	},
	mark_zone_to: function(where){
		return mark(kojima.zone_to[where]);
	},
	go_to: function(section,key){
		return go(kojima[section][key]);
	},
	drop_to: {
		'd3': 'kojima-drop-to-d3',
	},
	mark_drop_to: function(where){
		return mark(kojima.drop_to[where]);
	},
	inside: {
		'west': 'kojima-inside-west',
	},
	mark_inside: function(where){
		return mark(kojima.inside[where]);
	},
};

function do_kojima_corp_north() {
	var sh = 'say heh';
	var generic_outer_bridge = [
		"Your body rocks side to side as you make your way over this ",
		"bridge. The drop to the bottom of the canyon would be ",
		"nothing short of fatal. The bridge seems to be reinforced with chain ",
		"metal, and thick pieces of wood.",
	].join('');
	var northern_end = [
		"Your body rocks side to side as you make your way over this ",
		"bridge. The drop to the bottom of the canyon would be ",
		"nothing short of fatal. The bridge seems to be reinforced with chain ",
		"metal, and thick pieces of wood.",
	].join('');
	var section_a = [
		"To the south, you can see a huge eccentric building of grey and red. ",
		"You spot a few armed guards at what looks like a checkpoint. The bridge ",
		"below your feet moves a little less than the one to the North. It's likely ",
		"there are camoflauged drones scanning your every move.",
	].join('');

	var k = function(times,dir){
    return repeat(times, [
      dir,
      "rbuild title {blu}Kojima{/blu} - Bridge Outside",
      "rbuild description " + generic_outer_bridge,
			texture(['AIR','DAMP','FOREST','OUTSIDE','WOODEN_BRIDGE']),
    ]);
	}
	var lr = function(){
		return kojima.mark_lancer_room();
	}
	var sr = function(){
		return kojima.mark_scout_room();
	}
	var er = function(){
		return kojima.mark_eviscerator_room();
	}
	var tr = function(){
		return kojima.mark_trooper_room();
	}
	var g = function(times,dir){
		return repeat(times,[dir]);
	}
  //USEFUL: var cofob_center_vnum = exec("print_vnum " + cofob.center);
  //USEFUL: go(cofob.east_exit),
  run([
		'recall',
    repeat(10, ['s']),
    mark(kojima.north_start),
    "rbuild pave continue",
    repeat(5, [
      "e",
      "rbuild title {blu}Kojima{/blu} - Bridge - North" ,
      "rbuild description " + northern_end ,
			texture(['AIR','DAMP','FOREST','OUTSIDE','WOODEN_BRIDGE'].join(' ')),
    ]),
    repeat(4, [
      "s",
      "rbuild title {blu}Kojima{/blu} - Bridge - North - Section A",
      "rbuild description " + section_a,
			texture(['AIR','DAMP','FOREST','OUTSIDE','WOODEN_BRIDGE'].join(' ')),
    ]),
    repeat(4,['n']),
		's',
		sr(),
		's',
		's',
		's',
		lr(),
    repeat(3, [
      "w",
      "rbuild title {blu}Kojima{/blu} - Bridge - North - Near Checkpoint",
      "rbuild description " + section_a,
			texture(['AIR','DAMP','FOREST','OUTSIDE','WOODEN_BRIDGE'].join(' ')),
    ]),
		mark('kojima-north-1'),
    repeat(7, [
      "s",
      "rbuild title {blu}Kojima{/blu} - Bridge - North - Checkpoint",
      "rbuild description " + section_a,
			texture(['AIR','DAMP','FOREST','OUTSIDE','WOODEN_BRIDGE'].join(' ')),
    ]),
		lr(),
    repeat(3, [
      "w",
      "rbuild title {blu}Kojima{/blu} - Bridge - North - Outer Bridge",
      "rbuild description " + section_a,
			texture(['AIR','DAMP','FOREST','OUTSIDE','WOODEN_BRIDGE'].join(' ')),
    ]),
    repeat(16, [
      "e",
      "rbuild title {blu}Kojima{/blu} - Bridge - North - Outer Bridge",
      "rbuild description " + section_a,
			texture(['AIR','DAMP','FOREST','OUTSIDE','WOODEN_BRIDGE'].join(' ')),
    ]),
		go('kojima-north-1'),
		repeat(3,['s']),
		er(),
		mark('kojima-bridge-northwest-1'),
    repeat(6, [
      "w",
      "rbuild title {blu}Kojima{/blu} - Bridge - Northwest - Bridge",
      "rbuild description " + section_a,
			texture(['AIR','DAMP','FOREST','OUTSIDE','WOODEN_BRIDGE'].join(' ')),
    ]),
		tr(),
		repeat(3,['e']),
    repeat(2, [
      "s",
      "rbuild title {blu}Kojima{/blu} - Bridge - Northwest - Bridge",
      "rbuild description " + section_a,
			texture(['AIR','DAMP','FOREST','OUTSIDE','WOODEN_BRIDGE'].join(' ')),
    ]),
    repeat(4, [
      "w",
      "rbuild title {blu}Kojima{/blu} - Bridge - Northwest - Bridge",
      "rbuild description " + section_a,
			texture(['AIR','DAMP','FOREST','OUTSIDE','WOODEN_BRIDGE'].join(' ')),
    ]),
		tr(),
		k(2,'s'),
		sr(),
		k(2,'w'),
		lr(),
		k(2,'s'),
		lr(),
		'n',
		sr(),
		's',
		k(3,'e'),
		'w',
		lr(),
		'w',
		k(3,'s'),
		lr(),
		k(5,'e'),
		lr(),
		k(3,'s'),
		g(3,'n'),
		k(3,'n'),
		lr(),
		k(2,'e'),
		er(),
		k(3,'e'),
		er(),
		k(4,'e'),
		g(4,'w'),
		k(3,'s'),
		er(),
		'n',
		k(2,'w'),
		k(1,'s'),
		er(),
		k(1,'s'),
		// inside first northern entrance to KOJIMA CORP
		kojima.mark_north_entrance('west'),
		g(2,'n'),
		g(2,'e'),
		g(1,'s'),
		k(1,'s'),
		kojima.mark_north_entrance('east'),
		g(4,'n'),
		g(5,'w'),
		g(3,'s'), // JUNCTION A
		kojima.mark_junction('A'),
		k(2,'w'),
		k(3,'s'),
		sr(),
		k(2,'w'),
		lr(),
		k(2,'s'),
		sr(),
		k(2,'s'),
		kojima.mark_west_entrance('north'),
		er(),
		k(3,'w'),
		kojima.mark_junction('B'),
		lr(),
		k(2,'n'),
		sr(),
		k(1,'n'),
		k(3,'w'),
		kojima.mark_zone_to('x1'),
		kojima.go_to('junction','B'),
		k(2,'s'),
		sr(),
		k(1,'w'),
		lr(),
		k(2,'w'),
		kojima.mark_zone_to('x2'),
		g(3,'e'),
		k(1,'s'),
		kojima.mark_drop_to('d3'),
		k(3,'s'),
		lr(),
		k(2,'e'),
		lr(),
		k(5,'s'),
		kojima.mark_zone_to('x3'),
		kojima.go_to('junction','B'),
		g(3,'e'),
		
	]);
  var inside_kojima_corp = [
		"The inside of {red}Kojima Corp{/red} looks and feels like an office turned ",
		"into a permanent underground bunker. The reinforced walls have burn marks. ",
		"There are footprints of heavy duty boots and spent shell casings lining the ",
		"metal floors. Cameras built in to the walls sit behind thick reinforced glass. ",
		"You feel like you're being watched. Each step you take makes a clunking sound ",
		"against the metal floor. You step over ruined concrete and tracked-in mud every ",
		"couple of steps. Tall narrow glass windows are placed very high near the ceiling.",
	].join('');
	k = function(times,dir){
    return repeat(times, [
      dir,
      "rbuild title {blu}Kojima{/blu} - Inside Kojima Corp",
      "rbuild description " + inside_kojima_corp,
			texture(['ROOFTOP','INSIDE','METAL_WALL','METAL_FLOORS','GLASS_WINDOWS',].join(' ')),
    ]);
	}
	var mysterious_chest = [
		"The room is empty except for a large unlocked metal chest in the middle of ",
		"the room. It looks like a militaristic container used to transport ammunition ",
		"or sensitive technology across long distances. You might be able to open it.",
	].join('');
	var payload = function(times,dir,rarity,levels,count){
    return repeat(times, [
      dir,
      "rbuild title {blu}Kojima{/blu} - Mysterious Payload",
      "rbuild description " + mysterious_chest,
			texture(['ROOFTOP','INSIDE','METAL_WALL','METAL_FLOORS',].join(' ')),
  		"admin:loot:payload clear",
  		"admin:loot:payload new " + rarity + " " + levels + " " + count,
    ]);
	}
	var ammo_container = [
		"The room is empty except for a tall ammunition container that happens ",
		"to be unlocked. The size of the container is massive and could potentiall ",
		"hold very powerful items,",
	].join('');
	var killer_ammo = function(times,dir,rarity,levels,count){
    return repeat(times, [
      dir,
      "rbuild title {blu}Kojima{/blu} - An ammunition container",
      "rbuild description " + ammo_container,
			texture(['ROOFTOP','INSIDE','METAL_WALL','METAL_FLOORS',].join(' ')),
  		"admin:loot:ammo clear",
  		"admin:loot:ammo new " + rarity + " " + levels + " " + count,
    ]);
	}

	run([
		k(1,'e'),
		kojima.mark_inside('west'),
		k(1,'e'),
		k(2,'n'),
		lr(),
		k(7,'e'),
		g(7,'w'),
		k(2,'e'),
		sr(),
		k(2,'e'),
		sr(),
		k(1,'e'),
		lr(),
		kojima.mark_junction('C'),
		k(3,'w'),
		payload(1,'s','UNCOMMON','20-30','1'),
		kojima.go_to('junction','C'),
		k(2,'n'),
		payload(1,'w','UNCOMMON,RARE','20-30','1'),
		'e',
		g(2,'s'),
		k(3,'s'),
		payload(1,'w','UNCOMMON,RARE,LEGENDARY','20-35','1-3'),
		'e',
		sr(),
		k(3,'s'),
		killer_ammo(1,'w','UNCOMMON,RARE,LEGENDARY','20','1-2'),
		'e',
		k(3,'s'),
		k(4,'e'),
		k(2,'n'),
		k(2,'w'),
		//TODO: rope_drop(), for zip-lining
		k(2,'e'),
		k(3,'n'),
		k(1,'e'),
		payload(1,'e','UNCOMMON,RARE,LEGENDARY','20-35','1-3'),
		payload(1,'s','UNCOMMON,RARE,LEGENDARY','20-35','1-3'),
		payload(1,'s','UNCOMMON,RARE,LEGENDARY','20-35','1-3'),
		g(2,'n'),
		g(2,'w'),
		k(2,'n'),
		k(5,'e'),
		g(1,'w'),
		k(2,'s'),
		payload(1,'e','UNCOMMON,RARE,LEGENDARY','20-35','1-3'),
		k(3,'s'),
		payload(1,'s','UNCOMMON,RARE,LEGENDARY','20-35','1-3'),
		kojima.go_to('junction','C'),
		k(2,'e'),
		k(2,'n'),
		k(3,'e'),
		k(1,'s'),
		killer_ammo(1,'s','UNCOMMON,RARE,LEGENDARY','20-35','1-2'),
		k(1,'e'),
		payload(1,'e','UNCOMMON,RARE,LEGENDARY','20-35','1-3'),
		k(1,'e'),
		killer_ammo(1,'e','UNCOMMON,RARE,LEGENDARY','20-35','1-2'),
		k(4,'n'),
		killer_ammo(1,'n','UNCOMMON,RARE,LEGENDARY','20-35','1-2'),
		k(3,'w'),
		killer_ammo(1,'w','UNCOMMON,RARE,LEGENDARY','20-35','1-2'),
		g(4,'e'),
		g(5,'s'),
		g(4,'w'),
		kojima.go_to('junction','C'),
		'rbuild pave off',
		'rbuild save-paved 0',
	]);
	
  return true;
}
function build_atrocity_mp5_contract() {
  var give_items_vnum = 5;
  build_contract([
    {
      vnum: contracts.atrocity_mp5_contract,
      title: "Atrocity MP5 Contract.",
      description: [
        "Good evening, soldier.\r\n",
				"\r\n",
				"The recon team that is closely working with our R&D branch of the {yel}TRITON{/yel} laboratory\r\n",
				"has recently uncovered the location of a high powered {grn}MP5 sub machine gun{/grn}.\r\n",
				"You will need to find several pieces of machinery used to construct the MP5.\r\n",
				"\r\n",
				"First, you will need find {grn}TEN{/grn} unmodified {grn}MP5's{/grn}.\r\n",
				"It is our understanding that the {grn}Kidnappers{/grn} will be armed with\r\n",
				"an MP5.\r\n",
				"\r\n",
				"One of the components of the Atrocity MP5 is taken from a {grn}G36C Assault Rifle{/grn}.\r\n",
				"You will need to bring back {grn}TEN{/grn} {grn}G36C Assault Rifles{/grn}.\r\n",
				"\r\n",
        "{yel}Here are the contract instructions:{/yel}\r\n",
        "{yel}1) {grn}Acquire {yel}TEN{/yel}{grn} MP5's.{/grn}\r\n",
        "{yel}2) {grn}Acquire {yel}TEN{/yel}{grn} G36C's.{/grn}\r\n",
        "{yel}3) {grn}Bring back all items to the {yel}TRITON{/yel} Labs scientist.{/grn}\r\n",
      ].join(""),
      steps: [
				{
          task_type: "GOAL_GIVE",
          task_target: "TARGET_MOB",
          mob_vnum: scripted.triton.scientist.mob,
          reward_xp: 1150,
          reward_money: 1150,
          quota: 10,
          yaml: "rifle/mp5.yml",
          description: [
            "Give 10 MP5's to the TRITON Labs Scientist\r\n",
          ].join(""),
        },
				{
          task_type: "GOAL_GIVE",
          task_target: "TARGET_MOB",
          mob_vnum: scripted.triton.scientist.mob,
          reward_xp: 1150,
          reward_money: 1150,
          quota: 10,
          yaml: "rifle/g36c.yml",
          description: [
            "Give 10 G36C's to the TRITON Labs Scientist\r\n",
          ].join(""),
        }
      ],
    },
  ]);
  var greet_vnum = 4;
  var convnum = contracts.atrocity_mp5_contract;

  delete_scripted_sequence_by_vnum(greet_vnum);
  delete_scripted_steps_by_sequence_vnum(greet_vnum);

  delete_scripted_sequence_by_vnum(give_items_vnum);
  delete_scripted_steps_by_sequence_vnum(give_items_vnum);

  register_scripted_sequence(
    greet_vnum /** virtual number of this sequence */,
    [
      {
        wait_ticks: 3,
        dialogue: "Great, give me the 20 items and I can manufacture the {grn}Atrocity MP5{/grn} for you.\r\n",
        type: "dialogue",
        mob: scripted.triton.mob,
        room: scripted.triton.room,
			},
    ]
  );
  register_contract_step_callback(
    convnum /** contract vnum */,
    "GOAL_GIVE" /** task type */,
    "TARGET_MOB" /** task target */,
    scripted.triton.room /** target vnum (room vnum in this case) */,
    greet_vnum /** scripted sequence vnum */
  );
  register_scripted_sequence(
    give_items_vnum /** virtual number of this sequence */,
    [
      {
        wait_ticks: 3,
        dialogue: "That's quite a lot of items, I must admit. Though, I think you'll be satisfied with " + 
				"the result. We have put many, MANY man hours into the production of such a weapon...",
        mob: scripted.triton.mob,
        room: scripted.triton.room,
        type: "say",
      },
      {
        wait_ticks: 3,
        type: "mob_gives_yaml",
        mob: scripted.triton.mob,
        room: scripted.triton.room,
        yaml: "rifle/atrocity-mp5.yml",
        quantity: 1,
      },
      {
        wait_ticks: 3,
        dialogue:
          "Do let me know how this works out for you.",
        mob: scripted.triton.mob,
        room: scripted.triton.room,
        type: "say",
      },
      {
        wait_ticks: 10,
        dialogue: "$n rubs $s temples and forces a smile...",
        mob: scripted.triton.mob,
        room: scripted.triton.room,
        type: "act",
      },
      {
        wait_ticks: 3,
        dialogue: "$n cleans $s glasses and sighs in exhaustion.",
        mob: scripted.triton.mob,
        room: scripted.triton.room,
        type: "act",
      },
      {
        wait_ticks: 3,
        dialogue:
          "I must get back to my work unfortunately.",
        mob: scripted.triton.mob,
        room: scripted.triton.room_north,
        type: "say",
      },
    ]
  );
  register_contract_step_callback(
    convnum /** contract vnum */,
    "GOAL_FIND" /** task type */,
    "TARGET_ROOM" /** task target */,
    scripted.triton.room /** target vnum (room vnum in this case) */,
    give_items_vnum /** scripted sequence vnum */
  );
}
function do_kojima(){
	do_kojima_corp_north();
	build_tengu_trooper();
	build_tengu_lancer();
	build_tengu_scout();
	build_tengu_eviscerator();
}
function do_goat(){
	var GOAT_VNUM = 1;
  var mobs = {
    goat: {
      mbuild: {
        attr: {
          exp: 8950,
          virt: GOAT_VNUM,
          mana: 2245,
          max_mana: 2245,
          hit: 65000,
          max_hit: 65000,
          move: 65000,
          max_move: 65000,
          damroll: 930,
          gold: 250,
          level: 100,
          weight: 1,
          height: 1,
          strength: 255,
          strength_add: 255,
          intelligence: 255,
          dexterity: 255,
          constitution: 255,
          electronics: 255,
          chemistry: 255,
          strategy: 255,
          marksmanship: 255,
          sniping: 255,
          weapon_handling: 255,
          demolitions: 255,
          armor: 255,
          medical: 255,
          charisma: 255,
          damnodice: 255,
          damsizedice: 255,
          name: "A cross-eyed goat",
          sex: "MALE",
          action: "ISNPC HELPER AGGR_GOOD",
          short_description: "A cross-eyed goat",
          long_description: "A cross-eyed goat",
          description: "A cross-eyed goat glares at you and the floor simultaneously",
					raid: 0,
        } /** end attr */,
        targets: "PLAYERS",
        "extended-type": "GOAT",
        "roam-pattern": "{*}",
      },
      meqbuild: {
        name: "goat",
        positions: {
          wield: "rifle/orthos-aug-a3.yml",
					secondary: "rifle/orthos-aug-a3.yml",
          //hands: "armor/mp-enforcer-gloves.yml",
          //body: "armor/vulture-ballistic-vest.yml",
          //legs: "armor/titan-shin-guards.yml",
          //feet: "armor/xm50-ultralight-boots.yml",
        },
      },
      zbuild: {
        command: "mob",
        zone_id: 0,
        mob_vnum: GOAT_VNUM,
        room_vnum: [
					128, // SPAWN
				],
        max: 1,
        if_flag: 0,
      },
    },
  };
	auto_feed_mob_list(mobs);
}
var od_market = {
	south_start: 'north-abbott-outdoor-market-start',
	walkway_north: 'north-abbott-outdoor-market-walkway-north',
	walkway_south: 'north-abbott-outdoor-market-walkway-south',
	nu_toxin_dealer_rooms: [],
	produce_vendor_rooms: [],
	nu_toxin_marker: function(ctr) {
		return 'north-abbott-outdoor-nu-toxin-dealer-' + ctr;
	},
	produce_vendor_marker: function(ctr) {
		return 'north-abbott-outdoor-produce-vendor-' + ctr;
	},
	get_produce_vendor_rooms: function(){
		return od_market.produce_vendor_rooms;
	},
	get_nu_toxin_dealer_rooms: function(){
		return od_market.nu_toxin_dealer_rooms;
	},
};
function do_north_abbott_outdoor_market() {
	od_market.nu_toxin_dealer_rooms = [];
	od_market.produce_vendor_rooms = [];
	var south_entrance= [
		"The asphault turns into reddish-brown sand mixed with dirt and debris. ",
		"A sign above your head says {grn}'North Abbott Outdoor Market'{/grn}. ",
	].join('');
	var fruit_lane = [
		"Vendors are here desperately trying to get you to buy fruits and vegetables. ",
		"You notice flies on everything and wonder how it is they even sell anything ",
		"with the amount of traffic and insects. ",
	].join('');
	var infested_twice = [
		"You are now in the innermost part of the outdoor market. There is no clear way to ",
		"leave the market other than going back exactly how you came. You notice there are ",
		"some people here that look like they don't belong. Somehow you get the feeling ",
		"the hoodlums are running the show.",
	].join('');
	var walkway = [
		"Each booth sells a variety of things, but the most common product happens to be ",
		"fruits and vegetables. A few tents sell used clothing and electronics. ",
	].join('');
	var southern_road = [
		"The asphalt turns from well-paved to almost non-existent. The North Abbott Outdoor ",
		"Market is the result of funds being funneled into a legitimate source but siphoned out ",
		"by intimidation tactics and the threat of violence. It is largely known that the ",
		"Outdoor Market is run by criminals but due to it's proximity to low income neighborhoods ",
		"nobody has any incentive to clean it up. ",
	].join('');
	var nu_toxin_dealer = [
		"Nu-Toxin is a controversial substance that is always skirting the line between ",
		"legality and banned substance. The effects of the drink are nothing short of miraculous ",
		"if you look at it from a standard scientific point of view. Many athletes and ",
		"partying teenagers have lost their lives to the controversial substance. Ironically enough, ",
		"the substance can be used as a Performance Enhancing Drug. Currently, the molecular structure ",
		"of Nu-Toxin makes it legal, but who knows when it will be deemed illegal and it's creators ",
		"release a new form with a slightly altered genetic makeup. Stock up!",
	].join('');
  var produce_vendor = [
		"Fruits and vegetables are probably the only thing you can buy at the Outdoor Market that hasn't ",
		"been tainted by crime. A good helping of flies and insects roam the area. ", 
	].join('');

	var r = function (times,dir){
		return repeat(times,[dir]);
	};
	var k = function(times,dir){
    return repeat(times, [
      dir,
      "rbuild title {blu}North Abbott{/blu} - Southern Road",
      "rbuild description " + southern_road,
			texture(['AIR','DRY','OUTSIDE','TRASHY','STREET','GHETTO','GANG_INFESTED',]),
    ]);
	}
	var g = function(times,dir){
		return repeat(times,[dir]);
	}
  //USEFUL: var cofob_center_vnum = exec("print_vnum " + cofob.center);
  //USEFUL: go(cofob.east_exit),
  run([
		'recall',
		g(2,'n'),
		g(2,'e'),
		g(9,'n'),
		g(6,'e'),
		g(16,'n'),
    mark(od_market.south_start),
    "rbuild pave continue",
		k(10,'n'),
	]);
	var mid_walk = function(times,dir){
    return repeat(times, [
      dir,
      "rbuild title {blu}North Abbott Outdoor Market{/blu} - Middle Walkway",
      "rbuild description " + infested_twice,
			texture(['AIR','DRY','OUTSIDE','TRASHY','STREET','GHETTO','GANG_INFESTED',]),
    ]);
	}

	var ctr = 0;
	var mark_nu_toxin = function(){
		var r =	od_market.nu_toxin_marker(++ctr);
		return mark(r);
	}
	var produce_ctr = 0;
	var mark_produce_vendor = function(){
		var r =	od_market.produce_vendor_marker(++produce_ctr);
		return mark(r);
	}
	var vendor = function(dir){
		return repeat(1,[
			dir,
      "rbuild title {blu}North Abbott{/blu} - Produce Vendor",
      "rbuild description " + produce_vendor,
			texture(['OUTSIDE','AIR','DRY','TENT','TRASHY','STREET','GHETTO','GANG_INFESTED',]),
			mark_produce_vendor(),
		]);
	};
	var nu_toxin = function(dir){
		return repeat(1,[
			dir,
      "rbuild title {blu}North Abbott{/blu} - Nu Toxin Dealer",
      "rbuild description " + nu_toxin_dealer,
			texture(['OUTSIDE','AIR','DRY','TENT','TRASHY','STREET','GHETTO','GANG_INFESTED',]),
			mark_nu_toxin(),
		]);
	};
	run([
		r(8,'s'),
		mark(od_market.walkway_south),
		mid_walk(10,'w'),

		repeat(10,['e']),
		r(4,'n'),
		mark(od_market.walkway_north),
		mid_walk(10,'w'),

		go(od_market.walkway_south),
		repeat(3,['w']),
		vendor('n'),
		's',
		repeat(3,['w']),
		vendor('n'),
		's',

		repeat(3,['w']),
		vendor('n'),
		's',

		go(od_market.walkway_north),
		repeat(2,['w']),
		vendor('s'),
		'n',

		repeat(2,['w']),
		vendor('s'),
		'n',

		repeat(2,['w']),
		vendor('s'),
		'n',


		repeat(2,['w']),
		nu_toxin('s'),
		'n',
		
		'rbuild pave off',
		'rbuild save-paved 0',
	]);
	for(var i=1; i <= ctr;i++){
		od_market.nu_toxin_dealer_rooms.push(exec('print_vnum ' + od_market.nu_toxin_marker(i)));
	}
	for(var i=1; i <= produce_ctr;i++){
		od_market.produce_vendor_rooms.push(exec('print_vnum ' + od_market.produce_vendor_marker(i)));
	}
	var NU_TOXIN_DEALER_VNUM = 2;
	var PRODUCE_VENDOR_VNUM = 3;
	var PRODUCE_HOODLUM_VNUM = 4;
	var vnums = [
		NU_TOXIN_DEALER_VNUM,
		PRODUCE_VENDOR_VNUM,
		PRODUCE_HOODLUM_VNUM,
	];
  clear_mob_zone_commands_for(vnums);
  var mobs = {
    nu_toxin_dealer: {
      mbuild: {
        attr: {
          exp: 85,
          virt: NU_TOXIN_DEALER_VNUM,
          mana: 30,
          max_mana: 30,
          hit: 450,
          max_hit: 450,
          move: 10,
          max_move: 10,
          damroll: 4,
          gold: 2950,
          level: 10,
          weight: 5,
          height: 5,
          strength: 8,
          strength_add: 8,
          intelligence: 0,
          dexterity: 8,
          constitution: 8,
          electronics: 0,
          chemistry: 0,
          strategy: 1,
          marksmanship: 2,
          sniping: 2,
          weapon_handling: 2,
          demolitions: 0,
          armor: 5,
          medical: 0,
          charisma: 25,
          damnodice: 10,
          damsizedice: 10,
          name: "A {yel}Nu-Toxin{/yel} dealer",
          sex: "MALE",
          action: "ISNPC HELPER AGGR_GOOD",
          short_description: "A {yel}Nu-Toxin{/yel} dealer",
          long_description: "A {yel}Nu-Toxin{/yel} dealer",
          description: "A {yel}Nu-Toxin{/yel} dealer sizes you up. He looks confident he could take you.",
					raid: 0,
        } /** end attr */,
        targets: "PLAYERS",
        "extended-type": "SHOPLIFTER",
        "roam-pattern": "Nu-Toxin",
      },
      meqbuild: {
        name: "nu-toxin-dealer",
        positions: {
          wield: "rifle/tekdc9.yml",
          hands: "armor/mp-enforcer-gloves.yml",
          body: "armor/basic-ballistic-vest.yml",
          legs: "armor/black-jeans.yml",
          feet: "armor/atom-fade-shoes.yml",
        },
      },
      zbuild: {
        command: "mob",
        zone_id: 0,
        mob_vnum: NU_TOXIN_DEALER_VNUM,
        room_vnum: od_market.get_nu_toxin_dealer_rooms(),
        max: 1,
        if_flag: 0,
      },
    },
    produce_hoodlum: {
      mbuild: {
        attr: {
          exp: 85,
          virt: PRODUCE_HOODLUM_VNUM,
          mana: 30,
          max_mana: 30,
          hit: 550,
          max_hit: 550,
          move: 11,
          max_move: 15,
          damroll: 8,
          gold: 2950,
          level: 11,
          weight: 5,
          height: 5,
          strength: 10,
          strength_add: 10,
          intelligence: 0,
          dexterity: 10,
          constitution: 10,
          electronics: 0,
          chemistry: 0,
          strategy: 5,
          marksmanship: 10,
          sniping: 10,
          weapon_handling: 10,
          demolitions: 0,
          armor: 10,
          medical: 0,
          charisma: 25,
          damnodice: 15,
          damsizedice: 10,
          name: "A thug",
          sex: "MALE",
          action: "ISNPC HELPER AGGR_GOOD",
          short_description: "A thug",
          long_description: "A thug",
          description: "A thug sizes you up. He looks confident he could take you.",
					raid: 0,
        } /** end attr */,
        targets: "PLAYERS",
        "extended-type": "SHOPLIFTER",
      },
      meqbuild: {
        name: "north-abbott-thug",
        positions: {
          wield: "rifle/tekdc9.yml",
          hands: "armor/mp-enforcer-gloves.yml",
          body: "armor/basic-ballistic-vest.yml",
          legs: "armor/black-jeans.yml",
          feet: "armor/atom-fade-shoes.yml",
        },
      },
      zbuild: {
        command: "mob",
        zone_id: 0,
        mob_vnum: PRODUCE_HOODLUM_VNUM,
        room_vnum: od_market.get_produce_vendor_rooms(),
        max: 2,
        if_flag: 0,
      },
    },
    produce_vendor: {
      mbuild: {
        attr: {
          exp: 15,
          virt: PRODUCE_VENDOR_VNUM,
          mana: 15,
          max_mana: 15,
          hit: 150,
          max_hit: 150,
          move: 10,
          max_move: 10,
          damroll: 4,
          gold: 110,
          level: 10,
          weight: 5,
          height: 5,
          strength: 8,
          strength_add: 8,
          intelligence: 0,
          dexterity: 8,
          constitution: 8,
          electronics: 0,
          chemistry: 0,
          strategy: 1,
          marksmanship: 2,
          sniping: 2,
          weapon_handling: 2,
          demolitions: 0,
          armor: 5,
          medical: 0,
          charisma: 25,
          damnodice: 10,
          damsizedice: 10,
          name: "A produce vendor",
          sex: "MALE",
          action: "ISNPC HELPER AGGR_GOOD",
          short_description: "A produce vendor",
          long_description: "A produce vendor",
          description: "A produce vendor attempts to sell you food",
					raid: 0,
        } /** end attr */,
        targets: "PLAYERS",
        "extended-type": "INNOCENT",
      },
      meqbuild: {
        name: "produce-vendor",
        positions: {
          //wield: "rifle/tekdc9.yml",
          //hands: "armor/mp-enforcer-gloves.yml",
          //body: "armor/basic-ballistic-vest.yml",
          legs: "armor/black-jeans.yml",
          feet: "armor/atom-fade-shoes.yml",
        },
      },
      zbuild: {
        command: "mob",
        zone_id: 0,
        mob_vnum: PRODUCE_VENDOR_VNUM,
        room_vnum: od_market.get_produce_vendor_rooms(),
        max: 1,
        if_flag: 0,
      },
    },
  };
	auto_feed_mob_list(mobs);
  return true;
}

function add_nu_toxin_loot(){
	run([
		'goto north-abbott-outdoor-market-walkway-south',
		'w',
		'w',
		'w',
	]);
}

function main() {
  funcs.always();
	//run([
	//	'goto north-abbott-outdoor-market-walkway-south',
	//	'w',
	//	'w',
	//	'w',
	//]);
	//do_north_abbott_outdoor_market();
}

main();
