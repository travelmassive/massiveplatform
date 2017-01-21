<?php

// tm_users.duplicates.inc - handle duplicate users methods

// Notes:
// Add indexes to support duplicate user search
// ALTER TABLE field_data_field_user_first_name ADD INDEX idx_first_name_value (`field_user_first_name_value`);
// ALTER TABLE field_data_field_user_last_name ADD INDEX idx_last_name_value (`field_user_last_name_value`);

/**
 * Check for duplicate accounts with same first and last name
 */
function tm_users_check_duplicate_account() {

  global $user;
  global $conf;

  // only display for logged in members
  if (!user_is_logged_in()) {
    return;
  }

  // check if enabled in settings
  if (isset($conf["tm_users_check_duplicate_accounts"])) {
    if ($conf["tm_users_check_duplicate_accounts"] == false) {
      return;
    }
  }

  // don't check if user dismissed message
  if (isset($_COOKIE['Drupal_visitor_dismiss_duplicate_account_message'])) {
    if ($_COOKIE['Drupal_visitor_dismiss_duplicate_account_message'] == true) {
      return;
    }
  }

  // find possible duplicate accounts
  $possible_duplicates = tm_users_get_duplicate_accounts($user->uid);

  // don't show on contact page
  $current_path = current_path();
  if ($current_path == "contact") { 
    return;
  }

  // sort out older duplicate accounts
  $older_duplicates = array();
  foreach($possible_duplicates as $possible_duplicate) {
    if ($possible_duplicate < $user->uid) {
      $older_duplicates[] = $possible_duplicate;
    }
  }

  // if older duplicate accounts
  if (sizeof($older_duplicates) > 0) {

    $duplicate_account = user_load($older_duplicates[0]);
    $first_name = strip_tags($duplicate_account->field_user_first_name[LANGUAGE_NONE][0]['value']);
    $last_name = strip_tags($duplicate_account->field_user_last_name[LANGUAGE_NONE][0]['value']);

    $duplicate_user_url = drupal_get_path_alias("user/" . $duplicate_account->uid);

    // check to see if we are viewing the duplicate account
    $viewing_duplicate_acccount = false;
    $parts = explode("/", $current_path);
    if (sizeof($parts) == 2) {
      if (($parts[0] == "user") and ($parts[1] == $duplicate_account->uid)) {
        $viewing_duplicate_acccount = true;
      }
    }

    // create message
    if ($viewing_duplicate_acccount) {
      $message = "If this account belongs to you, please <a href='/contact'>contact our support team</a> for help. &nbsp;<a href='javascript:tm_hide_duplicate_account_message();' class='hide_duplicate_account_link' style='color: #888;'>Hide this message</a>";
    } else {
      $message = "We found an existing account with your name <a href='/" . $duplicate_user_url . "'>" . $first_name . " " . $last_name . "</a>. Is this you? &nbsp;<a href='javascript:tm_hide_duplicate_account_message();' class='hide_duplicate_account_link' style='color: #888;'>Hide this message</a>";
    }

    drupal_set_message($message, "page_notice");
  } else {

    // dismiss cookie, don't search again
    // cookie will be destroyed after browser sessions, so next session will perform a search again
    setcookie("Drupal.visitor.dismiss_duplicate_account_message", 1);

  }
}

/**
 * Return an array of uids that have same first and last name
 */
function tm_users_get_duplicate_accounts($uid) {

  // check account
  $account = user_load($uid);
  if ($account == null) {
    return array();
  }

  // get first and last name
  $first_name = $account->field_user_first_name[LANGUAGE_NONE][0]['value'];
  $last_name = $account->field_user_last_name[LANGUAGE_NONE][0]['value'];

  // find matching users with same first and last name
  $sql_query = "SELECT DISTINCT(fn.entity_id) as uid FROM field_data_field_user_first_name fn LEFT JOIN field_data_field_user_last_name ln ON fn.entity_type = 'user' AND ln.entity_type = 'user' AND fn.entity_id = ln.entity_id WHERE fn.field_user_first_name_value = :first_name AND ln.field_user_last_name_value = :last_name AND fn.entity_id != :uid";

  $rows = db_query($sql_query, array(':first_name' => $first_name, ':last_name' => $last_name, ":uid" => $uid))->fetchAll();

  $uids = array();
  foreach ($rows as $row) {
    $uids[] = $row->uid;
  }

  return $uids;
}
