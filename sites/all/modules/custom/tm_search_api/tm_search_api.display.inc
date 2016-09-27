<?php

// tm_search_api.display.inc - search display methods

/* Search view */
function tm_search_api_view() {

	global $conf;
	global $user;

	// css and js
	drupal_add_css(drupal_get_path('module', 'tm_search_api') . '/css/search-results.css');
	drupal_add_js(drupal_get_path('module', 'tm_search_api') . '/js/search-results.js');

	// add js vars

	// site name
	drupal_add_js(array('tm_search' => array('site_name' => $conf["tm_site_name"])), array('type' => 'setting'));

	// blog
	drupal_add_js(array('tm_search' => array('blog_path' => "blog")), array('type' => 'setting'));

	// marketplace
	if (isset($conf["tm_marketplace_menu_path"])) {
		drupal_add_js(array('tm_search' => array('marketplace_path' => $conf["tm_marketplace_menu_path"])), array('type' => 'setting'));
	}

	// discussions
	if (isset($conf["tm_discuss_menu_path"])) {
		drupal_add_js(array('tm_search' => array('discuss_path' => $conf["tm_discuss_menu_path"])), array('type' => 'setting'));
	}

	// load template
	$template = file_get_contents(drupal_get_path('module', 'tm_search_api') .'/templates/tm_search_api_form.tpl.php');
	$template = str_replace("__EXAMPLE_SEARCH__", $conf["tm_search_api_example_search"], $template);

	// add search tips and no results text from conf
	$search_tips = "";
	foreach ($conf["tm_search_api_tips"] as $search_tip) {
		$search_tips .= "<p class='search tip'><span class='search tip-prefix'>Tip &raquo; </span>" . $search_tip . "</p>";
	}
	$template = str_replace("__SEARCH_TIPS__", $search_tips, $template);
	$template = str_replace("__NO_RESULTS_TEXT__", $conf["tm_search_api_no_results_text"], $template);
	$template = str_replace("__SEARCH_WELCOME_MESSAGE__", tm_search_api_welcome_message(), $template);
	$template = str_replace("__SEARCH_HELP_TEXT__", $conf["tm_search_api_help_text"], $template);
	$template = str_replace("__SEARCH_HELP_TIPS__", $conf["tm_search_api_help_tips"], $template);

	// include branded search banner
	$search_page_banner = tm_branding_get_element("search_page_banner");
	$search_page_link = tm_branding_get_element("search_page_link");
	if (($search_page_banner != "") and ($search_page_link != "")) {
		$search_banner_html = "<div style='margin: auto; text-align: center;'><a target='_blank' href='" . $search_page_link . "'><img style='width: 85%; height: 85%; max-width: 500px;' src='" . $search_page_banner . "'></a></div>";
		$template = $search_banner_html . $template;
	}

	// create form
	$form['html'] = array(
    	'#type' => 'item',
    	'#markup' => $template,
  	);
  	return $form;

}

/* Create welcome text */
function tm_search_api_welcome_message() {

	// check cache
	$cache = cache_get('tm-search-api-welcome-message', 'cache');
	if (!empty($cache)) {
		return($cache->data);
	}
	
	// number of members
	$query = "SELECT COUNT(*) amount FROM {users} u";
	$num_members = db_query($query)->fetch();
	$num_members = $num_members->amount;

	// number of organizations
	$query = "SELECT COUNT(*) amount FROM {node} n ".
	      	 "WHERE n.type = :type";
	$num_organizations = db_query($query, array(':type' => "organization"))->fetch();
	$num_organizations = $num_organizations->amount;

	$members_rounded = floor($num_members / 100) * 100;
	$organizations_rounded = floor($num_organizations / 100) * 100;

	// in case we don't yet have over 100 members
	if ($members_rounded == 0) {
		$members_rounded = $num_members;
	}

	// in case we don't yet have over 100 organizations
	if ($organizations_rounded == 0) {
		$organizations_rounded = $num_organizations;
	}

	// example:
	// Discover over 18,000 members and 1,200 companies
	$welcome_message = "Discover more than " . number_format($members_rounded) . " members and " . number_format($organizations_rounded) . " companies";

	// return api data in json format
  	//cache_set('tm-search-api-welcome-message', $welcome_message, 'cache', time() + 900); // 15 mins
	return $welcome_message;
}

/* Modify metatags */
function tm_search_api_metatag_metatags_view_alter(&$output, $instance) {

	global $conf;

	// remove following metatags as we create them in tm_preprocess_html
	if (current_path() == "search") {
		unset($output['canonical']);
		unset($output['og:url']);
		unset($output['og:title']);
	}
}

/* Return html search results for search page */
function tm_search_api_ajax() {

	global $user;
	global $conf;

	// Step 1. Check input
	if (!isset($_GET["query"])) {
		return;
	}

	$page = 1;
	if (isset($_GET["page"])) {
		$page = intval($_GET["page"]);
	}
	if ($page == 0) { 
		return;
	}

	// Step 2. Don't allow anonymous users see beyond first page
	if (($user->uid == 0) && ($page > 1)) {
		return;
	}

	// Step 3. Get user input
	$search_query = trim($_GET["query"]);
	
	$filter_people = true;
	$filter_events = true;
	$filter_past_events = false;
	$filter_chapters = true;
	$filter_companies = true;

	if (isset($_GET["filter_people"])) {
		$filter_people = ($_GET["filter_people"] == "true");
	}
	if (isset($_GET["filter_events"])) {
		$filter_events = ($_GET["filter_events"] == "true");
	}
	if (isset($_GET["filter_past_events"])) {
		$filter_past_events = ($_GET["filter_past_events"] == "true");
	}
	if (isset($_GET["filter_chapters"])) {
		$filter_chapters = ($_GET["filter_chapters"] == "true");
	}
	if (isset($_GET["filter_companies"])) {
		$filter_companies = ($_GET["filter_companies"] == "true");
	}

	// Check we have at least one filter
	if (!$filter_people 
		&& !$filter_events 
		&& !$filter_past_events 
		&& !$filter_chapters
		&& !$filter_companies) {
		return;
	}

	// Step 4. Construct query array
	$query = array("search_query" => $search_query,
				   "filter_people" => $filter_people,
				   "filter_events" => $filter_events,
				   "filter_past_events" => $filter_past_events,
				   "filter_chapters" => $filter_chapters,
				   "filter_companies" => $filter_companies,
				   "page" => $page,
				   );

	
	// Step 5. Run query
	// Catch excections if they occur
	try {

		// If query is not single word and not in quotes, perform additional query
		// Perform exact match query and put this at the top
		// This helps to bring exact match names to the top: ie: Travel Massive, etc
		// Only operate on first page
		// Note: search_api_db does not perform exact matching so this may not work exactly as expected
		// see: https://www.drupal.org/node/2043339#comment-7654017

		$insert_top_results = array(); // results to insert
		$performance_time = 0; // keep track of performance time
		$num_top_matches = $conf["tm_search_api_top_results_display"];
		$top_results = array();
		if ((strpos($query["search_query"], '"') === false)
			&& (strpos($query["search_query"], ' ') !== false)) {

			$search_exact_query = $query;
			$search_exact_query["page"] = 1;
			$top_search_query = '"' . $query["search_query"] . '"';

			// Optional, strip spaces
			// ie: Travel Massive = travelmassive
			if ($conf["tm_search_api_top_results_strip_spaces"]) {
				$top_search_query .= ' ' . str_replace(" ", "", $query["search_query"]);
			}

			$search_exact_query["search_query"] = $top_search_query;

			// How many exact matches to insert to top
			$search_exact_query["limit_num_results"] = $num_top_matches; 
			
			// Perform search for top matches
			$results = tm_search_api_fetch_results($search_exact_query);
			$top_results = $results['results'];

			// Insert top results for quote match and insert at top
			$performance_time = $results['performance']['complete']; // keep track of performance time
		}
		
		// Search general search terms
		$results = tm_search_api_fetch_results($query);

		// Merge top_results and results
		if ($query["page"] == 1) {
			// first page, top_results go at top
			$all_results = array_merge($top_results, $results['results']);
		} else {
			// subsequent pages, hide any top_results
			$all_results = array();
			foreach($results['results'] as $result) {
				if (!array_key_exists($result['id'], $top_results)) {
					$all_results[$result['id']] = $result;
				}
			}
		}

		$results['results'] = $all_results;
		$results['performance']['complete'] = $results['performance']['complete'] + $performance_time;

	} catch (Exception $e) {
		if ($conf["tm_search_api_catch_exceptions"]) {
			$html = "Sorry, a search error occured.";
			echo $html;
			drupal_exit();
		} else {
			throw $e;
		}	
	}
	
	// If moderator or chapter leader, then search for unapproved member by email address
	// (For performance the search index doesn't index unapproved members)
	// Note: this doesn't need to be cached as we are inserting it live
	if (($filter_people) and (strpos($search_query, "@") !== false)) {
		if ((in_array("administrator", $user->roles)) or (in_array("moderator", $user->roles)) 
			or (in_array("chapter leader", $user->roles))) {

			// Find first email address
			preg_match("/[_a-z0-9-]+(\.[_a-z0-9-]+)*@[a-z0-9-]+(\.[a-z0-9-]+)*(\.[a-z]{2,3})/i", $search_query, $email_matches);

			if (sizeof($email_matches) > 0) {

				// Search for user
				$unapproved_user = tm_search_api_find_unapproved_user_by_mail($email_matches[0]);

				// If user is found, insert to top of result set
				if ($unapproved_user != null) {

					$insert_result = array("user/".$unapproved_user->uid => array("id" => "user/".$unapproved_user->uid, "score" => 1));
					$results['results'] = array_merge($insert_result, $all_results);
					$results['result count'] = $results['result count'] + 1;
				}
			}
		}
	}

	// Step 6. Render results
	$html = tm_search_api_render_results_html($results, $search_query);

	// Step 7. Hidden fields for result meta data
	$html .= "<div id='search-results-meta' style='display:none'>";
	$html .= "<input type='hidden' id='search-results-last-page' value='" . $results['last page'] . "'>";
	$html .= "<input type='hidden' id='search-results-total' value='" . $results['result count'] . "'>";
	$html .= "<input type='hidden' id='search-results-time' value='" . round($results['performance']['complete'], 2) . "'>";

	$html .= "<input type='hidden' id='search-results-raw-query' value='" . json_encode($results['tm_debug_search_raw_query']) . "'>";
	$html .= "<input type='hidden' id='search-results-keywords' value='" . json_encode($results['tm_debug_search_keywords']) . "'>";
	$html .= "<input type='hidden' id='search-results-options' value='" . json_encode($results['tm_debug_search_options']) . "'>";
	$html .= "<input type='hidden' id='search-results-query-no-options' value='" . json_encode($results['tm_debug_search_query_no_options']) . "'>";
	$html .= "<input type='hidden' id='search-results-cached' value='" . json_encode($results['tm_debug_results_cached']) . "'>";


	$html .= "</div>";

	// Step 8. Return html
	echo $html;
	drupal_exit();
}

/* Render html results */
function tm_search_api_render_results_html($results, $search_query, $result_item_count = 0) {

	global $conf;

	$results_html_items = array();

	foreach ($results['results'] as $result) {

		$result_item_count++;

		// either "user/uid" or "node/nid";
		$parts = explode("/", $result['id']);

		// user result
		if ($parts[0] == "user") {
			$results_user = user_load($parts[1]);
			$results_html_items[] = tm_search_api_render_result_user($result, $results_user, $search_query, $result_item_count);
		}

		// node (chapter, event, company)
		if ($parts[0] == "node") {

			$node = node_load($parts[1]);

			// chapter result
			if ($node->type == "chapter") {
				$results_html_items[] = tm_search_api_render_result_chapter($result, $node, $search_query, $result_item_count);
			}

			// event result
			if ($node->type == "event") {
				$results_html_items[] = tm_search_api_render_result_event($result, $node, $search_query, $result_item_count);
			}

			// organization result
			if ($node->type == "organization") {
				$results_html_items[] = tm_search_api_render_result_organization($result, $node, $search_query, $result_item_count);
			}
		}

	}

	$results_html = implode("\n", $results_html_items);

	return $results_html;
}

/* Render user search result */
function tm_search_api_render_result_user($result, $results_user, $search_query, $result_item_count) {

	global $conf;

	// open in new window
	// set via tm_search_open_external
	$href_target = null;
	if ($conf["tm_search_open_external"] == true) {
		$href_target = "_blank";
	}

	// user image
	if (isset($results_user->field_avatar[LANGUAGE_NONE][0]['uri'])) {
		$image_url = image_style_url("avatar", $results_user->field_avatar[LANGUAGE_NONE][0]['uri']);
	} else {
		$default_image_uri = _tm_users_get_default_avatar($conf["tm_images_default_field_avatar"], $results_user->uid);
		$image_url = image_style_url("avatar", $default_image_uri);
	}

	// privacy mode
	$privacy_mode_text = "";
	if (isset($results_user->field_user_has_private_profile[LANGUAGE_NONE][0]['value'])) {
	  if ($results_user->field_user_has_private_profile[LANGUAGE_NONE][0]['value']) {
		$privacy_mode_text = " (Private profile)";
	  }
	}

	// set render fields
	$result_url = drupal_get_path_alias("user/" . $results_user->uid);
	$result_image_url = $image_url;
	$result_title = l("People > " . $results_user->field_user_first_name[LANGUAGE_NONE][0]['value'] . " " . $results_user->field_user_last_name[LANGUAGE_NONE][0]['value'] . $privacy_mode_text, $result_url, array('attributes' => array('target'=>$href_target)));

	// about
	$result_text = $results_user->field_about_me[LANGUAGE_NONE][0]['safe_value'];

	// location
	if (sizeof($results_user->field_location_city) > 0) {
		$result_text .= " | " . $results_user->field_location_city[LANGUAGE_NONE][0]['safe_value'];
	}

	// organization and role
	if (sizeof($results_user->field_job_organization) > 0) {
		$result_text .= " | " . $results_user->field_job_organization[LANGUAGE_NONE][0]['safe_value'];
	}
	if (sizeof($results_user->field_job_role) > 0) {
		$result_text .= " | " . $results_user->field_job_role[LANGUAGE_NONE][0]['safe_value'];
	}

	// segment
	if (sizeof($results_user->field_segment) > 0) {
		$taxonomy_term = taxonomy_term_load($results_user->field_segment[LANGUAGE_NONE][0]['tid']);
		$result_text .= " | " . $taxonomy_term->name;
		$taxonomy_parent = taxonomy_get_parents($taxonomy_term->tid);
		if ($taxonomy_parent) {
			$result_text .= " | " . array_values($taxonomy_parent)[0]->name . " > " . $taxonomy_term->name;
		} else {
			$result_text .= " | " . $taxonomy_term->name;
		}
	}

	// social links
	if (sizeof($results_user->field_link_website) > 0) {
		$result_text .= " | Website: " . $results_user->field_link_website[LANGUAGE_NONE][0]['url'];
	}
	if (sizeof($results_user->field_link_snapchat) > 0) {
		$result_text .= " | Snapchat: " . $results_user->field_link_snapchat[LANGUAGE_NONE][0]['url'];
	}
	if (sizeof($results_user->field_link_twitter) > 0) {
		$result_text .= " | Twitter: " . $results_user->field_link_twitter[LANGUAGE_NONE][0]['url'];
	}
	if (sizeof($results_user->field_link_instagram) > 0) {
		$result_text .= " | Instagram: " . $results_user->field_link_instagram[LANGUAGE_NONE][0]['url'];
	}
	if (sizeof($results_user->field_link_facebook) > 0) {
		$result_text .= " | Facebook: " . $results_user->field_link_facebook[LANGUAGE_NONE][0]['url'];
	}
	if (sizeof($results_user->field_link_linkedin) > 0) {
		$result_text .= " | LinkedIn: " . $results_user->field_link_linkedin[LANGUAGE_NONE][0]['url'];
	}
	if (sizeof($results_user->field_link_youtube) > 0) {
		$result_text .= " | YouTube: " . $results_user->field_link_youtube[LANGUAGE_NONE][0]['url'];
	}
	if (sizeof($results_user->field_link_vimeo) > 0) {
		$result_text .= " | Vimeo: " . $results_user->field_link_vimeo[LANGUAGE_NONE][0]['url'];
	}
	if (sizeof($results_user->field_link_strava) > 0) {
		$result_text .= " | Strava: " . $results_user->field_link_strava[LANGUAGE_NONE][0]['url'];
	}

	// user questions
	if (sizeof($results_user->field_user_question_1) > 0) {
		$result_text .= " | " . $results_user->field_user_question_1[LANGUAGE_NONE][0]['safe_value'];
	}
	if (sizeof($results_user->field_user_question_2) > 0) {
		$result_text .= " | " . $results_user->field_user_question_2[LANGUAGE_NONE][0]['safe_value'];
	}
	if (sizeof($results_user->field_user_question_3) > 0) {
		$result_text .= " | " . $results_user->field_user_question_3[LANGUAGE_NONE][0]['safe_value'];
	}
	if (sizeof($results_user->field_user_question_4) > 0) {
		$result_text .= " | " . $results_user->field_user_question_4[LANGUAGE_NONE][0]['safe_value'];
	}
	if (sizeof($results_user->field_user_question_5) > 0) {
		$result_text .= " | " . $results_user->field_user_question_5[LANGUAGE_NONE][0]['safe_value'];
	}

	// strip tags
	$result_text = strip_tags($result_text);

	$result_info = tm_search_api_result_highlight($result_text, $search_query); 

	// render item
	$html = tm_search_api_render_search_result($result_url, $result_image_url, $result_title, $result_info, $result_item_count);
	return $html;

}

/* Render chapter search result */
function tm_search_api_render_result_chapter($result, $chapter, $search_query, $result_item_count) {

	global $conf;

	// open in new window
	// set via tm_search_open_external
	$href_target = null;
	if ($conf["tm_search_open_external"] == true) {
		$href_target = "_blank";
	}

	// chapter image
	if (isset($chapter->field_image[LANGUAGE_NONE][0]['uri'])) {
		$image_url = image_style_url("banner", $chapter->field_image[LANGUAGE_NONE][0]['uri']);
	} else {
		$default_image_uri = _tm_chapters_get_default_image($conf["tm_images_default_field_image"], $chapter->nid);
		$image_url = image_style_url("banner", $default_image_uri);
	}

	// set render fields
	$result_url = drupal_get_path_alias("node/" . $chapter->nid);
	$result_image_url = $image_url;
	$result_title = l("Chapters > " . $chapter->title . " " . $conf["tm_site_name"], $result_url, array('attributes' => array('target'=>$href_target)));
	$result_text = $chapter->body[LANGUAGE_NONE][0]['safe_value'];

	// country
	if (sizeof($organization->field_country) > 0) {
		$country = country_load($organization->field_country[LANGUAGE_NONE][0]['iso2']);
		if ($country) {
			$result_text .= " | " . $country->label();
		}
	}

	$result_info = tm_search_api_result_highlight($result_text, $search_query); 

	// render item
    $html = tm_search_api_render_search_result($result_url, $result_image_url, $result_title, $result_info, $result_item_count);

	return $html;

}

/* Render event search result */
function tm_search_api_render_result_event($result, $event, $search_query, $result_item_count) {

	global $conf;

	// open in new window
	// set via tm_search_open_external
	$href_target = null;
	if ($conf["tm_search_open_external"] == true) {
		$href_target = "_blank";
	}

	// chapter image
	if (isset($event->field_image[LANGUAGE_NONE][0]['uri'])) {
		$image_url = image_style_url("banner", $event->field_image[LANGUAGE_NONE][0]['uri']);
	} else {
		$default_image_uri = _tm_events_get_default_image($conf["tm_images_default_field_image"], $event->nid);
		$image_url = image_style_url("banner", $default_image_uri);
	}

	// if event has chapter
	if (isset($event->field_chapter[LANGUAGE_NONE][0]['target_id'])) {
		$event_chapter = node_load($event->field_chapter[LANGUAGE_NONE][0]['target_id']);
		//$event_chapter->title;
	}

	// set render fields
	$result_url = drupal_get_path_alias("node/" . $event->nid);
	$result_image_url = $image_url;
	$preview_mode_text = "";
	if (tm_events_is_preview_mode($event->nid)) {
		$preview_mode_text = " (Preview mode)";
	} 
	$result_title = l("Events > " . $event->title . $preview_mode_text, $result_url, array('attributes' => array('target'=>$href_target)));

	// show when event is
	$current_date = new DateTime('now', new DateTimeZone('UTC'));
	$event_date = $event->field_event_date[LANGUAGE_NONE][0]['value'];
	$event_timezone = _tm_get_event_timezone($event);
	$event_datetime = new DateTime($event_date, new DateTimeZone($event_timezone));

	$seconds_ago = $event_datetime->format('U') - $current_date->format('U');
	if ($seconds_ago < 0) {
		$human_time = _tm_event_time_elapsed($seconds_ago * -1) . " ago";
	} else {
		$human_time = "in " . _tm_event_time_elapsed($seconds_ago);
	}

	$formatted_date = date_format(date_create($event_date),"F jS Y");
    $date_text = $formatted_date . " (" . $human_time . ") ";

    // event body
    $result_text = "";
	if (sizeof($event->body) > 0) {
		$result_text .= $event->body[LANGUAGE_NONE][0]['safe_value'];
	}

	// show location if allowed
	if ($event->field_event_hide_location[LANGUAGE_NONE][0]['value'] == 0) {
		$result_text .= " | " . $event->field_location[LANGUAGE_NONE][0]['safe_value'];
	} 
	$result_info = $date_text . tm_search_api_result_highlight($result_text, $search_query); 

	// render item
    $html = tm_search_api_render_search_result($result_url, $result_image_url, $result_title, $result_info, $result_item_count);

	return $html;

}

/* Render organisation search result */
function tm_search_api_render_result_organization($result, $organization, $search_query, $result_item_count) {

	global $conf;

	// open in new window
	// set via tm_search_open_external
	$href_target = null;
	if ($conf["tm_search_open_external"] == true) {
		$href_target = "_blank";
	}
	
	// organization image
	if (isset($organization->field_avatar[LANGUAGE_NONE][0]['uri'])) {
		$image_url = image_style_url("avatar", $organization->field_avatar[LANGUAGE_NONE][0]['uri']);
	} else {
		// note: organization will always have a logo as it's mandatory field
		$image_url = image_style_url("avatar", $conf["tm_images_default_field_avatar"]);
	}

	// set render fields
	$result_url = drupal_get_path_alias("node/" . $organization->nid);
	$result_image_url = $image_url;
	$result_title = l("Companies > " . $organization->title, $result_url, array('attributes' => array('target'=>$href_target)));
	$result_text = $organization->body[LANGUAGE_NONE][0]['safe_value'];

	// country
	if (sizeof($organization->field_country) > 0) {
		$country = country_load($organization->field_country[LANGUAGE_NONE][0]['iso2']);
		if ($country) {
			$result_text .= " | " . $country->label();
		}		
	}

	// segment
	if (sizeof($organization->field_segment) > 0) {
		$taxonomy_term = taxonomy_term_load($organization->field_segment[LANGUAGE_NONE][0]['tid']);
		$taxonomy_parent = taxonomy_get_parents($taxonomy_term->tid);
		if ($taxonomy_parent) {
			$result_text .= " | " . array_values($taxonomy_parent)[0]->name . " > " . $taxonomy_term->name;
		} else {
			if (isset($taxonomy_term->name)) {
				$result_text .= " | " . $taxonomy_term->name;
			}
		}
	}

	$result_info = tm_search_api_result_highlight($result_text, $search_query);

	// render item
    $html = tm_search_api_render_search_result($result_url, $result_image_url, $result_title, $result_info, $result_item_count);

	return $html;

}

/* Render a snippet of the search result highlighting the query words */
function tm_search_api_result_highlight($text, $search_query) {

	mb_internal_encoding('UTF-8');
	$snippet_words = 12; // must be even
	$snippet = 

	// Step 1. Split search query into options and terms
	$parsed = tm_search_parse_search_query($search_query);
	$search_terms = $parsed["search_terms"];
	$search_query_no_options = $parsed["search_query_no_options"];
	$query_options = $parsed["query_options"];

	// Append any in: options so we can highlight them
	// ie: in:Twitter, in:Instagram
	if (isset($query_options["has"])) {
		foreach ($query_options["has"] as $query_in) {
			$search_terms[] = $query_in; // append so we can highight 
		}
	}

	$words = $search_terms;

	// Step 2. Create snippet around first word in query
	$snippet = tm_search_api_extract_relevant($words, $text, $rellength=300, $prevcount=50, $indicator='...');

	// Step 3. Highlight search words
	foreach ($words as $word) {
		$highlighted = preg_filter('/' . preg_quote(strip_tags($word)) . '/iu', '__BEGIN_SPAN__$0__END_SPAN__</span>', $snippet);
		if (!empty($highlighted)) {
			$snippet = $highlighted;
		}
	}

	// Step 4. Replace tokens with html
	$snippet = str_replace("__BEGIN_SPAN__", "<span class='search highlight'>", $snippet);
	$snippet = str_replace("__END_SPAN__", "</span>", $snippet);

	// Step 5. Return highlighted snippet
	return $snippet;
}

/* Render search results into html template */
function tm_search_api_render_search_result($result_url, $result_image_url, $result_title, $result_info, $result_item_count) {
	global $tm_search_api_result_template;
	$html = str_replace("__RESULT_URL__", $result_url, $tm_search_api_result_template);
	$html = str_replace("__RESULT_IMAGE_URL__", $result_image_url, $html);
	$html = str_replace("__RESULT_TITLE__", $result_title, $html);
	$html = str_replace("__RESULT_INFO__", $result_info, $html);
	$html = str_replace("__RESULT_LI_CLASS__", "search-result-item-" . $result_item_count, $html);
	return $html;
}

/**
 * Add html meta tags
 * Add omnisearch tag
 * Called from tm/themes/template.php
 */
function tm_search_api_preprocess_html(&$variables, $hook) {

	global $conf;

	// omnisearch
	// ie: <link rel="search" type="application/opensearchdescription+xml" title="ZitSeng.com" href="/opensearch.xml" />
	// see: https://zitseng.com/archives/10130
	if (current_path() == "frontpage") {
		if (module_exists("tm_search_api")) {
			$meta = array(
				'#tag' => 'link', 
				'#attributes' => array(
					'rel' => 'search',
					'type' => 'application/opensearchdescription+xml',
					'title' => $conf['tm_search_meta_opensearch_title'],
					'href' => "/opensearch.xml"
				),
			);
			drupal_add_html_head($meta, 'tm_search_api_opensearch_tag');
		}
	}
	
	// Check we are on search page
  	$search_path = "search";
  	if (current_path() != $search_path) {
    	return;
  	}
	
	// add tag
	$variables['classes_array'][] = "tm_search_api";

	// gererate canonical url
	$search_path = "search";
	$canonical = url('/' . $search_path, array('absolute' => TRUE)) . "/";

	// canonical url
	$meta = array(
	  '#tag' => 'link', 
	  '#attributes' => array(
	    'rel' => 'canonical', 
	    'href' => $canonical,
	  ),
	);
	drupal_add_html_head($meta, 'canonical');

	// og:url
	$meta = array(
	  '#tag' => 'meta', 
	  '#attributes' => array(
	    'name' => 'og:url', 
	    'content' => $canonical,
	  ),
	);
	drupal_add_html_head($meta, 'og:url');

	// og:title
	if (isset($conf['tm_search_meta_og_title'])) {
	  $meta = array(
	    '#tag' => 'meta', 
	    '#attributes' => array(
	      'name' => 'og:title', 
	      'content' => $conf['tm_search_meta_og_title'],
	    ),
	  );
	  drupal_add_html_head($meta, 'og:title');
	}

	// og:image
	if (isset($conf['tm_search_meta_og_image'])) {
	  $meta = array(
	    '#tag' => 'meta', 
	    '#attributes' => array(
	      'name' => 'og:image', 
	      'content' => $conf['tm_search_meta_og_image'],
	    ),
	  );
	  drupal_add_html_head($meta, 'og:image');
	}

	// description
	if (isset($conf['tm_search_meta_description'])) {
	  $meta = array(
	    '#tag' => 'meta', 
	    '#attributes' => array(
	      'name' => 'description',
	      'property' => 'og:description',
	      'content' => $conf['tm_search_meta_description'],
	    ),
	  );
	  drupal_add_html_head($meta, 'description');
	}

}

/* Find unapproved user by email */
function tm_search_api_find_unapproved_user_by_mail($mail) {

	$query = "SELECT uid FROM {users} u WHERE u.mail = :mail";
	$uid = db_query($query, array(':mail' => $mail))->fetch();

	if ($uid != null) {
		$user = user_load($uid->uid);
		if (!in_array("approved user", $user->roles)) {
			return $user;
		}
	}

	// not found
	return false;
}

/* html templates */
global $tm_search_api_result_template;
$tm_search_api_result_template = <<<EOT
<li class="search-result __RESULT_LI_CLASS__">
   <article class="card contained view-mode-grid search-result clearfix">
    <!-- Needed to activate contextual links -->
    	<div style="padding-left: 2em; padding-right: 4em;">
    	
	        <div class="media">
		        <div class="avatar">
		        	<span class="badge-search-result">
		        		 <a href="__RESULT_URL__" class="search-result" target="_blank"><img class="seach-result-image" typeof="foaf:Image" src="__RESULT_IMAGE_URL__" width="256" height="256" alt=""></a>
		        	</span>
		        </div>
	        </div>

	    	<div class="teaser">
	      		<span class="prime-title search-result">__RESULT_TITLE__</span>
	      		<p class="search-result"><span class="search-result-snippet">__RESULT_INFO__</span>
	      	</div>
	     
      	</div>
      	
	</article>
</li>
EOT;

// sorry, not the drupal way here...
global $tm_search_api_form_template;
$tm_search_api_form_template = <<<EOT
 <form class="search-form" action="/search" method="GET" id="search-form" accept-charset="UTF-8">
  <div>
    <div class="container-inline form-wrapper" id="edit-basic">
      <div class="form-item form-type-textfield form-item-keys">
        <label for="edit-keys">Enter your keywords </label>
        <input type="text" id="edit-keys" name="query" value="" size="40" maxlength="255" class="form-text">
      </div>
    <input type="submit" id="search-box-submit" value="Search" class="form-submit"></div>
  </div>
</form>
<p style='font-size: 0.85rem; line-height: 1.25rem; color: #636e79; margin-bottom: 0;'>Find members, companies, events or chapters. Or, visit our <a href='/search'>Search page</a>.</p>
EOT;
