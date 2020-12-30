<?php

// tm_chapters.display.inc - chapter display methods

/**
 * Implement hook_ds_field_info()
 */
function tm_chapters_ds_fields_info($entity_type) {
  if ($entity_type != 'node') {
    return;
  }
  $fields = array();

  $fields['tm_ds_members_chapter'] = array(
    'title' => t('Amount chapter members'),
    'field_type' => DS_FIELD_TYPE_FUNCTION,
    'ui_limit' => array('chapter|full'),
    'function' => '_tm_members_chapter'
  );

  // stats
  $fields['tm_ds_chapter_stats'] = array(
    'title' => t('Chapter Highlight Stats'),
    'field_type' => DS_FIELD_TYPE_FUNCTION,
    'ui_limit' => array('chapter|full'),
    'function' => '_tm_ds_chapter_stats'
  );

  // instant messaging
  $fields['tm_ds_chapter_instant_messaging'] = array(
    'title' => t('Instant Messaging Link'),
    'field_type' => DS_FIELD_TYPE_FUNCTION,
    'ui_limit' => array('chapter|full',),
    'function' => '_tm_chapter_instant_messaging'
  );

  return array($entity_type => $fields);
}

/**
 * Custom function to print the amount of members belonging to a chapter
 */
function _tm_members_chapter($field, $title = NULL) {
  global $conf;
  $member_label = $conf["tm_member_label"];
  $members_label = $conf["tm_members_label"];

  $flag = flag_get_flag('signup');
  $amount_members = $flag->get_count($field['entity']->nid);
  return $amount_members ? format_plural($amount_members, '1 ' . $member_label,'@members '.$members_label, array('@members' => $amount_members)) : t('No members yet');
}

/**
 * Set default images
 * Refer to settings.php configuration
 */
function tm_chapters_preprocess_field(&$variables) {
  
  global $conf;

  // Check element type
  $element_type = null;
  if (isset($variables['element']['#object']->type)) {
    $element_type = $variables['element']['#object']->type;
  }

  // Only operate on event
  if ($element_type != "chapter") {
    return;
  }

  // Replace default cover image
  if ($variables['element']['#field_name'] == 'field_image') {
    $variables['items'][0]['#item']['uri'] = _tm_chapters_get_default_image($variables['items'][0]['#item']['uri'], $variables['element']['#object']->nid);
  }

}

/**
 * Return default cover image
 * If tm_images_default_path is set then return a random image based in node id
 * URL must be in public:// format
 */
function _tm_chapters_get_default_image($default_image_uri, $nid) {

  global $conf;

  if (isset($conf["tm_images_default_path"])) {
    if (($default_image_uri == $conf["tm_images_default_field_image"])or ($default_image_uri == "")) {
      $cover_files = $conf["tm_images_default_cover_chapter"];
      $image_index = $nid % sizeof($cover_files);
      $cover_file = $conf["tm_images_default_path"] . $cover_files[$image_index];
      return $cover_file;
    }
  }

  // no random image, return what we got
  return $default_image_uri;
}

/**
 * Helper method to return chapter cover uri
 * If chapter has default cover, use _tm_chapters_get_default_image() to return random image
 * Otherwise, return chapter image
 */
function _tm_chapters_get_image_uri($chapter) {

  global $conf;

  // Return avatar if it's not the default avatar
  if (isset($chapter->field_image[LANGUAGE_NONE][0]['uri'])) {
    if ($chapter->field_image[LANGUAGE_NONE][0]['uri'] != $conf["tm_images_default_field_image"]) {
      return $chapter->field_image[LANGUAGE_NONE][0]['uri'];
    }
  }

  // Return random avatar
  return _tm_chapters_get_default_image($conf["tm_images_default_field_image"], $chapter->nid);
}

/**
 * Implement hook node_view
 */
function tm_chapters_node_view($node, $view_mode) {

  if (($node->type == "chapter") && ($view_mode == "full")) {

    // show help wanted
    tm_chapters_show_help_needed_message($node);

    // drop cookie via javascript to set default chapter on signup page
    drupal_add_js(array('chapter_referral_id' => array('id' => $node->nid)), array('type' => 'setting'));
    drupal_add_js(drupal_get_path('module', 'tm_chapters') . '/js/tm_chapters_referral.js');

  }
}

/**
 * Display chapter leader needed message
 */
function tm_chapters_show_help_needed_message($chapter) {

  global $conf; 

  $display_message = false;

  // Case 1. No chapter leaders
  if (!isset($chapter->field_chapter_leaders[LANGUAGE_NONE])) {
    $display_message = true;
  }

  // Case 2. Chapter leaders option is set
  if (!$display_message) {
    if (isset($chapter->field_chapter_leaders_needed[LANGUAGE_NONE])) {
      $display_message = ($chapter->field_chapter_leaders_needed[LANGUAGE_NONE][0]['value']);
    }
  }

  // Case 3. No events in mimimum timeframe
  // Note: this excludes events that are published to the chapter
  if (!$display_message) {

    // Check chapter age is older than minimum days
    $max_days_since_event = $conf["tm_chapters_leaders_needed_max_days_since_event"];
    $max_date = new DateTime('NOW -' . $max_days_since_event . " DAYS", new DateTimeZone('UTC'));
    $chapter_created = new DateTime('NOW', new DateTimeZone('UTC'));
    $chapter_created->setTimeStamp($chapter->created);
    if ($max_date > $chapter_created) {

      // Check that there are no upcoming events
      $next_event = tm_events_get_next_event($chapter->nid);
      if ($next_event == null) {

        // If no past events, show message
        $last_event = tm_events_get_last_event($chapter->nid);
        if ($last_event == null) {
          $display_message = true;
        } else {
          // If there are past events, show message if they are older than minimum days
          $current_date = new DateTime('now', new DateTimeZone('UTC')); // current date
          $event_timezone = _tm_get_event_timezone($last_event); // get event timezone
          $event_date = $last_event->field_event_date[LANGUAGE_NONE]; // get last event date
          $event_datetime = new DateTime($event_date[0]['value'], new DateTimeZone($event_timezone));
          $time_since_event = ($current_date->format('U') - $event_datetime->format('U'));
          if (($time_since_event / (60 * 60 * 24)) > $max_days_since_event) {
            $display_message = true;
          }
        }

      } // end if check no upcoming events
    } // end if check chapter is older than minimum days
  } // end case 3

  // Display message
  if ($display_message) {
    // Check when last event was
    drupal_add_js(array('tm_chapters' => array('leaders_needed_message' => $conf["tm_chapters_leaders_needed_message"])), array('type' => 'setting'));

    // Add javascript to form
    drupal_add_js(drupal_get_path('module', 'tm_chapters') . '/js/tm_chapters_leaders_needed.js');

    drupal_get_messages("help-tip");
    drupal_set_message($conf["tm_chapters_leaders_needed_message"], "help-tip");

  }
 
}

/**
 * Render chapter's highlight stats
 */
function _tm_ds_chapter_stats($field, $title = NULL) {

  // get chapter
  $chapter = $field['entity'];
  $nid = $chapter->nid;

  // rely on the permanent cache stored by _tm_reports_get_chapter_insights
  // run drush tm-chapters-update-display-stats-cache to update
  $chapter_ids_key = md5($nid); 
  $cache = cache_get('tm-reports-get-chapter-insights-display-' . $chapter_ids_key, 'cache');
  if (empty($cache)) {
    return;
  }

  global $conf;

  // fetch report data from cache
  // tm-reports-get-chapter-insights-display-NID
  $report_data = $cache->data;

  $connections_text = "Connections";
  $new_members_text = "New Member";
  $upcoming_events_text = "New Event";
  $past_events_text = "Past Event";

  $num_connections = $report_data["data_values"]["total_chapter_member_connections"];
  $num_new_members = $report_data["data_values"]["members_30_days_total"];
  $num_events_upcoming = $report_data["data_values"]["num_events_upcoming"];
  
  $num_past_events = $report_data["data_values"]["num_events_past_total"];

  // Make sure we have stats to display
  if (($num_new_members == 0) and ($num_events_upcoming == 0) and ($num_past_events == 0)) {
    return; 
  }

  $html = "<div class='tm_stats_container'>";

  if ($conf["tm_chapters_display_stats_connections"]) {
    if ($num_connections > 1) {
      $html .= "<span class='tm_stats tm_stats_connections'>" . tm_base_stats_format($num_connections) . " <span class='tm_stats_connections_divider'>|</span> " . $connections_text . " </span>";
    }
  }
  
  if ($num_new_members > 0) {
    if ($num_new_members > 1) { $new_members_text = "New Members"; }
    $html .= "<a href='/node/" . $nid . "/members'><span class='tm_stats tm_stats_new_members'>" . $num_new_members . " <span class='tm_stats_new_members_divider'>|</span> " . $new_members_text . " </span></a>";
  }
  
  if ($num_events_upcoming > 0) {
    if ($num_events_upcoming > 1) { $upcoming_events_text = "New Events"; }
    $html .= "<span class='tm_stats tm_stats_events'>" . $num_events_upcoming . " <span class='tm_stats_events_divider'>|</span> " . $upcoming_events_text . "</span>";
  } elseif ($num_past_events > 0) {
    if ($num_past_events > 1) { 
    $past_events_text = "Past Events"; }
      $html .= "<a href='/node/" . $nid . "/events'><span class='tm_stats tm_stats_events'>" . $num_past_events . " <span class='tm_stats_events_divider'>|</span> " . $past_events_text . "</span></a>";
  }

  $html .= "</div>";

  return $html;
}

/* Display Suite - Render event location */
/* If field_event_hide_location is set, only show location to registered attendees */
function _tm_chapter_instant_messaging($field, $title = NULL) {

  global $user;

  // check node is a chapter
  $node = $field['entity'];
  if ($node->type != "chapter") {
    return;
  }

  // check user can view the instant messaging field
  if (!tm_chapters_user_can_view_instant_messaging($user, $node)) {
    return;
  }

  // check field exists
  if (!isset($node->field_link_instant_messaging[LANGUAGE_NONE][0]['url'])) {
    return;
  }

  // get instant messenger link
  $invite_url = trim($node->field_link_instant_messaging[LANGUAGE_NONE][0]['url']);

  // must start with http:// or https://
  if (!(stripos($invite_url, "https://") === 0) and !(stripos($invite_url, "https://") === 0)) {
    return;
  }

  // tidy up
  $invite_url = str_ireplace("https://", "https://", $invite_url);
  $invite_url = str_ireplace("http://", "http://", $invite_url);

  // render
  $prefix = "";
  $label = $invite_url;
  if (stripos($invite_url, "whatsapp.com") !== false) {
    $prefix = "Join our ";
    $label = "WhatsApp Chat";
  }
  if (stripos($invite_url, "t.me") !== false) {
    $prefix = "Join our ";
    $label = "Telegram Group";
  }
  $html = $prefix . "<a target='_blank' rel='nofollow noopener' href='" . strip_tags($invite_url) . "'>" . $label . "</a>";

  return $html;
}

/**
 * Display an image wall of profiles
 */
function tm_chapters_profile_wall() {

  global $conf;

  $chapter_id = intval($_GET["chapter_id"]);
  if ($chapter_id == null) {
    print "You need to use a ?chapter_id";
    drupal_exit();
  }

  // check cache
  $cache = cache_get('tm-chapters-image-wall-' . $chapter_id, 'cache');
  if (!empty($cache)) {
   print $cache->data;
   drupal_exit();
  }

  $flag = flag_get_flag('signup');
  $query = "SELECT uid from flagging where fid = :fid and entity_id = :chapter_id order by rand() limit 5000";
  $results = db_query($query, array(':fid' => $flag->fid, ':chapter_id' => $chapter_id))->fetchAllKeyed(0,0);

  $html = "";
  foreach ($results as $uid) {

    // load account
    $account = user_load($uid);
    if ($account == null) {
      continue;
    }

    // Set avatar
    $avatar_url = image_style_url("avatar_grid", _tm_users_get_avatar_uri($account));
    $img_url = str_replace("'", "%27", file_create_url($avatar_url));

    // URL
    $user_url = drupal_get_path_alias("user/" . $account->uid);

    // Name
    $user_full_name = strip_tags($account->field_user_first_name[LANGUAGE_NONE][0]['value']) . " " . strip_tags($account->field_user_last_name[LANGUAGE_NONE][0]['value']);
    $user_full_name = str_replace("''", "", $user_full_name);

    // Only members with profile pictures
    if (strpos($avatar_url, "default_avatar") === false) {
      $html .= "<a target='_blank' title='" . $user_full_name . "' href='/" . $user_url . "'><img class='tm-subscribers-user-avatar' src='" . $img_url . "'></a> ";
    }
  }

  // store cache
  cache_set('tm-chapters-image-wall-' . $chapter_id, $html, 'cache', time() + 300); // 5 mins

  print $html;
  drupal_exit();
}
