<?php

// tm_match.controller.inc

// suggest some uids
function tm_match_suggest_uids($uid, $limit = 10) {

	// You mission, if you choose to accept it.
	// Find people to match with.

	global $conf;

	$account = user_load($uid);
	if ($account == null) {
		return array();
	}

	$query = "SELECT uid from users where uid > 0 ORDER BY RAND() LIMIT " . intval($limit);
	$results = db_query($query)->fetchAll();

	// check results
	if ($results == null) {
		return array();
	}

	// turn into array
	$uids = array();
	foreach($results as $result) {
		$uids[] = $result->uid;
	}

	// All uids
	$suggested_uids = array();

	// 1. Follows you
	$follows_you_uids = array();
	$follow_members_flag = flag_get_flag('follow_members');
	$query = "SELECT uid FROM flagging WHERE fid = :fid AND entity_id = :entity_id ORDER BY RAND() LIMIT " . $limit;
	$results = db_query($query, array(":fid" => $follow_members_flag->fid, ":entity_id" => $uid))->fetchAll();
	foreach($results as $result) {
		$follows_you_uids[] = $result->uid;
	}

	// 2. Members in your chapter
	$chapter_ids = tm_users_get_chapters($account);
	if (sizeof($chapter_ids) > 0) {
		$joined_chapter_flag = flag_get_flag('signup');
		$query = "SELECT DISTINCT(uid) as uid FROM flagging WHERE entity_id IN(" . implode(",", $chapter_ids) . ") AND fid = :fid ORDER BY RAND() LIMIT 10";
		$results = db_query($query, array(":fid" => $joined_chapter_flag->fid))->fetchAll();
		foreach($results as $result) {
			$suggested_uids[] = $result->uid;
		}
	}

	// 3. Members in your country
	if (isset($account->field_user_country[LANGUAGE_NONE][0]['iso2'])) {
		$country = $account->field_user_country[LANGUAGE_NONE][0]['iso2'];
		$query = "SELECT entity_id FROM field_data_field_user_country WHERE field_user_country_iso2 = :iso2 ORDER BY RAND() LIMIT 10";
		$results = db_query($query, array(":iso2" => $country))->fetchAll();
		foreach($results as $result) {
			$suggested_uids[] = $result->entity_id;
		}
	}

	// 4. Members in your location
	if (isset($account->field_location_city[LANGUAGE_NONE][0]['value'])) {
		$location = trim(explode(",", $account->field_location_city[LANGUAGE_NONE][0]['value'])[0]); // get first part of location
		if ($location != "") {
			$query = "SELECT entity_id FROM field_data_field_location_city WHERE entity_type = 'user' AND field_location_city_value = :location ORDER BY RAND() LIMIT 10";
			$results = db_query($query, array(":location" => $location))->fetchAll();
			foreach($results as $result) {
				$suggested_uids[] = $result->entity_id;
			}
		}
	}

	// 5. Members in your segment
	$field_segment_tid = tm_users_get_taxonomy_tid($uid);
	if ($field_segment_tid != null) {
		$query = "SELECT entity_id FROM field_data_field_segment WHERE entity_type = 'user' AND field_segment_tid = :tid ORDER BY RAND() LIMIT 10";
		$results = db_query($query, array(":tid" => $field_segment_tid))->fetchAll();
		foreach($results as $result) {
			$suggested_uids[] = $result->entity_id;
		}
	}

	// 6. Subscribed Members
	$user_subscription_enable_flag = flag_get_flag('user_subscription_enable');
	if ($user_subscription_enable_flag != null) {
		$query = "SELECT entity_id FROM flagging WHERE fid = :fid ORDER BY RAND() LIMIT 10";
		$results = db_query($query, array(":fid" => $user_subscription_enable_flag->fid))->fetchAll();
		foreach($results as $result) {
			$suggested_uids[] = $result->entity_id;
		}
	}

	// 7. Featured Members
	$max_featured_uids = 10;
	if (sizeof($conf["tm_match_suggest_featured_uids"]) < $max_featured_uids) {
		$max_featured_uids = sizeof($conf["tm_match_suggest_featured_uids"]);
	}
	$featured_member_uids = array_rand($conf["tm_match_suggest_featured_uids"], $max_featured_uids);
	foreach ($featured_member_uids as $suggested_uid) {
		$suggested_uids[] = $suggested_uid;
	}

	// 8. Chapter Leaders
	$chapter_leader_role = user_role_load_by_name("chapter leader");
	$query = "SELECT uid FROM users_roles WHERE rid = :rid ORDER BY RAND() LIMIT 10";
	$results = db_query($query, array(":rid" => $chapter_leader_role->rid))->fetchAll();
	foreach($results as $result) {
		$suggested_uids[] = $result->uid;
	}

	// 9. I'm lucky members
	$register_flag = flag_get_flag("event_register"); // $register_flag->fid
	// select random users who has registered for event in past year
	$query_sql = "SELECT uid FROM flagging WHERE fid = :fid AND timestamp >= UNIX_TIMESTAMP(DATE_SUB(NOW(),INTERVAL 1 YEAR)) ORDER BY RAND() LIMIT 10";
	$query = db_query($query_sql, array(':fid' => $register_flag->fid));
	$results = $query->fetchAll();
	foreach ($results as $result) {
		$suggested_uids[] = $result->uid;
	}

	// 10. Logged in in past 24 hours
	$query_sql = "SELECT DISTINCT(uid) as uid FROM ip_tracker WHERE last_visit >= UNIX_TIMESTAMP(DATE_SUB(NOW(),INTERVAL 1 DAY)) ORDER BY RAND() LIMIT 10";
	$query = db_query($query_sql);
	$results = $query->fetchAll();
	foreach ($results as $result) {
		$suggested_uids[] = $result->uid;
	}

	// 11. Lots of followers
	$query_sql = "SELECT entity_id FROM flag_counts WHERE entity_type = 'user' AND count > 50 ORDER BY RAND() LIMIT 10";
	$query = db_query($query_sql);
	$results = $query->fetchAll();
	foreach ($results as $result) {
		$suggested_uids[] = $result->entity_id;
	}

	// 12. Has been featured in a list
	if (module_exists("tm_lists")) {
		$query_sql = "SELECT DISTINCT(entity_id) as entity_id FROM tm_lists WHERE entity_type = 1 ORDER BY RAND() LIMIT 10";
		$query = db_query($query_sql);
		$results = $query->fetchAll();
		foreach ($results as $result) {
			$suggested_uids[] = $result->entity_id;
		}
	}

	// Unique uids
	$suggested_unique_uids = array_unique($suggested_uids); 

	// Get 20 random suggested
	$pick_items = $limit * 2;
	if (sizeof($suggested_unique_uids) < $pick_items) {
		$pick_items = sizeof($suggested_unique_uids);
	}
	shuffle($suggested_unique_uids);
	$suggested_unique_uids = array_slice($suggested_unique_uids, 0, $pick_items);
	
	// Join the 10 followers, with 20 other randoms suggested
	$suggested_all = array_unique(array_merge($follows_you_uids, $suggested_unique_uids));

	// Check swipes
	$swiped_already = tm_match_get_all_swipes($uid);

	// Filter
	$suggested_filtered = array();
	foreach ($suggested_all as $suggested_uid) {

		// Don't show yourself
		if ($suggested_uid == $uid) {
			continue;
		}

		// No anonymous or admin
		if ($suggested_uid <= 1) {
			continue;
		}

		// Not swiped already
		if (in_array($suggested_uid, $swiped_already)) {
			continue;
		}

		// Validate user
		$check_user = user_load($suggested_uid);
		if ($uid == null) {
			continue;
		}

		// Must be approved profiles
		if (!in_array("approved user", $check_user->roles)) {
			continue;
		}

		// Filter out reported
		if (tm_users_is_member_reported($suggested_uid)) {
			continue;
		}

		// Filter out private profiles
		if (isset($check_user->field_user_has_private_profile[LANGUAGE_NONE][0]['value'])) {
			if ($check_user->field_user_has_private_profile[LANGUAGE_NONE][0]['value']) {
				continue;
			}
		}

		// You don't already follow them
		if (tm_match_user_is_following($uid, $check_user->uid, $follow_members_flag->fid)) {
			continue;
		}

		// Add it
		$suggested_filtered[] = $suggested_uid;
	}

	// Shuffle and limit
	if (sizeof($suggested_filtered) < $limit) {
		$limit = sizeof($suggested_filtered);
	}
	shuffle($suggested_filtered);
	$filtered_uids = array_slice($suggested_filtered, 0, $limit);

	// Return suggestions
	return $filtered_uids;

}

// swipe left
function tm_match_swipe_left($uid, $left_uid) {
	
	$timestamp = time();

	$fields = array(
		'uid' => $uid,
		'left_uid' => $left_uid,
		'timestamp' => $timestamp
	);

	$result = drupal_write_record('tm_match', $fields);
	return $result;
}

// swipe right
function tm_match_swipe_right($uid, $right_uid, $timestamp = null) {
	
	global $conf;

	// configure timestamp
	if ($timestamp == null) {
		$timestamp = time();
	}	
	
	$fields = array(
		'uid' => $uid,
		'right_uid' => $right_uid,
		'timestamp' => $timestamp
	);

	$result = drupal_write_record('tm_match', $fields);

	// demo mode
	if ($conf["tm_match_demo_mode"]) {
		return $result;
	}

	// Set static variables for tm_notifications_new_follower():
	$static_tm_match_swiped_right = &drupal_static("tm_match_swiped_right");
	$static_tm_match_swiped_right = true;

	// Follow the mnember
	$account = user_load($uid);
	$right_user = user_load($right_uid);
	if (($account != null) and ($right_user != null)) {	
		$flag = flag_get_flag('follow_members');
		$flag->flag('flag', $right_user->uid, $account, TRUE);
	}

	return $result;
	
}

// check limit
// based on left swipes
function tm_match_check_rate_limit($uid) {

	global $conf;

	$max_left_swipes = $conf["tm_match_swipe_limit_right"];
	$total_left_swipes = tm_match_count_all_right_swipes($uid, $conf["tm_match_swipe_limit_period"]);

	if ($total_left_swipes > $max_left_swipes) {
		return false;
	}

	// ok
	return true;
}

// get all uids
function tm_match_get_all_swipes($uid) {

	$query = "SELECT DISTINCT IFNULL(left_uid, right_uid) AS uid FROM tm_match WHERE uid = :uid";

	$results = db_query($query, array(":uid" => $uid))->fetchAll();

	if ($results == null) {
		return array();
	}

	// turn into array
	$uids = array();
	foreach($results as $result) {
		$uids[] = $result->uid;
	}

	return $uids;
}

// check how many swipes
function tm_match_count_all_swipes($uid, $timestamp = 0) {

	$query = "SELECT COUNT(DISTINCT IFNULL(left_uid, right_uid)) AS total_swipes FROM tm_match WHERE uid = :uid AND timestamp > :timestamp";

	$result = db_query($query, array(":uid" => $uid, ":timestamp" => $timestamp))->fetch();

	if ($result == null) {
		return 0;
	}

	return $result->total_swipes;
}

// check how many swipes
function tm_match_count_all_right_swipes($uid, $timestamp = 0) {

	$query = "SELECT COUNT(DISTINCT right_uid) AS total_swipes FROM tm_match WHERE uid = :uid AND timestamp > :timestamp";

	$result = db_query($query, array(":uid" => $uid, ":timestamp" => $timestamp))->fetch();

	if ($result == null) {
		return 0;
	}

	return $result->total_swipes;
}

// check is someone has swiped right on another member
function tm_match_user_has_swiped_right($uid, $right_uid) {

	$query = "SELECT COUNT(DISTINCT right_uid) AS total_swipes FROM tm_match WHERE uid = :uid AND right_uid = :right_uid";
	$result = db_query($query, array(":uid" => $uid, ":right_uid" => $right_uid))->fetch();
	return $result->total_swipes;

}

// quick lookup if user is following another user
function tm_match_user_is_following($uid_of_follower, $uid, $follow_members_flag_id = null) {

	// fetch flag if not provided	
	if ($follow_members_flag_id == null) {
		$follow_members_flag_id = flag_get_flag('follow_members')->fid;
	}

	// check if card_uid is following uid
	$query = "SELECT entity_id FROM flagging WHERE fid = :fid AND uid = :uid AND entity_id = :entity_id";
	$results = db_query($query, array(":fid" => $follow_members_flag_id, ":uid" => $uid_of_follower, ":entity_id" => $uid))->fetchAll();
	return (sizeof($results) > 0);
}

