<?php

// tm_status_updates.frontpage.inc - render front page newsfeed / status updates

/**
 * Render front page flag feeds
 */
function tm_status_updates_show_frontpage_updates($display_num_items = 5, $display_max_items = 5, $cache_key = "page-status-updates", $cache_time = 120, $show_unapproved = false, $show_repeat_user = false) {

	// cache for logged out users
	if (!user_is_logged_in()) {
		$cache_value = cache_get($cache_key, 'cache');
		if (!empty($cache_value)) {
			print($cache_value->data);
			return;
		}
	}

	$limit_from = 0;
	$limit_to = 10;
	$display_limit = 5;

	// get sorted list of status update and flag results
	$all_results = tm_status_updates_get_combined_feed_global($limit_from, $limit_to);

	// render item html from results
	$results_html = tm_status_updates_render_results_items($all_results, $display_limit);

	// render newsfeed section
	global $tm_status_updates_feed_template_frontpage_css;
	$newsfeed_html = tm_status_updates_render_results_feed($results_html, "/newsfeed", "View more", $tm_status_updates_feed_template_frontpage_css);

	// store in cache
	// for public users
	if (!user_is_logged_in()) {
		if ($cache_time > 0) {
			cache_set($cache_key, $newsfeed_html, 'cache', time() + $cache_time);
		}
	}

	// return html
	return $newsfeed_html;
}


