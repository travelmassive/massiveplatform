<?php

// tm_chapters.helpers.inc - chapter helper methods

/* Helper method get the number of members in a chapter 
 *  Will include non-approved members in the count
 */
function _tm_chapters_get_num_members($chapter_id) {
    $flag = flag_get_flag('signup');
    return $flag->get_count($chapter_id);
}

/**
 * Helper method to return formal name of chapter
 * If chapter is a group, omit the site name
 */
function tm_chapters_formal_name($chapter) {

  global $conf;

  $chapter_name = "";

  if (isset($chapter->field_chapter_is_group[LANGUAGE_NONE][0]['value'])) {
    if ($chapter->field_chapter_is_group[LANGUAGE_NONE][0]['value']) {
      // ie: Responsible Tourism Networking
      $chapter_name = $chapter->title;
    } 
  }

  if ($chapter_name == "") {
    if (stripos($chapter->title, $conf["tm_site_name"]) === false) {
      // New York Travel Massive
      $chapter_name =  $conf["tm_site_name"] . " " . $chapter->title;
    } else {
      // Travel Massive Live
      $chapter_name = $chapter->title;
    }
  }

  return $chapter_name;
}

/**
 * Helper method get number of events member has attended or waitlisted in this chapter
 * Flag name can be "event_register" or "event_waitlist"
 */
function tm_chapters_get_num_events_for_member($uid, $chapter_id, $flag_name = "event_register") {

  $flag = flag_get_flag($flag_name, NULL);
  $query = "SELECT COUNT(*) amount FROM flagging LEFT JOIN field_data_field_chapter ON field_data_field_chapter.entity_id = flagging.entity_id WHERE field_chapter_target_id = :chapter_id AND flagging.fid = :fid AND flagging.uid = :uid";
  $num_events = db_query($query, array(':chapter_id' => $chapter_id, 'fid' => $flag->fid, ':uid' => $uid))->fetch();

  return $num_events->amount;
}

/**
 * Helper method get all chapter short codes
 * Return array of nid => shortcode
 */
function tm_chapters_get_all_chapter_shortcodes() {

  $query = "SELECT entity_id, field_chapter_shortcode_value FROM field_data_field_chapter_shortcode WHERE entity_type = 'node' AND bundle = 'chapter' AND deleted = 0";
  $results = db_query($query)->fetchAll();

  $chapters_shortcodes = array();
  foreach ($results as $result) {
    $chapters_shortcodes[$result->entity_id] = $result->field_chapter_shortcode_value;
  }

  return $chapters_shortcodes;

}
