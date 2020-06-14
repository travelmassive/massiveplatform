<?php

// tm_events.ical.inc - export ical methods


/**
 * Check if user can download ical file
 */
function tm_events_download_ical_check() {

	// all visitors can download iCal
	return true;
}

/**
 * iCal Download function 
 */
function tm_events_download_ical($nid){

	// load event
	$event = node_load($nid);

	// check valid event
	if ($event == null) {
		return "";
	}

	if ($event->type != "event") {
		return "";
	}

	// generate filename
	$filename = tm_events_generate_ical_filename($event);

	// ical header
	drupal_add_http_header('Content-type', 'text/calendar; charset=utf-8');
	drupal_add_http_header('Content-Disposition', 'attachment; filename = '.$filename);

	// generate ical
	$ical = tm_events_generate_ical($event);
	print($ical);

}

/**
 * Generate iCal filename
 */
function tm_events_generate_ical_filename($event) {

	// filename
	$filename = $event->title.".ics";
	$filename = str_replace(array(","," "), "_", $filename); // replace spaces and ,

	return $filename;
}

/**
 * Generate iCal 
 */
function tm_events_generate_ical($event, $single_event = true) {

	// import icalendar library
	define('_ZAPCAL',1);
	define('_ZAPCAL_MAXYEAR', 2036);
	define('_ZAPCAL_MAXREVENTS', 5000);
	require_once('includes/icalendar/date.php');
	require_once('includes/icalendar/recurringdate.php');
	require_once('includes/icalendar/ical.php');
	require_once('includes/icalendar/timezone.php');

	global $conf;

	// event title
	$title = $event->title;

	// date and timezone
	$event_timezone = _tm_get_event_timezone($event);
	$dt_timezone = new DateTimeZone($event_timezone);
	$datestart = new DateTime($event->field_event_date[LANGUAGE_NONE][0]['value'], $dt_timezone);
	$dateend = new DateTime($event->field_event_date[LANGUAGE_NONE][0]['value2'], $dt_timezone);
	$event_start = date_format($datestart, 'Y-m-d H:i:s'); // ie: "2020-01-01 12:00:00";
	$event_end = date_format($dateend, 'Y-m-d H:i:s'); // ie: "2020-01-01 13:00:00";

	// create the ical object
	$icalobj = new ZCiCal();

	// Add timezone data
	ZCTimeZoneHelper::getTZNode(substr($event_start,0,4),substr($event_end,0,4),$event_timezone, $icalobj->curnode);

	// create the event within the ical object
	$eventobj = new ZCiCalNode("VEVENT", $icalobj->curnode);
	$eventobj->addNode(new ZCiCalDataNode("SUMMARY:" . tm_events_ical_escape_string($title)));
	$eventobj->addNode(new ZCiCalDataNode("DTSTART:" . ZCiCal::fromSqlDateTime($event_start)));
	$eventobj->addNode(new ZCiCalDataNode("DTEND:" . ZCiCal::fromSqlDateTime($event_end)));
	$uid = $uniqueid = "event-" . $event->nid . "@" . $conf["tm_email_server_url_domain"];
	$eventobj->addNode(new ZCiCalDataNode("UID:" . $uid));
	$eventobj->addNode(new ZCiCalDataNode("DTSTAMP:" . ZCiCal::fromSqlDateTime()));

	// Add description
	$description = "";
	if (isset($event->body[LANGUAGE_NONE][0]['value'])) {
		$description = $event->body[LANGUAGE_NONE][0]['value'];
	}
	$eventobj->addNode(new ZCiCalDataNode("DESCRIPTION:" . tm_events_ical_escape_string($description)));

	// Location
	// The location is made available here to logged out users to improve useability
	// The link is delivered via email and may be accessed by an unauthenticated browser
	$venue_name = $event->field_event_venue_name[LANGUAGE_NONE][0]['value'];
	$location = $venue_name . "\n" . $event->field_location[LANGUAGE_NONE][0]['value'];
	$location = str_replace("\n", ", ", $location);
	$eventobj->addNode(new ZCiCalDataNode("LOCATION:" . tm_events_ical_escape_string($location)));

	// URL
	$uri = $conf["tm_email_server_http"] . "://" . $conf["tm_email_server_url_domain"] . "/" . drupal_get_path_alias('node/' . $event->nid);
	$eventobj->addNode(new ZCiCalDataNode("URL:" . tm_events_ical_escape_string($uri)));

	//render ical
	$ical = $icalobj->export();
	return $ical;
}

/**
 * Helper method to escape string for iCal format
 */
function tm_events_ical_escape_string($str) {

	// strip tags
	$str = strip_tags($str);

	// http://stackoverflow.com/questions/6735500/create-ical-calendar-event-with-php
	$str = preg_replace('/([\,;])/','\\\$1', $str);

	// replace new lines with \\n
	$str = str_replace(array("\r\n", "\r", "\n"), "\\n", $str); 

	return $str;

}
