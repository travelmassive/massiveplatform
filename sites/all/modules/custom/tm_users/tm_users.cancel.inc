<?php

// tm_users.cancel.inc - cancel account methods

/**
 * Approval callback for cancelling account
 * Wrapper for tm_users_check_request_approval
 */
function tm_users_check_cancel_account() {
  return tm_users_check_request_approval();
}

/**
 * Prevent cancelling account if user is chapter leader, moderator or administrator
 */
function tm_users_user_cancel_methods_alter(&$methods) {
 
  global $user;
  $args = arg();
  if (sizeof($args) == 3) {
    if (($args[0] == "user") and ($args[2] == "cancel")) {

      $cancel_uid = $args[1];
      $cancel_user = user_load($cancel_uid);

      if (in_array("chapter leader", $cancel_user->roles) or in_array("moderator", $cancel_user->roles) or in_array("administrator", $cancel_user->roles)) {
        drupal_set_message("Sorry, you can't remove this user account.");
        drupal_goto("/user/" . $cancel_uid);
      }
    }
  }

  // Add a custom method
  // see: http://valuebound.com/resources/blog/custom-account-cancellation-methods-in-drupal-7
  $methods['tm_users_remove_user'] = array(
    'title' => t('TM - Delete the account and reassign chapter or event content to the <i
      >Anonymous</i> user.'),
    'description' => t('Chapters and events are kept'),
    // access should be used for administrative methods only.
    'access' => user_access('Administer permissions'),
  );

}

/**
 * User has confirmed they want to remove account
 * Notify administrator and also send on any feedback
 * Note, user is not 100% deleted yet, the user must confirm password as final step following this
 */
function tm_users_confirm_cancel_account($account) {

  global $user;

  // only fire this method if cancelling own account
  if ($account->uid != $user->uid) {
    drupal_goto("/");
    return;
  }

  global $conf;
  if (isset($conf['tm_user_remove_own_account_notify'])) {
    $notify_email = $conf['tm_user_remove_own_account_notify'];

    // feedback
    $feedback = "";
    if (isset($_GET["feedback"])) {
      $feedback = trim($_GET["feedback"]);
    }

    // notify administrator
    $first_name = $account->field_user_first_name['und'][0]['value'];
    $last_name = $account->field_user_last_name['und'][0]['value'];
    $profile_url = $conf["tm_email_server_http"] . "://" . $_SERVER['SERVER_NAME'] . "/" . drupal_get_path_alias("user/" . $account->uid);
    $profile_link = "<a href='" . $profile_url . "'>" . $profile_url . "</a>";

    $message = "The following member just cancelled their account:";
    $message .= "<br><br>Name: " . $first_name . " " . $last_name . "";
    $message .= "<br>Email address: " . $account->mail;
    $message .= "<br>Profile URL: " . $profile_url . " (will be deleted)";

    if ($feedback != "") {
      $feedback = strip_tags($feedback); // remove html
      $feedback = str_replace("\n", "<br>", $feedback); // add line breaks
      $message .= "<br><br><strong>Feedback</strong> " . $feedback;
    }
    $message .= "<br><br>No action is required, this is a notification to let you know.";

    // send the email
    tm_users_send_html_mail("tm_users", "remove_account_confirm", $notify_email, LANGUAGE_NONE, array(), "Member cancelled account notification", $message, NULL);
  }

  // redirect to the official cancel page
  drupal_goto("user/" . $account->uid . "/cancel");

}

/**
 * Safeguard any chapters or events owned by users if their account is removed
 * see: http://valuebound.com/resources/blog/custom-account-cancellation-methods-in-drupal-7
 */
function tm_base_user_cancel($edit, $account, $method) {

  if ($method == "tm_users_remove_user") {

    // Assign events to anon user
    module_load_include('inc', 'node', 'node.admin');
    $nodes = db_select('node', 'n')
      ->fields('n', array('nid'))
      ->condition('uid', $account->uid)
      ->condition('type', 'event')
      ->execute()
      ->fetchCol();
    node_mass_update($nodes, array('uid' => 0));

    // Assign chapters to anon user
    module_load_include('inc', 'node', 'node.admin');
    $nodes = db_select('node', 'n')
      ->fields('n', array('nid'))
      ->condition('uid', $account->uid)
      ->condition('type', 'chapter')
      ->execute()
      ->fetchCol();
    node_mass_update($nodes, array('uid' => 0));

    // Update message
    drupal_get_messages('status');

  }

}

