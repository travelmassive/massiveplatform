<?php

// tm_notifications_newsfeed.drush.inc

/**
 * Implements hook_drush_command().
 */
function tm_notifications_newsfeed_drush_command() {

	$items['newsfeed-schedule-deliveries'] = array(
	  'description' => 'Schedule notification deliveries.',
	  'callback' => 'tm_notifications_newsfeed_schedule_deliveries',
	  'aliases' => array('newsfeed_sd'),
	  'options' => array(
	  	'test-mode' => 'If true, run a test pass without altering records (default false)',
	  ),
	);

	$items['newsfeed-clear-schedule'] = array(
	  'description' => 'Remove all scheduled notifications.',
	  'callback' => 'tm_notifications_clear_schedule',
	  'aliases' => array('newsfeed_cs')
	);

	$items['newsfeed-test'] = array(
		'description' => 'Send a test notification.',
		'callback' => 'tm_notifications_newsfeed_test',
		'aliases' => array('newsfeed_td'),
		'options' => array(
			'date-range' => 'daily, weekly (default) or custom (needs date-start and date-end)',
			'date-start' => 'Start date (yyyy-mm-dd)',
			'date-end' => 'End date (yyyy-mm-dd)',
			'member-email' => 'Email address of member to lookup.',
			'send-to' => 'Email address to send the notification.',
		),
	);

	$items['newsfeed-test-worker'] = array(
		'description' => 'Test the daily worker.',
		'callback' => 'tm_notifications_newsfeed_test_worker',
		'options' => array(
			'member-email' => 'Email address of member to lookup.',
			'user-id' => 'User id of member.',
			'type' => 'daily or weekly',
		),
	);

  return $items;
}

/**
 * Callback to clear all scheduled deliveries
 */
function tm_notifications_clear_schedule() {

	drush_print("Clearing all scheduled deliveries...");

	$res = db_delete('tm_notifications_worker_schedule')
	->condition('worker', 'tm_notifications_newsfeed_weekly_worker')
	->execute();

	$res = db_delete('tm_notifications_worker_schedule')
	->condition('worker', 'tm_notifications_newsfeed_daily_worker')
	->execute();

	drush_print("Done.");
}

/**
 * Callback for the newsfeed-schedule-deliveries command
 */
function tm_notifications_newsfeed_schedule_deliveries() {

	global $conf;

	// check test-mode
	$test_mode = false;
	$test_mode_opt = drush_get_option('test-mode');
	if ($test_mode_opt != null) {
		$test_mode = ($test_mode_opt == true);
		drush_print("Test mode enabled.");
		drush_print("Database records will not be altered.");
	} else {
		drush_print("Existing schedules will be removed then updated for each user.");
	}

	// time of day to schedule daily and weekly notifications
	$scheduled_daily_time = $conf["tm_newsfeed_daily_notification_time"]; // ie: 10:00:00;
	$scheduled_weekly_time = $conf["tm_newsfeed_weekly_notification_time"]; // ie 11:00:00;
	drush_print("Scheduled daily time: " . $scheduled_daily_time);
	drush_print("Scheduled weekly time: " . $scheduled_weekly_time);
	drush_print("----------------------------");

	// keep track of how long it took
	$start_time = microtime(true);

	// get notification settings
	$daily_notification = new TMNotification("daily_newsfeed");
	$weekly_notification = new TMNotification("weekly_newsfeed");

	// fetch all users
	$result = db_query('SELECT uid, name, timezone FROM {users} WHERE uid > 0 ORDER BY uid');

	// process each user
	while ($record = $result->fetch()) {

		// get uid
		$user_id = $record->uid;

		// testing
		// skip % of users using modulus on user_id
		if (isset($conf["tm_newsfeed_schedule_test_mod_userid"])) {
			$mod = $conf["tm_newsfeed_schedule_test_mod_userid"];
			if (($user_id % $mod) != 0) {
				drush_print("Skipping uid " . $user_id . " (mod is " . $mod . ")");
				continue;
			}
		}

		// get user timezone
		$users_timezone = tm_notifications_get_users_timezone($user_id, $record->timezone);
		try {
			$timezone = new DateTimeZone($users_timezone);
		} catch (Exception $e) {
			$timezone = new DateTimeZone("UTC");
			drush_print("Invalid timezone for user " . $user_id . ": " . $users_timezone_string);
		}

		// command output
		$drush_output = "uid: " . str_pad($user_id, 6). ", username: " . str_pad(substr($record->name,0,20),20) . ", timezone: " . str_pad(substr($users_timezone, 0, 20),20);

		// WEEKLY NOTIFICATION SCHEDULE
		// Send on weekly anniversary day of signup
		// If anniversary day is Saturday, schedule for Friday
		// If anniversary day is Sunday, schedule for Monday
		$weekly_identifier = 'tm_notifications_newsfeed_weekly-' . $user_id;

		// remove existing schedule
		if (!$test_mode) {
			tm_notifications_schedule_remove($weekly_identifier);
		}

		if ($weekly_notification->isEnabled($user_id)) {

			// get day of week that account was created
			$scheduled_day = tm_users_get_signup_day_of_week($user_id);

			// Don't send on weekends
			if ($scheduled_day == "Saturday") {
				$scheduled_day = "Friday";
			}
			if ($scheduled_day == "Sunday") {
				$scheduled_day = "Monday";
			}

			// schedule time on 
			$schedule_date = new DateTime($scheduled_day . " " . $scheduled_weekly_time, $timezone);
			$now = new DateTime(null, $timezone);

			// if today's scheduled date already passed, set for following week
			if ($schedule_date < $now) {
				$schedule_date->modify('+1 weeks');
				$drush_output .= ", weekly (schedule next week " . str_pad($scheduled_day, 9) . ")";
			} else {
				$drush_output .= ", weekly (schedule this week " . str_pad($scheduled_day, 9) . ")";
			}
			$drush_output .= " = " . $schedule_date->format("Y-m-d H:i");

			// set schedule
			if (!$test_mode) {
				tm_notifications_schedule($weekly_identifier, $schedule_date, 'tm_notifications_newsfeed_weekly_worker', array('user_id' => $user_id));
			}

		}

		// DAILY NOTIFICATION SCHEDULE
		$daily_identifier = 'tm_notifications_newsfeed_daily-' . $user_id;

		// remove existing schedule
		if (!$test_mode) {
			tm_notifications_schedule_remove($daily_identifier);
		}

		// add new schedule
		if ($daily_notification->isEnabled($user_id)) {

			// schedule time today
			$schedule_date = new DateTime("today " . $scheduled_daily_time, $timezone);
			$now = new DateTime(null, $timezone);

			// if today's scheduled date already passed, set for tomorrow
			if ($schedule_date < $now) {
				$drush_output .= ", daily (schedule next day)";
				$schedule_date->modify('+1 days');
			} else {
				$drush_output .= ", daily (schedule today   )";
			}
			$drush_output .= " = " . $schedule_date->format("Y-m-d H:i");

			// set schedule
			if (!$test_mode) {
				tm_notifications_schedule($daily_identifier, $schedule_date, 'tm_notifications_newsfeed_daily_worker', array('user_id' => $user_id));
			}

		}
	
		// print output
		drush_print($drush_output);

	}

	// completed
	drush_print("----------------------------");
	drush_print("Completed in " . round((microtime(true) - $start_time), 2) . ' seconds.');
	drush_print("Done.");

}

/**
 * Determine the users timezone
 * Checks last geoip (if tm_geoip enabled) and then home chapter
 */
function tm_notifications_get_users_timezone($user_id, $fallback_timezone) {

	// Option 1. From user's last login location (tm_geoip)
	if (module_exists("tm_geoip")) {
		$last_ip = tm_geoip_get_users_last_ip($user_id);
		$geoip = tm_geoip_lookup($last_ip);
		if ($geoip != null) {
			if (isset($geoip['location']['time_zone'])) {
				return $geoip['location']['time_zone'];
			}
		}
	}

	// Option 2. Timezone of home chapter
	// get users's home chapter
	$users_home_chapter = null;
	$account = user_load($user_id);
	if (isset($account->field_home_chapter[LANGUAGE_NONE][0]['target_id'])) {
		$users_home_chapter = $account->field_home_chapter[LANGUAGE_NONE][0]['target_id'];
		$chapter = node_load($users_home_chapter);
		if (isset($chapter->field_chapter_timezone[LANGUAGE_NONE][0]['value'])) {
			$chapter_timezone = $chapter->field_chapter_timezone[LANGUAGE_NONE][0]['value'];
			if ($chapter_timezone != null) {
				return $chapter_timezone;
			}
		}
	}		

	// Option 3. Fallback timezone
	if ($fallback_timezone != null) {
		return $fallback_timezone;	                       
	}

	// Return UTC if fallback is null
	return "UTC";
}

/**
 * Callback for the newsfeed-test-daily command
 */
function tm_notifications_newsfeed_test() {

	// get member email option
	$member_email = drush_get_option('member-email');
	if ($member_email == null) {
		drush_print('Usage: --member-email=user@example --send-to=user@example');
		return;
	}

	// get user by email
	$user_id = tm_users_get_userid_by_email($member_email);
	if ($user_id == null) {
		drush_print('User with email ' . $member_email . " not found.");
		return;
	}

	// get date range
	$date_range = drush_get_option('date-range');
	if ($date_range == null) {
		$date_range = "weekly";
	}

	// set start and end date
	if ($date_range == "daily") {
		$start_date = new DateTime('-1 day', new DateTimeZone('UTC'));
		$end_date = new DateTime(null, new DateTimeZone('UTC'));
	}

	if ($date_range == "weekly") {
		$start_date = new DateTime('-1 week', new DateTimeZone('UTC'));
		$end_date = new DateTime(null, new DateTimeZone('UTC'));
	}

	if ($date_range == "custom") {

		$start_date_opt = drush_get_option('date-start');
		$start_date = new DateTime();
		$start_date = $start_date->createFromFormat("Y-m-d", $start_date_opt, new DateTimeZone('UTC'));

		$end_date_opt = drush_get_option('date-end');
		$end_date = new DateTime();
		$end_date = $end_date->createFromFormat("Y-m-d", $end_date_opt, new DateTimeZone('UTC'));
	}
	
	// prepare vars for newsfeed
	$user_ids = array($user_id);
	$start_date = $start_date->format('Y-m-d H:i:s');
	$end_date = $end_date->format('Y-m-d H:i:s');

	// generate notification
	drush_print("Generating notification...");
	drush_print("-----------------------");
	drush_print("User id: " . $user_id);
	drush_print("Date range: " . $date_range); 
	drush_print("Start date: " . $start_date . " (+0 UTC)"); 
	drush_print("End date: " . $end_date . " (+0 UTC)");

	$gen = new TmNewsfeedEmailGenerator();
	$notifications = $gen->start($user_ids, $start_date, $end_date);

	// check we got a notification
	if (sizeof($notifications) == 0) {
		drush_print("No notifications generated.");
		return;
	}

	// get notification
	$notification = $notifications[0];

	// print notification to output
	drush_print("-----------------------");
	drush_print("Subject: " . $notification['subject']);
	drush_print("Preheader: " . $notification['preheader']);
	drush_print("Headline: " . $notification['headline']);
	drush_print("Body:");
	drush_print($notification['body']);
	drush_print("-----------------------");

	// print debug output
	drush_print(str_replace("\\n", "\n", $gen->get_debug_log()));
	drush_print("-----------------------");

	// send test email notification
	$send_to = drush_get_option('send-to');
	if ($send_to != null) {
		if (!valid_email_address($send_to)) {
			drush_print("Email address: " . $send_to . " is not valid.");
		}

		// send test email
		drush_print("Sending test email to: " . $send_to . "...");
		tm_newsfeed_notification_send_test_email($send_to, $notification['subject'], $notification['body'], $notification['preheader'], $notification['headline']);
	}

	drush_print("Done.");
	return;
}

/**
 * Callback method for the newsfeed-test-worker command
 */
function tm_notifications_newsfeed_test_worker() {

	// get member email and user_id
	$member_email = drush_get_option('member-email');
	$user_id = drush_get_option('user-id');
	$notif_type = drush_get_option('type');

	// default type is daily
	if ($notif_type == null) {
		$notif_type = "daily";
	}
	$notif_type = strtolower($notif_type);

	if (($member_email == null) and ($user_id == null)) {
		drush_print('Usage: --member-email=user@example or --user-id=1001 --type=daily or weekly');
		return;
	}

	// get user by email
	if ($user_id == null) {
		$user_id = tm_users_get_userid_by_email($member_email);
		if ($user_id == null) {
			drush_print('User with email ' . $member_email . " not found.");
			return;
		}
	}

	// check valid uid
	if (!tm_newsfeed_check_uid($user_id)) {
		drush_print('User id ' . $user_id . " not valid.");
		return;
	}

	// run worker
	if ($notif_type == "daily") {
		drush_print("Running tm_notifications_newsfeed_daily() with user_id = " . $user_id . "...");
		tm_notifications_newsfeed_daily_worker(array("user_id" => $user_id));
	}

	if ($notif_type == "weekly") {
		drush_print("Running tm_notifications_newsfeed_weekly() with user_id = " . $user_id . "...");
		tm_notifications_newsfeed_weekly_worker(array("user_id" => $user_id));
	}
	
	drush_print("Done.");
}

