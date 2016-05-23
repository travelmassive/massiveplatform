<?php

// tm_users.csv.inc - export csv methods

/**
 * Check if user can download global newsletter list
 * Requires export-global-newsletter-csv role
 */
function tm_users_download_global_newsletter_csv_check() {

	global $user;

	// Check administrator or export-global-newsletter-csv
	if (in_array('administrator', $user->roles) or (in_array('export-global-newsletter-csv', $user->roles))) {
      return true;
    }

	// not valid
	return false;
}

/**
 * Check if user can download global newsletter list
 * Requires moderator role
 */
function tm_users_download_chapter_leaders_csv_check() {

	global $user;

	// Check administrator or export-global-newsletter-csv
	if (in_array('administrator', $user->roles) or (in_array('moderator', $user->roles))) {
      return true;
    }

	// not valid
	return false;
}

/*
 * Global Newsletter Subscribers function 
 */
function tm_users_download_global_newsletter_csv(){

	global $conf;
	global $user;

	$oldzone = date_default_timezone_get();
	date_default_timezone_set(drupal_get_user_timezone());
	$csvname = strtolower(str_replace(" ", "_", $conf['tm_site_name'])) . '_global_newsletter_'.date('M_d_Y_Hi',time()) . ".csv";
	$csvname = str_replace(array(","," "), "_", $csvname); // replace spaces and ,
	date_default_timezone_set($oldzone);

	// csv header
	drupal_add_http_header('Content-Type', 'text/csv; utf-8');
	drupal_add_http_header('Content-Disposition', 'attachment; filename = '.$csvname);

	$fh = fopen('php://output', 'w');

	// put privacy information in the header
	$message = "# IMPORTANT: This information is confidential and may not be transmitted to any 3rd party.";
	$message .= "\n# You may only use this data in compliance with the " . $conf['tm_site_name'] . " Privacy Terms.\n\n";
	fputs($fh,$message,strlen($message));

	// puts a header row across the top of the csv
	$item = array(t('First name'),t('Last name'),t('Country Code'),t('Location'),t('Home Chapter'),t('Home Chapter Country'),t('Email'));
	fputcsv($fh, $item);

	// fetch newsletter subscribers
	$global_newsletter_sql = "select field_user_first_name_value, field_user_last_name_value, field_user_country_iso2 as country, field_location_city_value as location_field, chapter.title as home_chapter, field_data_field_country.field_country_iso2 as home_chapter_country, mail from (select field_user_first_name_value, field_user_last_name_value, mail, uid from users u, field_data_field_user_first_name fn, field_data_field_user_last_name ln where u.uid = fn.entity_id and u.uid = ln.entity_id) as ud 
left join tm_notifications n on ud.uid = n.uid and n.bundle = 'global_newsletter'
left join field_data_field_user_country uc on uc.entity_id = ud.uid and uc.entity_type = 'user'
left join field_data_field_location_city uloc on uloc.entity_id = ud.uid and uloc.entity_type = 'user'
left join field_data_field_home_chapter uchapter on uchapter.entity_id = ud.uid and uchapter.entity_type = 'user' 
left join node chapter on chapter.nid = uchapter.field_home_chapter_target_id
left join field_data_field_country on field_data_field_country.entity_id = chapter.nid and field_data_field_country.bundle = 'chapter'
where n.value = 1";
	
	// perform query
	$query = db_query($global_newsletter_sql);
	while ($row = $query->fetchAssoc()) {
		fputcsv($fh, $row);
	}

	// close file handle
	fclose($fh);
}

/*
 * Chapter Leaders CSV function 
 */
function tm_users_download_chapter_leaders_csv(){

	global $conf;
	global $user;

	$oldzone = date_default_timezone_get();
	date_default_timezone_set(drupal_get_user_timezone());
	$csvname = strtolower(str_replace(" ", "_", $conf['tm_site_name'])) . '_chapter_leaders_'.date('M_d_Y_Hi',time()) . ".csv";
	$csvname = str_replace(array(","," "), "_", $csvname); // replace spaces and ,
	date_default_timezone_set($oldzone);

	// csv header
	drupal_add_http_header('Content-Type', 'text/csv; utf-8');
	drupal_add_http_header('Content-Disposition', 'attachment; filename = '.$csvname);

	$fh = fopen('php://output', 'w');

	// put privacy information in the header
	$message = "# IMPORTANT: This information is confidential and may not be transmitted to any 3rd party.";
	$message .= "\n# You may only use this data in compliance with the " . $conf['tm_site_name'] . " Privacy Terms.";
	$message .= "\n# Note: Chapter leaders of more than one chapter will be listed in multiple rows.\n\n";
	fputs($fh,$message,strlen($message));

	// puts a header row across the top of the csv
	$item = array(t('First name'),t('Last name'),t('Country Code'),t('Location'),t('Chapter'),t('Chapter Country'),t('Email'));

	fputcsv($fh, $item);

	// fetch chapter leaders details
	$chapter_leaders_list_sql = "select distinct field_user_first_name_value, field_user_last_name_value, field_user_country_iso2 as country, field_location_city_value as location_field, chapter.title as chapter, field_data_field_country.field_country_iso2 as chapter_country, mail 
	from 
	(select field_user_first_name_value, field_user_last_name_value, mail, uid from users u, field_data_field_user_first_name fn, field_data_field_user_last_name ln where u.uid = fn.entity_id and u.uid = ln.entity_id) as ud 
	left join field_data_field_chapter_leaders cl on cl.field_chapter_leaders_target_id = ud.uid 
	left join field_data_field_user_country uc on uc.entity_id = ud.uid and uc.entity_type = 'user'
	left join field_data_field_location_city uloc on uloc.entity_id = ud.uid and uloc.entity_type = 'user'
	left join node chapter on chapter.nid = cl.entity_id
	left join field_data_field_country on field_data_field_country.entity_id = chapter.nid and field_data_field_country.bundle = 'chapter'
	where field_chapter_leaders_target_id is not null";

	// perform query
	$query = db_query($chapter_leaders_list_sql);
	while ($row = $query->fetchAssoc()) {
		fputcsv($fh, $row);
	}

	// close file handle
	fclose($fh);
}

