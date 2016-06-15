<?php

// tm_status_updated.render.inc - render methods for status updates

/**
 * Render most recent status update on node or user
 * Called from theme template
 */
function tm_status_updates_render_theme() {

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

	// fetch latest status update
	$result = tm_status_updates_get_latest_update($entity_id, $entity_type);

	// no update
	if ($result == null) {
		return;
	}

	// check past X days (need conf)

	// render status update
	$html = tm_status_updates_render_update($result);

	// add profile css
	global $tm_status_updates_feed_template_profile_css;

	return $tm_status_updates_feed_template_profile_css . "\n" . $html;
}

/**
 * Render a status update
 */
function tm_status_updates_render_update($result, $show_repeat_user = false, $show_unapproved = false, &$last_flagging_user_uid = null) {

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

	// ensure posting user is approved
	if (!$show_unapproved) {
		if (!in_array("approved user", $posting_user->roles))  {
			return "";
		}
	}

	// member who posted
	if ($posting_user->uid != $user->uid) {
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
  	$status_update_html = tm_base_linkify(strip_tags($result->status_update), array('http', 'https'), array("rel" => "nofollow", "target" => "_blank"));

  	// feed text
  	$feed_info = "<span class='status_update'>" . $status_update_html;

	// user update
	if ($entity_type == "user") {

		$image_uri = _tm_users_get_avatar_uri($posting_user);
		$image_url = image_style_url("avatar", $image_uri);
		$feed_title = l($posting_user_name, $entity_url) . " posted an update";
	}

	// organization update
	if ($entity_type == "organization") {

		$image_uri = _tm_organizations_get_avatar_uri($entity);
		$image_url = image_style_url("avatar", $image_uri);
		$feed_title = l($entity->title, $entity_url) . " posted an update";
	}

	// chapter update
	if ($entity_type == "chapter") {

		$image_uri = _tm_chapters_get_image_uri($entity);
		$image_url = image_style_url("banner", $image_uri);
		$feed_title = l(tm_chapters_formal_name($entity), $entity_url) . " posted an update";
	}

	// event update
	if ($entity_type == "event") {
		// not implemented yet
		return "";
	}

	// show actions if it's your post
	$actions_display = "none";
	$actions_html = "<span class='status_update_ago'>" . $posted_time . "</span>";
	if (tm_status_updates_check_permission($result->entity_type, $result->entity_id, "edit", $user->uid)) {
		$actions_display = "block";
		$actions_html .= " <a href='' style='color: #888; text-decoration: underline;'>edit</a>";
	}

	// render
	$html = tm_status_updates_render_flag_feed($entity_url, $image_url, $feed_title, $feed_info, $actions_display, $actions_html);

	return $html;

}

/**
 * Render flagged item
 */
function tm_status_updates_render_flagged_item($result, $show_repeat_user = false, $show_unapproved = false, &$last_flagging_user_uid = null) {

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
	$show_flags = array("approval_approved_by", "event_register", "event_waitlist", "signup", "follow_members", "follow_organizations", "tm_sponsor");
	if (!in_array($flag->name, $show_flags)) {
		return "";
	}

	if ($result->entity_type == "node") {
		$flagged_node = node_load($result->entity_id);
		$flagged_node_url = drupal_get_path_alias("node/" . $flagged_node->nid);
	}
	if ($result->entity_type == "user") {
		$flagged_user = user_load($result->entity_id);
		$flagged_user_name = strip_tags($flagged_user->field_user_first_name[LANGUAGE_NONE][0]['value'] . " " . $flagged_user->field_user_last_name[LANGUAGE_NONE][0]['value']);
		$flagged_user_url = drupal_get_path_alias("user/" . $flagged_user->uid);
	}

	$flagging_user = user_load($result->uid);
	$flagging_user_name = strip_tags($flagging_user->field_user_first_name[LANGUAGE_NONE][0]['value'] . " " . $flagging_user->field_user_last_name[LANGUAGE_NONE][0]['value']);
	$flagging_user_url = drupal_get_path_alias("user/" . $flagging_user->uid);
		$flagged_time = format_interval(time() - $result->timestamp, 1) . " ago";

	// skip showing feed for non-approved users
	if (!$show_unapproved) {
		if (!in_array("approved user", $flagging_user->roles))  {
			return "";
		}
	}

	// skip showing if same user has flagged multiple times
	// exception of approval flag
	if (!$show_repeat_user && ($flag->name != "approval_approved_by")) {
		if ($last_flagging_user_uid == $flagging_user->uid) {
			$last_flagging_user_uid = $flagging_user->uid;
			return "";
		}
	}

	// keep track of last flagged user
	$last_flagging_user_uid = $flagging_user->uid;

	switch ($flag->name) {

		// NEW MEMBER APPROVED
		case "approval_approved_by":

			// get image url
			$image_uri = _tm_users_get_avatar_uri($flagged_user);
			$image_url = image_style_url("avatar", $image_uri);

			if (isset($flagged_user->field_location_city[LANGUAGE_NONE][0]['value'])) {
				$flagged_user_location = $flagged_user->field_location_city[LANGUAGE_NONE][0]['value'];
				$feed_info = "<span class='time_ago'>" . $flagged_time . "</span>, " . strip_tags($flagged_user_location);
			} else {
				$feed_info = "<span class='time_ago'>" . $flagged_time . "</span>";
			}

			$feed_title = l($flagged_user_name, $flagged_user_url) . " joined the community";
			$html = tm_status_updates_render_flag_feed($flagged_user_url, $image_url, $feed_title, $feed_info);
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
			$feed_info = "<span class='time_ago'>" . $flagged_time . "</span>, in " . $event_chapter->title;
			} else {
				$feed_info = "<span class='time_ago'>" . $flagged_time . "</span>";
			}

			$feed_title = l($flagging_user_name, $flagging_user_url) . " registered for " . l($event->title, $flagged_node_url);
			$html = tm_status_updates_render_flag_feed($flagging_user_url, $image_url, $feed_title, $feed_info);
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
			$feed_info = "<span class='time_ago'>" . $flagged_time . "</span>, in " . $event_chapter->title;
			} else {
				$feed_info = "<span class='time_ago'>" . $flagged_time . "</span>";
			}

			$feed_title = l($flagging_user_name, $flagging_user_url) . " joined the waitlist for " . l($event->title, $flagged_node_url);
			$html = tm_status_updates_render_flag_feed($flagging_user_url, $image_url, $feed_title, $feed_info);
			break;

	    // MEMBER JOINED CHAPTER
	   	case "signup":

			$chapter = $flagged_node;

			// get image url
			$image_uri = _tm_chapters_get_image_uri($chapter);
			$image_url = image_style_url("banner", $image_uri);

			$feed_info = "<span class='time_ago'>" . $flagged_time . "</span>";
			$feed_title = l($flagging_user_name, $flagging_user_url) . " joined " . l($chapter->title . " " . $conf["tm_site_name"], $flagged_node_url);
		    $html = tm_status_updates_render_flag_feed($flagged_node_url, $image_url, $feed_title, $feed_info);
			break;

	    // MEMBER FOLLOWED SOMEONE
	    case "follow_members":

			// get image url
			$image_uri = _tm_users_get_avatar_uri($flagging_user);
			$image_url = image_style_url("avatar", $image_uri);

			$feed_info = "<span class='time_ago'>" . $flagged_time . "</span>";
			$feed_title = l($flagging_user_name, $flagging_user_url) . " followed " . l($flagged_user_name, $flagged_user_url);
			//$feed_title = l($flagged_user_name, $flagged_user_url) . " was followed by " . l($flagging_user_name, $flagging_user_url);
			$html = tm_status_updates_render_flag_feed($flagged_user_url, $image_url, $feed_title, $feed_info);
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

			$feed_info = "<span class='time_ago'>" . $flagged_time . "</span>";
			$feed_title = l($flagging_user_name, $flagging_user_url) . " followed " . l($organization->title, $flagged_node_url);
			$html = tm_status_updates_render_flag_feed($flagged_node_url, $image_url, $feed_title, $feed_info);
			break;

	   	// ORGANIZATION FLAGGED AS A SPONSOR
	   	case "tm_sponsor":

			$organization = $flagged_node;

			// get image url
			$image_uri = _tm_organizations_get_avatar_uri($organization);
			$image_url = image_style_url("avatar", $image_uri);

			$feed_info = "<span class='time_ago'>" . $flagged_time . "</span>";
			$feed_title = l($organization->title, $flagged_node_url) . " just became a sponsor, thanks!";
			$html = tm_status_updates_render_flag_feed($flagged_node_url, $image_url, $feed_title, $feed_info);
			break;
	}	

	return $html;
}

/**
 * Helper method to render results list
 */
function tm_status_updates_render_results_items($results, $display_limit = 1000) {

	global $conf;

	// render results
	$html_items = array();
	$count_items = 0;
	foreach($results as $result) {

		if ($count_items > $display_limit) {
			break;
		}

		// status update
		if (isset($result->created)) {

			// hide spammers
			if (in_array($result->poster_uid, $conf["tm_status_updates_silence_uids"])) {
				continue;
			}

			$html = tm_status_updates_render_update($result);
		}

		// flagged item
		if (isset($result->fid)) {

			// hide spammers
			if (in_array($result->uid, $conf["tm_status_updates_silence_uids"])) {
				continue;
			}

			$html = tm_status_updates_render_flagged_item($result);
		}

		if ($html != "") {
			$html_items[] = $html;
		}

		$count_items++;
	}

	// return html
	$feed_html = implode("\n", $html_items);
	return $feed_html;
}

/**
 * Render flag feed from template
 */
function tm_status_updates_render_flag_feed($feed_url, $feed_image_url, $feed_title, $feed_info, $actions_display = "none", $actions_html = "") {
	global $tm_status_updates_feed_template;
	$html = str_replace("__FEED_URL__", $feed_url, $tm_status_updates_feed_template);
	$html = str_replace("__FEED_IMAGE_URL__", $feed_image_url, $html);
	$html = str_replace("__FEED_TITLE__", $feed_title, $html);
	$html = str_replace("__FEED_INFO__", $feed_info, $html);
	$html = str_replace("__FEED_LI_STYLE__", "display: block;", $html);	
	$html = str_replace("__FEED_LI_CLASS__", "feeditem", $html);
	$html = str_replace("__ACTIONS_DISPLAY__", $actions_display, $html);
	$html = str_replace("__ACTIONS_HTML__", $actions_html, $html);

	return $html;
}

/**
 * Helper method to render results feed into template
 * Will include the css
 */
function tm_status_updates_render_results_feed($result_html, $more_url = "", $more_text = "", $include_css = "") {

	global $tm_status_updates_feeds_template;
	$newsfeed_html = str_replace("__FEED_ITEMS__", $result_html, $tm_status_updates_feeds_template);
	$newsfeed_html = str_replace("__FEED_TITLE__", "Latest Updates", $newsfeed_html);
	
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

	return $include_css . "\n" . $newsfeed_html;
}

// TEMPLATES

global $tm_status_updates_feeds_template;
$tm_status_updates_feeds_template = <<<EOT
<section class="contained contained-block status-updates">
	<header class="contained-head">
		<h1 class="prime-title status-updates top">__FEED_TITLE__</h1>
	</header>
	<div class="contained-body status-updates">
		<ul class="user-list related-list">
			__FEED_ITEMS__
		</ul>
	</div>
	__MORE_LINK__
</section>
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
    	<div style="padding-left: 2em; padding-right: 4em;">
    	
	        <div class="media">
		        <div class="avatar">
		        	<span class="badge-status-updates">
		        		 <a href="__FEED_URL__" class="status-updates"><img class="status-updates-image" typeof="foaf:Image" src="__FEED_IMAGE_URL__" width="256" height="256" alt=""></a>
		        	</span>
		        </div>
	        </div>

	    	<div class="teaser">
	      		<span class="prime-title status-updates">__FEED_TITLE__</span>
	      		<p class="meta status-updates"><span class="role">__FEED_INFO__</span>
	      	</div>

	      	<div class="actions" style="display: __ACTIONS_DISPLAY__; float: right; font-size: smaller;">
	      		__ACTIONS_HTML__
	      	</div>
	     
      	</div>
      	
	</article>
</li>
EOT;

global $tm_status_updates_feed_template_css;
$tm_status_updates_feed_template_css = <<<EOT
<style>
.prime-title.status-updates a:hover { color: #3080b2;}
.prime-title.status-updates a { text-decoration: underline;}
.card.status-updates { padding-top: 0.5rem; padding-bottom: 0.5rem;}
.card.status-updates:hover { background-color: #e7f2f7;}
img.status-updates-image { max-height: 64px; max-width: 64px;}
</style>
EOT;

global $tm_status_updates_feed_template_frontpage_css;
$tm_status_updates_feed_template_frontpage_css = <<<EOT
<style>
.prime-title.status-updates a:hover { color: #3080b2;}
.prime-title.status-updates a { text-decoration: underline;}
.card.status-updates { padding-top: 0.5rem; padding-bottom: 0.5rem;}
.card.status-updates:hover { background-color: #e7f2f7;}
img.status-updates-image { max-height: 64px; max-width: 64px;}
</style>
EOT;

global $tm_status_updates_feed_template_profile_css;
$tm_status_updates_feed_template_profile_css = <<<EOT
<style>
.prime-title.status-updates a:hover { color: #3080b2;}
.prime-title.status-updates a { text-decoration: underline;}
.card.status-updates { padding-top: 0.5rem; padding-bottom: 0.5rem;}
img.status-updates-image { max-height: 64px; max-width: 64px;}
</style>
EOT;
