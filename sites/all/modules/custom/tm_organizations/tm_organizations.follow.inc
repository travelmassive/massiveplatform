<?php

// tm_organizations.follow.inc - organization follow methods

/**
 * Display follow message for organziations
 */
function tm_fields_organization_follow_message($field) {
  
  if (!user_is_logged_in()) {
    return;
  }

  if ($field['entity']->type == "organization") {

    $node_id = $field['entity']->nid;

    // always render following message 
    // it is hidden on page load and displayed in tm_organizations_preprocess_flag
    $company = node_load($node_id);
    if (isset($company->field_follower_message[LANGUAGE_NONE][0]['value'])) {
      $message = $company->field_follower_message[LANGUAGE_NONE][0]['value'];
      $message = strip_tags(trim($message));
      if ($message != "") {
        return "You are following " . t($company->title) . "<p class='ds-organization-follow-message-insert'>" . $message . "</p>";
      }
    } else {
      return "You are following " . t($company->title);
    }
  }

}

/**
 * Preprocess organization flags
 */
function tm_organizations_preprocess_flag(&$variables) {

  // modify flag to send organization message
  $send_message = _tm_organizations_preprocess_follow_flag($variables);

  // modify flag if message modification not set
  if (!$send_message) {
    _tm_organizations_preprocess_follow_flag_show_messager($variables);
  }

}

/** 
 * Modify flag to show follow message on follow
 */
function _tm_organizations_preprocess_follow_flag_show_messager(&$variables) {

  if (!user_is_logged_in()) {
    return;
  }

  // operate on follow_organizations
  $flag = &$variables['flag'];
  $entity_id = $variables['entity_id'];
  if ($flag->name != "follow_organizations") {
    return;
  }

  // if already following
  // show the .tm-ds-organization-follow-message field
  if ($variables['last_action'] == "flagged") {

    drupal_add_js('
    jQuery(document).ready(function($) {

      // show message
      $( ".tm-ds-organization-follow-message").show();

      // hide when unfollowed
      $( ".flag-follow-organizations .unflag-action").click(function() {
        setTimeout(function() {
          $( ".tm-ds-organization-follow-message" ).slideUp();
        }, 500); 
      }); 

    });
    ', 'inline');
    return;
  }

  // hide follow message when unfollowing
  if ($variables['last_action'] == "unflagged") {

    drupal_add_js('
    jQuery(document).ready(function($) {

      // show when followed
      $( ".flag-follow-organizations .flag-action").click(function() {
        setTimeout(function() {
          $( ".tm-ds-organization-follow-message" ).slideDown();
        }, 500); 
      }); 

    });
    ', 'inline');
  }

}

/**
 * Modify follow flag
 * Allow short message to be sent to person followed
 * Must be approved member and not followed previously
 * Return true if we operated on flag, false otherwise
 */
function _tm_organizations_preprocess_follow_flag(&$variables) {

  global $conf;
  global $user;

  // Check feature is turned on
  if (!$conf["tm_following_enable_message_organization"]) {
    return false;
  }

  // Shortcuts
  $flag = $variables['flag'];
  $status = $variables['status'];
  $entity_id = $variables['entity_id'];

  // Check we are logged in
  if (!user_is_logged_in()) { 
    return false;
  }

  // We only want to operate on the flag displayed on the current organization page
  // Not any flags displayed in related company lists, etc
  if (!(($flag->entity_type == 'node') && (arg(1) == $entity_id))) {
    return false;
  }
 
  // Follow organization flag
  if (($flag->name == "follow_organizations") && ($status == "unflagged")) {
   
    // Check we haven't already followed before with follow_organizations_log
    // If we have, user will be able to follow again but there won't be a message
    $flag_log = flag_get_flag('follow_organizations_log', NULL);
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
      $( ".flag-follow-organizations a.flag" ).unbind("click");
      $( ".flag-follow-organizations a.flag" ).bind("click.confirm", function(event) {
        jq_follow_message_organization(' . $entity_id . ');
        return false;
      });
    });
    ', 'inline');

    return true;
  }

  return false;

}

/** 
 * Follow organization and send message
 */
function tm_organizations_follow_message($following_organization) {

  global $conf;
  global $user;

  $flag = flag_get_flag('follow_organizations', NULL);
  $flag_log = flag_get_flag('follow_organizations_log', NULL);

  // STEP 1. VALIDATION

  // feature turned on
   // Check feature is turned on
  if (!$conf["tm_following_enable_message_organization"]) {
    drupal_set_message("Sorry this feature is not enabled", 'warning');
    drupal_goto(drupal_get_path_alias('node/' . $following_organization->nid));
    return;
  }

  // Must be approved user
  if (!in_array("approved user", $user->roles)) {
    drupal_set_message("Oops, you must be an approved member to perform this action", 'warning');
    drupal_goto(drupal_get_path_alias('node/' . $following_organization->nid));
    return;
  }

  // Check you aren't already following them
  $you_are_following_them = ($flag->is_flagged($following_organization->nid, $user->uid));
  if ($you_are_following_them) {
    drupal_set_message("Oops, you are already following this member", 'warning');
    drupal_goto(drupal_get_path_alias('node/' . $following_organization->nid));
    return;
  }

  // Check you haven't already followed them before (via follow_organizations_log)
  $you_have_followed_them_before = ($flag_log->is_flagged($following_organization->nid, $user->uid));
  if ($you_have_followed_them_before) {
    drupal_set_message("Oops, you have previously followed this organization, message not allowed", 'warning');
    drupal_goto(drupal_get_path_alias('node/' . $following_organization->nid));
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
    drupal_goto(drupal_get_path_alias('node/' . $following_organization->nid));
    return;
  }

  // If message too long
  if (strlen($send_message_safe) > 200) {
    drupal_set_message("Oops, follow message is too long", 'warning');
    drupal_goto(drupal_get_path_alias('node/' . $following_organization->nid));
    return;
  }

  // STEP 3. SET STATIC VARIABLE FOR NOTIFICATIONS HOOK
  // Set variable
  $hook_static_var_message = &drupal_static("tm_notifications_new_follower_organization_message");
  $hook_static_var_message = $send_message_safe;

  // STEP 4. SET FOLLOW LOG FLAG
  $flag->flag('flag', $following_organization->nid, $user, TRUE);
  $flag_log->flag('flag', $following_organization->nid, $user, TRUE);

  // STEP 5. RETURN USER TO COMPANY PROFILE
  // Note: tm_notifications_new_follower_flag_flag will occur and send recipient a message
  drupal_set_message("You are now following " . t($following_organization->title));
  drupal_goto(drupal_get_path_alias('node/' . $following_organization->nid));

}
