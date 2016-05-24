<?php

// tm_users.helpers.inc - helper methods for user

/* Get the signup score of the users profile */
/* It will be between 0 and 100 based on how much the user has filled in */
function tm_users_signup_score($uid = null) {
  return tm_users_signup_tips(true, $uid);
}

/**
 * Check if a member is in a user's chapter
 */
function tm_users_member_in_users_chapter($member_uid, $chapter_leader_uid, $return_true_no_chapter = false) {

  // Step 1. Fetch list of chapters the user has joined
  $flags = flag_get_user_flags("node", null, $member_uid);
  $chapters_nids = array();
  foreach ($flags as $flag_name => $flag) {
    if ($flag_name == "signup") {
      foreach($flag as $f) {
        $chapters_nids[] = $f->entity_id;
      }
    }
  }

  // If user has not joined any chapters, we can return true
  // This is for cases where chapter leader can moderator members with no chapter
  if (((sizeof($chapters_nids)) == 0) && ($return_true_no_chapter)) {
    return true;
  }

  // Step 2. Fetch list of chapters the user is a leader of
  $chapters_leader_nids = array();
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'chapter')
    ->propertyCondition('status', NODE_PUBLISHED)
    ->fieldCondition('field_chapter_leaders', 'target_id', $chapter_leader_uid, '=');

  $result = $query->execute();
  if (isset($result['node'])) {
    $chapters_leader_nids = array_keys($result['node']);
  }

  $intersect = array_intersect($chapters_nids, $chapters_leader_nids);
  return (sizeof($intersect) > 0);
}

/**
 * Checks if a user is a chapter leader of a chapter
 */
function tm_users_is_chapter_leader($uid, $chapterid) {
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'chapter')
    ->propertyCondition('nid', $chapterid)
    ->propertyCondition('status', NODE_PUBLISHED)
    ->fieldCondition('field_chapter_leaders', 'target_id', $uid, '=');

  $result = $query->execute();

  if (isset($result['node'])) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Get a list of uids of companies managed by the user
 */
function tm_users_get_companies($uid){

  $query = new EntityFieldQuery();
  $markup = '';
  $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'organization')
    ->propertyCondition('status', NODE_PUBLISHED)
    ->fieldCondition('field_owner', 'target_id', $uid, '=');

  $result = $query->execute();

  $companies_nids = array();
  if (isset($result['node'])) {
    $companies_nids = array_keys($result['node']);
  }

  return $companies_nids;
}

/**
 * Return default cover image
 * If tm_images_default_path is set then return a random image based in node id
 * URL must be in public:// format
 */
function _tm_users_get_default_image($default_image_uri, $uid) {

  global $conf;

  if (isset($conf["tm_images_default_path"])) {
    if (($default_image_uri == $conf["tm_images_default_field_image"]) or ($default_image_uri == "")) {
      $cover_files = $conf["tm_images_default_cover_user"];
      $image_index = $uid % sizeof($cover_files);
      $cover_file = $conf["tm_images_default_path"] . $cover_files[$image_index];
      return $cover_file;
    }
  }

  // no random image, return what we got
  return $default_image_uri;
}

/**
 * Helper method to get field value for comparison
 * Used in tm_users_user_update
 */
function _tm_users_get_field_value($field) {

  // empty value
  if (sizeof($field) == 0) {
    return ""; 
  }

  // empty value
  if (sizeof($field[LANGUAGE_NONE]) == 0) {
    return "";
  }
 
  // text value
  if (isset($field[LANGUAGE_NONE][0]['value'])) {
    return $field[LANGUAGE_NONE][0]['value'];
  }

  // url value
  if (isset($field[LANGUAGE_NONE][0]['url'])) {
    return $field[LANGUAGE_NONE][0]['url'];
  }

  // file value
  if (isset($field[LANGUAGE_NONE][0]['fid'])) {
    return $field[LANGUAGE_NONE][0]['fid'];
  }

  // unkown type
  return null;

}

/**
 *  Get count of followers for a user
 */
function tm_users_get_num_followers($uid) {

  $flag = flag_get_flag("follow_members");
  $query = "SELECT COUNT(*) total FROM {flagging} f WHERE f.fid = :fid AND f.entity_id = :uid AND f.entity_type = 'user'";
  $result = db_query($query, array(':fid' => $flag->fid, ':uid' => $uid))->fetch();
  return $result->total;
}

/**
 * Get count of how many people user is following
 */
function tm_users_get_num_following($uid) {

  $flag = flag_get_flag("follow_members");
  $query = "SELECT COUNT(*) total FROM {flagging} f WHERE f.fid = :fid AND f.uid = :uid AND f.entity_type = 'user'";
  $result = db_query($query, array(':fid' => $flag->fid, ':uid' => $uid))->fetch();
  return $result->total;
}

/**
 * Get count of how many events user has registered for
 */
function tm_users_get_num_registered_events($uid) {

  $flag = flag_get_flag("event_register");
  $query = "SELECT COUNT(*) total FROM {flagging} f WHERE f.fid = :fid AND f.uid = :uid AND f.entity_type = 'node'";
  $result = db_query($query, array(':fid' => $flag->fid, ':uid' => $uid))->fetch();
  return $result->total;
}

/**
 * Helper method to get all moderators
 */
function tm_users_get_all_moderators() {

  $moderator_role = user_role_load_by_name("moderator");

  $query_sql = "SELECT DISTINCT users.uid FROM users
    LEFT JOIN users_roles ON users_roles.uid = users.uid 
    WHERE users_roles.rid = :role_id";
  $query = db_query($query_sql, array(':role_id' => $moderator_role->rid));
  $results = $query->fetchAll();

  $moderator_ids = array();
  foreach ($results as $result) {
    $moderator_ids[] = $result->uid;
  }

  return $moderator_ids;
}

/**
 * Helper method to get all chapters user is a member of
 */
function tm_users_get_chapters($user) {

  $flags = flag_get_user_flags("node", null, $user->uid);
  $chapters_nids = array();
  foreach ($flags as $flag_name => $flag) {
    if ($flag_name == "signup") {
      foreach($flag as $f) {
        $chapters_nids[] = $f->entity_id;
      }
    }
  }

  return $chapters_nids;
}

/**
 * Helper method to get all chapters user is a leader of
 */
function tm_users_get_chapters_leading($user) {

  // Step 2. Fetch list of chapters the user is a leader of
  $chapters_leader_nids = array();
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'chapter')
    ->propertyCondition('status', NODE_PUBLISHED)
    ->fieldCondition('field_chapter_leaders', 'target_id', $user->uid, '=');

  $result = $query->execute();
  if (isset($result['node'])) {
    $chapters_leader_nids = array_keys($result['node']);
  }

  return $chapters_leader_nids;
}

/**
 * Helper method to get events a member has registered for
 */
function tm_users_get_users_events($user) {

  $flags = flag_get_user_flags("node", null, $user->uid);
  $event_nids = array();
  foreach ($flags as $flag_name => $flag) {
    if ($flag_name == "event_register") {
      foreach($flag as $f) {
        $event_nids[] = $f->entity_id;
      }
    }
  }

  return $event_nids;
}

/**
 * Return uid of a user by email
 * Return null if user email does not exist
 */
function tm_users_get_userid_by_email($email) {

  // check db
  $query = "SELECT uid, mail FROM {users} u WHERE mail = :email LIMIT 1";
  $result = db_query($query, array(':email' => $email))->fetch();

  // found a match
  if (isset($result->uid)) {
    return $result->uid;
  }

  // no match
  return null;
}

/**
 * Return day of the week that a user signed up
 * Takes into account user timezone. If not record found return null
 */
function tm_users_get_signup_day_of_week($uid) {

  // check db
  $query = "SELECT uid, created, timezone FROM {users} u WHERE uid = :uid LIMIT 1";
  $result = db_query($query, array(':uid' => $uid))->fetch();

  // create datetime object using timezone and created
  if (isset($result->uid)) {
    
    // create datetime object
    try {
      $dt = new DateTime($result->timezone); 
    } catch (Exception $e) {
      $dt = new DateTime("UTC"); 
    }
    
    $created_time = $dt->setTimestamp($result->created);
    return $created_time->format("l"); // day of week
  }

  // no match
  return null;
}

