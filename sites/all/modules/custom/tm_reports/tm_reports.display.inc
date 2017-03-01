<?php

// tm_reports.display.inc - reporting display methods

/**
 * Data labels for insights table
 */
function tm_reports_get_insights_labels() {

  global $conf;

  $segment_label = "Industry Segment";
  if (isset($conf["tm_segment_label"])) {
    $segment_label = $conf["tm_segment_label"];
  }

  // DATA LABELS
  $data_labels = ["last_event_time" => "Last Event",
  "next_event_time" => "Next Event",
  "num_events_upcoming" => "Future Events",
  "num_events_past_30_days" => "Events in past 30 days",
  "num_events_past_12_months" => "Events in past 12 Months",
  "num_events_past_total" => "Total Past Events",
  "num_partner_events_past_30_days" => "Partner Events in past 30 days",
  "num_partner_events_past_12_months" => "Partner Events in past 12 Months",
  "members_30_days_total" => "New Members (30 days)",
  "members_90_days_total" => "New Members (90 days)",
  "members_30_days_pct" => "% Growth (30 days)",
  "members_90_days_pct" => "% Growth (90 days)",
  "members_total" => "Total Members",
  "members_approved_total"  => "Total " . tm_users_get_approved_member_label("plural"),
  "members_unapproved_total" => "Total " . tm_users_get_unapproved_member_label("plural"),
  "members_approved_pct" => "% " . tm_users_get_approved_member_label("plural"),
  "members_unapproved_pct" => "% " . tm_users_get_unapproved_member_label("plural"),
  "members_approved_with_segment" => tm_users_get_approved_member_label("plural") . " with " . $segment_label,
  "members_approved_without_segment" => tm_users_get_approved_member_label("plural") . " without " . $segment_label,
  "members_with_segment_pct" => "% " . tm_users_get_approved_member_label("plural") . " with " . $segment_label,
  "num_event_waitlists" => "Total Event Waitlists",
  "num_event_registrations" => "Total Event Registrations",
  "num_event_payments" => "Total Event Payments",
  "num_unique_event_waitlists" => "Total Unique Event Waitlists",
  "num_unique_event_registrations" => "Total Unique Event Registrations",
  "pct_unique_event_registrations" => "% " . tm_users_get_approved_member_label("plural") . " who have registered for events",
  "num_unique_event_payments" => "Total Unique Event Payments",
  "num_mutual_follows" => "Total Mutual Connections",
  "num_connections" => "Total Connections (All Followers)",
  "num_organization_followers" => "Total Organization Followers",
  "num_member_followers" => "Total Member Followers",
  "num_chapter_countries" => "Chapter Countries",
  "num_member_countries" => "Member Countries",
  "num_events" => "Total Number Events",
  "num_organizations" => "Number of Organizations",
  "num_members_sent_messages" => "Members who have sent Messages",
  "num_total_status_updates" => "Total Status Updates",
  "num_promoted_status_updates" => "Promoted Status Updates",
  "num_moderated_status_updates" => "Moderated Status Updates",
  "num_total_status_links" => "Total Links Shared",
  "num_total_preview_links_created" => "Total Links Previewed",
  "num_total_status_views_daily" => "Status Views (24 Hours)",
  "num_total_status_views_all_time" => "Total Status Views",
  "num_signedin_members_24_hours" => "All Active Members (24 Hours)",
  "num_signedin_members_7_days" => "All Active Members (7 Days)",
  "num_signedin_members_30_days" => "All Active Members (30 Days)",
  "num_signedin_members_90_days" => "All Active Members (90 Days)",
  "num_signedin_members_older_than_90_days" => "All Inactive Members (> 90 Days)",
  "num_signedin_members_24_hours_approved" => "Active " . tm_users_get_approved_member_label("plural") . " (24 Hours)",
  "num_signedin_members_7_days_approved" => "Active " . tm_users_get_approved_member_label("plural") . " (7 Days)",
  "num_signedin_members_30_days_approved" => "Active " . tm_users_get_approved_member_label("plural") . " (30 Days)",
  "num_signedin_members_90_days_approved" => "Active " . tm_users_get_approved_member_label("plural") . " (90 Days)",
  "num_signedin_members_older_than_90_days_approved" => "Inactive " . tm_users_get_approved_member_label("plural") . " (> 90 Days)",
  "pct_signedin_members_24_hours" => "% Active Members (24 Hours) (All/" . tm_users_get_approved_member_label("plural") . ")",
  "pct_signedin_members_7_days" => "% Active Members (7 Days) (All/" . tm_users_get_approved_member_label("plural") . ")",
  "pct_signedin_members_30_days" => "% Active Members (30 Days) (All/" . tm_users_get_approved_member_label("plural") . ")",
  "pct_signedin_members_90_days" => "% Active Members (90 Days) (All/" . tm_users_get_approved_member_label("plural") . ")",
  "pct_signedin_members_older_than_90_days" => "% Inactive Members (> 90 Days) (All/" . tm_users_get_approved_member_label("plural") . ")",
  "num_signedin_members_24_hours_chapter" => "Chapter Active Members (24 Hours)",
  "num_signedin_members_7_days_chapter" => "Chapter Active Members (7 Days)",
  "num_signedin_members_30_days_chapter" => "Chapter Active Members (30 Days)",
  "num_signedin_members_90_days_chapter" => "Chapter Active Members (90 Days)",
  "num_signedin_members_older_than_90_days_chapter" => "Chapter Inactive Members (> 90 Days)",
  "pct_signedin_members_24_hours_chapter" => "% Active Members (24 Hours)",
  "pct_signedin_members_7_days_chapter" => "% Active Members (7 Days)",
  "pct_signedin_members_30_days_chapter" => "% Active Members (30 Days)",
  "pct_signedin_members_90_days_chapter" => "% Active Members (90 Days)",
  "pct_signedin_members_older_than_90_days_chapter" => "% Inactive Members (> 90 Days)",
  "num_logins_facebook" => "Total Users Logged in with Facebook",
  "num_logins_twitter" => "Total Users Logged in with Twitter",
  "num_chapters" => "Number of Chapters",
  "num_chapter_leaders" => "Number of Chapter Leaders",
  "num_members_with_home_chapter" => "Members from this Chapter (Home Chapter)",
  "pct_members_with_home_chapter" => "% Members from this Chapter (Home Chapter)",
  "num_members_without_home_chapter" => "Members from other Chapters",
  "pct_members_without_home_chapter" => "% Members from other Chapters",
  "num_unique_chapter_event_registrations" => "Members who have registered for events",
  "pct_unique_chapter_event_registrations" => "% " . tm_users_get_approved_member_label("plural") . " who have registered for events",
  "avg_event_registeration_frequency" => "Average Number Of Events Attended (of those who registered)",
  "total_chapter_member_connections" => "Total Chapter Connections (Member Follows)",
  "event_total_registrations" => "Total event registrations",
  "event_total_waitlist" => "Total event waitlists",
  "event_total_payments" => "Total event payments",
  ];
  return $data_labels;
}

/**
 * Generate a chart of flags on a node (ie: signups)
 */
function _tm_reports_insights_flagging_chart_html($node_id, $flag_id, $chart_title = "Flagging", $v_title = "Members", $h_title = "Month", $div_id = "insights_flagging", $timestamp_interval = "INTERVAL 1 YEAR", $group_by = "month", $aggregate_total = false, $chartType = "ColumnChart") {

  // Step 1. Get chapter insight data
  $results = _tm_reports_flagging_time_series($node_id, $flag_id, "node", $timestamp_interval, $group_by);

  // Step 2. (Optional) Aggregate total
  if ($aggregate_total) {
    $total = 0;
    $aggregated_results = array();
    foreach ($results as $result) {
      $total = $total + $result["total"];
      $result["total"] = $total;
      $aggregated_results[] = $result;
    }
    $results = $aggregated_results;
  }

  // Step 3. Render chart
  $html = _tm_reports_insights_chart_html($results, $chart_title, $v_title, $h_title, $div_id, "LineChart");

  return $html;
}

/**
 * Generate a chart of all user signups
 */
function _tm_reports_insights_all_signups_html() {

  // Step 1. Get chapter insight data
  $results = _tm_reports_new_users_time_series();
  // Step 2. Render chart
  $html = _tm_reports_insights_chart_html($results, "", "New Members", "Month", "new_signups");
  return $html;
}

/**
 * Generate a chart of all event registrations
 */
function _tm_reports_insights_all_event_registrations_html() {

  $flag = flag_get_flag('event_register', NULL);
  // Step 1. Get chapter insight data
  $results = _tm_reports_flagging_time_series(NULL, $flag->fid, "node");
  // Step 2. Render chart
  $html = _tm_reports_insights_chart_html($results, "", "Event Registrations", "Month", "event_registrations");
  return $html;
}

/**
 * Generate a chart of all event payments
 */
function _tm_reports_insights_all_event_payments_html() {

  $flag = flag_get_flag('event_paid', NULL);
  // Step 1. Get chapter insight data
  $results = _tm_reports_flagging_time_series(NULL, $flag->fid, "node");
  // Step 2. Render chart
  $html = _tm_reports_insights_chart_html($results, "", "Event Payments", "Month", "event_payments");
  return $html;
}

/**
 * Generate a chart of all member follows
 */
function _tm_reports_insights_all_member_connections_html() {

  $flag = flag_get_flag('follow_members', NULL);
  // Step 1. Get chapter insight data
  $results = _tm_reports_flagging_time_series(NULL, $flag->fid, "user");
  // Step 2. Render chart
  $html = _tm_reports_insights_chart_html($results, "", "Follow Members", "Month", "follow_members");
  return $html;
}

/**
 * Generate a chart of all organisation follows
 */
function _tm_reports_insights_all_organization_connections_html() {

  $flag = flag_get_flag('follow_organizations', NULL);
  // Step 1. Get chapter insight data
  $results = _tm_reports_flagging_time_series(NULL, $flag->fid, "node");
  // Step 2. Render chart
  $html = _tm_reports_insights_chart_html($results, "", "Follow Organization", "Month", "follow_organizations");
  return $html;
}

/**
 * Generate a chart of number of chapters over time
 */
function _tm_reports_insights_monthly_chapters_html() {

  // Step 1. Get chapter insight data
  $results = _tm_reports_monthly_chapters_time_series("INTERVAL 1 YEAR");
  // Step 2. Render chart
  $html = _tm_reports_insights_chart_html($results, "", "Total Chapters", "Month", "monthly_chapters");
  return $html;
}

/**
 * Generate a chart of status updates per month
 */
function _tm_reports_insights_monthly_status_updates_html() {

  // Step 1. Get status update data
  $results = _tm_reports_status_updates_time_series("INTERVAL 1 YEAR");
  // Step 2. Render chart
  $html = _tm_reports_insights_chart_html($results, "", "Status Updates", "Month", "status_updates");
  return $html;
}

/**
 * Generate a chart of links shared per month
 */
function _tm_reports_insights_monthly_links_shared_html() {

  // Step 1. Get status update data
  $results = _tm_reports_links_shared_time_series("INTERVAL 1 YEAR");
  // Step 2. Render chart
  $html = _tm_reports_insights_chart_html($results, "", "Links Shared", "Month", "links_shared");
  return $html;
}

/**
 * Generate a chart of frequency of events for attendees
 */
function _tm_reports_insights_event_registration_frequency_html($node_id = null, $node_type = "chapter", $aggregate = false) {
  
  // Step 1. Get status update data
  $results = _tm_reports_event_registration_frequency($node_id, $node_type, $aggregate);
  // Step 2. Render chart
  $js_fn = "insights_event_registration_frequency";
  if ($aggregate) {
    $js_fn = "insights_event_registration_frequency_aggregate";
  }
  $html = _tm_reports_insights_chart_html($results, "", "Members", "Number of Events", $js_fn);
  return $html;
}

/**
 * Generate Google Chart from a data series
 */
function _tm_reports_insights_chart_html($results, $chart_title, $v_title, $h_title, $div_id, $chartType = "ColumnChart") {

  // Format in Google chart
  $html = "<script>";

  $html .= "\ngoogle.setOnLoadCallback(draw_" . $div_id . ");";
  $html .= "\nfunction draw_" . $div_id . "() {";
  $html .= "\nvar data = new google.visualization.DataTable();";
  $html .= "\ndata.addColumn('string', '" . $h_title . "');";
  $html .= "\ndata.addColumn('number', '" . $v_title . "');";

  $html .= "data.addRows([";

  foreach ($results as $result) {
    if (isset($result["label"])) {
      $html .= "['" . $result["label"] . "', " . $result["total"] . "],";
    } else {
      $html .= "['" . $result["group_year"] . "-" . $result["group_month"] . "', " . $result["total"] . "],";
    }
  }

  $html .= <<<EOT
      ]);
EOT;

  $html .= "
      var options = {
        title: '" . $chart_title . "',
        hAxis: {
          title: '" . $h_title . "',
        },
        vAxis: {
          title: '" . $v_title . "'
        },
        legend: { position: 'none' },
      };";

  $html .= "\nvar chart = new google.visualization." . $chartType . "(document.getElementById('" . $div_id . "'));";
  $html .= "\nchart.draw(data, options);";
  $html .= "\n}";
  $html .= "\n</script>";
  $html .= '<div id="' . $div_id . '" style="width: 100%; height: 400px;"></div>';

  return $html;
}

/**
 * Load Google Chart API via JS
 */
function _tm_reports_insights_js_css_header() {

  // Html to load Google chart data
  $html = <<<EOT
<script type='text/javascript' src='https://www.google.com/jsapi'></script>
<script type='text/javascript'>
// load google charts
google.load('visualization', '1.1', {packages:['corechart', 'table', 'bar']});

// hide drupal title
document.getElementById("page-title").style.display = 'none';
</script>
<style>
.google-visualization-table-tr-head { display: none; }
</style>
EOT;

  return $html;
}
 
/*
 * Generate js redraw listener code to resize charts/graphs on window resize
 */
function _tm_reports_insights_js_redraw_function($redraw_functions) {

  $html = "<script>\n";

  $html .= "window.onresize = function(e) {
  delay(function(){";

  foreach ($redraw_functions as $fn) {
    $html .= "\nif (typeof " . $fn . " == 'function') { " . $fn . "(); }";
  }
  
  $html .= "}, 250); 
}

var delay = (function(){
  var timer = 0;
  return function(callback, ms){
    clearTimeout (timer);
    timer = setTimeout(callback, ms);
  };
})();

</script>";

  return $html;
}

/**
 * Generate Chapter/Global Insights Table via Google Charts
 */
function _tm_reports_insights_table_html($node_id = "", $node_type = "chapter") {

  // Step 1. Get chapter insight data or global data
  if ($node_id == "") {
    $data = _tm_reports_get_global_insights();
  } elseif ($node_type == "chapter") {
    $data = _tm_reports_get_chapter_insights($node_id);
  } elseif ($node_type == "event") {
    $data = _tm_reports_get_event_insights($node_id, true); // no cache
  }
  $chapter_insights = $data["data_values"];
  $data_labels = $data["data_labels"];

  // Step 2. Format in Google chart
  $html = <<<EOT
<script>
google.setOnLoadCallback(drawTable);
function drawTable() {
  var data = new google.visualization.DataTable();
  data.addColumn('string', 'Description');
  data.addColumn('string', 'Value');
  data.addRows([
EOT;

  $hide_values = [];
  if ($node_id != "") {
    $hide_values = ["members_approved_with_segment", "members_approved_without_segment", "members_unapproved_total"];
  }
  foreach ($chapter_insights as $key => $value) {
    if (!in_array($key, $hide_values)) {
      if ($data_labels[$key][0] == "%") {
        $html .= "['" . $data_labels[$key] . "', '" . $value . "%'],";
      } else {
        $html .= "['" . $data_labels[$key] . "', '" . $value . "'],";
      }
    }
  }

  $html .= <<<EOT
  ]);

  var table = new google.visualization.Table(document.getElementById('insights_table'));
  table.draw(data, {showRowNumber: false, sort: false, width: '100%'});
}
</script>
<div id="insights_table" style="width: 100%;"></div>
EOT;

  return $html;
}

/**
 * Generate Chapter / Global Segments Pie Chart via Google Charts
 */
function _tm_reports_insights_segments_html($node_id = "", $node_type = "chapter") {

  // // Step 1. Get segment data
  $segment_data = _tm_reports_get_industry_segment_data($node_id, $node_type);

  // Step 2. Format in Google chart
  $html = <<<EOT

<script>
google.setOnLoadCallback(drawChart);
function drawChart() {
  var data = google.visualization.arrayToDataTable([
  ['Segment', 'Members'],
EOT;

  foreach ($segment_data as $segment) {
    $html .= "['" . str_replace("'", "", $segment["name"]) . "'," . $segment["total"] . "],";
  }
  $html .= <<<EOT
  ]);

EOT;
  
  // Report chart colors
  // If ?reports_color_hash is provided, create a unique color code for each segment
  // This is so segment colors are consistent across all graphs regardless of order in the chart
  // ie: 0: { color: '#014eff' },
  // ie: 1: { color: '#ab6754' }
  // default is off
  $color_hash = null;

  // get from cookie
  if (isset($_COOKIE['Drupal_visitor_tm_reports_color_hash'])) {
   $color_hash = $_COOKIE['Drupal_visitor_tm_reports_color_hash'];
  }

  // ie: 
  // ?chart_color_hash
  // ?chart_color_hash = 123
  if (isset($_GET["reports_color_hash"])) {
    $color_hash = $_GET["reports_color_hash"];
    if ($color_hash == "") {
      $color_hash = null;
    }
    user_cookie_save(array("tm_reports_color_hash" => $color_hash));
  }

  if ($color_hash != null) {
    $count = 0;
    $color_parts = array();
    foreach ($segment_data as $segment) {
      $color_parts[] = $count++ . ": { color: '#" . _tm_reports_string_color_code($segment["name"] . $color_hash) . "' }";
    }
    $html .= "var slices = {" . implode(", ", $color_parts) . "};";
  } else {
    $html .= "var slices = {};";
  }

  $random_number = rand(0,1000);
  $html .= <<<EOT
  var options = { 'slices': slices };
  var chart = new google.visualization.PieChart(document.getElementById('insights_segments'));
  chart.draw(data, options);
}
</script>
<div id="insights_segments" style="width: 100%; height: 700px;"></div>
<div style='padding: 8px; text-align: right;'><a href='?reports_color_hash={$random_number}'>Randomize chart color</a> | <a href='?reports_color_hash='>Reset</a></div>
EOT;

  return $html;
}

/**
 * Helper method to convert string to color code
 */
function _tm_reports_string_color_code($str) {
  // ref: http://stackoverflow.com/questions/3724111/how-can-i-convert-strings-to-an-html-color-code-hash
  $code = dechex(crc32($str));
  $code = substr($code, 0, 6);
  return $code;
}
