<?php

// tm_reports.global.inc - global report methods

/**
 * Generate global insights
 */
function tm_reports_global_insights() {

  global $user;
  global $conf;

  $segment_label = "Industry Segment";
  if (isset($conf["tm_segment_label"])) {
    $segment_label = $conf["tm_segment_label"];
  }

  $flag_signup = flag_get_flag('signup', NULL);
  //$return_link = l(t('Community Reports'), url('admin/tm_reports', array('absolute' => TRUE)));

  // Generate HTML
  $html = _tm_reports_insights_js_css_header();
  $redraw_functions = array("drawChart", "draw_new_signups", "draw_event_registrations", "draw_event_payments", "draw_follow_members", "draw_follow_organizations", "draw_monthly_chapters", "draw_status_updates", "draw_links_shared", "draw_insights_event_registration_frequency", "draw_insights_event_registration_frequency_aggregate");
  $html .= _tm_reports_insights_js_redraw_function($redraw_functions);

  // Start artlce
  $html .= "<article class='trilithon node view-mode-full node-by-viewer clearfix'>";

  // Insights
  $html .= "<section class='contained contained-block'>";
  $html .= "<header class='contained-head'>
            <h1 class='prime-title'>Global Insights</span></h1>"; // <!--&middot; <span style='color: #0068a8;'>" . $return_link . "</span></h1>
  $html .= "</header>";
  $html .= _tm_reports_insights_table_html();
  $html .= "</section>";

  // New Members
  $html .= "<section class='contained contained-block'>";
  $html .= "<header class='contained-head'>
            <h1 class='prime-title'>New Members</h1>
            </header>";
  $html .= _tm_reports_insights_all_signups_html();
  $html .= "</section>";

  // Event Registrations
  $html .= "<section class='contained contained-block'>";
  $html .= "<header class='contained-head'>
            <h1 class='prime-title'>Event Registrations</h1>
            </header>";
  $html .= _tm_reports_insights_all_event_registrations_html();
  $html .= "</section>";

  // Event Payments
  $html .= "<section class='contained contained-block'>";
  $html .= "<header class='contained-head'>
            <h1 class='prime-title'>Event Payments</h1>
            </header>";
  $html .= _tm_reports_insights_all_event_payments_html();
  $html .= "</section>";

  // Member Follows
  $html .= "<section class='contained contained-block'>";
  $html .= "<header class='contained-head'>
            <h1 class='prime-title'>Member Connections</h1>
            </header>";
  $html .= _tm_reports_insights_all_member_connections_html();
  $html .= "</section>";

  // Organzation Follows
  $html .= "<section class='contained contained-block'>";
  $html .= "<header class='contained-head'>
            <h1 class='prime-title'>Organization Connections</h1>
            </header>";
  $html .= _tm_reports_insights_all_organization_connections_html();
  $html .= "</section>";

  // New Chapters
  $html .= "<section class='contained contained-block'>";
  $html .= "<header class='contained-head'>
            <h1 class='prime-title'>Total Chapters</h1>
            </header>";
  $html .= _tm_reports_insights_monthly_chapters_html();
  $html .= "</section>";

  // Status updates
  $html .= "<section class='contained contained-block'>";
  $html .= "<header class='contained-head'>
            <h1 class='prime-title'>Status Updates</h1>
            </header>";
  $html .= _tm_reports_insights_monthly_status_updates_html();
  $html .= "</section>";

  // Links shared
  $html .= "<section class='contained contained-block'>";
  $html .= "<header class='contained-head'>
            <h1 class='prime-title'>Links Shared</h1>
            </header>";
  $html .= _tm_reports_insights_monthly_links_shared_html();
  $html .= "</section>";

  // Registration frequency
  $html .= "<section class='contained contained-block'>";
  $html .= "<header class='contained-head'>
            <h1 class='prime-title'>Number of Events Members Have Attended</h1>
            </header>";
  $html .= _tm_reports_insights_event_registration_frequency_html(null, null, true);
  $html .= "</section>";

  // Registration frequency
  $html .= "<section class='contained contained-block'>";
  $html .= "<header class='contained-head'>
            <h1 class='prime-title'>Number of Events Members Have Attended (Not Aggregated)</h1>
            </header>";
  $html .= _tm_reports_insights_event_registration_frequency_html(null, null, false);
  $html .= "</section>";

  // Segments
  $html .= "<section class='contained contained-block'>";
  $html .= "<header class='contained-head'>
            <h1 class='prime-title'>" . $segment_label . "</h1>
            </header>";
  $html .= _tm_reports_insights_segments_html();
  $html .= "</section>";

  // End article
  $html .= "</article>";

  $form['html'] =  array(
    '#type' => 'markup',
    '#markup' => $html);

  return $form;
}

/**
 * Calculate global stats
 * NOTE: Cached
 */
function _tm_reports_get_global_insights() {

  // check cache
  $cache = cache_get('tm-reports-get-global-insights', 'cache');
  if (!empty($cache)) {
    return $cache->data;
  }

  $insights = array();

  $approved_role = user_role_load_by_name("approved user");
  $days_ago_30 = time() - (60 * 60 * 24 * 30); // 30 days ago
  $days_ago_90 = time() - (60 * 60 * 24 * 90); // 90 days ago
  
  // NUMBER OF FUTURE EVENTS
  $query_sql = "SELECT COUNT(*) total FROM {field_data_field_event_date} e 
  WHERE e.entity_type = 'node' AND field_event_date_value >= NOW();";
  $query = db_query($query_sql)->fetch();
  $insights["num_events_upcoming"] = $query->total;

  // NUMBER OF EVENTS PAST MONTH
  $query_sql = "SELECT COUNT(*) total FROM {field_data_field_event_date} e 
  LEFT JOIN field_revision_field_is_partner_event p on e.entity_id = p.entity_id
  WHERE p.entity_type = 'node' AND field_is_partner_event_value = 0
  AND e.entity_type = 'node' AND field_event_date_value >= DATE_SUB(NOW(),INTERVAL 30 DAY);";
  $query = db_query($query_sql)->fetch();
  $insights["num_events_past_30_days"] = $query->total;

  // NUMBER OF EVENTS PAST YEAR
  $query_sql = "SELECT COUNT(*) total FROM {field_data_field_event_date} e 
  LEFT JOIN field_revision_field_is_partner_event p on e.entity_id = p.entity_id
  WHERE p.entity_type = 'node' AND field_is_partner_event_value = 0
  AND e.entity_type = 'node' AND field_event_date_value >= DATE_SUB(NOW(),INTERVAL 1 YEAR);";
  $query = db_query($query_sql)->fetch();
  $insights["num_events_past_12_months"] = $query->total;

  // NUMBER OF PARTNER EVENTS PAST MONTH
  $query_sql = "SELECT COUNT(*) total FROM {field_data_field_event_date} e 
  LEFT JOIN field_revision_field_is_partner_event p on e.entity_id = p.entity_id
  WHERE p.entity_type = 'node' AND field_is_partner_event_value = 1
  AND e.entity_type = 'node' AND field_event_date_value >= DATE_SUB(NOW(),INTERVAL 30 DAY);";
  $query = db_query($query_sql)->fetch();
  $insights["num_partner_events_past_30_days"] = $query->total;

  // NUMBER OF PARTNER EVENTS PAST YEAR
  $query_sql = "SELECT COUNT(*) total FROM {field_data_field_event_date} e 
  LEFT JOIN field_revision_field_is_partner_event p on e.entity_id = p.entity_id
  WHERE p.entity_type = 'node' AND field_is_partner_event_value = 1
  AND e.entity_type = 'node' AND field_event_date_value >= DATE_SUB(NOW(),INTERVAL 1 YEAR);";
  $query = db_query($query_sql)->fetch();
  $insights["num_partner_events_past_12_months"] = $query->total;

  // ALL NEW MEMBERS PAST 30 DAYS
  $query_sql = "SELECT COUNT(*) total FROM {users} u WHERE created >= :time_ago";
  $query = db_query($query_sql, array(':time_ago' => $days_ago_30))->fetch();
  $members_30_days_total = $query->total;
  $insights["members_30_days_total"] = $members_30_days_total;

  // ALL NEW MEMBERS PAST 90 DAYS
  $query_sql = "SELECT COUNT(*) total FROM {users} u WHERE created >= :time_ago";
  $query = db_query($query_sql, array(':time_ago' => $days_ago_90))->fetch();
  $members_90_days_total = $query->total;
  $insights["members_90_days_total"] = $members_90_days_total;

  // ALL MEMBERSHIP GROWTH % PAST 30 DAYS
  $query_sql = "SELECT COUNT(*) total FROM {users} u WHERE created < :time_ago";
  $query = db_query($query_sql, array(':time_ago' => $days_ago_30))->fetch();
  $members_30_days_ago = $query->total;
  $members_30_days_pct = 0;
  if ($members_30_days_ago > 0) {      
    $members_30_days_pct = round((($members_30_days_total / $members_30_days_ago) * 100), 0);
  }
  $insights["members_30_days_pct"] = $members_30_days_pct;

  // ALL MEMBERSHIP GROWTH % PAST 90 DAYS
  $query_sql = "SELECT COUNT(*) total FROM {users} u WHERE created < :time_ago";
  $query = db_query($query_sql, array(':time_ago' => $days_ago_90))->fetch();
  $members_90_days_ago = $query->total;
  $members_90_days_pct = 0;
  if ($members_90_days_ago > 0) {      
    $members_90_days_pct = round((($members_90_days_total / $members_90_days_ago) * 100), 0);
  }
  $insights["members_90_days_pct"] = $members_90_days_pct;
  
  // ALL MEMBERS
  $query_sql = "SELECT COUNT(*) total FROM {users} u";
  $query = db_query($query_sql)->fetch();
  $members_total = $query->total;
  $insights["members_total"] = $members_total;

  // ALL APPROVED MEMBERS
  $query_sql = "SELECT COUNT(DISTINCT u.uid) total FROM {users} u RIGHT JOIN users_roles ON users_roles.uid = u.uid WHERE users_roles.rid = :role_id";
  $query = db_query($query_sql, array(':role_id' => $approved_role->rid))->fetch();
  $members_approved_total = $query->total;
  $insights["members_approved_total"] = $members_approved_total;

  // TOTAL UNAPPROVED MEMBERS
  $members_unapproved_total = $members_total - $members_approved_total;
  $insights["members_unapproved_total"] = $members_unapproved_total;

  // % UNAPPROVED MEMBERS
  $members_unapproved_pct = round((($members_unapproved_total / $members_total) * 100), 0);
  $insights["members_unapproved_pct"] = $members_unapproved_pct;

  // NUM MEMBERS WITH A SEGMENT
  $query_sql = "SELECT COUNT(DISTINCT users.uid) total FROM users
  LEFT JOIN users_roles ON users_roles.uid = users.uid 
  RIGHT JOIN field_data_field_segment on users.uid = field_data_field_segment.entity_id 
  WHERE field_data_field_segment.entity_id is not null
  AND users_roles.rid = :role_id";
  $query = db_query($query_sql, array(':role_id' => $approved_role->rid))->fetch();
  $insights["members_approved_with_segment"] = $query->total;

  // NUM MEMBERS WITHOUT A SEGMENT
  $insights["members_approved_without_segment"] = $insights["members_approved_total"] - $insights["members_approved_with_segment"];

  // % MEMBERS WITH AN INDUSTRY SEGMENT
  $members_with_segment_pct = 0;
  if (($insights["members_approved_with_segment"] > 0) && ($members_approved_total > 0)) {
    $members_with_segment_pct = round((($insights["members_approved_with_segment"] / $members_approved_total) * 100), 0);
  }
  $insights["members_with_segment_pct"] = $members_with_segment_pct;

  // NUM CHAPTERS
  $query = "SELECT COUNT(*) amount FROM {node} n ".
              "WHERE n.type = :type";
  $num_chapters = db_query($query, array(':type' => "chapter"))->fetch();
  $insights["num_chapters"] = $num_chapters->amount;

  // NUM CHAPTER LEADERS
  $chapter_leader_role = user_role_load_by_name("chapter leader");
  $query = "SELECT COUNT(*) amount FROM {users_roles} r ".
              "WHERE r.rid = :rid";
  $num_chapter_leaders = db_query($query, array(':rid' => $chapter_leader_role->rid))->fetch();
  $insights["num_chapter_leaders"] = $num_chapter_leaders->amount;

  // NUMBER OF ORGANIZATIONS
  $query = "SELECT COUNT(*) amount FROM {node} n ".
              "WHERE n.type = :type";
  $num_organizations = db_query($query, array(':type' => "organization"))->fetch();
  $insights["num_organizations"] = $num_organizations->amount;

  // NUMBER OF EVENTS
  $query = "SELECT COUNT(*) amount FROM {node} n ".
              "WHERE n.type = :type";
  $num_events = db_query($query, array(':type' => "event"))->fetch();
  $insights["num_events"] = $num_events->amount;
 
  // NUMBER OF CHAPTER COUNTRIES
  $result = db_query("SELECT DISTINCT iso2, name from {field_data_field_country} f LEFT JOIN {countries_country} c ON f.field_country_iso2 = c.iso2 WHERE f.bundle = 'chapter' AND c.iso2 IS NOT NULL ORDER BY name ASC");
  $chapter_countries = $result->fetchAllKeyed();
  $num_chapter_countries = (sizeof($chapter_countries));
  $insights["num_chapter_countries"] = $num_chapter_countries;

  // NUMBER OF MEMBER COUNTRIES
  $result = db_query("SELECT DISTINCT field_user_country_iso2 from {field_data_field_user_country} ORDER BY field_user_country_iso2 ASC");
  $member_countries = $result->fetchAll();
  $num_member_countries = (sizeof($member_countries));
  $insights["num_member_countries"] = $num_member_countries;

  // NUMBER OF FOLLOWS
  $flag = flag_get_flag('follow_members', NULL);
  $query = "SELECT COUNT(*) amount FROM {flagging} f WHERE f.fid = :fid";
  $num_member_followers = db_query($query, array(':fid' => $flag->fid))->fetch();
  $insights["num_member_followers"] = $num_member_followers->amount;

  // NUMBER OF COMPANY FOLLOWS
  $flag = flag_get_flag('follow_organizations', NULL);
  $query = "SELECT COUNT(*) amount FROM {flagging} f WHERE f.fid = :fid";
  $num_organization_followers = db_query($query, array(':fid' => $flag->fid))->fetch();
  $insights["num_organization_followers"] = $num_organization_followers->amount;

  // NUMBER OF CONNECTIONS (member + organization follows)
  $insights["num_connections"] = $insights["num_member_followers"] + $insights["num_organization_followers"];

  // NUMBER OF MUTUAL FOLLOWS
  $flag = flag_get_flag('follow_members', NULL);
  $query = "select count(distinct flagging.uid, f2.uid) as amount from flagging inner join flagging as f2 on flagging.uid = f2.entity_id where flagging.fid = :fid and f2.fid = :fid order by flagging.uid";
  $num_mutual_followers = db_query($query, array(':fid' => $flag->fid))->fetch();
  $insights["num_mutual_follows"] = $num_mutual_followers->amount;

  // NUMBER OF EVENT REGISTRATIONS
  $flag = flag_get_flag('event_register', NULL);
  $query = "SELECT COUNT(*) amount FROM {flagging} f WHERE f.fid = :fid";
  $num_registrations = db_query($query, array(':fid' => $flag->fid))->fetch();
  $insights["num_event_registrations"] = $num_registrations->amount;

  // NUMBER OF WAITLISTS
  $flag = flag_get_flag('event_waitlist', NULL);
  $query = "SELECT COUNT(*) amount FROM {flagging} f WHERE f.fid = :fid";
  $num_waitlists = db_query($query, array(':fid' => $flag->fid))->fetch();
  $insights["num_event_waitlists"] = $num_waitlists->amount;

   // NUMBER OF EVENT PAYMENTS
  $flag = flag_get_flag('event_paid', NULL);
  $query = "SELECT COUNT(*) amount FROM {flagging} f WHERE f.fid = :fid";
  $num_registrations = db_query($query, array(':fid' => $flag->fid))->fetch();
  $insights["num_event_payments"] = $num_registrations->amount;

  // NUMBER OF UNIQUE EVENT REGISTRATIONS
  $flag = flag_get_flag('event_register', NULL);
  $query = "SELECT COUNT(DISTINCT uid) amount FROM {flagging} f WHERE f.fid = :fid";
  $num_registrations = db_query($query, array(':fid' => $flag->fid))->fetch();
  $insights["num_unique_event_registrations"] = $num_registrations->amount;
  $insights["pct_unique_event_registrations"] = round(($insights["num_unique_event_registrations"] / $members_approved_total) * 100, 0);

  // FREQUENCY OF EVENTS
  $flag = flag_get_flag('event_register', NULL);
  $query_sql = "SELECT AVG(amount) as total FROM (SELECT uid, COUNT(uid) AS amount FROM flagging f WHERE f.fid = :fid GROUP BY uid) as t2";
  $query = db_query($query_sql, array(":fid" => $flag->fid))->fetch();
  $avg_event_registeration_frequency = round($query->total, 2);
  $insights['avg_event_registeration_frequency'] = $avg_event_registeration_frequency;

  // NUMBER OF UNIQUE WAITLISTS
  $flag = flag_get_flag('event_waitlist', NULL);
  $query = "SELECT COUNT(DISTINCT uid) amount FROM {flagging} f WHERE f.fid = :fid";
  $num_waitlists = db_query($query, array(':fid' => $flag->fid))->fetch();
  $insights["num_unique_event_waitlists"] = $num_waitlists->amount;

   // NUMBER OF UNIQUE EVENT PAYMENTS
  $flag = flag_get_flag('event_paid', NULL);
  $query = "SELECT COUNT(DISTINCT uid) amount FROM {flagging} f WHERE f.fid = :fid";
  $num_registrations = db_query($query, array(':fid' => $flag->fid))->fetch();
  $insights["num_unique_event_payments"] = $num_registrations->amount;

  // NUMBER OF PEOPLE WHO HAVE SENT MESSAGES
  $flag = flag_get_flag('message_sent', NULL);
  $query = "SELECT COUNT(DISTINCT uid) amount FROM {flagging} f WHERE f.fid = :fid";
  $num_waitlists = db_query($query, array(':fid' => $flag->fid))->fetch();
  $insights["num_members_sent_messages"] = $num_waitlists->amount;

  // TOTAL NUMBER OF STATUS UPDATES
  $query = "SELECT COUNT(id) amount FROM {tm_newsfeed_status_updates} WHERE is_removed = 0";
  $num_updates = db_query($query)->fetch();
  $insights["num_total_status_updates"] = $num_updates->amount;

  // TOTAL NUMBER OF PROMOTED UPDATES
  $query = "SELECT COUNT(id) amount FROM {tm_newsfeed_status_updates} WHERE is_promoted = 1 AND is_removed = 0";
  $num_promoted = db_query($query)->fetch();
  $insights["num_promoted_status_updates"] = $num_promoted->amount;

  // TOTAL NUMBER OF MODERATED UPDATES
  $query = "SELECT COUNT(id) amount FROM {tm_newsfeed_status_updates} WHERE moderator_hide = 1";
  $num_moderated = db_query($query)->fetch();
  $insights["num_moderated_status_updates"] = $num_moderated->amount;

  // TOTAL NUMBER OF LINKS SHARED
  $query = "SELECT COUNT(id) amount FROM {tm_newsfeed_status_updates} WHERE is_removed = 0 AND preview_link_id IS NOT NULL";
  $num_links = db_query($query)->fetch();
  $insights["num_total_status_links"] = $num_links->amount;

  // TOTAL NUMBER OF PREVIEW LINKS CREATED
  $query = "SELECT COUNT(id) amount FROM {tm_newsfeed_preview_links}";
  $preview_links_created = db_query($query)->fetch();
  $insights["num_total_preview_links_created"] = $preview_links_created->amount;

  // TOTAL NUMBER OF UPDATE VIEWS DAILY
  $query = "SELECT SUM(view_count) amount FROM {tm_newsfeed_view_count_daily}";
  $view_count_daily = db_query($query)->fetch();
  $insights["num_total_status_views_daily"] = $view_count_daily->amount;

  // TOTAL NUMBER OF STATUS UPDATE VIEWS
  $query = "SELECT SUM(total_view_count) amount FROM {tm_newsfeed_view_count_total}";
  $view_count = db_query($query)->fetch();
  $insights["num_total_status_views_all_time"] = $view_count->amount + $view_count_daily->amount;

  // NUMBER OF SIGNED IN MEMBERS TODAY (ALL)
  $query = "SELECT COUNT(DISTINCT uid) amount FROM users WHERE FROM_UNIXTIME(access) > DATE_SUB(NOW(), INTERVAL 1 DAY)";
  $signed_in_today = db_query($query)->fetch();
  $insights["num_signedin_members_24_hours"] = $signed_in_today->amount;

  // NUMBER OF SIGNED IN MEMBERS 7 DAYS (ALL)
  $query = "SELECT COUNT(DISTINCT uid) amount FROM users WHERE FROM_UNIXTIME(access) > DATE_SUB(NOW(), INTERVAL 7 DAY)";
  $signed_in_7_days = db_query($query)->fetch();
  $insights["num_signedin_members_7_days"] = $signed_in_7_days->amount;

  // NUMBER OF SIGNED IN MEMBERS 30 DAYS (ALL)
  $query = "SELECT COUNT(DISTINCT uid) amount FROM users WHERE FROM_UNIXTIME(access) > DATE_SUB(NOW(), INTERVAL 30 DAY)";
  $signed_in_30_days = db_query($query)->fetch();
  $insights["num_signedin_members_30_days"] = $signed_in_30_days->amount;

  // NUMBER OF SIGNED IN MEMBERS 90 DAYS (ALL)
  $query = "SELECT COUNT(DISTINCT uid) amount FROM users WHERE FROM_UNIXTIME(access) > DATE_SUB(NOW(), INTERVAL 90 DAY)";
  $signed_in_90_days = db_query($query)->fetch();
  $insights["num_signedin_members_90_days"] = $signed_in_90_days->amount;

  // NUMBER OF MEMBERS WHO HAVEN'T SIGNED IN SINCE 90 DAYS (ALL)
  $query = "SELECT COUNT(DISTINCT uid) amount FROM users WHERE FROM_UNIXTIME(access) < DATE_SUB(NOW(), INTERVAL 90 DAY)";
  $signed_in_older_than_90_days = db_query($query)->fetch();
  $insights["num_signedin_members_older_than_90_days"] = $signed_in_older_than_90_days->amount;

  // NUMBER OF SIGNED IN MEMBERS TODAY (APPROVED)
  $query = "SELECT COUNT(DISTINCT users.uid) amount FROM users RIGHT JOIN users_roles ON users_roles.uid = users.uid WHERE users_roles.rid = :role_id AND FROM_UNIXTIME(access) > DATE_SUB(NOW(), INTERVAL 1 DAY)";
  $signed_in_today_approved = db_query($query, array(':role_id' => $approved_role->rid))->fetch();
  $insights["num_signedin_members_24_hours_approved"] = $signed_in_today_approved->amount;

  // NUMBER OF SIGNED IN MEMBERS 7 DAYS (APPROVED)
  $query = "SELECT COUNT(DISTINCT users.uid) amount FROM users RIGHT JOIN users_roles ON users_roles.uid = users.uid WHERE users_roles.rid = :role_id AND FROM_UNIXTIME(access) > DATE_SUB(NOW(), INTERVAL 7 DAY)";
  $signed_in_7_days_approved = db_query($query, array(':role_id' => $approved_role->rid))->fetch();
  $insights["num_signedin_members_7_days_approved"] = $signed_in_7_days_approved->amount;

  // NUMBER OF SIGNED IN MEMBERS 30 DAYS (APPROVED)
  $query = "SELECT COUNT(DISTINCT users.uid) amount FROM users RIGHT JOIN users_roles ON users_roles.uid = users.uid WHERE users_roles.rid = :role_id AND FROM_UNIXTIME(access) > DATE_SUB(NOW(), INTERVAL 30 DAY)";
  $signed_in_30_days_approved = db_query($query, array(':role_id' => $approved_role->rid))->fetch();
  $insights["num_signedin_members_30_days_approved"] = $signed_in_30_days_approved->amount;

  // NUMBER OF SIGNED IN MEMBERS 90 DAYS (APPROVED)
  $query = "SELECT COUNT(DISTINCT users.uid) amount FROM users RIGHT JOIN users_roles ON users_roles.uid = users.uid WHERE users_roles.rid = :role_id AND FROM_UNIXTIME(access) > DATE_SUB(NOW(), INTERVAL 90 DAY)";
  $signed_in_90_days_approved = db_query($query, array(':role_id' => $approved_role->rid))->fetch();
  $insights["num_signedin_members_90_days_approved"] = $signed_in_90_days_approved->amount;

  // NUMBER OF MEMBERS WHO HAVEN'T SIGNED IN SINCE 90 DAYS (APPROVED)
  $query = "SELECT COUNT(DISTINCT users.uid) amount FROM users RIGHT JOIN users_roles ON users_roles.uid = users.uid WHERE users_roles.rid = :role_id AND FROM_UNIXTIME(access) < DATE_SUB(NOW(), INTERVAL 90 DAY)";
  $signed_in_older_than_90_days_approved = db_query($query, array(':role_id' => $approved_role->rid))->fetch();
  $insights["num_signedin_members_older_than_90_days_approved"] = $signed_in_older_than_90_days_approved->amount;

  // % OF SIGNED IN MEMBERS TODAY
  $insights["pct_signedin_members_24_hours"] = round(($signed_in_today->amount / $members_total) * 100, 0) . " / " . round(($signed_in_today_approved->amount / $members_approved_total) * 100, 0);

  // % OF SIGNED IN MEMBERS 7 DAYS
  $insights["pct_signedin_members_7_days"] = round(($signed_in_7_days->amount / $members_total) * 100, 0) . " / " . round(($signed_in_7_days_approved->amount / $members_approved_total) * 100, 0);

  // % OF SIGNED IN MEMBERS 30 DAYS
  $insights["pct_signedin_members_30_days"] = round(($signed_in_30_days->amount / $members_total) * 100, 0) . " / " . round(($signed_in_30_days_approved->amount / $members_approved_total) * 100, 0);

  // % OF SIGNED IN MEMBERS 90 DAYS
  $insights["pct_signedin_members_90_days"] = round(($signed_in_90_days->amount / $members_total) * 100, 0) . " / " . round(($signed_in_90_days_approved->amount / $members_approved_total) * 100, 0);

  // % OF MEMBERS WHO HAVEN'T SIGNED IN SINCE 90 DAYS
  $insights["pct_signedin_members_older_than_90_days"] = (100 - round(($signed_in_90_days->amount / $members_total) * 100, 0)) . " / " . (100 - round(($signed_in_90_days_approved->amount / $members_approved_total) * 100, 0));

  // TOTAL FACEBOOK SIGNUPS
  $query_sql = "SELECT COUNT(*) total FROM field_data_field_facebook_signin_link";
  $query = db_query($query_sql)->fetch();
  $num_logins_facebook = $query->total;
  $insights['num_logins_facebook'] = $num_logins_facebook;

  // TOTAL TWITTER SIGNUPS
  $query_sql = "SELECT COUNT(*) total FROM tm_twitter_account";
  $query = db_query($query_sql)->fetch();
  $num_logins_twitter = $query->total;
  $insights['num_logins_twitter'] = $num_logins_twitter;

  // TRACK VIEWS
  if (module_exists("tm_track_views")) {
    
    // unique profile views 7 days
    $query = "SELECT COUNT(uid) AS amount FROM tm_track_views WHERE entity_type = 1 AND timestamp > UNIX_TIMESTAMP(NOW() - INTERVAL 7 DAY)";
    $num_profile_views_7_days = db_query($query)->fetch();
    $insights["num_profile_views_7_days"] = $num_profile_views_7_days->amount;

    // unique profile views 30 days
    $query = "SELECT COUNT(uid) AS amount FROM tm_track_views WHERE entity_type = 1 AND timestamp > UNIX_TIMESTAMP(NOW() - INTERVAL 30 DAY)";
    $num_profile_views_30_days = db_query($query)->fetch();
    $insights["num_profile_views_30_days"] = $num_profile_views_30_days->amount;

    // unique profile views 90 days
    $query = "SELECT COUNT(uid) AS amount FROM tm_track_views WHERE entity_type = 1 AND timestamp > UNIX_TIMESTAMP(NOW() - INTERVAL 90 DAY)";
    $num_profile_views_90_days = db_query($query)->fetch();
    $insights["num_profile_views_90_days"] = $num_profile_views_90_days->amount;

    // unique profile views 7 days
    $query = "SELECT COUNT(DISTINCT(uid)) AS amount FROM tm_track_views WHERE entity_type = 1 AND timestamp > UNIX_TIMESTAMP(NOW() - INTERVAL 7 DAY)";
    $num_unique_profile_views_7_days = db_query($query)->fetch();
    $insights["num_unique_profile_views_7_days"] = $num_unique_profile_views_7_days->amount;

    // unique profile views 30 days
    $query = "SELECT COUNT(DISTINCT(uid)) AS amount FROM tm_track_views WHERE entity_type = 1 AND timestamp > UNIX_TIMESTAMP(NOW() - INTERVAL 30 DAY)";
    $num_unique_profile_views_30_days = db_query($query)->fetch();
    $insights["num_unique_profile_views_30_days"] = $num_unique_profile_views_30_days->amount;

    // unique profile views 90 days
    $query = "SELECT COUNT(DISTINCT(uid)) AS amount FROM tm_track_views WHERE entity_type = 1 AND timestamp > UNIX_TIMESTAMP(NOW() - INTERVAL 90 DAY)";
    $num_unique_profile_views_90_days = db_query($query)->fetch();
    $insights["num_unique_profile_views_90_days"] = $num_unique_profile_views_90_days->amount;

  }

  // RETURN DATA
  $data_labels = tm_reports_get_insights_labels();
  $results = ["data_values" => $insights, "data_labels" => $data_labels];
  cache_set('tm-reports-get-global-insights', $results, 'cache', time() + 900); // 15 mins
  return $results;
}
