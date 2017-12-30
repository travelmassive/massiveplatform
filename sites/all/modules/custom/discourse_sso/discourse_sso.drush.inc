<?php

// discourse_sso.drush.inc - drush commands for discourse sso

include_once "includes/discourse_sso.endpoint.inc";

/**
 * Implements hook_drush_command().
 */
function discourse_sso_drush_command() {

	$items['tm-discourse-sso-sync-account'] = array(
		'description' => 'Sync account info with discourse sso.',
		'callback' => 'discourse_sso_sync_account_drush',
		'options' => array(
			'uid' => 'uid to sync',
			'debug_api' => 'debug output',
		),
	);

	$items['tm-discourse-sso-log-out'] = array(
		'description' => 'Log out account with discourse sso.',
		'callback' => 'discourse_sso_log_out_drush',
		'options' => array(
			'uid' => 'uid to log out',
			'debug_api' => 'debug output',
		),
	);

	return $items;
}

/**
 * Log out account with Discourse API
 */
function discourse_sso_log_out_drush() {

	global $conf;

	// get uid
	$uid = drush_get_option('uid');
	if ($uid == null) {
		drush_print("Usage: drush tm-discourse-sso-sync-account --uid=123 (optional: --debug_api=true)");
		return;
	}

	// check debug
	$debug = false;
	$debug_option = drush_get_option('debug_api');
	if ($debug_option == "true") {
		$debug = true;
	}

	// load user account
	$account = user_load($uid);
	if ($account == null) {
		drush_print("Invalid uid " . $uid);
		return;
	}

	// get discourse uid
	$result = discourse_sso_get_discourse_uid($uid);

	// if discourse uid
	$res = json_decode($result);
	if (isset($res->users[0]->id)) {
		$discourse_uid = $res->users[0]->id;
		drush_print("Logging out discourse uid " . $discourse_uid);
		discourse_sso_logout_user($res->users[0]->id);
	} else {
		drush_print("Something went wrong...");
	}
}


/**
 * Sync account info with Discourse SSO
 * Use the Discourse SSO provider to update the user's account information
 * ie: avatar, etc
 */
function discourse_sso_sync_account_drush() {

	global $conf;

	// get uid
	$uid = drush_get_option('uid');
	if ($uid == null) {
		drush_print("Usage: drush tm-discourse-sso-sync-account --uid=123 (optional: --debug_api=true)");
		return;
	}

	// check debug
	$debug = false;
	$debug_option = drush_get_option('debug_api');
	if ($debug_option == "true") {
		$debug = true;
	}

	// load user account
	$account = user_load($uid);
	if ($account == null) {
		drush_print("Invalid uid " . $uid);
		return;
	}

	// get api url
	$url = $conf["tm_discourse_sso_sync_url"] . 'admin/users/sync_sso';

	// get sso payload
	// example:
	// $params = array();
	// $params['name'] = 'Updated User Name';
	// $params['username'] = 'new user name';
	// $params['email'] = 'newusername@example.massiveplatform.com';
	// $params['external_id'] = 123; // sync with uid
	// $params['avatar_url'] = 'https://example.massiveplatform.com/images/newavatar.jpg';
	// $params['avatar_force_update'] = 'true';
	$params = discourse_sso_get_user_params($account);

	// fetch from api
	$result = discourse_sso_api($params, $url);

	// verify result
	if ($result === false) {
		drush_print("There was a problem (curl_exec() returned false)");
		return;
	}

	// check output
	$res = json_decode($result);
	if (isset($res->single_sign_on_record->external_id)) {
		drush_print("Record updated for id " . $res->id . " (external_id " . $params['external_id'] . ")");
	} else {
		drush_print("Something went wrong (invalid secret?) ...");
	}

	// debug
	if ($debug) {
		drush_print("DEBUG:");
		drush_print($result);
	}
}
