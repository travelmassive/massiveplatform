<?php

// tm_newsfeed.marketplace.inc - marketplace methods

function tm_newsfeed_marketplace_fetch($country) {

	global $conf;

	// check cache
	$cache_key = 'tm-newsfeed-marketplace-' . str_replace(" ", "-", strtolower($country));
	if ($conf["tm_newsfeed_marketplace_cache"] > 0) {
		$cache = cache_get($cache_key, 'cache');
		if (!empty($cache)) {
			return ($cache->data);
		}
	}

	//open connection
	$ch = curl_init();

	// timeout
	$curl_timeout = $conf["tm_newsfeed_curl_timeout"];

	// url
	$url = $conf["tm_newsfeed_marketplace_api_url"] . "?country=" . $country;

	//set the url, number of POST vars, POST data
	$CURLOPT_SSL_VERIFYHOST = 0; 
	if ($conf["tm_newsfeed_curl_check_certificate"]) {
		$CURLOPT_SSL_VERIFYHOST = 2;
	}
	curl_setopt($ch, CURLOPT_URL, $url);
	curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);

	// basic http auth
	if (isset($conf["tm_newsfeed_marketplace_api_userpass"])) {
		curl_setopt($ch, CURLOPT_USERPWD, $conf["tm_newsfeed_marketplace_api_userpass"]);
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
	cache_set($cache_key, $result, 'cache', time() + $conf["tm_newsfeed_marketplace_cache"]);

	// return result
	try {
		return json_decode($result);
	} catch (Exception $e) {
		return array();
	}
	
}

