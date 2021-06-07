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

  // Step 3. Fetch list of chapters the user is a community manager of
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'chapter')
    ->propertyCondition('status', NODE_PUBLISHED)
    ->fieldCondition('field_community_managers', 'target_id', $chapter_leader_uid, '=');

  $result = $query->execute();
  if (isset($result['node'])) {
    foreach (array_keys($result['node']) as $nid) {
      $chapters_leader_nids[] = $nid;
    }
  }

  $intersect = array_intersect($chapters_nids, $chapters_leader_nids);
  return (sizeof($intersect) > 0);
}

/**
 * Checks if a user is a chapter leader of a chapter
 * Also checks if user is community manager (can be toggled)
 */
function tm_users_is_chapter_leader($uid, $chapterid, $check_community_manager = true) {

  // ensure wer have a chapter id
  if ($chapterid == null) {
    return FALSE;
  }

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

  // check if community manager
  if ($check_community_manager) {
    if (tm_users_is_community_manager($uid, $chapterid)) {
      return TRUE;
    }
  }
  

  return FALSE;
}

/**
 * Checks if a user is a community manager of a chapter
 */
function tm_users_is_community_manager($uid, $chapterid) {

  // ensure wer have a chapter id
  if ($chapterid == null) {
    return FALSE;
  }

  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'chapter')
    ->propertyCondition('nid', $chapterid)
    ->propertyCondition('status', NODE_PUBLISHED)
    ->fieldCondition('field_community_managers', 'target_id', $uid, '=');

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
function tm_users_get_num_followers($uid, $flag = null) {

  // allow passing flag for performance
  if ($flag == null) {
    $flag = flag_get_flag("follow_members");
  }

  $query = "SELECT COUNT(*) total FROM {flagging} f WHERE f.fid = :fid AND f.entity_id = :uid AND f.entity_type = 'user'";
  $result = db_query($query, array(':fid' => $flag->fid, ':uid' => $uid))->fetch();
  return $result->total;
}

/**
 * Get count of how many people user is following
 */
function tm_users_get_num_following($uid, $flag = null) {

  // allow passing flag for performance
  if ($flag == null) {
    $flag = flag_get_flag("follow_members");
  }
  
  $query = "SELECT COUNT(*) total FROM {flagging} f WHERE f.fid = :fid AND f.uid = :uid AND f.entity_type = 'user'";
  $result = db_query($query, array(':fid' => $flag->fid, ':uid' => $uid))->fetch();
  return $result->total;
}

/**
 * Get count of how many events user has registered for
 */
function tm_users_get_num_registered_events($uid, $flag = null) {

  // allow passing flag for performance
  if ($flag == null) {
     $flag = flag_get_flag("event_register");
  }
 
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
function tm_users_get_chapters($user, $signup_flag = null) {

  // allow passing flag for performance
  if ($signup_flag == null) {
    $signup_flag = flag_get_flag("signup", NULL);
  }

  // query flagging table for signup flag with user
  $query = "SELECT entity_id FROM {flagging} f WHERE f.fid = :fid AND f.entity_type='node' AND f.uid=:uid";
  $rows = db_query($query, array(':fid' => $signup_flag->fid, ':uid' => $user->uid))->fetchAll();

  // add to chapters_nids
  $chapters_nids = array();
  foreach ($rows as $row) {
    $chapters_nids[] = $row->entity_id;
  }

  return $chapters_nids;
}

/**
 * Helper method to get all chapters user is a leader of
 */
function tm_users_get_chapters_leading($user, $is_community_manager = false) {

  // Fetch list of chapters the user is a leader of
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

  // Fetch list of chapters the user is a community manager of
  if ($is_community_manager) {
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'chapter')
    ->propertyCondition('status', NODE_PUBLISHED)
    ->fieldCondition('field_community_managers', 'target_id', $user->uid, '=');

    $result = $query->execute();
    if (isset($result['node'])) {
      foreach (array_keys($result['node']) as $nid) {
        if (!in_array($nid, $chapters_leader_nids)) {
           $chapters_leader_nids[] = $nid;
        }
      }
    }
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
 * Helper method to get events a member is waitlisted for
 */
function tm_users_get_users_events_waitlisted($user) {

  $flags = flag_get_user_flags("node", null, $user->uid);
  $event_nids = array();
  foreach ($flags as $flag_name => $flag) {
    if ($flag_name == "event_waitlist") {
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

/**
 * Helper method to get when user last performed a public flagging action
 * ie: signing up for an event, joining a chapter, etc
 */
function tm_users_get_last_public_flag_timestamp($uid) {

  // get list of flags which perform public actions
  $fids = array();
  $fids[] = flag_get_flag('featured_member')->fid;
  $fids[] = flag_get_flag('follow_members')->fid;
  $fids[] = flag_get_flag('follow_organizations')->fid;
  $fids[] = flag_get_flag('signup')->fid;

  // If tm_status_updates module enabled, look at member status update
  if (module_exists("tm_status_updates")) {
    $fids[] = flag_get_flag('status_updated_by_member')->fid;
  }

  // get timestamp of users's most recent flagging action
  $query = "SELECT timestamp FROM flagging WHERE uid = :uid AND fid IN (" . implode(",", $fids) . ") ORDER BY timestamp DESC LIMIT 1";
  $result = db_query($query, array(":uid" => $uid))->fetch();

  // get timestamp of last flag update
  if (isset($result->timestamp)) {
    return $result->timestamp;
  }
  // no flag updates
  return null;
}

/**
 * Return when users profile was last updated
 * Compare account and flagging timestamps
 */
function tm_users_get_update_timestamp($uid) {

  // check account
  $account = user_load($uid);
  if ($account == null) {
    return null;
  }

  // Step 1. Get account update timestamp
  $timestamp_account = null;
  if (isset($account->field_last_modified_timestamp[LANGUAGE_NONE][0]['value'])) {
    $timestamp_account = $account->field_last_modified_timestamp[LANGUAGE_NONE][0]['value'];
  }

  // Step 2. Get last public flagged action
  $timestamp_flag = tm_users_get_last_public_flag_timestamp($uid);  

  // Get most recent timestamp
  return max(array($timestamp_account, $timestamp_flag));
}

/**
 * Helper method to get approved member text
 */
function tm_users_get_approved_member_label($type = "singular") {

  global $conf;

   // plural
   if ($type == "plural") {
     $unapproved_label = "Verified Members";
     return $unapproved_label;
   }

   // singular
   $unapproved_label = "Verified Member";
   return $unapproved_label;
}

/**
 * Helper method to get unapproved member text
 */
function tm_users_get_unapproved_member_label($type = "singular") {
 
  global $conf;

  // plural
  if ($type == "plural") {
    return "Guest Members";
  }

  // singular
  return "Guest Member";
  
}

/**
 * Get array or user's organization profiles
 */
function tm_users_get_organization_profiles($uid, $join_flag_id = null) {

  // prepare query
  if ($join_flag_id == null) {

    // ordinary query
    $query = "SELECT entity_id FROM field_data_field_owner WHERE entity_type = 'node' AND bundle = 'organization' AND field_owner_target_id = :uid";
    $rows = db_query($query, array(':uid' => $uid))->fetchAll();

  } else {

    // sort by flag id
    // use with tm_subscriptions
    // profiles with subscriptions will be first
    $query = "SELECT fd.entity_id, f.fid FROM field_data_field_owner fd
    LEFT JOIN flagging f ON f.entity_id = fd.entity_id 
    AND ((f.fid = :fid) OR (f.fid IS NULL)) 
    WHERE fd.entity_type = 'node' AND fd.bundle = 'organization'
    AND fd.field_owner_target_id = :uid
    ORDER by fd.entity_id DESC";
    $rows = db_query($query, array(':fid' => $fid, ':uid' => $uid))->fetchAll();

  }
   
  $nids = array();
  foreach ($rows as $row) {
    $nids[] = $row->entity_id;
  }

  return $nids;
}

/**
 * Get user's friends (mutual followers)
 */
function tm_users_get_friends($uid) {
  $flag = flag_get_flag("follow_members");
  $query = "SELECT f1.entity_id from flagging f1 LEFT JOIN flagging f2 ON f1.entity_id = f2.uid AND f1.uid = f2.entity_id WHERE f1.fid = :fid AND f2.fid = :fid AND f1.uid = :uid";
  $results = db_query($query, array(':fid' => $flag->fid, ':uid' => $uid))->fetchAllKeyed(0,0);
  return $results;
}

/**
 * Get taxononomy tid of a user
 * Return tid or null if not found
 */
function tm_users_get_taxonomy_tid($uid) {

  $query = "SELECT field_segment_tid FROM field_data_field_segment WHERE entity_type = 'user' AND entity_id = :uid LIMIT 1";
  $rows = db_query($query, array(':uid' => $uid))->fetchAll();

  // return result
  foreach ($rows as $row) {
    return $row->field_segment_tid;
  }

  // none
  return null;
}

/**
 * Get last modified time for a user
 * Return tid or null if not found
 */
function tm_users_get_last_modified_time($uid) {

  $query = "SELECT field_last_modified_timestamp_value FROM field_data_field_last_modified_timestamp WHERE entity_type = 'user' AND entity_id = :uid LIMIT 1";
  $rows = db_query($query, array(':uid' => $uid))->fetchAll();

  // return result
  foreach ($rows as $row) {
    return $row->field_last_modified_timestamp_value;
  }

  // none
  return null;
}

/**
 * Helper method to get full name of user
 */
function tm_users_get_full_name($uid) {

  // load user
  $account = user_load($uid);
  if ($account == null) {
    return "Anonymous";
  }

  $full_name = "User";
  if (isset($account->field_user_first_name[LANGUAGE_NONE][0]['value'])) {
    $full_name = strip_tags($account->field_user_first_name[LANGUAGE_NONE][0]['value']);
  }

  if (isset($account->field_user_last_name[LANGUAGE_NONE][0]['value'])) {
    $full_name .= " " . strip_tags($account->field_user_last_name[LANGUAGE_NONE][0]['value']);
  }

  return $full_name;
}

/**
 * Helper method if member is reported
 */
function tm_users_is_member_reported($uid) {
  $moderate_report_member_flagged = flag_get_entity_flags("user", $uid, "moderate_report_member");
  return (sizeof($moderate_report_member_flagged) > 0);
}

/*
 * Encrypt and decrypt uid
 * see: https://naveensnayak.wordpress.com/2013/03/12/simple-php-encrypt-and-decrypt/
 */
function tm_users_encrypt_uid($action, $uid, $secret_key="", $secret_iv="") {

  global $conf;
  $output = false;

  $encrypt_method = "AES-256-CBC";

  if ($secret_key == "") {
    $secret_key = $conf["tm_users_avatar_secret_key"]; // 'This is my secret key';
  }
  if ($secret_iv == "") {
    $secret_iv = $conf["tm_users_avatar_secret_iv"]; // 'This is my secret iv';
  }

  // hash
  $key = hash('sha256', $secret_key);
  
  // iv - encrypt method AES-256-CBC expects 16 bytes - else you will get a warning
  $iv = substr(hash('sha256', $secret_iv), 0, 16);

  if( $action == 'encrypt' ) {
      $output = openssl_encrypt($uid, $encrypt_method, $key, 0, $iv);
      $output = base64_encode($output);
  }
  else if( $action == 'decrypt' ){
      $output = openssl_decrypt(base64_decode($uid), $encrypt_method, $key, 0, $iv);
  }

  return $output;
}

/**
 * Get count of how many events user has registered for
 */
function tm_users_get_num_flag_verified($uid, $flag = null) {

  // allow passing flag for performance
  if ($flag == null) {
     $flag = flag_get_flag("approval_approved_by");
  }
 
  $query = "SELECT COUNT(*) total FROM {flagging} f WHERE f.fid = :fid AND f.uid = :uid AND f.entity_type = 'user'";
  $result = db_query($query, array(':fid' => $flag->fid, ':uid' => $uid))->fetch();
  return $result->total;
}

/**
 *  Get count of approval actions
 */
function tm_users_get_num_flag_moderations($uid, $flag_ids = null) {

  // allow passing flag for performance
  if ($flag_ids == null) {
	  $flag_ids = array();
    $flag_ids[] = flag_get_flag("approval_is_company_or_brand")->fid;
    $flag_ids[] = flag_get_flag("approval_need_more_info")->fid;
    $flag_ids[] = flag_get_flag("moderate_non_community_profile")->fid;
  }

  $query = "SELECT COUNT(*) total FROM {flagging} f WHERE f.fid IN (" . implode(",", $flag_ids) . ") AND f.uid = :uid AND f.entity_type = 'user'";
  $result = db_query($query, array(':uid' => $uid))->fetch();
  return $result->total;
}

/** 
 * Helper method to determine if a user is featured
 * Note: $flag->is_flagged(uid) doesn't seem to work
 */
function tm_user_is_featured_member($uid) {
  
  $flag = flag_get_flag('featured_member');
  $query = "select uid from flagging where fid = :fid and entity_id = :entity_id and entity_type = 'user' limit 1";
	$result = db_query($query, array(":fid" => $flag->fid, ":entity_id" => $uid))->fetch();
	return (isset($result->uid));
}
