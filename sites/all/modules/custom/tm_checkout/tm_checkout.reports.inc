<?php

// tm_checkout.reports.inc - payment report methods

/**
 * Permission callback. Check if user can access subscription data
 */
function tm_checkout_access_payment_report_check() {

  global $user;

  // load account
  $uid = arg(2);
  $account = user_load($uid);
  if ($account == null) {
    return;
  }

  // Allow moderator or administrators to view payments
  if (in_array("administrator", $user->roles) or in_array("moderator", $user->roles)) {
    return true;
  }

  // Allow own user user to view payments
  return ($account->uid == $user->uid);
}

/**
 * Payment reports for event
 */
function tm_checkout_reports_user_payments($account = null) {

  global $conf;
  global $user;

  // Check user is logged in
  if (!user_is_logged_in()) {
    drupal_set_message("Please <a href='/user/login?destination=/checkout/my-payments" . $account->uid . "'>sign in</a> to view payment details.");
    drupal_goto("user/" . $account->uid);
    return;
  }

  // default to own account
  if ($account == null) {
    $account = user_load($user->uid);
  }
  
  // Check if has permission to view payments
  // Must be administrator, moderator or own user
  if (!(in_array("administrator", $user->roles) or (in_array("moderator", $user->roles)))
   and ($account->uid != $user->uid)) {
    drupal_set_message("You don't have permission to view payment details for this account.");
    drupal_goto("user/" . $account->uid);
    return;
  }

  // Add javascript to form
  drupal_add_js(drupal_get_path('module', 'tm_checkout') . '/js/tm_checkout_reports.js');
  $account_link = l(t($account->realname), drupal_get_path_alias("user/" . $account->uid));
  drupal_add_js(array('tm_checkout_reports' => array('form_subtitle' => $account_link)), array('type' => 'setting'));
  drupal_add_js(array('tm_checkout_reports' => array('form_heading' => 'Your Payments')), array('type' => 'setting'));

  $form = array();

  // get event sales from payment processor callback
  $results = tm_checkout_get_user_payments($account->uid);

  // Help message
  $help_message = $conf["tm_checkout_reports_help_message"];
  drupal_get_messages('help-tip');
  drupal_set_message($help_message, 'help-tip');

  // Start report
  $html = "<div id='tm-subscriptions-report'>";

  // Render account details
  // field_user_account_receipt
  if (isset($account->field_user_account_receipt[LANGUAGE_NONE][0]['safe_value'])) {
    $html .= "<p style='font-weight: bold;'>";
    $html .= str_replace("\n", "<br>", strip_tags($account->field_user_account_receipt[LANGUAGE_NONE][0]['safe_value']));
    $html .= "<span class='tm-subscriptions-report-edit'><br><a href='/user/" . $account->uid . "/edit#receipt-details'>Edit</a></span>";
    $html .= "</p>";
  } else {
    $html .= "<p class='tm-subscriptions-report-add'><a href='/user/" . $account->uid . "/edit#receipt-details'>Add your company details to this receipt</a></p>";
  }

  // field_user_account_contact
  if (isset($account->field_user_account_contact[LANGUAGE_NONE][0]['safe_value'])) {
    $html .= "Contact: " . strip_tags($account->field_user_account_contact[LANGUAGE_NONE][0]['safe_value']);
  }

  // Get payments
  $payments = $results;

  // Render payments table
  $paid_by_th = "";
  if ($account->uid != $user->uid) {
    $paid_by_th = "<th>Paid By</th>";
  }
  $html .= "<p><table class='tm-subscriptions-report-table'>";
  $html .= "<tr><th>Payment</th><th>Item</th>" . $paid_by_th . "<th class='tm-payments-report-extra'>Payment Date</th><th class='tm-payments-report-extra'>Payment ID</th></tr>";

  // No payments
  if (sizeof($payments) == 0) {
  	$html .= "<tr><td colspan='5'><p style='font-size: larger;'>We haven't received any payments.</p></td></tr>";
  }

  // Payment Details
  foreach($payments as $payment) {

    // checkout action
    $checkout_action = $payment->checkout_action;

    // is refunded
    $is_refunded = ($payment->refunded == 1);

    // item labels
    $item_label = "Payment";
    if ($checkout_action == "event_payment") {
      $item_label = "<a href='/" . drupal_get_path_alias("node/" . $payment->entity_id) . "'>Event Ticket: " . strip_tags($payment->entity_title) . "</a>";
    }

    if ($checkout_action == "upgrade_subscription") {
      $item_label = tm_subscriptions_get_user_subscription_field($account, "subscription_label");
    }

    if ($checkout_action == "renew_subscription") {
      $item_label = "Renew " . tm_subscriptions_get_user_subscription_field($account, "subscription_label");
    }

    $html .=  "<tr>";
    $html .=  "<td>";
    if ($is_refunded) {
      $html .= "<span class='tm-payments-report-status refunded' style='margin-right: 8px;'>Refunded</span>";
    }
    $html .= ($payment->display_amount + 0) . " " . strtoupper($payment->stripe_currency);
    $html .= "</td>";
    $html .=  "<td>" . $item_label . "</td>";
    if ($account->uid != $user->uid) { 
      $user_profile_url = drupal_get_path_alias("user/" . $payment->user_id);
      $html .=  "<td><a target='_blank' href='/" . $user_profile_url . "'>" . $payment->user_first_name . " " . $payment->user_last_name . "</a></td>";
    }
    $html .=  "<td class='tm-payments-report-extra'>" . date("Y-m-d", $payment->timestamp) . "</td>";
    $html .=  "<td class='tm-payments-report-extra'>" . $payment->payment_id . "</a></td>";
    $html .=  "</tr>";
  }

  $html .=  "</table></p>";

  if (sizeof($payments) > 0) {

	  // Payment Totals
	  $html .= "<p>";
    if (sizeof($payments) > 0) {

      // pluralize
      if (sizeof($payments) == 1) {
        $html .= "1 payment<br>";
      } else {
        $html .= (sizeof($payments) . " payments<br>");
      }     
    }

    // Our header
    $html .= "<hr style='border: none; border-top: 1px solid #ddd;'>" . $conf["tm_checkout_receipt_details"];
	  
	}

  $html .= "</div>";

  $form['event_sales'] = array('#markup' => $html);

  return $form;

}

