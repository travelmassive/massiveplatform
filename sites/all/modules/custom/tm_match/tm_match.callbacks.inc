<?php

// tm_match_callbacks.inc

// swipe left
function tm_match_callback_swipe_left($uid, $left_uid) {

	// check user
	if (!user_is_logged_in()) {
		drupal_exit();
	}

	// check user id
	global $user;
	if ($uid != $user->uid) {
		drupal_exit();
	}

	// check offline
	global $conf;
	if ($conf["tm_match_offline"]) {
		drupal_exit();
	}

	// dont match yourself
	if ($uid == $left_uid) {
		drupal_exit();
	}

	// check approved
	if (!in_array("approved user", $user->roles)) {
		drupal_exit();
	}

	// check rate limit for left swipes
	if (!tm_match_check_rate_limit($uid)) {
		drupal_exit();
	}

	tm_match_swipe_left($uid, $left_uid);

}

// swipe right
function tm_match_callback_swipe_right($uid, $right_uid) {
	
	// check user
	if (!user_is_logged_in()) {
		drupal_exit();
	}

	// check user id
	global $user;
	if ($uid != $user->uid) {
		drupal_exit();
	}

	// check offline
	global $conf;
	if ($conf["tm_match_offline"]) {
		drupal_exit();
	}

	// dont match yourself
	if ($uid == $right_uid) {
		drupal_exit();
	}

	// check approved
	if (!in_array("approved user", $user->roles)) {
		drupal_exit();
	}
	
	tm_match_swipe_right($uid, $right_uid);

}

