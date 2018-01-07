<?php

// tm_users.drush.inc - drush commands for user management

/**
 * Implements hook_drush_command().
 */
function tm_users_drush_command() {

	$items['tm-assign-missing-approval-flags'] = array(
		'description' => 'Assign any missing approval flags to another user.',
		'callback' => 'tm_users_assign_missing_approval_flags_drush',
		'options' => array(
			'confirm' => 'Confirm that you want to proceed',
			'uid' => 'uid to reassign to',
			'target_uid' => 'target a user id'
		),
	);

	$items['tm-generate-newsletter-csv'] = array(
		'description' => 'Generate the global newsletter csv.',
		'callback' => 'tm_users_download_global_newsletter_csv_drush'
	);

	return $items;
}

/**
 * Resassign missing approval flags
 * This happens if a member who approved other accounts is removed
 * If you are using follower sort in tm_search_api then you should run this if an approving user is removed to ensure members have the approval_flag to be sorted on
 */
function tm_users_assign_missing_approval_flags_drush() {

	global $conf;

	// check confirm
	$confirm = false;
	$confirm_opt = drush_get_option('confirm');
	if ($confirm_opt != null) {
		$confirm = ($confirm_opt == "true");
	}
	if ($confirm_opt == null) {
		drush_print("Usage: drush tm-assign-missing-approval-flags --confirm=true\nOptional: --uid=123");
		return;
	}

	if (!$confirm) {
		drush_print("Running simulation only. Use --confirm=true to process.");
	}

	// get uid - default 1 (optional)
	$assign_uid = drush_get_option('uid');
	if ($assign_uid == null) {
		$assign_uid = 1;
	}
	drush_print("Resassign uid: " . $assign_uid);

	// target uid (optional)
	$target_uid = drush_get_option('target_uid');
	if ($target_uid != null) {
		drush_print("Target uid: " . $target_uid);
	}

	// get min and max uid
	$all_uids = db_query("SELECT name,uid FROM users ORDER BY uid ASC")->fetchAllKeyed(1,0);
	$approving_user = user_load($assign_uid);
	if ($approving_user == null) {
		drush_print("Invalid approving user id: " . $assign_uid);
	}

	// get approval flag
	$approval_flag = flag_get_flag('approval_approved_by', NULL);

	// loop through all users
	foreach ($all_uids as $uid => $name) {

		// target uid
		if ($target_uid != null) {
			if ($target_uid != $uid) {
				continue;
			}
		}

		// load user
		$account = user_load($uid);
			if ($account !== FALSE) {

				// check if user is approved
				if (in_array("approved user", $account->roles)) {

					// check if user is flagged
					$who_flagged = flag_get_entity_flags("user", $uid, "approval_approved_by");
					if (sizeof($who_flagged) == 0) {
						drush_print("Approved user " . $uid . " is not flagged. Flagging with approval_approved_by to uid: " . $assign_uid);

					if ($confirm) {

						// set approval flag
						$approval_flag->flag('flag', $uid, $approving_user, true);

						// set it back six months	
						db_query("update flagging set timestamp = (timestamp - (60*60*24*180)) where fid = " . $approval_flag->fid . " and uid = " . $assign_uid . " and entity_id = " . $uid . ";");
					}
				}
			}
		}
	}

	drush_print("Completed.");

}

/**
 * Generate global Newsletter Subscribers csv 
 */
function tm_users_download_global_newsletter_csv_drush(){

	drush_print("Generating newsletter CSV.");
	drush_print("Fetching query data...");

	// generate csv
	$starttime = microtime(true);
	$csv = tm_users_download_global_newsletter_generate(true);

	// save to cache 25 hours
	cache_set('tm-users-global-newsletter-csv', $csv, 'cache', time() + (60 * 60 * 25));
	$endtime = microtime(true);
	$timediff = $endtime - $starttime;
	drush_print("Done (" . round($timediff, 2) . " seconds). Saved to cache key tm-users-global-newsletter-csv");
	drush_print("You can fetch the CSV from admin/export_global_newsletter");

}

