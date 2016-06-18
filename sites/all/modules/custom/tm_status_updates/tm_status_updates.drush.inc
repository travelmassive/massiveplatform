<?php

// tm_status_updates.drush.inc - drush commands

/**
 * Implements hook_drush_command().
 */
function tm_status_updates_drush_command() {

	$items['status-updates-index-view-counter'] = array(
	  'description' => 'Index daily view counter.',
	  'callback' => 'tm_status_updates_index_view_counter',
	);

	return $items;
}

/**
 * Wrapper method for tm_status_updates_update_total_view_count()
 */
function tm_status_updates_index_view_counter() {
	print "Updating tm_newsfeed_view_count_total and tm_newsfeed_view_count_daily ...\n";
	tm_status_updates_update_total_view_counts();
	print "Done.\n";
}


