<?php

// discourse_sso.api.inc - api methods for discourse sso

/**
 * Get Discourse user ID
 * https://meta.discourse.org/t/sso-synced-login-state-tips/60464
 */
function discourse_sso_get_discourse_uid($uid) {

	global $conf;
	
	// ie: http(s)://forum.example.com/admin/users/{user_id}/log_out
	$url = $conf["tm_discourse_sso_sync_url"] . 'users/by-external/' . $uid . '.json';

	// empty params
	$params = array();

	// call api
	$result = discourse_sso_api_get($params, $url);

	return $result;
}


/**
 * Log out user
 * https://meta.discourse.org/t/sso-synced-login-state-tips/60464
 */
function discourse_sso_logout_user($discourse_uid) {

	global $conf;
	
	// ie: http(s)://forum.example.com/admin/users/{user_id}/log_out
	$url = $conf["tm_discourse_sso_sync_url"] . 'admin/users/' . $discourse_uid . '/log_out';

	// empty params
	$params = array();

	// call api
	$result = discourse_sso_api($params, $url);

	return $result;
}

/**
 * Post to SSO API
 * Returns result
 */
function discourse_sso_api($params, $url) {

	global $conf;

	// get sso secret
	$sso_secret = discourse_sso_get_secret();

	// get payload and sig
	$payload = base64_encode(http_build_query($params));
	$signed = hash_hmac('sha256', $payload, $sso_secret);

	// set api key and username in header
	// see: https://docs.discourse.org/
	// see: https://meta.discourse.org/t/discourse-api-documentation/22706
	$http_headers = array('Api-Key: ' . $conf["tm_discourse_sso_api_key"], 'Api-Username: ' . $conf["tm_discourse_sso_api_username"]);

	// url-ify the data for the POST
	$fields_string = http_build_query([
				// 'api_key' => $conf["tm_discourse_sso_api_key"], // moved to http headers
				// 'api_username' => $conf["tm_discourse_sso_api_username"], // moved to http headers
	            'sso' => $payload,
	            'sig' => $signed,
	        ], '', '&', PHP_QUERY_RFC3986);

	// open connection
	$ch = curl_init();

	// set the url, number of POST vars, POST data
	$CURLOPT_SSL_VERIFYHOST = 2;
	$CURLOPT_SSL_VERIFYPEER = true;
	if ($conf["tm_discourse_sso_sync_check_certificate"] == false) {
		$CURLOPT_SSL_VERIFYHOST = 0;
		$CURLOPT_SSL_VERIFYPEER = false;
	}

	curl_setopt($ch, CURLOPT_URL, $url);
	curl_setopt($ch, CURLOPT_HTTPHEADER, $http_headers);
	curl_setopt($ch, CURLOPT_POST, count($params));
	curl_setopt($ch, CURLOPT_POSTFIELDS, $fields_string);
	curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
	curl_setopt($ch, CURLOPT_VERBOSE, false);
	curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, $CURLOPT_SSL_VERIFYHOST);
	curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, $CURLOPT_SSL_VERIFYPEER);
	curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 0); 
	curl_setopt($ch, CURLOPT_TIMEOUT, $conf["tm_discourse_sso_sync_process_timeout"]); // timeout in seconds

	//execute post
	$result = curl_exec($ch);

	return $result;
}

/**
 * GET from Discourse API
 * Returns result
 */
function discourse_sso_api_get($params, $url) {

	global $conf;

	// url-ify the data for the POST
	$query_string = '?api_key=' . $conf["tm_discourse_sso_api_key"] . '&' . 'api_username=' . $conf["tm_discourse_sso_api_username"];

	// open connection
	$ch = curl_init();

	// set the url, number of POST vars, POST data
	$CURLOPT_SSL_VERIFYHOST = 2;
	$CURLOPT_SSL_VERIFYPEER = true;
	if ($conf["tm_discourse_sso_sync_check_certificate"] == false) {
		$CURLOPT_SSL_VERIFYHOST = 0;
		$CURLOPT_SSL_VERIFYPEER = false;
	}

	curl_setopt($ch, CURLOPT_URL, $url . $query_string);
	curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
	curl_setopt($ch, CURLOPT_VERBOSE, false);
	curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, $CURLOPT_SSL_VERIFYHOST);
	curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, $CURLOPT_SSL_VERIFYPEER);
	curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 0); 
	curl_setopt($ch, CURLOPT_TIMEOUT, $conf["tm_discourse_sso_sync_process_timeout"]); // timeout in seconds

	//execute get
	$result = curl_exec($ch);

	return $result;
}

