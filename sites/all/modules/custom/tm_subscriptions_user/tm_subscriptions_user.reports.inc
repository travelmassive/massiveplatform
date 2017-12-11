<?php

// tm_subscriptions_user.reports.inc - payment report methods

/**
 * Permission callback. Check if user can access subscription data
 */
function tm_subscriptions_user_access_payment_report_check($account) {
  
  global $user;

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
function tm_subscriptions_reports_user_payments($account) {

  global $conf;
  global $user;

  // Check user is logged in
  if (!user_is_logged_in()) {
    drupal_set_message("Please <a href='/user/login?destination=/payment/reports/subscription-payments-member/" . $account->uid . "'>sign in</a> to view payment details.");
    drupal_goto("user/" . $account->uid);
    return;
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
  drupal_add_js(drupal_get_path('module', 'tm_subscriptions_user') . '/js/tm_subscriptions_user_reports.js');
  $account_link = l(t($account->realname), drupal_get_path_alias("user/" . $account->uid));
  drupal_add_js(array('tm_subscriptions_reports' => array('form_subtitle' => $account_link)), array('type' => 'setting'));
  drupal_add_js(array('tm_subscriptions_reports' => array('form_heading' => 'Your Payments')), array('type' => 'setting'));

  $form = array();

  // get event sales from payment processor callback
  $results = tm_subscriptions_get_user_payments($account->uid);

  // check we got a result
  if ($results == null) {
    drupal_set_message("Oops, an error occured fetching your payment history.<br>Please try again later.", 'error');
    return "";
  }

  // check valid result
  if ($results->success == false) {
    drupal_set_message("Oops, an error occured processing payment history (" . t($results->error) . ").<br>Please try again later.", 'error');
    return "";
  }

  // Help message
  $help_message = $conf["tm_subscriptions_reports_help_message"];
  drupal_get_messages('help-tip');
  drupal_set_message($help_message, 'help-tip');

  // Start report
  $html = "<div id='tm-subscriptions-report'>";

  // Render account details
  // field_user_account_receipt
  if (isset($account->field_user_account_receipt[LANGUAGE_NONE][0]['safe_value'])) {
    $html .= "<p style='font-weight: bold;'>";
    $html .= str_replace("\n", "<br>", strip_tags($account->field_user_account_receipt[LANGUAGE_NONE][0]['safe_value']));
    $html .= "<span class='tm-subscriptions-report-edit'><br><a href='/user/" . $account->uid . "/edit#company-account-details'>Edit</a></span>";
    $html .= "</p>";
  } else {
    $html .= "<p class='tm-subscriptions-report-add'><a href='/user/" . $account->uid . "/edit#user-account-details'>Add your company details to this receipt</a></p>";
  }

  // field_company_account_contact
  if (isset($account->field_user_account_contact[LANGUAGE_NONE][0]['safe_value'])) {
    $html .= "Contact: " . strip_tags($account->field_user_account_contact[LANGUAGE_NONE][0]['safe_value']);
  }

  // Get payments
  $payments = $results->data->payments;
  $payment_totals = $results->data->payment_totals;
  $refund_totals = $results->data->refund_totals;

  // Render payments table
  $html .= "<p><table class='tm-subscriptions-report-table'>";
  $html .= "<tr><th>Payment</th><th>Subscription</th><th>Paid By</th><th class='tm-payments-report-extra'>Payment Date</th><th class='tm-payments-report-extra'>Payment ID</th></tr>";

  // No payments
  if (sizeof($payments) == 0) {
  	$html .= "<tr><td colspan='5'><p style='font-size: larger;'>We haven't received any payments.</p></td></tr>";
  }

  // Payment Details
  foreach($payments as $payment) {

    // look up subsription label
    // if can't be found, fall back to subscription type
    $subscription_label = tm_subscriptions_get_user_subscription_field($account, "subscription_label");
    if ($subscription_label == "") {
      $subscription_label = $payment->subscription_type;
    }

    $html .=  "<tr>";
    $html .=  "<td>" . ($payment->display_amount + 0) . " " . strtoupper($payment->stripe_currency) . "</td>";
    $html .=  "<td>" . $subscription_label . "</td>";
    $html .=  "<td><a target='_blank' href='/user/" . $payment->user_id . "'>" . $payment->user_first_name . " " . $payment->user_last_name . "</a></td>";    
    $html .=  "<td class='tm-payments-report-extra'>" . $payment->created_at . "</td>";
    $html .=  "<td class='tm-payments-report-extra'>" . $payment->charge_id . "</a></td>";
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
      foreach($payment_totals as $currency => $total) {
        $html .= "<strong>Total: " . $total . " " . strtoupper($currency) . "</strong><br>";
      }
    }

    // Our header
    $html .= "<hr>" . $conf["tm_subscriptions_receipt_details"];
	  
	}

  $html .= "</div>";

  $form['event_sales'] = array('#markup' => $html);

  return $form;

}

/**
 * Fetch subscription sales from payment processor callback
 */
function tm_subscriptions_get_user_payments($uid) {

  global $conf;

  // construct url
  $token = $conf["tm_subscriptions_payments_reports_secret_token"];
  $url = $conf["tm_subscriptions_payments_reports_url"] . "?token=" . $token . "&entity_type=user&entity_id=" . $uid;

  //open connection
  $ch = curl_init();

  //set the url, number of POST vars, POST data
  $CURLOPT_SSL_VERIFYHOST = 2;
  $CURLOPT_SSL_VERIFYPEER = true;
  if ($conf["tm_subscriptions_process_check_certificate"] == false) {
    $CURLOPT_SSL_VERIFYHOST = 0;
    $CURLOPT_SSL_VERIFYPEER = false;
  }
  curl_setopt($ch, CURLOPT_URL, $url);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
  curl_setopt($ch, CURLOPT_VERBOSE, false);
  curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, $CURLOPT_SSL_VERIFYHOST);
  curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, $CURLOPT_SSL_VERIFYPEER);
  curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 0); 
  curl_setopt($ch, CURLOPT_TIMEOUT, $conf["tm_subscriptions_process_timeout"]); // timeout in seconds

  //execute post
  $result = curl_exec($ch);

  if ($result === false) {
    return null;
  }

  //close connection
  curl_close($ch);

  // return results
  $res = json_decode($result);
  return $res;
}

