<?php

// tm_api.drush.inc - drush commands for user management

/**
 * Implements hook_drush_command().
 */
function tm_api_drush_command() {

	$items['tm-api-cache-stats'] = array(
		'description' => 'Generate and cache API stats.',
		'callback' => 'tm_api_stats_cache_data_drush',
	);

	return $items;
}

/**
 * Generate and cache API stats
 * Call tm_api_stats_get_data
 */
function tm_api_stats_cache_data_drush() {

	drush_print("Calling tm_api_stats_get_data().");
	tm_api_stats_get_data(false);
	drush_print("Completed.");

}
