<?php

// tm_reports.csv.inc - export csv methods

/**
 * Download chapter insights in csv format
 */
function tm_reports_download_chapter_insights_csv(){

	global $conf;
	global $user;

	$oldzone = date_default_timezone_get();
	date_default_timezone_set(drupal_get_user_timezone());
	$csvname = strtolower(str_replace(" ", "_", $conf['tm_site_name'])) . '_chapter_insights_'.date('M_d_Y_Hi',time()) . ".csv";
	$csvname = str_replace(array(","," "), "_", $csvname); // replace spaces and ,
	$csvname = "confidential_" . $csvname;
	date_default_timezone_set($oldzone);

	// csv header
	drupal_add_http_header('Content-Type', 'text/csv; utf-8');
	drupal_add_http_header('Content-Disposition', 'attachment; filename = '.$csvname);

	// check cache
	$cache = cache_get('tm-reports-all-chapter-insights', 'cache');
	if (!empty($cache)) {
		foreach ($cache->data as $row) {
			echo(implode(",", $row));
			echo("\n");
		}
	} else {
		echo "Please run drush tm-reports-chapter-insights-cache to generate csv";
	}
}

/**
 * Fetch and cache all global reports
 */
function tm_reports_chapter_insights_get_data($drush_command = false, $fetch_new = false) {

	// cache for 1 day
	$cache_seconds = 60 * 60 * 24;

	// check cache
  	$cache = cache_get('tm-reports-all-chapter-insights', 'cache');
  	if ((!$fetch_new) and (!empty($cache))) {
    	return $cache->data;
  	}

	// Get all chapters
	$query = "SELECT nid, title FROM node WHERE type = 'chapter' AND status = 1 ORDER BY title ASC";
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

		if ($drush_command) {
			drush_print("Calculating report for chapter: " . $title . "...");
		}

		$chapter = node_load($nid);

		// fetch results
		$chapter_results = _tm_reports_get_chapter_insights($nid, true, $cache_seconds);

		// get country and continent
		$country_code = "NONE";
		$country_name = "NONE";
		$continent_code = "NONE";
		$continent_name = "NONE";
		if (isset($chapter->field_country[LANGUAGE_NONE][0]['iso2'])) {
			$country_code = $chapter->field_country[LANGUAGE_NONE][0]['iso2'];
			$country = country_load($country_code);
			$country_name = str_replace(",", " - ", $country->name);
			$continent_code = str_replace(",", " - ", $country->continent);
			$continent_name = str_replace(",", " - ", $continents[$continent_code]);
		}

		// create header row
		if ($first_row) {
			$row = array("chapter_id", "chapter_name", "country_name", "continent_name", "country_code", "continent_code", "chapter_age_days");
			foreach ($chapter_results['data_values'] as $key => $value) {
				$row[] = str_replace(",", "", $key);
			}
			$first_row = false;
			$rows[] = $row;
		}

		// create data row
		$datediff = time() - $chapter->created;
		$chapter_days_old = round($datediff / (60 * 60 * 24));
		$row = array($chapter->nid, $chapter->title, $country_name, $continent_name, $country_code, $continent_code, $chapter_days_old);
		foreach ($chapter_results['data_values'] as $key => $value) {
			$row[] = str_replace(",", "", $value);
		}
		$rows[] = $row;
	}

	// return api data in json format
  	cache_set('tm-reports-all-chapter-insights', $rows, 'cache', time() + (60 * 60 * 25)); // 25 hours
  	return $rows;
}
