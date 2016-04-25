<?php

// tm_users.edit.inc - edit form methods

/**
 * Modify profile form
 */
function tm_users_form_user_profile_form_alter(&$form, &$form_state) {

  // Only edit form if user is not logged in
  // Needed for simple_pass_reset to work
  if (!user_is_logged_in()) { return; }

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

  // Add javascript protection to prompt to save profile if its edited
  // Use case is that many users don't click the save button on their profile
  drupal_add_js(drupal_get_path('module', 'tm_users') . '/js/user-edit-protection.js');

  // Allow users to cancel their accounts
  // Note: Chapter leaders and moderators don't get the link as they may have content
  // Todo: With subscription model, warn user of any active subscriptions
  // See: https://github.com/travelmassive/massiveplatform/issues/81
  global $user;
  if ((!in_array("chapter leader", $user->roles)) && (!in_array("moderator", $user->roles)) && (!in_array("administrator", $user->roles))) {

    $cancel_url = "/user/" . $user->uid . "/cancel";
    $form['cancel_account_link'] = array('#markup' => l(t('Remove account'), 'javascript:jq_confirm_cancel_account(' . $user->uid . ');', array('fragment' => '','external'=>true, 'attributes' => array('class' => array('cancel_account_link')))), '#weight' => 100);
    $form['#group_children']['cancel_account_link'] = 'group_user_account';
  }

  // Show additional link fields based on $conf['tm_users_link_fields']
  global $conf;
  $all_fields = array('website', 'twitter', 'facebook', 'linkedin', 'instagram', 'youtube', 'vimeo', 'snapchat');
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

  // hide reason for joining (only display on signup and requesting approval)
  hide($form['field_reason_for_joining']);

  // set site specific help text for friendly url
  global $conf;
  
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
  
}

