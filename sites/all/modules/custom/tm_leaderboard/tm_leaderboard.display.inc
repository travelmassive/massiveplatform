<?php

// tm_leaderbaord.display.inc - display methods

function tm_leaderboard_render() {

	global $conf;
	global $user;

	// check cache
	$cache = cache_get('tm-reports-all-chapter-insights', 'cache');
	if (empty($cache)) {
		return "Sorry, the Chapter Leaderboard is not available right now. Please check back later.";
	}

	// test data
	// $cache = tm_leaderboard_get_test_data();

	// helper with column names
	$column_names = $cache->data[0];
	$column_name_lookup = array();
	foreach ($column_names as $val => $name) {
		$column_name_lookup[$name] = $val;
	}

	// helper with column indexes
	$count = 0;
	$chapter_name_indexes = array();
	foreach($cache->data as $row) {
		$count++;
		if ($count == 1) {
			continue;
		}
		$chapter_name_indexes[$row[1]] = $count - 1;
	}
	$column_name_lookup = array();
	foreach ($column_names as $val => $name) {
		$column_name_lookup[$name] = $val;
	}

	$scores = tm_leaderboard_calculate_scores();

	// page title
	$max_listings = $conf["tm_leaderboard_global_show_results"];
	$page_title = 'Chapter Leaderboard - Top ' . $max_listings;

	// filter by region
	$regions = tm_base_get_all_regions();
	$region_code = "";
	if (isset($_GET["region"])) {
		$region = $_GET["region"];
		if (isset($regions[$_GET["region"]])) {
			$region_code = $_GET["region"];
			$region_name = $regions[$region_code];
			$page_title = "Chapter Leaderboard &mdash; " . $regions[$region_code];
		}
	}

	// filter by country
	$countries = tm_base_get_all_country_names();
	$country_code = "";
	if (isset($_GET["country"])) {
		$country = $_GET["country"];
		if (isset($countries[$_GET["country"]])) {
			$country_code = $_GET["country"];
			$country_name = $countries[$country_code];
			$page_title = "Chapter Leaderboard &mdash; " . $countries[$country_code];
		}
	}

	// show all
	$showall = false;
	if (isset($_GET["showall"])) {
		if (!in_array("chapter leader", $user->roles) and !in_array("moderator", $user->roles)) {
			drupal_set_message("Sorry, you don't have access to global leaderboard", "warning");
			drupal_goto("/leaderboard");
		}
		$showall = true;
		$page_title = "Chapter Leaderboard &mdash; All Rankings";
		drupal_set_message($conf["tm_leaderboard_chapter_leaders_tip"], "help-tip");
	}

	$html = "";
	$html .= '<div id="leaderboard">';
  	$html .= '<h1>' . $page_title . '</h1>';

  	$html .= "<table width=100%>";

  	$html .= "<thead>";
  	$html .= "<tr>";
  	$html .= "<th>Ranking</th>";
  	$html .= "<th class='left'>Chapter</th>";
  	$html .= "<th class='hide_narrow'>Leaders</th>";
  	$html .= "<th class='hide_narrow'>Total Members</th>";
  	$html .= "<th class='hide_narrow'>Events Past Year</th>";
  	$html .= "<th class='hide_narrow'>Attendees Past Year</th>";
  	$html .= "<th>(Points)</th>";
  	$html .= "</tr>";
  	$html .= "</thead>";

  	$ranking = 1;
	foreach($scores as $chapter_name => $score) {

		$chapter_name_index = $chapter_name_indexes[$chapter_name];

		// filter by region
		if ($region_code != "") {
			if ($cache->data[$chapter_name_index][$column_name_lookup["continent_code"]] != $region_code) {
				continue;
			}
		}

		// filter by country
		if ($country_code != "") {
			if ($cache->data[$chapter_name_index][$column_name_lookup["country_code"]] != $country_code) {
				continue;
			}
		}
		
		// show all
		if (!$showall) {
			if ($cache->data[$chapter_name_index][$column_name_lookup["num_events_past_12_months"]] == 0) {
				continue;
			}
			if ($ranking > $max_listings) {
				continue;
			}
		}		

		// load chapter
		$chapter_id = $cache->data[$chapter_name_index][$column_name_lookup["chapter_id"]];
		$chapter = node_load($chapter_id);

		$html .= "<tr>";
		$html .= "<td><strong>" . $ranking . ".</strong></td>";

		$chapter_url = drupal_get_path_alias("node/" . $chapter_id);
		$html .= "<td nowrap class='left'><a target='_blank' href='" . $chapter_url ."'>" . $chapter_name . "</a></td>";

		$html .= "<td nowrap class='left hide_narrow'>";
		$html .= tm_leaderboard_render_chapter_leaders($chapter);
		$html .= "</td>";

		$html .= "<td class='hide_narrow' nowrap>" . $cache->data[$chapter_name_index][$column_name_lookup["members_total"]] . "</td>";
		$html .= "<td class='hide_narrow' nowrap>" . $cache->data[$chapter_name_index][$column_name_lookup["num_events_past_12_months"]] . "</td>";
		$html .= "<td class='hide_narrow' nowrap>" . $cache->data[$chapter_name_index][$column_name_lookup["num_event_registrations_12_months"]] . "</td>";
		$html .= "<td>" . $score . "</td>";

		$html .= "</tr>";
		$ranking++;
	}

	$html .= "</table>";

	$html .= "<div id='leaderboard_links'>
		Leaderboards &mdash; 
		<a href='/leaderboard'>Top " . $conf["tm_leaderboard_global_show_results"] . "</a> &middot; 
		<a href='/leaderboard?region=AS'>Asia</a>  &middot;
		<a href='/leaderboard?region=AF'>Africa</a>  &middot;
		<a href='/leaderboard?region=NA'>North America</a>  &middot;
		<a href='/leaderboard?region=SA'>South America</a>  &middot;
		<a href='/leaderboard?region=OC'>Oceania</a>";

	if (in_array("chapter leader", $user->roles) or in_array("moderator", $user->roles)) {
		$html .= "<br><i>Chapter Leaders</i> &mdash; <a href='/leaderboard?showall'>See All Rankings</a>";
	}

	$html .= '</div>';

	return $html;
}

/**
 * Render chapter leaders
 */
function tm_leaderboard_render_chapter_leaders($chapter) {

	global $conf;

	$html = "";

	// Show chapter leader avatars
	$leader_uids = array();
	if (isset($chapter->field_chapter_leaders[LANGUAGE_NONE])) {
		foreach ($chapter->field_chapter_leaders[LANGUAGE_NONE] as $chapter_leader) {

			// load account
			$leader_uid = $chapter_leader['target_id'];
			$account = user_load($leader_uid);

			// Set avatar
			$avatar_url = image_style_url("avatar", _tm_users_get_avatar_uri($account));
			$img_url = file_create_url($avatar_url);
			$user_url = drupal_get_path_alias("user/" . $account->uid);

			$html .= "<a target='_blank' href='" . $user_url . "'><img class='leaderboard_avatar' src='" . $img_url . "'></a> ";
		}
	}

	return $html;
}

/**
 * Sample data
 * 
 */
function tm_leaderboard_get_test_data() {

	// drush tm-reports-chapter-insights-cache
	// drush php-eval "print(json_encode(cache_get('tm-reports-all-chapter-insights', 'cache')));"
	// replace $json with data from output

	$json = '';
	return json_decode($json);
}