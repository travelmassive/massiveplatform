<?php

// tm_status_updated.render.inc - render methods for status updates

/**
 * Render most recent status update on node or user
 * Called from theme template
 */
function tm_status_updates_render_theme() {

	global $user;
	global $conf;

	// get current path
	// ie: user/123 or node/123
	$parts = explode("/", current_path());

	// check valid path
	if (sizeof($parts) != 2) {
		return;
	}

	// must be node or user
	if (($parts[0] != "user") and ($parts[0] != "node")) {
		return;
	}

	// must be nid or uid
	if (!is_numeric($parts[1])) {
		return;
	}

	// get entity id
	$entity_id = $parts[1];

	// get entity type
	if ($parts[0] == "user") {
		$entity_type = "user";
	} else {
		// load node to determine type
		$node = node_load($entity_id);
		if ($node == null) {
			return;
		}
		$entity_type = $node->type;
	}

	// check privacy for user
	if ($entity_type == "user") {

		// check accounts privacy setting
		$account = user_load($entity_id);
		if (isset($account->field_user_has_private_profile[LANGUAGE_NONE][0]['value'])) {
			if ($account->field_user_has_private_profile[LANGUAGE_NONE][0]['value']) {

				// check anonymous user
				if (user_is_anonymous()) {
					return;
				}

				// check viewing user is approved
				if ((!in_array('approved user', $user->roles)) and (!in_array('administrator', $user->roles))) {
					return;
				}
			}
		}
	}
	
	// fetch latest status update
	$result = tm_status_updates_get_latest_update($entity_id, $entity_type);

	// no update
	if ($result == null) {
		return;
	}

	// Display on profile for limited time
	$days_displayed = $conf["tm_status_updates_profile_display_days"];
	$date_posted = new DateTime("@".$result->timestamp, new DateTimeZone('UTC'));
	$date_now = new DateTime(null, new DateTimeZone('UTC'));
	$posted_seconds = $date_now->getTimestamp() - $date_posted->getTimestamp();
	if (($posted_seconds / (24 * 60 * 60)) > $days_displayed) {
		return;
	}

	// Get viewer IP
	$count_viewer_ip = tm_geoip_get_real_visitor_ip();

	// If moderator or administrator, show unapproved member's posts
	$show_unapproved = false;
	if (user_is_logged_in()) {
		$account = user_load($user->uid);
		if (in_array("moderator", $user->roles) or in_array("administrator", $user->roles)) {
			$show_unapproved = true;
		}
	}

	// Render status update
	$html = tm_status_updates_render_update($result, $count_viewer_ip = null, true, $show_unapproved);

	// Check if no display
	if ($html == "") {
		return;
	}

	// Increment view
	tm_status_updates_increment_daily_view_count($result->id);

	// Add profile css
	global $tm_status_updates_feed_template_profile_css;

	// Add actions js
	drupal_add_js('var tm_update_status_uid = ' . $user->uid . ';', array('type' => 'inline'));
	if (user_is_logged_in()) {
		drupal_add_js(drupal_get_path('module', 'tm_status_updates') . '/js/tm_status_updates_actions.js');
	}

	return $tm_status_updates_feed_template_profile_css . "\n" . $html;
}

/**
 * Render a status update
 */
function tm_status_updates_render_update($result, $count_viewer_ip = null, $show_you = true, $show_unapproved = false) {

	global $user;
	global $conf;

	// get entity type and id
	$entity_type = $result->entity_type;
	$entity_id = $result->entity_id;

	// load entity
	if ($entity_type == "user") {
		$entity = user_load($entity_id);
		$entity_url = drupal_get_path_alias("user/" . $entity_id);
	} else {
		$entity = node_load($entity_id);
		$entity_url = drupal_get_path_alias("node/" . $entity_id);
	}
	if ($entity == null) {
		return "";
	}

	// get posting user
	$posting_user = user_load($result->poster_uid);

	// allow viewing your own content if you are unapproved
	if ($result->poster_uid == $user->uid) {
		$show_unapproved = true;
	}

	// ensure posting user is approved
	if (!$show_unapproved) {
		if (!in_array("approved user", $posting_user->roles))  {
			return "";
		}
	}

	// member who posted
	if (!$show_you or ($posting_user->uid != $user->uid)) {
		$posting_user_name = strip_tags($posting_user->field_user_first_name[LANGUAGE_NONE][0]['value'] . " " . $posting_user->field_user_last_name[LANGUAGE_NONE][0]['value']);
	} else {
		$posting_user_name = "You";
	}
	
	// get time posted
	$date_posted = new DateTime("@".$result->timestamp, new DateTimeZone('UTC'));
	$date_now = new DateTime(null, new DateTimeZone('UTC'));
	$posted_time = format_interval($date_now->getTimestamp() - $date_posted->getTimestamp(), 1) . " ago";
	if ($posted_time == "0 sec ago") {
		$posted_time = "just now";
	}

  	// prepare status update text
  	$status_update_html = tm_base_linkify(strip_tags($result->status_update), array('http', 'https', 'newsfeed_hashtag', 'newsfeed_mention'), array("rel" => "nofollow", "target" => "_blank"));

  	// post update or shared link
  	$post_action_text = "posted an update";
  	if ($result->preview_link_id != null) {
  		$post_action_text = "shared a link";
  	}

  	// feed text
  	$feed_info = "<span class='status_update'>" . $status_update_html . "</span>";

  	// feed image class
  	$feed_image_class = "status-updates-image";

	// user update
	if ($entity_type == "user") {

		$image_uri = _tm_users_get_avatar_uri($posting_user);
		$image_url = image_style_url("avatar", $image_uri);
		$feed_title = l($posting_user_name, $entity_url) . " " . $post_action_text;
		$feed_image_class = "status-updates-image-avatar";
	}

	// organization update
	if ($entity_type == "organization") {

		$image_uri = _tm_organizations_get_avatar_uri($entity);
		$image_url = image_style_url("avatar", $image_uri);
		$feed_title = l($entity->title, $entity_url) . " " . $post_action_text;
		$feed_image_class = "status-updates-image-avatar";
	}

	// chapter update
	if ($entity_type == "chapter") {

		$image_uri = _tm_chapters_get_image_uri($entity);
		$image_url = image_style_url("banner", $image_uri);
		$feed_title = l(tm_chapters_formal_name($entity), $entity_url) . " " . $post_action_text;
	}

	// event update
	if ($entity_type == "event") {
		// not implemented yet
		return "";
	}

	// post details
	$details = array();
	$details["location_text"] = $result->location_text;
	if ($details["location_text"] != "") {
		$parts = explode(",", $details["location_text"]);
		if (sizeof($parts) > 0) {
			$search_string = strip_tags($parts[0]);
			if (strpos($search_string, " ")) {
				$search_string = '"' . $search_string . '"';
			}
			$details["location_url"] = "/search/?query=in:" . urlencode($search_string);
		}
	}
	$details["posted_time"] = $posted_time;
	$details["view_count"] = tm_status_updates_get_view_count($result->id);
	$details["is_edited"] = ($result->edited != null);
	$details["is_promoted"] = ($result->is_promoted == 1);

	// posted by
	if ($result->entity_type != "user") {
		$posted_by = user_load($result->poster_uid);
		if ($posted_by != null) {
			$details["posted_by_url"] = "/" . drupal_get_path_alias("user/" . $posted_by->uid);
			$details["posted_by"] = strip_tags($posted_by->field_user_first_name[LANGUAGE_NONE][0]['value'] . " " . $posted_by->field_user_last_name[LANGUAGE_NONE][0]['value']);
		}
	}

	// display more links to logged in user
	if (user_is_logged_in()) {

		// actions actions html
		$details["more_links"] = array();

		// edit link
		if (tm_status_updates_check_permission($result->entity_type, $result->entity_id, "edit", $user->uid)) {
			$details["more_links"][] = "<a href='javascript:void(0);' onClick='tm_user_status_edit_form(" . $result->id . ");'>Edit</a>";
		}

		// delete link
		if (tm_status_updates_check_permission($result->entity_type, $result->entity_id, "remove", $user->uid)) {

			$details["more_links"][] = "<a href='javascript:void(0);' onClick='tm_user_status_remove_confirm(" . $result->id . ");'>Delete</a>";
		}

		// moderator
		if (tm_status_updates_check_permission($result->entity_type, $result->entity_id, "moderate", $user->uid)) {
			$details["more_links"][] = "<a href='javascript:void(0);' onClick='tm_user_status_moderate_confirm(" . $result->id . ");'>Moderate</a>";
		}

		// promote
		if (tm_status_updates_check_permission($result->entity_type, $result->entity_id, "promote", $user->uid)) {
			if (!$result->is_promoted) {
				$details["more_links"][] = "<a href='javascript:void(0);' onClick='tm_user_status_promote_confirm(" . $result->id . ");'>Promote</a>";
			} else {
				$details["more_links"][] = "<a href='javascript:void(0);' onClick='tm_user_status_unpromote_confirm(" . $result->id . ");'>Unpromote</a>";
			}
		}

		// report if it's not yours
		if (!tm_status_updates_check_permission($result->entity_type, $result->entity_id, "edit", $user->uid)) {

			if ($result->entity_type == "organization") {
				if ($result->entity_id != $user->uid) {
					$community_values_url = $conf["tm_community_values_url"];
					$details["more_links"][] = "<a href='javascript:void(0);' onClick='jq_confirm_report_organization(" . $result->entity_id . ", \"" . $community_values_url . "\");'>Report</a>";
				}
			} else {
				// report posting user
				$community_values_url = $conf["tm_community_values_url"];
				$details["more_links"][] = "<a href='javascript:void(0);' onClick='jq_confirm_report_member(" . $result->poster_uid . ", \"" . $community_values_url . "\");'>Report</a>";
			}
		}

		// don't display menu if there are no actions
		if (sizeof($details["more_links"]) == 0) {
			unset($details["more_links"]);
		}
	}
	
	// preview url
	if ($result->preview_link_id != null) {
		$preview_data = tm_status_updates_get_preview_link($result->preview_link_id);
		$preview_html = tm_status_updates_render_link_preview($preview_data);
		$details["preview_link_html"] = $preview_html;
	}

	// Record visitor ip if set
	if ($count_viewer_ip != null) {
		tm_status_updates_increment_daily_view_count($result->id, $count_viewer_ip);
	}

	// render
	$html = tm_status_updates_render_flag_feed($entity_url, $image_url, $feed_title, $feed_info, $details, $feed_image_class);

	return $html;
}

/**
 * Render flagged item
 */
function tm_status_updates_render_flagged_item($result, $show_you = true, $show_unapproved = false) {

	global $user;
	global $conf;

	// reset vars
	$html = "";
	$feed_info = "";

	$flagged_node = null;
	$flagged_user = null;
	
	// get flag types (store in static var so we don't keep looking up)
	$flag_types = &drupal_static("tm_flag_types");
	if ($flag_types == null) {
		$flag_types = array();
		$all_flags = flag_get_flags();
		foreach ($all_flags as $flag) {
			$flag_types[$flag->fid] = $flag;
		}
	}
	
	// get flag
	$flag = $flag_types[$result->fid];

	// make sure we want to display this flag
	$show_flags = array("approval_approved_by", "event_register", "event_waitlist", "signup", "follow_members", "follow_organizations", "tm_sponsor", "user_subscription_enable", "event_paid");
	if (!in_array($flag->name, $show_flags)) {
		return "";
	}

	if ($result->entity_type == "node") {
		$flagged_node = node_load($result->entity_id);
		$flagged_node_url = drupal_get_path_alias("node/" . $flagged_node->nid);

		// don't show event if it's in preview mode
		if ($flagged_node->type == "event") {
			if (tm_events_is_preview_mode($flagged_node->nid)) {
				return "";
			}
		}

	}
	if ($result->entity_type == "user") {
		$flagged_user = user_load($result->entity_id);
		if (!$show_you or ($flagged_user->uid != $user->uid)) {
			$flagged_user_name = strip_tags($flagged_user->field_user_first_name[LANGUAGE_NONE][0]['value'] . " " . $flagged_user->field_user_last_name[LANGUAGE_NONE][0]['value']);
		} else {
			$flagged_user_name = "you";
		}
		$flagged_user_url = drupal_get_path_alias("user/" . $flagged_user->uid);
	}

	$flagging_user = user_load($result->uid);
	if ($flagging_user == false) {
		return "";
	}
	if (!$show_you or ($flagging_user->uid != $user->uid)) {
		if ($flagging_user->uid > 0) {
			$flagging_user_name = strip_tags($flagging_user->field_user_first_name[LANGUAGE_NONE][0]['value'] . " " . $flagging_user->field_user_last_name[LANGUAGE_NONE][0]['value']);
		} else {
			$flagging_user_name = ""; // Anonymous
		}
	} else {
		$flagging_user_name = "You";
	}
	$flagging_user_url = drupal_get_path_alias("user/" . $flagging_user->uid);
	$flagged_time = format_interval(time() - $result->timestamp, 1) . " ago";

	// allow viewing your own content if you are unapproved
	if ($flagging_user->uid == $user->uid) {
		$show_unapproved = true;
	}

	// skip showing feed for non-approved users
	if (!$show_unapproved) {
		if (!in_array("approved user", $flagging_user->roles))  {
			return "";
		}
	}

	// keep track of last flagged user
	$last_flagging_user_uid = $flagging_user->uid;

	// show when last posted
	$details = array();
	$details["posted_time"] = $flagged_time;

	switch ($flag->name) {

		// NEW MEMBER APPROVED
		case "approval_approved_by":

			// get image url
			$image_uri = _tm_users_get_avatar_uri($flagged_user);
			$image_url = image_style_url("avatar", $image_uri);

			// Show user's home chapter as location
			if (isset($flagged_user->field_home_chapter[LANGUAGE_NONE][0]['target_id'])) {
				$home_chapter = node_load($flagged_user->field_home_chapter[LANGUAGE_NONE][0]['target_id']);

				if ($home_chapter != false) {

					// check if chapter is group
					$chapter_is_group = false;
					if (isset($home_chapter->field_chapter_is_group[LANGUAGE_NONE][0]['value'])) {
						$chapter_is_group = ($home_chapter->field_chapter_is_group[LANGUAGE_NONE][0]['value']);
					}

					// if not group, use the chapter as location
					if (!$chapter_is_group) {
						$details["location_text"] = $home_chapter->title;
						$details["location_url"] = "/" . drupal_get_path_alias("node/" . $home_chapter->nid);
					}
				}
				
			}

			// Show user's location field if no home chapter
			if (!isset($details["location_text"])) {
				if (isset($flagged_user->field_location_city[LANGUAGE_NONE][0]['value'])) {
					$flagged_user_location = $flagged_user->field_location_city[LANGUAGE_NONE][0]['value'];
					$details["location_text"] = $flagged_user_location;
				}
			}

			$feed_title = l($flagged_user_name, $flagged_user_url) . " joined the community";
			$html = tm_status_updates_render_flag_feed($flagged_user_url, $image_url, $feed_title, $feed_info, $details, "status-updates-image-avatar");
			break;

	   	// MEMBER REGISTERED FOR EVENT
	    case "event_register":

			$event = $flagged_node;

			// get image url
			$image_uri = _tm_users_get_avatar_uri($flagging_user);
			$image_url = image_style_url("avatar", $image_uri);

			// if event has chapter
			if (isset($event->field_chapter[LANGUAGE_NONE][0]['target_id'])) {
				$event_chapter = node_load($event->field_chapter[LANGUAGE_NONE][0]['target_id']);
				$details["location_text"] = $event_chapter->title;
				$details["location_url"] = "/" . drupal_get_path_alias("node/" . $event_chapter->nid);
			}

			$feed_title = l($flagging_user_name, $flagging_user_url) . " registered for " . l($event->title, $flagged_node_url);
			$html = tm_status_updates_render_flag_feed($flagging_user_url, $image_url, $feed_title, "", $details, "status-updates-image-avatar");
			break;

		// MEMBER PURCHASED A TICKET TO EVENT
	    case "event_paid":

			$event = $flagged_node;

			// get image url
			$image_uri = _tm_users_get_avatar_uri($flagging_user);
			$image_url = image_style_url("avatar", $image_uri);

			// if event has chapter
			if (isset($event->field_chapter[LANGUAGE_NONE][0]['target_id'])) {
				$event_chapter = node_load($event->field_chapter[LANGUAGE_NONE][0]['target_id']);
				$details["location_text"] = $event_chapter->title;
				$details["location_url"] = "/" . drupal_get_path_alias("node/" . $event_chapter->nid);
			}

			$feed_title = l($flagging_user_name, $flagging_user_url) . " purchased a ticket to " . l($event->title, $flagged_node_url);
			$html = tm_status_updates_render_flag_feed($flagging_user_url, $image_url, $feed_title, "", $details, "status-updates-image-avatar");
			break;

	    // MEMBER REGISTERED FOR WAITLIST
	    case "event_waitlist":

			$event = $flagged_node;

			// get image url
			$image_uri = _tm_users_get_avatar_uri($flagging_user);
			$image_url = image_style_url("avatar", $image_uri);

			// if event has chapter
			if (isset($event->field_chapter[LANGUAGE_NONE][0]['target_id'])) {
				$event_chapter = node_load($event->field_chapter[LANGUAGE_NONE][0]['target_id']);
				$details["location_text"] = $event_chapter->title;
				$details["location_url"] = "/" . drupal_get_path_alias("node/" . $event_chapter->nid);
			}

			$feed_title = l($flagging_user_name, $flagging_user_url) . " joined the waitlist for " . l($event->title, $flagged_node_url);
			$html = tm_status_updates_render_flag_feed($flagging_user_url, $image_url, $feed_title, "", $details, "status-updates-image-avatar");
			break;

	    // MEMBER JOINED CHAPTER
	   	case "signup":

			$chapter = $flagged_node;

			// get image url
			$image_uri = _tm_chapters_get_image_uri($chapter);
			$image_url = image_style_url("banner", $image_uri);

			$feed_title = l($flagging_user_name, $flagging_user_url) . " joined " . l(tm_chapters_formal_name($chapter), $flagged_node_url);
		    $html = tm_status_updates_render_flag_feed($flagged_node_url, $image_url, $feed_title, "", $details);
			break;

	    // MEMBER FOLLOWED SOMEONE
	    case "follow_members":

			// get image url
			$image_uri = _tm_users_get_avatar_uri($flagging_user);
			$image_url = image_style_url("avatar", $image_uri);

			$feed_title = l($flagging_user_name, $flagging_user_url) . " followed " . l($flagged_user_name, $flagged_user_url);
			//$feed_title = l($flagged_user_name, $flagged_user_url) . " was followed by " . l($flagging_user_name, $flagging_user_url);
			$html = tm_status_updates_render_flag_feed($flagging_user_url, $image_url, $feed_title, "", $details, "status-updates-image-avatar");
			break;

	   	// MEMBER FOLLOWED AN ORGANIZATION
	    case "follow_organizations":

	    	$organization = $flagged_node;

			// get image url
			$image_uri = _tm_organizations_get_avatar_uri($organization);
			$image_url = image_style_url("avatar", $image_uri);

			$image_url = image_style_url("avatar", $conf["tm_images_default_field_avatar"]);
			if (isset($organization->field_avatar[LANGUAGE_NONE][0]['uri'])) {
				$image_url = image_style_url("avatar", $organization->field_avatar[LANGUAGE_NONE][0]['uri']);
			}

			// upvote or follow
			$display_upvotes = $conf["tm_organizations_upvote"];
			$organization_action_text = "followed";
			if ($display_upvotes) {
				$organization_action_text = "upvoted";
			}
			$feed_title = l($flagging_user_name, $flagging_user_url) . " " . $organization_action_text . " " . l($organization->title, $flagged_node_url);
			$html = tm_status_updates_render_flag_feed($flagged_node_url, $image_url, $feed_title, "", $details, "status-updates-image-avatar");
			break;

	   	// ORGANIZATION FLAGGED AS A SPONSOR
	   	case "tm_sponsor":

			$organization = $flagged_node;

			// get image url
			$image_uri = _tm_organizations_get_avatar_uri($organization);
			$image_url = image_style_url("avatar", $image_uri);

			$feed_title = l($organization->title, $flagged_node_url) . " just became a sponsor, thanks!";
			
			$html = tm_status_updates_render_flag_feed($flagged_node_url, $image_url, $feed_title, "", $details, "status-updates-image-avatar");
			break;

		// USER HAS SUBSCRIPTION
	   	case "user_subscription_enable":

			// get image url
			$image_uri = _tm_users_get_avatar_uri($flagged_user);
			$image_url = image_style_url("avatar", $image_uri);

			// Show user's home chapter as location
			if (isset($flagged_user->field_home_chapter[LANGUAGE_NONE][0]['target_id'])) {
				$home_chapter = node_load($flagged_user->field_home_chapter[LANGUAGE_NONE][0]['target_id']);

				if ($home_chapter != false) {

					// check if chapter is group
					$chapter_is_group = false;
					if (isset($home_chapter->field_chapter_is_group[LANGUAGE_NONE][0]['value'])) {
						$chapter_is_group = ($home_chapter->field_chapter_is_group[LANGUAGE_NONE][0]['value']);
					}

					// if not group, use the chapter as location
					if (!$chapter_is_group) {
						$details["location_text"] = $home_chapter->title;
						$details["location_url"] = "/" . drupal_get_path_alias("node/" . $home_chapter->nid);
					}
				}
				
			}

			// Show user's location field if no home chapter
			if (!isset($details["location_text"])) {
				if (isset($flagged_user->field_location_city[LANGUAGE_NONE][0]['value'])) {
					$flagged_user_location = $flagged_user->field_location_city[LANGUAGE_NONE][0]['value'];
					$details["location_text"] = $flagged_user_location;
				}
			}

			$feed_title = l($flagged_user_name, $flagged_user_url) . " upgraded to a " . $conf["tm_subscriptions_user_label"];
			$html = tm_status_updates_render_flag_feed($flagged_user_url, $image_url, $feed_title, $feed_info, $details, "status-updates-image-avatar");
			break;

	}

	return $html;
}

/**
 * Helper method to render results list
 */
function tm_status_updates_render_results_items($results, $count_viewer_ip, $hide_repeat_posters = true, $hide_own_repeats = false, $show_you = true, $display_limit = null, $hide_organization_updates = false) {

	global $user;
	global $conf;

	// set display limit
	if ($display_limit == null) {
		$display_limit = $conf["tm_status_updates_items_per_load"];
	}

	// preload event_register flag id
	$event_register_flag = flag_get_flag('event_register');
	$event_register_flag_id = $event_register_flag->fid;

	// preload event_waitlist flag id
	$event_waitlist_flag = flag_get_flag('event_waitlist');
	$event_waitlist_flag_id = $event_waitlist_flag->fid;

	// preload event_paid flag id
	$event_paid_flag = null;
	if (module_exists("tm_payments")) {
		$event_paid_flag = flag_get_flag('event_paid');
		$event_paid_flag_id = $event_paid_flag->fid;	
	}

	// render results
	$html_items = array();
	$count_items = 0;
	$last_user_uid = null;
	$last_post_was_event_register = false;
	foreach($results as $result) {

		$html = "";

		if ($count_items > $display_limit) {
			break;
		}

		// hide organization updates if $hide_organization_updates true
		if ($result->result_type == "status_update") {
			if ($result->status_update_entity_type == "organization") {
				if ($hide_organization_updates) {
					continue;
				}
			}
		}

		// check if item is an event registration or waitlist
		// pre load flagged_item
		$is_event_register_item = false;
		$is_event_item = false;
		if ($result->result_type == "flagged_item") {
			$flagged_item = tm_status_updates_get_flagged_by_id($result->flagging_id);
			if (($flagged_item->fid == $event_register_flag_id) or
			   ($flagged_item->fid == $event_waitlist_flag_id)) {
				$is_event_item = true;
			}
		}

		// determine if we are repeating post
		$own_post = ($user->uid == $result->owner_uid);
		$repeating_post = ($result->owner_uid == $last_user_uid);

		// if repeating post, and last was event register, and this is event payment
		// hide the previous post (register) so the ticket payment is displayed
		// this is because when purchasing a ticket, the flag order is event_register then event_paid
 		if ($repeating_post and $last_post_was_event_register) {
			if ($event_paid_flag != null) {
				if ($flagged_item->fid == $event_paid_flag->fid) {
					array_pop($html_items);
					$repeating_post = false;
				}	
			}
		}

		// hide repeating posts
		// (exception: event registration)
		if ($repeating_post and !$is_event_item) {
			if ($own_post and $hide_own_repeats) {
				continue;
			} elseif (!$own_post and $hide_repeat_posters) {
				continue;
			}
		}

		// hide silenced accounts
		if (in_array($result->owner_uid, $conf["tm_status_updates_silence_uids"])) {
			if (!($show_you and $own_post)) {
				continue;
			}
		}

		// render status update
		if ($result->result_type == "status_update") {
			$status_update = tm_status_updates_get_update_by_id($result->status_update_id);
			$html = tm_status_updates_render_update($status_update, $count_viewer_ip, $show_you);
		}

		// render flagged item
		if ($result->result_type == "flagged_item") {
			$html = tm_status_updates_render_flagged_item($flagged_item, $show_you);
		}

		// keep track of event register
		if ($result->result_type == "flagged_item") {
			$last_post_was_event_register = ($flagged_item->fid == $event_register_flag->fid);
		}

		// track last id
		$last_user_uid = $result->owner_uid;

		if ($html != "") {
			$html_items[] = $html;
			$count_items++;
		}
	}

	// return html
	$feed_html = implode("\n", $html_items);
	return $feed_html;
}

/**
 * Render flag feed from template
 */
function tm_status_updates_render_flag_feed($feed_url, $feed_image_url, $feed_title, $feed_info, $details, $feed_image_class = "status-updates-image") {

	global $tm_status_updates_feed_template;

	// update feed title with posted time
	if (isset($details["posted_time"])) {
		$edited_text = "";
		if (isset($details["is_edited"])) {
			if ($details["is_edited"] == true) {
				$edited_text = " (edited)";
			}
		}
		$feed_title .= "<span class='tm-status-update-separator'>&middot;</span> <span class='tm-status-update-time'>" . $details["posted_time"] . $edited_text . '</span>';
	}

	// posted by
	if (isset($details["posted_by"])) {
		$posted_by_html = $details["posted_by"];
		if (isset($details["posted_by_url"])) {
			$posted_by_html = "<a href='" . $details["posted_by_url"] . "'>" . $details["posted_by"] . "</a>";
		}
		$feed_title .= " <span class='tm-status-update-posted-by'>by " . $posted_by_html . '</span>';
	}

	// preview link
	$preview_link = "";
	if (isset($details["preview_link_html"])) {
		$preview_link = $details["preview_link_html"];
	}

	// add leading / if url is not absolute
	if ((stripos($feed_url, "https://") !== 0) and (stripos($feed_url, "http://") !== 0)
		and (strpos($feed_url, "/") !== 0)) {
		$feed_url = "/" . $feed_url;
	}


	$html = str_replace("__FEED_URL__", $feed_url, $tm_status_updates_feed_template);
	$html = str_replace("__FEED_IMAGE_URL__", $feed_image_url, $html);
	$html = str_replace("__FEED_IMAGE_CLASS__", $feed_image_class, $html);
	$html = str_replace("__FEED_TITLE__", $feed_title, $html);
	$html = str_replace("__FEED_INFO__", $feed_info, $html);
	$html = str_replace("__PREVIEW_LINK__", $preview_link, $html);
	$html = str_replace("__FEED_LI_STYLE__", "display: block;", $html);	
	$html = str_replace("__FEED_LI_CLASS__", "feeditem", $html);
	
	// render details
	$details_display = "none";
	$details_html = "";
	if ($details != null) {
		$details_display = "block";
		$details_html = tm_status_updates_render_details($details);
	}
	$html = str_replace("__DETAILS_DISPLAY__", $details_display, $html);
	$html = str_replace("__DETAILS_HTML__", $details_html, $html);

	return $html;
}

/**
 * Render details html
 * Provide a details array
 */
function tm_status_updates_render_details($details) {

	global $conf;

	// promoted
	$promoted_html = "";
	if (isset($details["is_promoted"])) {
		if ($details["is_promoted"]) {
			$promoted_html = "<span class='tm-status-update-promoted' style='float: left;'>Featured";
			$promoted_html .= "</span>";
		}	
	}

	// location
	$location_html = "";
	if (isset($details["location_text"])) {
		$location_html = "<span class='tm-status-update-location' style='float: left;'>";
		if (isset($details["location_url"])) {
			$location_html .= "<a href='" . $details["location_url"] . "'>" . strip_tags($details["location_text"]) . "</a>";
		} else {
			$location_html .= strip_tags($details["location_text"]);
		}
		$location_html .= "</span>";
	}

	// view count
	$view_count_html = "";
	if (isset($details["view_count"])) {
		$view_count = $details["view_count"];
		if ($view_count > 0) {
			$views_text = "view";
			if ($view_count > 1) {
				$views_text = "views";
			}
			$view_count_class = 'tm-status-update-views';
			if ($view_count > $conf["tm_status_updates_views_popular"]) {
				$view_count_class = 'tm-status-update-popular';
			}
			$view_count_html = "<span class='" . $view_count_class . "' style='float: left;'>" . $view_count . " " . $views_text . "</span>";
		} else {
			$view_count_html = "<span style='float: left;'>&nbsp;</span>";
		}
	}

	// more links
	$more_html = "";
	if (isset($details["more_links"])) {
		$more_html = "<span style='float: left; width: 12px;'>";
		$more_html .= "<li class='tm-status-update-actions-dropdown' data-dropd-wrapper>";

		$more_html .= "<a class='tm-status-update-actions' href='javascript:void(0);' data-dropd-toggle=''></a>";

		// more-actions
		$more_html .= "<div class='inner dropd dropd-s dropd-right' data-dropd='' style='top: 24px; min-width: 8rem;'>";

		// dropd-menu
		$more_html .= "<ul class='tm-status-update-actions-dropdown-menu dropd-menu'>";
		foreach ($details["more_links"] as $item) {
			$more_html .= "<li>" . $item . "</li>";
		}
		$more_html .= "</ul>";
		$more_html .= "</div>"; // more-actions
		$more_html .= "</li>"; // data-dropd-wrapper
		$more_html .= "</span>";
	}

	// return html
	return $promoted_html . $location_html . $view_count_html . $more_html;
}

/**
 * Helper method to render results feed into template
 * Will include the css
 */
function tm_status_updates_render_results_feed($result_html, $title, $css_class = "", $more_url = "", $more_text = "", $include_css = "") {

	global $tm_status_updates_feeds_template;

	$newsfeed_html = str_replace("__FEED_ITEMS__", $result_html, $tm_status_updates_feeds_template);
	$newsfeed_html = str_replace("__FEED_TITLE__", $title, $newsfeed_html);
	
	// more link
	if ($more_url != "") {
		global $tm_status_updates_feeds_more_link_template;
		$newsfeed_html = str_replace("__MORE_LINK__", $tm_status_updates_feeds_more_link_template, $newsfeed_html);
		$newsfeed_html = str_replace("__FEED_VIEW_MORE_URL__", $more_url, $newsfeed_html);
		$newsfeed_html = str_replace("__FEED_VIEW_MORE_TEXT__", $more_text, $newsfeed_html);
	} else {
		$newsfeed_html = str_replace("__MORE_LINK__", "", $newsfeed_html);
	}

	// css
	if ($include_css == "") {
		global $tm_status_updates_feed_template_css;
		$include_css = $tm_status_updates_feed_template_css;
	}

	// css class
	$newsfeed_html = str_replace("__CSS_CLASS__", $css_class, $newsfeed_html);


	return $include_css . "\n" . $newsfeed_html;
}

/**
 * Render link preview
 */
function tm_status_updates_render_link_preview($preview_data) {

	global $tm_status_update_link_preview_template;

	// make sure we have valid data
	if ($preview_data == null) {
		return "";
	}

	// get fields
	$url = $preview_data["link_url"];
	$title = $preview_data["link_og_title"];
	$description = $preview_data["link_og_description"];
	$image_file = $preview_data["image_file"];
	$image_url = file_create_url($image_file);

	// add protocol to url if not already
	if ((stripos($url, "http://") !== 0) and ((stripos($url, "https://") !== 0))) {
		$url = "http://" . $url;
	}

	// display url
	$display_url = $url;
	$url_parts = explode("/", $url);
	if (isset($url_parts[2])) {
		$display_url = str_replace("www.", "", $url_parts[2]);
	}

	// case of no title
	if ($title == tm_status_updates_render_max_length($url, 100)) {
		$title = "";
	}

	// case of no description
	if ($description == "") {
		$description = "No description available";
	}

	$html = str_replace("__PREVIEW_URL__", $url, $tm_status_update_link_preview_template);
	$html = str_replace("__PREVIEW_URL_DISPLAY__", tm_status_updates_render_max_length($display_url, 100), $html);
	$html = str_replace("__PREVIEW_TITLE__", tm_status_updates_render_max_length($title, 100), $html);
	$html = str_replace("__PREVIEW_DESCRIPTION__", tm_status_updates_render_max_length($description, 255), $html);

	$image_url_html = "";
	if ($image_file != null) {
		$image_url_html = '<a target="_blank" rel="nofollow" href="' . $url . '"><img class="tm-status-update-preview-image" align="left" src="' . $image_url . '"></a>';
	}
	$html = str_replace("__PREVIEW_IMAGE_URL__", $image_url_html, $html);

	return $html;
}

/**
 * Render ajax loader
 */
function tm_status_updates_render_loader($timestamp) {

	// check for empty timestamp
	if (($timestamp == "") or ($timestamp == null)) {
		$timestamp = "null";
	}

	// ajax loader
	global $tm_status_updates_loader_template;
	$loader_html = "";
	$loader_html = str_replace("__LOADER_TIMESTAMP__", $timestamp, $tm_status_updates_loader_template);

	return $loader_html;
}

/**
 * Render promoted updates
 */
function tm_status_updates_render_promoted($num_days = 7, $display_limit = 3) {

	// promoted updates
	$promoted_html = "";
	$promoted_updates = tm_status_updates_get_promoted_updates($num_days);
	if ($promoted_updates == null) {
		return "";
	}
	if (sizeof($promoted_updates) == 0) {
		return "";
	}
	shuffle($promoted_updates);
	$promoted_updates = array_slice($promoted_updates, 0, $display_limit);
	if (sizeof($promoted_updates) > 0) {
		$promoted_items_html = tm_status_updates_render_results_items($promoted_updates, null, false, false, false);
		$promoted_html = tm_status_updates_render_results_feed($promoted_items_html, "Featured Posts", "promoted");
	}

	return $promoted_html;
}

/**
 * Render a custom message for the newsfeed
 */
function tm_status_update_render_custom_message($message) {

	global $tm_status_update_custom_message_template;
	$html = str_replace("__MESSAGE__", $message, $tm_status_update_custom_message_template);
	return $html;
}

/**
 * Render login form for the newsfeed
 */
function tm_status_updates_render_login_message() {

	global $conf;

	$signup_message = "Welcome to the " . $conf["tm_site_name"] . " News Feed. Want to contribute to our community? ";
	if (isset($conf["tm_status_updates_signup_message"])) {
		$signup_message = $conf["tm_status_updates_signup_message"];
	}

	return $signup_message;
}

/**
 * Helper method to render text to max length
 */
function tm_status_updates_render_max_length($text, $max_length) {
	if (strlen($text) > $max_length) {
		$pos = strpos($text, ' ', $max_length);
		return substr($text, 0, $pos) . " ...";
	}
	return $text;
}

// TEMPLATES

global $tm_status_updates_feeds_template;
$tm_status_updates_feeds_template = <<<EOT
<section class="contained contained-block status-updates __CSS_CLASS__">
	<div class='newsfeed-close-container' onClick="javascript:this.parentNode.style.display='none';"></div>
	<header class="contained-head">
		<h1 class="prime-title status-updates top">__FEED_TITLE__</h1>
	</header>
	<div class="contained-body status-updates">
		<ul class="status-update-list related-list">
			__FEED_ITEMS__
		</ul>
	</div>
	__MORE_LINK__
</section>
EOT;

global $tm_status_updates_loader_template;
$tm_status_updates_loader_template = <<<EOT
<script>
tm_status_updates_loader_timestamp = __LOADER_TIMESTAMP__;</script>
</script>
<div class="item-list tm-status-updates-loader" style="margin-bottom: 32px;">
	<ul class="pager pager-load-more">
		<li>
			<a class='tm-status-update-pager-link' href="javascript:void(0);" onClick="tm_status_updates_fetch_more();">Load more</a>
			<img class="tm-status-update-pager-loading-image" style="display:none; margin-left: 16px; " src='/sites/all/themes/tm/images/load-more-ajax-loader-2.gif'>
		</li>
	</ul>
</div>
EOT;

global $tm_status_updates_feeds_more_link_template;
$tm_status_updates_feeds_more_link_template = <<<EOT
<div class="more-link" style="font-size: 14px;">
	<a href='__FEED_VIEW_MORE_URL__'>__FEED_VIEW_MORE_TEXT__</a>&nbsp;
</div>
EOT;

global $tm_status_updates_feed_template;
$tm_status_updates_feed_template = <<<EOT
<li class="status-updates __FEED_LI_CLASS__" style="__FEED_LI_STYLE__">
   <article class="card contained view-mode-grid status-updates clearfix">
    	<div class="status-update-container">
    	
	        <div class="media">
		        <div class="avatar">
		        	<span class="badge-status-updates">
		        		 <a href="__FEED_URL__" class="status-updates"><img class="__FEED_IMAGE_CLASS__" typeof="foaf:Image" src="__FEED_IMAGE_URL__" width="256" height="256" alt=""></a>
		        	</span>
		        </div>
	        </div>

	    	<div class="teaser tm-wrap-long-text">
	      		<span class="prime-title status-updates tm-wrap-long-text">__FEED_TITLE__</span>
	      		<p class="meta status-updates"><span class="role">__FEED_INFO__</span></p>
	      	</div>

	      	__PREVIEW_LINK__

	      	<div class="tm-status-update-details tm-wrap-long-text" style="display: __DETAILS_DISPLAY__; float: right; display: inline-block; margin-right: -38px; padding-top: 12px;">
	      		__DETAILS_HTML__
	      	</div>
	     
      	</div>
      	
	</article>
</li>
EOT;

global $tm_status_update_link_preview_template;
$tm_status_update_link_preview_template = <<<EOT
<!--googleoff: index-->
<div class="tm-status-update-preview">
	__PREVIEW_IMAGE_URL__
	<div class='tm-status-update-preview-body tm-wrap-long-text'>
	<h4><a target="_blank" rel="nofollow" href="__PREVIEW_URL__">__PREVIEW_TITLE__</a></h4>
	<span class='tm-status-update-preview-description'>__PREVIEW_DESCRIPTION__</span>
	<br><a class='tm-status-update-preview-link' target="_blank" rel="nofollow" href="__PREVIEW_URL__">(__PREVIEW_URL_DISPLAY__)</a>
	</div>
</div>
<!--googleon: index-->
EOT;

global $tm_status_update_custom_message_template;
$tm_status_update_custom_message_template = <<<EOT
<section class="contained contained-block newsfeed">
<div class="contained-body newsfeed">
	<p style="margin-left: 16px; margin-right: 8px; padding: 16px;">
		__MESSAGE__
	</p>
</div>
</section>
EOT;

global $tm_status_updates_feed_template_css;
$tm_status_updates_feed_template_css = <<<EOT
<style>
.contained-block.newsfeed { overflow: visible; }
.contained-block.promoted { overflow: visible; }
.status-update-container { padding-left: 1rem; padding-top: 0.5rem; padding-right: 4.2em; }
.prime-title.status-updates a:hover { color: #3080b2;}
.prime-title.status-updates a { text-decoration: underline;}
.card.status-updates { padding-top: 0.5rem; padding-bottom: 0.5rem;}
.card.status-updates:hover { background-color: #e7f2f7; }
img.status-updates-image { max-height: 64px; max-width: 64px; border-radius: 4px; }
img.status-updates-image-avatar { max-height: 64px; max-width: 64px; border-radius: 50%; }
.status-updates.newsfeed { border-left: 8px solid #488bf8; }
.status-updates.promoted { border-left: 8px solid #FC389A; }
</style>
EOT;

global $tm_status_updates_feed_template_frontpage_css;
$tm_status_updates_feed_template_frontpage_css = <<<EOT
<style>
.status-update-container { padding-left: 1rem; padding-top: 0.5rem; padding-right: 4.2em; }
.prime-title.status-updates a:hover { color: #3080b2;}
.prime-title.status-updates a { text-decoration: underline;}
.card.status-updates { padding-top: 0.5rem; padding-bottom: 0.5rem;}
.card.status-updates:hover { background-color: #e7f2f7;}
img.status-updates-image { max-height: 64px; max-width: 64px; border-radius: 4px; }
img.status-updates-image-avatar { max-height: 64px; max-width: 64px; border-radius: 50%; }
</style>
EOT;

global $tm_status_updates_feed_template_profile_css;
$tm_status_updates_feed_template_profile_css = <<<EOT
<style>
.status-update-container { padding-left: 1rem; padding-top: 0.5rem; padding-right: 4.2em; }
.prime-title.status-updates a:hover { color: #3080b2;}
.prime-title.status-updates a { text-decoration: underline;}
.card.status-updates { padding-top: 0.5rem; padding-bottom: 0.5rem;}
img.status-updates-image { max-height: 64px; max-width: 64px; border-radius: 4px; }
img.status-updates-image-avatar { max-height: 64px; max-width: 64px; border-radius: 50%; }
li.status-updates.feeditem { background-color: #fff; border-radius: 4px; box-shadow: 0 0.125rem 0 0 rgba(33, 48, 64, 0.1);}
.tm-status-update-details { font-size: 14px; }
</style>
EOT;
