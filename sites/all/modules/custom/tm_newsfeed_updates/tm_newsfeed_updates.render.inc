<?php

// tm_newsfeeds_updates.render.inc

/**
 * Render flagged item
 */
function tm_newsfeed_updates_render_flagged_item($result, $show_you = true, $show_unapproved = false) {

	global $user;
	global $conf;

	// reset vars
	$html = "";
	$feed_info = "";

	$flagged_node = null;
	$flagged_user = null;
	
	// load flags
	$flag_types = array();
	$all_flags = flag_get_flags();
	foreach ($all_flags as $flag) {
		$flag_types[$flag->fid] = $flag;
	}
	
	// get flag
	$flag = $flag_types[$result->fid];

	// make sure we want to display this flag
	$show_flags = array("approval_approved_by", "event_register", "event_waitlist", "signup", "follow_members", "follow_organizations", "tm_sponsor", "user_subscription_enable");
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
			$html = tm_newsfeed_updates_render_flag_feed($flagged_user_url, $image_url, $feed_title, $feed_info, $details, "newsfeed-updates-image-avatar");
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
			$html = tm_newsfeed_updates_render_flag_feed($flagging_user_url, $image_url, $feed_title, "", $details, "newsfeed-updates-image-avatar");
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
			$html = tm_newsfeed_updates_render_flag_feed($flagging_user_url, $image_url, $feed_title, "", $details, "newsfeed-updates-image-avatar");
			break;

	    // MEMBER JOINED CHAPTER
	   	case "signup":

			$chapter = $flagged_node;

			// get image url
			$image_uri = _tm_chapters_get_image_uri($chapter);
			$image_url = image_style_url("banner", $image_uri);

			$feed_title = l($flagging_user_name, $flagging_user_url) . " joined " . l(tm_chapters_formal_name($chapter), $flagged_node_url);
		    $html = tm_newsfeed_updates_render_flag_feed($flagged_node_url, $image_url, $feed_title, "", $details);
			break;

	    // MEMBER FOLLOWED SOMEONE
	    case "follow_members":

			// get image url
			$image_uri = _tm_users_get_avatar_uri($flagging_user);
			$image_url = image_style_url("avatar", $image_uri);

			$feed_title = l($flagging_user_name, $flagging_user_url) . " followed " . l($flagged_user_name, $flagged_user_url);
			//$feed_title = l($flagged_user_name, $flagged_user_url) . " was followed by " . l($flagging_user_name, $flagging_user_url);
			$html = tm_newsfeed_updates_render_flag_feed($flagging_user_url, $image_url, $feed_title, "", $details, "newsfeed-updates-image-avatar");
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
			$html = tm_newsfeed_updates_render_flag_feed($flagged_node_url, $image_url, $feed_title, "", $details, "newsfeed-updates-image-avatar");
			break;

	   	// ORGANIZATION FLAGGED AS A SPONSOR
	   	case "tm_sponsor":

			$organization = $flagged_node;

			// get image url
			$image_uri = _tm_organizations_get_avatar_uri($organization);
			$image_url = image_style_url("avatar", $image_uri);

			$feed_title = l($organization->title, $flagged_node_url) . " just became a sponsor, thanks!";
			
			$html = tm_newsfeed_updates_render_flag_feed($flagged_node_url, $image_url, $feed_title, "", $details, "newsfeed-updates-image-avatar");
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
			$html = tm_newsfeed_updates_render_flag_feed($flagged_user_url, $image_url, $feed_title, $feed_info, $details, "newsfeed-updates-image-avatar");
			break;

	}

	return $html;
}

/**
 * Render flag feed from template
 */
function tm_newsfeed_updates_render_flag_feed($feed_url, $feed_image_url, $feed_title, $feed_info, $details, $feed_image_class = "newsfeed-updates-image") {

	global $tm_newsfeed_updates_feed_template;

	// update feed title with posted time
	/*if (isset($details["posted_time"])) {
		$edited_text = "";
		if (isset($details["is_edited"])) {
			if ($details["is_edited"] == true) {
				$edited_text = " (edited)";
			}
		}
		$feed_title .= "<span class=\"tm-newsfeed-update-separator\">&middot;</span> <span class=\"tm-newsfeed-update-time\">" . $details["posted_time"] . $edited_text . '</span>';
	}*/

	// posted by
	if (isset($details["posted_by"])) {
		$posted_by_html = $details["posted_by"];
		if (isset($details["posted_by_url"])) {
			$posted_by_html = "<a href='" . $details["posted_by_url"] . "'>" . $details["posted_by"] . "</a>";
		}
		$feed_title .= " <span class=\"tm-newsfeed-update-posted-by\">by " . $posted_by_html . '</span>';
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


	$html = str_replace("__FEED_URL__", $feed_url, $tm_newsfeed_updates_feed_template);
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

global $tm_newsfeed_updates_feed_template;
$tm_newsfeed_updates_feed_template = <<<EOT

<div class="newsfeed-update-container">

    <div class="media">
        <div class="avatar">
        	<span class="badge-newsfeed-updates">
        		 <a href="__FEED_URL__" class="newsfeed-updates"><img class="__FEED_IMAGE_CLASS__" typeof="foaf:Image" src="__FEED_IMAGE_URL__" width="32" height="32" alt=""></a>
        	</span>
        </div>
    </div>

	<div class="teaser tm-wrap-long-text">
  		<span class="prime-title newsfeed-updates tm-wrap-long-text">__FEED_TITLE__</span>
  	</div>
 
</div>

EOT;

