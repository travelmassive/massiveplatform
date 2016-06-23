<?php

// tm_status_updates.controller.inc - controller methods

/**
 * Determine whether a user can edit a particular status update
 */
function tm_status_updates_check_permission($entity_type, $entity_id, $action, $check_uid) {

	// check valid action
	$valid_actions = array("create", "edit", "remove", "moderate", "promote");
	if (!in_array($action, $valid_actions)) {
		return false;
	}

	// use static cache as we look this up a lot per page load
	$check_value = &drupal_static("tm_status_updates_check_can_edit_" . $entity_type . "_" . $entity_id . "_" . $action . "_" . $check_uid);
	if ($check_value != null) {
		return $check_value;
	}

	// allow moderator or administrator to modererate and feature posts
	$account = user_load($check_uid);
	if (($action == "moderate") or ($action == "promote")) {
		if (in_array("moderator", $account->roles) or in_array("administrator", $account->roles)) {
			return true;
		} else {
			return false;
		}
	}

	// personal posts can only be edited by the owner
	if ($entity_type == "user") {
		return ($entity_id == $check_uid);
	}

	// company posts
	if ($entity_type == "organization") {
		$companies_nids = tm_organizations_get_users_companies($account->uid);
		foreach ($companies_nids as $company_nid) {
			if ($company_nid == $entity_id) {
				$check_value = true;
				return true; // this is a manager of the company profile
			}
		}
	}

	// chapter posts
	if ($entity_type == "chapter") {
		if ($account == null) { 
			$check_value = false;
			return false;
		}
		$chapters_nids = tm_users_get_chapters_leading($account);
		foreach ($chapters_nids as $chapter_nid) {
			if ($chapter_nid == $entity_id) {
				$check_value = true;
				return true; // this user is a chapter leader 
			}
		}
	}

	// not allowed to edit
	$check_value = false;
	return false;

}

/**
 * Get the a status update
 */
function tm_status_updates_get_update_by_id($status_update_id) {

	$query = "SELECT *, UNIX_TIMESTAMP(`created`) AS `timestamp` FROM tm_newsfeed_status_updates WHERE id = :status_update_id ORDER BY created DESC LIMIT 1";

	$results = db_query($query, array(':status_update_id' => $status_update_id))->fetchAll();

	// no updates
	if (sizeof($results) == 0) {
		return null;
	}

	// return update
	return $results[0];
}

/**
 * Get the most recent update from an entity
 */
function tm_status_updates_get_latest_update($entity_id, $entity_type) {

	$query = "SELECT *, UNIX_TIMESTAMP(`created`) AS `timestamp` FROM tm_newsfeed_status_updates WHERE entity_id = :entity_id AND entity_type = :entity_type AND moderator_hide = 0 AND is_removed = 0 ORDER BY created DESC LIMIT 1";

	$results = db_query($query, array(':entity_type' => $entity_type, ':entity_id' => $entity_id))->fetchAll();

	// no updates
	if (sizeof($results) == 0) {
		return null;
	}

	// return update
	return $results[0];
}

/**
 * Get the most recent promoted updates
 */
function tm_status_updates_get_promoted_updates($days_old = 7) {

	$query = "SELECT *, UNIX_TIMESTAMP(`created`) AS `timestamp` FROM tm_newsfeed_status_updates WHERE  created >= DATE_SUB(NOW(), INTERVAL " . intval($days_old) . " DAY) AND is_promoted = 1 AND moderator_hide = 0 AND is_removed = 0 ORDER BY created DESC";

	$results = db_query($query)->fetchAll();

	// no updates
	if (sizeof($results) == 0) {
		return null;
	}

	// return results
	return $results;
}

/**
 * Create a status update
 */
function tm_status_updates_create_update($entity_id, $entity_type, $status_update, $preview_link_id, $posting_user, $posting_user_ip, $location_text, $location_latitude, $location_longitude) {

	global $conf;

	// prepare fields
	$update_fields = array();
	$update_fields['entity_id'] = $entity_id;
	$update_fields['entity_type'] = $entity_type; // user, chapter, organization
	$update_fields['status_update'] = trim($status_update);
	if ($preview_link_id != null) {
		$update_fields['preview_link_id'] = $preview_link_id;
	}
	if ($location_text != null) {
		$update_fields['location_text'] = substr($location_text, 0, 254);
	}
	if ($location_latitude != null) {
		$update_fields['location_latitude'] = $location_latitude;
	}
	if ($location_longitude != null) {
		$update_fields['location_longitude'] = $location_longitude;
	}
	$update_fields['poster_uid'] = $posting_user->uid;
	$update_fields['poster_ip'] = $posting_user_ip;

	// Update status in database
	$status_update_id = db_insert('tm_newsfeed_status_updates')->fields($update_fields)->execute();

	// Flag user as having updated status
	$flag = flag_get_flag('status_updated_by_member');
	$flag->flag("unflag", $posting_user->uid); // unflag so we can flag again
	$flag->flag("flag", $posting_user->uid);

	// Initialize the view counter
	tm_status_updates_init_total_view_count($status_update_id);

	// Update view counters
	if ($conf["tm_status_updates_view_counter_no_cron"]) {
		tm_status_updates_update_total_view_counts();
	}

	return true;
}

/**
 * Remove status update
 */
function tm_status_updates_remove_by_id($status_update_id) {

	$query = "UPDATE tm_newsfeed_status_updates SET is_removed = 1, edited = NOW() WHERE id = :status_update_id";
	db_query($query, array(':status_update_id' => $status_update_id));
	return true;
}

/**
 * Moderate status update
 */
function tm_status_updates_moderate($status_update_id, $moderator_uid, $moderator_note) {

	$query = "UPDATE tm_newsfeed_status_updates SET moderator_hide = 1, moderator_uid = :moderator_uid, moderator_note = :moderator_note, moderator_timestamp = NOW() WHERE id = :status_update_id";
	db_query($query, array(':status_update_id' => $status_update_id, ':moderator_uid' => $moderator_uid, ':moderator_note' => $moderator_note));
	return true;
}

/**
 * Promote status update
 */
function tm_status_updates_promote($status_update_id, $is_promoted = true) {

	$is_promoted_value = "0";
	if ($is_promoted) {
		$is_promoted_value = "1";
	}
	$query = "UPDATE tm_newsfeed_status_updates SET is_promoted = " . $is_promoted_value . " WHERE id = :status_update_id";
	db_query($query, array(':status_update_id' => $status_update_id));
	return true;
}

/**
 * Edit status update
 */
function tm_status_updates_edit_update($status_update) {

	try {
		$query = "UPDATE tm_newsfeed_status_updates SET 
			status_update = :status_update, 
			preview_link_id = :preview_link_id, 
			poster_uid = :poster_uid, 
			poster_ip = :poster_ip, 
			location_text = :location_text, 
			location_latitude = :location_latitude, 
			location_longitude = :location_longitude, 
			edited = NOW() 
			WHERE id = :status_update_id";
		db_query($query, 
		array(':status_update' => $status_update->status_update, 
			  ':preview_link_id' => $status_update->preview_link_id, 
			  ':poster_uid' => $status_update->poster_uid, 
			  ':poster_ip' => $status_update->poster_ip, 
			  ':location_text' => $status_update->location_text, 
			  ':location_latitude' => $status_update->location_latitude, 
			  ':location_longitude' => $status_update->location_longitude, 
			  ':status_update_id' => $status_update->id));
		return true;
	} catch (Exception $e) {
		print_r($e);
		return false;
	}
	
}

/**
 * Return a sorted list of flag and status updates for a user account
 * Note: the flag items are returned within the range of timestamps from the status updates
 */
function tm_status_updates_get_combined_feed($account, $limit_from = 0, $limit_to = 10) {

	// Flag user as having updated status
	$follow_flags = array();
	$follow_flags[] = flag_get_flag('follow_members')->fid;
	$follow_flags[] = flag_get_flag('follow_organizations')->fid;
	$follow_flags[] = flag_get_flag('signup')->fid;
	$follow_flags[] = flag_get_flag('event_register')->fid;

	// get recent status update items
	$query = "SELECT *, UNIX_TIMESTAMP(`created`) AS `timestamp` FROM tm_newsfeed_status_updates WHERE (entity_id IN (SELECT entity_id FROM flagging WHERE uid = :uid AND fid IN (" . implode(",", $follow_flags) . ")) OR entity_id = :uid) AND is_removed = 0 AND moderator_hide = 0 ORDER BY timestamp DESC LIMIT " . intval($limit_from) . ", " . intval($limit_to);
	$status_updates = db_query($query, array(':uid' => $account->uid))->fetchAll();

	// get range of timestamps
	// use this for the flag query
	$min_timestamp = PHP_INT_MAX;
	$max_timestamp = 0;
	foreach($status_updates as $result) {
		if ($result->timestamp < $min_timestamp) {
			$min_timestamp = $result->timestamp;
		}
		if ($result->timestamp > $max_timestamp) {
			$max_timestamp = $result->timestamp;
		}
	}
	
	// if no status updates for personal newsfeed, extend range of flags
	if (sizeof($status_updates) == 0) {
		$min_timestamp = 0;
		$max_timestamp = PHP_INT_MAX;
	}

	// get recent flag items
	$query = "SELECT * FROM flagging WHERE (entity_id IN (SELECT entity_id FROM flagging WHERE uid = :uid AND fid IN (" . implode(",", $follow_flags) . ")) OR entity_id = :uid) AND fid IN (" . implode(",", $follow_flags) . ") AND timestamp >= " . $min_timestamp;

	// if on first page, show all new posts since min_timestamp
	if ($limit_from > 0) {
		$query .= " AND timestamp <= " . $max_timestamp;
	}
	$query .= " ORDER BY timestamp DESC";
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
 * Note: the flag items are returned within the range of timestamps from the status updates
 */
function tm_status_updates_get_combined_feed_global($limit_from = 0, $limit_to = 10) {
	
	// Flag user as having updated status
	$follow_flags = array();
	$follow_flags[] = flag_get_flag('follow_members')->fid;
	$follow_flags[] = flag_get_flag('follow_organizations')->fid;
	$follow_flags[] = flag_get_flag('signup')->fid;
	$follow_flags[] = flag_get_flag('event_register')->fid;

	// get recent status update items
	$query = "SELECT *, UNIX_TIMESTAMP(`created`) AS `timestamp` FROM tm_newsfeed_status_updates WHERE is_removed = 0 AND moderator_hide = 0 ORDER BY created DESC LIMIT " . intval($limit_from) . ", " . intval($limit_to);
	$status_updates = db_query($query)->fetchAll();

	// get range of timestamps
	// use this for the flag query
	$min_timestamp = PHP_INT_MAX;
	$max_timestamp = 0;
	foreach($status_updates as $result) {
		if ($result->timestamp < $min_timestamp) {
			$min_timestamp = $result->timestamp;
		}
		if ($result->timestamp > $max_timestamp) {
			$max_timestamp = $result->timestamp;
		}
	}

	// get recent flag items
	$query = "SELECT * FROM flagging WHERE fid IN (" . implode(",", $follow_flags) . ") AND timestamp >= " . $min_timestamp;

	// if on first page, show all new posts since min_timestamp
	if ($limit_from > 0) {
		$query .= " AND timestamp <= " . $max_timestamp;
	}
	$query .= " ORDER BY timestamp DESC";
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
 * Return a sorted status updates that match a tag
 * $tags should be array prefixed with #. ie: array('#awesome', '#tag') or array('word');
 */
function tm_status_updates_get_matching_search_terms($search_terms, $limit_from = 0, $limit_to = 100, $days_old = 30) {
	
	// check we have search terms
	if (sizeof($search_terms) == 0) {
		return array();
	}

	// get recent status update items
	$query = "SELECT *, UNIX_TIMESTAMP(`created`) AS `timestamp` FROM tm_newsfeed_status_updates WHERE created >= DATE_SUB(NOW(), INTERVAL " . intval($days_old) . " DAY) AND is_removed = 0 AND moderator_hide = 0 ";

	$query_options = array();
	for ($i = 0; $i < sizeof($search_terms); $i++) {
		$query .= " AND status_update LIKE :search_term" . $i;
		$query_options[':search_term' . $i] = '%' . $search_terms[$i] . '%';
	}

	$query .= " ORDER BY created DESC LIMIT " . intval($limit_from) . ", " . intval($limit_to);
	$status_updates = db_query($query, $query_options)->fetchAll();

	return $status_updates;
}

/**
 * Search for updates near a geographic location, sorted by how close to the distance
 */
function tm_status_updates_get_near_location($latitude, $longitude, $range, $limit_from, $limit_to, $days_old = 30) {


	// reference
	// http://stackoverflow.com/questions/2234204/latitude-longitude-find-nearest-latitude-longitude-complex-sql-or-complex-calc
	// converted to km

	// search for status updates near location
	$query = "SELECT *, UNIX_TIMESTAMP(`created`) AS `timestamp`, ";

	$query .= "SQRT(
		    POW(111.2 * (location_latitude - " . floatval($latitude) . "), 2) +
		    POW(111.2 * (" . floatval($longitude) . " - location_longitude) * COS(location_latitude / 57.3), 2)) AS distance";

	$query .= " FROM tm_newsfeed_status_updates WHERE location_latitude IS NOT null AND location_longitude IS NOT NULL AND created >= DATE_SUB(NOW(), INTERVAL " . intval($days_old) . " DAY) AND is_removed = 0 AND moderator_hide = 0";
	
	$query = "SELECT * FROM (" . $query . ") AS distance_results WHERE distance < " . floatval($range) . " ORDER BY created DESC LIMIT " . intval($limit_from) . ", " . intval($limit_to);

	$status_updates = db_query($query)->fetchAll();
	return $status_updates;
}


/**
 * Get the timestamp of the last update for an entity type
 */
function tm_status_updates_get_last_update_timestamp($entity_type, $entity_id) {

	// fetch the timestamp of the user's most recent status update
	$query = "SELECT CONVERT_TZ(`created`, @@session.time_zone, '+00:00') AS `created_utc`  FROM tm_newsfeed_status_updates WHERE entity_type = :entity_type AND entity_id = :entity_id ORDER BY created DESC LIMIT 1";
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
 * Helper method to compare timestamps between two object arrays
 **/
function _tm_status_updates_compare_timestamps($a, $b) {
	return ($a->timestamp < $b->timestamp);
}
