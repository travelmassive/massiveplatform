<?php

// tm_checkout.report_commissions.inc

/**
 * Permission callback. Check if user can access all payments data
 */
function tm_checkout_reports_all_payments_check() {

  global $conf;
  global $user;

  // require signed in user
  if (!user_is_logged_in()) {
    return false;
  }

  // get account
  $account = user_load($user->uid);
  
  // allow moderator and admin
  if ((in_array("moderator", $account->roles)) or (in_array("administrator", $account->roles))) {
    return true;
  }

  // not allowed
  return false;
}

/**
 * Permission callback. Check if user can access chapter payments data
 */
function tm_checkout_reports_chapter_payments_check($chapter_id) {

  global $conf;
  global $user;

  // require signed in user
  if (!user_is_logged_in()) {
    return false;
  }

  // require chapter id
  $chapter_id = intval($chapter_id);
  if ($chapter_id == 0) {
    return false;
  }

  // get account
  $account = user_load($user->uid);
  
  // allow moderator and admin
  if ((in_array("moderator", $account->roles)) or (in_array("administrator", $account->roles))) {
    return true;
  }

  // allow chapter leader or community manager
  if (tm_users_is_chapter_leader($account->uid, $chapter_id)) {
    return true;
  }

  // not allowed
  return false;
  
}

/**
 * Payments report for all chapters
 */
function tm_checkout_reports_all_payments() {
 
  global $conf;
  global $user;

  // Get days to report
  // examples:
  // /checkout/reports/all-payments/30
  // /checkout/reports/all-payments/all
  $days = (int) arg(3);
  $days_label = "(All-time)";
  if ($days != null) {
    $days_label = "(" . $days . " days)";
  }
  if ($days == "365") {
    $days_label = "(12 months)";
  }

  // include css
  drupal_add_css(drupal_get_path('module', 'tm_checkout') . '/css/tm_checkout_reports.css');

  // Help message
  $days_links = "<a href='/checkout/reports/all-payments/30'>30 days</a>, <a href='/checkout/reports/all-payments/30'>90 days</a>, <a href='/checkout/reports/all-payments/365'>12 months</a>, <a href='/checkout/reports/all-payments/all'>All-time</a>";
  $help_message = "Here's a report of your payments " . $days_label . " &mdash; " . $days_links;
  drupal_get_messages('help-tip');
  drupal_set_message($help_message, 'help-tip');

  // get all payments
  $payments = tm_checkout_get_all_payments($days);

  // Start report
  $html = "<div id='tm-subscriptions-report'>";

  // Render payments table
  $html .= "<p><table class='tm-subscriptions-report-table'>";
  $html .= "<tr><th>Payment</th><th>When</th><th>Item</th><th>Customer</th><th>Chapter</th><th class='tm-payments-report-extra'>Payment Date</th><th>Status</th><th class='tm-payments-report-extra'>Payment ID</th></tr>";

  // No payments
  if (sizeof($payments) == 0) {
    $html .= "<tr><td colspan='5'><p style='font-size: larger;'>You haven't received any payments in this period.</p></td></tr>";
  }

  // Payment Details
  $payment_totals = array();
  $refund_totals = array();

  // Chapters
  $event_chapter_labels = array();
  $chapter_shortcodes = tm_chapters_get_all_chapter_shortcodes();

  // Payment Details
  foreach($payments as $payment) {

    // checkout action
    $checkout_action = $payment->checkout_action;

    // item labels
    $item_label = "Payment";
    if ($checkout_action == "event_payment") {
      $item_label = "<a href='/node/" . $payment->entity_id . "'>Event Ticket: " . strip_tags($payment->entity_title) . "</a>";
    }

    if ($checkout_action == "upgrade_subscription") {
      $item_label = "Upgrade Subscription";
    }

    if ($checkout_action == "renew_subscription") {
      $item_label = "Renew Subscription";
    }

    // chapter label
    $chapter_label = "";
    if ($checkout_action == "event_payment") {
      if (!isset($event_chapter_labels[$payment->entity_id])) {
        $event = node_load($payment->entity_id);
        if (isset($event->field_chapter[LANGUAGE_NONE][0])) {
          $event_chapter_nid = $event->field_chapter[LANGUAGE_NONE][0]['target_id'];
          $chapter_label = "<a href='/node/" . $event_chapter_nid . "'>" . $chapter_shortcodes[$event_chapter_nid] . "</a>";
        }
        $event_chapter_labels[$payment->entity_id] = $chapter_label; // cache
      } else {
        $chapter_label = $event_chapter_labels[$payment->entity_id];
      }
    }

    $html .=  "<tr>";
    $html .=  "<td>";
    $html .= ($payment->display_amount + 0) . " " . strtoupper($payment->stripe_currency);
    $html .= "</td>";
    $html .=  "<td><i>" . _tm_event_time_elapsed(time() - $payment->timestamp) . " ago</i></td>";
    $html .=  "<td class='tm-checkout-reports-item'>" . $item_label . "</td>";
    $user_profile_url = "user/" . $payment->user_id;
    $html .=  "<td><a target='_blank' href='/" . $user_profile_url . "'>" . $payment->user_first_name . " " . $payment->user_last_name . "</a></td>";
    $html .= "<td>" . $chapter_label . "</td>";
    $html .=  "<td class='tm-payments-report-extra'>" . date("Y-m-d", $payment->timestamp) . "</td>";

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

  // Payments
  if (sizeof($payments) > 0) {
    $html .= "<p>";
    $html .= (sizeof($payments) . " payments<br><br>");
    foreach($payment_totals as $currency => $total) {
      $html .= "<strong>Total: " . $total . " " . strtoupper($currency) . "</strong><br>";
    }
    $html .= "<br><i>Note: Excludes processing and forex fees</i></p>";
  }

  $html .= "</div>";

  // return form
  $form = array();
  $form['all_payments'] = array('#markup' => $html);
  return $form;

}

/**
 * Payments report for chapter
 */
function tm_checkout_reports_chapter_payments() {
 
  global $conf;
  global $user;

  // Get chapter
  $chapter_id = (int) arg(3);
  if ($chapter_id == null) {
    drupal_goto("/");
  }
  $chapter = node_load($chapter_id);
  if ($chapter == null) {
     drupal_goto("/");
  }
 
  // Get days to report
  // examples:
  // /checkout/reports/chapter-payments/123/30
  // /checkout/reports/chapter-payments/123/all
  $days = (int) arg(4);
  $days_label = "(All-time)";
  if ($days != null) {
    $days_label = "(" . $days . " days)";
  }
  if ($days == "365") {
    $days_label = "(12 months)";
  }

  // include css
  drupal_add_css(drupal_get_path('module', 'tm_checkout') . '/css/tm_checkout_reports.css');

   // Add javascript to form
  drupal_add_js(drupal_get_path('module', 'tm_checkout') . '/js/tm_checkout_reports.js');
  $chapter_name = tm_chapters_formal_name($chapter);
  $chapter_link = l(t($chapter_name), drupal_get_path_alias("node/" . $chapter_id));
  drupal_add_js(array('tm_checkout_reports' => array('form_subtitle' => $chapter_link)), array('type' => 'setting'));
  drupal_add_js(array('tm_checkout_reports' => array('form_heading' => 'Chapter Payments')), array('type' => 'setting'));

  // Help message
  $days_links = "<a href='/checkout/reports/all-payments/30'>30 days</a>, <a href='/checkout/reports/all-payments/30'>90 days</a>, <a href='/checkout/reports/all-payments/365'>12 months</a>, <a href='/checkout/reports/all-payments/all'>All-time</a>";
  $help_message = "Here's a report of your earnings " . $days_label . " &mdash; " . $days_links;
  drupal_get_messages('help-tip');
  drupal_set_message($help_message, 'help-tip');

  // get all events for chapter
  $chapter_event_ids = tm_events_get_all_chapters_events($chapter_id);
  $payments = tm_checkout_get_entities_payments($chapter_event_ids, $days);

  // Start report
  $html = "<div id='tm-subscriptions-report'>";

  // Render payments table
  $html .= "<p><table class='tm-subscriptions-report-table'>";
  $html .= "<tr><th>Payment</th><th>Commission</th><th>When</th><th>Item</th><th>Customer</th><th class='tm-payments-report-extra'>Payment Date</th><th>Status</th><th class='tm-payments-report-extra'>Payment ID</th></tr>";

  // No payments
  if (sizeof($payments) == 0) {
    $html .= "<tr><td colspan='5'><p style='font-size: larger;'>You haven't received any payments in this period.</p></td></tr>";
  }

  // Payment Details
  $payment_totals = array();
  $refund_totals = array();
  $commission_totals = array();
  $event_commission = array();

  // Payment Details
  foreach($payments as $payment) {

    // checkout action
    $checkout_action = $payment->checkout_action;

    // calculate commission
    $commission_amount = 0;
    if ($checkout_action == "event_payment") {
      if (!isset($event_commission[$payment->entity_id])) {
        $event = node_load($payment->entity_id);
        if ($event != null) {
          $commission_amount = _tm_payments_get_event_commission($event);
          $event_commission[$payment->entity_id] = $commission_amount; // cache
        }
      }
      // lookup commission amount
      $commission_amount = $event_commission[$payment->entity_id];

      // store commission amount
      if (!isset($commission_totals[$payment->stripe_currency])) {
        $commission_totals[$payment->stripe_currency] = 0;
      }
      $commission_totals[$payment->stripe_currency] += ($payment->display_amount) * ($commission_amount/100);
    }

    // item labels
    $item_label = "Payment";
    if ($checkout_action == "event_payment") {
      $item_label = "<a href='/node/" . $payment->entity_id . "'>Event Ticket: " . strip_tags($payment->entity_title) . "</a>";
    }

    if ($checkout_action == "upgrade_subscription") {
      $item_label = "Upgrade Subscription";
    }

    if ($checkout_action == "renew_subscription") {
      $item_label = "Renew Subscription";
    }

    $html .=  "<tr>";
    $html .=  "<td>";
    $html .= ($payment->display_amount + 0) . " " . strtoupper($payment->stripe_currency);
    $html .= "</td>";
    $html .=  "<td>";
    $html .= ($payment->display_amount * ($commission_amount/100)) + 0;
    $html .= "</td>";
    $html .=  "<td><i>" . _tm_event_time_elapsed(time() - $payment->timestamp) . " ago</i></td>";
    $html .=  "<td class='tm-checkout-reports-item'>" . $item_label . "</td>";
    $user_profile_url = "user/" . $payment->user_id;
    $html .=  "<td><a target='_blank' href='/" . $user_profile_url . "'>" . $payment->user_first_name . " " . $payment->user_last_name . "</a></td>";
    $html .=  "<td class='tm-payments-report-extra'>" . date("Y-m-d", $payment->timestamp) . "</td>";

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

  // Total commissions
  if (sizeof($payments) > 0) {
    $html .= "<p>";
    $html .= (sizeof($payments) . " payments<br><br>");
    foreach($commission_totals as $currency => $total) {
      $html .= "<strong>Total Earnings: " . number_format($total, 2) . " " . strtoupper($currency) . "</strong><br>";
    }
    $html .= "<br><i>Note: Excludes processing and forex fees</i></p>";
  }

  // Legend
  $html .= "<p><table cellspacing='0' cellpadding='0'>";
  $html .= "<tr><td><span class='tm-payments-report-status paid'>Received</span></td><td>Customer payment received</td></tr>";
  $html .= "<tr><td><span class='tm-payments-report-status paid_out'>Paid Out</span></td><td>We've paid out the commission</td></tr>";
  $html .= "<tr><td><span class='tm-payments-report-status refunded' style='margin-right: 0.5rem;'>Refunded</span></td><td>Refund has been issued</td></tr>";
  $html .= "</table><p>";

  $html .= "</div>";

  // return form
  $form = array();
  $form['all_payments'] = array('#markup' => $html);
  return $form;

}
