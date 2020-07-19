<?php

// tm_track_views.display.inc

/**
 * Render how many people have viewed your profile
 * Called from _tm_ds_track_viewers_wrapper in tm_fields.module
 */
function _tm_ds_track_viewers($field, $title = NULL) {

	global $conf;
	global $user;

	// get entity
	$entity = $field['entity'];

	// get entity_id and entity_type
	if (isset($entity->uid)) {
		$entity_id = $entity->uid;
		$entity_type = "user";
	}
	if (isset($entity->nid)) {
		$entity_id = $entity->nid;
		$entity_type = "node";
		if ($entity->type != "organization") {
			return;
		}
	}

	// only show to logged in user
	if (!user_is_logged_in()) {
		return;
	}

	// only show for your own profile
	if ($entity_type == "user") {
		if ($entity_id != $user->uid) {
			return;
		}
	}
	if ($entity_type == "node") {
		if (!tm_organizations_is_company_profile_manager($user->uid, $entity_id)) {
			return;
		}
	}

	// get number of views
	$num_unique_viewers = tm_track_views_how_many_viewed($entity_id, $entity_type, $user->uid, $conf["tm_track_views_store_days"]);

	// don't display if no viewers
	if ($num_unique_viewers == 0) {
		return "";
	}

	// pluralize
	$member_text = "member";
	if ($num_unique_viewers > 1) {
		$member_text = "members";
	}

	// link
	$link = "/user/" . $entity_id . "/viewers";
	if ($entity_type == "node") {
		$link = "/company/" . $entity_id . "/viewers";
	}

	// render message
	$profile_views_label = "Who visited"; // default
	if (isset($conf["tm_track_views_see_who_viewed_label"])) {
		$profile_views_label = $conf["tm_track_views_see_who_viewed_label"];
	}
	$html = "Your profile has been viewed by " . $num_unique_viewers . " " . $member_text . " &middot; <a href='" . $link . "'>" . $profile_views_label . "</a>";

	return $html;
}

/**
 * Implements hook_views_pre_execute().
 * Check permissions
 */
function tm_track_views_views_pre_execute(&$view) {

	// validate
	if (($view->current_display != 'who_viewed_member_profile') and ($view->current_display != 'who_viewed_company_profile') and ($view->current_display != 'who_viewed_event') and ($view->current_display != 'who_viewed_list')) {
		return;
	}

	// Detect ajax request
	if (!empty($_SERVER['HTTP_X_REQUESTED_WITH']) && strtolower($_SERVER['HTTP_X_REQUESTED_WITH']) == 'xmlhttprequest') {
		return;
	}

	global $conf;
	global $user;

	// prepare vars
	$entity_id = arg(1); // ie: user/123/viewed
	$current_display = $view->current_display; // who_viewed_member_profile or who_viewed_company_profile
	$entity_type = "user";
	if (arg(0) != "user") {
		$entity_type = "node";
	}

	// check permission
	$result = tm_track_views_check_permission($current_display, $entity_id, $user);
	if ($result == false) {
		return;
	}

	// add return link to profile
	$profile_url = drupal_get_path_alias($entity_type . '/'. $entity_id);

	// profile label
	$profile_label = "View your profile";

	// members name
	if ($entity_type == "user") {
		$entity = user_load($entity_id);
		if ($entity != null) {
			$user_first_name = $entity->field_user_first_name[LANGUAGE_NONE][0]['value'];
			$user_last_name = $entity->field_user_last_name[LANGUAGE_NONE][0]['value'];
			$profile_label = strip_tags($user_first_name . " " . $user_last_name);
		}
	}

	// node title
	if ($entity_type == "node") {
		$entity = node_load($entity_id);
		if ($entity != null) {
			$profile_label = trim(strip_tags($entity->title));
		}
		if ($entity->type == "list") {
			$list_count = tm_lists_get_num_items($entity_id, 1);
			$profile_label = str_replace("##", $list_count, $profile_label);
		}
	}

	// set js
	$js_setting = array(
		'tm_track_views_title' => array(
			'profile_url' => $profile_url,
			'profile_label' => $profile_label,
		),
	);
	drupal_add_js($js_setting, 'setting');
	drupal_add_js(drupal_get_path('module', 'tm_track_views') . '/js/tm_track_views_title.js');

	// set label
	$label = "profile"; // for members and company pages 
	if ($view->current_display == 'who_viewed_event') {
		$label = "event";
	}
	if ($view->current_display == 'who_viewed_list') {
		$label = "list";
	}

	// get number of views
	$num_unique_viewers = tm_track_views_how_many_viewed($entity_id, $entity_type, $user->uid, $conf["tm_track_views_store_days"]);
	if ($num_unique_viewers == 0) {
		$help_message = "Nobody has viewed your " . $label . " yet.";
	} else {
		$help_message = "These members have viewed your " . $label . " in the past " . $conf["tm_track_views_store_days"] . " days.";
	}

	// show help tip
	drupal_set_message($help_message, 'help-tip');

	// always allow chapter leaders, moderators to view
	if ((in_array("chapter leader", $user->roles)) or (in_array("moderator", $user->roles)) or (in_array("administrator", $user->roles))) {
		return;
	}

	// allow all members to see who has viewed their profile
	if ($conf["tm_track_views_display_mode"] == "all") {
		return;
	}

	// only allow approved members to see who has viewed profile
	if ($conf["tm_track_views_display_mode"] == "approved") {
		$message = $conf["tm_track_views_approved_message"];
		if (in_array("approved user", $user->roles)) {
			return;
		}
	}

	// only allow subscribed members to see who has viewed profile
	if ($conf["tm_track_views_display_mode"] == "subscribed") {
		$message = $conf["tm_track_views_subscription_message"];
		if (module_exists("tm_subscriptions_user")) {
			if (tm_subscriptions_is_user_subscription_enabled($user->uid)) {
				return;
			}
		}
	}

	// always allow event owners to see who viewed
	if ($view->current_display == 'who_viewed_event') {
		return;
	}

	// blur inline css (before js loads)
	drupal_add_css('#main {-webkit-filter: blur(4px); -moz-filter: blur(4px); -o-filter: blur(4px); -ms-filter: blur(4px); filter: blur(4px);}', 'inline');

	// add view profile link
	$message .= '<p></p><div class="more-link"><a href="/user/' . $user->uid . '" class="bttn bttn-secondary bttn-m" title="View my profile">View my profile</a></div>';

	// add display message to js
	$js_setting = array(
		'tm_track_views' => array(
			'display_message' => $message,
		),
	);
	
	// insert display js
	drupal_add_js($js_setting, 'setting');
	drupal_add_js(drupal_get_path('module', 'tm_track_views') . '/js/tm_track_views_display.js');

}
