<?php

// tm_match.controller.inc

// suggest some uids
function tm_match_suggest_uids($uid, $limit = 5) {

	// You mission, if you choose to accept it.
	// Find people to match with.

	$query = "SELECT uid from users where uid > 0";
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

	// check swipes
	$swiped_already = tm_match_get_all_swipes($uid);

	// filter
	$filtered_uids = array();
	foreach($uids as $uid) {
		if (!in_array($uid, $swiped_already)) {
			$filtered_uids[] = $uid;
		}
	}

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
function tm_match_swipe_right($uid, $right_uid) {
	
	$timestamp = time();
	
	$fields = array(
		'uid' => $uid,
		'right_uid' => $right_uid,
		'timestamp' => $timestamp
	);

	$result = drupal_write_record('tm_match', $fields);
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

	$query = "SELECT DISTINCT IFNULL(left_uid, right_uid) as uid from tm_match WHERE uid = :uid";

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

	$query = "SELECT COUNT(DISTINCT IFNULL(left_uid, right_uid)) as total_swipes from tm_match WHERE uid = :uid AND timestamp > :timestamp";

	$result = db_query($query, array(":uid" => $uid, ":timestamp" => $timestamp))->fetch();

	if ($result == null) {
		return 0;
	}

	return $result->total_swipes;
}


// check how many swipes
function tm_match_count_all_right_swipes($uid, $timestamp = 0) {

	$query = "SELECT COUNT(DISTINCT right_uid) as total_swipes from tm_match WHERE uid = :uid AND timestamp > :timestamp";

	$result = db_query($query, array(":uid" => $uid, ":timestamp" => $timestamp))->fetch();

	if ($result == null) {
		return 0;
	}

	return $result->total_swipes;
}
