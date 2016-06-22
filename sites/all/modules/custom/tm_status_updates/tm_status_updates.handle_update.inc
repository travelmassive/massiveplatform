<?php

// tm_status_updates.handle_update.inc -  handlers for updating status updates

/**
* Check if user can post an update
*/
function tm_status_updates_update_status_check($account) {

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

	// need post_as
	if (!isset($_POST["post_as"])) {
		return false;
	}

	// need update_text
	if (!isset($_POST["status_update_text"])) {
		return false;
	}

	// get entity
	// ie: uid-123
	// ie: nid-100
	$post_as = $_POST["post_as"];
	$parsed = tm_status_updates_parse_post_as($post_as);
	$entity_type = $parsed["entity_type"];
	$entity_id = $parsed["entity_id"];
	if (($entity_type == null) or ($entity_id == null)) {
		return false;
	}

	// check if user can edit based on their company profiles, and chapters
	if (tm_status_updates_check_permission($entity_type, $entity_id, "create", $user->uid)) {
		return true;
	}

	// not allowed to post
	return false;
}

/** 
* Update status
* tm_status_updates_update_status_check() should be called first.
* returns array. ie: array("result" => false, "error_message" => "something"); 
*/
function tm_status_updates_update_status_ajax() {

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

	// Get post_as
	// ie: uid-123
	// ie: nid-123
	$post_as = trim($_POST["post_as"]);
	$parsed = tm_status_updates_parse_post_as($post_as);
	$entity_type = $parsed["entity_type"];
	$entity_id = $parsed["entity_id"];
	if (($entity_type == null) or ($entity_id == null)) {
		$ret = array("result" => false, "error_message" => "Entity type or id is empty");
		drupal_json_output($ret);
		return;
	}

	// Get status update
	$status_update_text = trim($_POST["status_update_text"]);

	// Get preview link id if provided
	$preview_link_id = null;
	if (isset($_POST["preview_link_id"])) {
		$preview_link_id = $_POST["preview_link_id"];
	}

	// Softfail if update too short
	if (strlen($status_update_text) < 3) {
		$ret = array("result" => false, "error_message" => "Update is too short.", "redirect" => null);
		drupal_json_output($ret);
		return;
	}

	// Softfail if update too long
	if (strlen($status_update_text) > 250) {
		$ret = array("result" => false, "error_message" => "Update is too long.", "redirect" => null);
		drupal_json_output($ret);
		return;
	}

	// Get location
	$location_text = null;
	if (isset($_POST["location_text"])) {
		$location_text = trim(strip_tags($_POST["location_text"]));
	}

	$location_latitude = null;
	if (isset($_POST["location_latitude"])) {
		$location_latitude = floatval($_POST["location_latitude"]);
	}

	$location_longitude = null;
	if (isset($_POST["location_longitude"])) {
		$location_longitude = floatval($_POST["location_longitude"]);
	}

	// Get IP of posting user
	$posting_user_ip = tm_geoip_get_real_visitor_ip();

	// If location is not set, fallback to geoip lookup
	if (($location_latitude == null) or ($location_longitude = null)) {
		$ip_location = tm_geoip_lookup($posting_user_ip);
		if (isset($ip_location["location"]["latitude"])) {
			$location_latitude = $ip_location["location"]["latitude"];
		}
		if (isset($ip_location["location"]["longitude"])) {
			$location_longitude = $ip_location["location"]["longitude"];
		} 
	}

	// Create status update
	$ret = tm_status_updates_create_update($entity_id, $entity_type, $status_update_text, $preview_link_id, $posting_user, $posting_user_ip, $location_text, $location_latitude, $location_longitude);
	if (!$ret) {
		$ret = array("result" => false, "error_message" => "Error saving status update", "redirect" => null);
		drupal_json_output($ret);
		return;
	}

	// Updated successfully
	if ($entity_type == "user") {
		$redirect = "/newsfeed"; // redirect to user's news feed
	} else {
		$redirect = "/node/" . $entity_id; // redirect to entity page
	}
	$ret = array("result" => true, "error_message" => null, "redirect" => $redirect);
	drupal_json_output($ret);
	return;

}

/**
* Helper method to parse post_as field
* ie: uid-123, or nid-100
* Returns array with entity_type and entity_id
*/
function tm_status_updates_parse_post_as($post_as) {

	$parts = explode("-", $post_as);
	if (sizeof($parts) < 0) { 
		return array("entity_type" => null, $entity_id => null);
	}

	// post as uid
	if ($parts[0] == "uid") {
		$entity_type = "user";
		$entity_id = intval($parts[1]);
	}

	// post as nid
	if ($parts[0] == "nid") {
		$entity_id = intval($parts[1]);
		$nid = intval($entity_id);
		$node = node_load($nid);
		if ($node == false) {
			return array("entity_type" => null, $entity_id => null);
		}
		$entity_type = null;
		if ($node->type == "chapter") {
			$entity_type = "chapter";
		}
		if ($node->type == "organization") {
			$entity_type = "organization";
		}
		if ($node->type == "event") {
				$entity_type = "event";
		}

		// this entity type can't post
		if ($entity_type == null) {
			return array("entity_type" => null, $entity_id => null);
		}
	}

	return array("entity_type" => $entity_type, "entity_id" => $entity_id);
}
