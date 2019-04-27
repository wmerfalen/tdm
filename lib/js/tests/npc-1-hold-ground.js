require_js('tests/test.js');

function test_main(sniper_id){
	/** First we need to give the player that's logged in a sniper rifle */
	/** But in order to do that, we need to make our player a god so we can
	 * give him arbitrary amounts of ammo and a rifle
	 */
	var output = [
		cmd_exec('+god'),
		cmd_exec('+impl')
	];
	run_test('obuild instantiate ' + sniper_id,'obuild-instantiate-sniper-by-id');
	run_test("mbuild new","mbuild-new");
}

