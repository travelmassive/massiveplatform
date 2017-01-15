<?php

// tm_status_updates.drush.inc - drush commands

/**
 * Implements hook_drush_command().
 */
function tm_status_updates_drush_command() {

	$items['status-updates-index-view-counter'] = array(
	  'description' => 'Index daily view counter.',
	  'callback' => 'tm_status_updates_index_view_counter_drush',
	);

	$items['status-updates-reinstall-tables'] = array(
	  'description' => 'Reinstall database tables (note: will lose all status update data).',
	  'callback' => 'tm_status_updates_reinstall_database_tables_drush',
	  'options' => array(
	  	'confirm' => 'Confirm that you want to proceed',
	  ),
	);

	$items['status-updates-post-update'] = array(
		'description' => 'Post an update via command line',
		'callback' => 'tm_status_updates_post_update',
		'options' => array(
			'entity_id' => 'entity_id',
			'entity_type' => 'entity_type',
			'poster_uid' => 'poster_uid',
			'status_update' => 'status_update',
			'preview_url' => 'preview_url',
			'location_text' => 'location_text',
			'location_latitude' => 'location_latitude',
			'location_longitude' => 'location_longitude'
		),
	);

	$items['status-updates-post-organization-update'] = array(
		  'description' => 'Post automated updates for an organization profile.',
		  'callback' => 'tm_status_updates_post_organization_update',
		  'options' => array(
		  	'nid' => 'nid'
		  )
		);

	$items['status-updates-post-organization-updates'] = array(
	  'description' => 'Post automated updates for all organization profiles.',
	  'callback' => 'tm_status_updates_post_organization_updates',
	  'options' => array()
	);

	return $items;
}

/**
 * Wrapper method for tm_status_updates_update_total_view_count()
 */
function tm_status_updates_index_view_counter_drush() {
	drush_print("Updating tm_newsfeed_view_count_total and tm_newsfeed_view_count_daily ...");
	tm_status_updates_update_total_view_counts();
	drush_print("Done.");
}

/**
 * Wrapper method for tm_status_updates_reinstall_database_tables()
 */
function tm_status_updates_reinstall_database_tables_drush() {

	// check confirm
	$confirm = false;
	$confirm_opt = drush_get_option('confirm');
	if ($confirm_opt != null) {
		$confirm = ($confirm_opt == "true");
	}
	if ($confirm == false) {
		drush_print("Usage: status-updates-reinstall-tables --confirm=true");
		return;
	}

	drush_print("Reinstalling newsfeed tables ...");
	tm_status_updates_reinstall_database_tables();
	drush_print("Done.\n");
}

/**
 * Post an update from the command line
 */
function tm_status_updates_post_update() {

	// usage instructions
	$usage = "Usage: status-updates-post-update --entity_id=123 --entity_type=user|chapter|organization --poster_uid=456 --status_update='Massive Platform rocks! #massive' --preview_url='http://massiveplatform.com'
Optional: --location-text='Sydney' --location_latitude=-33.8 --location_longitude=151.2";

	// get entity id
	$entity_id = drush_get_option('entity_id');
	if ($entity_id == null) {
		drush_print($usage);
		return;
	}

	// get entity type
	$entity_type = drush_get_option('entity_type');
	if ($entity_type == null) {
		drush_print($usage);
		return;
	}

	// status update
	$status_update = drush_get_option('status_update');
	if ($status_update == null) {
		drush_print($usage);
		return;
	}

	// preview_url
	$preview_url = drush_get_option('preview_url');
	if ($preview_url == null) {
		drush_print($usage);
		return;
	}

	// poster_uid
	$poster_uid = drush_get_option('poster_uid');
	if ($poster_uid == null) {
		drush_print($usage);
		return;
	}

	// location_text
	$location_text = drush_get_option('location_text');

	// location_latitude
	$location_latitude = drush_get_option('location_latitude');

	// location_longitude
	$location_longitude = drush_get_option('location_longitude');
	
	// post update
	drush_print("Posting update ...");
	$ret = tm_status_updates_post_update_helper($entity_id, $entity_type, $status_update, $preview_url, $poster_uid, $location_text, $location_latitude, $location_longitude);

	if ($ret == true) {
		drush_print("Posted succesfully!");
	}

}

/**
 * Post organisational update
 */
function tm_status_updates_post_organization_update() {

	// usage instructions
	$usage = "Usage: status-updates-post-organization-update --nid=123";

	// get entity type
	$nid = drush_get_option('nid');
	if ($nid == null) {
		drush_print($usage);
		return;
	}

	// post organization update
	tm_status_updates_post_organization_update_helper($nid);

}

/**
 * Post automatic updates to organization newsfeeds
 * Run this command regularly
 */
function tm_status_updates_post_organization_updates() {

	// Get organizations that will do automatic posting
	$nids = tm_status_updates_get_organization_newsfeeds();
	drush_print("Processing " . sizeof($nids) . " organizations...");

	// Process updates in random order
	shuffle($nids);

	// Process each organization
	foreach($nids as $nid) {

		drush_print("Processing nid " . $nid . " ...");
		tm_status_updates_post_organization_update_helper($nid);
		drush_print("");
		
	}

	drush_print("Finished all posting");
	return;
}

/**
 * Helper function to post organiztion update from wordpress feed
 */
function tm_status_updates_post_organization_update_helper($nid) {

	// load organization
	$organization = node_load($nid);
	if ($organization == null) {
		drush_print("Invalid node id: " . $nid);
		return;
	}

	// get first owner
	if (!isset($organization->field_owner[LANGUAGE_NONE][0]['target_id'])) {
		drush_print("Organization does not have an owner");
		return;
	}
	$poster_uid = $organization->field_owner[LANGUAGE_NONE][0]['target_id'];

	// get newsfeed url
	$newsfeed_url = trim($organization->field_company_newsfeed_url[LANGUAGE_NONE][0]['value']);
	if ($newsfeed_url == "") {
		drush_print("Missing newsfeed URL");
		return;
	}

	// get newsfeed tags
	$newsfeed_tags = "";
	if (isset($organization->field_company_newsfeed_tags[LANGUAGE_NONE][0]['value'])) {
		$newsfeed_tags = trim($organization->field_company_newsfeed_tags[LANGUAGE_NONE][0]['value']);
	}

	// get newsfeed location
	$newsfeed_location = null;
	if (isset($organization->field_company_newsfeed_location[LANGUAGE_NONE][0]['value'])) {
		$newsfeed_location = trim($organization->field_company_newsfeed_location[LANGUAGE_NONE][0]['value']);
		if ($newsfeed_location == "") {
			$newsfeed_location = null;
		}
	}

	// get lat/lon
	// ie: Sydney|-33.8,151.2
	$newsfeed_latitude = null;
	$newsfeed_longitude = null;
	$location_parts = explode("|", $newsfeed_location);
	$newsfeed_location = $location_parts[0];
	if (isset($location_parts[1])) {
		$lat_lon = explode(",", $location_parts[1]);
		if (sizeof($lat_lon) == 2) {
			$newsfeed_latitude = $lat_lon[0];
			$newsfeed_longitude = $lat_lon[1];
		}
	}

	// fetch newsfeed
	drush_print("Checking newsfeed for " . $organization->title . " ...");
	drush_print("Newsfeed URL: " . $newsfeed_url);
	$feed_item = tm_status_updates_post_update_fetch_wordpress($newsfeed_url);

	// check newsfeed
	if ($feed_item == null) {
		drush_print("Could not fetch newsfeed.");
		return false;
	}

	// get data from parsed feed
	$status_update = $feed_item["title"];
	$preview_url = $feed_item["link"];

	// use tags
	if ($newsfeed_tags == "") {
		$generate_tags = array();
		foreach($feed_item["categories"] as $tag) {
			$generate_tags[] = "#" . str_replace(" ", "", $tag);
		}
		$newsfeed_tags = implode(" ", $generate_tags);
	}

	# add tags to status update
	$status_update .= " " . $newsfeed_tags;

	// post update
	try {
		drush_print("Posting update ...");
		$ret = tm_status_updates_post_update_helper($nid, "organization", $status_update, $preview_url, $poster_uid, $newsfeed_location, $newsfeed_latitude, $newsfeed_longitude);

		if ($ret == true) {
			drush_print("Posted new article " . $preview_url);
		}
	} catch (Exception $e) {
		drush_print("Exception caught: " . $e);
	}

}


/**
 * Helper method to post update
 */
function tm_status_updates_post_update_helper($entity_id, $entity_type, $status_update, $preview_url, $poster_uid, $location_text = null, $location_latitude = null, $location_longitude = null, $posting_user_ip = "127.0.0.1") {

	$posting_user = user_load($poster_uid);
	if ($posting_user == null) {
		drush_print("Unable to load posting user " . $poster_uid);
		return false;
	}

	// check for duplicate post
	$latest_update = tm_status_updates_get_latest_update($entity_id, $entity_type);
	if ($latest_update != null) {
		if ($latest_update->status_update == trim($status_update)) {
			drush_print("Duplicate post, not posting...");
			return false;
		}
	}

	// Fetch URL and parse opengraph data
	$preview_link_id = null;
	$preview_data = tm_status_updates_fetch_link_meta($preview_url);
	if ($preview_data != null) {

	  $image_file = tm_status_updates_fetch_meta_image($preview_data["link_og_image"]);
	  $preview_data["image_file"] = $image_file;

	  $preview_link_id = tm_status_updates_insert_preview_link($posting_user->uid, $preview_url, $preview_data["link_og_title"], $preview_data["link_og_description"], $preview_data["link_og_image"], $preview_data["image_file"]);
	}

	// Post URL
	return tm_status_updates_create_update($entity_id, $entity_type, $status_update, $preview_link_id, $posting_user, $posting_user_ip, $location_text, $location_latitude, $location_longitude);
}


/**
 * Helper method to fetch wordpress feed
 */
function tm_status_updates_post_update_fetch_wordpress($feed_url) {

	global $conf;

	// Step 1. Fetch feed url
	try {
		$curl = curl_init();
		curl_setopt_array($curl, Array(
			CURLOPT_URL            => $feed_url,
			CURLOPT_USERAGENT      => $conf["tm_site_name"] . ' Newsfeed',
			CURLOPT_TIMEOUT        => 120,
			CURLOPT_CONNECTTIMEOUT => 30,
			CURLOPT_RETURNTRANSFER => TRUE,
			CURLOPT_ENCODING       => 'UTF-8'
		));
		$data = curl_exec($curl);
		curl_close($curl);
	}
	catch (Exception $e) {
		drush_print("Error fetching feed URL");
		return;
	}

	// Step 2. Parse feed url
	libxml_use_internal_errors(true);
	$xml = simplexml_load_string($data, 'SimpleXMLElement', LIBXML_NOCDATA);
	if ($xml === false) {
		drush_print("Error parsing feed (is it a valid Wordpress feed?)");
		return null;
	}

	// get first item
	if (!isset($xml->channel->item[0])) {
		drush_print("Fetched feed but could not find first item (is it a valid Wordpress feed?)");
		return null;
	}

	$item = $xml->channel->item[0];

	$feed_item = array();
	$feed_item["title"] = $item->title . "";
	$feed_item["description"] = $item->description . "";
 	$feed_item["link"] = $item->link . "";
 	$feed_item["created"] = $item->pubDate . "";
 	$feed_item["author"] = $item->children('dc', TRUE) . "";

 	$feed_item["categories"] = array();

 	if (isset($item->category)) {
 		foreach ($item->category as $category) {
 			$feed_item["categories"][] = $category . "";
 		}
 	}		

 	return $feed_item;
	
}

