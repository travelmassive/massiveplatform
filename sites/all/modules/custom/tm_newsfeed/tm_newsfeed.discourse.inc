<?php

// tm_newsfeed.discourse.inc - discourse forum methods

function tm_newsfeed_discourse_fetch() {

	global $conf;

	// check cache
	$cache_key = 'tm-newsfeed-discourse-fetch';
	if ($conf["tm_newsfeed_discourse_cache"] > 0) {
		$cache = cache_get($cache_key, 'cache');
		if (!empty($cache)) {
			try {
				return json_decode($cache->data);
			} catch (Exception $e) {
				return array();
			}
		}
	}

	//open connection
	$ch = curl_init();

	// timeout
	$curl_timeout = $conf["tm_newsfeed_curl_timeout"];

	// url
	// use /latest.json api
	$url = $conf["tm_newsfeed_discourse_base_url"] . "/latest.json";

	//set the url, number of POST vars, POST data
	$CURLOPT_SSL_VERIFYHOST = 0; 
	if ($conf["tm_newsfeed_curl_check_certificate"]) {
		$CURLOPT_SSL_VERIFYHOST = 2;
	}
	curl_setopt($ch, CURLOPT_URL, $url);
	curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);

	// basic http auth
	if (isset($conf["tm_newsfeed_discourse_api_userpass"])) {
		curl_setopt($ch, CURLOPT_USERPWD, $conf["tm_newsfeed_discourse_api_userpass"]);
	}
	curl_setopt($ch, CURLOPT_VERBOSE, false);
	curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, $CURLOPT_SSL_VERIFYHOST);
	curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 0); 
	curl_setopt($ch, CURLOPT_TIMEOUT, $curl_timeout); // timeout in seconds

	//execute post
	$result = curl_exec($ch);

	$curl_error = "";
	if ($result === false) {
		$curl_error = "CURL error: " . curl_error($ch);
	}

	//close connection
	curl_close($ch);

	// return api data in json format
	if ($conf["tm_newsfeed_discourse_cache"] > 0) {
		cache_set($cache_key, $result, 'cache', time() + $conf["tm_newsfeed_discourse_cache"]);
	}
	
	// return result
	try {
		return json_decode($result);
	} catch (Exception $e) {
		return array();
	}
	
}

// fetch discourse newsfeed for a user
function tm_newsfeed_discourse_fetch_for_user($user_id) {

	// Fetch results for user
	$results = tm_newsfeed_discourse_fetch();

	return $results;
}

