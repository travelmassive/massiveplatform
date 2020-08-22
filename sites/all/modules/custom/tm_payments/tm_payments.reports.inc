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

  // get event sales from tm_checkout
  $results = tm_checkout_get_event_payments($event->nid);

  // Help message
  $help_message = $conf["tm_payments_reports_help_message"];
  drupal_get_messages('help-tip');
  drupal_set_message($help_message, 'help-tip');

  // Get payments
  $payments = $results;

  // Render payments table
  $html = "<div id='tm-payments-report'>";
  $html .= "<p><table class='tm-payments-report-table'>";
  $html .= "<tr><th>Payment</th><th>When</th><th>Customer</th><th>Status</th><th class='tm-payments-report-extra'>Timestamp</th><th class='tm-payments-report-extra'>Payment ID</th></tr>";

  // No payments
  if (sizeof($payments) == 0) {
  	$html .= "<tr><td colspan='6'><p>You haven't sold any tickets yet.</p></td></tr>";
  }

  // Payment Details
  $payment_totals = array();
  $refund_totals = array();

  foreach($payments as $payment) {

    $html .=  "<tr>";
    $html .=  "<td>" . ($payment->display_amount + 0) . " " . strtoupper($payment->stripe_currency) . "</td>";
    $html .=  "<td><i>" . _tm_event_time_elapsed(time() - $payment->timestamp) . " ago</i></td>";
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
    $html .=  "<td class='tm-payments-report-extra'>" . date("Y-m-d j:i", $payment->timestamp)  . "</td>";
    $html .=  "<td class='tm-payments-report-extra'>" . $payment->payment_id . "</a></td>";
    $html .=  "</tr>";

    // calculate payment and refund
    if (!$payment->refunded) {
      if (!isset($payment_totals[$payment->stripe_currency])) {
        $payment_totals[$payment->stripe_currency] = 0;
      }
      $payment_totals[$payment->stripe_currency] += $payment->display_amount;
    }

  }

  $html .=  "</table></p>";

  if (sizeof($payments) > 0) {

	  // Payment Totals
	  $html .= "<p>";
	  $html .= (sizeof($payments) . " payments<br>");
	  foreach($payment_totals as $currency => $total) {
	    $html .= "<strong>Total: " . $total . " " . strtoupper($currency) . "</strong><br>";
	  }
	  $html .= "<i>Note: Excludes processing and forex fees</i></p>";
	}

  // Estimated commission
  $html .= "<p>";
  $event_commission = _tm_payments_get_event_commission($event);
  $html .= "Event commission: " . ($event_commission + 0) . "%<br>";
  foreach($payment_totals as $currency => $total) {
    $html .= "<strong>Estimated earnings: " . round($total * ($event_commission/100)) . " " . strtoupper($currency) . "</strong><br>";
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

