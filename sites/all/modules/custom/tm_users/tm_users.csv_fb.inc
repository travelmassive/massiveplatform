<?php

// tm_users.csv_fb.inc - export facebook csv methods

/**
 * Check if user can download facebook customer data
 * Requires export-global-newsletter-csv role
 */
function tm_users_download_facebook_csv_check() {

	global $user;

	// Check administrator or export-global-newsletter-csv
	if (in_array('administrator', $user->roles) or (in_array('export-global-newsletter-csv', $user->roles))) {
      return true;
    }

	// not valid
	return false;
}

/**
 * Facebook customer data function 
 */
function tm_users_download_facebook_csv(){

	global $conf;
	global $user;

	$oldzone = date_default_timezone_get();
	date_default_timezone_set(drupal_get_user_timezone());
	$csvname = strtolower(str_replace(" ", "_", $conf['tm_site_name'])) . '_facebook_customers_'.date('M_d_Y_Hi',time()) . ".csv";
	$csvname = str_replace(array(","," "), "_", $csvname); // replace spaces and ,
	$csvname = "confidential_" . $csvname;
	date_default_timezone_set($oldzone);

	// csv header
	drupal_add_http_header('Content-Type', 'text/csv; utf-8');
	drupal_add_http_header('Content-Disposition', 'attachment; filename = '.$csvname);
	// check cache
	$cache = cache_get('tm-users-facebook-csv', 'cache');
	if (!empty($cache)) {
		$uncompressed = gzuncompress($cache->data);
		echo $uncompressed;
	} else {
		echo "Please run drush tm-generate-facebook-csv to generate csv";
	}
}


/*
 * Facebook Customer Data function 
 */
function tm_users_download_facebook_customers_generate($drush_progress = false) {

	global $conf;
	
	$fh = fopen('php://memory', 'w');

	// puts a header row across the top of the csv
	$item = array('tm_user_id', 'email', 'fn', 'ln', 'city', 'country', 'uid', 'value');
	fputcsv($fh, $item);

	// fetch customer data
	// note: don't include anyone who has opted out of analytics
	// note: value is determined by number of events attended
	// note: uid is based on Facebook login
	$customer_data_sql = "select uid as drupal_uid, mail as email, field_user_first_name_value as fn, field_user_last_name_value as ln, ifnull(SUBSTRING_INDEX(`location_field`, ',', 1), home_chapter) as ct, ifnull(country, home_chapter_country) as country, fbuid as uid from (
select field_user_first_name_value, field_user_last_name_value, mail, field_user_country_iso2 as country, field_location_city_value as location_field, chapter.title as home_chapter, field_data_field_country.field_country_iso2 as home_chapter_country, ud.uid, field_data_field_facebook_signin_id.field_facebook_signin_id_value as fbuid from (select field_user_first_name_value, field_user_last_name_value, mail, uid, created from users u, field_data_field_user_first_name fn, field_data_field_user_last_name ln where u.uid = fn.entity_id and u.uid = ln.entity_id) as ud 
left join tm_notifications n on ud.uid = n.uid and n.bundle = 'global_newsletter'
left join field_data_field_user_country uc on uc.entity_id = ud.uid and uc.entity_type = 'user'
left join field_data_field_location_city uloc on uloc.entity_id = ud.uid and uloc.entity_type = 'user'
left join field_data_field_home_chapter uchapter on uchapter.entity_id = ud.uid and uchapter.entity_type = 'user' 
left join node chapter on chapter.nid = uchapter.field_home_chapter_target_id
left join field_data_field_country on field_data_field_country.entity_id = chapter.nid and field_data_field_country.bundle = 'chapter'
left join field_data_field_facebook_signin_id on field_data_field_facebook_signin_id.entity_id = ud.uid
left join field_data_field_user_disable_analytics on field_data_field_user_disable_analytics.entity_id = ud.uid
where n.value = 1
and ((field_data_field_user_disable_analytics.field_user_disable_analytics_value is null) or (field_data_field_user_disable_analytics.field_user_disable_analytics_value <> 1))  
order by ud.uid asc
) as user_data";

	// get event register flag
	$event_register_flag = flag_get_flag("event_register");
	
	// perform query
	$query = db_query($customer_data_sql);
	$count = 1;
	$starttime = microtime(true);
	$row_count = 0;
	while ($row = $query->fetchAssoc()) {

		// show progress in drush
		if ($drush_progress) {
			if (($count % 1000) == 0) {

				$endtime = microtime(true);
				$timediff = $endtime - $starttime;
				$per_second = round(1000 / $timediff, 2);
				drush_print("Processed " . $count . " rows... (" . $per_second . "/s)");
				$starttime = microtime(true);
				
			}
		}
		$count++;

		// remove city if home chapter has site name
		// ie: Massive Live
		if (stripos($row["ct"], $conf["tm_site_name"]) !== false) {
			$row["ct"] = "";
		}

		// (value) number of events registered for
		$user_id = $row["drupal_uid"];
		$num_events = tm_users_get_num_registered_events($user_id, $event_register_flag);
		$row[] = $num_events;

		fputcsv($fh, $row);
	}

	rewind($fh);

	// put it all in a variable
	$csv = stream_get_contents($fh);

	// close file handle
	fclose($fh);

	// compress
	$compressed = gzcompress($csv, 9);

	// return compressed csv
	return $compressed;
}
