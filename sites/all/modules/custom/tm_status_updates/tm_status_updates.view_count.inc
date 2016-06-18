<?php

// tm_status_udpates.view_count.inc - methods to implement status update view count


/**
 * Get total view count of a status update
 */
function tm_status_updates_get_view_count($status_update_id) {

	// get count from totals table
	$query = "SELECT total_view_count FROM tm_newsfeed_view_count_total WHERE status_update_id = :status_update_id";
	$result = db_query($query, array(':status_update_id' => $status_update_id))->fetch();
	$total_count = $result->total_view_count;

	// add count from daily table
	$query = "SELECT COUNT(*) AS total_views from tm_newsfeed_view_count_daily WHERE status_update_id = :status_update_id";
	$result = db_query($query, array(':status_update_id' => $status_update_id))->fetch();
	$daily_count = $result->total_views;

	// total
	$total_views = $total_count + $daily_count;
	return $total_views;
}

/**
 * Increment daily view count with visitor's IP
 */
function tm_status_updates_increment_daily_view_count($status_update_id, $viewer_ip = null) {

	// Fetch viewer ip if not provided
	if ($viewer_ip == null) {
		$viewer_ip = tm_geoip_get_real_visitor_ip();
	}

	// Remove any existing entries for today for ip
	$query = "DELETE FROM tm_newsfeed_view_count_daily WHERE created >= DATE_SUB(NOW(), INTERVAL 1 DAY) AND status_update_id = :status_update_id AND viewer_ip = :viewer_ip";
	db_query($query, array(':status_update_id' => $status_update_id, ':viewer_ip' => $viewer_ip));

	// Insert count for this ip
	$query = "INSERT INTO tm_newsfeed_view_count_daily (status_update_id, view_count, viewer_ip) VALUES (:status_update_id, 1, :viewer_ip)";
	db_query($query, array(':status_update_id' => $status_update_id, ':viewer_ip' => $viewer_ip));

}

/**
 * Initialize a total view count for a status update
 * Note: Must be called when a new status update is created
 */
function tm_status_updates_init_total_view_count($status_update_id) {

	$query = "INSERT INTO tm_newsfeed_view_count_total (status_update_id, total_view_count) VALUES (:status_update_id, 0)";
	db_query($query, array(':status_update_id' => $status_update_id));
}

/**
 * Update total view counts from daily table
 */
function tm_status_updates_update_total_view_counts() {

	// Update total counter from totals of past day
	$query = "UPDATE tm_newsfeed_view_count_total totals SET total_view_count = total_view_count + (SELECT COUNT(status_update_id) FROM tm_newsfeed_view_count_daily daily WHERE totals.status_update_id = daily.status_update_id AND created < DATE_SUB(NOW(), INTERVAL 1 DAY))";
	db_query($query);

	// Remove any counts from daily table older than a day
	$query = "DELETE FROM tm_newsfeed_view_count_daily WHERE created < DATE_SUB(NOW(), INTERVAL 1 DAY)";
	db_query($query);
}
