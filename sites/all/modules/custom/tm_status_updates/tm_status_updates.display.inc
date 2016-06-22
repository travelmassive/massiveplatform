<?php

// tm_status_updates.display.inc - status update display methods

/**
 * Display global news feed
 */
function tm_status_updates_display_global() {

	global $user;
	global $conf;

	$display_from = 0;
	$display_to = $conf["tm_status_updates_items_per_load"];

	// hide title
	drupal_add_css('#page-title { display: none; }', 'inline');

	// if user is not signed in, show them call to action
	if ($user->uid == 0) {
		// instead of status update form, show sign in and call to action
		$tm_status_update_form = "Sign up to post and update!";
	} else {
	
		// load js libraries
		tm_status_updates_display_include_js(true, true, true);

		// load update form
		$account = user_load($user->uid);
		$tm_status_update_form = tm_status_updates_render_update_form($account);
	}

	// get sorted list of status update and flag results
	$all_results = tm_status_updates_get_combined_feed_global($display_from, $display_to);

	// get viewer ip
	$viewer_ip = tm_geoip_get_real_visitor_ip();

	// render item html from results
	$results_html = tm_status_updates_render_results_items($all_results, $viewer_ip);

	// render newsfeed section
	$newsfeed_html = tm_status_updates_render_results_feed($results_html, "Global Updates");

	// add loader
	$loader_html = tm_status_updates_render_loader("global", $display_from, $display_to);
	$newsfeed_html .= $loader_html;

	// recommended members
	$recommended_html = tm_status_updated_render_recommended_members($user);

	// construct page from rendered items
	$html = file_get_contents(drupal_get_path('module', 'tm_status_updates') .'/templates/tm_status_updates_newsfeed.tpl.php');
	$html = str_replace("__STATUS_UPDATE_FORM__", $tm_status_update_form, $html);
	$html = str_replace("__NEWSFEED_RESULTS__", $newsfeed_html, $html);
	$html = str_replace("__RECOMMENDED_MEMBERS__", $recommended_html, $html);

	return $html;
}

/**
 * Status updates render
 */
function tm_status_updates_display_newsfeed() {
 
 	global $user;
 	global $conf;
 
 	$display_from = 0;
 	$display_to = $conf["tm_status_updates_items_per_load"];

 	// render global newsfeed if not logged in
 	if (!user_is_logged_in()) {
 		return tm_status_updates_display_global();
 	}

 	// load user account
 	$account = user_load($user->uid);

	// hide title
	drupal_add_css('#page-title { display: none; }', 'inline');

	// load js libraries
	tm_status_updates_display_include_js(true, true, true);

	// render update form
	$tm_status_update_form = tm_status_updates_render_update_form($account);

	// get sorted list of status update and flag results
	$all_results = tm_status_updates_get_combined_feed($account, $display_from, $display_to);

	// get viewer ip
	$viewer_ip = tm_geoip_get_real_visitor_ip();

	// render item html from results
	$results_html = tm_status_updates_render_results_items($all_results, $viewer_ip);

	// render newsfeed section
	$newsfeed_html = tm_status_updates_render_results_feed($results_html, "Latest Updates");
	
	// add loader
	$loader_html = tm_status_updates_render_loader("newsfeed", $display_from, $display_to);
	$newsfeed_html .= $loader_html;

	// recommended members
	$recommended_html = tm_status_updated_render_recommended_members($user);

	// construct page from rendered items
	$html = file_get_contents(drupal_get_path('module', 'tm_status_updates') .'/templates/tm_status_updates_newsfeed.tpl.php');
	$html = str_replace("__STATUS_UPDATE_FORM__", $tm_status_update_form, $html);
	$html = str_replace("__NEWSFEED_RESULTS__", $newsfeed_html, $html);
	$html = str_replace("__RECOMMENDED_MEMBERS__", $recommended_html, $html);

	return $html;
}

/**
 * Load newsfeed via ajax
 */
function tm_status_updates_display_ajax() {

	global $conf;
	global $user;

	$items_per_load = $conf["tm_status_updates_items_per_load"];
	$limit_results = $conf["tm_status_updates_limit_results"];

	$limit_from = 0;
	if (isset($_GET["limit_from"])) {
		$limit_from = intval($_GET["limit_from"]);
	}

	$limit_to = $items_per_load;
	if (isset($_GET["limit_to"])) {
		$limit_to = intval($_GET["limit_to"]);
	}

	$feed_type = "newsfeed";
	if (isset($_GET["feed_type"])) {
		$feed_type = $_GET["feed_type"];
	}

	// limit results
	$hide_pager = false;
	if ($limit_from > $limit_results) {
		$limit_from = $limit_results;
		$hide_pager = true;
	}

	if ($limit_to > $limit_results) { 
		$limit_to = $limit_results;
		$hide_pager = true;
	}

	// get sorted list of status update and flag results
	if ($feed_type == "global") {
		$all_results = tm_status_updates_get_combined_feed_global($limit_from, $limit_to);
	}
	if ($feed_type == "newsfeed") {
		$all_results = tm_status_updates_get_combined_feed($user, $limit_from, $limit_to);
	}

	// get viewer ip
	$viewer_ip = tm_geoip_get_real_visitor_ip();

	// render item html from results
	$results_html = tm_status_updates_render_results_items($all_results, $viewer_ip);

	// hide pager if more than display results or if no result
	if (sizeof($all_results) == 0) {
		$hide_pager = true;
	}
	if ($hide_pager) {
		$results_html .= "<script>tm_status_update_remove_loader();</script>";
	}

	echo $results_html . $loader_html;
	return;
}

/**
 * Helper method to load js libraries
 */
function tm_status_updates_display_include_js($actions = true, $post = false, $google_maps = false) {

	global $user;
	global $conf;

	// js vars
	drupal_add_js("var tm_update_status_uid = " . $user->uid . ";", "inline");
	drupal_add_js("var tm_update_status_items_per_load = " . $conf["tm_status_updates_items_per_load"] . ";", "inline");

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


