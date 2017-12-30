<?php

// discourse_sso.helpers.inc - helper methods for discourse sso

/**
 * Logout a user
 */
function discourse_sso_helper_logout_user($uid) {

	global $conf;

	// get discourse uid
	$result = discourse_sso_get_discourse_uid($uid);

	// if discourse uid
	$res = json_decode($result);
	if (isset($res->users[0]->id)) {
		$discourse_uid = $res->users[0]->id;
		discourse_sso_logout_user($res->users[0]->id);
		return true;
	}

	// something went wrong
	return false;
}

