<?php

// tm_chapters.drush.inc - drush commands

/**
 * Implements hook_drush_command().
 */
function tm_chapters_drush_command() {

	$items['tm-community-management-tasks'] = array(
		'description' => 'Run automated community management tasks.',
		'callback' => 'tm_chapters_community_management_tasks_drush',
		'options' => array(
			'confirm' => 'Confirm that you want to proceed',
			'target_chapter' => 'id of single chapter to process',
		),
	);

	$items['tm-community-management-reports'] = array(
		'description' => 'Run automated community management reports.',
		'callback' => 'tm_chapters_community_management_reports_drush',
		'options' => array(
			'confirm' => 'Confirm that you want to proceed',
			'target_chapter' => 'id of single chapter to process',
		),
	);

	$items['tm-community-management-remove-all-managers'] = array(
		'description' => 'Remove all community managers from all chapters.',
		'callback' => 'tm_chapters_community_management_remove_all_managers_drush',
		'options' => array(
			'confirm' => 'Confirm that you want to proceed',
		),
	);

	$items['tm-chapters-update-display-stats-cache'] = array(
		'description' => 'Update display stats cache for all chapters.',
		'callback' => 'tm_chapters_update_display_stats_cache_drush',
		'options' => array(
			'confirm' => 'Confirm that you want to proceed',
		),
	);

	$items['tm-chapters-members-csv-cache'] = array(
		'description' => 'Update chapter csv for all chapters.',
		'callback' => 'tm_chapters_members_csv_drush',
		'options' => array(
			'confirm' => 'Confirm that you want to proceed',
		),
	);

	return $items;
}

/**
 * Run automatied community management tasks
 */
function tm_chapters_community_management_tasks_drush() {

	global $conf;

	// check confirm
	$confirm = false;
	$confirm_opt = drush_get_option('confirm');
	if ($confirm_opt != null) {
		$confirm = ($confirm_opt == "true");
	}
	if ($confirm_opt == null) {
		drush_print("Usage: drush tm-community-management-tasks --uri=HOSTNAME --confirm=true\nOptional: --target_chapter=123");
		return;
	}

	if (!$confirm) {
		drush_print("Running simulation only. Use --confirm=true to process.");
	}

	if (drush_get_option("uri") == null) {
		drush_print("Warning! Need to --uri to ensure URLS are correctly rendered in emails");
	}

	// target chapter
	$target_chapter = drush_get_option('target_chapter');
	if ($target_chapter != null) {
		drush_print("Target chapter id: " . $target_chapter);
	}

	// Get all chapters
	$query = "SELECT nid, title FROM node WHERE type = 'chapter' AND status = 1 ORDER BY title";
	$results = db_query($query)->fetchAll();
	$chapter_nids = array();
	foreach ($results as $result) {
	  $chapter_nids[$result->title] = $result->nid;
	}

	// STEP 1.
	// CHECK ACTIVITY
	drush_print("Checking signin in activity...");
	foreach ($chapter_nids as $title => $nid) {

		// optional: only target specific chapter
		if ($target_chapter != null) {
			if ($target_chapter != $nid) {
				continue;
			}
		}

		// Load chapter
		$chapter = node_load($nid);

		// Clone chapter 
		$resign_chapter_leaders = array();
		$warn_chapter_leaders = array();

		// Analyse chapter leaders
		if (isset($chapter->field_chapter_leaders[LANGUAGE_NONE])) {
		  foreach ($chapter->field_chapter_leaders[LANGUAGE_NONE] as $leader) {
		    $leader = user_load($leader['target_id']);

		    $print_line = "- " . $title . ": " . $leader->field_user_first_name[LANGUAGE_NONE][0]['value'] . " " . $leader->field_user_last_name[LANGUAGE_NONE][0]['value'];

		    // when last accessed
		    $last_visit_timestamp = $leader->access;
			$last_visit_difference = time() - $last_visit_timestamp;
			if (($last_visit_timestamp == 0) or ($last_visit_timestamp == null)) {
				$last_visit_time = "never";
			} else {
				$last_visit_time = format_interval($last_visit_difference, 1) . " ago";
			}
			$print_line .= ". Last seen: " . $last_visit_time;
			if ($last_visit_difference > (60 * 60 * 24 * $conf["tm_chapters_leader_inactive_retire_days"])) {

				// three months
				$print_line .= " ( > " . $conf["tm_chapters_leader_inactive_retire_days"] .  " days inactive - RETIRE)";

				$resign_chapter_leaders[] = $leader->uid;

			} else if ($last_visit_difference > (60 * 60 * 24 * $conf["tm_chapters_leader_inactive_warning_days"])) {

				if (!in_array($leader->uid, $warn_chapter_leaders)) {

					$print_line .= " ( > " . $conf["tm_chapters_leader_inactive_warning_days"] . " days inactive - SEND WARNING)";

					$notification_emails = array($leader->mail);
					
					// Cc all community managers of any chapters member is a leader of
					$cc_uids = array();
					foreach (tm_users_get_chapters_leading($leader) as $nid) {
						$leaders_chapter = node_load($nid);
						if (isset($leaders_chapter->field_community_managers[LANGUAGE_NONE])) {
							foreach ($leaders_chapter->field_community_managers[LANGUAGE_NONE] as $community_manager) {
								$cc_uids[] = $community_manager['target_id'];
							}
						}
					}

					// unique ids
					$cc_uids = array_unique($cc_uids);

					// get addreses
					$cc_emails = array();
					foreach ($cc_uids as $uid) {
						if ($uid != $leader->uid) {
							$cc_user = user_load($uid);
							$cc_emails[] = $cc_user->mail;
						}
					}

					// Send warning
					if ($confirm) {

						// prepare subject and body
						$subject = $conf["tm_chapters_leader_inactive_warning_subject"];
						$subject = str_replace("__SITE_NAME__", $conf["tm_site_name"], $subject);

						$body = $conf["tm_chapters_leader_inactive_warning"];
						$body = str_replace("__SITE_NAME__", $conf["tm_site_name"], $body);
						$body = str_replace("__FIRST_NAME__", strip_tags($leader->field_user_first_name[LANGUAGE_NONE][0]['value']), $body);

						tm_chapters_leaders_send_notification($notification_emails, $subject, $body, $chapter, $cc_emails);
					}


				} else {
					$print_line .= " ( > " . $conf["tm_chapters_leader_inactive_warning_days"] . " days inactive - SEND WARNING)";

				}

				$warn_chapter_leaders[] = $leader->uid;
			
			}

			drush_print($print_line);

		  }

		  if ($confirm) {
		  	// Remove any chapter leaders in bulk for each chapter
		  	foreach ($chapter->field_chapter_leaders[LANGUAGE_NONE] as $key => $leader) {
		  		if (in_array($leader["target_id"], $resign_chapter_leaders)) {
		  			unset($chapter->field_chapter_leaders[LANGUAGE_NONE][$key]);
		  		}
		  	}
		  	node_save($chapter);
		  }
		  

		} else {
			drush_print("- " . $title . ": No chapter leaders");
		}

	}

	// STEP 2.
	// CHECK LAST EVENT
	drush_print("\nChecking last event date...");
	foreach ($chapter_nids as $title => $nid) {
		
		// optional: only target specific chapter
		if ($target_chapter != null) {
			if ($target_chapter != $nid) {
				continue;
			}
		}

		$chapter = node_load($nid);
		$chapter_name = tm_chapters_formal_name($chapter);

		// Check events..
		// LAST EVENT DATE
		// Fetch the nearest past event that references a particular chapter.
		// Only returns one event, the one with the closest starting date.
		// Fetch the current and make sure it's UTC and user-defined timezone.
		$current_date = new DateTime('now', new DateTimeZone('UTC'));
		$query = new EntityFieldQuery();
		$query->entityCondition('entity_type', 'node')
		  ->entityCondition('bundle', 'event')
		  ->propertyCondition('status', 1)
		  ->fieldCondition('field_chapter', 'target_id', $nid)
		  ->fieldCondition('field_event_date', 'value2', $current_date->format('Y-m-d H:i:s'), '<=')
		  ->fieldOrderBy('field_event_date', 'value2', 'DESC')
		  ->range(0,1);
		$result = $query->execute();

		// FUTURE EVENT DATE WITHIN 3 MONTHS
		// Fetch the nearest past event that references a particular chapter.
		// Only returns one event, the one with the closest starting date.
		// Fetch the current and make sure it's UTC and user-defined timezone.
		$in_three_months = new DateTime('now +3 months', new DateTimeZone('UTC'));
		$query = new EntityFieldQuery();
		$query->entityCondition('entity_type', 'node')
		  ->entityCondition('bundle', 'event')
		  ->propertyCondition('status', 1)
		  ->fieldCondition('field_chapter', 'target_id', $nid)
		  ->fieldCondition('field_event_date', 'value2', $current_date->format('Y-m-d H:i:s'), '>=')
		  ->fieldCondition('field_event_date', 'value2', $in_three_months->format('Y-m-d H:i:s'), '<=')
		  ->fieldOrderBy('field_event_date', 'value2', 'DESC')
		  ->range(0,1);
		$future_result = $query->execute();

		// CHECK IF CHAPTER OLDER THAN THREE MONTHS
		$three_months_ago = new DateTime('NOW -3 months', new DateTimeZone('UTC'));
		$chapter_created = new DateTime('NOW', new DateTimeZone('UTC'));
		$chapter_created->setTimeStamp($chapter->created);

		// LOGICAL VARS
		$has_past_events = (isset($result['node']));
		$has_future_events = (isset($future_result['node']));
		$older_than_three_months_no_events = (($chapter_created < $three_months_ago) and !$has_past_events and !$has_future_events);

		// Initialize time_until.
		$time_until = NULL;

		$last_event_time = "No past events";

		// Send warning if:
		// - has had past events, and no future events in past 3 months
		// - chapter is older than three months, and has no past or future events
		if (($has_past_events and !$has_future_events) or ($older_than_three_months_no_events)) {

			$send_warning = false;

			// Check past event date
			if ($has_past_events) {

			  $next_event_array = array_keys($result['node']);
			  $next_event = node_load($next_event_array[0]);
			  $event_date = field_get_items('node',$next_event,'field_event_date');
			  
			  // Create datetime object and ensure UTC timezone, instead of user-defined timezone.
			  $event_timezone = _tm_get_event_timezone($next_event);
			  $event_datetime = new DateTime($event_date[0]['value'], new DateTimeZone($event_timezone));
			  $last_event_time = _tm_event_time_elapsed($current_date->format('U') - $event_datetime->format('U')) . " ago";
			  $days_since_last_event = ($current_date->format('U') - $event_datetime->format('U')) / (60 * 60 * 24);

			  // Send warning if past event older than X days
			  if ($days_since_last_event > $conf["tm_chapters_leader_no_events_days"]) {
			  	$send_warning = true;
			  }
			  
			 } else {
			 	$send_warning = true;
			 	$last_event_time = "never";
			 }

		  if ($send_warning) {

		  	drush_print("- " . $chapter->title . ": last event " . $last_event_time . " ( > " . $conf["tm_chapters_leader_no_events_days"] . " days - SEND WARNING)");

		  	// Send warning
		  	if ($confirm) {

		  		// get formal chapter name
			  	$chapter_url = drupal_get_path_alias('node/' . $chapter->nid);

			  	// notify chapter leaders
			  	$notify_uids = array();
			  	if (isset($chapter->field_chapter_leaders[LANGUAGE_NONE])) {
			  		foreach ($chapter->field_chapter_leaders[LANGUAGE_NONE] as $chapter_leader) {
			  			$notify_uids[] = $chapter_leader['target_id'];
			  		}
			  	}

			  	// notify community managers
			  	if (isset($chapter->field_community_managers[LANGUAGE_NONE])) {
			  		foreach ($chapter->field_community_managers[LANGUAGE_NONE] as $community_manager) {
			  			$notify_uids[] = $community_manager['target_id'];
			  		}
			  	}

			  	// unique ids
			  	$notify_uids = array_unique($notify_uids);

			  	// get addreses
			  	$notify_emails = array();
			  	foreach ($notify_uids as $uid) {
			  		$notify_user = user_load($uid);
			  		$notify_emails[] = $notify_user->mail;
			  	}

			  	// if no chapter leaders or community managers, send to support email
			  	if (sizeof($notify_emails) == 0) {
			  		$notify_emails[] = $conf['tm_chapters_support_email'];
			  	}

			  	// saluation
			  	$salutation = "team";
			  	if (sizeof($notify_uids) == 1) {
			  		$single_user = user_load($notify_uids[0]); 
			  		$salutation = strip_tags($single_user->field_user_first_name[LANGUAGE_NONE][0]['value']);
			  	}

			  	// number of future events
			  	$query_sql = "SELECT COUNT(*) total FROM {field_data_field_event_date} e 
			  	LEFT JOIN field_revision_field_is_partner_event p on e.entity_id = p.entity_id
			  	WHERE p.entity_type = 'node' AND field_is_partner_event_value = 0
			  	AND e.entity_type = 'node' AND field_event_date_value >= NOW();";
			  	$num_events = db_query($query_sql)->fetch();
			  	$upcoming_events = $num_events->total;

			  	// number of members in past 30 days
			  	$query = "SELECT COUNT(*) amount FROM {users} u WHERE u.created > UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 30 day));";
			  	$num_members = db_query($query)->fetch();
			  	$number_signups = $num_members->amount;

		  		// prepare subject and body
		  		$subject = $conf["tm_chapters_leader_no_events_warning_subject"];
		  		$subject = str_replace("__SITE_NAME__", $conf["tm_site_name"], $subject);
		  		$subject = str_replace("__CHAPTER_NAME__", $chapter_name, $subject);

		  		$body = $conf["tm_chapters_leader_no_events_warning"];
		  		$body = str_replace("__SALUTATION__", $salutation, $body);
		  		$body = str_replace("__SITE_NAME__", $conf["tm_site_name"], $body);
		  		$body = str_replace("__CHAPTER_NAME__", $chapter_name, $body);
		  		$body = str_replace("__UPCOMING_EVENTS__", $upcoming_events, $body);
		  		$body = str_replace("__NUMBER_SIGNUPS__", $number_signups, $body);

		  		tm_chapters_leaders_send_notification($notify_emails, $subject, $body, $chapter);
		  	}


		  } else {
		  	drush_print("- " . $chapter->title . ": last event " . $last_event_time);
		  }

		} else {

			if (isset($future_result['node'])) {
				drush_print("- " . $chapter->title . ": Upcoming event in next 3 months");
			} else {
				drush_print("- " . $chapter->title . ": No previous events");
			}
		}

	}
	

	drush_print("Done.");
}


/**
 * Run automatied community management tasks
 */
function tm_chapters_community_management_reports_drush() {

	global $conf;

	// check confirm
	$confirm = false;
	$confirm_opt = drush_get_option('confirm');
	if ($confirm_opt != null) {
		$confirm = ($confirm_opt == "true");
	}
	if ($confirm_opt == null) {
		drush_print("Usage: drush tm-community-management-reports --uri=HOSTNAME --confirm=true\nOptional: --target_chapter=123");
		return;
	}

	if (!$confirm) {
		drush_print("Running simulation only. Use --confirm=true to process.");
	}

	if (drush_get_option("uri") == null) {
		drush_print("Warning! Need to --uri to ensure URLS are correctly rendered in emails");
	}

	// target chapter
	$target_chapter = drush_get_option('target_chapter');
	if ($target_chapter != null) {
		drush_print("Target chapter id: " . $target_chapter);
	}

	// Get all chapters
	$query = "SELECT nid, title FROM node WHERE type = 'chapter' AND status = 1 ORDER BY title";
	$results = db_query($query)->fetchAll();
	$chapter_nids = array();
	foreach ($results as $result) {
	  $chapter_nids[$result->title] = $result->nid;
	}

	// STEP 1.
	// SEND REPORTS
	drush_print("Sending chapter report emails...");
	foreach ($chapter_nids as $title => $nid) {

		// optional: only target specific chapter
		if ($target_chapter != null) {
			if ($target_chapter != $nid) {
				continue;
			}
		}

		// Load chapter
		$chapter = node_load($nid);
		$chapter_name = tm_chapters_formal_name($chapter);

		// Print chapter
		drush_print("- " . $chapter->title);

		// get some report data
		$report_data = _tm_reports_get_chapter_insights($nid);

		
		// notify chapter leaders
		$notify_uids = array();
		if (isset($chapter->field_chapter_leaders[LANGUAGE_NONE])) {
			foreach ($chapter->field_chapter_leaders[LANGUAGE_NONE] as $chapter_leader) {
				$notify_uids[] = $chapter_leader['target_id'];
			}
		}

		// notify community managers
		if (isset($chapter->field_community_managers[LANGUAGE_NONE])) {
			foreach ($chapter->field_community_managers[LANGUAGE_NONE] as $community_manager) {
				$notify_uids[] = $community_manager['target_id'];
			}
		}

		// unique ids
		$notify_uids = array_unique($notify_uids);

		// get addreses
		$notify_emails = array();
		foreach ($notify_uids as $uid) {
			$notify_user = user_load($uid);
			$notify_emails[] = $notify_user->mail;
		}

		// if no chapter leaders or community managers, send to support email
		if (sizeof($notify_emails) == 0) {
			$notify_emails[] = $conf['tm_chapters_support_email'];
		}

		// saluation
		$salutation = "team";
		if (sizeof($notify_uids) == 1) {
			$single_user = user_load($notify_uids[0]); 
			$salutation = strip_tags($single_user->field_user_first_name[LANGUAGE_NONE][0]['value']);
		}

		// chapter insights url
		$chapter_insights_url = drupal_get_path_alias('node/' . $chapter->nid) . "/insights";

		// subject stats
		// report members joined in 30 days
		$subject_stats = "";
		if ($report_data["data_values"]["members_30_days_total"] > 0) {
			$new_members = "new members";
			if ($report_data["data_values"]["members_30_days_total"] == 1) {
				$new_members = "new member";
			}
			$subject_stats = " - " . $report_data["data_values"]["members_30_days_total"] . " " . $new_members;
		}
		// report > 10% growth
		if (($report_data["data_values"]["members_30_days_pct"] >= 10) and ($report_data["data_values"]["members_30_days_pct"] < 1000)) {
			$subject_stats = " - " . $report_data["data_values"]["members_30_days_pct"] . "% growth";
		}

		// approval link
		$unapproved_members_link = drupal_get_path_alias('node/' . $chapter->nid) . "/unapproved-members";
		$approval_reminder_message = "";
		$min_approval_members_pct = $conf["tm_chapters_leader_weekly_report_min_approved"];
		if ($report_data["data_values"]["members_approved_pct"] < $min_approval_members_pct) {
			$approval_reminder_message .= "<br><strong>Your Verified Members is below " . $min_approval_members_pct . "%</strong><br>Please take a moment to review your <a href='__SERVER_HTTP__://__SERVER_NAME__/" . $unapproved_members_link . "'>Guest Members</a> and pending requests.<br>";
		}

		// num_event_payments
		$event_payments_message = "";
		if (isset($report_data["data_values"]["num_event_payments"])) {
			if ($report_data["data_values"]["num_event_payments"] > 0) {
				$event_payments_message = "(" . number_format($report_data["data_values"]["num_event_payments"]) . " payments)";
			}
		}

		// num guests members
		$membership_counts = _tm_chapters_get_membership_counts($chapter->nid);
      	$members_unapproved_total = $membership_counts["members_unapproved_total"];
      	$guest_members_stat = "";
      	if ($members_unapproved_total > 0) {
      		$guest_members_stat = "<br>Guest members: " . $members_unapproved_total . " (<a href='__SERVER_HTTP__://__SERVER_NAME__/" . $unapproved_members_link . "'>please review</a>)";
      	}

		// commissions
		// Feb 2019 - retired
		$commissions_summary = "";

		// prepare subject and body
		$subject = $conf["tm_chapters_leader_weekly_report_subject"];
		$subject = str_replace("__SITE_NAME__", $conf["tm_site_name"], $subject);
		$subject = str_replace("__CHAPTER_NAME__", $chapter_name, $subject);
		$subject = str_replace("__REPORT_DATE__", date("j F", time() - (60*60*12)), $subject);
		$subject = str_replace("__SUBJECT_STATS__", $subject_stats, $subject);
		$subject = str_replace("__SALUTATION__", $salutation, $subject);

		$body = $conf["tm_chapters_leader_weekly_report"];
		$body = str_replace("__SALUTATION__", $salutation, $body);
		$body = str_replace("__SITE_NAME__", $conf["tm_site_name"], $body);
		$body = str_replace("__CHAPTER_NAME__", $chapter_name, $body);
		$body = str_replace("__UPCOMING_EVENTS__", $upcoming_events, $body);
		$body = str_replace("__NUMBER_SIGNUPS__", $number_signups, $body);
		$body = str_replace("__CHAPTER_INSIGHTS_URL__", $chapter_insights_url, $body);
		$body = str_replace("__REPORT_DATE__", date("j F", time() - (60*60*12)), $body);
		$body = str_replace("__APPROVAL_REMINDER_MESSAGE__", $approval_reminder_message, $body);
		$body = str_replace("__EVENT_PAYMENTS_MESSAGE__", $event_payments_message, $body);
		$body = str_replace("__COMMISSIONS_SUMMARY__", $commissions_summary, $body);
		$body = str_replace("__REVIEW_GUESTS_URL__", $unapproved_members_link, $body);
		$body = str_replace("__TOTAL_CHAPTER_GUEST_MEMBERS__", $members_unapproved_total, $body);
		$body = str_replace("__GUEST_MEMBERS_STAT__", $guest_members_stat, $body);

		// insert stats
		foreach ($report_data["data_values"] as $key => $value) {
			if (is_numeric($value)) {
				$value = number_format($value);
			}
			$body = str_replace("__" . strtoupper($key) . "__", $value, $body);
		}

		// Send report
		if ($confirm) {

			// send report
			tm_chapters_leaders_send_notification($notify_emails, $subject, $body, $chapter);

		}
		
	}

}

/**
 * Remove all community managers from all chapters
 */
function tm_chapters_community_management_remove_all_managers_drush() {

	// check confirm
	$confirm = false;
	$confirm_opt = drush_get_option('confirm');
	if ($confirm_opt != null) {
		$confirm = ($confirm_opt == "true");
	}
	if ($confirm_opt == null) {
		drush_print("Usage: drush tm-community-management-remove-all-managers --confirm=true");
		return;
	}
	if ($confirm == false) {
		drush_print("Not confirmed, exiting.");
		return;
	}

	// remove all community managers
	$query = "SELECT nid, title FROM node WHERE type = :chapter AND status = 1 ORDER BY title";
	$results = db_query($query, array(":chapter" => "chapter"))->fetchAll();
	$chapter_nids = array();
	foreach ($results as $result) {
	  $chapter = node_load($result->nid);
	  drush_print("Removed community managers for " . $chapter->title . "...");
	  if (isset($chapter->field_community_managers[LANGUAGE_NONE])) {
		foreach ($chapter->field_community_managers[LANGUAGE_NONE] as $key => $manager) {
			unset($chapter->field_community_managers[LANGUAGE_NONE][$key]);
		}
	  }
	  node_save($chapter);
	}

	drush_print("Completed.");
}

/**
 * Update chapter stats display cache for all chapters
 * This is relied on for public display of chapter stats
 */
function tm_chapters_update_display_stats_cache_drush() {

	// cache for 1 day
	$cache_seconds = 60 * 60 * 25;

	// check confirm
	$confirm = false;
	$confirm_opt = drush_get_option('confirm');
	if ($confirm_opt != null) {
		$confirm = ($confirm_opt == "true");
	}
	if ($confirm_opt == null) {
		drush_print("Usage: drush tm-chapters-update-display-stats-cache --confirm=true");
		return;
	}
	if ($confirm == false) {
		drush_print("Not confirmed, exiting.");
		return;
	}

	// update display stats cache for all chapters
	$query = "SELECT nid, title FROM node WHERE type = :chapter AND status = 1 ORDER BY title";
	$results = db_query($query, array(":chapter" => "chapter"))->fetchAll();
	$chapter_nids = array();
	foreach ($results as $result) {
	  $chapter = node_load($result->nid);
	  drush_print("Updating display stats cache for " . $chapter->title . "...");
	  _tm_reports_get_chapter_insights($chapter->nid, true, $cache_seconds);
	}

	drush_print("Completed.");
}

/**
 * Generate chapter members csv 
 */
function tm_chapters_members_csv_drush(){

	drush_print("Generating members CSV.");

	// check confirm
	$confirm = false;
	$confirm_opt = drush_get_option('confirm');
	if ($confirm_opt != null) {
		$confirm = ($confirm_opt == "true");
	}
	if ($confirm_opt == null) {
		drush_print("Usage: drush tm-chapters-members-csv-cache --confirm=true");
		return;
	}
	if ($confirm == false) {
		drush_print("Not confirmed, exiting.");
		return;
	}

	// calculate time
	$totaltime = microtime(true);

	// update members csv cache for all chapters
	$query = "SELECT nid, title FROM node WHERE type = :chapter AND status = 1 ORDER BY title";
	$results = db_query($query, array(":chapter" => "chapter"))->fetchAll();
	$chapter_nids = array();
	foreach ($results as $result) {

		// load chapter
		$chapter = node_load($result->nid);
		drush_print("Updating members csv cache for " . $chapter->title . "...");

		// generate csv
		$starttime = microtime(true);
		$csv = tm_chapters_members_csv_generate($chapter->nid);

		// save to cache 25 hours
		cache_set('tm-chapters-members-csv-' . $chapter->nid, $csv, 'cache', time() + (60 * 60 * 25));
		$endtime = microtime(true);
		$timediff = $endtime - $starttime;
		drush_print("Done (" . round($timediff, 2) . " seconds). Saved to cache key tm-chapters-members-csv-" . $chapter->nid);

	}

	// completed time
	$endtime = microtime(true);
	$timediff = $endtime - $totaltime;
	drush_print("Total time " . round($timediff, 2) . " seconds");
	drush_print("Completed.");

}


