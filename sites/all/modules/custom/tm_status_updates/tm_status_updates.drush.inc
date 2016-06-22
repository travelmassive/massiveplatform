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


