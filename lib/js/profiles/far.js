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

/**
 * ================================================================================
 * ================================================================================
 * ================================================================================
 * ================================================================================
 */

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

var funcs = {
  always: function () {
    cmd("+impl");
    cmd("+god");
    cmd("+build");
  },
};
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

function mud_date() {
  send(get_month() + " " + get_day() + " " + get_iyear());
}

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

function do_lockers(){
  run(["recall"]);
  create_tier_1_armor_locker();
  create_ammunition_locker();
  create_tier_1_weapons_locker();
}
function main() {
  funcs.always();
}

main();
