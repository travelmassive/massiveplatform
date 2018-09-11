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

	$items['tm-reports-regions-insights-cache'] = array(
	  'description' => 'Run and cache regions insights report',
	  'callback' => 'tm_reports_region_insights_cache',
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
	$rows = tm_reports_chapter_insights_get_data(true, true);
	drush_print("Done.");
}

/**
 * Cache region reports and cache
 */
function tm_reports_region_insights_cache() {

	// cache for 1 day
	$cache_seconds = 60 * 60 * 24;

	// get data
	$starttime = microtime(true);
	drush_print("\nProcessing country insights...");

	// get countries that have a chapter
	$countries = _tm_chapters_get_countries_with_chapters();
	foreach($countries as $country_iso2) {
		$chapter_ids = _tm_chapters_get_chapters_in_country($country_iso2);
		drush_print("Processing country insights for " . $country_iso2 . " (" . sizeof($chapter_ids) . " chapters) ...");
		_tm_reports_get_chapter_insights($chapter_ids, true, $cache_seconds);
	}

	// get all continents
	$continents = array("AF", "AN", "AS", "EU", "NA", "OC", "SA");
	foreach($continents as $continent) {
		$chapter_ids = _tm_chapters_get_chapters_in_continent($continent);
		drush_print("Processing continent insights for " . $continent . " (" . sizeof($chapter_ids) . " chapters) ...");
		_tm_reports_get_chapter_insights($chapter_ids, true, $cache_seconds);
	}

	$endtime = microtime(true);
	$timediff = $endtime - $starttime;
	drush_print("Done (" . round($timediff, 2) . " seconds).");
	// drush_print("You can fetch the CSV from admin/export_global_newsletter");
}

