<?php

// tm_match.stats.inc

function tm_match_stats_total_users() {

	$query = "SELECT COUNT(DISTINCT uid) AS total FROM tm_match";
	$result = db_query($query)->fetch();
	return $result->total;
}

function tm_match_stats_total_swipes() {

	$query = "SELECT COUNT(*) AS total FROM tm_match";
	$result = db_query($query)->fetch();
	return $result->total;

}

function tm_match_stats_total_matches() {

	$query = "SELECT COUNT(*)/2 as total FROM tm_match m1 JOIN tm_match m2 ON m1.uid = m2.right_uid WHERE m1.right_uid = m2.uid";
	$result = db_query($query)->fetch();
	return intval($result->total);
	
}

