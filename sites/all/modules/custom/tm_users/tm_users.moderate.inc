<?php

// tm_users.moderate.inc - profile moderation methods

/**
 * Profile does not meet community criteria
 */
function tm_users_moderate_non_community_profile($user){

  global $conf;
  $approving_user = $GLOBALS['user'];

  // Permission check in addition to access arguments
  // Don't allow flagging of admin account
  if (((in_array('administrator', $user->roles)) 
    or (in_array('moderator', $user->roles))) 
    or (in_array('chapter leader', $user->roles))) {
    drupal_set_message("Unable to flag this account as non-community.");
    return;
  }

  // Send notification message to user that their account doesn't meet community guidelines
  // there doesn't seem to be a hook that can detect we are applying a new role, so do it here
  $approving_user = user_uid_optional_load();
  // set profile url (either friendly url or community url)
  $profile_url = drupal_get_path_alias("user/" . $user->uid);

  $moderator_first_name = $approving_user->field_user_first_name['und'][0]['value'];

  // get moderator message to send to member
  // add line breaks
  $moderator_message = trim($_GET["moderator_message"]);
  $moderator_message_safe = "";
  if ($moderator_message != "") {
      $moderator_message_safe = "<br><strong>Message from " . $moderator_first_name . ":</strong> " . check_plain($moderator_message) . "<br>";
      $moderator_message_safe = str_replace("__NL__", "<br>", $moderator_message_safe); 
  }

  // Set moderator's reply-to address if they have allowed in their notification settings
  $reply_to = null;
  $notif = new TMNotification("user_approval_allow_reply_to");
  if ($notif->isEnabled($approving_user->uid)) {
    $reply_to = $approving_user->mail;
  }

  $community_values_url = $conf["tm_community_values_url"];
  $membership_criteria_message = $conf["tm_notification_moderation_non_community_message"];

  tm_notifications_notify('user_approval_non_community', $user, array('approving-user' => $approving_user, 'user' => $user, '__PROFILE_URL__' => $profile_url, '__MODERATOR_FIRST_NAME__' => $moderator_first_name, '__MODERATOR_MESSAGE__' => $moderator_message_safe, '__COMMUNITY_VALUES_URL__' => $community_values_url, '__REPLY_TO__' => $reply_to, '__MODERATION_MEMBERSHIP_CRITERIA__' => $membership_criteria_message));

  // flag the user their profile does not meet community values
  flag('unflag', 'moderate_non_community_profile', $user->uid, $approving_user);
  flag('flag', 'moderate_non_community_profile', $user->uid, $approving_user); 

  // set the user to unapproved
  $role_name = 'approved user'; // The name of the role to remove.
  if ($role = user_role_load_by_name($role_name)) {
    user_multiple_role_edit(array($user->uid), 'remove_role', $role->rid);
  }

  // unflag requested approval (so they can re-request if they already did)
  flag('unflag', 'approval_requested_by_user', $user->uid, $user);

  // Notify user
  drupal_set_message("An email has been sent to " . $user->mail . " requesting they address the membership guidelines of the community.");

  drupal_goto(drupal_get_path_alias("user/$user->uid"));
}

/**
 * Report a profile
 */
function tm_users_moderate_report_member($reported_user) {

  global $conf;
  global $user;

  // Permission check in addition to access arguments
  // Don't allow flagging of admin account
  if (in_array('administrator', $reported_user->roles)) {
    drupal_set_message("Unable to report this member for moderation.");
    return;
  }

  // Reporting user
  $reporting_user = user_load($user->uid);
  $reporting_profile_url = drupal_get_path_alias("user/" . $reporting_user->uid);
	$reporting_full_name = $reporting_user->field_user_first_name[LANGUAGE_NONE][0]['safe_value'] . " " . $reporting_user->field_user_last_name[LANGUAGE_NONE][0]['safe_value'];
    $reporting_user = user_load($user->uid);
  $reporting_user_mail = $reporting_user->mail;

	// Reported user
  $reported_profile_url = drupal_get_path_alias("user/" . $reported_user->uid);
	$reported_full_name = $reported_user->field_user_first_name[LANGUAGE_NONE][0]['safe_value'] . " " . $reported_user->field_user_last_name[LANGUAGE_NONE][0]['safe_value'];

	// Report reason 
  $report_reason = trim($_GET["reason"]);
 	$report_reason_safe = check_plain($report_reason);

 	// Get report message to send to moderator
  // add line breaks
  $report_message = trim($_GET["report_message"]);
  $report_message_safe = "";
  if ($report_message != "") {
      $report_message_safe .= "<br><strong>Additional information:</strong> " . check_plain($report_message) . "<br>";
      $report_message_safe = str_replace("__NL__", "<br>", $report_message_safe); 
  }

  // Set reporter's reply-to address
  $reply_to = $reporting_user->mail;

  // Community values url
  $community_values_url = $conf["tm_community_values_url"];

  // Notify all moderators
  $moderator_ids = tm_users_get_all_moderators();
  foreach ($moderator_ids as $moderator_id) {

      // load the user to notify
      $moderator_user = user_load($moderator_id);

      tm_notifications_notify('user_report_member', $moderator_user, array('reporting_user' => $reported_user, 'reported_user' => $reported_user, '__REPORTING_PROFILE_URL__' => $reporting_profile_url, '__REPORTED_PROFILE_URL__' => $reported_profile_url, '__REPORT_REASON__' => $report_reason_safe, '__REPORT_MESSAGE__' => $report_message_safe, '__COMMUNITY_VALUES_URL__' => $community_values_url, '__REPLY_TO__' => $reply_to, '__REPORTING_FULL_NAME__' => $reporting_full_name, '__REPORTED_FULL_NAME__' => $reported_full_name, '__REPORTING_EMAIL__' => $reporting_user_mail));
  }

  // flag user
  flag('unflag', 'moderate_report_member', $reported_user->uid, $reporting_user); 
	flag('flag', 'moderate_report_member', $reported_user->uid, $reporting_user); 

  // Notify user
  drupal_set_message("Thanks for reporting an issue. Our moderation team will take a look as soon as posssible.");

  // Redirect to user's profile
  drupal_goto(drupal_get_path_alias("user/$reported_user->uid"));
}

/**
 * Resolve a profile report
 * Remove all report flags
 */
function tm_users_moderate_report_member_resolved($reported_user) {

  // Remove all report flags
  $report_flag = flag_get_flag('moderate_report_member', NULL);
  flag_reset_flag($report_flag, $reported_user->uid);

  // Notify user
  drupal_set_message("This account is no longer flagged for report.");

  // Redirect to user's profile
  drupal_goto(drupal_get_path_alias("user/$reported_user->uid"));
}

/**
 * Access callback for reporting a member
 */
function tm_users_moderate_report_member_check() {

	global $user;

	// Only allow approved members to flag
  if (!in_array('approved user', $user->roles) and !in_array('administrator', $user->roles)) {
  	return false;
  }

  // ok
  return true;
}

/**
 * Access callback for resolving reporting a member
 */
function tm_users_moderate_report_member_resolved_check() {

  global $user;

  // Only allow moderators to resolve
  if (!in_array('moderator', $user->roles) and !in_array('administrator', $user->roles)) {
    return false;
  }

  // ok
  return true;
}
