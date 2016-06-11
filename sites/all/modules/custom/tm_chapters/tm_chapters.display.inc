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
 * Implement hook node_view
 */
function tm_chapters_node_view($node, $view_mode) {

  if (($node->type == "chapter") && ($view_mode == "full")) {
    tm_chapters_show_help_needed_message($node);
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
