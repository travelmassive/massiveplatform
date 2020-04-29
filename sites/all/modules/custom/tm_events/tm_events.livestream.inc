<?php

// tm_events.livestream.inc

/* Display Suite - Render live stream */
function _tm_event_livestream($field, $title = NULL) {

  global $user;

  // Check if we display chatbox
  $node = $field['entity'];
  if (!tm_event_livestream_check_display($node)) {
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

  // render live stream
  $html = tm_event_render_livestream_embed($youtube_video_id);
  return $html;
}

/* Check if we display the live stream */
function tm_event_livestream_check_display($node) {

  global $conf;

  // check node is event
  if ($node->type != "event") {
    return false;
  }

  // check if past event within last 1 hour
  if (tm_events_is_past_event($node, 60)) {
    return false;
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
    if ((strlen($livestream_url < 20) && (strpos($livestream_url, ".") === false))) {
      $youtube_video_id = $livestream_url;
    }
  }

  // return youtube video id
  return $youtube_video_id;

}

/* Render youtube live stream embed */
function tm_event_render_livestream_embed($youtube_video_id) {
  
  // testing
  // $youtube_video_id = "jhFDyDgMVUI"; // "rbtv2zxhufM"

  // embed template
  $embed_html = '<div id="livestream_video_container" style="width: 100%; margin-left: 16px; margin-right: 16px; padding-right: 32px; margin-bottom: 32px;"><iframe id="livestream_video" width="560" height="315" src="https://www.youtube.com/embed/' . $youtube_video_id . '?autoplay=1" frameborder="0" allow="accelerometer; autoplay; encrypted-media; picture-in-picture" allowfullscreen></iframe></div>';

  return $embed_html;

}
