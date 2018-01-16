<?php

// tm_reports.chapter.inc - chapter report methods

/**
 * Check permission to view chapter insights
 */
function tm_reports_chapter_insights_check_permission() {

  global $user;
  global $conf;
  $nid = _orig_nid();

  if($nid == 0) {
    return false;
  }

  // check if:
  // - chapter leader, or community manager
  // - moderator, or
  // - administrator
  return (tm_users_is_chapter_leader($user->uid, $nid) or (in_array("moderator", $user->roles)) or (in_array("administrator", $user->roles)));

}

/** 
 * Redirect from chapter reports to clean URL for chapter insights
 */
function tm_reports_chapter_insights_redirect($nid) {
  $chapter = node_load($nid);
  $redirect_url = drupal_get_path_alias("node/" . $nid) . "/insights";
  drupal_goto("/" . $redirect_url);
}

/**
 * Generate chapter stats for chapter leader 
 */
function tm_reports_chapter_insights($nid) {

  global $user;
  global $conf;
  $nid = _orig_nid();

  if($nid == 0) {
    print drupal_not_found();
    return;
  }

  $segment_label = "Industry Segment";
  if (isset($conf["tm_segment_label"])) {
    $segment_label = $conf["tm_segment_label"];
  }

  $flag_signup = flag_get_flag('signup', NULL);
  $flag_event_register = flag_get_flag('event_register', NULL);
  $flag_event_waitlist = flag_get_flag('event_waitlist', NULL);
  $flag_event_paid = flag_get_flag('event_paid', NULL);
  $chapter = node_load($nid);
  $return_link = l(t('View chapter'), url('node/'.$nid, array('absolute' => TRUE)));

  // Generate HTML
  $html = _tm_reports_insights_js_css_header();
  $redraw_functions = array("drawChart", "draw_insights_signups", "draw_insights_event_registrations", "draw_insights_event_waitlists", "draw_insights_event_payments", "draw_insights_event_registration_frequency", "draw_insights_event_registration_frequency_aggregate");
  $html .= _tm_reports_insights_js_redraw_function($redraw_functions);

  // Start article
  $html .= "<article class='trilithon node view-mode-full node-by-viewer clearfix'>";

  // Insights
  $html .= "<section class='contained contained-block'>";
  $html .= "<header class='contained-head'>
            <h1 class='prime-title'>" . $chapter->title . " Insights &middot; <span style='color: #0068a8;'>" . $return_link . "</span></h1>
            </header>";
  $html .= _tm_reports_insights_table_html($chapter->nid);
  $html .= "</section>";

  // New Members
  $html .= "<section class='contained contained-block'>";
  $html .= "<header class='contained-head'>
            <h1 class='prime-title'>New Members - " . $chapter->title . "</h1>
            </header>";
  $html .= _tm_reports_insights_flagging_chart_html($chapter->nid, $flag_signup->fid, "", "New Members", "Month", "insights_signups");
  $html .= "</section>";

  // Event Registrations
  $chapter_event_nids = _tm_reports_get_chapter_event_nids($chapter->nid);

  // Show event charts if chapter has had events
  if (sizeof($chapter_event_nids) > 0) {

    $html .= "<section class='contained contained-block'>";
    $html .= "<header class='contained-head'>
              <h1 class='prime-title'>Event Registrations - " . $chapter->title . "</h1>
              </header>";
    $html .= _tm_reports_insights_flagging_chart_html($chapter_event_nids, $flag_event_register->fid, "", "Event Registrations", "Month", "insights_event_registrations");
    $html .= "</section>";

    // Event Waitlists
    // Only display is chapter has had waitlisted members
    $flag_event_waitlist = flag_get_flag('event_waitlist', NULL);
    $query = "SELECT COUNT(uid) AS amount FROM flagging WHERE entity_id IN (SELECT entity_id AS nid FROM field_data_field_chapter WHERE bundle = 'event' AND field_chapter_target_id = :chapter_id) AND fid = :fid";
    $num_event_waitlists = db_query($query, array(':fid' => $flag_event_waitlist->fid, ':chapter_id' => $chapter->nid))->fetch();
    if ($num_event_waitlists->amount > 0) {

      // ? $insights["num_event_payments"] = $num_event_payments->amount;  

      $html .= "<section class='contained contained-block'>";
      $html .= "<header class='contained-head'>
                <h1 class='prime-title'>Event Waitlists - " . $chapter->title . "</h1>
                </header>";
      $html .= _tm_reports_insights_flagging_chart_html($chapter_event_nids, $flag_event_waitlist->fid, "", "Event Waitlists", "Month", "insights_event_waitlists");
      $html .= "</section>";

    }

    // Event Payments
    // Only display if payments have been made
    $query = "SELECT COUNT(uid) AS amount FROM flagging WHERE entity_id IN (SELECT entity_id AS nid FROM field_data_field_chapter WHERE bundle = 'event' AND field_chapter_target_id = :chapter_id) AND fid = :fid";
    $num_event_payments = db_query($query, array(':fid' => $flag_event_paid->fid, ':chapter_id' => $chapter->nid))->fetch();
    if ($num_event_payments->amount > 0) {

      $html .= "<section class='contained contained-block'>";
      $html .= "<header class='contained-head'>
                <h1 class='prime-title'>Event Payments - " . $chapter->title . "</h1>
                </header>";
      $html .= _tm_reports_insights_flagging_chart_html(_tm_reports_get_chapter_event_nids($chapter->nid), $flag_event_paid->fid, "", "Event Payments", "Month", "insights_event_payments");
      $html .= "</section>";
    }

    // Registration frequency
    $html .= "<section class='contained contained-block'>";
    $html .= "<header class='contained-head'>
              <h1 class='prime-title'>Number of Events Members Have Attended - " . $chapter->title . "</h1>
              </header>";
    $html .= _tm_reports_insights_event_registration_frequency_html($chapter->nid, "chapter", true);
    $html .= "</section>";

  }

  // Industry Segments
  $html .= "<section class='contained contained-block'>";
  $html .= "<header class='contained-head'>
            <h1 class='prime-title'>" . $segment_label . " - " . $chapter->title . "</h1>
            </header>";
  $html .= _tm_reports_insights_segments_html($chapter->nid);
  $html .= "</section>";

  // Close article
  $html .= "</article>";

  // show tip?
  if (isset($conf["tm_reports_chapter_insights_tip"])) {
    $html .= "<style>.messages--status.messages { margin-left: 1rem; } </style>";
    drupal_set_message($conf["tm_reports_chapter_insights_tip"], "help-tip");
  }

  $form['html'] =  array(
    '#type' => 'markup',
    '#markup' => $html);

  return $form;
}

/* Modify chapter report view pre render */
function _tm_reports_views_pre_render_chapter_report(&$view) {

  // developer: uncomment to reset flag count table
  //_tm_flags_fix_flag_counts();

  // Vars for members counts
  $table = 'flagging';
  $table_alias = 'fl';
  $real_field = 'fid';

  // Add additional columns for each row
  foreach ($view->result as $i => $result) {

    // CHAPTER ID
    $chapter_id = $view->result[$i]->nid;

    // GET INSIGHTS
    $insights = _tm_reports_get_chapter_insights($chapter_id)["data_values"];

    // Last Date format
    if (is_numeric($i) && (!empty($view->result[$i]->field_field_event_date[0]['raw']['value']))) {

      // Change Date format
      $event_date = strtotime($view->result[$i]->field_field_event_date[0]['raw']['value']);
      $formatted_date = date("Y-m-d", $event_date);
      $view->result[$i]->field_field_event_date[0]['rendered']['#markup'] = $formatted_date;
      
      // Link date to event
      $event_nid = $view->result[$i]->field_chapter_node_nid;
      $url = 'node/'.$event_nid;
      $interval = format_interval((time() - $event_date) , 1) . t(' ago');
      $event_time_ago_link = l($interval, $url);
      $view->result[$i]->field_field_event_date_1[0]['rendered']['#markup'] = "<span style='white-space: nowrap;'>" . $event_time_ago_link . "</span>";     
    }

    // Next Date format
    if (is_numeric($i) && (!empty($view->result[$i]->field_field_event_date_3[0]['raw']['value']))) {
      // Change Date format
      $event_date = strtotime($view->result[$i]->field_field_event_date_3[0]['raw']['value']);
      $interval = format_interval(($event_date - time()) , 1);
      $next_event_nid = $view->result[$i]->next_event_date_entity_id;
      $url = 'node/'.$next_event_nid;
      $next_event_link = l($interval, $url);        
      $view->result[$i]->field_field_event_date_3[0]['rendered']['#markup'] = $next_event_link;   
    }

    // NEW MEMBERS PAST 30 DAYS
    $view->field["members_30_days_total"] = _tm_reports_add_handler_field_numeric($view, $table, $table_alias, $real_field, "members_30_days_total", "New Members (30 Days)");
    $view->field["members_30_days_total"]->view->result[$i]->members_30_days_total = $insights["members_30_days_total"];

    // NEW MEMBERS PAST 90 DAYS
    $view->field["members_90_days_total"] = _tm_reports_add_handler_field_numeric($view, $table, $table_alias, $real_field, "members_90_days_total", "New Members (90 Days)");
    $view->field["members_90_days_total"]->view->result[$i]->members_90_days_total = $insights["members_90_days_total"];

     // MEMBERSHIP GROWTH % PAST 30 DAYS
    $view->field["members_30_days_pct"] = _tm_reports_add_handler_field_numeric($view, $table, $table_alias, $real_field, "members_30_days_pct", "% Growth (30 Days)");
    $view->field["members_30_days_pct"]->view->result[$i]->members_30_days_pct = $insights["members_30_days_pct"];

    // MEMBERSHIP GROWTH % PAST 90 DAYS
    $view->field["members_90_days_pct"] = _tm_reports_add_handler_field_numeric($view, $table, $table_alias, $real_field, "members_90_days_pct", "% Growth (90 Days)");
    $view->field["members_90_days_pct"]->view->result[$i]->members_90_days_pct = $insights["members_90_days_pct"];

    // TOTAL MEMBERS
    $view->field["members_total"] = _tm_reports_add_handler_field_numeric($view, $table, $table_alias, $real_field, "members_total", "Total Members");
    $view->field["members_total"]->view->result[$i]->members_total = $insights["members_total"];

    // TOTAL APPROVED MEMBERS
    $view->field["members_approved_total"] = _tm_reports_add_handler_field_numeric($view, $table, $table_alias, $real_field, "members_approved_total", "Total " . tm_users_get_approved_member_label("plural"));
    $view->field["members_approved_total"]->view->result[$i]->members_approved_total = $insights["members_approved_total"];

    // TOTAL UNAPPROVED MEMBERS
    $view->field["members_unapproved_total"] = _tm_reports_add_handler_field_numeric($view, $table, $table_alias, $real_field, "members_unapproved_total", "Total " . tm_users_get_unapproved_member_label("plural"));
    $view->field["members_unapproved_total"]->view->result[$i]->members_unapproved_total = $insights["members_unapproved_total"];

    // % UNAPPROVED MEMBERS
    $view->field["members_unapproved_pct"] = _tm_reports_add_handler_field_numeric($view, $table, $table_alias, $real_field, "members_unapproved_pct", "% " . tm_users_get_unapproved_member_label("plural"));
    $view->field["members_unapproved_pct"]->view->result[$i]->members_unapproved_pct = $insights["members_unapproved_pct"];

  }
}

/**
 * Calculate stats for chapter
 * NOTE: Cached
 */
function _tm_reports_get_chapter_insights($chapter_ids, $ignore_cache = false) {

  // prepare array if only one chapter
  if (!is_array($chapter_ids)) {
    $chapter_ids = array($chapter_ids);
  }

  // check if array
  // max key length is 250 chars (memcached) so lets md5 it so we get 32 char
  $chapter_ids_key = md5(implode("-", $chapter_ids)); 

  // check cache
  $cache = cache_get('tm-reports-get-chapter-insights-' . $chapter_ids_key, 'cache');
  if (!empty($cache) and (!$ignore_cache)) {
    return $cache->data;
  }

  $insights = array();

  $flag = flag_get_flag('signup', NULL);
  $approved_role = user_role_load_by_name("approved user");
  $days_ago_30 = time() - (60 * 60 * 24 * 30); // 30 days ago
  $days_ago_90 = time() - (60 * 60 * 24 * 90); // 90 days ago

  // NEXT EVENT DATE
  // Fetch the nearest upcoming event that references a particular chapter.
  // Only returns one event, the one with the closest starting date.
  // Fetch the current and make sure it's UTC and user-defined timezone.
  $current_date = new DateTime('now', new DateTimeZone('UTC'));
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'event')
    ->propertyCondition('status', 1)
    ->fieldCondition('field_chapter', 'target_id', $chapter_ids)
    ->fieldCondition('field_event_date', 'value2', $current_date->format('Y-m-d H:i:s'), '>=')
    ->fieldOrderBy('field_event_date', 'value2', 'ASC')
    ->range(0,1);
  $result = $query->execute();

  // Initialize time_until.
  $time_until = NULL;

  $next_event_time = "No upcoming events";
  if (isset($result['node'])) {
    $next_event_array = array_keys($result['node']);
    $next_event = node_load($next_event_array[0]);
    $event_date = field_get_items('node',$next_event,'field_event_date');
    
    // Create datetime object and ensure UTC timezone, instead of user-defined timezone.
    $event_timezone = _tm_get_event_timezone($next_event);
    $event_datetime = new DateTime($event_date[0]['value'], new DateTimeZone($event_timezone));
    //$time_until = format_interval($event_datetime->format('U') - $current_date->format('U'), 2);
    $next_event_time = _tm_event_time_elapsed($event_datetime->format('U') - $current_date->format('U'));
    if ($time_until == "0 seconds") {
      $next_event_time = "Event now";
    }
  }
  $insights["next_event_time"] = $next_event_time;

  // LAST EVENT DATE
  // Fetch the nearest upcoming event that references a particular chapter.
  // Only returns one event, the one with the closest starting date.
  // Fetch the current and make sure it's UTC and user-defined timezone.
  $current_date = new DateTime('now', new DateTimeZone('UTC'));
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'event')
    ->propertyCondition('status', 1)
    ->fieldCondition('field_chapter', 'target_id', $chapter_ids)
    ->fieldCondition('field_event_date', 'value2', $current_date->format('Y-m-d H:i:s'), '<=')
    ->fieldOrderBy('field_event_date', 'value2', 'DESC')
    ->range(0,1);
  $result = $query->execute();

  // Initialize time_until.
  $time_until = NULL;

  // NUMBER OF EVENTS PAST ALL TIME
  $query_sql = "SELECT COUNT(*) total FROM {field_data_field_event_date} e 
  LEFT JOIN field_data_field_chapter on field_data_field_chapter.entity_id = e.entity_id
  WHERE field_data_field_chapter.entity_type = 'node'
  AND field_data_field_chapter.field_chapter_target_id IN (:chapter_ids)
  AND e.entity_type = 'node' AND field_event_date_value >= NOW();";
  $query = db_query($query_sql, array(':chapter_ids' => $chapter_ids))->fetch();
  $insights["num_events_upcoming"] = $query->total;

  $last_event_time = "No past events";
  if (isset($result['node'])) {
    $next_event_array = array_keys($result['node']);
    $next_event = node_load($next_event_array[0]);
    $event_date = field_get_items('node',$next_event,'field_event_date');
    
    // Create datetime object and ensure UTC timezone, instead of user-defined timezone.
    $event_timezone = _tm_get_event_timezone($next_event);
    $event_datetime = new DateTime($event_date[0]['value'], new DateTimeZone($event_timezone));
    //$time_until = format_interval($event_datetime->format('U') - $current_date->format('U'), 2);
    $last_event_time = _tm_event_time_elapsed($current_date->format('U') - $event_datetime->format('U'));
  }
  $insights["last_event_time"] = $last_event_time . " ago";

  // NUMBER OF EVENTS PAST YEAR
  $query_sql = "SELECT COUNT(*) total FROM {field_data_field_event_date} e 
  LEFT JOIN field_data_field_chapter on field_data_field_chapter.entity_id = e.entity_id
  WHERE field_data_field_chapter.entity_type = 'node'
  AND field_data_field_chapter.field_chapter_target_id IN (:chapter_ids)
  AND e.entity_type = 'node' AND field_event_date_value >= DATE_SUB(NOW(),INTERVAL 1 YEAR);";
  $query = db_query($query_sql, array(':chapter_ids' => $chapter_ids))->fetch();
  $insights["num_events_past_12_months"] = $query->total;

  // NUMBER OF EVENTS PAST ALL TIME
  $query_sql = "SELECT COUNT(*) total FROM {field_data_field_event_date} e 
  LEFT JOIN field_data_field_chapter on field_data_field_chapter.entity_id = e.entity_id
  WHERE field_data_field_chapter.entity_type = 'node'
  AND field_data_field_chapter.field_chapter_target_id IN (:chapter_ids)
  AND e.entity_type = 'node' AND field_event_date_value <= NOW();";
  $query = db_query($query_sql, array(':chapter_ids' => $chapter_ids))->fetch();
  $insights["num_events_past_total"] = $query->total;

  // NEW MEMBERS PAST 30 DAYS
  $query_sql = "SELECT COUNT(*) total FROM {flagging} f WHERE f.fid = :fid AND f.entity_type = 'node' AND f.entity_id IN (:chapter_ids) AND timestamp >= :time_ago";
  $query = db_query($query_sql, array(':fid' => $flag->fid, ':chapter_ids' => $chapter_ids, ':time_ago' => $days_ago_30))->fetch();
  $insights["members_30_days_total"] = $query->total;

  // NEW MEMBERS PAST 90 DAYS
  $query_sql = "SELECT COUNT(*) total FROM {flagging} f WHERE f.fid = :fid AND f.entity_type = 'node' AND f.entity_id IN (:chapter_ids) AND timestamp >= :time_ago";
  $query = db_query($query_sql, array(':fid' => $flag->fid, ':chapter_ids' => $chapter_ids, ':time_ago' => $days_ago_90))->fetch();
  $insights["members_90_days_total"] = $query->total;

   // MEMBERSHIP GROWTH % PAST 30 DAYS
  $query_sql = "SELECT COUNT(*) total FROM {flagging} f WHERE f.fid = :fid AND f.entity_type = 'node' AND f.entity_id IN (:chapter_ids) AND timestamp < :time_ago";
  $query = db_query($query_sql, array(':fid' => $flag->fid, ':chapter_ids' => $chapter_ids, ':time_ago' => $days_ago_30))->fetch();
  $members_30_days_ago = $query->total;
  $members_30_days_pct = 0;
  if ($members_30_days_ago > 0) {      
    $members_30_days_pct = round((($insights["members_30_days_total"] / $members_30_days_ago) * 100), 0);
  }
  $insights["members_30_days_pct"] = $members_30_days_pct;

  // MEMBERSHIP GROWTH % PAST 90 DAYS
  $query_sql = "SELECT COUNT(*) total FROM {flagging} f WHERE f.fid = :fid AND f.entity_type = 'node' AND f.entity_id IN (:chapter_ids) AND timestamp < :time_ago";
  $query = db_query($query_sql, array(':fid' => $flag->fid, ':chapter_ids' => $chapter_ids, ':time_ago' => $days_ago_90))->fetch();
  $members_90_days_ago = $query->total;
  $members_90_days_pct = 0;
  if ($members_90_days_ago > 0) {      
    $members_90_days_pct = round((($insights["members_90_days_total"] / $members_90_days_ago) * 100), 0);
  }
  $insights["members_90_days_pct"] = $members_90_days_pct;

  // TOTAL MEMBERS
  $query_sql = "SELECT COUNT(*) total FROM {flagging} f WHERE f.fid = :fid AND f.entity_type = 'node' AND f.entity_id IN (:chapter_ids)";
  $query = db_query($query_sql, array(':fid' => $flag->fid, ':chapter_ids' => $chapter_ids))->fetch();
  $members_total = $query->total;
  if ($members_total == 0) {
    $members_total = 1; // prevent div0
  }
  $insights["members_total"] = $members_total;

  // TOTAL APPROVED MEMBERS
  $query_sql = "SELECT COUNT(DISTINCT users_roles.uid) total FROM {flagging} f RIGHT JOIN users_roles ON users_roles.uid = f.uid WHERE users_roles.rid = :role_id AND f.fid = :fid AND f.entity_type = 'node' AND f.entity_id IN (:chapter_ids)";
  $query = db_query($query_sql, array(':fid' => $flag->fid, ':chapter_ids' => $chapter_ids, ':role_id' => $approved_role->rid))->fetch();
  $members_approved_total = $query->total;
  $insights["members_approved_total"] = $members_approved_total;
  if ($members_approved_total == 0) { 
    $members_approved_total = 1; // prevent division by zero
  }

  // TOTAL UNAPPROVED MEMBERS
  $members_unapproved_total = $members_total - $members_approved_total;
  $insights["members_unapproved_total"] = $members_unapproved_total;  

  // % APPROVED MEMBERS
  $members_unapproved_pct = 0;
  if (($members_total > 0) && ($members_approved_total > 0)) {
    $members_unapproved_pct = round((($members_approved_total / $members_total) * 100), 0);
  }
  $insights["members_approved_pct"] = $members_unapproved_pct;

  // % UNAPPROVED MEMBERS
  $members_unapproved_pct = 0;
  if (($members_total > 0) && ($members_unapproved_total > 0)) {
    $members_unapproved_pct = round((($members_unapproved_total / $members_total) * 100), 0);
  }
  $insights["members_unapproved_pct"] = $members_unapproved_pct;

  // NUM MEMBERS WITH A SEGMENT
  $query_sql = "SELECT COUNT(DISTINCT users_roles.uid) total FROM {flagging} f
  LEFT JOIN users_roles ON users_roles.uid = f.uid 
  RIGHT JOIN field_data_field_segment on f.uid = field_data_field_segment.entity_id 
  WHERE field_data_field_segment.entity_id is not null
  AND users_roles.rid = :role_id
  AND f.fid = :fid AND f.entity_type = 'node' AND f.entity_id IN (:chapter_ids)";
  $query = db_query($query_sql, array(':fid' => $flag->fid, ':chapter_ids' => $chapter_ids, ':role_id' => $approved_role->rid))->fetch();
  $insights["members_approved_with_segment"] = $query->total;

  // NUM MEMBERS WITHOUT A SEGMENT
  $insights["members_approved_without_segment"] = $insights["members_approved_total"] - $insights["members_approved_with_segment"];

  // % MEMBERS WITH AN INDUSTRY SEGMENT
  $members_with_segment_pct = 0;
  if (($insights["members_approved_with_segment"] > 0) && ($members_approved_total > 0)) {
    $members_with_segment_pct = round((($insights["members_approved_with_segment"] / $members_approved_total) * 100), 0);
  }
  $insights["members_with_segment_pct"] = $members_with_segment_pct;

  // NUMBER OF SIGNED IN MEMBERS TODAY (APPROVED)
  $query = "SELECT COUNT(DISTINCT users.uid) amount FROM users RIGHT JOIN users_roles ON users_roles.uid = users.uid RIGHT JOIN {flagging} f ON f.uid = users.uid WHERE users_roles.rid = :role_id AND FROM_UNIXTIME(access) > DATE_SUB(NOW(), INTERVAL 1 DAY) AND f.fid = :fid AND f.entity_type = 'node' AND f.entity_id IN (:chapter_ids) AND f.fid = :fid AND f.entity_type = 'node' AND f.entity_id IN (:chapter_ids)";
  $signed_in_today_chapter = db_query($query, array(':fid' => $flag->fid, ':chapter_ids' => $chapter_ids, ':role_id' => $approved_role->rid))->fetch();
  $insights["num_signedin_members_24_hours_chapter"] = $signed_in_today_chapter->amount;

  // NUMBER OF SIGNED IN MEMBERS 7 DAYS (APPROVED)
  $query = "SELECT COUNT(DISTINCT users.uid) amount FROM users RIGHT JOIN users_roles ON users_roles.uid = users.uid RIGHT JOIN {flagging} f ON f.uid = users.uid WHERE users_roles.rid = :role_id AND FROM_UNIXTIME(access) > DATE_SUB(NOW(), INTERVAL 7 DAY) AND f.fid = :fid AND f.entity_type = 'node' AND f.entity_id IN (:chapter_ids)";
  $signed_in_7_days_chapter = db_query($query, array(':fid' => $flag->fid, ':chapter_ids' => $chapter_ids, ':role_id' => $approved_role->rid))->fetch();
  $insights["num_signedin_members_7_days_chapter"] = $signed_in_7_days_chapter->amount;

  // NUMBER OF SIGNED IN MEMBERS 30 DAYS (APPROVED)
  $query = "SELECT COUNT(DISTINCT users.uid) amount FROM users RIGHT JOIN users_roles ON users_roles.uid = users.uid RIGHT JOIN {flagging} f ON f.uid = users.uid WHERE users_roles.rid = :role_id AND FROM_UNIXTIME(access) > DATE_SUB(NOW(), INTERVAL 30 DAY) AND f.fid = :fid AND f.entity_type = 'node' AND f.entity_id IN (:chapter_ids)";
  $signed_in_30_days_chapter = db_query($query, array(':fid' => $flag->fid, ':chapter_ids' => $chapter_ids, ':role_id' => $approved_role->rid))->fetch();
  $insights["num_signedin_members_30_days_chapter"] = $signed_in_30_days_chapter->amount;

  // NUMBER OF SIGNED IN MEMBERS 90 DAYS (APPROVED)
  $query = "SELECT COUNT(DISTINCT users.uid) amount FROM users RIGHT JOIN users_roles ON users_roles.uid = users.uid RIGHT JOIN {flagging} f ON f.uid = users.uid WHERE users_roles.rid = :role_id AND FROM_UNIXTIME(access) > DATE_SUB(NOW(), INTERVAL 90 DAY) AND f.fid = :fid AND f.entity_type = 'node' AND f.entity_id IN (:chapter_ids)";
  $signed_in_90_days_chapter = db_query($query, array(':fid' => $flag->fid, ':chapter_ids' => $chapter_ids, ':role_id' => $approved_role->rid))->fetch();
  $insights["num_signedin_members_90_days_chapter"] = $signed_in_90_days_chapter->amount;

  // NUMBER OF MEMBERS WHO HAVEN'T SIGNED IN SINCE 90 DAYS (APPROVED)
  $query = "SELECT COUNT(DISTINCT users.uid) amount FROM users RIGHT JOIN users_roles ON users_roles.uid = users.uid RIGHT JOIN {flagging} f ON f.uid = users.uid WHERE users_roles.rid = :role_id AND FROM_UNIXTIME(access) < DATE_SUB(NOW(), INTERVAL 90 DAY) AND f.fid = :fid AND f.entity_type = 'node' AND f.entity_id IN (:chapter_ids)";
  $signed_in_older_than_90_days_chapter = db_query($query, array(':fid' => $flag->fid, ':chapter_ids' => $chapter_ids, ':role_id' => $approved_role->rid))->fetch();
  $insights["num_signedin_members_older_than_90_days_chapter"] = $signed_in_older_than_90_days_chapter->amount;

  // % OF SIGNED IN MEMBERS TODAY
  $insights["pct_signedin_members_24_hours_chapter"] = round(($signed_in_today_chapter->amount / $members_approved_total) * 100, 0);

  // % OF SIGNED IN MEMBERS 7 DAYS
  $insights["pct_signedin_members_7_days_chapter"] = round(($signed_in_7_days_chapter->amount / $members_approved_total) * 100, 0);

  // % OF SIGNED IN MEMBERS 30 DAYS
  $insights["pct_signedin_members_30_days_chapter"] = round(($signed_in_30_days_chapter->amount / $members_approved_total) * 100, 0);

  // % OF SIGNED IN MEMBERS 90 DAYS
  $insights["pct_signedin_members_90_days_chapter"] = round(($signed_in_90_days_chapter->amount / $members_approved_total) * 100, 0);

  // % OF MEMBERS WHO HAVEN'T SIGNED IN SINCE 90 DAYS
  $insights["pct_signedin_members_older_than_90_days_chapter"] = (100 - round(($signed_in_90_days_chapter->amount / $members_approved_total) * 100, 0));

  // NUMBER OF MEMBERS WITH HOME CHAPTER
  $query = "SELECT COUNT(*) AS amount FROM flagging f RIGHT JOIN field_data_field_home_chapter hc ON hc.entity_id = f.uid WHERE f.fid = :fid AND f.entity_id IN (:chapter_ids) AND hc.field_home_chapter_target_id IN (:chapter_ids)";
  $num_members_with_home_chapter = db_query($query, array(':fid' => $flag->fid, ':chapter_ids' => $chapter_ids))->fetch();
  $insights["num_members_with_home_chapter"] = $num_members_with_home_chapter->amount;
  $insights["pct_members_with_home_chapter"] = round(($insights["num_members_with_home_chapter"] / $members_total) * 100, 0);
  $insights["num_members_without_home_chapter"] = $members_total - $num_members_with_home_chapter->amount;
  $insights["pct_members_without_home_chapter"] = round(($insights["num_members_without_home_chapter"] / $members_total) * 100, 0);

  // NUMBER OF TOTAL EVENT REGISTRATIONS
  $flag_event_register = flag_get_flag('event_register', NULL);
  $query = "SELECT COUNT(uid) AS amount FROM flagging WHERE entity_id IN (SELECT entity_id AS nid FROM field_data_field_chapter WHERE bundle = 'event' AND field_chapter_target_id IN (:chapter_ids)) AND fid = :fid";
  $num_event_registrations = db_query($query, array(':fid' => $flag_event_register->fid, ':chapter_ids' => $chapter_ids))->fetch();
  $insights["num_event_registrations"] = $num_event_registrations->amount;

  // NUMBER OF TOTAL EVENT WAITLISTS
  $flag_event_waitlist = flag_get_flag('event_waitlist', NULL);
  $query = "SELECT COUNT(uid) AS amount FROM flagging WHERE entity_id IN (SELECT entity_id AS nid FROM field_data_field_chapter WHERE bundle = 'event' AND field_chapter_target_id IN (:chapter_ids)) AND fid = :fid";
  $num_event_waitlists = db_query($query, array(':fid' => $flag_event_waitlist->fid, ':chapter_ids' => $chapter_ids))->fetch();
  $insights["num_event_waitlists"] = $num_event_waitlists->amount;

  // NUMBER OF TOTAL EVENT PAYMENTS
  $flag_event_paid = flag_get_flag('event_paid', NULL);
  $query = "SELECT COUNT(uid) AS amount FROM flagging WHERE entity_id IN (SELECT entity_id AS nid FROM field_data_field_chapter WHERE bundle = 'event' AND field_chapter_target_id IN (:chapter_ids)) AND fid = :fid";
  $num_event_payments = db_query($query, array(':fid' => $flag_event_paid->fid, ':chapter_ids' => $chapter_ids))->fetch();
  if ($num_event_payments->amount > 0) {
    $insights["num_event_payments"] = $num_event_payments->amount;
  } else {
    $insights["num_event_payments"] = 0;
  }

  // NUMBER OF UNIQUE EVENT REGISTRATIONS
  $query = "SELECT COUNT(DISTINCT(uid)) AS amount FROM flagging WHERE entity_id IN (SELECT entity_id AS nid FROM field_data_field_chapter WHERE bundle = 'event' AND field_chapter_target_id IN (:chapter_ids)) AND fid = :fid";
  $num_unique_chapter_event_registrations = db_query($query, array(':fid' => $flag_event_register->fid, ':chapter_ids' => $chapter_ids))->fetch();
  $insights["num_unique_chapter_event_registrations"] = $num_unique_chapter_event_registrations->amount;
  $insights["pct_unique_chapter_event_registrations"] = round(($insights["num_unique_chapter_event_registrations"] / $members_approved_total) * 100, 0);

  // FREQUENCY OF EVENTS
  $flag = flag_get_flag('event_register', NULL);
  $query_sql = "SELECT AVG(amount) AS total FROM (SELECT uid, COUNT(uid) AS amount FROM flagging f WHERE entity_id IN (SELECT entity_id AS nid FROM field_data_field_chapter WHERE bundle = 'event' AND field_chapter_target_id IN (:chapter_ids)) AND f.fid = :fid GROUP BY uid) AS t2";
  $query = db_query($query_sql, array("chapter_ids" => $chapter_ids, ":fid" => $flag->fid))->fetch();
  $avg_event_registeration_frequency = round($query->total, 1);
  $insights['avg_event_registeration_frequency'] = $avg_event_registeration_frequency;

  // NUMBER OF CONNECTIONS BETWEEN MEMBERS IN THE CHAPTER
  $follow_flag = flag_get_flag('follow_members', NULL);
  $follow_organizations_flag = flag_get_flag('follow_organizations', NULL);
  $signup_flag = flag_get_flag('signup', NULL);

  // get outbound follows (total follows from chapter members)
  $query_sql = "SELECT COUNT(uid) AS total FROM flagging f WHERE f.fid = :follow_fid AND f.uid IN (SELECT uid FROM flagging f2 WHERE f2.entity_id IN (:chapter_ids) AND f2.fid = :signup_fid)";
  $query = db_query($query_sql, array(":chapter_ids" => $chapter_ids, ":follow_fid" => $follow_flag->fid, ":signup_fid" => $signup_flag->fid))->fetch();
  $total_chapter_member_connections_outbound = $query->total;

  // get inbound follows (total following of chapter members)
  $query_sql = "SELECT SUM(fc.count) AS total FROM flag_counts fc WHERE fc.fid = :follow_fid AND entity_id IN (SELECT uid FROM flagging f2 WHERE f2.entity_id IN (:chapter_ids) AND f2.fid = :signup_fid)";
  $query = db_query($query_sql, array(":chapter_ids" => $chapter_ids, ":follow_fid" => $follow_flag->fid, ":signup_fid" => $signup_flag->fid))->fetch();
  $total_chapter_member_connections_inbound = $query->total;

  // chapter members following organizations
  $query_sql = "SELECT COUNT(uid) AS total FROM flagging f WHERE f.fid = :follow_fid AND f.uid IN (SELECT uid FROM flagging f2 WHERE f2.entity_id IN (:chapter_ids) AND f2.fid = :signup_fid)";
  $query = db_query($query_sql, array(":chapter_ids" => $chapter_ids, ":follow_fid" => $follow_organizations_flag->fid, ":signup_fid" => $signup_flag->fid))->fetch();
  $total_chapter_member_connections_organizations = $query->total;

  $total_chapter_member_connections = $total_chapter_member_connections_outbound + $total_chapter_member_connections_inbound + $total_chapter_member_connections_organizations;
  $insights['total_chapter_member_connections'] = $total_chapter_member_connections;

  // RETURN DATA
  $data_labels = tm_reports_get_insights_labels();
  $results = ["data_values" => $insights, "data_labels" => $data_labels];
  cache_set('tm-reports-get-chapter-insights-' . $chapter_ids_key, $results, 'cache', time() + 900); // 15 mins
  cache_set('tm-reports-get-chapter-insights-display-' . $chapter_ids_key, $results, 'cache'); // uptdate permanent display cache for public chapter pages

  return $results;
}

