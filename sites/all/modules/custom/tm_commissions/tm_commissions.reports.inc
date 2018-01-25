<?php

// tm_commissions.reports.inc - commission report methods

/**
 * Display terms of use before displaying commissions
 */
function tm_commissions_reports_chapter_commissions_confirm($nid) {

  global $conf;

  // Check we have a node
  $nid = _orig_nid();
  if($nid == 0) {
    print drupal_not_found();
    return;
  }

  // Chapter url
  $chapter_url = drupal_get_path_alias("node/" . $nid);

  // Create form
  $form = array();

  // Display terms
  $html = "<p>" . $conf["tm_commissions_confirm_terms_text"] . "</p>";

  // Agree and cancel buttons
  $html .= '<input onClick="document.location=\'/' . $chapter_url . '/commissions/365\'" type="submit" id="edit-submit" name="op" value="I agree" class="form-submit">';
  $html .= ' <input onClick="document.location=\'/' . $chapter_url . '\'" type="submit" value="Cancel" class="form-submit">';
 
  $form['confirm_agreement'] = array('#markup' => $html);

  return $form;

}

/**
 * Display terms of use before displaying commissions
 */
function tm_commissions_reports_all_chapter_commissions_confirm() {

  global $conf;

  // Create form
  $form = array();

  // Display terms
  $html = "<p>" . $conf["tm_commissions_confirm_terms_text"] . "</p>";

  // Agree and cancel buttons
  $html .= '<input onClick="document.location=\'/chapters/all-chapters/commissions/365\'" type="submit" id="edit-submit" name="op" value="I agree" class="form-submit">';
  $html .= ' <input onClick="document.location=\'/\'" type="submit" value="Cancel" class="form-submit">';
  $form['confirm_agreement'] = array('#markup' => $html);

  return $form;

}

/**
 * Commissions report for chapter
 */
function tm_commissions_reports_chapter_commissions($nid) {
 
  global $conf;

  // Check we have a node
  $nid = _orig_nid();
  if($nid == 0) {
    print drupal_not_found();
    return;
  }

  // Get days to report
  // examples:
  // /chapter/sydney/commissions/30
  // /chapter/sydney/commissions/all
  $days = (int) arg(3);
  $days_label = " (All-time)";
  if ($days != null) {
    $days_label = " (" . $days . " days)";
  }
  if ($days == "365") {
    $days_label = " (12 months)";
  }

  // Load chapter
  $chapter = node_load($nid);

  // Add javascript to form
  drupal_add_js(drupal_get_path('module', 'tm_commissions') . '/js/tm_commissions_reports.js');
  $chapter_link = l(t($chapter->title), drupal_get_path_alias("node/" . $chapter->nid));
  $thirty_days = l(t("30 Days"), drupal_get_path_alias("node/" . $chapter->nid) . "/commissions/30");
  $twelve_months = l(t("12 Months"), drupal_get_path_alias("node/" . $chapter->nid) . "/commissions/365");
  $all_time = l(t("All-Time"), drupal_get_path_alias("node/" . $chapter->nid) . "/commissions/all");
  $all_chapters = l(t("All Chapters"), "/chapters/all-chapters/commissions/365");

  drupal_add_js(array('tm_commissions_reports' => array('form_subtitle' => $chapter->title . " &mdash; " . $thirty_days . " &middot; " . $twelve_months . " &middot; " . $all_time . " &middot; " . $all_chapters)), array('type' => 'setting'));
  drupal_add_js(array('tm_commissions_reports' => array('form_heading' => 'Commissions Report' . $days_label)), array('type' => 'setting'));

  // Create form
  $form = array();

  // Get chapter commissions from payment processor callback
  $chapter_code = $chapter->field_chapter_shortcode[LANGUAGE_NONE][0]['safe_value'];
  $results = tm_commissions_get_chapter_commissions($chapter_code, $days);

  // Check we got a result
  if ($results == null) {
    drupal_set_message("Oops, an error occured fetching chapter commissions.<br>Please try again later.", 'error');
    return "";
  }

  // Check valid result
  if ($results->success == false) {
    drupal_set_message("Oops, an error occured processing chapter commissions (" . t($results->error) . ").<br>Please try again later.", 'error');
    return "";
  }

  // Help message
  $help_message = $conf["tm_commissions_reports_help_message"];
  drupal_get_messages('help-tip');
  drupal_set_message($help_message, 'help-tip');

  // Get payments
  $commissions = $results->data->commissions;
  if (isset($results->data->commissions_totals->total_by_type->chapter)) {
      $commissions_totals = $results->data->commissions_totals->total_by_type->chapter;
  } else {
    $commissions_totals = array();
  }
  $refund_totals = $results->data->refund_totals;

  // Render commissions table
  $html = "<div id='tm-payments-report'>";
  $html .= "<p><table class='tm-payments-report-table'>";
  $html .= "<tr><th>Commission</th><th>Customer</th><th>Type</th><th>Description</th><th>Status</th><th class='tm-payments-report-extra'>Date</th></tr>";

  // No commissions
  if (sizeof($commissions) == 0) {
  	$html .= "<tr><td colspan='6'>No commissions reported in this period.</td></tr>";
  }

  // Commissions Details
  $current_year_month = null;
  $month_count = 0;
  foreach($commissions as $commission) {

    // Keep track of month
    $created_at = new DateTime($commission->created_at);
    $year_month = $created_at->format('Y m');

    // Display new month
    if ($year_month != $current_year_month) {
      $month_title = $created_at->format('F Y');
      $html .= "<tr style='border-top: 1px solid #ddd; font-weight: bold;'><td colspan='5'></td><td>" . $month_title . "</td></tr>";
      $current_year_month = $year_month;
      $month_count++;
    }

    // Display row
    $html .= "<tr>";
    $html .= "<td nowrap>" . tm_commissions_format_number($commission->chapter_amount) . " " . strtoupper($commission->currency) . "</td>";
    $html .= "<td>" . tm_commissions_format_string($commission->customer_name, 20) . "</td>";
    $html .= "<td>" . $commission->item_type . "</td>";
    if ($commission->item_url != "") {
          $html .= "<td><a target='_blank' href='" . $commission->item_url . "'>" . tm_commissions_format_string($commission->item_description, 20) . "</a></td>";
    } else {
          $html .=  "<td>" . tm_commissions_format_string($commission->item_description, 20) . "</td>";
    }
    $payment_status = "Received";
    $payment_status_class = "paid";
    if ($commission->paid_out_chapter) {
      $payment_status = "Paid Out";
      $payment_status_class = "paid_out";
    }
    if ($commission->refunded) {
      $payment_status = "Refunded";
      $payment_status_class = "refunded";
    }
    $html .=  "<td><span class='tm-payments-report-status " . $payment_status_class . "'>" . $payment_status . "</span></td>";
    $created_at = new DateTime($commission->created_at);
    $html .=  "<td class='tm-payments-report-extra'>" . $created_at->format('Y-m-d') . "</td>";
    $html .=  "</tr>";
  }

  $html .=  "</table></p>";

  // Count of commissions
  if ($results->max_results == 1) {
      $html .= "<i>The maximum result limit (" . sizeof($commissions) . ") has been reached.</i><br>";
  } else {
      $html .= "<i>" . (sizeof($commissions) . " total transactions</i><br>");
  }

  // Legend
  $html .= "<p><table cellspacing='0' cellpadding='0'>";
  $html .= "<tr><td><span class='tm-payments-report-status paid'>Received</span></td><td>Customer payment received</td></tr>";
  $html .= "<tr><td><span class='tm-payments-report-status paid_out'>Paid Out</span></td><td>We've paid out the commission</td></tr>";
  $html .= "<tr><td><span class='tm-payments-report-status refunded' style='margin-right: 0.5rem;'>Refunded</span></td><td>Refund has been issued</td></tr>";
  $html .= "</table><p>";

  // Show Commissions stats
  if (sizeof($commissions) > 0) {

    // Divider
    $html .= "<div style='border-bottom: 2px solid #ddd; margin-top: 1rem;'></div>";

    // Commissions Totals
    $html .= "<p>";
    $html .= "<h3 style='margin-bottom: 1rem;'>Total commissions earned " . $days_label . ".</h3>";

    foreach($commissions_totals as $currency => $total) {
      $html .= "<strong>" . tm_commissions_format_number($total) . " " . strtoupper($currency) . "</strong><br>";
    }
  }

  // Display footer terms
  $html .= "<div style='border-bottom: 2px solid #ddd; margin-top: 1rem;'></div>";
  $html .= "<br>" . $conf["tm_commissions_footer_text"];

  $html .= "</div>";

  $form['chapter_commissions'] = array('#markup' => $html);

  return $form;

}

/**
 * Commissions report for all chapters
 */
function tm_commissions_reports_all_chapter_commissions() {
 
  global $conf;
  global $user;

  // Get days to report
  // examples:
  // /chapter/sydney/commissions/30
  // /chapter/sydney/commissions/all
  $days = (int) arg(3);
  $days_label = " (All-time)";
  if ($days != null) {
    $days_label = " (" . $days . " days)";
  }
  if ($days == "365") {
    $days_label = " (12 months)";
  }

  // Add javascript to form
  drupal_add_js(drupal_get_path('module', 'tm_commissions') . '/js/tm_commissions_reports.js');
  $thirty_days = l(t("30 Days"), "/chapters/all-chapters/commissions/30");
  $twelve_months = l(t("12 Months"), "/chapters/all-chapters/commissions/365");
  $all_time = l(t("All-time"), "/chapters/all-chapters/commissions/all-chapters");

  // Create links to chapters
  $chapters_leading = tm_users_get_chapters_leading($user);
  $chapter_links = array();
  foreach ($chapters_leading as $chapter_nid) {
    $chapter = node_load($chapter_nid);
    $chapter_links[] = l(t($chapter->title), drupal_get_path_alias("node/" . $chapter->nid) . "/commissions/365");
  }
  $chapter_links_html = implode(" &middot; " , $chapter_links);

  drupal_add_js(array('tm_commissions_reports' => array('form_subtitle' => "All Chapters &mdash; " . $thirty_days . " &middot; " . $twelve_months . " &middot; " . $all_time . " &middot; "  . $chapter_links_html)), array('type' => 'setting'));
  drupal_add_js(array('tm_commissions_reports' => array('form_heading' => 'Commissions Report' . $days_label)), array('type' => 'setting'));

  $form = array();

  // Get all commissions from payment processor callback
  $chapter_code = "";
  $results = tm_commissions_get_chapter_commissions($chapter_code, $days);

  // check we got a result
  if ($results == null) {
    drupal_set_message("Oops, an error occured fetching chapter commissions.<br>Please try again later.", 'error');
    return "";
  }

  // check valid result
  if ($results->success == false) {
    drupal_set_message("Oops, an error occured processing chapter commissions (" . t($results->error) . ").<br>Please try again later.", 'error');
    return "";
  }

  // Help message
  $help_message = $conf["tm_commissions_reports_help_message"];
  drupal_get_messages('help-tip');
  drupal_set_message($help_message, 'help-tip');

  // Get payments
  $commissions = $results->data->commissions;
  $commissions_totals = array();
  if (isset($results->data->commissions_totals->total_by_type->chapter)) {
      $commissions_totals = $results->data->commissions_totals->total_by_type->chapter;
  }
  $chapter_commissions = $results->data->commissions_totals->by_type->chapter;
  $refund_totals = $results->data->refund_totals;

  // Render commissions table
  $html = "<div id='tm-payments-report'>";
  $html .= "<p><table class='tm-payments-report-table'>";
  $html .= "<tr><th>Commission</th><th>Chapter</th><th>Type</th><th>Description</th><th>Status</th><th class='tm-payments-report-extra'>When</th></tr>";

  // No commissions
  if (sizeof($commissions) == 0) {
    $html .= "<tr><td colspan='6'>No commissions reported in this period.</td></tr>";
  }

  // Get chapter short codes
  $chapter_shortcodes = tm_chapters_get_all_chapter_shortcodes();

  // Commissions Details
  $current_year_month = null;
  $month_count = 0;
  foreach($commissions as $commission) {

    // keep track of month
    $created_at = new DateTime($commission->created_at);
    $year_month = $created_at->format('Y m');

    if ($year_month != $current_year_month) {

      $month_title = "🔥 Latest Commissions";
      if ($month_count > 0) {
        $month_title = $created_at->format('F Y');
      }
      $html .= "<tr style='border-top: 1px solid #ddd; font-weight: bold;'><td colspan='5'></td><td>" . $month_title . "</td></tr>";
      $current_year_month = $year_month;
      $month_count++;
    }

    $html .= "<tr>";
    $html .= "<td nowrap>" . tm_commissions_format_number($commission->chapter_amount) . " " . strtoupper($commission->currency) . "</td>";

    // chapter_url
    $chapter_id = array_search($commission->chapter_code, $chapter_shortcodes);
    $chapter_url = drupal_get_path_alias("node/" . $chapter_id);

    $html .= "<td><a target='_blank' href='/" . $chapter_url . "'>" . $commission->chapter_code . "</a></td>";
    $html .= "<td>" . $commission->item_type . "</td>";
    if ($commission->item_url != "") {
          $html .= "<td><a target='_blank' href='" . $commission->item_url . "'>" . tm_commissions_format_string($commission->item_description, 20) . "</a></td>";
    } else {
          $html .=  "<td>" . tm_commissions_format_string($commission->item_description, 20) . "</td>";
    }
    $payment_status = "Received";
    $payment_status_class = "paid";
    if ($commission->paid_out_chapter) {
      $payment_status = "Paid Out";
      $payment_status_class = "paid_out";
    }
    if ($commission->refunded) {
      $payment_status = "Refunded";
      $payment_status_class = "refunded";
    }
    $html .=  "<td><span class='tm-payments-report-status " . $payment_status_class . "'>" . $payment_status . "</span></td>";

    // show days ago if this month
    if ($month_count == 1) {
      $html .= "<td><i>" . _tm_event_time_elapsed($commission->seconds_ago) . " ago</i></td>";
    } else {
      $html .=  "<td><i>" . $created_at->format('Y-m-d') . "</i></td>";
    }
    $html .=  "</tr>";
  }

  $html .=  "</table></p>";

  // Count of commissions
  if ($results->max_results == 1) {
      $html .= "<i>The maximum result limit (" . sizeof($commissions) . ") has been reached.</i><br>";
  } else {
      $html .= "<i>" . (sizeof($commissions) . " total transactions</i><br>");
  }

  // Legend
  $html .= "<p><table cellspacing='0' cellpadding='0'>";
  $html .= "<tr><td><span class='tm-payments-report-status paid'>Received</span></td><td>Customer payment received</td></tr>";
  $html .= "<tr><td><span class='tm-payments-report-status paid_out'>Paid Out</span></td><td>We've paid out the commission</td></tr>";
  $html .= "<tr><td><span class='tm-payments-report-status refunded' style='margin-right: 0.5rem;'>Refunded</span></td><td>Refund has been issued</td></tr>";
  $html .= "</table><p>";

  // Divider
  $html .= "<div style='border-bottom: 2px solid #ddd; margin-top: 1rem;'></div>";

  // Display commission stats
  if (sizeof($chapter_commissions) > 0) {

    // Commissions Totals
    $html .= "<p>";
    $html .= "<h3 style='margin-bottom: 0;'>Total commissions earned by chapter " . $days_label . ".</h3>";

    foreach($chapter_commissions as $chapter => $name) {

      // Don't show empty row
      if ($chapter == "_empty_") {
        continue;
      }

      // Get chapter URL
      $chapter_id = array_search($chapter, $chapter_shortcodes);
      $chapter_url = drupal_get_path_alias("node/" . $chapter_id);
      $html .= "<br><span style='display: inline-block; width: 50px; line-height: 1.8rem;'><a target='_blank' href='/" . $chapter_url . "'>" . $chapter . "</a></span> ";
      $parts = array();
      foreach ($chapter_commissions->$chapter as $currency => $total) {
        $parts[] = "<span style='display: inline-block; width: 112px; padding-left: 0.5rem; border-radius: 8px; background-color: #fff; font-size: smaller;'>" . tm_commissions_format_number($total) . " " . strtoupper($currency) . "</span>";
      }
      $html .= implode(" + ", $parts);
    }
    $html .= "</p>";
  }

  // Commission by currency
  if (sizeof($commissions) > 0) {

    // Divider
    $html .= "<div style='border-bottom: 2px solid #ddd; margin-top: 1.5rem;'></div>";

    // Commissions Totals
    $html .= "<p>";
    $html .= "<h3>Total commissions earned by currency " . $days_label . ".</h3>";
    foreach($commissions_totals as $currency => $total) {
      $html .= "<strong>" . tm_commissions_format_number($total) . " " . strtoupper($currency) . "</strong><br>";
    }
  }

  // Display footer terms
  $html .= "<div style='border-bottom: 2px solid #ddd; margin-top: 1rem;'></div>";
  $html .= "<br>" . $conf["tm_commissions_footer_text"];

  $html .= "</div>";

  $form['chapter_commissions'] = array('#markup' => $html);

  return $form;

}

/**
 * Helper method format number
 * Add two decimal places or none, ie:
 * 15
 * 29.50
 * 29.15
 */
function tm_commissions_format_number($number) {
  $formatted = number_format($number, 2); // format to two decimal places
  $formatted = str_replace(",", "", $formatted); // remove ,
  $formatted = str_replace(".00", "", $formatted); // remove .00
  return $formatted;
}

/**
 * Helper method format string
 * Add ... if longer than, ie:
 * Short string
 * This is a long str...
 */
function tm_commissions_format_string($string, $length) {

  if (strlen($string) > $length) {
      $string = substr($string, 0, $length) . "...";
  }
  return $string;
}

/**
 * Fetch chapter commissions from commissions processor callback
 */
function tm_commissions_get_chapter_commissions($chapter_code, $days = "") {

  global $conf;

  if ($days == 0) {
    $days = "";
  }

  // construct url
  $token = $conf["tm_payments_reports_secret_token"];
  $url = $conf["tm_commissions_reports_url"] . "?token=" . $token . "&chapter_code=" . $chapter_code . "&days=" . $days;

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

