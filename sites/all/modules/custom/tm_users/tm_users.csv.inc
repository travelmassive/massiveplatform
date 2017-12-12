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

/**
 * Global Newsletter Subscribers function 
 */
function tm_users_download_global_newsletter_csv(){

	global $conf;
	global $user;

	$oldzone = date_default_timezone_get();
	date_default_timezone_set(drupal_get_user_timezone());
	$csvname = strtolower(str_replace(" ", "_", $conf['tm_site_name'])) . '_global_newsletter_'.date('M_d_Y_Hi',time()) . ".csv";
	$csvname = str_replace(array(","," "), "_", $csvname); // replace spaces and ,
	$csvname = "confidential_" . $csvname;
	date_default_timezone_set($oldzone);

	// csv header
	drupal_add_http_header('Content-Type', 'text/csv; utf-8');
	drupal_add_http_header('Content-Disposition', 'attachment; filename = '.$csvname);

	// check cache
	$cache = cache_get('tm-users-global-newsletter-csv', 'cache');
	if (!empty($cache)) {
		echo $cache->data;
	} else {
		echo "Please run drush tm-generate-newsletter-csv to generate csv";
	}
}

/*
 * Global Newsletter Subscribers function 
 */
function tm_users_download_global_newsletter_generate($drush_progress = false) {

	// outside of loop for better performance
    $module_exists_tm_subscriptions_user = module_exists("tm_subscriptions_user");
    $module_exists_tm_subscriptions = module_exists("tm_subscriptions");

    // preload flags
    $approval_approved_by_flag = flag_get_flag('approval_approved_by');

	if ($module_exists_tm_subscriptions_user) {
		$user_subscription_enable_flag = flag_get_flag('user_subscription_enable', NULL);
	}

	$organization_subscription_enable_flag_id = null;
	if ($module_exists_tm_subscriptions) {
		$organization_subscription_enable_flag = flag_get_flag('organization_subscription_enable', NULL);
		$organization_subscription_enable_flag_id = $organization_subscription_enable_flag->fid;
	}

	$fh = fopen('php://memory', 'w');

	// lookup approved member role id
	$approved_member_role = user_role_load_by_name("approved user")->rid;

	// puts a header row across the top of the csv
	$item = array(t('First name'),t('Last name'), t('Email'), t('Country Code'), t('Location'), t('Home Chapter'), t('Home Chapter Country'), t('User ID'), t('Account Age (Days)'), t('Last Active (Days)'), t('Joined Chapters'), t('Approved Status'), t('Profile URL'), t('First Organization Profile'), t('Organization Profile URL'), t('Days Since Approved'), t('Member Subscription Type'), t('Organization Subscription Type'), t('Show Member Subscription CTA'), t('Show Organization Subscription CTA'), t('User Segment L1'), t('User Segment L2'), t('Organization Segment L1'), t('Organization Segment L2'),);
	fputcsv($fh, $item);

	// fetch newsletter subscribers
	$global_newsletter_sql = "select field_user_first_name_value, field_user_last_name_value, mail, field_user_country_iso2 as country, field_location_city_value as location_field, chapter.title as home_chapter, field_data_field_country.field_country_iso2 as home_chapter_country, ud.uid, FLOOR((UNIX_TIMESTAMP() - ud.created) / (60 * 60 * 24)) as created_days from (select field_user_first_name_value, field_user_last_name_value, mail, uid, created from users u, field_data_field_user_first_name fn, field_data_field_user_last_name ln where u.uid = fn.entity_id and u.uid = ln.entity_id) as ud 
left join tm_notifications n on ud.uid = n.uid and n.bundle = 'global_newsletter'
left join field_data_field_user_country uc on uc.entity_id = ud.uid and uc.entity_type = 'user'
left join field_data_field_location_city uloc on uloc.entity_id = ud.uid and uloc.entity_type = 'user'
left join field_data_field_home_chapter uchapter on uchapter.entity_id = ud.uid and uchapter.entity_type = 'user' 
left join node chapter on chapter.nid = uchapter.field_home_chapter_target_id
left join field_data_field_country on field_data_field_country.entity_id = chapter.nid and field_data_field_country.bundle = 'chapter'
where n.value = 1 order by ud.uid asc";
	
	// get array of chapter shortcodes to lookup
	$chapter_shortcodes = tm_chapters_get_all_chapter_shortcodes();

	// get signup flag
	$signup_flag = flag_get_flag("signup", NULL);

	// perform query
	$query = db_query($global_newsletter_sql);
	$count = 1;
	$starttime = microtime(true);
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

		$user_id = $row["uid"];

		// days since last active
		$last_visit_timestamp = tm_geoip_get_users_last_visit_time($user_id);
		if ($last_visit_timestamp != null) {
			$days_since_last_visit = intval((time() - $last_visit_timestamp) / (60 * 60 * 24));
			$row[] = $days_since_last_visit;
		} else {
			$row[] = -1; // never logged in
		}

		// get joined chapters
		// ie: SYD,NYC,BNE
		$joined_chapters = tm_users_get_users_chapters_shortcodes($chapter_shortcodes, $signup_flag, $user_id);
		$row[] = implode(" ", $joined_chapters);

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

		// fetch organizations 
		// subscribed company will put first subscribed organization
		$organization_profile_nids = tm_users_get_organization_profiles($user_id, $organization_subscription_enable_flag_id);

		// include whether account has a organization profile
		// get company name
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
		// will put first subscribed organization
		if ($organization_profile_nids == null) {
			$row[] = "";
		} else {
			$organization_profile_url = drupal_get_path_alias("node/" . $organization_profile_nids[0]);
			$organization_profile_url_absolute = url($organization_profile_url,array('absolute'=>true));
			$row[] = $organization_profile_url_absolute;
		}

		// days since approved
		// get list of people who approved
      	$who_approved = flag_get_entity_flags("user", $user_id, "approval_approved_by");
      	if (sizeof($who_approved) > 0) {

      		// sort by flag time
      		usort($who_approved, function($a, $b){
        		return ($a->timestamp > $b->timestamp) ? -1 : 1;
      		});

      		$latest_approved_flag = $who_approved[0];
      		$approved_difference = time() - $latest_approved_flag->timestamp;
      		$days_since_approved = intval(($approved_difference) / (60 * 60 * 24));
      		$row[] = $days_since_approved;
		} else {
			$row[] = -1; // never approved
		}

		// member subscription type
		// use flag lookup directly for performance
		$user_subscription_type = "";
		if ($module_exists_tm_subscriptions_user) {
			if ($user_subscription_enable_flag->is_flagged($user_id)) {
				$user_subscription_type = tm_subscriptions_user_get_type($user_id);
			}
		}
		$row[] = $user_subscription_type;

		// company subscription type
		// will put first subscribed organization
		$company_subscription_type = "";
		if ($module_exists_tm_subscriptions) {

			if (sizeof($organization_profile_nids) > 0) {
				$company_subscription_type = tm_subscriptions_get_type($organization_profile_nids[0]);
			}
		}
		$row[] = $company_subscription_type;

		// show membership subscription CTA
		$show_membership_cta = 0; // default false
		if ($module_exists_tm_subscriptions_user) {
			if (tm_subscriptions_check_show_user_cta($user_id, $user_id, true)) {
				$show_membership_cta = 1; // user will be displayed a CTA
			}
		}
		$row[] = $show_membership_cta;

		// show organization subscription CTA
		$show_organization_membership_cta = 0; // default false
		if (sizeof($organization_profile_nids) > 0) {
			if ($module_exists_tm_subscriptions) {
				if (tm_subscriptions_check_show_organization_cta($organization_profile_nids[0], $user_id, true)) {
					$show_organization_membership_cta = 1; // user will be displayed a CTA
				}
			}
		}
		$row[] = $show_organization_membership_cta;

		// user segment
		$user_segment_l1 = "";
		$user_segment_l2 = "";
		$field_segment_tid = tm_users_get_taxonomy_tid($user_id);
		if ($field_segment_tid != null){
			$term = taxonomy_term_load($field_segment_tid);
			// load parent term
			$parent_terms = taxonomy_get_parents($term->tid);
			if (sizeof($parent_terms) > 0) {
				$parent_term = current($parent_terms);
				$user_segment_l1 = $parent_term->name;
				$user_segment_l2 = $term->name;
			} else {
				$user_segment_l1 = $term->name;
				$user_segment_l2 = "";
			}
		}
		$row[] = $user_segment_l1;
		$row[] = $user_segment_l2;

		// company segment
		$company_segment_l1 = "";
		$company_segment_l2 = "";
		if (sizeof($organization_profile_nids) > 0) {
			$field_segment_tid = tm_organizations_get_taxonomy_tid($organization_profile_nids[0]);
			if ($field_segment_tid != null){
				$term = taxonomy_term_load($field_segment_tid);
				// load parent term
				$parent_terms = taxonomy_get_parents($term->tid);
				if (sizeof($parent_terms) > 0) {
					$parent_term = current($parent_terms);
					$company_segment_l1 = $parent_term->name;
					$company_segment_l2 = $term->name;
				} else {
					$company_segment_l1 = $term->name;
					$company_segment_l2 = "";
				}
			}
		}
		$row[] = $company_segment_l1;
		$row[] = $company_segment_l2;

		fputcsv($fh, $row);
	}

	rewind($fh);

	// put it all in a variable
	$csv = stream_get_contents($fh);

	// close file handle
	fclose($fh);

	// return csv
	return $csv;
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
	$csvname = "confidential_" . $csvname;
	date_default_timezone_set($oldzone);

	// csv header
	drupal_add_http_header('Content-Type', 'text/csv; utf-8');
	drupal_add_http_header('Content-Disposition', 'attachment; filename = '.$csvname);

	$fh = fopen('php://output', 'w');

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

/**
 * Helper method for use within tm_users_download_global_newsletter_csv()
 * Get a list of user's chapters shortcodes
 */
function tm_users_get_users_chapters_shortcodes($chapter_shortcodes, $signup_flag, $user_id) {
  $joined_chapters = array();
  $query = "SELECT entity_id FROM {flagging} f WHERE f.fid = :fid AND f.entity_type='node' AND f.uid=:uid";
  $joined_chapters_rows = db_query($query, array(':fid' => $signup_flag->fid, ':uid' => $user_id))->fetchAll();
  foreach ($joined_chapters_rows as $chapters_row) {
    $joined_chapters[] = $chapter_shortcodes[$chapters_row->entity_id];
  }

  return $joined_chapters;
}

