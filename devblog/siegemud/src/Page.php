<?php namespace siegemud\src;
require(implode(DIRECTORY_SEPARATOR,
	[dirname(__FILE__), '..','..','vendor','autoload.php'
]));
use \mentoc\Page as MentocPage;
use \mentoc\Config as MentocConfig;
class Page {
	protected static $SLASH = DIRECTORY_SEPARATOR;
	protected static $m_title = null;
	protected static $m_template = null;
	public static $page = null;
	public static $config_file = null;
	private static $instance = null;
	public static function bootstrap() : self {
		$view_dir = dirname(__FILE__) . '/../../views/';
		$cache_dir = dirname(__FILE__) . '/../../views/.cache/';
		if(!file_exists($view_dir)){
			mkdir($view_dir,0755,true);
		}
		if(!file_exists($cache_dir)){
			mkdir($cache_dir,0755,true);
		}
		MentocConfig::set('view_dir',$view_dir);
		MentocConfig::set('cache_dir',$cache_dir);
		return new self();
	}

	public $config = [
		'view_dir' => null,
		'cache_dir' => null
	];

	public function __construct(){
		$this->config['view_dir'] = MentocConfig::get('view_dir');
		$this->config['cache_dir'] = MentocConfig::get('cache_dir');
	}

	public function die(){
		die();
	}

	public function start_capture() : self {
		ob_start();
		return $this;
	}

	public function end_capture() : self {
		file_put_contents($this->config['view_dir'] . '/'
		 	. $this->config['article'], ob_get_contents(),0);
		ob_clean();
		return $this;
	}

	public static function fetch_or_capture($article,$title) : self {
		self::get();
		if(file_exists(self::get()->config['view_dir'] . '/' . $article)){
			include_once(self::get()->config['view_dir'] . '/' . $article);
			die();
		}else{
			self::get()->config['article'] = $article;
			self::get()->start_capture();
		}
		return self::get();
	}

	public function exists($article,&$text=null) : bool {

	}

	public function signature(): string {
		return 'William Merfalen [date:' . self::get()->config['article'] . ']';
	}

	public function print() {
		include_once(self::get()->config['view_dir'] . '/' . $article);
	}

	public function article($title) : self {

	}

	public static function get() : self {
		if(self::$instance instanceof self){
			return self::$instance;
		}else{
			return self::$instance = self::bootstrap();
		}
	}

};

