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
 * Restrict the cancel methods available
 */
function tm_users_user_cancel_methods_alter(&$methods) {
 
  global $user;

  // Don't allow cancellation of chapter leader or moderators account
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

  // Improve description of methods
  $methods['user_cancel_reassign']['title'] = 'Delete the account, preserving any events or shared company pages created.';
  $methods['user_cancel_reassign']['description'] = 'Your account will be removed and all account information deleted.';

  // These methods are still destructive (ie: on flags) so don't allow these options
  unset($methods['user_cancel_block_unpublish']);
  unset($methods['user_cancel_block']);

  // The user_cancel_delete is potentially harmful, so restrict to administrator
  if (in_array("administrator", $user->roles)) {
    $methods['user_cancel_delete']['title'] = 'Delete the account and its content. (Warning: includes any chapters, events, or company pages created by the user.)';
  } else {
    unset($methods['user_cancel_delete']);
  }

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
    if (isset($_GET["member-feedback"])) {
      $feedback = trim($_GET["member-feedback"]);
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
 * Handle hook_user_cancel
 * Preserve shared company pages, re-assign approval flags, and re-assign files
 */
function tm_users_user_cancel($edit, $account, $method) {

  if ($method == "user_cancel_reassign") {

    // STEP 1. Handle company pages 
    // Reassign or delete company pages if member account is removed
    // Case 1. Multiple owners - re-assign company to other managers
    // Case 2. One owner - remove the company page.
    // (Chapters, events are assigned to anonymous user)

    // Get list of member's company
    $company_nids = tm_organizations_get_nodes_created($account->uid);
    foreach ($company_nids as $nid) {

      // Check if we reassign or not
      $reassigned = false;
      $company = node_load($nid);
      if (isset($company->field_owner[LANGUAGE_NONE])) {
        foreach ($company->field_owner[LANGUAGE_NONE] as $owner) {

          // re-assign to first other owner
          if ((!$reassigned) and ($owner['target_id'] != $account->uid)) {

            $reassigned = true;
            $company->uid = $owner['target_id'];

            $message = "Canceling user account: ownership of company page " . strip_tags($company->title) . " (nid: " . $company->nid . ") has been assigned from uid " . $company->uid . " to " . $owner['target_id'];
            watchdog('tm_users', $message, WATCHDOG_INFO);

          }
        }
      }

      // save company if reassigned
      if ($reassigned) {

        // remove user from owner list
        for ($i = 0; $i < sizeof($company->field_owner[LANGUAGE_NONE]); $i++) {
          if ($company->field_owner[LANGUAGE_NONE][$i]['target_id'] == $account->uid) {
            unset($company->field_owner[LANGUAGE_NONE][$i]);
          }
        }

        $tm_user_cancel_reassign_company = &drupal_static('tm_user_cancel_reassign_company');
        $tm_user_cancel_reassign_company = true;
        node_save($company);
        drupal_static_reset('tm_user_cancel_reassign_company');

      } else {
        node_delete($company->nid);
      }
    }

    // STEP 2. Handle members flags
    // Re-assign any approval flags to anonymous user
    // Then the flag modules's user_cancel method will operate
    $flag = flag_get_flag('approval_approved_by', NULL);
    $new_uid = 0; // Anonymous user
    $query = "UPDATE flagging SET uid = :new_uid WHERE fid = :fid AND uid = :old_uid";
    db_query($query, array(":new_uid" => $new_uid, ":old_uid" => $account->uid, ":fid" => $flag->fid));

    // STEP 3. Update files (ie: image uploads)
    // Re-assign any files to anonymous user
    $query = "UPDATE file_managed SET uid = :new_uid WHERE uid = :old_uid";
    db_query($query, array(":new_uid" => $new_uid, ":old_uid" => $account->uid));

    // Update message
    drupal_get_messages('status');

  }

}

/**
 * Implement module_implements_alter
 * Make tm_users operate first cancelling user account
 */
function tm_users_module_implements_alter(&$implementations, $hook) {

  if ($hook == 'user_cancel') {
    $group = $implementations['tm_users'];
    unset($implementations['tm_users']);
    $implementations = array_merge(array('tm_users' => $group), $implementations);
  }
}
