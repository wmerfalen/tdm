<?php

/*
|--------------------------------------------------------------------------
| Web Routes
|--------------------------------------------------------------------------
|
| Here is where you can register web routes for your application. These
| routes are loaded by the RouteServiceProvider within a group which
| contains the "web" middleware group. Now create something great!
|
*/

use \App\Util as Util;
Route::get('/', function () {
	//Util::redisSet('phrase_count',3);
	//Util::redisSet('phrases_correct',0);
	return view('smith');
});

Route::get('/reset',function(){
	foreach(['3s','as','ob'] as $key){
		Util::redisSet("phrase_{$key}",0);
	}
});

Route::post('/s', function () {
	$f = $_POST['phrase'] ?? 'foo';
	if($f === 'foo'){
		echo "Something seems to be wrong. Please try again.";
		return;
	}else{
		$phrase = strtolower(str_replace(' ','',$f));
		echo $phrase;
		if($phrase == '3stooges'){
			Util::redisSet('phrase_3s',1);
		}
		if($phrase == 'agentsmith'){
			Util::redisSet('phrase_as',1);
		}
		if($phrase == 'ohbrother'){
			Util::redisSet('phrase_ob',1);
		}
	}
	$ctr = 0;
	foreach(['3s','as','ob' ] as $key ){
		if(Util::redisGet("phrase_{$key}") == 1){
			$ctr++;
		}
	}
	if($ctr == 3){
		echo '<h1>';
		echo "Congratulations, you have all phrases correct. Text Will and he will send you a gift card to your favorite restaurant. We love you.";
		echo '</h1>';
	}else{
		echo "<h1>";
		echo "You got $ctr out of 3 phrases correct.";
		echo '</h1>';
	}
	echo $f;
});
