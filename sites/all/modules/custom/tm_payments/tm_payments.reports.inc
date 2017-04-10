<?php

// tm_payments.reports.inc - payment report methods

/**
 * Payment reports for event
 */
function tm_payments_reports_event_sales($event) {
 
  global $conf;

  // Add javascript to form
  drupal_add_js(drupal_get_path('module', 'tm_payments') . '/js/tm_payments_reports.js');
  $event_link = l(t($event->title), drupal_get_path_alias("node/" . $event->nid));
  drupal_add_js(array('tm_payments_reports' => array('form_subtitle' => $event_link)), array('type' => 'setting'));
  drupal_add_js(array('tm_payments_reports' => array('form_heading' => 'Ticket Sales')), array('type' => 'setting'));

  $form = array();

  // get event sales from payment processor callback
  $results = tm_payments_get_event_sales($event->nid);

  // check we got a result
  if ($results == null) {
    drupal_set_message("Oops, an error occured fetching event sales.<br>Please try again later.", 'error');
    return "";
  }

  // check valid result
  if ($results->success == false) {
    drupal_set_message("Oops, an error occured processing event sales (" . t($results->error) . ").<br>Please try again later.", 'error');
    return "";
  }

  // Help message
  $help_message = $conf["tm_payments_reports_help_message"];
  drupal_get_messages('help-tip');
  drupal_set_message($help_message, 'help-tip');

  // Get payments
  $payments = $results->data->payments;
  $payment_totals = $results->data->payment_totals;
  $refund_totals = $results->data->refund_totals;

  // Render payments table
  $html = "<div id='tm-payments-report'>";
  $html .= "<p><table class='tm-payments-report-table'>";
  $html .= "<tr><th>Payment</th><th>When</th><th>Customer</th><th>Status</th><th class='tm-payments-report-extra'>Timestamp</th><th class='tm-payments-report-extra'>Payment ID</th></tr>";

  // No payments
  if (sizeof($payments) == 0) {
  	$html .= "<tr><td colspan='6'>You haven't sold any tickets yet. Go hustle and sell some tickets! :)</td></tr>";
  }

  // Payment Details
  foreach($payments as $payment) {

    $html .=  "<tr>";
    $html .=  "<td>" . ($payment->display_amount + 0) . " " . $payment->stripe_currency . "</td>";
    $html .=  "<td><i>" . _tm_event_time_elapsed($payment->seconds_ago) . "</i></td>";
    $html .=  "<td><a target='_blank' href='/user/" . $payment->user_id . "'>" . $payment->user_first_name . " " . $payment->user_last_name . "</a></td>";
    $payment_status = "Received";
    $payment_status_class = "paid";
    if ($payment->paid_out) {
      $payment_status = "Paid Out";
      $payment_status_class = "paid_out";
    }
    if ($payment->refunded) {
      $payment_status = "Refunded";
      $payment_status_class = "refunded";
    }
    $html .=  "<td><span class='tm-payments-report-status " . $payment_status_class . "'>" . $payment_status . "</span></td>";
    $html .=  "<td class='tm-payments-report-extra'>" . $payment->created_at . "</td>";
    $html .=  "<td class='tm-payments-report-extra'>" . $payment->charge_id . "</a></td>";
    $html .=  "</tr>";
  }

  $html .=  "</table></p>";

  if (sizeof($payments) > 0) {

	  // Payment Totals
	  $html .= "<p>";
	  $html .= (sizeof($payments) . " payments<br>");
	  foreach($payment_totals as $currency => $total) {
	    $html .= "<strong>Total: " . $total . " " . $currency . "</strong><br>";
	  }
	  $html .= "<i>Note: Excludes processing and forex fees</i></p>";
	}

  // Estimated commission
  $html .= "<p>";
  $event_commission = _tm_payments_get_event_commission($event);
  $html .= "Event commission: " . ($event_commission + 0) . "%<br>";
  foreach($payment_totals as $currency => $total) {
    $html .= "<strong>Estimated earnings: " . round($total * ($event_commission/100)) . " " . $currency . "</strong><br>";
  }
  $html .= "</p>";


  // Who to payout to
  $payout_uid = $event->uid; // default event owner
  if (isset($event->field_event_payment_payout_to[LANGUAGE_NONE][0]['target_id'])) {
    $payout_uid = $event->field_event_payment_payout_to[LANGUAGE_NONE][0]['target_id'];
  }
  $payout_user = user_load($payout_uid);
  $payout_user_name = tm_events_username($payout_user);
  $html .= "<p>";
  $html .= "Payout set to: <a target='_blank' href='/" . drupal_get_path_alias("user/" . $payout_uid) . "'>" . $payout_user_name . "</a>";
  $html .= "</p>";

  // Legend
  $html .= "<p><table cellspacing='0' cellpadding='0'>";
  $html .= "<tr><td><span class='tm-payments-report-status paid'>Received</span></td><td>Customer payment received</td></tr>";
  $html .= "<tr><td><span class='tm-payments-report-status paid_out'>Paid Out</span></td><td>We've paid out your commission</td></tr>";
  $html .= "<tr><td><span class='tm-payments-report-status refunded' style='margin-right: 0.5rem;'>Refunded</span></td><td>Refund has been issued</td></tr>";
  $html .= "</table><p>";

  $html .= "</div>";

  $form['event_sales'] = array('#markup' => $html);

  return $form;

}

/**
 * Fetch event sales from payment processor callback
 */
function tm_payments_get_event_sales($event_id) {

  global $conf;

  // construct url
  $token = $conf["tm_payments_reports_secret_token"];
  $url = $conf["tm_payments_reports_url"] . "?token=" . $token . "&event_id=" . $event_id;

  //open connection
  $ch = curl_init();

  //set the url, number of POST vars, POST data
  $CURLOPT_SSL_VERIFYHOST = 2;
  $CURLOPT_SSL_VERIFYPEER = true;
  if ($conf["tm_payments_process_check_certificate"] == false) {
    $CURLOPT_SSL_VERIFYHOST = 0;
    $CURLOPT_SSL_VERIFYPEER = false;
  }
  curl_setopt($ch, CURLOPT_URL, $url);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
  curl_setopt($ch, CURLOPT_VERBOSE, false);
  curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, $CURLOPT_SSL_VERIFYHOST);
  curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, $CURLOPT_SSL_VERIFYPEER);
  curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 0); 
  curl_setopt($ch, CURLOPT_TIMEOUT, $conf["tm_payments_process_timeout"]); // timeout in seconds

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

