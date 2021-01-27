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
		$uncompressed = gzuncompress($cache->data);
		echo $uncompressed;
	} else {
		echo "Please run drush tm-generate-newsletter-csv to generate csv";
	}
}

/*
 * Global Newsletter Subscribers function 
 */
function tm_users_download_global_newsletter_generate($drush_progress = false) {

	global $conf;

	// outside of loop for better performance
    $module_exists_tm_subscriptions_user = module_exists("tm_subscriptions_user");
    $module_exists_tm_subscriptions = module_exists("tm_subscriptions");

    // preload flags
    $approval_approved_by_flag = flag_get_flag('approval_approved_by');

	if ($module_exists_tm_subscriptions_user) {
		$user_subscription_enable_flag = flag_get_flag('user_subscription_enable', NULL);
		$user_subscription_enable_flag_id = $user_subscription_enable_flag->fid;
	}

	$organization_subscription_enable_flag_id = null;
	if ($module_exists_tm_subscriptions) {
		$organization_subscription_enable_flag = flag_get_flag('organization_subscription_enable', NULL);
		$organization_subscription_enable_flag_id = $organization_subscription_enable_flag->fid;
	}

	// preload all taxonomies
	$vocabulary = taxonomy_vocabulary_machine_name_load("tm_segments");
	$taxonomy_terms = entity_load('taxonomy_term', FALSE, array('vid' => $vocabulary->vid));
	$taxonomy_parents = array();

	// preload parent terms
	foreach ($taxonomy_terms as $taxonomy_obj) {
		$parent_terms = taxonomy_get_parents($taxonomy_obj->tid);
		if (sizeof($parent_terms) > 0) {
			$taxonomy_parents[$taxonomy_obj->tid] = $parent_terms;
		} else {
			$taxonomy_parents[$taxonomy_obj->tid] = array();
		}

	}	

	$fh = fopen('php://memory', 'w');

	// lookup approved member role id
	$approved_member_role = user_role_load_by_name("approved user")->rid;

	// lookup chapter leader role id
	$chapter_leader_role = user_role_load_by_name("chapter leader")->rid;

	// is in chapter
	$member_of_chapters = array();
	if (isset($conf["tm_users_download_csv_member_of_chapters"])) {
		$member_of_chapters = $conf["tm_users_download_csv_member_of_chapters"];
	}

	// puts a header row across the top of the csv
	$item = array(t('First_Name'),t('Last_Name'), t('Email'), t('Country_Code'), t('Location'), t('Home_Chapter'), t('Home_Chapter_Country'), t('User_ID'), t('Account_Age_Days'), t('Last_Active_Days'), t('Last_Updated_Days'), t('Days_Since_Verified'), t('Event_Registrations'), t('Region_Code'), t('Joined_Chapters'), t('Member_Status'), t('Member_Subscription_Type'), t('Organization_Subscription_Type'), t('Show_Member_Subscription_CTA'), t('Show_Organization_Subscription_CTA'), t('Profile_URL'), t('First_Organization_Profile'), t('Organization_Profile_URL'), t('User_Segment_L1'), t('User_Segment_L2'), t('Organization_Segment_L1'), t('Organization_Segment_L2'), t('AB_Group'), t('IP'));

	// Is in chapters
	foreach ($member_of_chapters as $chapter_shortcode) {
		$item[] = "Member_" . $chapter_shortcode;
	}

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

	// get array of chapter and regions
	$chapter_regions = _tm_chapters_get_all_country_regions();

	// get signup flag
	$signup_flag = flag_get_flag("signup", NULL);

	// get event register flag
	$event_register_flag = flag_get_flag("event_register");

	// perform query
	$query = db_query($global_newsletter_sql);
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

		$user_id = $row["uid"];

		// fetch organizations 
		// subscribed company will put first subscribed organization
		$organization_profile_nids = tm_users_get_organization_profiles($user_id, $organization_subscription_enable_flag_id);

		// days since last active
		$last_visit_timestamp = tm_geoip_get_users_last_visit_time($user_id);
		if ($last_visit_timestamp != null) {
			$days_since_last_visit = intval((time() - $last_visit_timestamp) / (60 * 60 * 24));
			$row[] = $days_since_last_visit;
		} else {
			$row[] = -1; // never logged in
		}

		// days since last modified
		$last_modified_timestamp = tm_users_get_last_modified_time($user_id);
		if ($last_modified_timestamp != null) {
			$days_since_last_modified = intval((time() - $last_modified_timestamp) / (60 * 60 * 24));
			$row[] = $days_since_last_modified;
		} else {
			$row[] = -1; // never modified
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

		// number of events registered for
		$num_events = tm_users_get_num_registered_events($user_id, $event_register_flag);
		$row[] = $num_events;

		// home chapter region, or country region
		if ($row["home_chapter_country"] != "") {
			$row[] = $chapter_regions[$row["home_chapter_country"]];
		} else if ($row["country"] != "") {
			$row[] = $chapter_regions[$row["country"]];
		} else {
			$row[] = "";
		}

		// get joined chapters
		// ie: SYD,NYC,BNE
		$joined_chapters = tm_users_get_users_chapters_shortcodes($chapter_shortcodes, $signup_flag, $user_id);
		$row[] = implode(" ", $joined_chapters);

		// check if chapter leader
		$is_chapter_leader = tm_users_userid_has_roleid($user_id, $chapter_leader_role);

		// include approval status
		$approved_status = tm_users_userid_has_roleid($user_id, $approved_member_role);

		// member type
		// chapter leader, approved, or not approved
		if ($is_chapter_leader) {
			$row[] = "chapter leader";
		} else {
			if ($approved_status == true) {
				$row[] = "approved";
			} else {
				$row[] = "not approved";
			}
		}

		// member subscription type
		// use flag lookup directly for performance
		$user_subscription_type = "none";
		if ($module_exists_tm_subscriptions_user) {
			if (tm_flags_is_entity_flagged('user', $user_id, 'user_subscription_enable', $user_subscription_enable_flag_id)) {
				$user_subscription_type = tm_subscriptions_user_get_type($user_id);
			}
			if ($user_subscription_type == "") {
				$user_subscription_type = "flagged";
			}
		}
		$row[] = $user_subscription_type;

		// company subscription type
		$company_subscription_type = "none";
		if ($module_exists_tm_subscriptions) {
			if (sizeof($organization_profile_nids) > 0) {

				// check if first organization is subscribed
				if (tm_flags_is_entity_flagged('node', $organization_profile_nids[0], 'organization_subscription_enable', $organization_subscription_enable_flag_id)) {

					// get subscription type
					$company_subscription_type = tm_subscriptions_get_type($organization_profile_nids[0]);
				}
			}
			if ($company_subscription_type == "") {
				$company_subscription_type = "flagged";
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
		// don't show CTA if chapter leader (in newsletter)
		$show_organization_membership_cta = 0; // default false
		if (sizeof($organization_profile_nids) > 0) {
			if ($module_exists_tm_subscriptions) {
				if (!$is_chapter_leader and tm_subscriptions_check_show_organization_cta($organization_profile_nids[0], $user_id, true)) {
					$show_organization_membership_cta = 1; // user will be displayed a CTA
				}
			}
		}
		$row[] = $show_organization_membership_cta;

		// include profile url
		$account_profile_url = drupal_get_path_alias("user/" . $user_id);
		$account_profile_url_absolute = url($account_profile_url,array('absolute'=>true));
		$row[] = $account_profile_url_absolute;

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

		// user segment
		$user_segment_l1 = "";
		$user_segment_l2 = "";
		$field_segment_tid = tm_users_get_taxonomy_tid($user_id);
		if ($field_segment_tid != null){
			$term = $taxonomy_terms[$field_segment_tid]; // taxonomy_term_load($field_segment_tid);
			// load parent term
			$parent_terms = $taxonomy_parents[$term->tid]; // taxonomy_get_parents($term->tid);
			if (is_array($parent_terms) && (sizeof($parent_terms) > 0)) {
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
				$term = $taxonomy_terms[$field_segment_tid]; // taxonomy_term_load($field_segment_tid);
				// load parent term
				$parent_terms = $taxonomy_parents[$term->tid]; // taxonomy_get_parents($term->tid);
				if (is_array($parent_terms) && (sizeof($parent_terms) > 0)) {
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

		// AB Group (1 to 100)
		$row[] = ($row_count % 100) + 1;

		// IP
		$ip = "";
		if (module_exists("tm_geoip")) {
			$ip = tm_geoip_get_users_last_ip($user_id);
		}
		$row[] = $ip;

		// Is member of chapter
		foreach ($member_of_chapters as $chapter_shortcode) {
			if (array_search($chapter_shortcode, $joined_chapters) !== FALSE) {
				$row["Member_" . $chapter_shortcode] = "1";
			} else {
				$row["Member_" . $chapter_shortcode] = "0";
			}
		}

		$row_count++;

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
	$item = array(t('User_ID'), t('First_Name'),t('Last_Name'),t('Chapter_Leader_Country_Code'),t('Chapter_Leader_Location'),t('Chapter_Leader_Chapter'),t('Chapter_Leader_Chapter_Country'),t('Email'),t('Chapter_Leader_Region_Code'),t('Last_Active_Days'),t('Num_Events'),t('Num_Followers'),t('Num_Following'),t('Num_Verifications'),t('Num_Flags'));

	fputcsv($fh, $item);

	// fetch chapter leaders details
	$chapter_leaders_list_sql = "select distinct ud.uid, field_user_first_name_value, field_user_last_name_value, field_user_country_iso2 as country, field_location_city_value as location_field, chapter.title as chapter, field_data_field_country.field_country_iso2 as chapter_country, mail 
	from 
	(select field_user_first_name_value, field_user_last_name_value, mail, uid from users u, field_data_field_user_first_name fn, field_data_field_user_last_name ln where u.uid = fn.entity_id and u.uid = ln.entity_id) as ud 
	left join field_data_field_chapter_leaders cl on cl.field_chapter_leaders_target_id = ud.uid 
	left join field_data_field_user_country uc on uc.entity_id = ud.uid and uc.entity_type = 'user'
	left join field_data_field_location_city uloc on uloc.entity_id = ud.uid and uloc.entity_type = 'user'
	left join node chapter on chapter.nid = cl.entity_id
	left join field_data_field_country on field_data_field_country.entity_id = chapter.nid and field_data_field_country.bundle = 'chapter'
	where field_chapter_leaders_target_id is not null";

	// get array of chapter and regions
	$chapter_regions = _tm_chapters_get_all_country_regions();

	// flags
	$flag_follow_members = flag_get_flag("follow_members");
	$flag_event_register = flag_get_flag("event_register");
	$flag_approval_approved_by = flag_get_flag("approval_approved_by");
	$flag_moderation_ids = array();
	$flag_moderation_ids[] = flag_get_flag("approval_is_company_or_brand")->fid;
	$flag_moderation_ids[] = flag_get_flag("approval_need_more_info")->fid;
	$flag_moderation_ids[] = flag_get_flag("moderate_non_community_profile")->fid;

	// perform query
	$query = db_query($chapter_leaders_list_sql);
	while ($row = $query->fetchAssoc()) {

		$user_id = $row["uid"];

		// add region to row
		if ($row["chapter_country"] != null) {
			$row[] = $chapter_regions[$row["chapter_country"]];
		} else {
			$row[] = "";
		}

		// days since last active
		$last_visit_timestamp = tm_geoip_get_users_last_visit_time($user_id);
		if ($last_visit_timestamp != null) {
			$days_since_last_visit = intval((time() - $last_visit_timestamp) / (60 * 60 * 24));
			$row[] = $days_since_last_visit;
		} else {
			$row[] = -1; // never logged in
		}

		// num events
		$num_events = tm_users_get_num_registered_events($user_id, $flag_event_register);
		$row[] = $num_events;

		// num followers
		$num_followers = tm_users_get_num_followers($user_id, $flag_follow_members);
		$row[] = $num_followers;

		// num following
		$num_following = tm_users_get_num_following($user_id, $flag_follow_members);
		$row[] = $num_following;

		// num verified
		$num_verified = tm_users_get_num_flag_verified($user_id, $flag_approval_approved_by);
		$row[] = $num_verified;

		// num moderator actions
		$num_flag_actions = tm_users_get_num_flag_moderations($user_id, $flag_moderation_ids);
		$row[] = $num_flag_actions;

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
