<?php

// tm_status_updates.display.inc - status update display methods

/**
 * Display global news feed
 */
function tm_status_updates_display_global() {

	global $user;
	global $conf;
	$limit_from = 0;
	$limit_to = 10;

	// hide title
	drupal_add_css('#page-title { display: none; }', 'inline');

	// if user is not signed in, show them call to action
	if ($user->uid == 0) {
		// instead of status update form, show sign in and call to action
		$tm_status_update_form = "Sign up to post and update!";
	} else {
		$account = user_load($user->uid);
		drupal_add_js(drupal_get_path('module', 'tm_status_updates') . '/js/tm_status_updates.js');
		$tm_status_update_form = tm_status_updates_render_update_form($account);
	}

	// get sorted list of status update and flag results
	$all_results = tm_status_updates_get_combined_feed_global($limit_from, $limit_to);

	// get viewer ip
	$viewer_ip = tm_geoip_get_real_visitor_ip();

	// render item html from results
	$results_html = tm_status_updates_render_results_items($all_results, $viewer_ip);

	// render newsfeed section
	$newsfeed_html = tm_status_updates_render_results_feed($results_html, true);

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
 	$limit_from = 0;
 	$limit_to = 10;

 	// render global newsfeed if not logged in
 	if (!user_is_logged_in()) {
 		return tm_status_updates_display_global();
 	}

 	// load user account
 	$account = user_load($user->uid);

	// hide title
	drupal_add_css('#page-title { display: none; }', 'inline');

	// render status update form
	drupal_add_js(drupal_get_path('module', 'tm_status_updates') . '/js/tm_status_updates.js');
	$tm_status_update_form = tm_status_updates_render_update_form($account);

	// get sorted list of status update and flag results
	$all_results = tm_status_updates_get_combined_feed($account, $limit_from, $limit_to);

	// get viewer ip
	$viewer_ip = tm_geoip_get_real_visitor_ip();

	// render item html from results
	$results_html = tm_status_updates_render_results_items($all_results, $viewer_ip);

	// render newsfeed section
	$newsfeed_html = tm_status_updates_render_results_feed($results_html);

	// recommended members
	$recommended_html = tm_status_updated_render_recommended_members($user);

	// construct page from rendered items
	$html = file_get_contents(drupal_get_path('module', 'tm_status_updates') .'/templates/tm_status_updates_newsfeed.tpl.php');
	$html = str_replace("__STATUS_UPDATE_FORM__", $tm_status_update_form, $html);
	$html = str_replace("__NEWSFEED_RESULTS__", $newsfeed_html, $html);
	$html = str_replace("__RECOMMENDED_MEMBERS__", $recommended_html, $html);

	return $html;
}

