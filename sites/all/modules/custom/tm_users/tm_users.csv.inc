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

	// lookup approved member role id
	$approved_member_role = user_role_load_by_name("approved user")->rid;

	// puts a header row across the top of the csv
	$item = array(t('First name'),t('Last name'),t('Country Code'),t('Location'),t('Home Chapter'),t('Home Chapter Country'),t('Email'),t('User ID'), t('Account Age (Days)'), t('Last Active (Days)'), t('Approved Status'), t('Profile URL'), t('First Organization Profile'), t('Organization Profile URL'), );
	fputcsv($fh, $item);

	// fetch newsletter subscribers
	$global_newsletter_sql = "select field_user_first_name_value, field_user_last_name_value, field_user_country_iso2 as country, field_location_city_value as location_field, chapter.title as home_chapter, field_data_field_country.field_country_iso2 as home_chapter_country, mail, ud.uid, FLOOR((UNIX_TIMESTAMP() - ud.created) / (60 * 60 * 24)) as created_days from (select field_user_first_name_value, field_user_last_name_value, mail, uid, created from users u, field_data_field_user_first_name fn, field_data_field_user_last_name ln where u.uid = fn.entity_id and u.uid = ln.entity_id) as ud 
left join tm_notifications n on ud.uid = n.uid and n.bundle = 'global_newsletter'
left join field_data_field_user_country uc on uc.entity_id = ud.uid and uc.entity_type = 'user'
left join field_data_field_location_city uloc on uloc.entity_id = ud.uid and uloc.entity_type = 'user'
left join field_data_field_home_chapter uchapter on uchapter.entity_id = ud.uid and uchapter.entity_type = 'user' 
left join node chapter on chapter.nid = uchapter.field_home_chapter_target_id
left join field_data_field_country on field_data_field_country.entity_id = chapter.nid and field_data_field_country.bundle = 'chapter'
where n.value = 1 order by ud.uid asc";
	
	// perform query
	$query = db_query($global_newsletter_sql);
	while ($row = $query->fetchAssoc()) {

		$user_id = $row["uid"];

		// days since last active
		$last_visit_timestamp = tm_geoip_get_users_last_visit_time($user_id);
		if ($last_visit_timestamp != null) {
			$days_since_last_visit = intval((time() - $last_visit_timestamp) / (60 * 60 * 24));
			$row[] = $days_since_last_visit;
		} else {
			$row[] = -1; // never logged in
		}

		// include approval status
		$approved_status = tm_users_userid_has_roleid($user_id, $approved_member_role);
		if ($approved_status == true) {
			$row[] = "approved";
		} else {
			$row[] = "not approved";
		}

		// include profile url
		$account_profile_url = drupal_get_path_alias("user/" . $user_id);
		$account_profile_url_absolute = url($account_profile_url,array('absolute'=>true));
		$row[] = $account_profile_url_absolute;


		// include whether account has a organization profile
		$organization_profile_nids = tm_users_get_organization_profiles($user_id);
		if ($organization_profile_nids == null) {
			$row[] = "";
		} else {
			$company_title = tm_organizations_get_title($organization_profile_nids[0]);
			if ($company_title != "") {
				$row[] = strip_tags(trim($company_title));
			} else {
				$row[] = "";
			}
		}

		// include organizations profile url
		if ($organization_profile_nids == null) {
			$row[] = "";
		} else {
			$organization_profile_url = drupal_get_path_alias("node/" . $organization_profile_nids[0]);
			$organization_profile_url_absolute = url($organization_profile_url,array('absolute'=>true));
			$row[] = $organization_profile_url_absolute;
		}

		fputcsv($fh, $row);
	}

	// close file handle
	fclose($fh);
}

/**
 * Helper method quickly look up if user is approved or not
 */
function tm_users_userid_has_roleid($uid, $rid) {

	$query = "SELECT uid FROM users_roles WHERE uid = :uid and rid = :rid LIMIT 1";
	$row = db_query($query, array(':uid' => $uid, ':rid' => $rid))->fetch();
	return ($row != null);
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

