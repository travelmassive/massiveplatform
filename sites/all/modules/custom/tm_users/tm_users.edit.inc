<?php

// tm_users.edit.inc - edit form methods

/**
 * Modify profile form
 */
function tm_users_form_user_profile_form_alter(&$form, &$form_state, $form_id) {

  // Only edit form if user is not logged in
  // Needed for simple_pass_reset to work
  if (!user_is_logged_in()) { return; }

  // Don't allow moderators to edit other moderators or admin accounts
  global $conf;
  global $user;
  $user_id = $form['#user']->uid;

  // Check that moderator can edit account
  if (!tm_users_check_moderator_edit_permission($user, $form['#user'])) {
    drupal_set_message("Sorry, you can't modify a moderator account.", "warning");
    drupal_goto("user/" . $user_id);
    return;
  }

  // Add an indication of the username, in case users are not allowed to edit 
  // their usernames.
  if (!user_access('change own username')) {
    $form['account']['name']['#access'] = FALSE;
    $form['account']['name']['#disabled'] = TRUE;
    $form['account']['name']['#required'] = FALSE;
    unset($form['account']['name']['#description']);
  }

  // We don't want a collapsible fieldset nor description for the timezone.
  $form['timezone']['#type'] = 'fieldset';
  $form['timezone']['#collapsible'] = FALSE;
  unset($form['timezone']['#description']);

  // Redirect user to their profile page when they save profile
  // http://drupal.stackexchange.com/questions/64171/redirect-user-to-their-profile-page-after-savin
  $form['#submit'][] = 'tm_users_user_profile_form_submit';

  // Add JS
  $js_array = array(); 

  // Add javascript protection to prompt to save profile if its edited
  // Use case is that many users don't click the save button on their profile
  $js_array[] = drupal_get_path('module', 'tm_users') . '/js/user-edit-protection.js';

  // Add field add links ("+ Add link") 
  $js_array[] = drupal_get_path('module', 'tm_users') . '/js/user-edit-field-add-links.js';

  if (!(in_array("administrator", $user->roles))) {
    // Add javascript to hide email and password fields to improve useability
    $js_array[] = drupal_get_path('module', 'tm_users') . '/js/user-edit-email-password.js';
  }
   
  $form['#attached']['js'] = $js_array;


  $form['user_show_change_email'] = array('#markup' => l(t('Change Email'), 'javascript:jq_show_edit_change_email();', array('fragment' => '','external'=>true, 'attributes' => array('style' => 'margin-top: 1rem; margin-left: 0.5rem;', 'class' => array('user_edit_change_email', 'bttn bttn-tertiary bttn-m cancel')))));

  $form['user_show_change_password'] = array('#markup' => l(t('Change Password'), 'javascript:jq_show_edit_change_password();', array('fragment' => '','external'=>true, 'attributes' => array('class' => array('user_edit_change_password', 'bttn bttn-tertiary bttn-m cancel')))));


  // Allow users to cancel their accounts
  // Note: Chapter leaders and moderators don't get the link as they may have content
  // See: https://github.com/travelmassive/massiveplatform/issues/81
  if ((!in_array("chapter leader", $user->roles)) && (!in_array("moderator", $user->roles)) && (!in_array("administrator", $user->roles))) {

    $cancel_url = "/user/" . $user_id . "/cancel";
    $form['cancel_account_link'] = array('#markup' => l(t('Remove account'), 'javascript:jq_confirm_cancel_account(' . $user_id . ');', array('fragment' => '','external'=>true, 'attributes' => array('class' => array('cancel_account_link')))), '#weight' => 100);
    $form['#group_children']['cancel_account_link'] = 'group_user_account';
  }

  // Show additional link fields based on $conf['tm_users_link_fields']
  $all_fields = array('website', 'twitter', 'facebook', 'linkedin', 'instagram', 'youtube', 'vimeo', 'snapchat', 'strava', 'tiktok');
  if (isset($conf["tm_users_link_fields"])) {
    $show_link_fields = $conf["tm_users_link_fields"];
    foreach ($all_fields as $field) {
      if (!in_array($field, $show_link_fields)) {
        unset($form['field_link_' . $field]); // hide field
      }
    }
  }

  // hide cover video field if not enabled
  if (!$conf["tm_cover_videos_enabled"]) {
    unset($form['field_user_cover_video']);
  }

  // Disable autocomplete on mail
  // see: http://www.jeffgeerling.com/blogs/jeff-geerling/disabling-autocomplete-forms 
  $form['account']['mail']['#attributes']['autocomplete'] = 'off';

  // Disable autocomplete on confirm password fields
  // see: https://www.drupal.org/node/540340
  // Get default process function array:
  $element_info = element_info('password_confirm');
  $process = $element_info['#process'];
  $process[] = 'tm_users_process_password_confirm';
  $form['account']['pass']['#process'] = $process;

  // Hide last modified timestamp
  unset($form['field_last_modified_timestamp']);

  // Hide Facebook signin profile link
  unset($form['field_facebook_signin_link']);

  // Allow Moderator to edit facebook signin id
  if ((in_array("moderator", $user->roles)) or (in_array("administrator", $user->roles))) {

    // Hide if facebook signin not enabled
    if (isset($conf['tm_signin_facebook'])) {
      if (!$conf['tm_signin_facebook']) {
        unset($form['field_facebook_signin_id']);
      }
    }
    // disable autocomplete
    $form['field_facebook_signin_id']['#attributes']['autocomplete'] = 'off';
  } else {
    unset($form['field_facebook_signin_id']);
  }

  // Allow customisation of industry segement
  if (isset($conf['tm_segment_label']) and isset($conf['tm_segment_user_description'])) {
    $form['field_segment'][LANGUAGE_NONE][0]['tid']['#title'] = $conf['tm_segment_label'];
    $form['field_segment'][LANGUAGE_NONE][0]['tid']['#description'] = $conf['tm_segment_user_description'];
  }

  // Hide subscriptions fields if tm_subscriptions_user not enabled
  if (!module_exists("tm_subscriptions_user")) {
    unset($form['field_user_subscription_type']); // hide subscription type
    unset($form['field_user_subscription_date']); // hide subscription end date
    unset($form['field_user_account_phone']); // hide subscription type
    unset($form['field_user_account_receipt']); // hide subscription type
  }

  // Add feedback link
  if ($conf["tm_users_feedback_show_menu"]) {
    $review_description = "<h2>Tell us what you think</h2>Please take a moment to <a class='tm_nps_review_link' href='/feedback'>" . strtolower($conf["tm_users_feedback_label"]) . "</a>";
    $form['#groups']['group_user_feedback']->format_settings['instance_settings']['description'] = $review_description;
  } else {
    $form['#groups']['group_user_feedback']->format_settings['instance_settings']['description'] = "";
  }

  // Set testimonial description 
  if (isset($conf["tm_users_review_description"])) {
      $form['field_user_testimonial'][LANGUAGE_NONE][0]['value']['#description'] = $conf["tm_users_review_description"] . "<br><br>";
  } else {
    $testimonial_description = $form['field_user_testimonial'][LANGUAGE_NONE][0]['value']['#description'];
    $testimonial_description = str_replace("__site_name__", $conf["tm_site_name"], $testimonial_description);
    $form['field_user_testimonial'][LANGUAGE_NONE][0]['value']['#description'] = $testimonial_description;
  }

  // hide user review to guests and new members
  if ((!in_array("moderator", $user->roles)) and (!in_array("administrator", $user->roles))) {
    $user_review_min_age = 30; // 30 days
    if (isset($conf["tm_users_review_min_age"])) {
      $user_review_min_age = $conf["tm_users_review_min_age"];
    }
    $account_age_days = ((time() - $form['#user']->created) / (60 * 60 * 24));
    $is_approved = in_array("approved user", $form['#user']->roles);
    if (($account_age_days < $user_review_min_age) or (!$is_approved)) {
      field_group_hide_field_groups($form, array('group_user_feedback'));
    }
  }

  // hide review if member is reported
  if (tm_users_is_member_reported($user->uid)) {
    field_group_hide_field_groups($form, array('group_user_feedback'));
  }

  // optional: disable industry segment, job role, organization for non-moderators
  if ((!in_array("moderator", $user->roles)) and (!in_array("administrator", $user->roles))) {
    
    // industry segment
    if (isset($conf["tm_users_disable_industry_segment"])) {
      if ($conf["tm_users_disable_industry_segment"] == true) {
        unset($form["field_segment"]);
      }
    }

    // job role
    if (isset($conf["tm_users_disable_job_role"])) {
      if ($conf["tm_users_disable_job_role"] == true) {
        unset($form["field_job_role"]);
      }
    }

    // job organization
    if (isset($conf["tm_users_disable_job_organization"])) {
      if ($conf["tm_users_disable_job_organization"] == true) {
        unset($form["field_job_organization"]);
      }
    }

  }

  // Once verified, member can't change their name without support
  if (in_array("approved user", $user->roles)) {
    if ((!in_array("chapter leader", $user->roles)) && (!in_array("moderator", $user->roles)) && (!in_array("administrator", $user->roles))) {
      $form["field_user_first_name"][LANGUAGE_NONE]['#disabled'] = true;
      $form["field_user_first_name"][LANGUAGE_NONE][0]['value']['#description'] = "As your profile is <strong>verified</strong>, please <a href='/contact'>contact support</a> if you wish to change your name.";
      $form["field_user_last_name"][LANGUAGE_NONE]['#disabled'] = true;
      $form["field_user_last_name"][LANGUAGE_NONE][0]['value']['#description'] = "&nbsp;";
    }
  }

  // Default for add country
  $form['field_user_country'][LANGUAGE_NONE]['#options']['_none'] = " - Select your country -";

}

/**
 * Hook for password confirm
 * Set to not autocomplete by browser
 */
function tm_users_process_password_confirm($element) {
  // see: https://bugs.chromium.org/p/chromium/issues/detail?id=370363#c7
  $element['pass1']['#title'] = t('New Password');
  $element['pass2']['#title'] = t('Confirm New Password');
  $element['pass1']['#attributes']['autocomplete'] = 'new-password';
  $element['pass2']['#attributes']['autocomplete'] = 'new-password';
  return $element;
}

/**
 * Hook user_update
 */
function tm_users_user_update(&$edit, $account, $category) {

  // Check if user had edited profile and saved it
  // If so, set $_SESSION["tm_account_changed"] to true
  // This can be picked up in tm_users_account_menu.tpl.php for further actions

  global $user;

  // Make sure we are editing our own account
  if ($user->uid != $account->uid) {
    return;
  }

  // Compare new and original fields that start with "field_"
  $different_fields = array();
  foreach ($edit as $key => $value) {

    if (strpos($key, "field_") === 0) {

      // get new and original field values
      $new_value = _tm_users_get_field_value($value);
      $orig_value = _tm_users_get_field_value($account->original->$key);
      
      // check if fields are different
      if ($new_value != $orig_value) {
        $different_fields[] = $key;
      }

    }
  }  

  // Did we update the account?
  if (sizeof($different_fields) > 0) {
    $_SESSION['tm_account_changed'] = true;
  } else {
    $_SESSION['tm_account_changed'] = false;
  }

}

/**
 * Modify the user's profile form
 */
function _tm_users_modify_profile_form(&$form, $form_state, $form_id) {

  global $conf;

  // get current user
  $user = user_load($form['#user']->uid);

  // Modify the home chapter form
  // Filter drop down list to the list of chapters the user is in

  // fetch list of chapters the user has joined
  $flags = flag_get_user_flags("node", NULL, $user->uid, NULL, FALSE);
  $chapters_nids = array();
  $home_chapter_options = array("_none" => "- None -");
  foreach ($flags as $flag_name => $flag) {
    if ($flag_name == "signup") {
      foreach($flag as $f) {
        $chapters_nids[] = $f->entity_id;
        $chapter = node_load($f->entity_id);
        $home_chapter_options[$f->entity_id] = $chapter->title;
      }
    }
  }

  // alter home chapter form for the user
  // list only the chapters they have joined
  // if they have a home chapter then set it as default
  asort($home_chapter_options);
  $form['field_home_chapter']['und']['#options'] = $home_chapter_options;
  if (isset($user->field_home_chapter['und'][0]['target_id'])) {
    $home_chapter_id = $user->field_home_chapter['und'][0]['target_id'];
    $form['field_home_chapter']['und']['#default_value'] = array(0 => $home_chapter_id);
  }
  
  // if they don't have a home chapter, hide the field
  if (sizeof($chapters_nids) == 0) {
    hide($form['field_home_chapter']);
  }

  // hide loggintoboggan revalidate notice since we print it anyway
  hide($form['revalidate']);

  // set site specific help text for friendly url
  
  // set title of user profile questions from config
  $form['field_user_question_1'][LANGUAGE_NONE][0]['value']['#title'] = $conf["tm_field_user_question_1_title"];
  $form['field_user_question_2'][LANGUAGE_NONE][0]['value']['#title'] = $conf["tm_field_user_question_2_title"];
  $form['field_user_question_3'][LANGUAGE_NONE][0]['value']['#title'] = $conf["tm_field_user_question_3_title"];
  $form['field_user_question_4'][LANGUAGE_NONE][0]['value']['#title'] = $conf["tm_field_user_question_4_title"];
  $form['field_user_question_5'][LANGUAGE_NONE][0]['value']['#title'] = $conf["tm_field_user_question_5_title"];

  // job role description
  $form['field_job_role'][LANGUAGE_NONE][0]['value']['#description'] = $conf["tm_field_job_role_description"];

  // friendly url
  $form['field_friendly_url'][LANGUAGE_NONE][0]['value']['#title'] = $conf["tm_field_friendly_url_title"];

  $form['field_friendly_url'][LANGUAGE_NONE][0]['value']['#description'] = $conf["tm_field_friendly_url_description"];

  // Hide reason for joining if it wasn't provided when requesting approval
  unset($form['field_reason_for_joining'][LANGUAGE_NONE][0]['value']['#description']);
  if (sizeof($form['#user']->field_reason_for_joining) == 0) {
    hide($form['field_reason_for_joining']);
  }

  // Disable site chat if not approved or if tm_chat is not enabled
  if (!in_array("approved user", $user->roles) or (!module_exists("tm_chat"))) {
    hide($form['field_user_disable_chat']);
  }

  // If google_tag module exists, set disable analytics label and description
  if (module_exists("google_tag")) {

    if (isset($conf["tm_users_disable_analytics_label"])) {
      $form['field_user_disable_analytics'][LANGUAGE_NONE]['#title'] = $conf["tm_users_disable_analytics_label"];
    }

    if (isset($conf["tm_users_disable_analytics_description"])) {
      $form['field_user_disable_analytics'][LANGUAGE_NONE]['#description'] = $conf["tm_users_disable_analytics_description"];
    }

    if (isset($_SERVER['HTTP_DNT'])) {
      if ($_SERVER['HTTP_DNT'] == 1) {
        $form['field_user_disable_analytics'][LANGUAGE_NONE]['#default_value'] = true;
        $form['field_user_disable_analytics'][LANGUAGE_NONE]['#disabled'] = true;
      }
    }
    
  } else {
    hide($form['field_user_disable_analytics']);
  }
  
}

/**
 * Check if moderator can edit admin or other moderator's account
 * Call this in tm_user_presave() and tm_users_form_user_profile_form_alter()
 * $editor = the user editing the account
 * $account = the account we are editing
 */
function tm_users_check_moderator_edit_permission($editor, $account) {

  // Check for new user
  if (!isset($account->roles)) {
    return true;
  }

  $user_is_moderator = (in_array("moderator", $account->roles));
  $user_is_admin = (in_array("administrator", $account->roles));
  $editor_is_moderator = (in_array("moderator", $editor->roles));
  $editor_is_admin = (in_array("administrator", $editor->roles));

  if (($user_is_moderator or $user_is_admin) and ($editor->uid != $account->uid)) {
    if (!$editor_is_admin) {
      return false;
    }
  }

  // ok
  return true;
}

