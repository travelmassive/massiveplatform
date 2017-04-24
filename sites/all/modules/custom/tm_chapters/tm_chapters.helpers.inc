<?php

// tm_chapters.helpers.inc - chapter helper methods

/**
 * Helper method get the number of members in a chapter 
 *  Will include non-approved members in the count
 */
function _tm_chapters_get_num_members($chapter_id) {
    $flag = flag_get_flag('signup');
    return $flag->get_count($chapter_id);
}

/**
 * Helper method return number of approved and unapproved members
 */
function _tm_chapters_get_membership_counts($chapter_id) {

  $membership_counts = array();
  $flag = flag_get_flag('signup', NULL);
  $approved_role = user_role_load_by_name("approved user");

  // TOTAL MEMBERS
  $membership_counts["members_total"] = $flag->get_count($chapter_id);

  // TOTAL APPROVED MEMBERS
  $query_sql = "SELECT COUNT(DISTINCT users_roles.uid) total FROM {flagging} f RIGHT JOIN users_roles ON users_roles.uid = f.uid WHERE users_roles.rid = :role_id AND f.fid = :fid AND f.entity_type = 'node' AND f.entity_id = :chapter_id";
  $query = db_query($query_sql, array(':fid' => $flag->fid, ':chapter_id' => $chapter_id, ':role_id' => $approved_role->rid))->fetch();
  $members_approved_total = $query->total;
  $membership_counts["members_approved_total"] = $members_approved_total;

  // TOTAL UNAPPROVED MEMBERS
  $members_unapproved_total = $membership_counts["members_total"] - $members_approved_total;
  $membership_counts["members_unapproved_total"] = $members_unapproved_total;

  return $membership_counts;

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

/**
 * Helper method
 * Get uids of all chapter members
 */
function _tm_chapters_get_chapter_member_uids($chapter_id) {

  $chapterflag = flag_get_flag('signup', NULL);

  $members_in_chapter = array();

  $results = db_select('flagging', 'f')
    ->fields('f')
    ->condition('fid', $chapterflag->fid ,'=')
    ->condition('entity_id', $chapter_id ,'=')
    ->execute();
  while ($result = $results->fetchAssoc()){
    $members_in_chapter[] = $result['uid'];
  }

  // return ids of members in chapter
  return $members_in_chapter;
}

/**
 * Helper method
 * Get unique uids of all chapter members from array of chapters
 */
function _tm_chapters_get_chapter_member_uids_multiple($chapter_ids) {

  $uids = array();
  foreach ($chapter_ids as $chapter_id) {
    $chapter_uids = _tm_chapters_get_chapter_member_uids($chapter_id);
    $uids = array_merge($uids, $chapter_uids);
  }

  // get unique uids (member can be a member of multiple chapters)
  $unique_uids = array_unique($uids);

  // return unique uids
  return $unique_uids;

}

