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

  // need post_as and action
  if (!isset($_POST["post_as"])) {
    return false;
  }

  if (!isset($_POST["action"])) {
    return false;
  }
 
  // get entity
  // ie: uid-123
  // ie: nid-100
  $post_as = $_POST["post_as"];
  $parsed = tm_status_updates_parse_post_as($post_as);
  $entity_type = $parsed["entity_type"];
  $entity_id = $parsed["entity_id"];
  if (($entity_type == null) or ($entity_id == null)) {
    return false;
  }

  // get action
  $action = $_POST["action"];

  // check if user can edit based on their company profiles, and chapters
  if (tm_status_updates_check_permission($entity_type, $entity_id, $action, $user->uid)) {
    return true;
  }

  // not allowed to post
  return false;
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

  // Get post_as
  // ie: uid-123
  // ie: nid-123
  $post_as = trim($_POST["post_as"]);
  $parsed = tm_status_updates_parse_post_as($post_as);
  $entity_type = $parsed["entity_type"];
  $entity_id = $parsed["entity_id"];
  if (($entity_type == null) or ($entity_id == null)) {
   return false;
  }

  // Get action
  // ie: create, edit, delete, moderate
  $action = trim($_POST["action"]);

  // Get status update
  $status_update = trim($_POST["update_status"]);

  if ($action == "create") {
    return tm_status_updates_post_user_create($entity_type, $entity_id, $status_update);
  }

  if ($action == "edit") {
    return tm_status_updates_post_user_edit();
  }

  if ($action == "delete") {
    return tm_status_updates_post_user_delete();
  }

  if ($action == "moderate") {
    tm_status_updates_post_user_moderate();
  }
}

/**
 * Handle create status update
 */
function tm_status_updates_post_user_create($entity_type, $entity_id, $status_update) {

  global $user;

  // Get posting user
  $posting_user = $user;
 
  // Softfail if update too short
  if (strlen($status_update) < 3) {
    $ret = array("result" => false, "error_message" => "Update is too short.", "redirect" => null);
    drupal_json_output($ret);
    return;
  }

  // Softfail if update too long
  if (strlen($status_update) > 250) {
    $ret = array("result" => false, "error_message" => "Update is too long.", "redirect" => null);
    drupal_json_output($ret);
    return;
  }

  // Update status in database
  $invite = db_insert('tm_newsfeed_status_updates')
    ->fields(array(
      'entity_id' => $entity_id,
      'entity_type' => $entity_type, // user, chapter, organization
      'status_update' => trim($status_update),
      //'created' => 'NOW',
      //'latitude' => '',
      //'longitude' => '',
      'poster_uid' => $posting_user->uid,
      'poster_ip' => tm_geoip_get_real_visitor_ip()
    ))
    ->execute();

  // Flag user as having updated status
  $flag = flag_get_flag('status_updated_by_member');
  $flag->flag("unflag", $posting_user->uid); // unflag so we can flag again
  $flag->flag("flag", $posting_user->uid);

  // Updated successfully
  if ($entity_type == "user") {
    $redirect = "/newsfeed"; // redirect to user's news feed
  } else {
    $redirect = "/node/" . $entity_id; // redirect to entity page
  }
  $ret = array("result" => true, "error_message" => null, "redirect" => $redirect);
  drupal_json_output($ret);
  return;
}

/**
 * Helper method to parse post_as field
 * ie: uid-123, or nid-100
 * Returns array with entity_type and entity_id
 */
function tm_status_updates_parse_post_as($post_as) {

  $parts = explode("-", $post_as);
  if (sizeof($parts) < 0) { 
    return array("entity_type" => null, $entity_id => null);
  }

  // post as uid
  if ($parts[0] == "uid") {
    $entity_type = "user";
    $entity_id = intval($parts[1]);
  }

  // post as nid
  if ($parts[0] == "nid") {
    $entity_id = intval($parts[1]);
    $nid = intval($entity_id);
    $node = node_load($nid);
    if ($node == false) {
      return array("entity_type" => null, $entity_id => null);
    }
    $entity_type = null;
    if ($node->type == "chapter") {
      $entity_type = "chapter";
    }
    if ($node->type == "organization") {
      $entity_type = "organization";
    }
    if ($node->type == "event") {
      $entity_type = "event";
    }

    // this entity type can't post
    if ($entity_type == null) {
      return array("entity_type" => null, $entity_id => null);
    }
  }

  return array("entity_type" => $entity_type, "entity_id" => $entity_id);
}

