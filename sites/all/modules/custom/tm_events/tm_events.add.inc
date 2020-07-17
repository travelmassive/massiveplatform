<?php 

// tm_events.add.inc

/**
 * Render html for add event
 */
function tm_events_add_page() {

	global $user;
	global $conf;

	// user logged in
	if (!user_is_logged_in()) {
		drupal_set_message("Please <a href='/user/login?destination=/events/add'>log in</a> to your account to add an event.");
		// create form
		$form['html'] = array(
			'#type' => 'item',
			'#markup' => "",
			);
		return $form;
	}

	// get messages
	drupal_get_messages();

	// remove last_edited_event cookie
	user_cookie_delete("last_edited_event");

	$company_message = "";
	$member_message = "";

	$html = "<style>.column { padding: 0; }</style>";
	$html .= "<div class='contained contained-block'>";
	$html .= "<ul>";

	// Check if can create chapter event
	$chapter_nids = tm_users_get_chapters_leading($user, true);
	$add_chapter_event = false;
	foreach ($chapter_nids as $nid) {
		if (_tm_events_check_create_chapter_event($user, $nid)) {
			$chapter = node_load($nid);
			$chapter_title = tm_chapters_formal_name($chapter);
			$html .= "<li><a href='/node/add/event?field_chapter=" . $nid . "'>Add event in " . $chapter_title . "</a></li>";
			$add_chapter_event = true;
		}
	}

	// Check if can create company event
	$company_nids = tm_organizations_get_users_companies($user->uid);
	foreach ($company_nids as $nid) {
		$company = node_load($nid);
		$company_title = strip_tags(trim($company->title));
		if (_tm_events_check_create_company_event($user, $nid)) {	
			$html .= "<li><a href='/node/add/event?field_event_company=" . $nid . "'>Add event for " . $company_title . "</a></li>";
		}
		else {
			$company_event_message = _tm_events_check_create_company_event_message($user, $nid);
			drupal_add_js(array('tm_events' => array('create_company_event_message' => $company_event_message)), array('type' => 'setting'));
			if ($company_event_message != "") {
				$html .= "<li><a href='javascript:jq_create_company_event_message();'>Add event for " . $company_title . "</a></li>";
			}
		}
	}

	// Check if can create member event
	// Hide for chapter leaders
	$can_create_member_event = false;
	if ((!$add_chapter_event) or (in_array('moderator', array_values($user->roles)))) {
		$can_create_member_event = _tm_events_check_create_member_event($user);
		if ($can_create_member_event) {
			$html .= "<li><a href='/node/add/event'>Add member event</a></li>";
		} else {
			$member_event_message = _tm_events_check_create_member_event_message($user);
			drupal_add_js(array('tm_events' => array('create_member_event_message' => $member_event_message)), array('type' => 'setting'));
			if ($member_event_message != "") {
				$html .= "<li><a href='javascript:jq_create_member_event_message();'>Add member event</a></li>";
			}
		}
	}

	// List any upcoming events that the member can edit (including preview mode)
	$upcoming_events = tm_events_get_users_future_events($user->uid, $add_chapter_event);
	$can_edit_upcoming_events = false;
	if (sizeof($upcoming_events) > 0) {
		
		$html .= "<hr style='border: none; border-bottom: 1px solid #ddd; width: 95%'>";

		$account = user_load($user->uid);
		foreach ($upcoming_events as $nid) {

			$event = node_load($nid);

			// check valid event
			if ($event == null) {
				continue;
			}

			// check can edit event
			if (!tm_events_check_permission_edit_event($account, $event)) {
				continue;
			}

			// can edit an upcoming event
			$can_edit_upcoming_events = true;

			// how long until event
			$event_extra_text = "";
			$event_date = field_get_items('node',$event,'field_event_date');
			$current_date = new DateTime('now', new DateTimeZone('UTC'));
			$event_timezone = _tm_get_event_timezone($event);
			$event_datetime = new DateTime($event_date[0]['value'], new DateTimeZone($event_timezone));
			if (($event_datetime->format('U') - $current_date->format('U')) > 0) {
				$event_extra_text =  "in " . _tm_event_time_elapsed($event_datetime->format('U') - $current_date->format('U'));
			}

			// get preview mode
			if (!tm_events_is_preview_mode($nid)) {
				$event_extra_text .=" (Preview mode)";
			}

			// display menu item
			$event_menu_link = drupal_get_path_alias("node/" . $event->nid);
			$html .= "<li><a href='/" . $event_menu_link . "'>" . t($event->title) . "</a> <span style='color: #808080;'>" . $event_extra_text . "</span></li>"; 
		}
	}

	// Redirect if no editable events
	if (!$can_edit_upcoming_events) {

		// If user can only create chapter event, go directly to event page
		if ((!$can_create_member_event) and (sizeof($company_nids) == 0) and (sizeof($chapter_nids) == 1)) {
			drupal_goto("/node/add/event", array("query" => array("field_chapter" => $chapter_nids[0])));
		}

		// If user can only create member event, go directly to event page
		if (($can_create_member_event) and (sizeof($company_nids) == 0) and (sizeof($chapter_nids) == 0)) {
			drupal_goto("/node/add/event");
		}
	}

	// create tip
	$tip = "Add an event for our community.";
	if (isset($conf["tm_event_add_event_help_tip"])) {
		$tip = $conf["tm_event_add_event_help_tip"];
		// lookup and replace stats from api data
        if (module_exists("tm_api")) {
          $api_data = tm_api_stats_get_data();
          $tip = tm_api_replace_text_fields($api_data, $tip);
        }
	}

	// show message, or tip
	if ($tip != "") {
		drupal_set_message($tip, "help-tip");
	}
	
	$html .= "</ul>";
	$html .= "</div>";

	// create form
	$form['html'] = array(
		'#type' => 'item',
		'#markup' => $html,
	);
  	return $form;

}