<?php

// tm_leaderboard.scores.inc - calculate scores

/**
 * Calculate all chapters scores
 */
function tm_leaderboard_calculate_scores() {

	// check cache
	$cache = cache_get('tm-reports-all-chapter-insights', 'cache');
	if (empty($cache)) {
		return null;
	}

	// test data
	// $cache = tm_leaderboard_get_test_data();
	
	// helper with column names
	$column_names = $cache->data[0];
	$column_name_lookup = array();
	foreach ($column_names as $val => $name) {
		$column_name_lookup[$name] = $val;
	}

	// process each chapter
	$count = 0;
	$scores = array();
	foreach ($cache->data as $row) {
		
		$count++;
		if ($count == 1) {
			continue;
		}

		$chapter_name = $row[$column_name_lookup["chapter_name"]];
		$chapter_score = tm_leaderboard_calculate_chapter_score($column_name_lookup, $row);
		$scores[$chapter_name] = $chapter_score;
	}

	// sort and return leaderboard
	arsort($scores);
	return $scores;
}

/**
 * Calculate scores for a chapter
 */
function tm_leaderboard_calculate_chapter_score($column_name_lookup, $data) {

	// total members
	$members_total = $data[$column_name_lookup["members_total"]];

	// events
	$num_events_past_12_months = $data[$column_name_lookup["num_events_past_12_months"]];

	// registrations
	$num_event_registrations_12_months = $data[$column_name_lookup["num_event_registrations_12_months"]];

	// member connections
	$total_chapter_member_connections = $data[$column_name_lookup["total_chapter_member_connections"]];

	// total members
	$members_total = $data[$column_name_lookup["members_total"]];

	// total approved members
	$members_approved_total = $data[$column_name_lookup["members_approved_total"]];

	// overall scores
	$score = ($num_events_past_12_months * 100) + ($num_event_registrations_12_months * 10) + ($members_approved_total / 2);
	
	// round score
	$score = round($score / 10, 0);

	return $score;
}
