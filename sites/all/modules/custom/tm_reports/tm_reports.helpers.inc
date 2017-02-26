<?php

// tm_reports.helpers.inc - reporting helper methods

/**
 * Check user is moderator
 */
function tm_reports_user_is_moderator() {
  global $user;
  return (in_array("moderator", $user->roles) or in_array("administrator", $user->roles));
}

/**
 * Get Industry Segment Data from a chapter, event or global (approved members)
 * Note: CACHED
 */
function _tm_reports_get_industry_segment_data($node_id = "", $node_type = "chapter") {

  // check cache
  $cache = cache_get('tm-reports-get-industry-segment-data-' . $node_type . "-" . $node_id, 'cache');
  if (!empty($cache)) {
    return $cache->data;
  }

  $approved_role = user_role_load_by_name("approved user");

  // SQL - Join to Chapter or Event
  if (($node_id != "") and ($node_type == "chapter")) {
    $chapter = node_load($node_id);
    $flag = flag_get_flag('signup', NULL);
    
    $query = "select count(users.uid) as total, taxonomy_term_data.tid as segment_tid, taxonomy_term_data.name from users
  left join flagging on users.uid = flagging.uid
  left join field_data_field_segment on users.uid = field_data_field_segment.entity_id
  left join taxonomy_term_hierarchy on taxonomy_term_hierarchy.tid = field_data_field_segment.field_segment_tid 
  left join taxonomy_term_data ON (IF (taxonomy_term_hierarchy.parent > 0, taxonomy_term_hierarchy.parent, taxonomy_term_hierarchy.tid) = taxonomy_term_data.tid)
  left join users_roles ON users_roles.uid = users.uid 
  where users_roles.rid = :role_id
  and fid = :fid
  and flagging.entity_id = :chapter_nid
  and flagging.entity_type = 'node'
  and field_data_field_segment.entity_type = 'user'
  and taxonomy_term_data.tid is not null
  group by segment_tid
  order by name";

    $rows = db_query($query, array(':fid' => $flag->fid, ':role_id' => $approved_role->rid, ':chapter_nid' => $chapter->nid))->fetchAll();

  }

  // SQL - Join to Event
  // Ignore approved role - guests can also join events
  if (($node_id != "") and ($node_type == "event")) {
    $event = node_load($node_id);
    $flag = flag_get_flag('event_register', NULL);
    
    $query = "select count(users.uid) as total, taxonomy_term_data.tid as segment_tid, taxonomy_term_data.name from users
  left join flagging on users.uid = flagging.uid
  left join field_data_field_segment on users.uid = field_data_field_segment.entity_id
  left join taxonomy_term_hierarchy on taxonomy_term_hierarchy.tid = field_data_field_segment.field_segment_tid 
  left join taxonomy_term_data ON (IF (taxonomy_term_hierarchy.parent > 0, taxonomy_term_hierarchy.parent, taxonomy_term_hierarchy.tid) = taxonomy_term_data.tid)
  where fid = :fid
  and flagging.entity_id = :event_nid
  and flagging.entity_type = 'node'
  and field_data_field_segment.entity_type = 'user'
  and taxonomy_term_data.tid is not null
  group by segment_tid
  order by name";

    $rows = db_query($query, array(':fid' => $flag->fid, ':event_nid' => $event->nid))->fetchAll();

  }

  // SQL - Global
  if ($node_id == "") {

    $query = "select count(users.uid) as total, taxonomy_term_data.tid as segment_tid, taxonomy_term_data.name from users
  left join field_data_field_segment on users.uid = field_data_field_segment.entity_id
  left join taxonomy_term_hierarchy on taxonomy_term_hierarchy.tid = field_data_field_segment.field_segment_tid 
  left join taxonomy_term_data ON (IF (taxonomy_term_hierarchy.parent > 0, taxonomy_term_hierarchy.parent, taxonomy_term_hierarchy.tid) = taxonomy_term_data.tid)
  left join users_roles ON users_roles.uid = users.uid 
  where users_roles.rid = :role_id
  and field_data_field_segment.entity_type = 'user'
  and taxonomy_term_data.tid is not null
  group by segment_tid
  order by name";

    $rows = db_query($query, array(':role_id' => $approved_role->rid))->fetchAll();

  }
  
  // Prepare row data
  $segment_data = array();
  foreach ($rows as $row) {
    $segment_data[] = array("name" => $row->name, "total" => $row->total, "tid" => $row->segment_tid);
  }

  // RETURN DATA
  $data_labels = tm_reports_get_insights_labels();
  cache_set('tm-reports-get-industry-segment-data-' . $node_type . "-" . $node_id, $segment_data, 'cache', time() + 900); // 15 mins
  return $segment_data;
}

/**
 * Get monthly flagging data for a node and flag over time
 * Note: CACHED
 */
function _tm_reports_flagging_time_series($node_id, $flag_id, $entity_type = "node", $timestamp_interval = "INTERVAL 1 YEAR") {

  // check cache
  if (is_array($node_id)) {
    $cache_key_node_id = implode("_", $node_id);
  } else {
    $cache_key_node_id = $node_id;
  }
  $cache_key = "tm-reports-flagging-time-series-" . $cache_key_node_id . "-" . $flag_id . "-" . $entity_type . "-" . $timestamp_interval;
  $cache_key = str_replace(" ", "-", $cache_key);
  $cache = cache_get($cache_key, 'cache');
  if (!empty($cache)) {
    return $cache->data;
  }

  // SQL 
  $query = "select count(*) as total, YEAR(FROM_UNIXTIME(timestamp)) as group_year, MONTH(FROM_UNIXTIME(timestamp)) as group_month from flagging
  where FROM_UNIXTIME(timestamp) >= DATE_SUB(NOW()," . $timestamp_interval . ")";

  // node_id can be null, id, or array
  if ((is_array($node_id) and sizeof($node_id) > 0)) {
    $query .= " and entity_id IN (:node_id)"; 
  } else if ($node_id != NULL) {
    $query .= " and entity_id = :node_id"; 
  }
  
  $query .= " and fid = :fid
  and entity_type = :entity_type
  group by group_year, group_month order by group_year, group_month asc;";

  $params = array(':node_id' => $node_id, ':fid' => $flag_id, ':entity_type' => $entity_type);
  if ($node_id == NULL) {
    $params = array(':fid' => $flag_id, ':entity_type' => $entity_type);
  }

  $rows = db_query($query, $params)->fetchAll();

  // pad the grouped data data
  $series = _tm_reports_helper_pad_monthly_grouped_data($rows);

  // return data
  cache_set($cache_key, $series, 'cache', time() + 900); // 15 mins
  return $series;
}

/**
 * Get monthly new user signup data over time
 */
function _tm_reports_new_users_time_series($timestamp_interval = "INTERVAL 1 YEAR") {

   // check cache
  $cache_key = "tm-reports-new-users-" . $timestamp_interval;
  $cache_key = str_replace(" ", "-", $cache_key);
  $cache = cache_get($cache_key, 'cache');
  if (!empty($cache)) {
    return $cache->data;
  }

  // SQL 
  $query = "select count(*) as total, YEAR(FROM_UNIXTIME(created)) as group_year, MONTH(FROM_UNIXTIME(created)) as group_month from users
  where FROM_UNIXTIME(created) >= DATE_SUB(NOW()," . $timestamp_interval . ")
  group by group_year, group_month order by group_year, group_month asc;";
  
  $rows = db_query($query)->fetchAll();

  // pad the grouped data data
  $series = _tm_reports_helper_pad_monthly_grouped_data($rows);

  // return data
  cache_set($cache_key, $series, 'cache', time() + 900); // 15 mins
  return $series;
}

/**
 * Get total chapters over time
 */
function _tm_reports_monthly_chapters_time_series($timestamp_interval = "INTERVAL 1 YEAR") {

   // check cache
  $cache_key = "tm-reports-monthly-chapters-" . $timestamp_interval;
  $cache_key = str_replace(" ", "-", $cache_key);
  $cache = cache_get($cache_key, 'cache');
  if (!empty($cache)) {
    return $cache->data;
  } 

  // SQL 
  $query = "select count(*) as total, YEAR(FROM_UNIXTIME(created)) as group_year, MONTH(FROM_UNIXTIME(created)) as group_month from node
  where type = 'chapter' and status = 1 
  and FROM_UNIXTIME(created) >= DATE_SUB(NOW()," . $timestamp_interval . ")
  group by group_year, group_month order by group_year, group_month asc;";
  
  $rows = db_query($query)->fetchAll();

  // pad the grouped data data
  $series = _tm_reports_helper_pad_monthly_grouped_data($rows);

  // calculate series total
  $series_total = 0;
  for ($i = 0; $i < sizeof($series); $i++) {
    $series_total = $series_total + $series[$i]["total"];
  }

  // get total number of chapters
  $query = "SELECT COUNT(*) amount FROM {node} n ".
              "WHERE n.type = :type";
  $num_chapters = db_query($query, array(':type' => "chapter"))->fetch()->amount;

  // starting total
  $total = $num_chapters - $series_total;

  // aggregate monthly totals
  for ($i = 0; $i < sizeof($series); $i++) {
    $total = $series[$i]["total"] + $total;
    $series[$i]["total"] = $total;
  }

  // return data
  cache_set($cache_key, $series, 'cache', time() + 300); // 5 mins
  return $series;
}

/**
 * Get monthly status updates data over time
 */
function _tm_reports_status_updates_time_series($timestamp_interval = "INTERVAL 1 YEAR") {

   // check cache
  $cache_key = "tm-reports-status-updates-" . $timestamp_interval;
  $cache_key = str_replace(" ", "-", $cache_key);
  $cache = cache_get($cache_key, 'cache');
  if (!empty($cache)) {
    return $cache->data;
  }

  // SQL 
  $query = "select count(*) as total, YEAR(created) as group_year, MONTH(created) as group_month from tm_newsfeed_status_updates
  where is_removed = 0 and created >= DATE_SUB(NOW()," . $timestamp_interval . ")
  group by group_year, group_month order by group_year, group_month asc;";
  
  $rows = db_query($query)->fetchAll();

  // pad the grouped data data
  $series = _tm_reports_helper_pad_monthly_grouped_data($rows);

  // return data
  cache_set($cache_key, $series, 'cache', time() + 900); // 15 mins
  return $series;
}

/**
 * Get preview links shared over time
 */
function _tm_reports_links_shared_time_series($timestamp_interval = "INTERVAL 1 YEAR") {

   // check cache
  $cache_key = "tm-reports-links-shared-" . $timestamp_interval;
  $cache_key = str_replace(" ", "-", $cache_key);
  $cache = cache_get($cache_key, 'cache');
  if (!empty($cache)) {
    return $cache->data;
  }

  // SQL 
  $query = "select count(*) as total, YEAR(created) as group_year, MONTH(created) as group_month from tm_newsfeed_status_updates
  where is_removed = 0 and preview_link_id is not null and created >= DATE_SUB(NOW()," . $timestamp_interval . ")
  group by group_year, group_month order by group_year, group_month asc;";
  
  $rows = db_query($query)->fetchAll();

  // pad the grouped data data
  $series = _tm_reports_helper_pad_monthly_grouped_data($rows);

  // return data
  cache_set($cache_key, $series, 'cache', time() + 900); // 15 mins
  return $series;
}

/**
 * Helper method to pad empty sql grouped data in a time series
 */
function _tm_reports_helper_pad_monthly_grouped_data($rows) {

  // Step 1. Create a keyed array
  // key in array as as YYYY_MM
  $results = array();
  foreach ($rows as $row) {
    $results[$row->group_year . "_" . $row->group_month] = array("total" => $row->total, "group_year" => $row->group_year, "group_month" => $row->group_month);
  }

  // Step 2. Pad results for with no entries
  $start_year = 2014;
  $start_month = 1;
  $series = array();
  $started_data = false;
  $at_current_month = false;
  for ($i=$start_year; $i <= intval(date("Y")); $i++) {
    for ($j=$start_month; (($j <= 12) && !$at_current_month); $j++) { 

      // check if we are at current month
      $at_current_month = ($i == intval(date("Y")) && ($j == intval(date("n"))));

      $key = $i . "_" . $j;
      if (array_key_exists($key, $results)) {
        $started_data = true;
        $series[] = $results[$key]; // add existing data
      } else if ($started_data) {
        // pad data
        $series[] = array("total" => 0, "group_year" => $i, "group_month" => $j);
      }
    }
  }

  return $series;
}

/**
 * Get event attendance frequency
 */
function _tm_reports_event_registration_frequency($chapter_id = null, $aggregate = false) {

  // check cache
  $cache_key = "tm-reports-event-registration-frequency-" . $chapter_id . "-" . $aggregate;
  $cache_key = str_replace(" ", "-", $cache_key);
  $cache = cache_get($cache_key, 'cache');
  if (!empty($cache)) {
    return $cache->data;
  }

  // SQL
  $flag_event_register = flag_get_flag('event_register', NULL);
  if ($chapter_id == null) {
    $query = "SELECT amount, COUNT(*) AS freq FROM (SELECT uid, COUNT(uid) AS amount FROM flagging f WHERE f.fid = :fid group by uid) AS t2 GROUP BY amount ORDER BY amount ASC";
    $rows = db_query($query, array(":fid" => $flag_event_register->fid) )->fetchAll();
  } else {
    $query = "SELECT amount, COUNT(*) AS freq FROM (SELECT uid, COUNT(uid) AS amount FROM flagging f WHERE entity_id IN (SELECT entity_id AS nid FROM field_data_field_chapter WHERE bundle = 'event' AND field_chapter_target_id = :chapter_id) AND f.fid = :fid GROUP BY uid) as t2 GROUP BY amount ORDER BY amount ASC";
    $rows = db_query($query, array(":chapter_id" => $chapter_id, ":fid" => $flag_event_register->fid) )->fetchAll();
  }

  // pad the grouped data data
  $series = array();
  $max_amount = 10;
  $max_total = 0;
  foreach ($rows as $row) {
    if ($row->amount >= $max_amount) {
      $max_total = $max_total + $row->freq;
    } else {
      $series[$row->amount] = array("label" => $row->amount, "total" => $row->freq);
    }
  }
  
  // add max amount
  $series[$max_amount] = array("label" => $max_amount . "+", "total" => $max_total);

  // pad empty groups
  for ($i = 1; $i <= $max_amount; $i++) {
    if (!isset($series[$i])) {
      $series[$i] = array("label" => $i, "total" => 0);
    }
  }

  // aggregate 2+ events
  // ie: 1 event, 2 or more, 3 or more
  if ($aggregate) {
    $aggregate = 0;
    for ($i = $max_amount; $i >= 2; $i--) {
      $aggregate = $aggregate + $series[$i]["total"];
      $series[$i] = array("label" => $i . "+", "total" => $aggregate);
    }
  }

  // sort it
  ksort($series);

  // return data
  cache_set($cache_key, $series, 'cache', time() + 900); // 15 mins
  return $series;
}

/**
 * Get all event nids for a chapter (does not include published events)
 */
function _tm_reports_get_chapter_event_nids($chapter_id) {

  $event_nids = array();
  $query = "SELECT entity_id AS nid FROM field_data_field_chapter WHERE bundle = 'event' AND field_chapter_target_id = :chapter_id";
  $results = db_query($query, array(':chapter_id' => $chapter_id))->fetchAll();
  foreach ($results as $result) {
    $event_nids[] = $result->nid;
  }
  return $event_nids;
}

/**
 * Helper function _tm_reports_add_handler_field_numeric
 */
function _tm_reports_add_join_industry($field_alias, $tid) {
  $join = new views_join;
  $join->construct('taxonomy_term_hierarchy', 'users_flagging__field_data_field_segment', 'field_segment_tid', 'tid', array(), 'LEFT');
  $join->extra = array(
    array(
        'field' => 'tid',
        'value' => $tid,
    ),
    array(
        'field' => 'parent',
        'value' => $tid,
        'operator' => '=',
    ),
  );
  $join->extra_type = 'OR';
  return $join;
}
