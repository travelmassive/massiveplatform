<?php

// tm_reports.event.inc - event report methods

/**
 * Check permission to view event insights
 */
function tm_reports_event_insights_check_permission() {

  global $user;
  global $conf;
  $nid = arg(2); // ie: /events/insights/123

  if($nid == 0) {
    return false;
  }

  // check if:
  // - can edit event (including moderator or administrator)
  $node = node_load($nid);
  return tm_events_check_permission_edit_event($user, $node);

}

/**
 * Generate event stats for event organizer 
 */
function tm_reports_event_insights($nid) {

  global $user;
  global $conf;
  $nid = arg(2); // ie: /events/insights/123

  if($nid == 0) {
    print drupal_not_found();
    return;
  }

  // validate node
  $event = node_load($nid);
  if ($event == null) {
  	print drupal_not_found();
  	return;
  }

  $segment_label = "Industry Segments";
  if (isset($conf["tm_segment_label"])) {
    $segment_label = $conf["tm_segment_label"];
  }

  $event = node_load($nid);
  $return_link = l(t('View event'), url('node/'.$nid, array('absolute' => TRUE)));
 
  // Generate HTML
  $html = _tm_reports_insights_js_css_header();
  $redraw_functions = array("drawChart", "draw_insights_event_registrations", "draw_insights_event_registration_frequency_aggregate");
  $html .= _tm_reports_insights_js_redraw_function($redraw_functions);

  // show event tip
  if (isset($conf["tm_reports_event_insights_tip"])) {
    $html .= "<style>.messages--status.messages { margin-left: 1rem; } </style>";
    drupal_set_message($conf["tm_reports_event_insights_tip"], "help-tip");
  }

  // Start article
  $html .= "<article class='trilithon node view-mode-full node-by-viewer clearfix'>";

  // Insights
  $html .= "<section class='contained contained-block'>";
  $html .= "<header class='contained-head'>
            <h1 class='prime-title'>" . $event->title . " Insights &middot; <span style='color: #0068a8;'>" . $return_link . "</span></h1>
            </header>";
  $html .= _tm_reports_insights_table_html($event->nid, "event");
  $html .= "</section>";

  // Event registrations
  $flag_event_register = flag_get_flag('event_register', NULL);
  $html .= "<section class='contained contained-block'>";
  $html .= "<header class='contained-head'>
            <h1 class='prime-title'>Event Registrations</h1>
            </header>";
  $html .= _tm_reports_insights_flagging_chart_html($event->nid, $flag_event_register->fid, "", "Event Registrations", "Day", "insights_event_registrations", "INTERVAL 5 YEAR", "dayofyear");
  $html .= "</section>";

  // Registration frequency
  $html .= "<section class='contained contained-block'>";
  $html .= "<header class='contained-head'>
            <h1 class='prime-title'>Number of Events Members Have Attended (including this event)</h1>
            </header>";
  $html .= _tm_reports_insights_event_registration_frequency_html($event->nid, "event", true);
  $html .= "</section>";

  // Industry Segments
  $html .= "<section class='contained contained-block'>";
  $html .= "<header class='contained-head'>
            <h1 class='prime-title'>" . $segment_label . " Of Members Attending This Event</h1>
            </header>";
  $html .= _tm_reports_insights_segments_html($event->nid, "event");
  $html .= "</section>";

  // Close article
  $html .= "</article>";


  $form['html'] =  array(
    '#type' => 'markup',
    '#markup' => $html);

  return $form;
}

/**
 * Calculate stats for chapter
 * NOTE: Cached
 */
function _tm_reports_get_event_insights($event_id, $ignore_cache = false) {

  global $conf;

  // check cache
  $cache = cache_get('tm-reports-get-event-insights-' . $event_id, 'cache');
  if (!empty($cache) and (!$ignore_cache)) {
    return $cache->data;
  }

  // get flags
  $flag_event_register = flag_get_flag('event_register', NULL);
  $flag_event_waitlist = flag_get_flag('event_waitlist', NULL);
  $flag_event_paid = flag_get_flag('event_paid', NULL);

  // TOTAL MEMBERS REGISTERERED
  $query_sql = "SELECT COUNT(*) total FROM {flagging} f WHERE f.fid = :fid AND f.entity_type = 'node' AND f.entity_id = :event_id";
  $query = db_query($query_sql, array(':fid' => $flag_event_register->fid, ':event_id' => $event_id))->fetch();
  $event_total_registrations = $query->total;
  $insights["event_total_registrations"] = $event_total_registrations;

  // TOTAL MEMBERS WAITLISTED
  $query_sql = "SELECT COUNT(*) total FROM {flagging} f WHERE f.fid = :fid AND f.entity_type = 'node' AND f.entity_id = :event_id";
  $query = db_query($query_sql, array(':fid' => $flag_event_waitlist->fid, ':event_id' => $event_id))->fetch();
  $event_total_waitlist = $query->total;
  if ($event_total_waitlist > 0) {
  	$insights["event_total_waitlist"] = $event_total_waitlist;
  }

  // TOTAL MEMBERS PAID
  $query_sql = "SELECT COUNT(*) total FROM {flagging} f WHERE f.fid = :fid AND f.entity_type = 'node' AND f.entity_id = :event_id";
  $query = db_query($query_sql, array(':fid' => $flag_event_paid->fid, ':event_id' => $event_id))->fetch();
  $event_total_payments = $query->total;
  if ($event_total_payments > 0) {
  	$insights["event_total_payments"] = $event_total_payments;
  }

  // RETURN DATA
  $data_labels = tm_reports_get_insights_labels();
  $results = ["data_values" => $insights, "data_labels" => $data_labels];
  cache_set('tm-reports-get-event-insights-' . $event_id, $results, 'cache', time() + 300); // 5 mins
  cache_set('tm-reports-get-event-insights-display-' . $event_id, $results, 'cache'); // uptdate permanent display cache for public chapter pages

  return $results;
}

