<?php

// tm_commissions.reports.inc - commission report methods

/**
 * Display message if user needs role to access
 * example to add role:
 * drush user-add-role "pro chapter leader" --mail=demo@example.massiveplatform.com
 */
function tm_commissions_reports_request_access_message() {

  global $conf;
  global $user;

  // Create form
  $form = array();

  // Swap in first name
  $account = user_load($user->uid);
  $terms_text = str_replace("__first_name__", $account->field_user_first_name[LANGUAGE_NONE][0]['value'], $conf["tm_commissions_role_required_message"]); 

  // Display terms
  $html = "<p>" . $terms_text . "</p>";

  // Agree and cancel buttons
  $html .= '<input onClick="document.location=\'' . $conf["tm_commissions_role_required_learn_more_url"] . '\'" type="submit" id="edit-submit" name="op" value="Learn more" class="form-submit">';
  $html .= ' <input onClick="window.history.back()" type="submit" value="Cancel" class="form-submit">';


  $form['confirm_agreement'] = array('#markup' => $html);

  return $form;

}

/**
 * Display terms of use before displaying commissions
 */
function tm_commissions_reports_chapter_commissions_confirm($nid) {

  global $conf;
  global $user;

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

  // Swap in first name
  $account = user_load($user->uid);
  $terms_text = str_replace("__first_name__", $account->field_user_first_name[LANGUAGE_NONE][0]['value'], $conf["tm_commissions_confirm_terms_text"]); 

  // Display terms
  $html = "<p>" . $terms_text . "</p>";

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
  global $user;

  // Create form
  $form = array();

  // Swap in first name
  $account = user_load($user->uid);
  $terms_text = str_replace("__first_name__", $account->field_user_first_name[LANGUAGE_NONE][0]['value'], $conf["tm_commissions_confirm_terms_text"]); 

  // Display terms
  $html = "<p>" . $terms_text . "</p>";

  // Agree and cancel buttons
  $html .= '<input onClick="document.location=\'/chapters/all-chapters/commissions/365\'" type="submit" id="edit-submit" name="op" value="I agree" class="form-submit">';
  $html .= ' <input onClick="document.location=\'/\'" type="submit" value="Cancel" class="form-submit">';
  $form['confirm_agreement'] = array('#markup' => $html);

  return $form;

}

/**
 * Display terms of use before displaying region commissions
 */
function tm_commissions_reports_region_commissions_confirm() {

  global $conf;
  global $user;

  // Create form
  $form = array();

  // Region
  $region_code = arg(3);

  // Swap in first name
  $account = user_load($user->uid);
  $terms_text = str_replace("__first_name__", $account->field_user_first_name[LANGUAGE_NONE][0]['value'], $conf["tm_commissions_regional_confirm_terms_text"]); 

  // Display terms
  $html = "<p>" . $terms_text . "</p>";

  // Agree and cancel buttons
  $html .= '<input onClick="document.location=\'/chapters/all-chapters/commissions-by-region/' . $region_code . '/365\'" type="submit" id="edit-submit" name="op" value="I agree" class="form-submit">';
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
  $all_time = l(t("All-time"), drupal_get_path_alias("node/" . $chapter->nid) . "/commissions/all");
  $all_chapters = l(t("All Chapters"), "/chapters/all-chapters/commissions/365");

  // Generate menu links
  $menu_links_html = tm_commissions_generate_menu_links("all_chapters");

  drupal_add_js(array('tm_commissions_reports' => array('form_subtitle' => $chapter->title . " &mdash; " . $thirty_days . " &middot; " . $twelve_months . " &middot; " . $all_time . " / " . $menu_links_html)), array('type' => 'setting'));
  drupal_add_js(array('tm_commissions_reports' => array('form_heading' => 'Commissions Report' . $days_label)), array('type' => 'setting'));

  // Create form
  $form = array();

  // Get chapter commissions from payment processor callback
  $chapter_code = $chapter->field_chapter_shortcode[LANGUAGE_NONE][0]['safe_value'];
  $results = tm_commissions_get_chapter_commissions("chapter", $chapter_code, $days);

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
    $html .= "<td nowrap> " . tm_commissions_currency_symbol($commission->currency) . tm_commissions_format_number($commission->chapter_amount) . " " . strtoupper($commission->currency) . "</td>";
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

    // more details
    $more_details_html = tm_commissions_generate_detail_html($commission, "chapter_amount", $commission->chapter_code, "chapter");
    $html .=  "<td><span class='tm-payments-report-status " . $payment_status_class . "' " . "' onClick='javascript:jq_alert(\"\", \"" . $more_details_html . "\");'>" . $payment_status . "</span></td>";

    $created_at = new DateTime($commission->created_at);

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
      $html .= "<i>" . sizeof($commissions) . " total transactions</i><br>";
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
      $html .= "<strong>" . tm_commissions_currency_symbol($currency) . tm_commissions_format_number($total) . " " . strtoupper($currency) . "</strong>";

      // show paid out
      if (isset($results->data->commissions_totals->by_type->paid_out_chapter->$chapter_code->$currency)) {
        $paid_out = $results->data->commissions_totals->by_type->paid_out_chapter->$chapter_code->$currency;
        $balance = $total - $paid_out;
        $html .= " / Paid out " . tm_commissions_format_number($paid_out);
        $html .= " / Balance " . tm_commissions_format_number($balance);
      } else {
        $html .= " / Paid out 0.00 / Balance " . tm_commissions_format_number($total);
      }

      $html .= "<br>";
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
  $all_time = l(t("All-time"), "/chapters/all-chapters/commissions/all");

  // Generate menu links
  $menu_links_html = tm_commissions_generate_menu_links("chapters");

  drupal_add_js(array('tm_commissions_reports' => array('form_subtitle' => "All Chapters &mdash; " . $thirty_days . " &middot; " . $twelve_months . " &middot; " . $all_time . " / "  . $menu_links_html)), array('type' => 'setting'));
  drupal_add_js(array('tm_commissions_reports' => array('form_heading' => 'Commissions Report' . $days_label)), array('type' => 'setting'));

  $form = array();

  // Get all commissions from payment processor callback
  $chapter_code = "";
  $results = tm_commissions_get_chapter_commissions("chapter", $chapter_code, $days);

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
    $html .= "<td nowrap> " . tm_commissions_currency_symbol($commission->currency) . tm_commissions_format_number($commission->chapter_amount) . " " . strtoupper($commission->currency) . "</td>";

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

    // more details
    $more_details_html = tm_commissions_generate_detail_html($commission, "chapter_amount", $commission->chapter_code, "all_chapters");
    $html .=  "<td><span class='tm-payments-report-status " . $payment_status_class . "' " . "' onClick='javascript:jq_alert(\"\", \"" . $more_details_html . "\");'>" . $payment_status . "</span></td>";

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
      $html .= "<i>" . sizeof($commissions) . " total transactions</i><br>";
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
        $parts[] = "<span style='display: inline-block; width: 112px; padding-left: 0.5rem; border-radius: 8px; background-color: #fff; font-size: smaller;'>" . tm_commissions_currency_symbol($currency) . tm_commissions_format_number($total) . " " . strtoupper($currency) . "</span>";
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
      $html .= "<strong>" . tm_commissions_currency_symbol($currency) . tm_commissions_format_number($total) . " " . strtoupper($currency) . "</strong><br>";
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
 * Commissions report for region
 */
function tm_commissions_reports_region_commissions() {
 
  global $conf;
  global $user;

  // Get internal code
  // examples:
  // /admin/reports/commissions/NAM/30
  $internal_code = arg(3);

  // Get days to report
  // examples:
  // /admin/reports/commissions/NAM/30
  // /admin/reports/commissions/NAM/all
  $days = (int) arg(4);
  $days_label = " (All-time)";
  if ($days != null) {
    $days_label = " (" . $days . " days)";
  }
  if ($days == "365") {
    $days_label = " (12 months)";
  }

  // Add javascript to form
  drupal_add_js(drupal_get_path('module', 'tm_commissions') . '/js/tm_commissions_reports.js');
  $thirty_days = l(t("30 Days"), "/chapters/all-chapters/commissions-by-region/" . $internal_code . "/30");
  $twelve_months = l(t("12 Months"), "/chapters/all-chapters/commissions-by-region/" . $internal_code . "/365");
  $all_time = l(t("All-time"), "/chapters/all-chapters/commissions-by-region/" . $internal_code . "/all");

  // Generate menu links
  $menu_links_html = tm_commissions_generate_menu_links();

  // set label for code
  $internal_label = $internal_code;
  if (isset($conf["tm_commissions_display_internal"][$internal_code])) {
    $internal_label = $conf["tm_commissions_display_internal"][$internal_code];
  }
  
  drupal_add_js(array('tm_commissions_reports' => array('form_subtitle' => $internal_label . " &mdash; " . $thirty_days . " &middot; " . $twelve_months . " &middot; " . $all_time . " / "  . $menu_links_html)), array('type' => 'setting'));
  drupal_add_js(array('tm_commissions_reports' => array('form_heading' => 'Region Commissions' . $days_label)), array('type' => 'setting'));

  $form = array();

  // Get all commissions from payment processor callback
  $results = tm_commissions_get_chapter_commissions("code", $internal_code, $days);

  // check we got a result
  if ($results == null) {
    drupal_set_message("Oops, an error occured fetching commissions.<br>Please try again later.", 'error');
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
  if (isset($results->data->commissions_totals->by_type->internal)) {
    $internal_commissions = $results->data->commissions_totals->by_type->internal;
  } else {
    $internal_commissions = array();
  }
  $refund_totals = $results->data->refund_totals;

  // Render commissions table
  $html = "<div id='tm-payments-report'>";
  $html .= "<p><table class='tm-payments-report-table'>";
  $html .= "<tr><th>Commission</th><th>Region</th><th>Chapter</th><th>Customer</th><th>Type</th><th>Description</th><th>Status</th><th class='tm-payments-report-extra'>When</th></tr>";

  // No commissions
  if (sizeof($commissions) == 0) {
    $html .= "<tr><td colspan='7'>No commissions reported in this period.</td></tr>";
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
      $month_title = $created_at->format('F Y');
      $html .= "<tr style='border-top: 1px solid #ddd; font-weight: bold;'><td colspan='7'></td><td>" . $month_title . "</td></tr>";
      $current_year_month = $year_month;
      $month_count++;
    }

    // determine which column the code is at
    // ie: internal_code_2
    $internal_column = -1;
    if ($commission->internal_code_1 == $internal_code) {
      $internal_column = 1;
    }
    if ($commission->internal_code_2 == $internal_code) {
      $internal_column = 2;
    }
    if ($commission->internal_code_3 == $internal_code) {
      $internal_column = 3;
    }
    if ($commission->internal_code_4 == $internal_code) {
      $internal_column = 4;
    }
    if ($commission->internal_code_5 == $internal_code) {
      $internal_column = 5;
    }

    $internal_amount_varname = "internal_amount_" . $internal_column;

    $html .= "<tr>";
    $html .= "<td nowrap>" . tm_commissions_currency_symbol($commission->currency) . tm_commissions_format_number($commission->$internal_amount_varname) . " " . strtoupper($commission->currency) . "</td>";

    // chapter_url
    $chapter_id = array_search($commission->chapter_code, $chapter_shortcodes);
    $chapter_url = drupal_get_path_alias("node/" . $chapter_id);

    $html .= "<td>" . $internal_code . "</td>";
    $html .= "<td><a target='_blank' href='/" . $chapter_url . "'>" . $commission->chapter_code . "</a></td>";
    $html .= "<td>" . tm_commissions_format_string($commission->customer_name, 15) . "</td>";
    $html .= "<td>" . $commission->item_type . "</td>";
    if ($commission->item_url != "") {
          $html .= "<td><a target='_blank' href='" . $commission->item_url . "'>" . tm_commissions_format_string($commission->item_description, 15) . "</a></td>";
    } else {
          $html .=  "<td>" . tm_commissions_format_string($commission->item_description, 15) . "</td>";
    }
    $payment_status = "Received";
    $payment_status_class = "paid";
    $paid_out_varname = "paid_out_internal_" . $internal_column;
    if ($commission->$paid_out_varname) {
      $payment_status = "Allocated";
      $payment_status_class = "paid_out";
    }
    if ($commission->refunded) {
      $payment_status = "Refunded";
      $payment_status_class = "refunded";
    }

    // more details
    $more_details_html = tm_commissions_generate_detail_html($commission, $internal_amount_varname, $internal_code, "region");
    $html .=  "<td><span class='tm-payments-report-status " . $payment_status_class . "' " . "' onClick='javascript:jq_alert(\"\", \"" . $more_details_html . "\");'>" . $payment_status . "</span></td>";

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
      $html .= "<i>" . sizeof($commissions) . " total transactions</i><br>";
  }

  // Legend
  $html .= "<p><table cellspacing='0' cellpadding='0'>";
  $html .= "<tr><td><span class='tm-payments-report-status paid'>Received</span></td><td>Customer payment received</td></tr>";
  $html .= "<tr><td><span class='tm-payments-report-status paid_out'>Allocated</span></td><td>The commission has been allocated to the region's holding account</td></tr>";
  $html .= "<tr><td><span class='tm-payments-report-status refunded' style='margin-right: 0.5rem;'>Refunded</span></td><td>Refund has been issued</td></tr>";
  $html .= "</table><p>";

  // Show Commissions stats
  if (sizeof($commissions) > 0) {

    // Divider
    $html .= "<div style='border-bottom: 2px solid #ddd; margin-top: 1rem;'></div>";

    // Commissions Totals
    $html .= "<p>";
    $html .= "<h3 style='margin-bottom: 1rem;'>Region commissions earned " . $days_label . ".</h3>";

    if (isset($internal_commissions->$internal_code)) {
      foreach($internal_commissions->$internal_code as $currency => $total) {

        $html .= "<strong>" . tm_commissions_currency_symbol($currency) . tm_commissions_format_number($total) . " " . strtoupper($currency) . "</strong>";

        // show allocated
        if (isset($results->data->commissions_totals->by_type->paid_out_internal->$internal_code->$currency)) {
          $paid_out = $results->data->commissions_totals->by_type->paid_out_internal->$internal_code->$currency;
          $balance = $total - $paid_out;
          $html .= " / Allocated " . tm_commissions_format_number($paid_out);
          $html .= " / Balance " . tm_commissions_format_number($balance);
        } else {
          $html .= " / Allocated 0.00 / Balance " . tm_commissions_format_number($total);
        }

        $html .= "<br>";
      }

    }
    
  }

  // Display footer terms
  $html .= "<div style='border-bottom: 2px solid #ddd; margin-top: 1rem;'></div>";
  $html .= "<br>" . $conf["tm_commissions_regional_footer_text"];

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
  if ($formatted == "0") { 
    $formatted = "0.00"; // show full 0.00
  }
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
 * Helper method to get currency symbol
 */
function tm_commissions_currency_symbol($currency, $force = false) {

  global $conf;

  // check if we show currency symbol
  if (!$force and !$conf["tm_commissions_show_currency_symbol"]) {
    return "";
  }

  // lookup currency symbol
  $currency_symbol = "";
  if (isset($conf["tm_payments_currency_symbols"][strtolower($currency)])) {
    $currency_symbol = $conf["tm_payments_currency_symbols"][strtolower($currency)];
  }
  return $currency_symbol;
}

/**
 * Helper method to get code label
 */
function tm_commissions_code_label($code) {

   global $conf;

  // lookup currency symbol
  $code_label = $code;
  if (isset($conf["tm_commissions_code_label"][strtoupper($code)])) {
    $code_label = $conf["tm_commissions_code_label"][strtoupper($code)];
  }
  return $code_label;

}

/**
 * Helper method generate menu
 */
function tm_commissions_generate_menu_links($show_chapters = "all_chapters") {

  global $conf;
  global $user;

  // Show link to all commissions
  if ($show_chapters == "all_chapters") {
      $chapter_links_html = l(t("All Chapters"), "/chapters/all-chapters/commissions/365");
  }

  // Show links to chapters user is leading
  if ($show_chapters == "chapters") {

    // Create links to chapters
    $chapters_leading = tm_users_get_chapters_leading($user);
    $chapter_links = array();
    foreach ($chapters_leading as $chapter_nid) {
      $chapter = node_load($chapter_nid);
      $chapter_links[] = l(t($chapter->title), drupal_get_path_alias("node/" . $chapter->nid) . "/commissions/365");
    }
    $chapter_links_html = implode(" &middot; " , $chapter_links);
  }


  // Create links to regions
  $region_links_html = "";

  if (tm_commissions_access_region_commissions_check()) {
    $region_links = array();
    foreach ($conf["tm_commissions_display_internal"] as $region_code => $label) {
      $region_links[] = l(t($region_code), "/chapters/all-chapters/commissions-by-region/" . $region_code . "/365");
    }
    
    if (sizeof($region_links_html) > 0) {
      $region_links_html = " &middot; " . implode(" &middot; " , $region_links);
    }
  }

  return $chapter_links_html . $region_links_html;

}

/**
 * Helper method to generate details text for commission
 * Types: chapter, all_chapters, region
 */
function tm_commissions_generate_detail_html($commission, $varname, $code, $show_details = "chapter") {

  // calculate commission
  $commission_pct = 0;
  if ($commission->total_amount > 0) {
    $commission_pct = round(($commission->$varname / $commission->total_amount) * 100, 0);
  }
  
  // show id
  $more_details_html = "";
  $more_details_html .= "<strong>Commission Details</strong>";
 
  // date
  $created_at = new DateTime($commission->created_at);
  $created_at_text = $created_at->format('Y-m-d');
  $more_details_html .= "<br><small>ID: " . $commission->id . " / Date: " . $created_at_text . "</small>";

  // dash
  $more_details_html .= "<br>&mdash;";

  // chapter
  if ($show_details == "chapter") {
    $more_details_html .= "<br>Customer: " . strip_tags($commission->customer_name);
    $more_details_html .= "<br>Type: " . strip_tags($commission->item_type);
    $more_details_html .= "<br>Description: " . strip_tags($commission->item_description);
    $more_details_html .= "<br>Commission: " .  tm_commissions_currency_symbol($commission->currency) . tm_commissions_format_number($commission->$varname) . " " . strtoupper($commission->currency) . " &rarr; " . $code . " (" . $commission_pct . "%)";
  }

  // all_chapters
  if ($show_details == "all_chapters") {
    $more_details_html .= "<br>Type: " . strip_tags($commission->item_type);
    $more_details_html .= "<br>Description: " . strip_tags($commission->item_description);
    $more_details_html .= "<br>Commission: " .  tm_commissions_currency_symbol($commission->currency) . tm_commissions_format_number($commission->$varname) . " " . strtoupper($commission->currency) . " &rarr; " . $code . " (" . $commission_pct . "%)";
  }

  // show extra details
  if ($show_details == "region") {
    $more_details_html .= "<br>Customer: " . strip_tags($commission->customer_name);
    $more_details_html .= "<br>Type: " . strip_tags($commission->item_type);
    $more_details_html .= "<br>Description: " . strip_tags($commission->item_description);
    $more_details_html .= "<br>Commission: " .  tm_commissions_currency_symbol($commission->currency) . tm_commissions_format_number($commission->$varname) . " " . strtoupper($commission->currency) . " &rarr; " . $code . " (" . $commission_pct . "%)";
    if ($commission->invoice_id != "") {
      $more_details_html .= "<br>Invoice ID: " . $commission->invoice_id;
    }
    if ($commission->charge_id != "") {
      $more_details_html .= "<br>Charge ID: " . $commission->charge_id;
    }
  }
  
  // replace '
  $more_details_html = str_replace("'", "&#39;", $more_details_html);
  $more_details_html = str_replace("\"", "&#34;", $more_details_html);

  return $more_details_html;
}


/**
 * Fetch chapter commissions from commissions processor callback
 */
function tm_commissions_get_chapter_commissions($mode = "chapter", $lookup_code = "", $days = "") {

  global $conf;

  if ($days == 0) {
    $days = "";
  }

  // construct url
  $token = $conf["tm_payments_reports_secret_token"];
  $url = $conf["tm_commissions_reports_url"] . "?token=" . $token . "&mode=" . $mode . "&lookup_code=" . $lookup_code . "&days=" . $days;

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

