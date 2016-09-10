<?php

// tm_events.display.inc - display methods

/**
 * Display Suite - Show register and waitlist messages
 */
function _tm_event_message($field, $title = NULL) {

  global $user;

  // check if user is flagged
  $node = $field['entity'];

  if ($node->type == "event") {

      // just return body if not logged in
      if (!user_is_logged_in()) {
        return null;
      }

      // External event link
      if (isset($node->field_external_rsvp[LANGUAGE_NONE][0]['value'])) {
        $external_rsvp = $node->field_external_rsvp[LANGUAGE_NONE][0]['value'];
        if ($external_rsvp === '1') {
          $rsvp_link = $node->field_rsvp_link[LANGUAGE_NONE][0]['url'];
          if ($rsvp_link != "") {
            $heading = l(t('Register for this event'), $rsvp_link, array('attributes' => array('target'=>'_blank')));
            return $heading;
          }
        }
      }

      // extra information to prepend
      $heading = "You have not yet registered";
      $extra = "";

      // registered for event and register message
      $registered_flag = flag_get_flag('event_register');
      $registered_for_event = ($registered_flag && $registered_flag->is_flagged($node->nid, $user->uid));
      if ($registered_for_event) {
        // register message
        $heading = "<strong>You are registered for this event</strong>";

        if (isset($node->field_register_event_message['und'][0]['value'])) {
          $field_register_event_message = trim($node->field_register_event_message['und'][0]['value']);
          $field_register_event_message = strip_tags(trim($field_register_event_message));
          $extra = "<p>" . $field_register_event_message . "</p>";
        }     
      }

      // waitlisted for event and waitlist message
      $waitlist_flag = flag_get_flag('event_waitlist');
      $waitlisted_for_event = ($waitlist_flag && $waitlist_flag->is_flagged($node->nid, $user->uid));
      if ($waitlisted_for_event) {
        // waitlist message
        $heading = "<strong>You are on the waiting list</strong>";

        if (isset($node->field_waitlist_event_message['und'][0]['value'])) {
          $field_waitlist_event_message = trim($node->field_waitlist_event_message['und'][0]['value']);
          $field_waitlist_event_message = strip_tags(trim($field_waitlist_event_message));
          $extra = "<p>" . $field_waitlist_event_message . "</p>";
        }
      }

      // event description
      $description = $heading . $extra;

      if (trim($description) == "") {
        return null;
      }

      // return description
      return $description;
    }
}

/* Display Suite - Render event location */
/* If field_event_hide_location is set, only show location to registered attendees */
function _tm_event_location($field, $title = NULL) {

  global $user;

  // check if user is flagged
  $node = $field['entity'];
  if ($node->type != "event") {
    return;
  }

  // If logged in, check if registered or waitlisted
  $registered_flag = false; // default if not logged in
  $waitlist_for_event = false; // default if not logged in
  if (user_is_logged_in()) {
    $registered_flag = flag_get_flag('event_register');
    $registered_for_event = ($registered_flag && $registered_flag->is_flagged($node->nid, $user->uid));
    $waitlist_flag = flag_get_flag('event_waitlist');
    $waitlist_for_event = ($waitlist_flag && $waitlist_flag->is_flagged($node->nid, $user->uid));
  }

  // Determine if we show the location
  // If the event has field_event_hide_location set to TRUE
  $show_location = true;
  if (isset($node->field_event_hide_location[LANGUAGE_NONE][0]['value'])) {
    if ($node->field_event_hide_location[LANGUAGE_NONE][0]['value'] == 1) {

      // Hide if not logged in or not registered
      if (!user_is_logged_in()) {
        $show_location = false;
      }
      else if (!$registered_for_event) {
        $show_location = false;
      }
    }
  }

  // If not showing the location
  if (!$show_location && !$waitlist_for_event) {
    $html = 'Register to see the event location';
    return $html;
  } else if (!$show_location && $waitlist_for_event) {
    $html = 'Only registered attendees can see the location';
    return $html;
  }

  // Otherwise we can show the event location
  $event_venue_name = "";
  if (isset($node->field_event_venue_name[LANGUAGE_NONE][0]['value'])) {
    $event_venue_name = $node->field_event_venue_name[LANGUAGE_NONE][0]['value'];
  }

  $event_location = "";
  if (isset($node->field_location[LANGUAGE_NONE][0]['value'])) {
    $event_location = $node->field_location[LANGUAGE_NONE][0]['value'];
  }

  $event_map_link = "";
  if (isset($node->field_map_link[LANGUAGE_NONE][0]['url'])) {
    $event_map_link = $node->field_map_link[LANGUAGE_NONE][0]['url'];
  }
  
  // Render the location html
  $html = '<h2><span>Location</span></h2>';
  $html .= '<div class="detail-venue-name">' . t($event_venue_name) . '</div>';
  $html .= '<div class="detail-venue-address">' . str_replace("\n", "<br>", t($event_location)) . '</div>';
  if ($event_map_link) {
    $html .= '<div class="detail-venue-link"><a href="' . t($event_map_link) . '" rel="nofollow" target="_blank">View map</a></div>';
  }

  return $html;
}

/**
 * Custom function to prepare date for a date badge.
 */
function _tm_date_avatar($field, $title = NULL) {
  if (isset($field['entity']->field_event_date['und'][0]['value'])) {

    // Use the timezone of the event
    $event_timezone = _tm_get_event_timezone($field['entity']);

    $event_date = new DateTime($field['entity']->field_event_date['und'][0]['value'], new DateTimeZone($event_timezone));
    $event_date2 = new DateTime($field['entity']->field_event_date['und'][0]['value2'], new DateTimeZone($event_timezone));

    return theme('tm_date_avatar', array(
      'date' => $event_date,
      'date2' => $event_date2
    ));
  }
}

/**
 * Custom function to return the full date and time of an event
 */
function _tm_fulldate_event($field, $title = NULL) {

  global $user;
  global $conf;

  $event = $field['entity'];

  // Make sure we have an event date
  if (!isset($event->field_event_date[LANGUAGE_NONE][0]['value'])) {
    return;
  }

  // Step 1. Set time format
  $time_format = _tm_date_get_date_format($user, $event);

  // Step 2. Create event date using event's timezone
  $event_timezone = _tm_get_event_timezone($event);
  $dt_timezone = new DateTimeZone($event_timezone);
  $event_date = new DateTime($event->field_event_date[LANGUAGE_NONE][0]['value'], $dt_timezone);
  $event_date2 = new DateTime($event->field_event_date[LANGUAGE_NONE][0]['value2'], $dt_timezone);

  // Step 3. Set up event timezone array
  $timezone_name = _tm_date_pretty_timezone_name($event_timezone);
  $event_timezones = array($event_timezone => $timezone_name);

  // Step 4. Check if event is online
  if (isset($event->field_event_is_online[LANGUAGE_NONE][0]['value'])) {
    if ($event->field_event_is_online[LANGUAGE_NONE][0]['value'] == true) {

      // Include user's timezone
      if (isset($user->timezone)) {
        $timezone_name = _tm_date_pretty_timezone_name($user->timezone);
        $event_timezones[$user->timezone] = $timezone_name;
      }

      // Include tm_event_online_timezones
      foreach ($conf["tm_event_online_timezones"] as $timezone => $timezone_name) {
        $event_timezones[$timezone] = $timezone_name;
      }

    }
  }

  // Only show unique timezones  
  $event_timezones = array_unique($event_timezones, SORT_REGULAR);

  // Step 5. Create html
  $event_html = "";
  $first_date = true;
  foreach ($event_timezones as $timezone => $timezone_name) {

    // Set timezone of event date
    $display_dt_timezone = new DateTimeZone($timezone);
    $event_date->setTimeZone($display_dt_timezone);
    $event_date2->setTimeZone($display_dt_timezone);

    $css_class = "";
    if (!$first_date && (sizeof($event_timezones) > 1)) {
      $css_class = "date-alternative";
    }
    $first_date = false;
    $event_html .= '<span class="date ' . $css_class . '">';

    // Show timezone name if more than one timezone
    if (sizeof($event_timezones) > 1) {
      $event_html .= $timezone_name . " - ";
    }

    $multi_day = ($event_date->format('l, F j, Y') != $event_date2->format('l, F j, Y'));

    // multi day
    if ($multi_day) {
      $event_html .= '<span style="font-weight: normal; width: 2.2rem; display: inline-block;">Start</span>';
      $event_html .= $event_date->format('l, F j, Y') . ' <span style="font-weight: normal;">' . $event_date->format($time_format) . "</span>";
      $event_html .= '<br><span style="font-weight: normal; width: 2.2rem; display: inline-block;">End</span>';
      $event_html .= $event_date2->format('l, F j, Y') . ' <span style="font-weight: normal;">' . $event_date2->format($time_format) . "</span>";
      $event_html .= "</span>";
    }
    // same day, start and end time
    elseif ($event_date != $event_date2) {
      $event_html .= $event_date->format('l, F j, Y') . '</span><span class="time ' . $css_class . '">' . t('From') . ' ' . $event_date->format($time_format) . ' to ' . $event_date2->format($time_format);      
    }
    // only start time
    else {
      $event_html .= $event_date->format('l, F j, Y') . '</span><span class="time ' . $css_class . '">' . $event_date->format($time_format);
    }

    $event_html .= "</span>";

  }

  return $event_html;

}

/**
 * Return the display format for an event
 * Use the event and user's timezone to determine format.
 * G:i - 24hour - ie: 14:00
 * g:i a - 12hour - ie: 2:00pm
 */
function _tm_date_get_date_format($user, $event) {

  // Check chapter timezone
  // Either 24hr or 12hr
  // see: http://en.wikipedia.org/wiki/Date_and_time_representation_by_country
  // default time format is 24 hour
  $time_format = "G:i"; // ie: 14:00
  $country_codes_with_12_hours = array("US", "AU", "CA", "UK", "PH", "ZA", "GB");
  $chapter_country_code = "US"; // default
  if (isset($event->field_chapter[LANGUAGE_NONE][0]['target_id'])) {
    $chapter_id = $event->field_chapter[LANGUAGE_NONE][0]['target_id'];
    $chapter = node_load($chapter_id);
    $chapter_country_code = $chapter->field_country[LANGUAGE_NONE][0]['iso2'];
    if (in_array($chapter_country_code, $country_codes_with_12_hours)) {
      $time_format = "g:i a"; // ie: 2:00 pm
    }
  }
  
  // If user has county set, override timezone format based on their country
  $account = user_load($user->uid);
  if (sizeof($account->field_user_country) > 0) {
    // get user's country code if it's set
    if (isset($account->field_user_country[LANGUAGE_NONE][0]['iso2'])) {
      $field_user_country = $account->field_user_country[LANGUAGE_NONE][0]['iso2']; // ie: AU, US
      if (in_array($field_user_country, $country_codes_with_12_hours)) {
        $time_format = "g:i a"; // ie: 2:00 pm
      } else {
        $time_format = "G:i"; // ie: 14:00
      }
    }
  }

  return $time_format;
}

/**
 * Lookup and return a displayable name for a timezone
 */
function _tm_date_pretty_timezone_name($timezone) {

  $timezone_system_name = timezone_name_from_abbr($timezone); // ie: America/Los_Angeles
  $parts = explode("/", $timezone_system_name); // split
  $timezone_name = $parts[sizeof($parts) - 1]; // ie: Los_Angeles
  $timezone_name = str_replace("_", " ", $timezone_name); // ie: Los Angeles

  return $timezone_name;
}
