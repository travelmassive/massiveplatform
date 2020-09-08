<?php

// tm_status_updates.frontpage.inc - render front page newsfeed / status updates

/**
 * Render front page flag feeds
 */
function tm_status_updates_show_frontpage_updates($display_num_items = 5, $display_max_items = 5, $cache_key = "page-status-updates", $cache_time = 120, $show_unapproved = false, $show_repeat_user = false) {

	global $user;
	global $conf;

	// cache for logged out users
	if (!user_is_logged_in()) {
		$cache_value = cache_get($cache_key, 'cache');
		if (!empty($cache_value)) {
			print($cache_value->data);
			return;
		}
	}

	$display_limit = 4;

	// get sorted list of status update and flag results
	$all_results = tm_status_updates_poll_results("global", array(), null, 50);

	// preload users before rending to improve performance
	tm_status_updates_preload_entities($all_results["results"]);

	// get ip
	$viewer_ip = tm_geoip_get_real_visitor_ip();

	// render item html from results
	$results_html = tm_status_updates_render_results_items($all_results["results"], $viewer_ip, true, true, false, $display_limit, true);

	// render newsfeed section
	global $tm_status_updates_feed_template_frontpage_css;
	if ($conf["tm_status_updates_enabled"]) {
		$newsfeed_html = tm_status_updates_render_results_feed($results_html, "Latest Updates", "frontpage", "/newsfeed/global", "View more", $tm_status_updates_feed_template_frontpage_css);
	} else {
		$newsfeed_html = tm_status_updates_render_results_feed($results_html, "Latest Updates", "", "", "", $tm_status_updates_feed_template_frontpage_css);
	}

	// store in cache
	// for public users
	if (!user_is_logged_in()) {
		if ($cache_time > 0) {
			cache_set($cache_key, $newsfeed_html, 'cache', time() + $cache_time);
		}
	}

	// load js libraries
	tm_status_updates_display_include_js(true, false, false);

	// return html
	return $newsfeed_html;
}


