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

/**
 * Helper method
 * Get array of countries that have a chapter
 */
function _tm_chapters_get_countries_with_chapters() {

  // number of chapter countries
  $result = db_query("SELECT DISTINCT iso2, name from {field_data_field_country} f LEFT JOIN {countries_country} c ON f.field_country_iso2 = c.iso2 WHERE f.bundle = 'chapter' AND c.iso2 IS NOT NULL ORDER BY name ASC");
  $results = $result->fetchAll();

  $countries = array();
  foreach ($results as $result) {
    $countries[] = $result->iso2;
  }
  
  return $countries;
}


/**
 * Helper method
 * Get chapters in a country
 */
function _tm_chapters_get_chapters_in_country($country_iso2) {

  $query = "SELECT entity_id FROM field_data_field_country WHERE bundle = 'chapter' AND field_country_iso2 = :country_iso2 AND deleted = 0";
  $results = db_query($query, array(':country_iso2' => $country_iso2))->fetchAll();

  $chapter_uids = array();
  foreach ($results as $result) {
    $chapter_uids[] = $result->entity_id;
  }

  return $chapter_uids;
}

/**
 * Helper method
 * Get chapters in a country
 * Continents: 
 * AF = Africa
 * AN = Antarctica
 * AS = Asia
 * EU = Europe
 * NA = North America
 * OC = Oceania
 * SA = South Africa
 */
function _tm_chapters_get_chapters_in_continent($continent) {

  $query = "SELECT field_data_field_country.entity_id FROM field_data_field_country LEFT JOIN countries_country ON countries_country.iso2 = field_data_field_country.field_country_iso2 AND field_data_field_country.bundle = 'chapter' AND field_data_field_country.deleted = 0 WHERE countries_country.continent = :continent";
  $results = db_query($query, array(':continent' => $continent))->fetchAll();

  $chapter_uids = array();
  foreach ($results as $result) {
    $chapter_uids[] = $result->entity_id;
  }

  return $chapter_uids;
}

/**
 * Helper method
 * Get list of countries and regions
 * We refer to countries_country table
 */
function _tm_chapters_get_all_country_regions() {

  $query = "SELECT iso2, continent FROM countries_country";
  $results = db_query($query)->fetchAll();

  $regions = array();
  foreach ($results as $result) {
    $regions[$result->iso2] = $result->continent;
  }

  return $regions;
}

/**
 * Return list of all chapter ids
 */
function tm_chapters_get_all_chapters() {

  // Get all chapters
  $query = "SELECT nid, title FROM node WHERE type = 'chapter' AND status = 1 ORDER BY title";
  $results = db_query($query)->fetchAll();
  $chapter_nids = array();
  foreach ($results as $result) {
    $chapter_nids[] = $result->nid;
  }

  return $chapter_nids;
}

/**
 * Helper method to determine if we show the instant messaging field
 */
function tm_chapters_user_can_view_instant_messaging($account, $chapter) {

  // check is logged in
  if ($account->uid == 0) {
    return false;
  }

  // check if user approved
  if (!in_array('approved user', $account->roles)) {
    return false;
  }
  
  // check user is not reported
  if (tm_users_is_member_reported($account->uid)) {
    return false;
  }

  // check if user is member of chapter
  $users_chapters = tm_users_get_chapters($account);
  foreach ($users_chapters as $chapter_nid) {
    if ($chapter_nid == $chapter->nid) {
      return true;
    }
  }

  // check if community manager
  if (tm_users_is_community_manager($account->uid, $chapter->nid)) {
    return true;
  }

  // user is moderator
  if (!in_array('moderator', $account->roles)) {
    return true;
  }

  // not in chapter
  return false;
}
