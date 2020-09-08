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
      if (isset($chapter->field_chapter_timezone)) {
        if ($chapter->field_chapter_timezone != null) {
          $event_timezone = $chapter->field_chapter_timezone[LANGUAGE_NONE][0]['value'];
        }
      }
    }

    return $event_timezone;
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
    if (isset($_GET["field_chapter"])) {
      $chapter_id = $_GET["field_chapter"];
      return $chapter_id;
    } else {
      return null;
    }
  }

  // No chapter
  return null;

}

/** 
 * Helper method to get company object from edit form 
 */
function _tm_event_get_company_from_edit_form($form) {
  
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
      if (isset($event->field_event_company[LANGUAGE_NONE][0]['target_id'])) {
        $company_id = $event->field_event_company[LANGUAGE_NONE][0]['target_id'];
        return $company_id;
      } else {
        return null;
      }
    }
  }

  // New event
  // ie: /node/add/event?field_event_company=12832
  if (strpos("/" . $url_request, "node/add/event") == 1) {
    if (isset($_GET["field_event_company"])) {
      $company_id = $_GET["field_event_company"];
      return $company_id;
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

/**
 * Helper method to get the last event for a chapter
 */
function tm_events_get_last_event($chapter_id) {

  // Fetch the current and make sure it's UTC and user-defined timezone.
  $current_date = new DateTime('now', new DateTimeZone('UTC'));

  // Fetch the most recent event that references a particular chapter.
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'event')
    ->propertyCondition('status', 1)
    ->fieldCondition('field_chapter', 'target_id', $chapter_id)
    ->fieldCondition('field_event_date', 'value2', $current_date->format('Y-m-d H:i:s'), '<=')
    ->fieldOrderBy('field_event_date', 'value2', 'DESC')
    ->range(0,1);
  $result = $query->execute();

  // Past event found
  if (isset($result['node'])) {
    $past_event_array = array_keys($result['node']);
    $past_event = node_load($past_event_array[0]);
    return $past_event;
  }

  // No past event
  return null;
}

/**
 * Helper method to get the next event for a chapter
 */
function tm_events_get_next_event($chapter_id) {

  // Fetch the current and make sure it's UTC and user-defined timezone.
  $current_date = new DateTime('now', new DateTimeZone('UTC'));

  // Fetch the nearest upcoming event that references a particular chapter.
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'event')
    ->propertyCondition('status', 1)
    ->fieldCondition('field_chapter', 'target_id', $chapter_id)
    ->fieldCondition('field_event_date', 'value2', $current_date->format('Y-m-d H:i:s'), '>=')
    ->fieldOrderBy('field_event_date', 'value2', 'ASC')
    ->range(0,1);
  $result = $query->execute();

  // Upcoming event found
  if (isset($result['node'])) {
    $next_event_array = array_keys($result['node']);
    $next_event = node_load($next_event_array[0]);
    return $next_event;
  }

  // No upcoming event
  return null;
}

/**
 * Fast lookup of event is in preview mode
 */
function tm_events_is_preview_mode($nid) {

  $query = "SELECT field_event_is_preview_mode_value AS is_preview_mode FROM field_data_field_event_is_preview_mode WHERE entity_id = :nid";
  $row = db_query($query, array(':nid' => $nid))->fetch();
  if ($row != null) {
    return ($row->is_preview_mode == 1);
  }
  return false;
}

/**
 * Fast lookup if member is waitlisted for event
 */
function tm_events_user_is_waitlisted($event_id, $uid) {

  $waitlist_flag_id = flag_get_flag('event_waitlist')->fid;
  $query = "SELECT uid FROM flagging WHERE entity_id = :event_id AND fid = :fid AND uid = :uid";
  $row = db_query($query, array(':event_id' => $event_id, ':fid' => $waitlist_flag_id, ':uid' => $uid, ))->fetch();
  return ($row != null);
}

/**
 * Fast lookup if member is registered for event
 */
function tm_events_user_is_registered($event_id, $uid) {

  $register_flag_id = flag_get_flag('event_register')->fid;
  $query = "SELECT uid FROM flagging WHERE entity_id = :event_id AND fid = :fid AND uid = :uid";
  $row = db_query($query, array(':event_id' => $event_id, ':fid' => $register_flag_id, ':uid' => $uid, ))->fetch();
  return ($row != null);
}

/**
 * Fast lookup of future events member has registered for or is organizer of
 */
function tm_events_get_users_future_events($uid, $include_chapter_events = false) {

  $event_flags = array();
  $event_flags[] = flag_get_flag('event_register')->fid;
  $event_flags[] = flag_get_flag('event_waitlist')->fid;
  $event_flags[] = flag_get_flag('event_paid')->fid;

  $event_ids = array();

  // Step 1. Find all events member has registered for
  $query = "SELECT entity_id FROM flagging WHERE entity_type = 'node' AND fid IN (" . implode(",", $event_flags) . ") AND uid = :uid ORDER BY timestamp desc";
  $rows = db_query($query, array(':uid' => $uid))->fetchAll();
  foreach ($rows as $row) {
    $event_ids[] = $row->entity_id;
  }

  // Step 2. Find all events member is organizer of
  $query = "SELECT entity_id FROM field_data_field_organizer WHERE bundle = 'event' AND field_organizer_target_id = :uid AND deleted = 0";
  $rows = db_query($query, array(':uid' => $uid))->fetchAll();
  foreach ($rows as $row) {
    $event_ids[] = $row->entity_id;
  }

  // Step 3. Find all events member is owner of
  $query = "SELECT nid FROM node WHERE `type` = 'event' AND uid = :uid";
  $rows = db_query($query, array(':uid' => $uid))->fetchAll();
  foreach ($rows as $row) {
    $event_ids[] = $row->nid;
  }

  // Step 4. Find all events member is in chapter leader of
  if ($include_chapter_events) {
    $query = "SELECT nid FROM node 
        LEFT JOIN field_data_field_chapter ON node.nid = field_data_field_chapter.entity_id
        LEFT JOIN field_data_field_chapter_leaders ON field_data_field_chapter.field_chapter_target_id = field_data_field_chapter_leaders.entity_id  
      WHERE field_chapter_leaders_target_id = :uid 
      AND node.type = 'event'";
    $rows = db_query($query, array(':uid' => $uid))->fetchAll();
    foreach ($rows as $row) {
      $event_ids[] = $row->nid;
    }
  }

  // If not events, return
  $event_ids = array_unique($event_ids);
  if (sizeof($event_ids) == 0) {
    return array();
  }

  // Step 4. Get all future events within 24 hours
  // Don't worry about timezones as we have 24 hour threshold
  $future_event_ids = array();
  $query = "SELECT entity_id, field_event_date_value FROM field_data_field_event_date WHERE entity_type = 'node' AND entity_id IN (" . implode(",", $event_ids) . ") AND  field_event_date_value > DATE_SUB(NOW(), INTERVAL 1 DAY) ORDER BY field_event_date_value ASC";
  $rows = db_query($query)->fetchAll();
  foreach ($rows as $row) {
    $future_event_ids[] = $row->entity_id;
  }

  return $future_event_ids;
}

/**
 * Helper method
 * Get members in chapter who haven't registered to event 
 */
function tm_events_get_chapter_members_not_registered($event_id, $chapter_id) {

  $rsvpflag = flag_get_flag('event_register', NULL);
  $waitflag = flag_get_flag('event_waitlist', NULL);
  $chapterflag = flag_get_flag('signup', NULL);

  $members_in_chapter = array();
  $members_who_have_rsvp = array();
  $members_who_have_waitlist = array();

  $results = db_select('flagging', 'f')
    ->fields('f')
    ->condition('fid', $chapterflag->fid ,'=')
    ->condition('entity_id', $chapter_id ,'=')
    ->execute();
  while ($result = $results->fetchAssoc()){
    $members_in_chapter[] = $result['uid'];
  }

  $results = db_select('flagging', 'f')
    ->fields('f')
    ->condition('fid', $rsvpflag->fid ,'=')
    ->condition('entity_id', $event_id ,'=')
    ->execute();
  while ($result = $results->fetchAssoc()){
    $members_who_have_rsvp[] = $result['uid'];
  }

  $results = db_select('flagging', 'f')
    ->fields('f')
    ->condition('fid', $waitflag->fid ,'=')
    ->condition('entity_id', $event_id ,'=')
    ->execute();

  while ($result = $results->fetchAssoc()){
    $members_who_have_waitlist[] = $result['uid'];
  }

  // we only want people who are in the chapter who have not registered or waitlist
  $user_ids = array_diff($members_in_chapter, $members_who_have_rsvp, $members_who_have_waitlist);

  return $user_ids;
}

/**
 * Helper method
 * Get members in array of chapters who haven't registered to event 
 */
function tm_events_get_chapter_members_not_registered_multiple($event_id, $chapter_ids) {

  $uids = array();
  foreach ($chapter_ids as $chapter_id) {
    $chapter_uids = tm_events_get_chapter_members_not_registered($event_id, $chapter_id);
    $uids = array_merge($uids, $chapter_uids);
  }

  // get unique uids
  $unique_uids = array_unique($uids);

  // return unique uids
  return $unique_uids;

}

/**
 * Helper method to get array of chapters event is published to
 */
function tm_events_get_published_chapters($event) {

  if (!isset($event->field_chapter_publish[LANGUAGE_NONE][0])) {
    return array();
  }

  $chapter_ids = array();
  foreach ($event->field_chapter_publish[LANGUAGE_NONE] as $item) {
    $chapter_ids[] = $item['target_id'];
  }

  return $chapter_ids;
}

/**
 * Helper method to check if user a chapter leader of any published chapters.
 * Returns id of chapter user is leading, otherwise null if no match.
 */
function tm_events_check_published_chapter_leader($event, $user_id) {

  // get published chapters of event
  $published_chapters = tm_events_get_published_chapters($event);

  // check if user is a chapter leader of published chapter
  foreach($published_chapters as $published_chapter_id) {
    if (tm_users_is_chapter_leader($user_id, $published_chapter_id)) {
      return $published_chapter_id;
    }
  }

  // not match
  return null;
}

/** 
 * Helper method to get chapter from event
 */
function _tm_event_get_chapter_id($event) {
  
  if (isset($event->field_chapter[LANGUAGE_NONE][0]['target_id'])) {
    $chapter_id = $event->field_chapter[LANGUAGE_NONE][0]['target_id'];
    return $chapter_id;
  }

  // No chapter
  return null;

}

/**
 * Helper method get all future events for chapter(s)
 * Returns array of event_id, event_title, chapter_id, and chapter_short_code
 */
function tm_events_get_future_events_for_chapters($chapter_ids = null, $include_external_rsvp = false) {

  // Step 1. Get all future events for a chapter
  $query = "SELECT nid, title, field_chapter_target_id, field_chapter_shortcode_value FROM node
  LEFT JOIN field_data_field_chapter ON node.nid = field_data_field_chapter.entity_id
  LEFT JOIN field_data_field_chapter_shortcode ON field_data_field_chapter.field_chapter_target_id = field_data_field_chapter_shortcode.entity_id
  LEFT JOIN field_data_field_external_rsvp ON node.nid = field_data_field_external_rsvp.entity_id  
  LEFT JOIN field_data_field_event_date ON node.nid = field_data_field_event_date.entity_id
  WHERE node.type = 'event'";

  if ($chapter_ids != null) {
    $query .= " AND field_data_field_chapter.field_chapter_target_id IN (" . implode(",", $chapter_ids) . ")";
  }

  if (!$include_external_rsvp) {
     $query .= " AND field_external_rsvp_value = 0";
  }

  $query .= " AND field_event_date_value > DATE_SUB(NOW(), INTERVAL 1 DAY)
  ORDER BY field_event_date_value ASC";

  // Get rows
  $rows = db_query($query)->fetchAll();

  $event_ids = array();
  foreach ($rows as $row) {
    $event_ids[$row->nid] = array("event_id" => $row->nid, "event_title" => $row->title, "chapter_id" => $row->field_chapter_target_id  , "chapter_short_code" => $row->field_chapter_shortcode_value);
  }

  // return results
  return $event_ids;
}

/**
 * Check if event is past event
 */
function tm_events_is_past_event($event, $grace_minutes = 0) {
  
  // time since start and end of event
  $event_date = field_get_items('node',$event,'field_event_date');
  $current_date = new DateTime('now', new DateTimeZone('UTC'));
  $event_timezone = _tm_get_event_timezone($event);
  $event_datetime_start = new DateTime($event_date[0]['value'], new DateTimeZone($event_timezone));
  $event_datetime_end = new DateTime($event_date[0]['value2'], new DateTimeZone($event_timezone));
  $seconds_since_event_start = $current_date->format('U') - $event_datetime_start->format('U');
  $seconds_since_event_end = $current_date->format('U') - $event_datetime_end->format('U');

  // is past event
  $is_past_event = ($seconds_since_event_end - ($grace_minutes * 60) > 0);

  return $is_past_event;

}

/**
 * Get seconds until start of event
 */
function tm_events_seconds_until_event_starts($event) {
  
  // time since start and end of event
  $event_date = field_get_items('node',$event,'field_event_date');
  $current_date = new DateTime('now', new DateTimeZone('UTC'));
  $event_timezone = _tm_get_event_timezone($event);
  $event_datetime_start = new DateTime($event_date[0]['value'], new DateTimeZone($event_timezone));
  $event_datetime_end = new DateTime($event_date[0]['value2'], new DateTimeZone($event_timezone));
  $seconds_since_event_start = $current_date->format('U') - $event_datetime_start->format('U');
  $seconds_since_event_end = $current_date->format('U') - $event_datetime_end->format('U');

  return $seconds_since_event_start * -1;

}

/**
 * Get country name of an event
 */
function tm_events_get_event_country_name($event) {

  $event_chapter_country = "";
  
  if (isset($event->field_chapter[LANGUAGE_NONE][0])) {
    $event_chapter_nid = $event->field_chapter[LANGUAGE_NONE][0]['target_id'];
    $event_chapter = node_load($event_chapter_nid);
    if ($event_chapter != null) {
      if (isset($event_chapter->field_country[LANGUAGE_NONE][0]['iso2'])) {
        $country_code = $event_chapter->field_country[LANGUAGE_NONE][0]['iso2'];
        $country_names = tm_base_get_all_country_names();
        $event_chapter_country = $country_names[$country_code];
      }
    }
  }

  return $event_chapter_country;
}

/**
 * Helper method if event is online
 */
function tm_events_is_online_event($event) {

  // if online event is set
  if (isset($event->field_event_is_online[LANGUAGE_NONE][0]['value'])) {
    return ($event->field_event_is_online[LANGUAGE_NONE][0]['value'] == 1);
  }

  // not online event
  return false;
}

/**
 * Helper method to get all registered attendees for an event
 */
function tm_events_get_registered_attendees($event_id, $limit = 100000) {

  // get list of account
  $rsvpflag = flag_get_flag('event_register', NULL);
 
  // Generate uid list
  $user_ids = array();

  // Get registered attendees
  $results = db_select('flagging', 'f')
    ->fields('f')
    ->condition('fid', $rsvpflag->fid ,'=')
    ->condition('entity_id', $event_id ,'=')
    ->orderBy('timestamp', 'DESC')
    ->range(0,$limit)
    ->execute();

  // create user_ids list from results
  while ($result = $results->fetchAssoc()){
    $user_ids[] = $result['uid'];
  }
  
  return $user_ids;

}

/**
 * Helper method to get all events for a chapter
 */
function tm_events_get_all_chapters_events($chapter_id) {

  $event_ids = array();
  $query = "SELECT entity_id FROM field_data_field_chapter WHERE bundle = 'event' AND field_chapter_target_id = :chapter_id AND deleted = 0";
  $rows = db_query($query, array(':chapter_id' => $chapter_id))->fetchAll();
  foreach ($rows as $row) {
    $event_ids[] = $row->entity_id;
  }
  return $event_ids;
}
