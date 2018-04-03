<?php

// tm_track_views.tracking.inc

/**
 * Implements hook_entity_view().
 * Add a js for signed in users to track views of users and nodes
 * Can be used to show user history as well as 'who viewed your profile'
 */
function tm_track_views_entity_view($entity, $type, $view_mode, $langcode) {

	global $user;

	// full view
	if ($view_mode != 'full') {
		return;
	}

	// must be logged in
	if (!user_is_logged_in()) {
		return;
	}

	// only user or node types
	if (($type != 'user') and ($type != 'node')) {
		return;
	}

	// entity_id
	if ($type == 'user') {
		$entity_id = $entity->uid;
	} else {
		$entity_id = $entity->nid;
	}

	// js settings
	$js_setting = array(
		'tm_track_views' => array(
			'path' => 'api/track_views',
			'entity_id' => $entity_id,
			'entity_type' => $type,
		),
	);
	
	// insert tracking js
	drupal_add_js($js_setting, 'setting');
	drupal_add_js(drupal_get_path('module', 'tm_track_views') . '/js/tm_track_views.js');
}

/**
 * Handle ajax callback
 * api/track_views/user/123
 * api/track_views/node/456
 */
function tm_track_views_ajax($entity_id, $entity_type) {

	// must be logged in
	if (!user_is_logged_in()) {
		echo json_encode(array("result" => 0, "message" => "not logged in"));
		drupal_exit();
	}

	// only track users or node views
	if (($entity_type != 'user') and ($entity_type != 'node')) {
		echo json_encode(array("result" => 0, "message" => "invalid entity_type"));
		drupal_exit();
	}

	// enumerate entity_type
	$entity_type = tm_track_views_enumerate_entity_type($entity_type);

	global $user;
	$result = tm_track_views_db($entity_id, $entity_type, $user->uid);
	echo json_encode(array("result" => $result));
	drupal_exit();
}

/**
 * Track views in database
 * Enumerated entity_type: user = 1, node = 2
 */
function tm_track_views_db($entity_id, $entity_type, $uid, $timestamp = REQUEST_TIME) {
	
	$fields = array(
		'entity_id' => $entity_id,
		'entity_type' => $entity_type,
		'uid' => $uid,
		'timestamp' => $timestamp,
	);

	$result = drupal_write_record('tm_track_views', $fields);
	return $result;
}

