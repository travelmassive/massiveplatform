<?php

// tm_newsfeed_updates.contoller.inc

/**
 * Return a sorted list of flag and status updates for all accounts
 */
function tm_newsfeed_updates_get_combined_feed_global($timestamp = 0, $exclude_uid = 1, $limit = 100) {

	global $conf;
	
	// Get list of flags that can be displayed
	// Check for ids in config first for better performance
	if (isset($conf["tm_newsfeed_updates_flag_ids"])) {
		$display_flags = $conf["tm_newsfeed_updates_flag_ids"];
	} else {
		$display_flags = array();
		$display_flags[] = flag_get_flag('approval_approved_by')->fid;
		$display_flags[] = flag_get_flag('event_waitlist')->fid;
		$display_flags[] = flag_get_flag('tm_sponsor')->fid;
		$display_flags[] = flag_get_flag('follow_members')->fid;
		$display_flags[] = flag_get_flag('follow_organizations')->fid;
		$display_flags[] = flag_get_flag('signup')->fid;
		$display_flags[] = flag_get_flag('event_register')->fid;
		if (module_exists("tm_subscriptions")) {
			$display_flags[] = flag_get_flag('organization_subscription_enable')->fid;
		}
		if (module_exists("tm_subscriptions_user")) {
			$display_flags[] = flag_get_flag('user_subscription_enable')->fid;
		}
	}

	$query = "SELECT timestamp,
			fid,
			'flagged_item' as result_type,
			flagging_id,
			uid,
			entity_id, 
			entity_type
			FROM flagging 
			WHERE fid IN (" . implode(",", $display_flags) . ") 
			AND timestamp > :timestamp 
			AND uid <> :exclude_uid
			ORDER BY timestamp DESC 
			LIMIT " . intval($limit);

	$all_results = db_query($query, array(":timestamp" => $timestamp, ":exclude_uid" => $exclude_uid))->fetchAll();

	// return empty array if no results
	if ($all_results == null) {
		return array();
	}

	// return results
	return $all_results;
}
