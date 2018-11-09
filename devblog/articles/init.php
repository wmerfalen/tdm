<?php namespace siege; require_once(dirname(__FILE__) . '/../vendor/autoload.php');
use siegemud\src\Page as Page;
function start($article_id,$article_title){
	Page::fetch_or_capture($article_id,$article_title);
}
function end(){
	Page::get()
		->end_capture()
		->print(); 
}
