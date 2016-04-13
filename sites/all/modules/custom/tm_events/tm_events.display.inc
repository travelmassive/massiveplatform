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
    return theme('tm_date_avatar', array(
      'date' => $event_date,
    ));
  }
}
