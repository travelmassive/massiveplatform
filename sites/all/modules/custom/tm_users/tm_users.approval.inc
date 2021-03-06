<?php

// tm_users.approval.inc - approval methods

/**
 * Approval callback
 */
function tm_users_approve($user) {

  global $conf;
  $approving_user = $GLOBALS['user'];

  // apply approved role  
  $approved_role = user_role_load_by_name("approved user");
  user_multiple_role_edit(array($user->uid), 'add_role', $approved_role->rid);

  // verify email
  if (isset($conf["tm_users_approval_verify_email"])) {
    if ($conf["tm_users_approval_verify_email"]) {
      $not_verified_role = user_role_load_by_name("non-validated");
      user_multiple_role_edit(array($user->uid), 'remove_role', $not_verified_role->rid);
    }
  }
  
  // send notification message to user that they have been approved
  // there doesn't seem to be a hook that can detect we are applying a new role, so do it here
  $approving_user = user_uid_optional_load();
  // set profile url (either friendly url or community url)
  $profile_url = drupal_get_path_alias("user/" . $user->uid);
  $profile_uid = $user->uid;

  $approval_text = "Your account has been verified by " . $approving_user->field_user_first_name[LANGUAGE_NONE][0]['value'] . ".";

  $moderator_first_name = $approving_user->field_user_first_name['und'][0]['value'];

  // get moderator message to send to member
  // add line breaks
  $moderator_message = trim($_GET["moderator_message"]);
  $moderator_message_safe = "";
  if ($moderator_message != "") {
      $moderator_message_safe = "<br><br><strong>Message from " . $moderator_first_name . ":</strong> " . check_plain($moderator_message);
      $moderator_message_safe = str_replace("__NL__", "<br>", $moderator_message_safe);
      $approval_text .= $moderator_message_safe;
  }

  // Set moderator's reply-to address if they have allowed in their notification settings
  $reply_to = null;
  $notif = new TMNotification("user_approval_allow_reply_to");
  if ($notif->isEnabled($approving_user->uid)) {
    $reply_to = $approving_user->mail;
  }

  // hero headline
  $hero_headline = tm_notifications_render_hero_headline_html("Your account is verified 🎉");

  // generate member html
  $member_widget = tm_notifications_render_member_html($user);

  // subject line
  $subject_line = "Your " . $conf["tm_site_name"] . " account has been verified";

  $approval_welcome_message = $conf["tm_notification_approval_welcome_message"];
  tm_notifications_notify('user_approved', $user, array('approving-user' => $approving_user, 'user' => $user, '__PROFILE_URL__' => $profile_url, '__PROFILE_UID__' => $profile_uid, '__APPROVAL_INTRO_TEXT__' => $approval_text, '__REPLY_TO__' => $reply_to, '__APPROVAL_WELCOME_MESSAGE__' => $approval_welcome_message, '__HERO_HEADLINE__' => $hero_headline, "__MEMBER_WIDGET__" => $member_widget, '__SUBJECT_LINE__' => $subject_line));

  // Remove any moderation flags the user has

  // Unflag approval_is_company_or_brand
  $who_flagged = flag_get_entity_flags("user", $user->uid, "approval_is_company_or_brand");
  foreach ($who_flagged as $flagger) {
    $moderator = user_load($flagger->uid); 
    flag('unflag', 'approval_is_company_or_brand', $user->uid, $moderator);
  }

  // Unflag approval_need_more_info
  $who_flagged = flag_get_entity_flags("user", $user->uid, "approval_need_more_info");
  foreach ($who_flagged as $flagger) {
    $moderator = user_load($flagger->uid); 
    flag('unflag', 'approval_need_more_info', $user->uid, $moderator);
  }

  // Unflag moderate_non_community_profile
  $who_flagged = flag_get_entity_flags("user", $user->uid, "moderate_non_community_profile");
  foreach ($who_flagged as $flagger) {
    $moderator = user_load($flagger->uid); 
    flag('unflag', 'moderate_non_community_profile', $user->uid, $moderator);
  }

  // Remove approval request flag
  flag('unflag', 'approval_requested_by_user', $user->uid, $user);

  // Remove all report flags
  $report_flag = flag_get_flag('moderate_report_member', NULL);
  flag_reset_flag($report_flag, $user->uid);

  // Keep track of who approved with approval_approved_by flag
  flag('flag', 'approval_approved_by', $user->uid, $approving_user);

  // Give approver feedback and link to guest members pending
  // You have approved John Smith. There are 3 Guest Members pending approval in Sydney Travel Massive.
  $unapproved_members_html = "";
  if (sizeof($user->field_home_chapter) > 0) {
      $chapter = node_load($user->field_home_chapter[LANGUAGE_NONE][0]['target_id']);
      $chapter_url = url('node/' . $chapter->nid, array("absolute" => true));
      $unapproved_members_url = $chapter_url . "/unapproved-members";

      $membership_counts = _tm_chapters_get_membership_counts($chapter->nid);
      $members_unapproved_total = $membership_counts["members_unapproved_total"];
      if ($membership_counts["members_unapproved_total"] > 0) {

        if ($membership_counts["members_unapproved_total"] == 1) {
          $unapproved_members_html = "Please review <a href='" . $unapproved_members_url . "'>1 Guest Member</a> in " . tm_chapters_formal_name($chapter) . ".";
        } else {
          $unapproved_members_html = "Please review <a href='" . $unapproved_members_url . "'>" . $members_unapproved_total . " Guest Members</a> in " . tm_chapters_formal_name($chapter) . ".";
        }
       
      }
  }
  $approved_name = $user->field_user_first_name[LANGUAGE_NONE][0]['safe_value'] . " " . $user->field_user_last_name[LANGUAGE_NONE][0]['safe_value'];
  $message = "You have verified " . $approved_name . ". " . $unapproved_members_html;
  drupal_set_message($message);

  drupal_goto(drupal_get_path_alias("user/$user->uid"));
}

/**
 * Un-approval callback 
 */
function tm_users_unapprove($user){
  
  $role_name = 'approved user'; // The name of the role to remove.
  if ($role = user_role_load_by_name($role_name)) {
    user_multiple_role_edit(array($user->uid), 'remove_role', $role->rid);
  }

  drupal_goto(drupal_get_path_alias("user/$user->uid"));
}

/**
 * Approval callback for requesting approval
 */
function tm_users_check_request_approval() {

  // check the user is requesting approval for their own account
  $approval_user = arg(1);
  $logged_in_user = user_uid_optional_load();
  if (($approval_user != $logged_in_user->uid)) {
    return false;
  }

  // ok
  return true;
}

/**
 * Request approval
 */
function tm_users_request_approval($user) {

  // set profile url (either friendly url or community url)
  $profile_url = drupal_get_path_alias("user/" . $user->uid);
  $profile_uid = $user->uid;

  // get reason for approving and store update user object
  $reason_for_joining = trim($_GET["reason_for_joining"]);
  $user->field_reason_for_joining[LANGUAGE_NONE][0]['value'] = $reason_for_joining;
  field_attach_update('user', $user);

  // construct a list of moderators to notify
  $moderator_ids = array();

  // Get list of people who flagged this user as needs more info
  $who_flagged = flag_get_entity_flags("user", $user->uid, "approval_need_more_info");
  foreach ($who_flagged as $flagger) {
    $moderator_ids[] = $flagger->uid;
  }

  // Get list of people who flagged this user as a company or brand
  $who_flagged = flag_get_entity_flags("user", $user->uid, "approval_is_company_or_brand");
  foreach ($who_flagged as $flagger) {
    $moderator_ids[] = $flagger->uid;
  }

  // Get list of people who flagged this as non-community
  $who_flagged = flag_get_entity_flags("user", $user->uid, "moderate_non_community_profile");
  foreach ($who_flagged as $flagger) {
    $moderator_ids[] = $flagger->uid;
  }

  // if user has not been flagged, fall back to their home chapter
  if (sizeof($moderator_ids) == 0) {

    if (sizeof($user->field_home_chapter) > 0) {

      // get home chapter
      $home_chapter_id = $user->field_home_chapter['und'][0]['target_id'];
      $home_chapter = node_load($home_chapter_id);
      if (isset($home_chapter->field_chapter_leaders['und'])) {
        foreach ($home_chapter->field_chapter_leaders['und'] as $leader) {
          $moderator_ids[] = $leader['target_id'];
        }
      }
    }
  }

  // if user has not joined a chapter, fall back to all moderators
  if (sizeof($moderator_ids) == 0) {

    $moderator_id = array_search('moderator', user_roles());
    $query = 'SELECT DISTINCT(ur.uid) 
      FROM {users_roles} AS ur
      WHERE ur.rid IN (:rids)';
    $result = db_query($query, array(':rids' => array($moderator_id)));

    $moderator_ids = $result->fetchCol();
  }

  // don't send multiple notifications
  $moderator_ids = array_unique($moderator_ids);

  // notify list of moderators
  $first_name = $user->field_user_first_name[LANGUAGE_NONE][0]['value'];
  $last_name = $user->field_user_last_name[LANGUAGE_NONE][0]['value'];

  // send notification
  $reason_for_joining_safe = strip_tags(check_plain($reason_for_joining));
  if ($reason_for_joining_safe == "") {
    $reason_for_joining_safe = "(not provided)";
  }

  // hero headline
  $hero_headline = tm_notifications_render_hero_headline_html("Verification request");

  // generate member html
  $member_widget = tm_notifications_render_member_html($user);

  foreach ($moderator_ids as $uid) {

      $notify_user = user_load($uid);
      tm_notifications_notify('user_requested_approval', $notify_user, array('user' => $notify_user, '__USER_FIRST_NAME__' => $first_name, '__USER_LAST_NAME__' => $last_name, '__PROFILE_URL__' => $profile_url, '__PROFILE_UID__' => $profile_uid, '__REASON_FOR_JOINING__' => $reason_for_joining_safe, '__HERO_HEADLINE__' => $hero_headline, '__MEMBER_WIDGET__' => $member_widget));
  }

  // flag user as having requested approval
  $flag = flag_get_flag('approval_requested_by_user');
  $flag->flag("flag", $user->uid);

  // Notify user
  drupal_set_message("Thanks for submitting your profile for verification. We'll take a look and notify you soon.");

  drupal_goto(drupal_get_path_alias("user/$user->uid"));

}

/**
 * Need more info callback
 */
function tm_users_approval_need_more_info($user){

  global $conf;
  $approving_user = $GLOBALS['user'];

  // send notification message to user that they need to provide more information
  // there doesn't seem to be a hook that can detect we are applying a new role, so do it here
  $approving_user = user_uid_optional_load();
  // set profile url (either friendly url or community url)
  $profile_url = drupal_get_path_alias("user/" . $user->uid);
  $profile_uid = $user->uid;

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

  // hero headline
  $hero_headline = tm_notifications_render_hero_headline_html("Please update your profile.");

  // generate member html
  $member_widget = tm_notifications_render_member_html($user);

  // default moderation tips
  $moderation_tips = 'Here\'s some tips to get verified:
<ul>
<li>Tell us about yourself in the "About me" section</li>
<li>Upload a profile picture so members can recognize you</li>
<li>Tell us what city you are based in</li>
<li>Fill out your Job Role and Organization Name</li>
<li>Update your Industry Segment</li>
<li>Fill out your profile questions</li>
<li>Provide links to your website, or social media accounts</li>
</ul>
<br><strong>I\'ve updated my account, what next?</strong>
<br>Tap "Request verification" from your account menu. Our team will review your profile and you\'ll be notified when you are verified.
<br>
<br>If you need any assistance please reply to this email.';

  // custom moderation tips
  if (isset($conf["tm_notification_moderation_more_info_tips"])) {
    $moderation_tips = $conf["tm_notification_moderation_more_info_tips"];
  }
  $moderation_tips = str_replace("__SERVER_NAME__", $_SERVER["SERVER_NAME"], $moderation_tips);
  $moderation_tips = str_replace("__SERVER_HTTP__", $conf["tm_email_server_http"], $moderation_tips);  

  tm_notifications_notify('user_approval_need_more_info', $user, array('approving-user' => $approving_user, 'user' => $user, '__PROFILE_URL__' => $profile_url, '__PROFILE_UID__' => $profile_uid, '__MODERATOR_FIRST_NAME__' => $moderator_first_name, '__MODERATOR_MESSAGE__' => $moderator_message_safe, '__REPLY_TO__' => $reply_to, '__HERO_HEADLINE__' => $hero_headline, '__MEMBER_WIDGET__' => $member_widget, '__MODERATION_TIPS__' => $moderation_tips));

  // unflag then flag again
  flag('unflag', 'approval_need_more_info', $user->uid, $approving_user);
  flag('flag', 'approval_need_more_info', $user->uid, $approving_user);

  // unflag requested approval (so they can re-request if they already did)
  flag('unflag', 'approval_requested_by_user', $user->uid, $user);

  // Notify user
  drupal_set_message("An email has been sent to " . $user->mail . " requesting they complete their profile information.");

  drupal_goto(drupal_get_path_alias("user/$user->uid"));
}

/**
 * Account is company or brand callback
 */
function tm_users_approval_is_company_or_brand($user){

  global $conf;
  $approving_user = $GLOBALS['user'];
  $role_name = 'approved user'; // The name of the role to add.

  // send notification message to user that they need to provide more information
  // there doesn't seem to be a hook that can detect we are applying a new role, so do it here
  $approving_user = user_uid_optional_load();
  // set profile url (either friendly url or community url)
  $profile_url = drupal_get_path_alias("user/" . $user->uid);
  $profile_uid = $user->uid;

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

  // hero headline
  $hero_headline = tm_notifications_render_hero_headline_html("Your profile needs to be about you (not your brand).");

  // generate member html
  $member_widget = tm_notifications_render_member_html($user);

  // default moderation tips
  $moderation_tips = '<strong>Once you\'ve personalized your account, we\'ll be able to verify you and you can add a <a href="__SERVER_HTTP__://__SERVER_NAME__/companies">company page</a>.</strong>
<br><br>Here\'s some tips to get verified:
<ul>
<li>Provide your real First and Last name if you haven\'t already</li>
<li>Tell us about yourself in the "About me" section</li>
<li>Upload a photo of yourself so members can recognize you</li>
<li>Tell us what city you are based in</li>
<li>Fill out your profile questions</li>
<li>Provide links to your personal LinkedIn or social media accounts</li>
</ul>
<br><strong>I\'ve updated my account, what next?</strong>
<br>Tap "Request verification" from your account menu. Our team will review your profile and you\'ll be notified when you are verified.
<br>
<br>If you need any assistance please reply to this email.';

  // custom moderation tips
  if (isset($conf["tm_notification_moderation_is_company_tips"])) {
    $moderation_tips = $conf["tm_notification_moderation_is_company_tips"];
  }
  $moderation_tips = str_replace("__SERVER_NAME__", $_SERVER["SERVER_NAME"], $moderation_tips);
  $moderation_tips = str_replace("__SERVER_HTTP__", $conf["tm_email_server_http"], $moderation_tips);  

  tm_notifications_notify('user_approval_is_company_or_brand', $user, array('approving-user' => $approving_user, 'user' => $user, '__PROFILE_URL__' => $profile_url, '__PROFILE_UID__' => $profile_uid, '__MODERATOR_FIRST_NAME__' => $moderator_first_name, '__MODERATOR_MESSAGE__' => $moderator_message_safe, '__REPLY_TO__' => $reply_to, '__HERO_HEADLINE__' => $hero_headline, '__MEMBER_WIDGET__' => $member_widget, '__MODERATION_TIPS__' => $moderation_tips));

  // flag the user that its a company or brand
  flag('unflag', 'approval_is_company_or_brand', $user->uid, $approving_user);
  flag('flag', 'approval_is_company_or_brand', $user->uid, $approving_user); 

  // unflag requested approval (so they can re-request if they already did)
  flag('unflag', 'approval_requested_by_user', $user->uid, $user);

  // Notify user
  drupal_set_message("An email has been sent to " . $user->mail . " requesting they personalize their account.");

  drupal_goto(drupal_get_path_alias("user/$user->uid"));
}

/**
 * Auto approve member
 * Called from tm_users_user_insert()
 */
function tm_users_approval_on_signup($account) {

  global $conf;

  // check setting exists
  if (!isset($conf["tm_users_approval_on_signup"])) {
    return;
  }

  // check setting enabled
  if ($conf["tm_users_approval_on_signup"] != true) {
    return;
  }
  
  // add approved role to account
  $role_name = 'approved user'; // The name of the role to add.
  $role = user_role_load_by_name($role_name);
  user_multiple_role_edit(array($account->uid), 'add_role', $role->rid);

  // flag member approved by admin
  $approving_user = user_load(1);
  flag('flag', 'approval_approved_by', $account->uid, $approving_user);

  // hero headline
  $headline_text = "Welcome.";
  if (isset($conf["tm_users_approval_on_signup_headline"])) {
    $headline_text = $conf["tm_users_approval_on_signup_headline"];
  }
  $hero_headline = tm_notifications_render_hero_headline_html($headline_text);
  
  // generate member html
  $member_widget = tm_notifications_render_member_html($account);

  // other variables
  $profile_url = drupal_get_path_alias("user/" . $account->uid);
  $profile_uid = $account->uid;
  $approval_text = "";

  // subject line
  $subject_line = "Welcome to " . $conf["tm_site_name"];
 if (isset($conf["tm_users_approval_on_signup_subject"])) {
    $subject_line = $conf["tm_users_approval_on_signup_subject"];
  }
  
  // send welcome message
  $reply_to = null;
  $approval_welcome_message = $conf["tm_notification_approval_welcome_message"];
  tm_notifications_notify('user_approved', $account, array('approving-user' => $approving_user, 'user' => $account, '__PROFILE_URL__' => $profile_url, '__PROFILE_UID__' => $profile_uid, '__APPROVAL_INTRO_TEXT__' => $approval_text, '__REPLY_TO__' => $reply_to, '__APPROVAL_WELCOME_MESSAGE__' => $approval_welcome_message, '__HERO_HEADLINE__' => $hero_headline, "__MEMBER_WIDGET__" => $member_widget, '__SUBJECT_LINE__' => $subject_line));

}
