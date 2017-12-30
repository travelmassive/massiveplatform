<?php

// tm_users.drush.inc - drush commands

/**
 * Implements hook_drush_command().
 */
function tm_reports_drush_command() {

	$items['tm-reports-chapter-insights'] = array(
	  'description' => 'Run and display chapter insights report',
	  'callback' => 'tm_reports_chapter_insights_drush',
	);

	$items['tm-reports-chapter-insights-cache'] = array(
	  'description' => 'Run and cache chapter insights report',
	  'callback' => 'tm_reports_chapter_insights_cache',
	);

	return $items;
}

/**
 * Run global reports and output
 */
function tm_reports_chapter_insights_drush() {

	// get data and fetch new
	$rows = tm_reports_chapter_insights_get_data(true, true);

	// print in csv format
	drush_print("#Chapter insights");
	foreach ($rows as $row) {
		drush_print(implode(",", $row));
	}
}

/**
 * Cache global reports and cache
 */
function tm_reports_chapter_insights_cache() {

	// get data
	drush_print("\nProcessing chapter insights...");
	$rows = tm_reports_chapter_insights_get_data(true);
	drush_print("Done.");
}

