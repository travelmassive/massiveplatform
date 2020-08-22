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
				return true; // this is a manager of the company page 
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

	$query = "SELECT *, UNIX_TIMESTAMP(`created`) AS `timestamp` FROM tm_newsfeed_status_updates WHERE id = :status_update_id LIMIT 1";
	$results = db_query($query, array(':status_update_id' => $status_update_id))->fetchAll();

	// no updates
	if (sizeof($results) == 0) {
		return null;
	}

	// return update
	return $results[0];
}

/**
 * Get a flagged item by id
 */
function tm_status_updates_get_flagged_by_id($flagging_id) {

	$query = "SELECT * FROM flagging WHERE flagging_id = :flagging_id LIMIT 1";
	$results = db_query($query, array(':flagging_id' => $flagging_id))->fetchAll();

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

	$query = "SELECT UNIX_TIMESTAMP(`created`) AS `timestamp`, 'status_update' AS result_type, id as status_update_id, poster_uid AS owner_uid FROM tm_newsfeed_status_updates WHERE  created >= DATE_SUB(NOW(), INTERVAL " . intval($days_old) . " DAY) AND is_promoted = 1 AND moderator_hide = 0 AND is_removed = 0 ORDER BY created DESC";

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
	if ($entity_type == "user") {
		$flag = flag_get_flag('status_updated_by_member');
		$flag->flag("unflag", $entity_id, null, true); // unflag so we can flag again
		$flag->flag("flag", $entity_id, null, true);
	}

	// Flag organization as having updated status
	if ($entity_type == "organization") {
		$flag = flag_get_flag('status_updated_by_organization');
		$flag->flag("unflag", $entity_id, null, true); // unflag so we can flag again
		$flag->flag("flag", $entity_id, null, true);
	}

	// Flag chapter as having updated status
	if ($entity_type == "chapter") {
		$flag = flag_get_flag('status_updated_by_chapter');
		$flag->flag("unflag", $entity_id, null, true); // unflag so we can flag again
		$flag->flag("flag", $entity_id, null, true);
	}

	// Flag event as having updated status
	// Note: not implemented yet
	/*if ($entity_type == "event") {
		$flag = flag_get_flag('status_updated_by_event');
		$flag->flag("unflag", $entity_id, null, true); // unflag so we can flag again
		$flag->flag("flag", $entity_id, null, true);
	}*/
	
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
		return false;
	}
	
}

/**
 * Poll
 */
function tm_status_updates_poll_results($feed_type = "global", $options = array(), $timestamp = null, $fetch_next = 25) {

	global $conf;

	// settings
	$MAX_FETCHES = 30; // how many fetches to try before we return results
	$POLL_INTERVAL = "P1D"; // poll interval 1 day intervals

	// if null use UTC
	$datetime_start = new Datetime("now + 10 second", new DateTimeZone("UTC"));
	$datetime_end = new Datetime("now", new DateTimeZone("UTC"));

	// get timestamp
	if ($timestamp != null) {
		$datetime_start = $datetime_start->setTimestamp(intval($timestamp));
		$datetime_end = $datetime_end->setTimestamp(intval($timestamp));
	}

	// set end date to poll interval (ie: 1 day)
	$datetime_end->sub(new DateInterval($POLL_INTERVAL));

	// calculate date limit
	$limit_results_days = $conf["tm_status_updates_limit_results_days"];
	$limit_datetime = new DateTime("now", new DateTimeZone("UTC"));
	$limit_datetime->sub(new DateInterval('P' . intval($limit_results_days) . 'D'));

	$i = 0;
	$results = array();
	$last_timestamp = null;
	while ((sizeof($results) < $fetch_next) and ($i++ < $MAX_FETCHES)) {

		// check limits
		if ($datetime_start < $limit_datetime) {
			$i = $MAX_FETCHES;
			continue;
		}
		if ($datetime_end < $limit_datetime) {
			$datetime_end = clone $limit_datetime;
		}

		// format time
		$start_date = $datetime_start->format('Y-m-d H:i:s');
		$end_date = $datetime_end->format('Y-m-d H:i:s');

		// global feed
		if ($feed_type == "global") {
			$fetch_results = tm_status_updates_get_combined_feed_global($start_date, $end_date, $fetch_next);
		}

		// news feed
		if ($feed_type == "newsfeed") {
			$fetch_results = tm_status_updates_get_combined_feed_newsfeed($options["account"], $start_date, $end_date, $fetch_next);
		}

		// tag feed
		if ($feed_type == "tags") {
			$fetch_results = tm_status_updates_get_matching_search_terms($options["tags"], $start_date, $end_date, $fetch_next);
		}

		// search feed
		if ($feed_type == "search") {
			$fetch_results = tm_status_updates_get_matching_search_terms($options["search_terms"], $start_date, $end_date, $fetch_next);
		}

		// search feed
		if ($feed_type == "near") {
			$fetch_results = tm_status_updates_get_near_location($options["latitude"], $options["longitude"], $options["range"], $start_date, $end_date, $fetch_next);
		}

		// merge results
		$results = array_merge($results, $fetch_results);
		
		// increment one day
		$datetime_start->sub(new DateInterval($POLL_INTERVAL));
		$datetime_end->sub(new DateInterval($POLL_INTERVAL));

	}

	// size results
	$results = array_slice($results, 0, $fetch_next);

	if (sizeof($results) == 0) {
		return array("results" => $results, "first_timestamp" => null, "last_timestamp" => null);
	}

	// get first timestamp
	$first_timestamp = $results[0]->timestamp;
	$last_timestamp = $results[sizeof($results) - 1]->timestamp;

	// return results and timestamp pointer for next fetch
	return array("results" => $results, "first_timestamp" => $first_timestamp, "last_timestamp" => $last_timestamp);
}

/**
 * Return a sorted list of flag and status updates for all accounts
 */
function tm_status_updates_get_combined_feed_global($start_date = null, $end_date = null, $limit = 100) {

	// Status updates
	$query1 = "SELECT UNIX_TIMESTAMP(`created`) AS `timestamp`,
				'status_update' as result_type,
				id as status_update_id,
				'' flagging_id,
				poster_uid as 'owner_uid',
				entity_id as status_update_entity_id, 
				entity_type as status_update_entity_type, 
				'' as flagged_entity_id, 
				'' as flagged_entity_type
				FROM tm_newsfeed_status_updates 
				WHERE is_removed = 0 AND moderator_hide = 0
				AND created < CONVERT_TZ(:start_date, '+00:00', @@session.time_zone)
				AND created > CONVERT_TZ(:end_date, '+00:00', @@session.time_zone)
				ORDER BY timestamp DESC";

	// Get list of flags that can be displayed
	$display_flags = array();
	$display_flags[] = flag_get_flag('approval_approved_by')->fid;
	$display_flags[] = flag_get_flag('event_waitlist')->fid;
	$display_flags[] = flag_get_flag('tm_sponsor')->fid;
	$display_flags[] = flag_get_flag('follow_members')->fid;
	$display_flags[] = flag_get_flag('follow_organizations')->fid;
	$display_flags[] = flag_get_flag('signup')->fid;
	$display_flags[] = flag_get_flag('event_register')->fid;
	if (module_exists("tm_subscriptions_user")) {
		$display_flags[] = flag_get_flag('user_subscription_enable')->fid;
	}
	if (module_exists("tm_payments")) {
		$display_flags[] = flag_get_flag('event_paid')->fid;
	}

	$query2 = "SELECT timestamp,
			'flagged_item' as result_type,
			'' status_update_id,
			flagging_id,
			uid as 'owner_uid',
			'' as status_update_entity_id, 
			'' as status_update_entity_type, 
			entity_id as flagged_entity_id, 
			entity_type as flagged_entity_type
			FROM flagging 
			WHERE fid IN (" . implode(",", $display_flags) . ") 
			AND timestamp < UNIX_TIMESTAMP(CONVERT_TZ(:start_date, '+00:00', @@session.time_zone)) 
			AND timestamp > UNIX_TIMESTAMP(CONVERT_TZ(:end_date, '+00:00', @@session.time_zone))
			ORDER BY timestamp DESC";

	$joined = "(" . $query1 . ") UNION ALL (" . $query2 . ") ORDER BY timestamp DESC LIMIT " . intval($limit);
	$all_results = db_query($joined, array(':start_date' => $start_date, ':end_date' => $end_date))->fetchAll();

	// return empty array if no results
	if ($all_results == null) {
		return array();
	}

	// return results
	return $all_results;
}

/**
 * Return a sorted list of flag and status updates for a user account
 */
function tm_status_updates_get_combined_feed_newsfeed($account, $start_date = null, $end_date = null, $limit = 100) {

	// Get list of entities which can be followed (flagged) that can post status updates
	$follow_flags = array();
	$follow_flags[] = flag_get_flag('follow_members')->fid; // members
	$follow_flags[] = flag_get_flag('follow_organizations')->fid; // organizations
	$follow_flags[] = flag_get_flag('signup')->fid; // chapter
	$follow_flags[] = flag_get_flag('event_register')->fid; // events

	// Get your status updates or those of people you are following
	$query1 = "SELECT UNIX_TIMESTAMP(`created`) AS `timestamp`,
				'status_update' AS result_type,
				id AS status_update_id,
				'' flagging_id,
				poster_uid AS 'owner_uid',
				entity_id AS status_update_entity_id,
				entity_type AS status_update_entity_type,
				'' AS flagged_entity_id,
				'' AS flagged_entity_type
				FROM tm_newsfeed_status_updates 
				WHERE ((entity_id IN (SELECT entity_id FROM flagging WHERE uid = :uid AND fid IN (" . implode(",", $follow_flags) . ")) OR entity_id = :uid) AND is_removed = 0 AND moderator_hide = 0) 
				AND created < CONVERT_TZ(:start_date, '+00:00', @@session.time_zone)
				AND created > CONVERT_TZ(:end_date, '+00:00', @@session.time_zone)
				ORDER BY timestamp DESC";

	// Show actions by people you are following (following, signup, event registration or waitlist)
	$display_flags = $follow_flags;
	$display_flags[] = flag_get_flag('event_waitlist')->fid;

	// Show people joining your chapter or registering to same event as you
	$associated_entity_flags = array();
	$associated_entity_flags[] = flag_get_flag('signup')->fid; // chapter
	$associated_entity_flags[] = flag_get_flag('event_register')->fid;

	$query2 = "SELECT timestamp,
			'flagged_item' as result_type,
			'' status_update_id,
			flagging_id,
			uid AS 'owner_uid',
			'' AS status_update_entity_id,
			'' AS status_update_entity_type,
			entity_id AS flagged_entity_id,
			entity_type AS flagged_entity_type
			FROM flagging 
			WHERE ((uid = :uid)
			OR (((uid IN (SELECT entity_id FROM flagging WHERE uid = :uid AND fid IN (" . implode(",", $display_flags) . ")) OR (entity_id = :uid AND entity_type = 'user')) AND fid IN (" . implode(",", $display_flags) . ")) 
			OR (entity_id in (SELECT entity_id from flagging where uid = :uid AND fid IN (" . implode(",", $associated_entity_flags) . ")))))
			AND timestamp < UNIX_TIMESTAMP(CONVERT_TZ(:start_date, '+00:00', @@session.time_zone)) 
			AND timestamp > UNIX_TIMESTAMP(CONVERT_TZ(:end_date, '+00:00', @@session.time_zone))
			ORDER BY timestamp DESC";

	$joined = "(" . $query1 . ") UNION ALL (" . $query2 . ") ORDER BY timestamp DESC LIMIT " . intval($limit);

	$all_results = db_query($joined, array(':uid' => $account->uid, ':start_date' => $start_date, ':end_date' => $end_date))->fetchAll();
	return $all_results;
}

/**
 * Return a sorted status updates that match a tag
 * $tags should be array prefixed with #. ie: array('#awesome', '#tag') or array('word');
 */
function tm_status_updates_get_matching_search_terms($search_terms, $start_date = null, $end_date = null, $limit = 100) {
	
	// check we have search terms
	if (sizeof($search_terms) == 0) {
		return array();
	}

	// get recent status update items
	$query = "SELECT UNIX_TIMESTAMP(`created`) AS `timestamp`, 'status_update' AS result_type, id AS status_update_id, poster_uid AS owner_uid,
		entity_id AS status_update_entity_id,
		entity_type AS status_update_entity_type
		FROM tm_newsfeed_status_updates 
		WHERE is_removed = 0 AND moderator_hide = 0
		AND created < CONVERT_TZ(:start_date, '+00:00', @@session.time_zone)
		AND created > CONVERT_TZ(:end_date, '+00:00', @@session.time_zone) ";

	$query_options = array(":start_date" => $start_date, ":end_date" => $end_date);
	for ($i = 0; $i < sizeof($search_terms); $i++) {
		$query .= " AND status_update LIKE :search_term" . $i;
		$query_options[':search_term' . $i] = '%' . $search_terms[$i] . '%';
	}

	$query .= " ORDER BY created DESC LIMIT " . intval($limit);
	$status_updates = db_query($query, $query_options)->fetchAll();

	return $status_updates;
}

/**
 * Search for updates near a geographic location, sorted by how close to the distance
 */
function tm_status_updates_get_near_location($latitude, $longitude, $range, $start_date, $end_date, $limit = 100) {


	// reference
	// http://stackoverflow.com/questions/2234204/latitude-longitude-find-nearest-latitude-longitude-complex-sql-or-complex-calc
	// converted to km

	// search for status updates near location
	$query = "SELECT *, 'status_update' AS result_type, id AS status_update_id, poster_uid AS owner_uid, entity_id as status_update_entity_id, 
				entity_type as status_update_entity_type, ";

	$query .= "SQRT(
		    POW(111.2 * (location_latitude - " . floatval($latitude) . "), 2) +
		    POW(111.2 * (" . floatval($longitude) . " - location_longitude) * COS(location_latitude / 57.3), 2)) AS distance";

	$query .= " FROM tm_newsfeed_status_updates WHERE location_latitude IS NOT null AND location_longitude IS NOT NULL AND is_removed = 0 AND moderator_hide = 0
	AND created < CONVERT_TZ(:start_date, '+00:00', @@session.time_zone)
	AND created > CONVERT_TZ(:end_date, '+00:00', @@session.time_zone) ";
	
	$query = "SELECT *, UNIX_TIMESTAMP(`created`) AS `timestamp` FROM (" . $query . ") AS distance_results WHERE distance < " . floatval($range) . " ORDER BY created DESC LIMIT " . intval($limit);

	$status_updates = db_query($query, array(":start_date" => $start_date, ":end_date" => $end_date))->fetchAll();
	return $status_updates;
}


/**
 * Get the timestamp of the last update for an entity type
 */
function tm_status_updates_get_last_update_timestamp($entity_type, $entity_id) {

	// fetch the timestamp of the user's most recent status update
	$query = "SELECT CONVERT_TZ(`created`, @@session.time_zone, '+00:00') AS `created_utc` FROM tm_newsfeed_status_updates WHERE entity_type = :entity_type AND entity_id = :entity_id ORDER BY created DESC LIMIT 1";
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

/**
 * Get organisations that have automatic newsfeeds
 * Returns array of nids
 */
function tm_status_updates_get_organization_newsfeeds() {

  $query = "SELECT entity_id FROM {field_data_field_company_newsfeed_url} f WHERE f.bundle = 'organization'";
  $result = db_query($query)->fetchAllKeyed(0,0);
  return $result;
}

/**
 * Delete all updates for a user
 */
function tm_status_updates_delete_users_updates($uid) {

	// remove status updates
	$query = "DELETE FROM tm_newsfeed_status_updates WHERE entity_id = :uid AND entity_type = 'user'";
	db_query($query, array(":uid" => $uid));

	// remove preview links
	$query = "DELETE FROM tm_newsfeed_preview_links WHERE creator_uid = :uid";
	db_query($query, array(":uid" => $uid));
	
}
