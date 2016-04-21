<?php

// tm_search_api.search.inc - search methods

/* 
	Split query into options and keywords
	Returns an array with the following:
	- search_keywords - Sydney, Thailand, etc
	- query_options - in:Australia role:Founder, etc
	- search_query_no_options - search query with options removed
*/
function tm_search_parse_search_query($search_query) {

	global $conf;
	mb_internal_encoding('UTF-8'); // for regex

	// Step 1. Match search options

	// ie: 
	// in:Australia has:twitter role:CEO
	// notes:
	// - lowercase the options key
	// - will match quotes (ie: "San Francisco")
	// - accepts up to once space between key and : - ie: "role: CEO"
	$query_options = array();
	$raw_query_options = array(); // keep track so we can remove them from search query
	preg_match_all('/([A-Za-z]{1,16}):\s{0,1}(\".*?\"|.*?)(\s|$)/m', $search_query, $matches);

	$max_options = 8; // maximum number of search options allowed
	$num_options = sizeof($matches[0]);
	for($i = 0; (($i < $num_options) && ($i < $max_options)); $i++) {
		$option_key = strtolower($matches[1][$i]);
		$option_value = strtolower($matches[2][$i]);
		$option_value = trim(str_replace("\"", "", $option_value)); // remove quotes ie: "San Francisco"
		if (!array_key_exists($option_key, $query_options)) {
			$query_options[$option_key] = array();
		}
		$query_options[$option_key][] = $option_value;
		$raw_query_options[] = $matches[0][$i];
	}

	// Step 2. Remove options from search query
	$search_query_no_options = $search_query;
	foreach ($raw_query_options as $raw_query_option) {
		$search_query_no_options = str_replace($raw_query_option, "", $search_query_no_options);
	}

	// Step 3. Extract search keywords from search query
	// Split search string into keywords, preserving keywords contained in quotes
	// http://stackoverflow.com/questions/2202435/php-explode-the-string-but-treat-words-in-quotes-as-a-single-word
	preg_match_all('/"(?:\\\\.|[^\\\\"])*"|\S+/m', $search_query_no_options, $keyword_matches);
	// remove quotes from keywords
	$search_terms = array();
	foreach ($keyword_matches[0] as $keyword) {
		$keyword = str_replace("\"", "", $keyword);
		$keyword = str_replace("'", "", $keyword);
		$search_terms[] = trim($keyword);
	}

	// Step 4. Remove one and two letter words from search
	$allowed_words = array();
	foreach ($search_terms as $search_term) {
		if (strlen($search_term) > 2) {
			$allowed_words[] = $search_term;
		}
	}
	$search_terms = $allowed_words;

	// Step 5. Limit search terms
	$max_search_terms = 8; // default
	if (isset($conf["tm_search_api_max_search_terms"])) {
		$max_search_terms = $conf["tm_search_api_max_search_terms"];
	}
	if (sizeof($search_terms) > $max_search_terms) {
		$search_terms = array_slice($search_terms, 0, $max_search_terms);
	}

	// (Testing) Step 7. Pluralize search terms
	// If partial matching is turned off on search server, this might help get some extra results
	// Also de-pluralize. ie: nomads = "nomads", "nomad"
	if ($conf["tm_search_api_pluralize_keywords"]) {
		foreach($search_terms as $search_term) {
			if (strtolower((substr($search_term, -1) == "s"))) {
				// remove an s
				$search_terms[] = rtrim($search_term, "s");
			} else {
				// add an s
				$search_terms[] = $search_term . "s";
			}
		}
	}

	return array("search_terms" => $search_terms,
				 "search_query_no_options" => $search_query_no_options, 
				 "query_options" => $query_options);
}


/* Fetch search results */
function tm_search_api_fetch_results($query_array) {
	
	global $conf;
	global $user;

	// Create cache key using md5 sum of unique string of the search
	$cache_key_unique_string = $query_array["search_query"] . "_" .(int)(user_is_logged_in()) . "_" . (int)$query_array["filter_people"] . "_" . (int)$query_array["filter_events"] . "_" . (int)$query_array["filter_past_events"] . "_" . (int)$query_array["filter_chapters"] . "_" . (int)$query_array["filter_companies"] . "_" . (int)$query_array["page"];
	if (isset($query_array["limit_num_results"])) {
		$cache_key_unique_string .= "_" . $query_array["limit_num_results"];
	}
	$cache_key = "tm-search-api-" . md5($cache_key_unique_string);

	// Check cache_key
	if ($conf['tm_search_cache_results_seconds'] > 0) {
	  	$cache = cache_get($cache_key, 'cache');
	  	if (!empty($cache)) {
	  		$cache->data['tm_debug_results_cached'] = true;
	  		$cache->data['performance']['complete'] = 0.01; // since we got it from cache
	    	return($cache->data);
  		}
	}
	
	// Step 1. Get search options
	$search_query = $query_array["search_query"];
	$filter_people = $query_array["filter_people"];
	$filter_events = $query_array["filter_events"];
	$filter_past_events = $query_array["filter_past_events"];
	$filter_chapters = $query_array["filter_chapters"];
	$filter_companies = $query_array["filter_companies"];
	$page = $query_array["page"];	

	// page offset
	$num_results_per_page = 10; // default
	if (isset($conf["tm_search_api_results_per_page"])) {
		$num_results_per_page = $conf["tm_search_api_results_per_page"];
	}
	$offset = ($page - 1) * $num_results_per_page;
	if (isset($query_array["limit_num_results"])) {
		if ($query_array["limit_num_results"] == 0) {
			return tm_search_api_empty_results_array($search_query, null, null);
		}
		$offset = 0;
		$num_results_per_page = $query_array["limit_num_results"];
	}

	// Step 2. Parse search query 
	// Break into into search terms and options, and create option arrays
	$parsed = tm_search_parse_search_query($search_query);
	$search_terms = $parsed["search_terms"];
	$search_query_no_options = $parsed["search_query_no_options"];
	$query_options = $parsed["query_options"];

	// initialise option arrays
	$in_queries = tm_search_api_init_option($query_options, "in"); // in:Australia
	$segment_queries = tm_search_api_init_option($query_options, "segment"); // segment:marketing
	$has_queries = tm_search_api_init_option($query_options, "has"); // has:Snapchat
	$role_queries = tm_search_api_init_option($query_options, "role"); // role:CEO
	$at_queries = tm_search_api_init_option($query_options, "at"); // at:Skyscanner
	$flag_queries = tm_search_api_init_option($query_options, "flag"); // flag:sponsor

	// If no options enabled
	// then turn any filters off if all keywords are ignored by filter
	if ((sizeof($in_queries) == 0)
		&& (sizeof($segment_queries) == 0)
		&& (sizeof($has_queries) == 0)
		&& (sizeof($role_queries) == 0)
		&& (sizeof($at_queries) == 0)
		&& (sizeof($flag_queries) == 0)) {

		$filter_people = tm_search_api_check_filter_enabled($search_terms, $filter_people, "people");
		$filter_events = tm_search_api_check_filter_enabled($search_terms, $filter_events, "events");
		$filter_past_events = tm_search_api_check_filter_enabled($search_terms, $filter_past_events, "events");
		$filter_chapters = tm_search_api_check_filter_enabled($search_terms, $filter_chapters, "chapters");
		$filter_companies = tm_search_api_check_filter_enabled($search_terms, $filter_companies, "companies");
	}
	
	// Step 3. Initialise search api query
	$query = search_api_query('tm_search_multi_index');

	// Step 4. Create option filters
	$query_filter_in = $query->createFilter('OR'); // ie: in:Japan
	$query_filter_segment = $query->createFilter('OR'); // ie: segment:marketing
	$query_filter_has = $query->createFilter('OR'); // ie: has:snapchat
	$query_filter_role = $query->createFilter('OR'); // ie: role:founder
	$query_filter_at = $query->createFilter('OR'); // ie: at:expedia
	$query_filter_flag = $query->createFilter('OR'); // ie: flag:sponsor

	// Step 5. Check for cases where we won't have any results

	// Check if we don't have any filters enabled
	if (!$filter_people 
		&& !$filter_events 
		&& !$filter_past_events 
		&& !$filter_chapters 
		&& !$filter_companies) {

		// return empty results
		return tm_search_api_empty_results_array($search_query, $search_terms, $query_options);
	}

	// No search terms and no applicable option filters
	if ((sizeof($search_terms) == 0)
		&& (!tm_search_api_check_option_applicable("in", $in_queries, $filter_people, $filter_events, $filter_past_events, $filter_chapters, $filter_companies))
		&& (!tm_search_api_check_option_applicable("segment", $segment_queries, $filter_people, $filter_events, $filter_past_events, $filter_chapters, $filter_companies))
		&& (!tm_search_api_check_option_applicable("has", $has_queries, $filter_people, $filter_events, $filter_past_events, $filter_chapters, $filter_companies))
		&& (!tm_search_api_check_option_applicable("role", $role_queries, $filter_people, $filter_events, $filter_past_events, $filter_chapters, $filter_companies))
		&& (!tm_search_api_check_option_applicable("at", $at_queries, $filter_people, $filter_events, $filter_past_events, $filter_chapters, $filter_companies))
		&& (!tm_search_api_check_option_applicable("flag", $flag_queries, $filter_people, $filter_events, $filter_past_events, $filter_chapters, $filter_companies))) {

		// return empty results
		return tm_search_api_empty_results_array($search_query, $search_terms, $query_options);
	}

	// Step 6. Set up filters

	// PEOPLE FILTER
	if ($filter_people) {

		// Create base and keywor filters
		$query_filter_people_base = $query->createFilter('AND');
		$query_filter_people = $query->createFilter('OR');
		
		// User type
		$query_filter_people_base->condition('item_type', 'user');

		// Match first and last name
		// We try in this order:
		// Case 1. If single query is in quotes, try to match first and last name from word parts
		// Case 2. If query is two words, try to match first and last name
		// Case 3. Else, try first and last name on all keywords

		// Case 1. If single query and in quotes, split into two parts and try search_terms
		
		// Adjust if tm_search_api_pluralize_keywords is enabled
		$sizeof_terms = 1;
		if ($conf["tm_search_api_pluralize_keywords"]) {
			$sizeof_terms = 2;
		}
		if ((sizeof($search_terms) == $sizeof_terms) && (strpos($search_terms[0], " ") !== false)) {

			$parts = explode(" ", $search_terms[0]);
			$query_filter_people_name = $query->createFilter('AND');
			$query_filter_people_name->condition('user:field_user_first_name', $parts[0]);
			$query_filter_people_name->condition('user:field_user_last_name', $parts[1]);
			$query_filter_people_base->filter($query_filter_people_name);
	
			$query_filter_people->condition('user:field_user_first_name', $parts[0]);
			$query_filter_people->condition('user:field_user_last_name', $parts[1]);

			// also match username as it's created with first and last name
			$query_filter_people->condition('user:name', $search_terms[0] . $parts[1]);

		} else {

			// Adjust if tm_search_api_pluralize_keywords is enabled
			$sizeof_terms = 2;
			if ($conf["tm_search_api_pluralize_keywords"]) {
				$sizeof_terms = 4;
			}

			// Case 2. If only two search terms, attempt first and last name match
			if (sizeof($search_terms) == $sizeof_terms) {
				$query_filter_people->condition('user:field_user_first_name', $search_terms[0]);
				$query_filter_people->condition('user:field_user_last_name', $search_terms[1]);

				// also match username as it's created with first and last name
				$query_filter_people->condition('user:name', $search_terms[0] . $search_terms[1]);

			} else {

				//Case 3. Try first and last name on all keywords
				foreach ($search_terms as $search_term) {
					$query_filter_people->condition('user:field_user_first_name', $search_term);
					$query_filter_people->condition('user:field_user_last_name', $search_term);
				}
			}

		}

		// Other user fields
		foreach ($search_terms as $search_term) {

			// ignore specific keywords
			if (tm_search_api_check_is_ignored_keyword($search_term, "people")) { continue; }

			$query_filter_people->condition('user:field_about_me', $search_term);

			$query_filter_people->condition('user:field_location_city', $search_term);
			$query_filter_people->condition('user:field_user_country:name', $search_term);
			$query_filter_people->condition('user:field_user_country:official_name', $search_term);

			$query_filter_people->condition('user:field_job_organization', $search_term);
			$query_filter_people->condition('user:field_job_role', $search_term);

			$query_filter_people->condition('user:field_segment:name', $search_term);
			$query_filter_people->condition('user:field_segment:parent:name', $search_term);

			$query_filter_people->condition('user:field_link_website:url', $search_term);
			$query_filter_people->condition('user:field_link_snapchat:url', $search_term);
			$query_filter_people->condition('user:field_link_twitter:url', $search_term);
			$query_filter_people->condition('user:field_link_instagram:url', $search_term);
			$query_filter_people->condition('user:field_link_facebook:url', $search_term);
			$query_filter_people->condition('user:field_link_linkedin:url', $search_term);
			$query_filter_people->condition('user:field_link_youtube:url', $search_term);
			$query_filter_people->condition('user:field_link_vimeo:url', $search_term);

			$query_filter_people->condition('user:field_user_question_1', $search_term);
			$query_filter_people->condition('user:field_user_question_2', $search_term);
			$query_filter_people->condition('user:field_user_question_3', $search_term);
			$query_filter_people->condition('user:field_user_question_4', $search_term);
			$query_filter_people->condition('user:field_user_question_5', $search_term);

			// let approved members search by email address
			if (user_is_logged_in()) {
				if (in_array("approved user", $user->roles)) {
					$query_filter_people->condition('user:mail', $search_term);
				}
			}

			// add keyword filters to base filter
			$query_filter_people_base->filter($query_filter_people);
		}

		// in:field
		foreach ($in_queries as $in_query) {

			// ignore specific keywords
			if (tm_search_api_check_is_ignored_keyword($in_query, "people")) { continue; }

			$query_filter_in->condition('user:field_location_city', $in_query);
			$query_filter_in->condition('user:field_user_country:name', $in_query);
			$query_filter_in->condition('user:field_user_country:official_name', $in_query);
			$query_filter_in->condition('user:field_user_country:continent_name', $in_query);
		}

		// segment:field
		foreach ($segment_queries as $segment_query) {

			// ignore specific keywords
			if (tm_search_api_check_is_ignored_keyword($segment_query, "people")) { continue; }

			$query_filter_segment->condition('user:field_segment:name', $segment_query);
			$query_filter_segment->condition('user:field_segment:parent:name', $segment_query);
		}

		// has:field
		if (in_array("website", $has_queries)) {
			$query_filter_has->condition('user:field_link_website:url', null, '<>');
		}
		if (in_array("snapchat", $has_queries)) {
			$query_filter_has->condition('user:field_link_snapchat:url', null, '<>');
		}
		if (in_array("twitter", $has_queries)) {
			$query_filter_has->condition('user:field_link_twitter:url', null, '<>');
		}
		if (in_array("instagram", $has_queries)) {
			$query_filter_has->condition('user:field_link_instagram:url', null, '<>');
		}
		if (in_array("facebook", $has_queries)) {
			$query_filter_has->condition('user:field_link_facebook:url', null, '<>');
		}
		if (in_array("linkedin", $has_queries)) {
			$query_filter_has->condition('user:field_link_linkedin:url', null, '<>');
		}
		if (in_array("youtube", $has_queries)) {
			$query_filter_has->condition('user:field_link_youtube:url', null, '<>');
		}
		if (in_array("vimeo", $has_queries)) {
			$query_filter_has->condition('user:field_link_vimeo:url', null, '<>');
		}

		// role:field
		foreach ($role_queries as $role_query) {

			// ignore specific keywords
			if (tm_search_api_check_is_ignored_keyword($role_query, "people")) { continue; }

			$query_filter_role->condition('user:field_job_role', $role_query);
		}

		// at:field
		foreach ($at_queries as $at_query) {

			// ignore specific keywords
			if (tm_search_api_check_is_ignored_keyword($at_query, "people")) { continue; }

			$query_filter_at->condition('user:field_job_organization', $at_query);
		}
		
	}

	// EVENTS FILTER
	// Note: Won't filter by date without a keyword. ie: "in:Sydney" returns all results
	if ($filter_events || $filter_past_events) {

		// Create base and keyword filters
		$query_filter_events_base = $query->createFilter('AND');
		$query_filter_events = $query->createFilter('OR');

		// Event type
		$query_filter_events_base->condition('node:type', 'event');

		foreach ($search_terms as $search_term) {

			// ignore specific keywords
			if (tm_search_api_check_is_ignored_keyword($search_term, "events")) { continue; }

			$query_filter_events->condition('node:title', $search_term);
			$query_filter_events->condition('node:body:value', $search_term);
			$query_filter_events->condition('node:field_location', $search_term);
			$query_filter_events->condition('node:field_chapter:title', $search_term);
		}

		// Only search future events unless filter_past_events enabled
		$query_filter_events_date = $query->createFilter('OR');

		if ($filter_events) {
			$yesterday = new DateTime('yesterday');
			$query_filter_events_date->condition('node:field_event_date:value', $yesterday->getTimeStamp(), '>=');

		}
		if ($filter_past_events) {
			$today = new DateTime('today');
			$query_filter_events_date->condition('node:field_event_date:value', $today->getTimestamp(), '<=');
		}

		// add keyword filters to base filter
		$query_filter_events_base->filter($query_filter_events);
		$query_filter_events_base->filter($query_filter_events_date);

	}

	// CHAPTERS FILTER
	if ($filter_chapters) {

		// Create base and keywor filters
		$query_filter_chapters_base = $query->createFilter('AND');
		$query_filter_chapters = $query->createFilter('OR');

		// Chapter type
		$query_filter_chapters_base->condition('node:type', 'chapter');

		foreach ($search_terms as $search_term) {

			// ignore specific keywords
			if (tm_search_api_check_is_ignored_keyword($search_term, "chapters")) { continue; }

			$query_filter_chapters->condition('node:title', $search_term);
			$query_filter_chapters->condition('node:body:value', $search_term);

			$query_filter_chapters->condition('node:field_country:name', $search_term);
			$query_filter_chapters->condition('node:field_country:official_name', $search_term);
		}

		// add keyword filters to base filter
		$query_filter_chapters_base->filter($query_filter_chapters);

		// in:field
		foreach ($in_queries as $in_query) {

			// ignore specific keywords
			if (tm_search_api_check_is_ignored_keyword($in_query, "chapters")) { continue; }

			$query_filter_in->condition('node:field_country:name', $in_query);
			$query_filter_in->condition('node:field_country:official_name', $in_query);
			$query_filter_in->condition('node:field_country:continent_name', $in_query);
		}
	}

	// COMPANIES FILTER
	if ($filter_companies) {

		// Create base and keyword filters
		$query_filter_companies_base = $query->createFilter('AND');
		$query_filter_companies = $query->createFilter('OR');

		// Organization type
		$query_filter_companies_base->condition('node:type', 'organization');

		foreach ($search_terms as $search_term) {

			// ignore specific keywords
			if (tm_search_api_check_is_ignored_keyword($search_term, "companies")) { continue; }

			$query_filter_companies->condition('node:title', $search_term);
			$query_filter_companies->condition('node:body:value', $search_term);

			$query_filter_companies->condition('node:field_segment:name', $search_term);
			$query_filter_companies->condition('node:field_segment:parent:name', $search_term);

			$query_filter_companies->condition('node:field_location_city', $search_term);
			$query_filter_companies->condition('node:field_country:name', $search_term);
			$query_filter_companies->condition('node:field_country:official_name', $search_term);

			$query_filter_companies->condition('node:field_link_website:url', $search_term);
			$query_filter_companies->condition('node:field_link_snapchat:url', $search_term);
			$query_filter_companies->condition('node:field_link_twitter:url', $search_term);
			$query_filter_companies->condition('node:field_link_instagram:url', $search_term);
			$query_filter_companies->condition('node:field_link_facebook:url', $search_term);
			$query_filter_companies->condition('node:field_link_linkedin:url', $search_term);
			$query_filter_companies->condition('node:field_link_youtube:url', $search_term);
			$query_filter_companies->condition('node:field_link_vimeo:url', $search_term);
		}

		// in:field
		foreach ($in_queries as $in_query) {

			// ignore specific keywords
			if (tm_search_api_check_is_ignored_keyword($in_query, "companies")) { continue; }

			$query_filter_in->condition('node:field_country:name', $in_query);
			$query_filter_in->condition('node:field_country:official_name', $in_query);
			$query_filter_in->condition('node:field_country:continent_name', $in_query);
		}

		// segment:field
		foreach ($segment_queries as $segment_query) {

			// ignore specific keywords
			if (tm_search_api_check_is_ignored_keyword($segment_query, "companies")) { continue; }

			$query_filter_segment->condition('node:field_segment:name', $segment_query);
			$query_filter_segment->condition('node:field_segment:parent:name', $segment_query);
		}

		// has:field
		if (in_array("website", $has_queries)) {
			$query_filter_has->condition('node:field_link_website:url', null, '<>');
		}
		if (in_array("snapchat", $has_queries)) {
			$query_filter_has->condition('node:field_link_snapchat:url', null, '<>');
		}
		if (in_array("twitter", $has_queries)) {
			$query_filter_has->condition('node:field_link_twitter:url', null, '<>');
		}
		if (in_array("instagram", $has_queries)) {
			$query_filter_has->condition('node:field_link_instagram:url', null, '<>');
		}
		if (in_array("facebook", $has_queries)) {
			$query_filter_has->condition('node:field_link_facebook:url', null, '<>');
		}
		if (in_array("linkedin", $has_queries)) {
			$query_filter_has->condition('node:field_link_linkedin:url', null, '<>');
		}
		if (in_array("youtube", $has_queries)) {
			$query_filter_has->condition('node:field_link_youtube:url', null, '<>');
		}
		if (in_array("vimeo", $has_queries)) {
			$query_filter_has->condition('node:field_link_vimeo:url', null, '<>');
		}

		// flag:sponsor
		if (in_array("sponsor", $flag_queries)) {
			$query_filter_flag->condition('node:flag_tm_sponsor', true);
		}

		// add keyword filters to base filter
		$query_filter_companies_base->filter($query_filter_companies);
	}

	// Step 7. Attach filters
	$search_filter = $query->createFilter('AND'); // main search filter
	
    // keyword filters
    if (sizeof($search_terms) > 0) {
		$keyword_filter = $query->createFilter('OR'); // keyword search, ie: Sydney
		if ($filter_people) {
			$keyword_filter->filter($query_filter_people_base);
		}
		if ($filter_events || $filter_past_events) {
			$keyword_filter->filter($query_filter_events_base);
		}
		if ($filter_chapters) {	
			$keyword_filter->filter($query_filter_chapters_base);
		}
		if ($filter_companies) {
			$keyword_filter->filter($query_filter_companies_base);
		}
	}

    // option filters
    $options_filter = $query->createFilter('AND'); // option search, ie: in:Thailand
    $options_filter_types = $query->createFilter('OR');

    // set types on options filter
	if ($filter_people) {
		$options_filter_types->condition('item_type', 'user');
	}
	if ($filter_events || $filter_past_events) {
		$options_filter_types->condition('node:type', 'event');
	}
	if ($filter_chapters) {	
		$options_filter_types->condition('node:type', 'chapter');
	}
	if ($filter_companies) {
		$options_filter_types->condition('node:type', 'organization');
	}
	$options_filter->filter($options_filter_types);

	// add different option
	if (sizeof($in_queries) > 0) {
		$options_filter->filter($query_filter_in);
	}
	if (sizeof($segment_queries) > 0) {
		$options_filter->filter($query_filter_segment);
	}
	if (sizeof($has_queries) > 0) {
		$options_filter->filter($query_filter_has);
	}
	if (sizeof($role_queries) > 0) {
		$options_filter->filter($query_filter_role);
	}
	if (sizeof($at_queries) > 0) {
		$options_filter->filter($query_filter_at);
	}
	if (sizeof($flag_queries) > 0) {
		$options_filter->filter($query_filter_flag);
	}

	// attach type, keyword, option filters to main search filter
	if (sizeof($search_terms) > 0) {
		$search_filter->filter($keyword_filter);
	}
	$search_filter->filter($options_filter);
	$query->filter($search_filter);

	// Step 8. Set Range
	$query->range($offset, $num_results_per_page);

	// Step 9. Run query
	$results = $query->execute();

	// Step 10. Add extra meta data

	// Check last page
	$results['last page'] = "false";
	if ($offset + $num_results_per_page >= $results['result count']) { 
		$results['last page'] = "true";
	}

	// Debugging information
	$results['tm_debug_search_raw_query'] = $query_array["search_query"];
	$results['tm_debug_search_keywords'] = $search_terms;
	$results['tm_debug_search_query_no_options'] = $search_query_no_options;
	$results['tm_debug_search_options'] = $query_options;
	$results['tm_debug_results_cached'] = false;

	// Step 11. Cache result
	if ($conf['tm_search_cache_results_seconds'] > 0) {
		cache_set($cache_key, $results, 'cache', time() + $conf['tm_search_cache_results_seconds']);
	}

	// Step 12. Return result set
	return $results;
}

/* Check if a keyword for a filter type should be ignored */
function tm_search_api_check_is_ignored_keyword($keyword, $filter_type) {

	global $conf;

	$ignore_keywords = array();
	if ($filter_type == "people") {
		$ignore_keywords = $conf["tm_search_api_ignore_keywords_people"];
	}
	if ($filter_type == "events") {
		$ignore_keywords = $conf["tm_search_api_ignore_keywords_events"];
	}
	if ($filter_type == "chapters") {
		$ignore_keywords = $conf["tm_search_api_ignore_keywords_chapters"];
	}
	if ($filter_type == "companies") {
		$ignore_keywords = $conf["tm_search_api_ignore_keywords_companies"];
	}

	// check keyword against ignored keywords
	if (in_array(strtolower($keyword), $ignore_keywords)) {
		return true;
	}

	// if pluralize keywords, perform check against depluralized value
	if ($conf["tm_search_api_pluralize_keywords"]) {
		if (in_array(strtolower(rtrim($keyword, "s")), $ignore_keywords)) {
			return true;
		}
	}

	// not found
	return false;
}

/* Check if a filter should be enabled based on ignored keywords
   Return true if filter enabled and not all terms are ignored
   Return false if all search terms for filter are ignored
   Return false if filter disabled
   */
function tm_search_api_check_filter_enabled($search_terms, $filter_enabled, $filter_type) {

	// if filter enabled
	if ($filter_enabled) {

		$filter_status = false;
		foreach($search_terms as $search_term) {
			if (!tm_search_api_check_is_ignored_keyword($search_term, $filter_type)) {
				$filter_status = true;
			}
			if ($filter_status) {
				return true;
			}
		}
	}

	// filter is not enabled or all words are ignored
	return false;
}

/* Return empty results array */
function tm_search_api_empty_results_array($search_query, $search_terms, $query_options) {
	$results = array();
	$results['results'] = array();
	$results['last page'] = true;
	$results['results count'] = 0;
	$results['performance'] = array();
	$results['performance']['complete'] = 0;
	$results['tm_debug_search_raw_query'] = $search_query;
	$results['tm_debug_search_keywords'] = $search_terms;
	$results['tm_debug_search_options'] = $query_options;
	$results['tm_debug_search_query_no_options'] = $search_query;

	return $results;
}

/* Helper method to init query options array */
function tm_search_api_init_option($query_options, $key) {

	if (isset($query_options[$key])) {

		// has:
		if ($key == "has") {
			if (array_search("website", $query_options[$key]) !== false) { return $query_options[$key]; }
			if (array_search("snapchat", $query_options[$key]) !== false) { return $query_options[$key]; }
			if (array_search("twitter", $query_options[$key]) !== false) { return $query_options[$key]; }
			if (array_search("instagram", $query_options[$key]) !== false) { return $query_options[$key]; }
			if (array_search("facebook", $query_options[$key]) !== false) { return $query_options[$key]; }
			if (array_search("linkedin", $query_options[$key]) !== false) { return $query_options[$key]; }
			if (array_search("youtube", $query_options[$key]) !== false) { return $query_options[$key]; }
			if (array_search("vimeo", $query_options[$key]) !== false) { return $query_options[$key]; }

			// not supported
			return array();
		}

		// flag:
		if ($key == "flag") {
			if (array_search("sponsor", $query_options[$key]) !== false) { return $query_options[$key]; }

			// not supported
			return array();
		}

		// other options, return true
		return $query_options[$key];

	}

	// not supported
	return array();
}

/* Helper method to check if option filter is applicable to any enabled filters */
function tm_search_api_check_option_applicable($option_name, $option_enabled, $filter_people, $filter_events, $filter_past_events, $filter_chapters, $filter_companies) {

	// not applicable if disabled
	if (!$option_enabled) { return false; }

	// filters that support in:
	if ($option_name == "in") {
		if ($filter_people) { return true; }
		if ($filter_chapters) { return true; }
		if ($filter_companies) { return true; }
	}

	// filters that support segment:
	if ($option_name == "segment") {
		if ($filter_people) { return true; }
		if ($filter_companies) { return true; }
	}

	// filters that support has:
	if ($option_name == "has") {
		if ($filter_people) { return true; }
		if ($filter_chapters) { return true; }
		if ($filter_companies) { return true; }
	}

	// filters that support role:
	if ($option_name == "role") {
		if ($filter_people) { return true; }
	}

	// filters that support at:
	if ($option_name == "at") {
		if ($filter_people) { return true; }
	}

	// filters that support flag:
	if ($option_name == "flag") {
		if ($filter_companies) { return true; }
	}

	// option filter is not applicable to any enabled filter
	return false;
}
