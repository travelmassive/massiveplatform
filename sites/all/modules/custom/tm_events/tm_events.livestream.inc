<?php

// tm_events.livestream.inc

/* Display Suite - Render live stream */
function _tm_event_livestream($field, $title = NULL) {

  global $user;

  // Check if we display chatbox
  $node = $field['entity'];
  if (!tm_event_livestream_check_display($user, $node)) {
    return;
  }

  // Get live stream URL
  $livestream_url = tm_event_get_livestream_url($node);

  // Get youtube video id
  $youtube_video_id = tm_event_get_youtube_video_id($livestream_url);

  // Check valid video id
  if ($youtube_video_id == "") {
    return "";
  }

  // start time
  $start_time = tm_event_get_youtube_video_start_time($livestream_url);

  // render live stream
  $html = tm_event_render_livestream_embed($youtube_video_id, $start_time);
  return $html;
}

/* Check if we display the live stream */
function tm_event_livestream_check_display($account, $node, $check_mode = true, $check_livestream_on = true) {

  global $conf;

  // check node is event
  if ($node->type != "event") {
    return false;
  }

   // check livestream on
  if ($check_livestream_on) {
    if (!tm_event_livestream_is_on($node)) {
      return;
    }
  }

  // Bypass mode check for event editors and featured members
  if ($account != null) { 

    // Check if is featured member
    $is_featured_member = false;
    if (isset($node->field_event_featured_members[LANGUAGE_NONE])) {
      foreach ($node->field_event_featured_members[LANGUAGE_NONE] as $field_featured_member) {  
        if ($field_featured_member["target_id"] == $account->uid) {
          $is_featured_member = true;
        }
      }
    }

    // If is featured member, or can edit event
    if ($is_featured_member or tm_events_check_permission_edit_event($account, $node)) {
      $check_mode = false;
    }
  }

  // check live stream mode:
  // public - Everyone (default)
  // members - Only signed-in members
  // attendees - Only registered attendees
  $livestream_mode = tm_event_livestream_get_mode($node);

  // need to be signed in
  if ($check_mode and (($livestream_mode == "members") or ($livestream_mode == "attendees"))) {

    // must be signed in
    if (!user_is_logged_in()) {
      return false;
    }

    // for attendees, must be registered
    if ($livestream_mode == "attendees") {
      if (!tm_events_user_is_registered($node->nid, $account->uid)) {
        return false;
      }
    }
  }

  // check if livestream url provided
  if (tm_event_get_livestream_url($node) == "") {
    return false;
  }

  // validate livestream url
  $livestream_url = tm_event_get_livestream_url($node);
  $youtube_video_id = tm_event_get_youtube_video_id($livestream_url);
  if ($youtube_video_id == "") {
    return false;
  }

  // can view
  return true;

}

/* Display a message, if required, to the user about viewing the live stream */
function tm_event_livestream_show_display_message($account, $node) {

  $message = "";

  // check if livestream url provided
  if (tm_event_get_livestream_url($node) == "") {
    return;
  }

  // check live stream mode:
  // public - Everyone (default)
  // members - Only signed-in members
  // attendees - Only registered attendees
  $livestream_mode = tm_event_livestream_get_mode($node);

  $label = "live stream";
  if (tm_events_is_past_event($node)) {
    $label = "replay";
  }

  // only members can watch
  if ($livestream_mode == "members") {
    // need to sign in
    if (!user_is_logged_in()) {
        $message = "<a href='/user/login?destination=/node/" . $node->nid . "'>Sign in</a> to watch the " . $label . ".";
    }
  }

  // only attendees can watch
  if ($livestream_mode == "attendees") {
    if (!tm_events_is_past_event($node)) {
      // need to sign in
      if (!user_is_logged_in()) {
        $message = "<a href='/user/login?destination=/node/" . $node->nid . "'>Sign in</a> and register to watch the " . $label . ".";
      } else if (!tm_events_user_is_registered($node->nid, $account->uid)) {
        // need to register
        $message = "Register for this event to watch the " . $label . ".";
      }
    }
  }

  // no message
  if ($message != "") {
    drupal_set_message($message, "help-tip");
  }

}

/* Get live stream url */
function tm_event_get_livestream_url($node) {

  // get live stream  url
  if (isset($node->field_event_livestream_video_url[LANGUAGE_NONE][0]['value'])) {

    // return url if not blank
    $livestream_url = trim($node->field_event_livestream_video_url[LANGUAGE_NONE][0]['value']);
    if ($livestream_url != "") {
      return $livestream_url;
    }
  }

  // no url
  return null;
}

/**
 * Callback to toggle livestream
 */
function tm_events_livestream_toggle_callback($event_id, $state) {

  global $user;

  // user is logged in
  if (!user_is_logged_in()) {
    drupal_access_denied();
  }

  // valid event
  $event = node_load($event_id);
  if ($event == null) {
    drupal_access_denied();
  }

  // user can edit event
  if (!tm_events_check_permission_edit_event($user, $event)) {
    drupal_access_denied();
  }
  
  // set livestream on or off
  $event->field_event_livestream_on[LANGUAGE_NONE][0]['value'] = 0;
  if ($state == "on") {
     $event->field_event_livestream_on[LANGUAGE_NONE][0]['value'] = 1;
  }

  // save 
  node_save($event);
 
  // redirect
  drupal_goto("node/". $event_id);
}

/* Get youtube id from live stream URL */
function tm_event_get_youtube_video_id($livestream_url) {

  // default
  $youtube_video_id = "";

  // match youtube url
  // http://stackoverflow.com/questions/3392993/php-regex-to-get-youtube-video-id
  preg_match("/^(?:http(?:s)?:\/\/)?(?:www\.)?(?:m\.)?(?:youtu\.be\/|youtube\.com\/(?:(?:watch)?\?(?:.*&)?v(?:i)?=|(?:embed|v|vi|user)\/))([^\?&\"'>]+)/", $livestream_url, $matches);

  if (sizeof($matches) != 0 ) {
    $youtube_video_id = $matches[1];
  } else {
    // check if its just id
    if ((strlen($livestream_url < 20) && (strpos($livestream_url, ".") === false) && (stripos($livestream_url, "http") === false))) {
      $youtube_video_id = $livestream_url;
    }
  }

  // return youtube video id
  return $youtube_video_id;

}

/* Get youtube ?t= from live stream URL */
function tm_event_get_youtube_video_start_time($livestream_url) {

  // ?t=15
  // https://www.youtube.com/watch?v=rbtv2zxhufM&t=15
  $parts = explode("t=", $livestream_url);
  if (sizeof($parts) > 1) {
    $more_parts = explode("&", $parts[1]);
    return intval($more_parts[0]);
  }

  // ?time_continue=15
  // https://www.youtube.com/watch?v=rbtv2zxhufM&time_continue=15
  $parts = explode("time_continue=", $livestream_url);
  if (sizeof($parts) > 1) {
    $more_parts = explode("&", $parts[1]);
    return intval($more_parts[0]);
  }

  // not found
  return 0;
}

/* Render youtube live stream embed */
function tm_event_render_livestream_embed($youtube_video_id, $start_time = 0) {
  
  // testing
  // $youtube_video_id = "jhFDyDgMVUI"; // "rbtv2zxhufM"

  // optional start time
  $start_time_arg = "";
  if ($start_time > 0) {
    $start_time_arg = "&start=" . $start_time;
  }

  // embed template
  $embed_html = '<div id="livestream_video_container" style="width: 100%; position: relative; left: 16px; margin: 0 auto; margin-bottom: 32px; padding-right: 32px;"><iframe id="livestream_video" width="560" height="315" src="https://www.youtube.com/embed/' . $youtube_video_id . '?autoplay=1' . $start_time_arg . '" frameborder="0" allow="accelerometer; autoplay; encrypted-media; picture-in-picture" allowfullscreen></iframe><div id="livestream_video_container_buttons"><a href="javascript:tm_livestream_larger_video();" title="Increase video size"><span class="toggle bttn bttn-tertiary bttn-m" style="width: 38px;">+</span></a><br><a href="javascript:tm_livestream_smaller_video();" title="Decrease video size"><span class="bttn bttn-tertiary bttn-m" style="width: 38px; margin-top: 4px;">—</span></a></div></div>';

  return $embed_html;

}

/* Get live stream mode of event */
function tm_event_livestream_get_mode($node) {

  $livestream_mode = "public"; // default

  if (isset($node->field_event_livestream_mode[LANGUAGE_NONE][0]['value'])) {
    $livestream_mode = $node->field_event_livestream_mode[LANGUAGE_NONE][0]['value'];
  }

  return $livestream_mode;
}

/* Get live stream mode label of event */
function tm_event_livestream_get_mode_label($node) {

  $livestream_mode = tm_event_livestream_get_mode($node);

  if ($livestream_mode == "public") { return "everyone viewing this page"; }
  if ($livestream_mode == "members") { return "signed-in members"; }
  if ($livestream_mode == "attendees") { return "registered attendees"; }
  return "";
}

/* Get live stream enabled */
function tm_event_livestream_is_on($node) {

  if (isset($node->field_event_livestream_on[LANGUAGE_NONE][0]['value'])) {
    return $node->field_event_livestream_on[LANGUAGE_NONE][0]['value'];
  }

  return false;
}
