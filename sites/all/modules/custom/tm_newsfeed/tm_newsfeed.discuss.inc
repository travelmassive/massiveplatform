<?php

// tm_newsfeed.discuss.inc - discussions methods

function tm_newsfeed_discuss_fetch($search_category_classes = "all") {

	global $conf;

	// check cache
	sort($search_category_classes);
	$search_category_classes_string = implode("-", $search_category_classes);
	$cache_key = 'tm-newsfeed-discuss-' . str_replace(" ", "-", strtolower($search_category_classes_string));
	if ($conf["tm_newsfeed_discuss_cache"] > 0) {
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
	$search_category_classes_query = implode(",", $search_category_classes);
	$url = $conf["tm_newsfeed_discuss_api_url"] . "?mode=json&category_id=" . $search_category_classes_query;

	//set the url, number of POST vars, POST data
	$CURLOPT_SSL_VERIFYHOST = 0; 
	if ($conf["tm_newsfeed_curl_check_certificate"]) {
		$CURLOPT_SSL_VERIFYHOST = 2;
	}
	curl_setopt($ch, CURLOPT_URL, $url);
	curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);

	// basic http auth
	if (isset($conf["tm_newsfeed_discuss_api_userpass"])) {
		curl_setopt($ch, CURLOPT_USERPWD, $conf["tm_newsfeed_discuss_api_userpass"]);
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
	cache_set($cache_key, $result, 'cache', time() + $conf["tm_newsfeed_discuss_cache"]);

	// return result
	try {
		return json_decode($result);
	} catch (Exception $e) {
		return array();
	}
	
}

// fetch discussion newsfeed for a user
// prepare category_id classes based on user's chapters and events
function tm_newsfeed_discuss_fetch_for_user($account) {

	// discussion categories to search
	$search_category_classes = array();

	// add discussion categories from user's chapters
	// ie: continent-europe, country-germany
	$users_chapters = tm_users_get_chapters($account);
	foreach($users_chapters as $chapter_nid) {

	  $node = node_load($chapter_nid);

	  if (isset($node->field_discuss_category_id[LANGUAGE_NONE][0]['value'])) {
	    $search_category_classes[] = trim($node->field_discuss_category_id[LANGUAGE_NONE][0]['value']);
	  } else {

	    // Uncomment if you want to search in chapter discussions
	    // ie: chapter-berlin
	    // $search_category_classes[] = "chapter-" . strtolower(str_replace(" ", "_", $node->title));

	    // lookup country and continent
	    if (isset($node->field_country[LANGUAGE_NONE][0]['iso2'])) {
	      $country_code = $node->field_country[LANGUAGE_NONE][0]['iso2'];
	      $country = country_load($country_code);
	      if ($country != null) {

	        // ie: country-germany
	        $search_category_classes[] = "country-" . strtolower($country_code);

	        // ie: continent-europe
	        $search_category_classes[] = "continent-" . strtolower($country->continent);
	      }
	    }

	  }
	}

	// add discussion categories from user's events
	// if event has a discussion id
	$users_events = tm_users_get_users_events($account);
	foreach($users_events as $event_nid) {
	  $node = node_load($event_nid);
	  if (isset($node->field_event_discuss_category_id[LANGUAGE_NONE][0]['value'])) {
	      $search_category_classes[] = trim($node->field_event_discuss_category_id[LANGUAGE_NONE][0]['value']);
	  }
	}

	// If no matching category classes, show all discussions
	if (sizeof($search_category_classes) == 0) {
	  $search_category_classes = array("all");
	}

	// Remove duplicate classes
	$search_category_classes = array_unique($search_category_classes);
	
	// Fetch results for user
	$results = tm_newsfeed_discuss_fetch($search_category_classes);
	return $results;
}
