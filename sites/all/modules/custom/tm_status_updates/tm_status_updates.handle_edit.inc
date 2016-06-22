<?php

// tm_status_updates.handle_edit.inc -  handlers for editing status updates

/**
 * Check if user can edit an update
 */
function tm_status_updates_edit_status_check($account) {

	global $user;
	global $conf;

	// need to be logged in
	if (!user_is_logged_in()) {
	return false;
	}

	// check if status_updates are enabled
	if (!$conf["tm_status_updates_enabled"]) {
	return false;
	}

	// check its your uid
	if ($user->uid != $account->uid) {
	return false;
	}

	// get status update
	if (!isset($_POST["status_update_id"])) { 
		return false;
	}
	$status_update_id = $_POST["status_update_id"];
	$status_update = tm_status_updates_get_update_by_id($status_update_id);
	if ($status_update == null) {
		return false;
	}

	// get entity type and id
	$entity_type = $status_update->entity_type;
	$entity_id = $status_update->entity_id;

	// check if user can edit based on their company profiles, and chapters
	if (tm_status_updates_check_permission($entity_type, $entity_id, "edit", $user->uid)) {
		return true;
	}

	// not allowed to edit
	return false;
}

/** 
 * Edit status
 * tm_status_updates_edit_status_check() should be called first.
 * returns array. ie: array("result" => false, "error_message" => "something"); 
 */
function tm_status_updates_edit_status_ajax() {

	global $user;
	global $conf;

	// get posting uid from url
	// ie: /user/10527/post_update
	$uid = arg(1);

	// Load user posting update
	$posting_user = user_load($uid);
	if ($posting_user == false) {
		$ret = array("result" => false, "error_message" => "Unable to retrieve user account");
		drupal_json_output($ret);
		return;
	}

	// Get status_update
	$status_update_id = $_POST["status_update_id"];
	$status_update = tm_status_updates_get_update_by_id($status_update_id);
	if ($status_update == null) {
		return false;
	}

	// Update status update fields
	if (isset($_POST["status_update_text"])) {
		$status_update->status_update = trim($_POST["status_update_text"]);
	}

	if (isset($_POST["preview_link_id"])) {
		$status_update->preview_link_id = $_POST["preview_link_id"];
	}

	if (isset($_POST["location_text"])) {
		$status_update->status_location_text = trim($_POST["location_text"]);
	}

	if (isset($_POST["location_latitude"])) {
		$status_update->status_location_latitude = $_POST["location_latitude"];
	}

	if (isset($_POST["location_longitude"])) {
		$status_update->status_location_longitude = $_POST["location_longitude"];
	}

	// Softfail if update too short
	if (strlen($status_update->status_update) < 3) {
		$ret = array("result" => false, "error_message" => "Update is too short.", "redirect" => null);
		drupal_json_output($ret);
		return;
	}
  
	// Softfail if update too long
	if (strlen($status_update->status_update) > 250) {
		$ret = array("result" => false, "error_message" => "Update is too long (maximum 250 characters).", "redirect" => null);
		drupal_json_output($ret);
		return;
	}

	// Get uid and IP of editing user
	$status_update->poster_uid = $user->uid;
	$status_update->poster_ip = tm_geoip_get_real_visitor_ip();

	// Edit status update
	$ret = tm_status_updates_edit_update($status_update);
	if (!$ret) {
		$ret = array("result" => false, "error_message" => "Error saving changes", "redirect" => null);
		drupal_json_output($ret);
		return;
	}

	// Updated successfully
	if ($status_update->entity_type == "user") {
		$redirect = "/newsfeed"; // redirect to user's news feed
	} else {
		$redirect = "/node/" . $entity_id; // redirect to entity page
	}
	$ret = array("result" => true, "error_message" => null, "redirect" => $redirect);
	drupal_json_output($ret);
	return;
}

/**
 * Display edit form by ajax
 * tm_status_updates_edit_status_check() should be called first.
 */
function tm_status_updates_edit_form_ajax() {

	global $conf;

	// get status update
	$status_update_id = $_POST["status_update_id"];
	$status_update = tm_status_updates_get_update_by_id($status_update_id);
	if ($status_update == null) {
		print "Unable to retrieve status update";
		return false;
	}

	// generate edit form
	$edit_form = "<textarea id='edit_status_update_text' value='' placeholder='' rows='3' cols='50' placeholder=''>" . htmlentities($status_update->status_update) . "</textarea>";

	echo $edit_form;
	return;
}

