<?php

// tm_events.drush.inc - drush commands

// instructions:
// run hourly on 14,29,44,59 minutes: drush tm-events-livestream-tasks --uri=https://example.massiveplatform.com --confirm=true
// run hourly on 0,5,10,15,20,25,30,35,40,45,50,55 minutes: tm-events-reminder-tasks --uri=https://example.massiveplatform.com --confirm=true
// set up a crontab entry and a script to run and log these commands

/**
 * Implements hook_drush_command().
 */
function tm_events_drush_command() {

	$items['tm-events-livestream-tasks'] = array(
		'description' => 'Run automated event live stream tasks.',
		'callback' => 'tm_events_livestream_tasks_drush',
		'options' => array(
			'confirm' => 'Confirm that you want to proceed',
			'target-event' => 'id of single event to process',
			'seconds' => 'Set the seconds timers (developer)',
		),
	);

	$items['tm-events-reminder-tasks'] = array(
		'description' => 'Run automated event reminder tasks.',
		'callback' => 'tm_events_reminder_tasks_drush',
		'options' => array(
			'confirm' => 'Confirm that you want to proceed',
			'target-event' => 'id of single event to process',
			'seconds' => 'Set the seconds timers (developer)',
		),
	);

	return $items;
}

/**
 * Run automated event live stream tasks
 */
function tm_events_livestream_tasks_drush() {

	global $conf;

	// timestamp
	drush_print("\nStart: " . gmdate("Y-m-d H:i:s T O"));

	// check config
	$livestream_schedule_enabled = false;
	if (isset($conf["tm_event_online_livestream_schedule_enabled"])) {
		$livestream_schedule_enabled = $conf["tm_event_online_livestream_schedule_enabled"];
	}
	if (!$livestream_schedule_enabled) {
		drush_print('Please enable reminders in $conf["tm_event_online_livestream_schedule_enabled"] to run this task.');
		return;
	}

	// check confirm
	$confirm = false;
	$confirm_opt = drush_get_option('confirm');
	if ($confirm_opt != null) {
		$confirm = ($confirm_opt == "true");
	}
	if ($confirm_opt == null) {
		drush_print("Usage: drush tm-events-livestream-tasks --uri=HOSTNAME --confirm=true\nOptional: --target-event=123 --seconds=600");
		return;
	}
	if (!$confirm) {
		drush_print("Running simulation only. Use --confirm=true to process.");
	}

	// check uri
	if (drush_get_option("uri") == null) {
		drush_print("Warning! Need to --uri to ensure URLS are correctly rendered in emails");
	}

	// target event
	$target_event = drush_get_option('target-event');
	if ($target_event != null) {
		drush_print("Target event id: " . $target_event);
	}

	// set_seconds
	$set_seconds = drush_get_option('seconds');
	if ($set_seconds != null) {
		$confirm = false;
		drush_print("Set seconds: " . $set_seconds);
		drush_print("Running simulation only. Unset --seconds to enable --confirm=true.");
	}

	// get upcoming events in next 24 hours (we don't know timezone yet)
	$query = "SELECT entity_id FROM field_data_field_event_date WHERE entity_type = 'node' AND bundle = 'event' AND field_event_date_value > DATE_SUB(NOW(), INTERVAL 1 DAY) AND field_event_date_value < DATE_ADD(NOW(), INTERVAL 2 DAY) ORDER BY field_event_date_value ASC";
	$event_ids = db_query($query)->fetchAll();
	$event_data = array();

	drush_print("Found " . sizeof($event_ids) . " upcoming events (next 24 hours)...");
	
	// process events
	foreach($event_ids as $result) {

		// optional: only target specific event
		if ($target_event != null) {
			if ($target_event != $result->entity_id) {
				continue;
			}
		}

		// get information about event
		$event = node_load($result->entity_id);
		if ($event == null) {
			continue;
		}

		// display event id
	    drush_print("Event: (nid " . $result->entity_id . ") " . $event->title);

	    // get seconds until event start
		$seconds_until_event_starts = tm_events_seconds_until_event_starts($event);

		// override seconds until start
		if ($set_seconds != null) {
			$seconds_until_event_starts = $set_seconds;
		}

	    // event started already
		if ($seconds_until_event_starts <= -60) {
			drush_print("- Event already started (" . $seconds_until_event_starts . " seconds), skipping...");
			continue;
		}

	    // already live
	    if (tm_event_livestream_is_on($event)) {
	    	drush_print("- Event live stream already on, skipping...");
	    	continue;
	    }

	   	// Get live stream URL
        $livestream_url = tm_event_get_livestream_url($event);

        // Get youtube video id
        $youtube_video_id = tm_event_get_youtube_video_id($livestream_url);

        if ($youtube_video_id == null) {
        	drush_print("- Event does not have a live stream URL, skipping...");
	    	continue;
        }

		// display seconds until start
		drush_print("- Seconds until event: " . $seconds_until_event_starts);

		// 1 minute to live
		// Turn live stream on
		// Clients will take up to 30 seconds to refresh
		// process 1 minute earlier for cron
		if ($seconds_until_event_starts <= 60) {
			
			drush_print("- Going LIVE...");

			// send message
			if ($confirm) {
				
				// set livestream on
				$event->field_event_livestream_on[LANGUAGE_NONE][0]['value'] = 1;
				node_save($event);
				drush_print("- Node saved");

			} else {
				drush_print("- (Simulation) Node saved");
			}

		}
	
	}

	// timestamp
	drush_print("End: " . gmdate("Y-m-d H:i:s T O"));
	drush_print("----");
}

/**
 * Run automated event reminder tasks
 */
function tm_events_reminder_tasks_drush() {

	global $conf;

	// timestamp
	drush_print("\nStart: " . gmdate("Y-m-d H:i:s T O"));

	// check config
	$reminders_enabled = false;
	if (isset($conf["tm_event_online_reminders_enabled"])) {
		$reminders_enabled = $conf["tm_event_online_reminders_enabled"];
	}
	if (!$reminders_enabled) {
		drush_print('Please enable reminders in $conf["tm_event_online_reminders_enabled"] to run this task.');
		return;
	}

	// check confirm
	$confirm = false;
	$confirm_opt = drush_get_option('confirm');
	if ($confirm_opt != null) {
		$confirm = ($confirm_opt == "true");
	}
	if ($confirm_opt == null) {
		drush_print("Usage: drush tm-events-reminder-tasks --uri=HOSTNAME --confirm=true\nOptional: --target-event=123 --seconds=600 (600 = 10min, 5400 = 1hr30min, 86400 = 24hr");
		return;
	}
	if (!$confirm) {
		drush_print("Running simulation only. Use --confirm=true to process.");
	}

	// check uri
	if (drush_get_option("uri") == null) {
		drush_print("Warning! Need to --uri to ensure URLS are correctly rendered in emails");
	}

	// target event
	$target_event = drush_get_option('target-event');
	if ($target_event != null) {
		drush_print("Target event id: " . $target_event);
	}

	// set_seconds
	$set_seconds = drush_get_option('seconds');
	if ($set_seconds != null) {
		$confirm = false;
		drush_print("Set seconds: " . $set_seconds . " (600 = 10min, 5400 = 1hr30min, 86400 = 24hr");
		drush_print("Running simulation only. Unset --seconds to enable --confirm=true.");
	}

	// get upcoming events in next 24 hours (we don't know timezone yet)
	$query = "SELECT entity_id FROM field_data_field_event_date WHERE entity_type = 'node' AND bundle = 'event' AND field_event_date_value > DATE_SUB(NOW(), INTERVAL 1 DAY) AND field_event_date_value < DATE_ADD(NOW(), INTERVAL 2 DAY) ORDER BY field_event_date_value ASC";
	$event_ids = db_query($query)->fetchAll();
	$event_data = array();

	drush_print("Found " . sizeof($event_ids) . " upcoming events (next 24 hours)...");
	
	// process events
	foreach($event_ids as $result) {

		// optional: only target specific event
		if ($target_event != null) {
			if ($target_event != $result->entity_id) {
				continue;
			}
		}

		// get information about event
		$event = node_load($result->entity_id);
		if ($event == null) {
			continue;
		}

		// display event id
	    drush_print("Event: (nid " . $result->entity_id . ") " . $event->title);

	    // don't process external rsvp
		if ($conf['tm_event_restrict_announcement_rsvp']) {
			if ($event->field_external_rsvp[LANGUAGE_NONE][0]['value']) {
				drush_print("- Event is external RSVP, skipping...");
				continue;
			}
		}

	    // only process online event
        $is_online_event = tm_events_is_online_event($event);
		if (!$is_online_event) {
			drush_print("- Event is not online event, skipping...");
			continue;
		}

		// send reminders turned on
		$send_reminders = false;
		if (isset($event->field_event_send_reminders[LANGUAGE_NONE][0]['value'])) {
			$send_reminders = $event->field_event_send_reminders[LANGUAGE_NONE][0]['value'];
		}
		if (!$send_reminders) {
			drush_print("- Event reminders not turned on, skipping...");
			continue;
		}

		// check if sent reminder recently
		$min_minutes = 15; // 15 minutes
		$announcement_last_sent_flagged = flag_get_entity_flags("node", $event->nid, "reminder_last_sent");
		if (sizeof($announcement_last_sent_flagged) > 0) {
			$flagged_last = array_shift($announcement_last_sent_flagged);
			$difference = time() - $flagged_last->timestamp;
			if ($difference < (60 * $min_minutes)) { 
				drush_print("- Reminder has been sent in past " . $min_minutes . " minutes, skipping...");
				continue;
			}
		}

		// get seconds until event start
		$seconds_until_event_starts = tm_events_seconds_until_event_starts($event);

		// override seconds until start
		if ($set_seconds != null) {
			$seconds_until_event_starts = $set_seconds;
		}

		// event started already
		if ($seconds_until_event_starts <= 0) {
			drush_print("- Event already started (" . $seconds_until_event_starts . " seconds), skipping...");
			continue;
		}

		// display seconds until start
		drush_print("- Seconds until event: " . $seconds_until_event_starts);

		// Get chapter associated with event if there is one
		$chapter = null;
		if (isset($event->field_chapter[LANGUAGE_NONE][0]['target_id'])) {
			$chapter_id = $event->field_chapter[LANGUAGE_NONE][0]['target_id'];
			$chapter = node_load($chapter_id);
		}

		// Get company associated with event if there is one
		$company = null;
		if (isset($event->field_event_company[LANGUAGE_NONE][0]['target_id'])) {
			$company_id = $event->field_event_company[LANGUAGE_NONE][0]['target_id'];
			$company = node_load($company_id);
		}

		// 24 hour reminder
		// Send to non registered members
		if (($seconds_until_event_starts > (60 * 1438)  and ($seconds_until_event_starts <= (60 * 1441)))) {
			
			$reminder_type = "24hr";
			drush_print("- Reminder type: " . $reminder_type);

			// Don't send 24hr reminder (to all members) if in preview mode
			if (tm_events_is_preview_mode($event->nid)) {
				drush_print("- Event is in preview mode, not sending reminder.");
				continue;
			}

			// Check that event is not part of chapter that has 24 hr reminders disabled
			if (isset($conf["tm_event_online_reminder_24hr_disable_chapters"])) {
				if ($chapter != null) {
					if (in_array($chapter->nid, $conf["tm_event_online_reminder_24hr_disable_chapters"])) {
						drush_print("- Skipping 24hr reminder for chapter: " . $chapter->title);
						continue;
					}
				}
			}

			// generate message
			$event_title = trim(strip_tags($event->title));
			$subject = $conf["tm_event_online_reminder_" . $reminder_type . "_subject"];
			$subject = str_replace("__EVENT_TITLE__", $event_title, $subject);
			$headline_text = $conf["tm_event_online_reminder_" . $reminder_type . "_headline"];
			$intro_text = $conf["tm_event_online_reminder_" . $reminder_type . "_intro"];
			$button_label = $conf["tm_event_online_reminder_button_label_register"];
			$message_template = $conf["tm_event_online_reminder_message_register"];

			// display cover image
			$event_image_src = null;
			$event_image_link = null;
			if ($conf["tm_event_online_reminder_" . $reminder_type . "_image"]) {
				$event_image_link = $conf["tm_email_server_http"] . "://" .  $conf["tm_email_server_url_domain"] . "/" . drupal_get_path_alias('node/' . $event->nid);
				$default_image_uri = _tm_events_get_default_image($conf["tm_images_default_field_image"], $event->nid);
				$event_image_src = image_style_url("banner", $default_image_uri);
				if (isset($event->field_image[LANGUAGE_NONE][0]['uri'])) {
					$event_image_src = image_style_url("banner", $event->field_image[LANGUAGE_NONE][0]['uri']);
				}
			}

			// generate message for non-registered members
			$message = tm_events_generate_reminder_message($event, $message_template, $headline_text, $intro_text, $button_label);

			// recipients
			if ($chapter != null) {
				$send_to = 'chapter_and_not_rsvp_or_waitlist';
			}
			if ($company != null) {
				$send_to = 'company_followers';
			}
			if (($chapter == null) and ($company == null)) {
				$send_to = 'member_followers';
			}

			drush_print("- Recipients: " . $send_to);

			// send message
			if ($confirm) {
				tm_events_reminder_send_drush($event, $send_to, $subject, $headline_text, $message, $event_image_src, $event_image_link);
			} else {
				drush_print("Subject: " . $subject);
				drush_print($message);
			}
		}

		// 1.5 hour reminder
		// Send to attendees
		if (($seconds_until_event_starts > (60 * 88) and ($seconds_until_event_starts <= (60 * 91)))) {
			
			$reminder_type = "1hr30min";
			drush_print("- Reminder type: " . $reminder_type);

			// Check external rsvp
			if ($event->field_external_rsvp[LANGUAGE_NONE][0]['value']) {
				drush_print("- Event is external RSVP, skipping...");
				continue;
			}

			// generate message
			$event_title = trim(strip_tags($event->title));
			$subject = $conf["tm_event_online_reminder_" . $reminder_type . "_subject"];
			$subject = str_replace("__EVENT_TITLE__", $event_title, $subject);
			$headline_text = $conf["tm_event_online_reminder_" . $reminder_type . "_headline"];
			$intro_text = $conf["tm_event_online_reminder_" . $reminder_type . "_intro"];
			$button_label = $conf["tm_event_online_reminder_button_label_attendee"];
			$message_template = $conf["tm_event_online_reminder_message_attendee"];
			$recipients = 'rsvp'; // members who have registered

			// display cover image
			$event_image_src = null;
			$event_image_link = null;
			if ($conf["tm_event_online_reminder_" . $reminder_type . "_image"]) {
				$event_image_link = $conf["tm_email_server_http"] . "://" .  $conf["tm_email_server_url_domain"] . "/" . drupal_get_path_alias('node/' . $event->nid);
				$default_image_uri = _tm_events_get_default_image($conf["tm_images_default_field_image"], $event->nid);
				$event_image_src = image_style_url("banner", $default_image_uri);
				if (isset($event->field_image[LANGUAGE_NONE][0]['uri'])) {
					$event_image_src = image_style_url("banner", $event->field_image[LANGUAGE_NONE][0]['uri']);
				}
			}

			// generate message for attendees
			$message = tm_events_generate_reminder_message($event, $message_template, $headline_text, $intro_text, $button_label);

			// send message
			if ($confirm) {
				tm_events_reminder_send_drush($event, $recipients, $subject, $headline_text, $message, $event_image_src, $event_image_link);
			} else {
				drush_print("Subject: " . $subject);
				drush_print($message);
			}
		}

		// 10 minute reminder
		// Send to only attendees
		if (($seconds_until_event_starts > (60 * 8)) and ($seconds_until_event_starts <= (60 * 11))) {
			
			$reminder_type = "10min";
			drush_print("- Reminder type: " . $reminder_type);

			// Check external rsvp
			if ($event->field_external_rsvp[LANGUAGE_NONE][0]['value']) {
				drush_print("- Event is external RSVP, skipping...");
				continue;
			}

			// generate message
			$event_title = trim(strip_tags($event->title));
			$subject = $conf["tm_event_online_reminder_" . $reminder_type . "_subject"];
			$subject = str_replace("__EVENT_TITLE__", $event_title, $subject);
			$headline_text = $conf["tm_event_online_reminder_" . $reminder_type . "_headline"];
			$intro_text = $conf["tm_event_online_reminder_" . $reminder_type . "_intro"];
			$button_label = $conf["tm_event_online_reminder_button_label_attendee"];
			$message_template = $conf["tm_event_online_reminder_message_attendee"];
			$recipients = 'rsvp'; // members who have registered

			// display cover image
			$event_image_src = null;
			$event_image_link = null;
			if ($conf["tm_event_online_reminder_" . $reminder_type . "_image"]) {
				$event_image_link = $conf["tm_email_server_http"] . "://" .  $conf["tm_email_server_url_domain"] . "/" . drupal_get_path_alias('node/' . $event->nid);
				$default_image_uri = _tm_events_get_default_image($conf["tm_images_default_field_image"], $event->nid);
				$event_image_src = image_style_url("banner", $default_image_uri);
				if (isset($event->field_image[LANGUAGE_NONE][0]['uri'])) {
					$event_image_src = image_style_url("banner", $event->field_image[LANGUAGE_NONE][0]['uri']);
				}
			}

			// generate message
			$message = tm_events_generate_reminder_message($event, $message_template, $headline_text, $intro_text, $button_label);

			// send message
			if ($confirm) {
				tm_events_reminder_send_drush($event, $recipients, $subject, $headline_text, $message, $event_image_src, $event_image_link);
			} else {
				drush_print("Subject: " . $subject);
				drush_print($message);
			}  

		}
	}

	// timestamp
	drush_print("End: " . gmdate("Y-m-d H:i:s T O"));
	drush_print("----");
}

/**
 * Callback to send a test reminder email
 */
function tm_events_send_test_reminder_callback($event_id, $reminder_type) {

	global $user;

	// user is logged in
	if (!user_is_logged_in()) {
		drupal_access_denied();
	}

	// valid event
	$event = node_load($event_id);
	if ($event == null) {
		drupal_access_denied();
	}

	// user can edit event
	if (!tm_events_check_permission_edit_event($user, $event)) {
		drupal_access_denied();
	}

	// valid reminder type
	if (($reminder_type != "24hr") and ($reminder_type != "1hr30min") and ($reminder_type != "10min")) {
		drupal_access_denied();
	}

	// Get chapter associated with event if there is one
	$recipients = "your followers"; 
	if (isset($event->field_chapter[LANGUAGE_NONE][0]['target_id'])) {
		$recipients = "all chapter members";
	}

	// Get company associated with event if there is one
	$company = null;
	if (isset($event->field_event_company[LANGUAGE_NONE])) {
		$recipients = "your company followers"; 
	}

	// get email
	$send_to_email = $user->mail;

	// send test reminder
	tm_events_send_test_reminder($event, $reminder_type, $send_to_email);

	// notify user and redirect
	$reminder_labels = array("24hr" => "<b>1 day announcement</b> (for " . $recipients . ")", "1hr30min" => "<b>1 hour and 30 minute reminder</b> (for registered attendees)", "10min" => "<b>10 minute reminder</b> (for registered attendees)");
	drupal_set_message("A preview of the " . $reminder_labels[$reminder_type] . " has been sent to <i>" . $send_to_email . "</i>", "help-tip");
	drupal_goto("node/". $event_id);
}

/**
 * Send a test reminder email
 * reminder_type = "24hr", "1hr30min" or "10min"
 */
function tm_events_send_test_reminder($event, $reminder_type = "1hr30min", $send_to_email) {

	global $conf;

	// valid reminder type
	if (($reminder_type != "24hr") and ($reminder_type != "1hr30min") and ($reminder_type != "10min")) {
		return;
	}

	// generate message
	$event_title = trim(strip_tags($event->title));
	$subject = $conf["tm_event_online_reminder_" . $reminder_type . "_subject"];
	$subject = str_replace("__EVENT_TITLE__", $event_title, $subject);
	$headline_text = $conf["tm_event_online_reminder_" . $reminder_type . "_headline"];
	$intro_text = $conf["tm_event_online_reminder_" . $reminder_type . "_intro"];
	$button_label = $conf["tm_event_online_reminder_button_label_attendee"];
	$message_template = $conf["tm_event_online_reminder_message_attendee"];

	if ($reminder_type == "24hr") {
		$message_template = $conf["tm_event_online_reminder_message_register"];
		$button_label = $conf["tm_event_online_reminder_button_label_register"];
	}

	// display cover image
	$event_image_src = null;
	$event_image_link = null;
	if ($conf["tm_event_online_reminder_" . $reminder_type . "_image"]) {
		$event_image_link = $conf["tm_email_server_http"] . "://" .  $conf["tm_email_server_url_domain"] . "/" . drupal_get_path_alias('node/' . $event->nid);
		$default_image_uri = _tm_events_get_default_image($conf["tm_images_default_field_image"], $event->nid);
		$event_image_src = image_style_url("banner", $default_image_uri);
		if (isset($event->field_image[LANGUAGE_NONE][0]['uri'])) {
			$event_image_src = image_style_url("banner", $event->field_image[LANGUAGE_NONE][0]['uri']);
		}
	}

	// generate message
	$message = tm_events_generate_reminder_message($event, $message_template, $headline_text, $intro_text, $button_label);

	// send test email
	return tm_events_send_email(null, $send_to_email, "[Preview] " . $subject, $message, $event_image_src, $event_image_link, $event->nid, "", $headline_text);

}

/**
 * Generate reminder message
 */
function tm_events_generate_reminder_message($event, $message_template, $headline_text, $intro_text, $button_label = "View event") {
	
	global $conf;

	// message template
	/*$message_template = "<p>Hi [first_name],</p>
	<p>Thank you for signing up for today's event:</p>
	<p><b><a href='__EVENT_URL__'>__EVENT_TITLE__</a></b></p>
	<p><b>__INTRO_TEXT__</b></p>
	__EVENT_INSTRUCTIONS__
	<p>[EVENT_BUTTON]</p>
	__FEATURED_MEMBERS__
	__CONTACT_LINK__
	__SIGNOFF_MESSAGE__
	";*/
	
	// message
	$message = $message_template;

	// event url and title
	$event_url = $conf["tm_email_server_http"] . "://" . $conf["tm_email_server_url_domain"] . "/" . drupal_get_path_alias('node/' . $event->nid);
	$message = str_replace("__EVENT_URL__", $event_url, $message);

	// event title
	$event_title = trim(strip_tags($event->title));
	$message = str_replace("__EVENT_TITLE__", $event_title, $message);

	// going live text
	$message = str_replace("__INTRO_TEXT__", $intro_text, $message);

	// start timezones
	// ie: 10am NYC / 3pm London / 9pm Bangkok
	$start_timezones = tm_events_render_short_timezones($event); // "10am NYC / 3pm London / 9pm Bangkok";
	$message = str_ireplace("__START_TIMEZONES__", $start_timezones, $message);
	
	// event instructions
	$event_instructions = "";
	if (isset($event->field_event_online_instructions[LANGUAGE_NONE][0]['value'])) {

		// strip tags
		$event_instructions = trim(strip_tags($event->field_event_online_instructions[LANGUAGE_NONE][0]['value']));

		// add markdown support, or add line breaks
		if (tm_base_check_markdown_permission("event", $event->nid)) {
			$event_instructions = tm_base_parsedown($event_instructions, null, false);
			$event_instructions = str_replace("\n", "", $event_instructions);
		} else {
			$event_instructions = "<p>" . str_replace("\r", "<br>", $event_instructions) . "</p>";
		}
		$event_instructions = "<p><b>" . $conf["tm_event_online_reminder_instructions_heading"] . "</b></p>" . $event_instructions;
		
	}
	$message = str_replace("__EVENT_INSTRUCTIONS__", $event_instructions, $message);

	// button
	$button_html = tm_notifications_render_button_html($button_label, $event_url);
	$message = str_ireplace("[EVENT_BUTTON]", $button_html, $message);	
	
	// featured members
	$featured_members_html = "";
	$featured_member_parts = array();
	if (isset($event->field_event_featured_members[LANGUAGE_NONE])) {
		foreach ($event->field_event_featured_members[LANGUAGE_NONE] as $field_featured_member) {  

			// featured member
			$account = user_load($field_featured_member["target_id"]);
			if ($account != null) {

				// url
				$featured_member_url = $conf["tm_email_server_http"] . "://" . $conf["tm_email_server_url_domain"] . "/user/"  . $account->uid;

				// name
				$first_and_last_name = trim(strip_tags($account->field_user_first_name[LANGUAGE_NONE][0]['value'])) . " " . trim(strip_tags($account->field_user_last_name[LANGUAGE_NONE][0]['value']));

				// create link
				$featured_member_html = "<li>";
				$featured_member_html .= "<a href='" . $featured_member_url . "'>";
				$featured_member_html .= $first_and_last_name;
				$featured_member_html .= "</a>";

				// job role, and name
				$job_role = "";
				$organization_name = "";
				$sub_title = "";
				if (isset($account->field_job_role[LANGUAGE_NONE][0]['value'])) {
					$job_role = strip_tags(trim($account->field_job_role[LANGUAGE_NONE][0]['value']));
					$sub_title = $job_role;
				}
				if (isset($account->field_job_organization[LANGUAGE_NONE][0]['value'])) {
					if ($sub_title != "") {
						$sub_title .= " at ";
					}
					$organization_name = strip_tags(trim($account->field_job_organization[LANGUAGE_NONE][0]['value']));
					$sub_title .= $organization_name;
				}

				// only display if short enough
				// Try in this order
				// 1. job_role at organisation_name
				// 2. organization_name
				// 3. job_role
				$max_length = 55;
				if (isset($conf["tm_event_online_reminders_max_featured_member_text"])) {
					$max_length = $conf["tm_event_online_reminders_max_featured_member_text"];
				}
				if ((strlen($first_and_last_name . ", " . $sub_title) < $max_length) and (strlen($sub_title) > 3)) {
					$featured_member_html .= ", " . $sub_title;
				} elseif ((strlen($first_and_last_name . ", " . $organization_name) < $max_length) and (strlen($organization_name) > 3)) {
					$featured_member_html .= ", " . $organization_name;
				} elseif ((strlen($first_and_last_name . ", " . $job_role) < $max_length) and (strlen($job_role) > 3)) {
					$featured_member_html .= ", " . $job_role;
				}

				$featured_member_html .= "</li>";
				$featured_member_parts[] = $featured_member_html;						
			}
		}
	}
	if (sizeof($featured_member_parts) > 0) {
		
		// Get default featured label
		$featured_label = "Special Guests";
		$event_type = $event->field_event_type[LANGUAGE_NONE][0]['value'];
		if (isset($conf["tm_featured_members_event_types"][$event_type])) {
			$featured_label = $conf["tm_featured_members_event_types"][$event_type];
		}

		// Get custom label
		if (isset($event->field_event_featured_label[LANGUAGE_NONE][0]['value'])) {
			$label = trim(strip_tags($event->field_event_featured_label[LANGUAGE_NONE][0]['value']));
			if ($label != "") {
				$featured_label = $label;
			} 
		}
		$featured_members_html = "<p><b>" . $conf["tm_event_online_reminder_featured_emoji"] . $featured_label . "</b></p>";
		$featured_members_html .= "<ul>" . implode("\n", $featured_member_parts) . "</ul>";
	}
	$message = str_replace("__FEATURED_MEMBERS__", $featured_members_html, $message);

	// add contact link if html email
	$contact_url = $conf["tm_email_server_http"] . "://" . $conf["tm_email_server_url_domain"] . "/" . drupal_get_path_alias('events/message/' . $event->nid);

	$contact_html = '<p>Questions about this event? <a href="' . $contact_url . '">Send a message</a></p>';
	$message = str_replace("__CONTACT_LINK__", $contact_html, $message);

	// sign off
	$signoff_html = "<p>Cheers";

	// Get chapter associated with event if there is one
	$chapter = null;
	if (isset($event->field_chapter[LANGUAGE_NONE][0]['target_id'])) {
		$chapter_id = $event->field_chapter[LANGUAGE_NONE][0]['target_id'];
		$chapter = node_load($chapter_id);
	}

	// Get company associated with event if there is one
	$company = null;
	if (isset($event->field_event_company[LANGUAGE_NONE][0])) {
		if (sizeof($event->field_event_company[LANGUAGE_NONE][0]) > 0) {
			$company_id = $event->field_event_company[LANGUAGE_NONE][0]['target_id'];
			$company = node_load($company_id);
		}
	}

	// chapter signoff
	if ($chapter != null) {
		$signoff_html .= ",<br><i>&mdash; The " . $chapter->title  . " Team</i>";
	}

	// company signoff
	if (($chapter == null) and ($company != null)) {
		$signoff_html .= ",<br>&mdash; " . strip_tags($company->title);
	}

	// individual signoff
	if (($chapter == null) and ($company == null)) {
		$loaded = user_load($event->uid);
		$first_name = strip_tags($loaded->field_user_first_name[LANGUAGE_NONE][0]['value']);
		$last_name = strip_tags($loaded->field_user_last_name[LANGUAGE_NONE][0]['value']);
		$signoff_html .= ",<br><i>&mdash; " . $first_name . " " . $last_name . "</i>";
	}
	
	$signoff_html .= "</p>";
	$message = str_replace("__SIGNOFF_MESSAGE__", $signoff_html, $message);

  	// display event id, and optional chapter nids
  	$chapter_nids = tm_events_get_published_chapters($event);
	if ($chapter != null) {
		array_unshift($chapter_nids, $chapter->nid);
	}
  	$chapter_nids = array_unique($chapter_nids);
	if ($chapter != null) {
		$message .= "<p><small>(Event ID: " . $event->nid . ":" . implode(",", $chapter_nids) . ")</small></p>";
	} else {
		$message .= "<p><small>(Event ID: " . $event->nid . ")</small></p>";
	}

	return($message);
}

/**
 * Send reminder email
 */
function tm_events_reminder_send_drush($event, $recipients, $subject, $headline_text, $message, $event_image_src = null, $event_image_link = null, $is_drush_command = false) {

	global $conf;
	$drush_user = user_load(1);

	// $recipients = 'rsvp'; // members who are attending
	// $subject = "";
	// $message = "";
	// $event_image_src = null;
	// $event_image_link = null;
	$eventid = $event->nid;
	// $chapterids = "";
	$replyto = "";
	$approved_members = false;
	// $headline_text = "";
	$drush_command = true;

	// Get chapter associated with event if there is one
	$chapter = null;
	if (isset($event->field_chapter[LANGUAGE_NONE][0]['target_id'])) {
		$chapter_id = $event->field_chapter[LANGUAGE_NONE][0]['target_id'];
		$chapter = node_load($chapter_id);
	}

	// Get company associated with event if there is one
	$company = null;
	if (isset($event->field_event_company[LANGUAGE_NONE][0]['target_id'])) {
		$company_id = $event->field_event_company[LANGUAGE_NONE][0]['target_id'];
		$company = node_load($company_id);
	}

	// Get published event chapters
	$chapter_nids = tm_events_get_published_chapters($event);
	if ($chapter != null) {
		array_unshift($chapter_nids, $chapter->nid);
	}
	$chapter_nids = array_unique($chapter_nids);
	$chapterids = implode(",", $chapter_nids);

	// flag announcement last sent
  	flag('unflag', 'announcement_last_sent', $eventid, $drush_user);
 	flag('flag', 'announcement_last_sent', $eventid, $drush_user);

 	// reminder last sent
 	flag('unflag', 'reminder_last_sent', $eventid, $drush_user);
 	flag('flag', 'reminder_last_sent', $eventid, $drush_user);

 	// account is owner of event
 	$account = user_load($event->uid);

	// send emails
	$sendcount = tm_events_send_emails($recipients, $subject, $message, $event_image_src, $event_image_link, $eventid, $chapterids, $replyto, $approved_members, $headline_text, $account, $drush_command);

	// send copy
	if (isset($conf["tm_announcement_copy_email"])) {
		tm_events_send_email(null, $conf["tm_announcement_copy_email"], "[Copy][Cron] " . $subject . " (" . $sendcount . " sent)", $message, $event_image_src, $event_image_link, $eventid, $replyto, $headline_text);
 	}

 	drush_print("Sent reminder email to " . $sendcount . " recipients");

}
