<?php

// tm_users.drush.inc - drush commands

/**
 * Implements hook_drush_command().
 */
function tm_reports_drush_command() {

	$items['tm-reports-global-rankings'] = array(
	  'description' => 'Run global rankings report',
	  'callback' => 'tm_reports_global_rankings_drush',
	);

	return $items;
}

/**
 * Run global reports
 */
function tm_reports_global_rankings_drush() {

	// Get all chapters
	$query = "SELECT nid, title FROM node WHERE type = 'chapter' AND status = 1";
	$results = db_query($query)->fetchAll();
	$chapter_nids = array();
	foreach ($results as $result) {
	  $chapter_nids[$result->title] = $result->nid;
	}

	// get continents
	$continents = countries_get_continents();

	$rows = array();
	$first_row = true;
	foreach ($chapter_nids as $title => $nid) {

		drush_print("Calculating report for chapter: " . $title . "...");
		$chapter = node_load($nid);

		// fetch results
		$chapter_results = _tm_reports_get_chapter_insights($nid);

		// get country and continent
		$country_code = "NONE";
		$country_name = "NONE";
		$continent_code = "NONE";
		$continent_name = "NONE";
		if (isset($chapter->field_country[LANGUAGE_NONE][0]['iso2'])) {
			$country_code = $chapter->field_country[LANGUAGE_NONE][0]['iso2'];
			$country = country_load($country_code);
			$country_name = $country->name;
			$continent_code = $country->continent;
			$continent_name = $continents[$continent_code];
		}

		// create header row
		if ($first_row) {
			$row = array("chapter_name", "country_name", "continent_name", "country_code", "continent_code");
			foreach ($chapter_results['data_values'] as $key => $value) {
				$row[] = str_replace(",", "", $key);
			}
			$first_row = false;
		}
		$rows[] = $row;

		// create data row
		$row = array($chapter->title, $country_name, $continent_name, $country_code, $continent_code);
		foreach ($chapter_results['data_values'] as $key => $value) {
			$row[] = str_replace(",", "", $value);
		}
		$rows[] = $row;
		//$row_sting = implode(",", $row)		
	}

	drush_print("\nCSV export...");

	// print column labels
	drush_print();
	drush_print("#Column names");
	drush_print("column name, description");
	drush_print("chapter_name, Chapter Name");
	drush_print("country_name, Country Name");
	drush_print("continent_name, Continent Name");
	drush_print("country_code, Country Code");
	drush_print("continent_code, Continent Code");
	foreach ($chapter_results['data_labels'] as $key => $value) {
		drush_print($key . ", " . $value);
	}

	// print in csv format
	drush_print();
	drush_print("#Report data");
	foreach ($rows as $row) {
		drush_print(implode(",", $row));
	}
}






















