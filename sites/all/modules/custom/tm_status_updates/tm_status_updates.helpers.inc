<?php

// tm_status_updates.helpers.inc - helper methods for status updates

/**
 * Return a sorted list of flag and status updates for a user account
 */
function tm_status_updates_get_combined_feed($account, $limit_from = 0, $limit_to = 10) {
	
	// Flag user as having updated status
	$follow_flags = array();
	$follow_flags[] = flag_get_flag('follow_members')->fid;
	$follow_flags[] = flag_get_flag('follow_organizations')->fid;
	$follow_flags[] = flag_get_flag('signup')->fid;
	$follow_flags[] = flag_get_flag('event_register')->fid;

	// get recent status update items
	$query = "SELECT *, UNIX_TIMESTAMP(`created`) AS `timestamp` FROM tm_newsfeed_status_updates WHERE (entity_id IN (SELECT entity_id FROM flagging WHERE uid = :uid AND fid IN (" . implode(",", $follow_flags) . ")) OR entity_id = :uid) AND is_removed=0 AND moderator_hide=0 ORDER BY created DESC LIMIT " . (int)$limit_from . ", " . (int)$limit_to;
	$status_updates = db_query($query, array(':uid' => $account->uid))->fetchAll();

	// get recent flag items
	$query = "SELECT * FROM flagging WHERE (entity_id IN (SELECT entity_id FROM flagging WHERE uid = :uid AND fid IN (" . implode(",", $follow_flags) . ")) OR entity_id = :uid)  ORDER BY timestamp DESC LIMIT " . (int)$limit_from . ", " . (int)$limit_to;
	$flag_results = db_query($query, array(':uid' => $account->uid))->fetchAll();

	// merge
	$all_results = array();
	foreach($status_updates as $result) {
		$all_results[] = $result;
	}
	foreach($flag_results as $result) {
		$all_results[] = $result;
	}

	// sort
	usort($all_results, "_tm_status_updates_compare_timestamps");

	return $all_results;
}

/**
 * Return a sorted list of flag and status updates for all accounts
 */
function tm_status_updates_get_combined_feed_global($limit_from = 0, $limit_to = 10) {
	
	// Flag user as having updated status
	$follow_flags = array();
	$follow_flags[] = flag_get_flag('follow_members')->fid;
	$follow_flags[] = flag_get_flag('follow_organizations')->fid;
	$follow_flags[] = flag_get_flag('signup')->fid;
	$follow_flags[] = flag_get_flag('event_register')->fid;

	// get recent status update items
	$query = "SELECT *, UNIX_TIMESTAMP(`created`) AS `timestamp` FROM tm_newsfeed_status_updates WHERE (entity_id IN (SELECT entity_id FROM flagging WHERE fid IN (" . implode(",", $follow_flags) . "))) AND is_removed=0 AND moderator_hide=0 ORDER BY created DESC LIMIT " . (int)$limit_from . ", " . (int)$limit_to;
	$status_updates = db_query($query)->fetchAll();

	// get recent flag items
	$query = "SELECT * FROM flagging WHERE (entity_id IN (SELECT entity_id FROM flagging WHERE fid IN (" . implode(",", $follow_flags) . ")))  ORDER BY timestamp DESC LIMIT " . (int)$limit_from . ", " . (int)$limit_to;
	$flag_results = db_query($query)->fetchAll();

	// merge
	$all_results = array();
	foreach($status_updates as $result) {
		$all_results[] = $result;
	}
	foreach($flag_results as $result) {
		$all_results[] = $result;
	}

	// sort
	usort($all_results, "_tm_status_updates_compare_timestamps");

	return $all_results;
}

/**
 * Get the timestamp of the last update for an entity type
 */
function tm_status_updates_get_last_update_timestamp($entity_type, $entity_id) {

	// fetch the timestamp of the user's most recent status update
	$query = "SELECT CONVERT_TZ(`created`, @@session.time_zone, '+00:00') AS `created_utc`  FROM tm_newsfeed_status_updates WHERE entity_type = :entity_type AND entity_id = :entity_id AND is_removed=0 AND moderator_hide=0 ORDER BY created DESC LIMIT 1";
	$result = db_query($query, array(":entity_type" => $entity_type, ":entity_id" => $entity_id))->fetch();

	// get timestamp of last status update
	if (isset($result->created_utc)) {
		if ($result->created_utc != null) {
			$updated_dt = new Datetime($result->created_utc, new DateTimeZone('UTC'));
			return $updated_dt->format('U');
		}
	}

	// no status update for this member
	return null;
}

/**
 * Get the most recent update from an entity
 */
function tm_status_updates_get_latest_update($entity_id, $entity_type) {

	$query = "SELECT *, UNIX_TIMESTAMP(`created`) AS `timestamp` FROM tm_newsfeed_status_updates WHERE entity_id = :entity_id AND entity_type = :entity_type AND is_removed=0 AND moderator_hide=0 ORDER BY created DESC LIMIT 1";

	$results = db_query($query, array(':entity_type' => $entity_type, ':entity_id' => $entity_id))->fetchAll();

	// no updates
	if (sizeof($results) == 0) {
		return null;
	}

	// return update
	return $results[0];
}

/**
 * Helper method to compare timestamps between two object arrays
 **/
function _tm_status_updates_compare_timestamps($a, $b) {
	return ($a->timestamp < $b->timestamp);
}
