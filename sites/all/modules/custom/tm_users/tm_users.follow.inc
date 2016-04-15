<?php

// tm_users.follow.inc - follow methods

/**
 * Modify user flags
 */
function tm_users_preprocess_flag(&$variables) {

  // Modify follow user flag
  _tm_users_preprocess_follow_flag($variables);

}

/**
 * Modify follow flag
 * Allow short message to be sent to person followed
 * Must be approved member and not followed previously
 */
function _tm_users_preprocess_follow_flag(&$variables) {

  global $conf;
  global $user;

  // Check feature is turned on
  if (!$conf["tm_following_enable_message_user"]) {
    return;
  }

  // Shortcuts
  $flag = $variables['flag'];
  $status = $variables['status'];
  $entity_id = $variables['entity_id'];

  // Check we are logged in
  if (!user_is_logged_in()) { 
    return;
  }

  // We only want to operate on the flag displayed on the current user page
  // Not any flags displayed in related user lists, etc
  if (!(($flag->entity_type == 'user') && (arg(1) == $entity_id))) {
    return;
  }
 
  // Follow members flag
  if (($flag->name == "follow_members") && ($status == "unflagged")) {
   
    // Check we haven't already followed before with follow_members_log
    // If we have, user will be able to follow again but there won't be a message
    $flag_log = flag_get_flag('follow_members_log', NULL);
    $you_have_followed_them_before = ($flag_log->is_flagged($entity_id, $user->uid));
    if ($you_have_followed_them_before) {
      return;
    }

    // Check user role
    if (!in_array("approved user", $user->roles)) {
      return;
    }

    // Enhance the follow button
    $variables['flag_button_class'] = 'flag-limit follow bttn bttn-secondary bttn-m';
    drupal_add_js('
    jQuery(document).ready(function($) {
      $( ".flag-follow-members a.flag" ).unbind("click");
      $( ".flag-follow-members a.flag" ).bind("click.confirm", function(event) {
        jq_follow_message_member(' . $entity_id . ');
        return false;
      });
    });
    ', 'inline');
    return true;

  }

}

/* 
 * Follow member and send message
 */
function tm_users_follow_message($following_user) {

  global $conf;
  global $user;

  $flag = flag_get_flag('follow_members', NULL);
  $flag_log = flag_get_flag('follow_members_log', NULL);

  // STEP 1. VALIDATION

  // feature turned on
   // Check feature is turned on
  if (!$conf["tm_following_enable_message_user"]) {
    drupal_set_message("Sorry this feature is not enabled", 'warning');
    drupal_goto(drupal_get_path_alias('user/' . $following_user->uid));
    return;
  }

  // can't follow yourself
  if ($following_user->uid == $user->uid) {
    drupal_set_message("Oops, you can\'t follow yourself", 'warning');
    drupal_goto(drupal_get_path_alias('user/' . $following_user->uid));
    return;
  }

  // Must be approved user
  if (!in_array("approved user", $user->roles)) {
    drupal_set_message("Oops, you must be an approved member to perform this action", 'warning');
    drupal_goto(drupal_get_path_alias('user/' . $following_user->uid));
    return;
  }

  // Check you aren't already following them
  $you_are_following_them = ($flag->is_flagged($following_user->uid, $user->uid));
  if ($you_are_following_them) {
    drupal_set_message("Oops, you are already following this member", 'warning');
    drupal_goto(drupal_get_path_alias('user/' . $following_user->uid));
    return;
  }

  // Check you haven't already followed them before (via follow_members_log)
  $you_have_followed_them_before = ($flag_log->is_flagged($following_user->uid, $user->uid));
  if ($you_have_followed_them_before) {
    drupal_set_message("Oops, you have previously followed this member, message not allowed", 'warning');
    drupal_goto(drupal_get_path_alias('user/' . $following_user->uid));
    return;
  }

  // STEP 2. VALIDATE MESSAGE

  // Get message
  $send_message = trim($_GET["follow_message"]);
  // Remove links for non-chapter leaders and non-moderators
  if (!in_array("chapter leader", $user->roles) and !in_array("moderator", $user->roles)) {
    $send_message = preg_replace('@((https?://)?([-\w]+\.[-\w\.]+)+\w(:\d+)?(/([-\w/_\.]*(\?\S+)?)?)*)@', '(link removed)', $send_message);
  }

  $send_message_safe = strip_tags($send_message); // remove html
  $send_message_safe = str_replace("__NL__", "<br>", $send_message_safe);
  $send_message_safe = str_replace("\n", "<br>", $send_message_safe); // add line breaks

  // If message too short
  if ((strlen($send_message_safe) < 10) && ($send_message != "")) {
    drupal_set_message("Oops, follow message is too short", 'warning');
    drupal_goto(drupal_get_path_alias('user/' . $following_user->uid));
    return;
  }

  // If message too long
  if (strlen($send_message_safe) > 200) {
    drupal_set_message("Oops, follow message is too long", 'warning');
    drupal_goto(drupal_get_path_alias('user/' . $following_user->uid));
    return;
  }

  // STEP 3. SET STATIC VARIABLE FOR NOTIFICATIONS HOOK
  // Set variable
  $hook_static_var_message = &drupal_static("tm_notifications_new_follower_user_message");
  $hook_static_var_message = $send_message_safe;

  // STEP 4. SET FOLLOW LOG FLAG
  $flag->flag('flag', $following_user->uid, $user, TRUE);
  $flag_log->flag('flag', $following_user->uid, $user, TRUE);

  // STEP 5. RETURN USER TO PROFILE
  // Note: tm_notifications_new_follower_flag_flag will occur and send recipient a message
  $first_name = strip_tags($following_user->field_user_first_name[LANGUAGE_NONE][0]['value']);
  $last_name = strip_tags($following_user->field_user_last_name[LANGUAGE_NONE][0]['value']);
  drupal_set_message("You are now following " . $first_name . " " . $last_name);
  drupal_goto(drupal_get_path_alias('user/' . $following_user->uid));

}

