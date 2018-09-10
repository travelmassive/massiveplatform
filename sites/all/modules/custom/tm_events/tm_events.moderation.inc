
<?php

// tm_events.moderation.inc- event moderation methods

/**
 * Modify users account content before being displayed
 */
function tm_events_user_view($account, $view_mode, $langcode) {

  global $user;

  // Add moderation tools for moderator and chapter leader 
  if ($view_mode == "full") {
  	$account = user_load($user->uid);
	if ((in_array("chapter leader", $account->roles) or (in_array("moderator", $account->roles)))) {
		drupal_add_js(drupal_get_path('module', 'tm_events') . '/js/tm_events_moderation.js');
	}
  }
}

/**
 * Register a member member to a event callback
 * examples:
 * events/moderation-event-add-member/10507/13737?message=welcome
 */
function tm_events_moderation_events_register_member_callback() {

	global $user;
	$moderator = user_load($user->uid);

	// user to moderate
	$user_to_moderate = user_load(arg(2));

	// event to add
	$register_to_event = node_load(arg(3));

	tm_events_moderation_events_register_member($user_to_moderate, $register_to_event, $moderator);
}

/**
 * Register member to an event
 */
function tm_events_moderation_events_register_member($user_to_moderate, $register_to_event, $moderator, $is_console = false) {

	// check permissions
	if ((!in_array("chapter leader", $moderator->roles) and (!in_array("moderator", $moderator->roles)))) {
		if ($is_console) { return false; }
		drupal_set_message(t('You don\'t have permission to peform this action.'), 'error');
		drupal_goto("/");
	}

	// check user and event
	if (($user_to_moderate == null) or ($register_to_event == null)) {
		if ($is_console) { return false; }
		drupal_set_message(t('Oops, an error occurred performing the action (not a valid selection).'), 'error');
		drupal_goto("/" . drupal_get_path_alias("user/" . $user_to_moderate->uid));
	}

	// check user not registered for event already
	$registered_events = tm_users_get_users_events($user_to_moderate);
	if (in_array($register_to_event->nid, $registered_events)) {
		if ($is_console) { return false; }
		drupal_set_message(t('This member is already registered to <i>' . strip_tags($register_to_event->title) . '</i>.'), 'error');
		drupal_goto("/" . drupal_get_path_alias("user/" . $user_to_moderate->uid));
	}

	// check user not waitlisted for event already
	$waitlisted_events = tm_users_get_users_events_waitlisted($user_to_moderate);
	if (in_array($register_to_event->nid, $waitlisted_events)) {
		if ($is_console) { return false; }
		drupal_set_message(t('This member is already waitlisted to <i>' . strip_tags($register_to_event->title) . '</i>.'), 'error');
		drupal_goto("/" . drupal_get_path_alias("user/" . $user_to_moderate->uid));
	}

	// user must be chapter leader, community manager, or moderator of the event's chapter
	if (!in_array("moderator", $moderator->roles)) {

		// check the chapter
		$chapter_id = _tm_event_get_chapter_id($register_to_event);
		if ($chapter_id != null) {
			if (!tm_users_is_chapter_leader($moderator->uid, $chapter_id, $check_community_manager = true)) {
				if ($is_console) { return false; }
				drupal_set_message(t('You don\'t have permission to moderate this member.'), 'error');
				drupal_goto("/" . drupal_get_path_alias("user/" . $user_to_moderate->uid));
			}
		} else {
			// event has no chapter, not a chapter leader
			if ($is_console) { return false; }
			drupal_set_message(t('You don\'t have permission to moderate this member.'), 'error');
			drupal_goto("/" . drupal_get_path_alias("user/" . $user_to_moderate->uid));
		}
	}
	
	// add message into member register email
	// set static variables for tm_notifications_chapter_flag_flag():
	// tm_add_member_moderator_first_name
	// tm_add_member_moderator_last_name

	// set static variables for tm_notifications_events_handle_flagging():
	// tm_register_member_moderator_first_name
	// tm_register_member_moderator_last_name
	// tm_register_moderator_member_message

	// moderator first name
	$moderator_first_name = strip_tags($moderator->field_user_first_name[LANGUAGE_NONE][0]['value']);
	$static_add_member_moderator_first_name = &drupal_static("tm_add_member_moderator_first_name");
	$static_add_member_moderator_first_name = $moderator_first_name;
	$static_register_member_moderator_first_name = &drupal_static("tm_register_member_moderator_first_name");
	$static_register_member_moderator_first_name = $moderator_first_name;

	// moderator last name
	$moderator_last_name = strip_tags($moderator->field_user_last_name[LANGUAGE_NONE][0]['value']);
	$static_add_member_moderator_last_name = &drupal_static("tm_add_member_moderator_last_name");
	$static_add_member_moderator_last_name = $moderator_last_name;
	$static_register_member_moderator_last_name = &drupal_static("tm_register_member_moderator_last_name");
	$static_register_member_moderator_last_name = $moderator_last_name;

	if (isset($_GET["message"])) {
		$message = strip_tags(trim($_GET["message"]));
		if ($message != "") {
			// moderator message
			$message_html = "<br><i>Message from " . $moderator_first_name . ":</i><p><i>";
			$message_html .= $message;
			$message_html .= "</i></p>";

			$static_register_member_moderator_message = &drupal_static("tm_register_member_moderator_message");
      		$static_register_member_moderator_message = $message_html;
		}
	}

	// get full name
	$first_name = strip_tags($user_to_moderate->field_user_first_name[LANGUAGE_NONE][0]['value']);
	$last_name = strip_tags($user_to_moderate->field_user_last_name[LANGUAGE_NONE][0]['value']);
	$full_name = $first_name . " " . $last_name;

	// log in watchdog
	watchdog('tm_events', 'Member ' . $full_name . ' (uid: ' . $user_to_moderate->uid . ') registered to event ' . $register_to_event->title . ' (nid: ' . $register_to_event->nid . ') by ' . $moderator_first_name . ' (uid: ' . $moderator->uid . ')');

	// register to event
    $event_register_flag = flag_get_flag('event_register');
    if (!$event_register_flag->is_flagged($register_to_event->nid, $user_to_moderate->uid)) {
      flag('flag', 'event_register', $register_to_event->nid, $user_to_moderate);
    }

    // console return
    if ($is_console) { return true; }
    
	// notify moderator and redirect to members 
	$event_url = drupal_get_path_alias("node/" . $register_to_event->nid);
    drupal_set_message($full_name . ' has been registered for <a href="/' . $event_url . '">' . strip_tags($register_to_event->title) . '</a>.');
    drupal_goto("user/" . $user_to_moderate->uid);
}

/**
 * Render html options list for event moderation
 * examples:
 * /events/moderation-event-list-ajax/10507/register
 */
function tm_events_moderation_events_list_ajax() {

	global $user;
	$account = user_load($user->uid);

	// user to moderate
	$user_to_moderate = user_load(arg(2));

	// moderation_mode - add or remove
	$moderation_mode = arg(3);

	// get list of chapters user is leading or is community manager of
	$chapters = tm_users_get_chapters_leading($user, true);

	// if user is moderator
	$is_moderator = in_array("moderator", $account->roles);

	// get list of upcoming events
	if ($is_moderator) {
		$chapters = null; // all chapters
	}
	$upcoming_events = tm_events_get_future_events_for_chapters($chapters);
	
	// get list of events user has registered or is waitlisted for
	$registered_events = array();
	if ($user_to_moderate != null) {
		$registered_events = tm_users_get_users_events($user_to_moderate);
		$waitlisted_events = tm_users_get_users_events_waitlisted($user_to_moderate);
	}
	
	// calculate events to display
	$display_events = array();
	$disabled_events = array();
	foreach ($upcoming_events as $upcoming_event) {

		$nid = $upcoming_event["event_id"];

		// show events that member has not registered for
		if ($moderation_mode == "register") {
			if ((!in_array($nid, $registered_events)) and (!in_array($nid, $waitlisted_events))) {
				$display_events[] = $nid;
			} else {
				$disabled_events[] = $nid;
			}
		}

	}

	// no results
	if ((count($display_events) == 0) and (count($registered_events)) and (count($waitlisted_events))) {
		$html = "No upcoming events <script>jQuery('.jqidefaultbutton').hide();</script>";
		print $html;
		return;
	}

	// generate html
	$html = "<select id='moderation_event_ids'>";
	foreach ($display_events as $nid) {

		// show event title
		$event_label = strip_tags($upcoming_events[$nid]["event_title"]);

		// show chapter short code to moderator
		if ($is_moderator and ($upcoming_events[$nid]["chapter_short_code"] != null)) {
			$event_label .= " (" . $upcoming_events[$nid]["chapter_short_code"] . ")";
		}

		$html .= "<option value='" . $nid . "'>" . $event_label . "</option>\n";
	}

	if (count($display_events) == 0) {
		$html .= "<option>Choose an event</option>\n";
	}

	// show all available chapters when adding	
	if ($moderation_mode == "register") {
		
		foreach ($disabled_events as $nid) {

			// show if registered or on waitlist
			// (user needs to manage this using event management tools)
			$disabled_label = "(registered)";
			if (in_array($nid, $waitlisted_events)) {
				$disabled_label = "(waitlist)";
			}

			// show event title
			$event_label = strip_tags($upcoming_events[$nid]["event_title"]);

			// show chapter short code to moderator
			if ($is_moderator and ($upcoming_events[$nid]["chapter_short_code"] != null)) {
				$event_label .= " (" . $upcoming_events[$nid]["chapter_short_code"] . ")";
			}

			$html .= "<option value='" . $nid . "' disabled>" . $event_label . " " . $disabled_label . "</option>\n";
		}
	}
	
	$html .= "</select>";

	// no available events to add - hide apply button
	if (count($display_events) == 0) {
		$html .= "<script>jQuery('.jqidefaultbutton').hide();</script>";
	}

	print $html;

}
