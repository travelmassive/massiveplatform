<?php

// tm_status_updates.display.inc - status update display methods

/**
 * Display global news feed
 */
function tm_status_updates_display_feed($feed_type) {

	global $user;
	global $conf;

	$fetch_next = $conf["tm_status_updates_items_per_load"];

	// start performance timer
	$timer_start = microtime(true); 

	// load user
	$account = user_load($user->uid);

	// hide title
	drupal_add_css('#page-title { display: none; }', 'inline');

	// global feed
	if ($feed_type == "global") {
		$feed_title = "All members";

		if ($user->uid > 0) {
			$feed_title .= " &middot; <a style='font-size: smaller; text-decoration: none;color: #007DB7;' href='/newsfeed'>Your feed</a>";
		}

		if ($user->uid == 0) {

			// show login form
			$tm_status_update_form = "";
			$signup_message = tm_status_updates_render_login_message();
			drupal_set_message($signup_message . "<a href='/user/register'>Sign up now</a> or <a href='javascript:jq_login_signup_box();'>Log In</a>.", "help-tip");

		} else {

			// clear any previous sign in messages
			drupal_get_messages("help-tip");

			// show status update form
			$tm_status_update_form = tm_status_updates_render_update_form($account);
		}

		$all_results = tm_status_updates_poll_results("global", array(), null, $fetch_next);
	}

	// newsfeed
	if ($feed_type == "newsfeed") {

		$feed_title = "Your feed &middot; <a style='font-size: smaller; text-decoration: none;color: #007DB7;' href='/newsfeed/global'>All members</a>";
		$all_results = tm_status_updates_poll_results("newsfeed", array("account" => $account), null, $fetch_next);
		$tm_status_update_form = tm_status_updates_render_update_form($account);

		// If no results for your feed, switch to global feed
		if (sizeof($all_results) == 0) {
			return tm_status_updates_display_feed("global");
		}

		// clear any previous sign in messages
		drupal_get_messages("help-tip");
	}

	// tags
	if ($feed_type == "tags") {
		$feed_title = "Tagged updates";
		$tags = tm_status_updates_get_search_tags();
		$all_results = tm_status_updates_poll_results("tags", array("tags" => $tags), null, $fetch_next);
		$tm_status_update_form = "<h2 style='margin-bottom: 8px;'>Updates recently tagged " . implode(" and ", $tags) . "</h2><a href='/newsfeed'>&larr; Return to news feed</a>";
		drupal_add_js("var tm_update_status_loader_meta = ". json_encode(array("tags" => $tags)) . ";", "inline");
	}

	// search
	if ($feed_type == "search") {
		$feed_title = "Search updates";
		$search_terms = tm_status_updates_get_search_terms();
		$all_results = tm_status_updates_poll_results("search", array("search_terms" => $search_terms), null, $fetch_next);
		$tm_status_update_form = "<h2 style='margin-bottom: 8px;'>Updates matching '" . implode(" and ", $search_terms) . "'</h2><a href='/newsfeed'>&larr; Return to news feed</a>";
		drupal_add_js("var tm_update_status_loader_meta = ". json_encode(array("search_terms" => $search_terms)) . ";", "inline");
	}

	// near
	if ($feed_type == "near") {
		$feed_title = "Nearby updates";
		$location = tm_status_updates_get_location();
		$all_results = tm_status_updates_poll_results("near", array("latitude" => $location["latitude"], "longitude" => $location["longitude"], "range" => $location["range"]), null, $fetch_next);
		$tm_status_update_form = "<h2 style='margin-bottom: 8px;'>Nearby updates</h2><span style='background-color: #fff; font-size: 14px; border-radius: 4px; padding: 4px;'> latitude:" . $location["latitude"] . ", longitude:" . $location["longitude"] . ", range: " . $location["range"] . "km</span><br><a href='/newsfeed'>&larr; Return to news feed</a>";
		drupal_add_js("var tm_update_status_loader_meta = ". json_encode(array("location" => $location)) . ";", "inline");
	}

	// // load js libraries
	if ($user->uid == 0) {
		tm_status_updates_display_include_js(true, $feed_type, false, false);
	} else {
		tm_status_updates_display_include_js(true, $feed_type, true, true);
	}

	// get viewer ip
	$viewer_ip = tm_geoip_get_real_visitor_ip();

	// preload users before rending to improve performance
	tm_status_updates_preload_entities($all_results["results"]);

	// render item html from results
	$results_html = tm_status_updates_render_results_items($all_results["results"], $viewer_ip);

	// render newsfeed section
	$newsfeed_html = tm_status_updates_render_results_feed($results_html, $feed_title, "newsfeed");

	// show loader
	if (sizeof($all_results["results"]) >= $fetch_next) {
		$loader_html = tm_status_updates_render_loader($all_results["last_timestamp"]);
		$newsfeed_html .= $loader_html;
	}
 
	// promoted updates
	$promoted_html = tm_status_updates_render_promoted(7, $conf["tm_status_updates_show_featured"]);

	// recommended members
	$recommended_html = tm_status_updated_render_recommended_members($user);

	// show help text if now enough results
	$help_message = "";
	if (sizeof($all_results["results"]) < $fetch_next) {
		$loader_html = "";
		$help_message = "Build your personal news feed by following members and companies you're interested in. Or find out what's happening on the <a href='/newsfeed/global'>global news feed</a>.";
		//drupal_set_message($help_message, "help-tip");
		$help_html = tm_status_update_render_custom_message($help_message);
		$newsfeed_html .= $help_html;
	}

	// custom message
	$message_html = "";
	if (($help_message == "") and (isset($conf["tm_status_updates_custom_message"][$feed_type]))) {
		$message_html = tm_status_update_render_custom_message($conf["tm_status_updates_custom_message"][$feed_type]);
		if ($promoted_html == "") {
			// if no promoted updates, push the help text up to be even with left hand side
			$message_html = "<div style='margin-top: -14px;'>" . $message_html . "</div>";
		}

	}
	
	// if not approved show a message
	tm_status_updates_unapproved_member_message();

	// construct page from rendered items
	$html = file_get_contents(drupal_get_path('module', 'tm_status_updates') .'/templates/tm_status_updates_newsfeed.tpl.php');
	$html = str_replace("__STATUS_UPDATE_FORM__", $tm_status_update_form, $html);
	$html = str_replace("__NEWSFEED_RESULTS__", $newsfeed_html, $html);
	$html = str_replace("__PROMOTED_UPDATES__", $promoted_html, $html);
	$html = str_replace("__RECOMMENDED_MEMBERS__", $recommended_html, $html);
	$html = str_replace("__CUSTOM_MESSAGE__", $message_html, $html);

	// include performance timing
	$performance_time = round((microtime(true) - $timer_start), 3);
	$html .= "\n<div class='tm-status-updates-timer' style='display: none;'>" . $performance_time . "</div>";
	return $html;
}

/**
 * Global feed render
 * Wrapper for tm_status_updates_display_feed()
 */
function tm_status_updates_display_global() {
	return tm_status_updates_display_feed("global");
}

/**
 * Status updates render
 * Wrapper for tm_status_updates_display_feed()
 */
function tm_status_updates_display_newsfeed() {
 
 	global $user;

 	if ($user->uid == 0) {
 		return tm_status_updates_display_feed("global");
 	}
 	return tm_status_updates_display_feed("newsfeed");
}

/**
 * Tags feed render
 * Wrapper for tm_status_updates_display_feed()
 */
function tm_status_updates_display_tags() {
	return tm_status_updates_display_feed("tags");
}

/**
 * Search feed render
 * Wrapper for tm_status_updates_display_feed()
 */
function tm_status_updates_display_search() {
	return tm_status_updates_display_feed("search");
}

/**
 * Nearby feed render
 * Wrapper for tm_status_updates_display_feed()
 */
function tm_status_updates_display_near() {
	return tm_status_updates_display_feed("near");
}

/**
 * Load newsfeed via ajax
 */
function tm_status_updates_display_ajax() {

	global $conf;
	global $user;

	// only allow status updates for signed in users
	if (!user_is_logged_in()) {
		echo "";
		return;
	}

	$fetch_next = $conf["tm_status_updates_items_per_load"];
	$limit_results_days = $conf["tm_status_updates_limit_results_days"];

	// limit results to time stamp
	$limit_datetime = new DateTime("now", new DateTimeZone("UTC"));
	$limit_datetime->sub(new DateInterval('P' . intval($limit_results_days) . 'D'));
	$limit_timestamp = $limit_datetime->getTimestamp();

	// default timestamp 
	$timestamp = null;

	// get timestamp
	if (isset($_GET["timestamp"])) {

		// get timestamp if not null
		if ($_GET["timestamp"] != "") {
			$timestamp = intval($_GET["timestamp"]);
		}
	}

	// limit timestamp
	$hide_loader = false;
	if (($timestamp != null) and ($limit_timestamp > $timestamp)) {
		$timestamp = $limit_timestamp;
		$hide_loader = true;
	}

	$feed_type = "newsfeed";
	if (isset($_GET["feed_type"])) {
		$feed_type = $_GET["feed_type"];
	}

	// get meta from pager
	$meta = null;
	if (isset($_GET["meta"])) {
		$meta = $_GET["meta"];
	}
	try {
		$meta = json_decode($meta);
	} catch (Exception $e) {
		$meta = $_GET["meta"];
	}

	// start performance timer
	$timer_start = microtime(true); 

	// get sorted list of status update and flag results
	if ($feed_type == "global") {
		$all_results = tm_status_updates_poll_results("global", array(), $timestamp, $fetch_next);
	}
	if ($feed_type == "newsfeed") {
		$all_results = tm_status_updates_poll_results("newsfeed", array("account" => $user), $timestamp, $fetch_next);
	}
	if ($feed_type == "tags") {
		// get the tag search
		if (!isset($meta->tags)) {
			echo "No results";
			return;
		}
		$tags = $meta->tags;
		$all_results = tm_status_updates_poll_results("tags", array("tags" => $tags), $timestamp, $fetch_next);
	}
	if ($feed_type == "search") {
		// get the search_terms
		if (!isset($meta->search_terms)) {
			echo "No results";
			return;
		}
		$search_terms = $meta->search_terms;
		$all_results = tm_status_updates_poll_results("search", array("search_terms" => $search_terms), $timestamp, $fetch_next);
	}
	if ($feed_type == "near") {
		// get the location
		if (!isset($meta->location)) {
			echo "No results";
			return;
		}
		$location = $meta->location;
		$all_results = tm_status_updates_poll_results("near", array("latitude" => $location->latitude, "longitude" => $location->longitude, "range" => $location->range), $timestamp, $fetch_next);
	}

	// get viewer ip
	$viewer_ip = tm_geoip_get_real_visitor_ip();

	// preload users before rending to improve performance
	tm_status_updates_preload_entities($all_results["results"]);

	// render item html from results
	$results_html = tm_status_updates_render_results_items($all_results["results"], $viewer_ip);

	// include performance timing
	$performance_time = round((microtime(true) - $timer_start), 3);
	$results_html .= "\n<div class='tm-status-updates-timer' style='display: none;'>" . $performance_time . "</div>";

	// hide pager if more than display results or if no result
	if (sizeof($all_results) == 0) {
		$hide_pager = true;
	}

	// hide pager if:
	// - last_timestamp is at limit
	// - less results returned than fetch_next
	if (($limit_timestamp == $all_results["last_timestamp"]) 
		or (sizeof($all_results["results"]) < $fetch_next)) {
		$hide_loader = true;
	}

	if ($hide_loader) {
		$results_js = "\n<script>tm_status_update_remove_loader();</script>\n";
	} else {
		$results_js = "\n<script>tm_status_updates_loader_timestamp = " . $all_results["last_timestamp"] . ";</script>\n";
	}

	echo $results_html . $results_js;
	return;
}

/**
 * Helper method to get list of search terms
 * ie: newsfeed/tags/TAG1/TAG2/etc
 * ie: newsfeed/search/word1/word2/etc
 */
function tm_status_updates_get_search_terms() {

	// ie: newsfeed/tags/TAG1/TAG2/etc
	$parts = explode("/", current_path());
	$queries = array_slice($parts, 2);

	// create query_term array
	$query_terms = array();
	foreach($queries as $term) {
		$term_stripped = strip_tags($term);
		if (strlen($term_stripped) > 2) {
			$query_terms[] = $term_stripped; // must be > 2 characters
		} 
	}
	return $query_terms;
}

/**
 * Helper method to get list of tags
 * ie: newsfeed/tags/TAG1/TAG2/etc
 */
function tm_status_updates_get_search_tags() {
	
	// get query terms
	$query_terms = tm_status_updates_get_search_terms();
	
	// add # to each item
	$tags = array();
	foreach($query_terms as $tag) {
		$tags[] = '#' . $tag;
	}

	return $tags;
}

/**
 * Helper method to get location 
 * if /near, use geoip
 * if /near/lat/lon, set from url
 * if /near/lat/lon/range, set from url
 * optional ?range=100 for 100km
 * return location array("latitude" => 100.123, "longitude" => 50.2, "range" => 20)
 */
function tm_status_updates_get_location() {

	// default
	$latitude = null;
	$longitude = null;
	$range = 200; // 200km

	// ie: newsfeed/tags/TAG1/TAG2/etc
	$parts = explode("/", current_path());
	
	// ?range=
	if (isset($_GET["range"])) {
		$range = intval($_GET["range"]);
	}

	// /near
	if (sizeof($parts) == 2) {
		$visitor_ip = tm_geoip_get_real_visitor_ip();
		$geoip_info = tm_geoip_lookup($visitor_ip);
		if (isset($geoip_info['location']['latitude'])) {
			$latitude = $geoip_info['location']['latitude'];
		}
		if (isset($geoip_info['location']['longitude'])) {
			$longitude = $geoip_info['location']['longitude'];
		}
	}

	// /near/lat/lon
	if (sizeof($parts) == 4) {
		$latitude = floatval($parts[2]);
		$longitude = floatval($parts[3]);
	}

	// /near/lat/lon/range
	if (sizeof($parts) == 5) {
		$latitude = floatval($parts[2]);
		$longitude = floatval($parts[3]);
		$range = floatval($parts[4]);
	}

	$location = array("latitude" => $latitude, "longitude" => $longitude, "range" => $range);
	return $location;
}


/**
 * Helper method to load js libraries
 */
function tm_status_updates_display_include_js($actions = true, $feed_type = "global", $post = false, $google_maps = false) {

	global $user;
	global $conf;

	// js vars
	$js_vars = "";
	$js_vars .= "\nvar tm_update_status_uid = " . $user->uid . ";";
	$js_vars .= "\nvar tm_status_updates_loader_feed_type = '" . $feed_type . "';";
	$js_vars .= "\nvar tm_status_updates_loader_timestamp = null;";
	drupal_add_js($js_vars, "inline");

	// actions js
	if ($actions) {
		drupal_add_js(drupal_get_path('module', 'tm_status_updates') . '/js/tm_status_updates_actions.js');
	}

	// post js
	if ($post) {
		drupal_add_js(drupal_get_path('module', 'tm_status_updates') . '/js/tm_status_updates_post.js');
	}

	// google maps
	if ($google_maps) {

		// include google maps api so we can reverse geocode lat/lon
		$google_api_key = $conf["tm_status_updates_google_maps_api_key"];
		$google_maps_api_url = 'https://maps.googleapis.com/maps/api/js?callback=initGoogleMaps';
		if ($google_api_key != "") {
			$google_maps_api_url = 'https://maps.googleapis.com/maps/api/js?key=' . $google_api_key . '&callback=initGoogleMaps';
		}
		drupal_add_js($google_maps_api_url, 'external');
	}
}

/**
 * Helper method to display member approval message
 */
function tm_status_updates_unapproved_member_message() {

	global $user;
	global $conf;

	// don't show to logged out users
	if (!user_is_logged_in()) {
		return;
	}

	// if user is not approved yet
	$account = user_load($user->uid);

	// don't show if user has't yet validated address
	if (in_array('non-validated', $account->roles)) {
		return;
	}

	// check if user has been approved yet
	if (!in_array("approved user", $user->roles)) {

		// show approval link
		$who_flagged = flag_get_entity_flags("user", $user->uid, "approval_requested_by_user");
		if (sizeof($who_flagged) > 0) {
	  		foreach ($who_flagged as $flagger) {
	    		$difference = time() - $flagger->timestamp;
	  		}
	  		$flagged_time = format_interval($difference, 1) . " ago";
			$approval_link = l(t('Approval requested (' . $flagged_time . ')'), 'javascript:jq_approval_already_requested();', array('fragment' => '','external'=>true));
		} 
		else {
			$approval_link = l(t('Approve my account'), 'javascript:jq_request_approval(' . $user->uid . ')', array('fragment' => '','external'=>true, 'attributes' => array('class' => array('approval-link'))));
		}

		// tell user they need to be approved
		$message = $conf["tm_status_updates_unapproved_message"];
		drupal_set_message($message . " " . $approval_link, "help-tip");
	}
}

/**
 * Helper method to preload user and nodes that will be displayed for faster performance
 */
function tm_status_updates_preload_entities($results) {

	$uids = array();
	$nids = array();
	foreach ($results as $result) {
		
		// add person who flagged or posted
		$uids[] = $result->owner_uid;

		// status update
		if ($result->result_type == "status_update") {
			if ($result->status_update_entity_type != "user") {
				$nids[] = $result->status_update_entity_id; // add entity id of posting as
			}
		}

		// flagged item
		if ($result->result_type == "flagged_item") {
			if ($result->flagged_entity_type != "user") { 
				$nids[] = $result->flagged_entity_id; // node that was flagged
			}
		}
	}

	// unique
	$uids = array_unique($uids);
	$nids = array_unique($nids);

	// preload users
	$loaded_uids = user_load_multiple($uids);

	// preload nodes
	$loaded_nids = node_load_multiple($nids);

	return true;
}

