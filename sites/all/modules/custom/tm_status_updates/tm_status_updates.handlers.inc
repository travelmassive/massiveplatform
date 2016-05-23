<?php

// tm_status_updates.handlers.inc - ajax handlers for posting and updating status updates

/**
 * Check if its possible for user to post an update
 */
function tm_status_updates_check_user_post_update($uid) {

  return true;

  global $user;
  global $conf;

  // need to be logged in
  if (!user_is_logged_in()) {
    return false;
  }

  // check if messaging is enabled
  if (!$conf["tm_newsfeed_enabled"]) {
    return false;
  }

  // check its your uid
  if ($user->uid != $uid) {
    return false;
  }
 
  // sender need to be approved member
  /*if (!in_array("approved user", $user->roles)) { 
    return false;
  }*/

  // ok, we can post update
  return true;
}

/** 
 * Post update
 * tm_newsfeed_check_post_user_update should be called first to ensure they can send a message
 * returns array. ie: array("result" => false, "error_message" => "something"); 
 */
function tm_status_updates_post_user_update_ajax() {

  global $conf;

  // get posting uid from url
  // ie: /user/10527/post_update
  $uid = arg(1);

  // Load user posting update
  $posting_user = user_load($uid);
  if ($posting_user == false) {
    $ret = array("result" => false, "error_message" => "Unable to retrieve user account");
    drupal_json_output($ret);
    return;
  }

  // Get status update
  $status_update = trim($_POST["update_status"]);
 
  // Softfail if update too short
  if (strlen($status_update) < 3) {
    $ret = array("result" => false, "error_message" => "Update is too short.");
    drupal_json_output($ret);
    return;
  }

  // Softfail if update too long
  if (strlen($status_update) > 250) {
    $ret = array("result" => false, "error_message" => "Update is too long.");
    drupal_json_output($ret);
    return;
  }

  // Update status in database
  $invite = db_insert('tm_newsfeed_status_updates')
    ->fields(array(
      'entity_id' => $posting_user->uid,
      'entity_type' => 'user',
      'status_update' => trim($status_update),
      //'created' => 'NOW',
      //'latitude' => '',
      //'longitude' => '',
      'poster_uid' => $posting_user->uid,
    ))
    ->execute();

  // Flag user as having updated status
  $flag = flag_get_flag('newsfeed_user_status_updated');
  $flag->flag("unflag", $posting_user->uid); // unflag so we can flag again
  $flag->flag("flag", $posting_user->uid);

  // Updated successfully
  $ret = array("result" => true, "error_message" => null);
  drupal_json_output($ret);
  return;
}
