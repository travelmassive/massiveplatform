<?php

// tm_newsfeed.display.inc - display methods

/**
 * Generate a notification preview for moderator
 */
function tm_newsfeed_admin_notification_preview() {

	global $user;
	global $conf;

	// get userid from 
	if (isset($_GET["uid"])) {
		$user_id = $_GET["uid"];
	} else {
		$user_id = $user->uid;
	}

	$gen = new TmNewsfeedEmailGenerator();

	$user_ids = array($user_id);

	// get date range
	if (isset($_GET["date-range"])) {
		$date_range = $_GET["date-range"];
	} else {
		$date_range = "weekly";
	}

	// check valid date range
	if (($date_range != "daily") and ($date_range != "weekly") and ($date_range != "custom")) {
		echo "Invalid date range (daily, weekly, or custom)";
		return;
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

		$start_date_opt = $_GET['date-start'];
		$start_date = new DateTime();
		$start_date = $start_date->createFromFormat("Y-m-d", $start_date_opt, new DateTimeZone('UTC'));

		$end_date_opt = $_GET['date-end'];
		$end_date = new DateTime();
		$end_date = $end_date->createFromFormat("Y-m-d", $end_date_opt, new DateTimeZone('UTC'));

		if (($start_date == null) or ($end_date == null)) {
			echo "Invalid date format.";
			return;
		}
	}
	
	// prepare vars for newsfeed
	$user_ids = array($user_id);
	$start_date_string = $start_date->format('Y-m-d H:i:s');
	$end_date_string = $end_date->format('Y-m-d H:i:s');

	// generate notification
	$notifications = $gen->start($user_ids, $start_date_string, $end_date_string);

	// display preview details and options
	$html = "";
	$html .= "Date range: " . $date_range;
	$html .= " &middot; <a href='/admin/newsfeed/notification_preview?uid=" . $user_id . "&date-range=daily'>daily</a>";
	$html .= " &middot; <a href='/admin/newsfeed/notification_preview?uid=" . $user_id . "&date-range=weekly'>weekly</a>";
	$html .= " &middot; <a href='/admin/newsfeed/notification_preview?uid=" . $user_id . "&date-range=custom&date-start=" . $start_date->format('Y-m-d') . "&date-end=" . $end_date->format('Y-m-d') . "'>custom</a><br>";
	$html .= "Start date: " . $start_date_string . " (+0 UTC)<br>";
	$html .= "End date: " . $end_date_string . " (+0 UTC)<br>";

	$html .= "<hr style='border-top: 1px solid #eee;'>";

	if (sizeof($notifications) == 0) {
		$html .= "No notification generated (check content feeds).";
	} else {
		$html .= "<br><strong>Subject: " . $notifications[0]['subject'] . "</strong>";
		$html .= "<br>" . $notifications[0]['body'];
		$html .= "<hr style='border-top: 1px solid #eee;'>";
		$html .= "<a href='javascript:void(0);' onClick=\"document.getElementById('debug').style.display='block';\">Show developer info</a><div id='debug' style='display: none;'><pre>" . str_replace("\\n", "<br>", $gen->get_debug_log()) . "</pre></div>";
	}

	header('Content-Type: text/html; charset=utf-8');
	echo $html;
	return;
}

/**
 * Approval callback for test callback
 */
function tm_newsfeed_admin_notification_preview_callback() {

	global $conf;
	global $user;

	// check preview is enabled
	if (!$conf['tm_newsfeed_moderator_preview']) {
		return false;
	}

	// need to be administrator or moderator
	if (in_array("administrator", $user->roles) or in_array("moderator", $user->roles)) {
		return true;
	}

	// not allowed
	return false;
}
