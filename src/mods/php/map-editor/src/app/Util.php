<?php

namespace App;

use Redis;
use Illuminate\Http\Request;
use Illuminate\Support\Facades\Log;

class Util
{
	public static function rand($start=0,$finish=32000):string{
		static $call_count = 0;
		if(++$call_count > 3){
			return '';
		}
		if($start < 0){
			$start = 1;
		}
		if($finish > 32000){
			$finish = 32000;
		}
		return self::rand() . \App\Util::to_string([rand($start,$finish),rand($start,$finish),rand($start,$finish)]);
	}

	public static function to_string(array $r): string {
		$s='';
		foreach($r as $i){
			$s .= $i;
		}
		return $s;
	}

	public static function array_repeat(array $arr,int $times): array{
		$foo = [];
		for($i=$times;$i > 0;$i--){
			for($j=0;$j < count($arr);$j++){
				$foo[] = $arr[$j];
			}
		}
		return $foo;
	}

    /**
     * Returns a script tag to redirect the user to the specified page
     * @param $page string URI to redirect to
     */
    public static function scriptRedirect(string $page)
    {
        return '<script type="text/javascript">location.href="' . $page . '";</script>';
    }

    /**
    * @return string | a var dump
    * @param mixed | variable to be dumped
    * $param pretty | wraps in pre tags if pretty
    */
    static function var_dump_str($mixed = null, $pretty = 0) {
		Header::header('Content-type: text/plain');
        ob_start();
        echo '<pre>';
        var_dump($mixed);
        echo '</pre>';
        $content = ob_get_contents();
        ob_end_clean();
        return $content;
    }
    /**
     *   Function arrayGet: Processes laravel's array_get, if default is returned, logs it
     *   @param $array array
     *   @param $item string
     *   @param $default string|number|null
    **/
    public static function arrayGet($array, $item, $default = null)
    {
        $rtn = array_get($array, $item, $default);
        if ($default === $rtn) {
            $info = Util::getPrevLineFile(2);
            extract($info); //grab line, file
            Util::monoLog("arrayGet returned default at {$file}:{$line}");
        }
        return $rtn;
    }
    /**
     *   Function MonoLog: simple wrapper around Laravel's MonoLog
     *   @param $message string
     *   @param $type string
     *   $type options
     *     'emergency';
     *     'alert';
     *     'critical';
     *     'error';
     *     'warning';
     *     'notice'; DEFAULT
     *     'info';
     *     'debug';
    **/
    public static function monoLog($message, $type = 'notice')
    {
        if (is_array($type)) {
            $args = $type;
            $type = array_get($type, 'type', 'notice');
        } else {
            $args = [];
        }
        $heading = array_get($args, 'heading',
            array_get($args, 'log', 'monolog')
        );
        $message = strtoupper($heading . ': ' . $message);
        if ($type == 'critical' || $type == 'emergency') {
            mail('bvfbarten@gmail.com', ucfirst($type) . ' Alert', $message);
            mail('wmerfalen@gmail.com', ucfirst($type) . ' Alert', $message);
        }
        Log::{$type}($message);
    }
    public static function isWwwDomain()
    {
        $server = self::serverName();
        foreach (self::$stagingRegex as $i => $k) {
            if (preg_match("|" . $k . "|", $server)) {
                return false;
            }
        }
        if (preg_match("|^www\.|", $server)) {
            return true;
        }
        return false;
    }

    public static function remoteIp($default=null)
    {
        if (isset($_SERVER['REMOTE_ADDR'])) {
            return $_SERVER['REMOTE_ADDR'];
        } elseif ($default) {
            return $default;
        } else {
            return "127.0.0.1";
        }
    }


    public static function isHttpsException()
    {
        $server = self::serverName();
        foreach (self::$stagingRegex as $i => $k) {
            if (preg_match("|" . $k . "|", $server)) {
                return true;
            }
        }
        return false;
    }

    public static function realServerName()
    {
        $serverName = self::serverName();
        foreach (self::$stagingRegex as $i => $k) {
            $serverName = preg_replace("|" . $k . "|", "", $serverName);
        }
        return $serverName;
    }

    /*

       $example = [
            [0] =>
                [
                    'id' => 1,
                    'custom_nav' => 'foobar.html'],
                    ..
                ]
       ];

       Util::keyInArray('custom_nav',$example); //returns true

       returns: true
    */

    public static function keyInArray(string $key, array $array)
    {
        foreach ($array as $index => $value) {
            if (is_array($value)) {
                if (self::keyInArray($key, $value)) {
                    return true;
                }
            } else {
                if ($index == $key) {
                    return true;
                }
            }
        }
        return false;
    }


    /*
        $example = [
            [
                'id' => 1110,
                'foo' => 'bar',
                'value' => 'foobar',
                'date' => '10/15/1984'
            ],
            [
                'id' => '200',
                'foo' => 'dot',
                'value' => 'fizzbuzz',
                'date' => '10/02/1961'
            ]
        ];

        $return = Util::flatten(['foo' => 'value'],$example);
        Return now looks like:
            [
                'bar' => 'foobar',
                'dot' => 'fizzbuz'
            ]

    */
    public static function flatten(array $associations, array $records)
    {
        $return = [];
        foreach ($records as $i => $rec) {
            foreach ($associations as $key => $value) {
                if ($value === null) {
                    $return[$key][] = $rec[$key];
                } else {
                    $return[$rec[$key]][] = $rec[$value];
                }
            }
        }
        return $return;
    }

    public static function common(string $type, $category)
    {
        $entity = app()->make('App\Property\Site')->getEntity();
        return $entity->getWebPublicCommon($category);
    }

    public static function isCommandLine()
    {
        if (php_sapi_name() == 'cli') {
            return true;
        } else {
            return false;
        }
    }
    public static function getPrevLineFile($depth = 1)
    {
        $info = debug_backtrace();
        $line = array_get($info, "{$depth}.line");
        $file = array_get($info, "{$depth}.file");
        return compact('line', 'file');
    }
    public static function dd($item)
    {
        if (env('APP_DEBUG')) {
            $info = util::getPrevLineFile();
            $info['item'] = $item;
            dd($info);
        }
    }
    public static function requestUri()
    {
        if (isset($_SERVER['REQUEST_URI'])) {
            return $_SERVER['REQUEST_URI'];
        } else {
            return request()->getRequestUri();
        }
    }


    public static function isDevDomain()
    {
        return preg_match("|^dev\.|", Util::serverName());
    }

    public static function updateIfExists(string $model, array $where, array $setToValue)
    {
        $foo = app()->make($model);
        $row = $foo->where($where)->get();
        if (count($row)) {
            $key = array_keys($setToValue);
            $row = $row->first();
            foreach ($setToValue as $key => $value) {
                $row->$key = $value;
            }
            $row->save();
            return true;
        }
        return false;
    }

    public static function die404($req=null, $exception=null)
    {
        if ($req) {
            //TODO: if preferences to log 404s...
        }
        if ($exception) {
            //TODO: if preferences to log exceptions
        }
        //self::log("404: " . var_export($req,1) . " EXCEPTION: " . var_export($exception,1));
        $site = app()->make('App\Property\Site');
        //TODO: route this stuff through site controller's population methods
        $site->getEntity()->loadLegacyProperty();
        echo view('layouts/' . $site->getEntity()->getTemplateName() . '/404', [
            'entity'=> $site->getEntity(),
            'fsid' => $site->getEntity()->getTemplateName(),
            'page' => '404'
            ]);
        die();
    }

    public static function dieGeneric($req=null, $exception=null)
    {
        $site = app()->make('App\Property\Site');
        $path = "";
        if ($req) {
            $path = $req->path();
        }
        self::log($message = "Generic error: Site:" . $site->getEntity()->property_name . ": Page: {$path}" .
            " Message:" . $exception->getMessage() . "::Code:" .  $exception->getCode() . ":File:" . $exception->getFile() . "::Line:" . $exception->getLine() . "::TraceAsString" .  var_export($exception->getTraceAsString(), 1));
        //TODO: route this stuff through site controller's population methods
        if (ENV("EMAIL_LOGS") == '1') {
            self::log('emailing....');
            try {
                Mailer::queueError('wmerfalen@gmail.com', $site->getEntity()->getLegacyProperty()->url . ": $path -- ERROR", $message, ['matt@marketapts.com']);
            } catch (Exception $e) {
                self::log("ERROR SENDING EMAIL: " . var_export($e, 1));
            }
        }
        $site->getEntity()->loadLegacyProperty();
        echo view('layouts/' . $site->getEntity()->getTemplateName() . '/404', [
            'errorGeneric' => 1,
            'entity'=> $site->getEntity(),
            'fsid' => $site->getEntity()->getTemplateName(),
            'page' => '404'
            ]);
        die();
    }


    public static function isHttp()
    {
        return strcmp("http", $_SERVER['REQUEST_SCHEME']) == 0;
    }

    public static function isFpm()
    {
        return strcmp(php_sapi_name(), env('FPM_NAME')) == 0;
    }

    public static function isPage(string $p)
    {
        return preg_match("|^/$p|", self::requestUri());
    }

    public static function baseUri($inReq, $default=null) : string
    {
        if ($inReq instanceof Request) {
            $req = $inReq->getRequestUri();
        } else {
            $req = $inReq;
        }
        if (preg_match("|^/([a-zA-Z0-9\-\.]+)|", $req, $matches)) {
            return $matches[1];
        } elseif ($default) {
            return $default;
        } else {
            return $req;
        }
    }

    public static function isJson(string $s)
    {
        json_decode($s);
        return (json_last_error() == JSON_ERROR_NONE);
    }
    public static function redisIsNew(string $section)
    {
        if (empty($time = Redis::get(self::redisKey($section) . "_updated"))) {
            return true;
        }
        $bool =  $time > Redis::get(self::redisKey($section) . "_created");
        return $bool;
    }
    public static function redisRefresh()
    {
        foreach (Redis::keys(self::redisKey("*")) as $i => $key) {
            Redis::del($key);
        }
    }

    public static function redisUpdate(string $foo, $bar)
    {
        self::redisSetCreated($foo, time());
        self::redisSetUpdated($foo, time());
        self::redisSet($foo, $bar);
    }

    public static function redisUpdateKeys(array $config)
    {
        //TODO !security this is potentially dangerous. Do this the right way
        $keys = shell_exec("redis-cli --raw keys '" . preg_replace("|[']*|", "", $config['like']) . "' | grep -v \"'\" | grep '_updated'");
        foreach (explode("\n", $keys) as $i => $line) {
            Redis::set($line, time());
        }
    }

    public static function redisSet(string $foo, $bar)
    {
        if (preg_match("|commute\-text|", $foo)) {
            $e = new \Exception;
            file_put_contents(storage_path() . "/logs/redisSet.log", date("Y-m-d H:i:s") . "::{$foo} being set to " . \
                var_export($bar, 1) . " BT: " . var_export($e->getTraceAsString(), 1) . "\n", FILE_APPEND);
        }
        if (is_array($bar)) {
            Redis:;
            set(self::redisKey($foo) . ':array:', '1');
            Redis::set(self::redisKey($foo), self::redisEncode($bar));
            return;
        }
        Redis::set(self::redisKey($foo), $bar);
    }

    public static function log(string $foo, $opts = [])
    {
        self::monoLog($foo, $opts);
    }

    public static function serverName()
    {
        if (isset($_SERVER['SERVER_NAME'])) {
            return $_SERVER['SERVER_NAME'];
        } else {
            return env('BACKUP_BASE_URL');
        }
    }

    /**
    * @param key string Key being called
    * @param callable function to fetch if data is new
    * @param arrayType json_decodes data if set to true
    * @return string|array the fetched value from redis or callable
    */
    public static function redisFetchOrUpdate(string $key, $callable, $arrayType=false)
    {
        if (env("REDIS_ALWAYS_FETCH") === '1') {
            $foo = $callable();
            self::redisUpdate($key, is_array($foo) ? self::redisEncode($foo) : $foo);
            return $foo;
        }
        if (!self::redisIsNew($key)) {
            if ($arrayType) {
                return self::redisDecode(self::redisGet($key));
            } else {
                return self::redisGet($key);
            }
        } else {
            $foo = $callable();
            self::redisUpdate($key, is_array($foo) ? self::redisEncode($foo) : $foo);
            return $foo;
        }
    }

    public static function redisGet(string $foo, $decorate = true)
    {
        if (Redis::get(self::redisKey($foo, $decorate) . ':array:') == '1') {
            return self::redisDecode(Redis::get(self::redisKey($foo, $decorate)));
        }
        return Redis::get(self::redisKey($foo, $decorate));
    }

    public static function redisDeleteRawKey(string $key)
    {
        Redis::del($key);
        Redis::del($key . "_updated");
        Redis::del($key . "_created");
    }

    public static function redisSetCreated(string $foo, $time)
    {
        Redis::set(self::redisKey($foo) . "_created", $time);
    }

    public static function redisSetUpdated(string $section)
    {
        Redis::set(self::redisKey($section) . "_updated", time());
    }

    public static function redisKey(string $foo, $decorate=true)
    {
    	return str_replace("www.", "", Util::serverName()) . ":$foo";
    }

    public static function redisEncode($object)
    {
        return json_encode($object);
    }

    public static function redisDecode($str, $opts=true)
    {
        return json_decode($str, $opts);
    }

    public static function redisGlobalKey(string $key)
    {
        return "global_$key";
    }

    public static function isDev()
    {
        return strcmp(ENV('DEV'), '1') == 0;
    }

    public static function transformFloorplanName(string $name)
    {
        return preg_replace("|[^a-z0-9]+|", "", strtolower($name));
    }

    public static function isResidentPortal()
    {
        return preg_match("|^/resident\-portal/,*|", self::requestUri());
    }

    public static function depluralize(string $s)
    {
        return preg_replace("|[sS]{1}$|", "", $s);
    }

    public static function isHome()
    {
        $req = self::requestUri();
        return (
            preg_match("|^/index|", $req) ||
            preg_match("|^/home|", $req) ||
            preg_match("|^/[\\?]+|", $req)  ||
            preg_match("|^/$|", $req) ||
            strlen($req) == 0
        );
    }

    public static function formatRentPrice(string $price)
    {
        return round($price, 2, PHP_ROUND_HALF_UP);
    }

    public static function emailRegex()
    {
        return '^(?!(?:(?:\x22?\x5C[\x00-\x7E]\x22?)|(?:\x22?[^\x5C\x22]\x22?)){255,})(?!(?:(?:\x22?\x5C[\x00-\x7E]\x22?)|(?:\x22?[^\x5C\x22]\x22?)){65,}@)(?:(?:[\x21\x23-\x27\x2A\x2B\x2D\x2F-\x39\x3D\x3F\x5E-\x7E]+)|(?:\x22(?:[\x01-\x08\x0B\x0C\x0E-\x1F\x21\x23-\x5B\x5D-\x7F]|(?:\x5C[\x00-\x7F]))*\x22))(?:\.(?:(?:[\x21\x23-\x27\x2A\x2B\x2D\x2F-\x39\x3D\x3F\x5E-\x7E]+)|(?:\x22(?:[\x01-\x08\x0B\x0C\x0E-\x1F\x21\x23-\x5B\x5D-\x7F]|(?:\x5C[\x00-\x7F]))*\x22)))*@(?:(?:(?!.*[^.]{64,})(?:(?:(?:xn--)?[a-z0-9]+(?:-[a-z0-9]+)*\.){1,126}){1,}(?:(?:[a-z][a-z0-9]*)|(?:(?:xn--)[a-z0-9]+))(?:-[a-z0-9]+)*)|(?:\[(?:(?:IPv6:(?:(?:[a-f0-9]{1,4}(?::[a-f0-9]{1,4}){7})|(?:(?!(?:.*[a-f0-9][:\]]){7,})(?:[a-f0-9]{1,4}(?::[a-f0-9]{1,4}){0,5})?::(?:[a-f0-9]{1,4}(?::[a-f0-9]{1,4}){0,5})?)))|(?:(?:IPv6:(?:(?:[a-f0-9]{1,4}(?::[a-f0-9]{1,4}){5}:)|(?:(?!(?:.*[a-f0-9]:){5,})(?:[a-f0-9]{1,4}(?::[a-f0-9]{1,4}){0,3})?::(?:[a-f0-9]{1,4}(?::[a-f0-9]{1,4}){0,3}:)?)))?(?:(?:25[0-5])|(?:2[0-4][0-9])|(?:1[0-9]{2})|(?:[1-9]?[0-9]))(?:\.(?:(?:25[0-5])|(?:2[0-4][0-9])|(?:1[0-9]{2})|(?:[1-9]?[0-9]))){3}))\]))$';
    }
}

