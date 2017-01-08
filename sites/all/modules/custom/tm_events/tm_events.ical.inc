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
 * Generate iCal header
 */
function tm_events_generate_ical_header($prodid, $timezone) {
	$ical = "BEGIN:VCALENDAR
VERSION:2.0
PRODID:__PRODID__
CALSCALE:GREGORIAN
X-WR-TIMEZONE:__VTIMEZONE__";

	$ical = str_replace("__PRODID__", tm_events_ical_escape_string($prodid), $ical);
	$ical = str_replace("__VTIMEZONE__", $timezone, $ical);

	return $ical;
}

/**
 * Generate iCal footer
 */
function tm_events_generate_ical_footer() {
	$ical = "END:VCALENDAR";
	return $ical;
}

/**
 * Generate iCal 
 */
function tm_events_generate_ical($event, $single_event = true) {

	global $conf;

	// ical fields
	$prodid = "-//" . tm_events_ical_escape_string($conf["tm_site_name"]) . "//NONSGML v1.0//EN";
	$uniqueid = "event-" . $event->nid . "@" . $conf["tm_email_server_url_domain"];

	// date and timezone
	$event_timezone = _tm_get_event_timezone($event);
	$dt_timezone = new DateTimeZone($event_timezone);
	$datestart = new DateTime($event->field_event_date[LANGUAGE_NONE][0]['value'], $dt_timezone);
	$dateend = new DateTime($event->field_event_date[LANGUAGE_NONE][0]['value2'], $dt_timezone);

	// The location is made available here to logged out users to improve useability
	// The link is delivered via email and may be accessed by an unauthenticated browser
	$venue_name = $event->field_event_venue_name[LANGUAGE_NONE][0]['value'];
	$location = $venue_name . "\n" . $event->field_location[LANGUAGE_NONE][0]['value'];
	$location = str_replace("\n", ", ", $location);
	$description = "";
	if (isset($event->body[LANGUAGE_NONE][0]['safe_value'])) {
		$description = $event->body[LANGUAGE_NONE][0]['safe_value'];
	}
	$uri = $conf["tm_email_server_http"] . "://" . $conf["tm_email_server_url_domain"] . "/" . drupal_get_path_alias('node/' . $event->nid);

	$summary = $event->title;

	// ical template
	$ical = "";
	if ($single_event) {
		$ical .= tm_events_generate_ical_header($prodid, $event_timezone) . "\n";
	}

	$ical .= "BEGIN:VEVENT
UID:__UID__
DTSTART;TZID=__VTIMEZONE__:__DTSTART__
DTEND;TZID=__VTIMEZONE__:__DTEND__
DTSTAMP:__DTSTAMP__
LOCATION:__LOCATION__
DESCRIPTION:__DESCRIPTION__
URL;VALUE=URI:__URL__
SUMMARY:__SUMMARY__
END:VEVENT";

if ($single_event) {
	$ical .= "\n" . tm_events_generate_ical_footer();
}
	// format template
	$ical = str_replace("__DTEND__", date_format($dateend, 'Ymd\THis'), $ical);
	$ical = str_replace("__UID__", $uniqueid, $ical);
	$ical = str_replace("__DTSTAMP__", date('Ymd\THis\Z', time()), $ical);
	$ical = str_replace("__LOCATION__", tm_events_ical_escape_string($location), $ical);
	$ical = str_replace("__DESCRIPTION__", tm_events_ical_escape_string($description), $ical);
	$ical = str_replace("__URL__", tm_events_ical_escape_string($uri), $ical);
	$ical = str_replace("__SUMMARY__", tm_events_ical_escape_string($summary), $ical);
	$ical = str_replace("__DTSTART__", date_format($datestart, 'Ymd\THis'), $ical);
	$ical = str_replace("__VTIMEZONE__", $event_timezone, $ical);

	return $ical;
}

/**
 * Helper method to escape string for iCal format
 */
function tm_events_ical_escape_string($str) {
	// http://stackoverflow.com/questions/6735500/create-ical-calendar-event-with-php
	return preg_replace('/([\,;])/','\\\$1', $str);
}
