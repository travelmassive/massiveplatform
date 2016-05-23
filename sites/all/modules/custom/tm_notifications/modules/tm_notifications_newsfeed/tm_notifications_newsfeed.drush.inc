<?php

// tm_notificiations_newsfeed.drush.inc

/**
 * Implements hook_drush_command().
 */
function tm_notificiations_newsfeed_drush_command() {

	$items['newsfeed-schedule-deliveries'] = array(
	  'description' => 'Schedule delivery.',
	  'callback' => 'tm_notifications_newsfeed_schedule_deliveries',
	  'aliases' => array('newsfeed_sd'),
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
 * Callback for the newsfeed-schedule-deliveries command
 */
function tm_notifications_newsfeed_schedule_deliveries() {

	$all_users = array();

	$result = db_query('SELECT uid, name FROM {users} WHERE uid > 0 ORDER BY uid');

	// todo
	$catch_uid = 10327;
	while ($record = $result->fetch()) {
		if ($record->uid == $catch_uid) {
			drush_print('Found uid ' . $record->uid . "...");

			// create schedule notification identifier
			$identifier = 'tm_notifications_newsfeed_weekly-' . $record->uid;

			// remove any previous schedules
			tm_notifications_schedule_remove($identifier);
				
			// calculate scheduled date for notification		
			$schedule_date = new DateTime(null, new DateTimeZone('UTC')); // todo: timezone
			$schedule_date->modify('+1 minutes');
			
			// add to notification schedule
			tm_notifications_schedule($identifier, $schedule_date, 'tm_notifications_newsfeed_weekly_worker', array('user_id' => $record->uid));

		}
		//drush_print('Checking uid ' . $record->uid . "...");
	}

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
		tm_newsfeed_notification_send_test_email($send_to, $notification['subject'], $notification['body']);
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
		$notif_type == "daily";
	}
	$notif_type = strtolower($notif_type);

	if (($member_email == null) and ($user_id == null)) {
		drush_print('Usage: --member-email=user@example or --user-id=1001');
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

