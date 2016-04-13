<?php

// tm_events.helpers.inc - helper methods

/** 
 * Calculate how many many spots are left
 * Returns 0 if there are no spare seats (over registered)
 * Returns -1 if there are unlimited seats or external event 
 */
function _tm_events_seats_left($event) {

  // external event
  if (isset($event->field_external_rsvp[LANGUAGE_NONE][0]['value']) && 
    $event->field_external_rsvp[LANGUAGE_NONE][0]['value'] !== '0') {
    return -1;
  }

  // no seat limit
  if (!isset($event->field_signup_limit[LANGUAGE_NONE][0]['value'])) {
    return -1;
  }

  // get seat limit
  $limit = $event->field_signup_limit[LANGUAGE_NONE][0]['value'];
    
  // count number of registrations
  $flag = flag_get_flag('event_register');
  $registered_count = $flag->get_count($event->nid);

  // Seats left
  $left = $limit - $registered_count;

  // If oversubscribed return 0
  if ($left < 0) {
    return 0;
  }

  // Return number of seats
  return $left;
}

/**
 * Return default cover image
 * If tm_images_default_path is set then return a random image based in node id
 * URL must be in public:// format
 */
function _tm_events_get_default_image($default_image_uri, $nid) {

  global $conf;

  if (isset($conf["tm_images_default_path"])) {
    if (($default_image_uri == $conf["tm_images_default_field_image"]) or ($default_image_uri == "")) {
      $cover_files = $conf["tm_images_default_cover_event"];
      $image_index = $nid % sizeof($cover_files);
      $cover_file = $conf["tm_images_default_path"] . $cover_files[$image_index];
      return $cover_file;
    }
  }

  // no random image, return what we got
  return $default_image_uri;
}

/**
 * Custom function to fetch the next event, used on chapters. Returns interval
 * between the current date and the event's date.
 */
function _tm_next_event($field, $title = NULL) {
  if ($field['entity']->type == 'chapter') {
    $chapter_id = $field['entity']->nid;

    // Fetch the current and make sure it's UTC and user-defined timezone.
    $current_date = new DateTime('now', new DateTimeZone('UTC'));

    // Fetch the nearest upcoming event that references a particular chapter.
    // Only returns one event, the one with the closest starting date.
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'node')
      ->entityCondition('bundle', 'event')
      ->propertyCondition('status', 1)
      ->fieldCondition('field_chapter', 'target_id', $chapter_id)
      ->fieldCondition('field_event_date', 'value2', $current_date->format('Y-m-d H:i:s'), '>=')
      ->fieldOrderBy('field_event_date', 'value2', 'ASC')
      ->range(0,1);
    $result = $query->execute();

    // Initialize time_until.
    $time_until = NULL;

    if (isset($result['node'])) {
      $next_event_array = array_keys($result['node']);
      $next_event = node_load($next_event_array[0]);
      $event_date = field_get_items('node',$next_event,'field_event_date');
      
      // Create datetime object and ensure UTC timezone, instead of user-defined timezone.
      $event_timezone = _tm_get_event_timezone($next_event);
      $event_datetime = new DateTime($event_date[0]['value'], new DateTimeZone($event_timezone));
      //$time_until = format_interval($event_datetime->format('U') - $current_date->format('U'), 2);
      $time_until = _tm_event_time_elapsed($event_datetime->format('U') - $current_date->format('U'));
      if ($time_until == "0 seconds") {
        return "Event now";
      }
    }

    // calculate number of members in chapter
    $flag = flag_get_flag('signup');
    global $conf;
    $member_label = $conf["tm_member_label"];
    $members_label = $conf["tm_members_label"];
    $amount_members = $flag->get_count($chapter_id);
    $no_event_text = $amount_members ? format_plural($amount_members, '1 '.$member_label,'@members '.$members_label, array('@members' => $amount_members)) : t('No members yet');

    // check if its a new chapter (less than 30 days old)
    $chapter = node_load($chapter_id);
    $days_old = (time() - $chapter->created) / (24 * 60 * 60);
    if ($days_old < 30) {
      $no_event_text = "New chapter";
    }

    // check if you are a member of the chapter
    // fetch list of chapters the user has joined
    global $user;
    $chapters_nids = array();
    $flags = flag_get_user_flags("node", null, $user->uid);
    foreach ($flags as $flag_name => $flag) {
      if ($flag_name == "signup") {
        foreach($flag as $f) {
          $chapters_nids[] = $f->entity_id;
        }
      }
    }
    if (array_search($chapter_id, $chapters_nids) !== false) {
      $no_event_text = "You are a member";
    }

    // show time to next event
    // if no event, then display number of members in chapter
    return $time_until ? t('Event in @time', array('@time' => $time_until)) : t($no_event_text);
    }

}

/**
 * Loads all the available organizers for a given city/chapter.
 * Basically all the chapter leaders of that city/chapter.
 * 
 * @param int $chapter_id
 * 
 * @return Array [stdClass]
 */
function _load_organizers_per_chapter($chapter_id) {

  // if no chapter_id, get all chapter leaders
  if ($chapter_id == null) {
    $result = db_query('
        SELECT users.name AS name, users.uid AS uid
        FROM
          {users} users
        INNER JOIN {field_data_field_chapter_leaders} f_c_l ON users.uid = f_c_l.field_chapter_leaders_target_id AND f_c_l.entity_type = :entity_type AND f_c_l.deleted = :deleted
        WHERE users.status <> :user_status
        ORDER BY name', array(
          ':entity_type' => 'node',
          ':deleted' => 0,
          ':user_status' => 0
      ))->fetchAll();
  } else {
    $result = db_query('
      SELECT users.name AS name, users.uid AS uid, n.nid AS chapter_nid
      FROM
        {users} users
      INNER JOIN {field_data_field_chapter_leaders} f_c_l ON users.uid = f_c_l.field_chapter_leaders_target_id AND f_c_l.entity_type = :entity_type AND f_c_l.deleted = :deleted
      INNER JOIN {node} n ON f_c_l.entity_id = n.nid
      WHERE users.status <> :user_status AND n.nid = :chapter_id
      ORDER BY name', array(
        ':entity_type' => 'node',
        ':deleted' => 0,
        ':user_status' => 0,
        ':chapter_id' => $chapter_id
    ))->fetchAll();
  }

  $data = array();
  foreach ($result as $value) {
    $data[$value->uid] = $value->name;
  }
  
  return $data;
}

/**
 * Fetch the timezone of an event.
 * Return values in this order:
 * 1. Returns the event's field_event_timezone is set (by moderator)
 * 2. Returns the chapter's timezone if event has a chapter
 * 3. Returns UTC if no timezone set (no chapter, no timezone set)
 *
 * Note: Due to known bugs in the "date's timezone" handling we use "none" timezone handling
 * when saving field_event_date. For this reason we use the chapter's timezone.
 * see: https://www.drupal.org/node/1455578 (Date Locale & Time Zone settings)
 * see: https://www.drupal.org/node/1707122 (Bug)
 */
function _tm_get_event_timezone($event) {

    // default timezone
    $event_timezone = 'UTC';

    // check if event has timezone enabled
    if (isset($event->field_event_timezone[LANGUAGE_NONE][0]['value'])) {
      $event_timezone = $event->field_event_timezone[LANGUAGE_NONE][0]['value'];
      return $event_timezone;
    }

    // otherwise we use use the chapter timezone of the event (if has a chapter)
    if (isset($event->field_chapter[LANGUAGE_NONE][0]['target_id'])) {
      $chapter = node_load($event->field_chapter[LANGUAGE_NONE][0]['target_id']);
      // If chapter has timezone set
      if ($chapter->field_chapter_timezone != null) {
        $event_timezone = $chapter->field_chapter_timezone[LANGUAGE_NONE][0]['value'];
      }
    }

    return $event_timezone;
}

/**
 * Custom function to return the full date and time of an event
 */
function _tm_fulldate_event($field, $title = NULL) {

  if (isset($field['entity']->field_event_date['und'][0]['value'])) {

    // Use the timezone of the event
    $event_timezone = _tm_get_event_timezone($field['entity']);
    
    $event_date = new DateTime($field['entity']->field_event_date['und'][0]['value'], new DateTimeZone($event_timezone));
    $event_date2 = new DateTime($field['entity']->field_event_date['und'][0]['value2'], new DateTimeZone($event_timezone));

    // Set default time format
    // Either 24hr or 12hr
    // see: http://en.wikipedia.org/wiki/Date_and_time_representation_by_country
    // default time format is 24 hour
    $time_format = "G:i"; // ie: 14:00
    $country_codes_with_12_hours = array("US", "AU", "CA", "UK", "PH", "ZA", "GB");
    $chapter_country_code = "US"; // default
    if (isset($field['entity']->field_chapter['und'][0]['target_id'])) {
      $chapter_id = $field['entity']->field_chapter['und'][0]['target_id'];
      $chapter = node_load($chapter_id);
      $chapter_country_code = $chapter->field_country['und'][0]['iso2'];
      if (in_array($chapter_country_code, $country_codes_with_12_hours)) {
        $time_format = "g:i a"; // ie: 2:00 pm
      }
    }

    // If user has county set, override timezone format based on their country
    global $user;
    $account = user_load($user->uid);
    if (sizeof($account->field_user_country) > 0) {
      // get user's country code if it's set
      if (isset($account->field_user_country['und'][0]['iso2'])) {
        $field_user_country = $account->field_user_country['und'][0]['iso2']; // ie: AU, US
        if (in_array($field_user_country, $country_codes_with_12_hours)) {
          $time_format = "g:i a"; // ie: 2:00 pm
        } else {
          $time_format = "G:i"; // ie: 14:00
        }
      }
    }

    if ($event_date != $event_date2) {
      return '<span class="date">' . $event_date->format('l, F j, Y') . '</span><span class="time">' . t('From') . ' ' . $event_date->format($time_format) . ' ' . t('to') . ' ' . $event_date2->format($time_format) . '</span>';      
    }
    else {
      return '<span class="date">' . $event_date->format('l, F j, Y') . '</span><span class="time">' . $event_date->format($time_format) . '</span>';
    }
  } 
}

/**
 * Display how long until an event 
 */
function _tm_event_time_elapsed($elapsed) {
    $etime = $elapsed;

    if ($etime < 1) {
        return '0 seconds';
    }

    $a = array(12 * 30 * 24 * 60 * 60 => 'year',
        30 * 24 * 60 * 60 => 'month',
        24 * 60 * 60 => 'day',
        60 * 60 => 'hour',
        60 => 'min',
        1 => 'sec'
    );

    foreach ($a as $secs => $text) {
        $d = $etime / $secs;
        if ($d >= 1) {
            $r = round($d);
            return $r . ' ' . $text . ($r > 1 ? 's' : ''); // . ' from now';
        }
    }
}

/** 
 * Helper method to get chapter object from edit form 
 */
function _tm_event_get_chapter_from_edit_form($form) {
  
  // Split url parts
  $url_request = request_path();

  // Get chapter from existing form
  // - /node/12832/edit
  $parts = explode("/", $url_request);
  if (sizeof($parts) == 3) {
    if (($parts[0] == "node") && ($parts[2] == "edit")) {
      $event = $form['#entity'];
      $event_id = $event->nid;
      $event = node_load($event_id);
      if (isset($event->field_chapter[LANGUAGE_NONE][0]['target_id'])) {
        $chapter_id = $event->field_chapter[LANGUAGE_NONE][0]['target_id'];
        return $chapter_id;
      } else {
        return null;
      }
    }
  }
  

  // New event
  // ie: /node/add/event?field_chapter=12832
  if (strpos("/" . $url_request, "node/add/event") == 1) {
    if (isset($_GET["chapter"])) {
      $chapter_id = $_GET["chapter"];
      return $chapter_id;
    } else {
      return null;
    }
  }

  // No chapter
  return null;

}

/**
 *  Formats the user's name
 */
function tm_events_username($user) {

  if ((!isset($user->field_user_first_name[LANGUAGE_NONE][0]['safe_value'])) || (!isset($user->field_user_last_name[LANGUAGE_NONE][0]['safe_value']))) {
    return $user->name;
  }

  $name = $user->field_user_first_name[LANGUAGE_NONE][0]['safe_value'] . " " . $user->field_user_last_name[LANGUAGE_NONE][0]['safe_value'];
  return $name;
}

/**
 * Helper method - pluralize members
 */
function tm_events_pluralize_members($num_members) {
  if ($num_members == 1) {
    return $num_members . " member";
  }
  return $num_members . " members";
}


// to be removed

/**
 * AJAX callback for field organizer.
 * Triggered by field chapter.
 */
/*function _tm_events_field_organizer_ajax_cb($form, $form_state) {
  return $form['field_organizer'];
}*/