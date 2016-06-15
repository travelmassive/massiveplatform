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

	// render item html from results
	$results_html = tm_status_updates_render_results_items($all_results);

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

function tm_status_updates_people_suggestions($user_id) {

	// get query results
	$users_location = tm_people_suggest_chapters_and_locations($user_id);
	$users_job_role = tm_people_suggest_job_roles($user_id);
	$users_industry = tm_people_suggest_industry_segments($user_id);

	$users = array();

	// get ids
	$user_ids_location = array_map(function($o) { return $o->uid; }, $users_location);
	$user_ids_job_role = array_map(function($o) { return $o->uid; }, $users_job_role);
	$user_ids_industry = array_map(function($o) { return $o->uid; }, $users_industry);

	// random keys from each
	$keys_loc = array_rand($user_ids_location, 2);
	$keys_job = array_rand($user_ids_job_role);
	$keys_ind = array_rand($user_ids_industry, 2);

	// is someone in the location result in other lists? if so, add them above others
	$ids = array_merge(array_intersect($user_ids_location, $user_ids_job_role), array_intersect($user_ids_location, $user_ids_industry));

	foreach ($ids as $id) {
		$users[] = $id;
	}

	// add 2 people from location query, 1 from job_role query, 2 from industry query
	$users[] = $user_ids_location[$keys_loc[0]];
	$users[] = $user_ids_location[$keys_loc[1]];
	$users[] = $user_ids_job_role[$keys_job];
	$users[] = $user_ids_industry[$keys_ind[0]];
	$users[] = $user_ids_industry[$keys_ind[1]];

	// ensure we don't have duplicates
	// and only return 5 elements (array can have more due to adding ids common to multiple source queries)
	return array_slice(array_unique($users), 0, 5);
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

	// render item html from results
	$results_html = tm_status_updates_render_results_items($all_results);

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


