<?php

// tm_organizations.moderate.inc - organization moderation methods

/**
 * Report an organization profile
 */
function tm_organizations_moderate_report_organization($reported_organization) {

  global $conf;
  global $user;

  // Reporting user
  $reporting_user = user_load($user->uid);
  $reporting_profile_url = drupal_get_path_alias("user/" . $reporting_user->uid);
  $reporting_full_name = $reporting_user->field_user_first_name[LANGUAGE_NONE][0]['safe_value'] . " " . $reporting_user->field_user_last_name[LANGUAGE_NONE][0]['safe_value'];
    $reporting_user = user_load($user->uid);
  $reporting_user_mail = $reporting_user->mail;

  // Reported organization
  $reported_profile_url = drupal_get_path_alias("node/" . $reported_organization->nid);
  $reported_organization_name = check_plain($reported_organization->title);
  $reported_profile_id = $reported_organization->nid;

  // Report reason 
  $report_reason = trim($_GET["reason"]);
  $report_reason_safe = check_plain($report_reason);

  // Get report message to send to moderator
  // add line breaks
  $report_message = trim($_GET["report_message"]);
  $report_message_safe = "";
  if ($report_message != "") {
      $report_message_safe .= "<br><strong>Additional information:</strong> " . check_plain($report_message);
      $report_message_safe = str_replace("__NL__", "<br>", $report_message_safe); 
  }

  // Set reporter's reply-to address
  $reply_to = $reporting_user->mail;

  // Community values url
  $community_values_url = $conf["tm_community_values_url"];

  // Hero headline
  $hero_headline = tm_notifications_render_hero_headline_html("Organization reported.");

  // Notify all moderators
  $moderator_ids = tm_users_get_all_moderators();
  foreach ($moderator_ids as $moderator_id) {

      // load the user to notify
      $moderator_user = user_load($moderator_id);

      tm_notifications_notify('user_report_organization', $moderator_user, array('reporting_user' => $reporting_user, 'reported_organization' => $reported_organization, '__REPORTING_PROFILE_URL__' => $reporting_profile_url, '__REPORTED_PROFILE_URL__' => $reported_profile_url, '__REPORTED_PROFILE_ID__' => $reported_profile_id, '__REPORT_REASON__' => $report_reason_safe, '__REPORT_MESSAGE__' => $report_message_safe, '__COMMUNITY_VALUES_URL__' => $community_values_url, '__REPLY_TO__' => $reply_to, '__REPORTING_FULL_NAME__' => $reporting_full_name, '__REPORTED_ORGANIZATION_NAME__' => $reported_organization_name, '__REPORTING_EMAIL__' => $reporting_user_mail, '__HERO_HEADLINE__' => $hero_headline, 'hide_branding_message' => true));
  }

  // flag user
  flag('unflag', 'moderate_report_organization', $reported_organization->nid, $reporting_user); 
  flag('flag', 'moderate_report_organization', $reported_organization->nid, $reporting_user); 

  // Notify user
  drupal_set_message("Thanks for reporting an issue. Our moderation team has been notified and will respond to you.");

  // Redirect to user's profile
  drupal_goto(drupal_get_path_alias("node/$reported_organization->nid"));
}

/**
 * Resolve an organization profile report
 * Remove all report flags
 */
function tm_organizations_moderate_report_organization_resolved($reported_organization) {

  // Remove all report flags
  $report_flag = flag_get_flag('moderate_report_organization', NULL);
  flag_reset_flag($report_flag, $reported_organization->nid);

  // Notify user
  drupal_set_message("This organization profile is no longer reported for moderation.");

  // Redirect to user's profile
  drupal_goto(drupal_get_path_alias("node/$reported_organization->nid"));
}

/**
 * Access callback for reporting an organization
 */
function tm_organizations_moderate_report_check() {

  global $user;

  // Only allow approved members to flag
  if (!in_array('approved user', $user->roles) and !in_array('administrator', $user->roles)) {
    return false;
  }

  // ok
  return true;
}

/**
 * Access callback for resolving reporting an organization
 */
function tm_organizations_moderate_report_resolved_check() {

  global $user;

  // Only allow moderators to resolve
  if (!in_array('moderator', $user->roles) and !in_array('administrator', $user->roles)) {
    return false;
  }

  // ok
  return true;
}
