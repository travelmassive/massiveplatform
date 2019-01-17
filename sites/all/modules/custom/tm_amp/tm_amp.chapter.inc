<?php

// tm_amp.chapter.inc

/**
 * Render chapter
 */
function tm_amp_render_chapter($node_id) {

	global $conf;

	// Load node
	$node = node_load($node_id);
	if ($node == null) {
		return "Oops, an error occured (invalid id). <a href='/'>Return home</a>.";
	}

	// Note: We use https://www.ampstart.com/templates#news-blog template
	$template_file = drupal_get_path('module', 'tm_amp') . '/templates/tm_amp-template.tpl.php';
	$template = file_get_contents($template_file);

	// Replace global template vars
	$html = tm_amp_insert_template_vars($template);

	// Canonical URL
	$canonical_url = $conf["tm_email_server_http"] . "://" . $_SERVER["HTTP_HOST"] . "/" . drupal_get_path_alias("node/" . $node_id);
	$html = str_replace("__CANONICAL_URL__", $canonical_url, $html);

	// Title
	$title = strip_tags($node->title);

	// Replace variables
	$pre_heading = "<a href='/user/login?destination=" . strip_tags(drupal_get_path_alias("node/" . $node_id)) . "'>Log in</a> or <a href='/user/register'>Sign Up</a>"; // "&raquo; <a href='/chapters'>Chapters</a>"
	$html = str_replace("__PAGE_TITLE__", $title . " | " . $conf["tm_site_name"], $html);
	$html = str_replace("__PAGE_PRE_HEADING__", $pre_heading, $html);
	$html = str_replace("__PAGE_HEADING__", tm_chapters_formal_name($node), $html);

	$num_members = _tm_chapters_get_num_members($node_id);
	$html = str_replace("__PAGE_SUB_HEADING__", $num_members . " Members", $html);

	// Set chapter image
	if (isset($node->field_image[LANGUAGE_NONE][0]['uri'])) {
		$image_url = image_style_url("banner", $node->field_image[LANGUAGE_NONE][0]['uri']);
	} else {
		$default_image_uri = _tm_chapters_get_default_image($conf["tm_images_default_field_image"], $node->nid);
		$image_url = image_style_url("banner", $default_image_uri);
	}

	$img_url = file_create_url($image_url);

	// image
	$html = str_replace("__PAGE_IMAGE_URL__", "/" . drupal_get_path_alias("node/" . $node_id), $html); 
	$html = str_replace("__PAGE_IMAGE__", $img_url, $html);
	$html = str_replace("__PAGE_IMAGE_WIDTH__", 454, $html);
	$html = str_replace("__PAGE_IMAGE_HEIGHT__", 256, $html);
	$html = str_replace("__PAGE_IMAGE_LAYOUT__", 'layout="responsive"', $html);
	$html = str_replace("__PAGE_IMAGE_ALT__", tm_amp_escape_string($title), $html);

	// description
	$description = "";
	$description_linked = "";
	if (isset($node->body[LANGUAGE_NONE][0]['value'])) {
		$description = strip_tags($node->body[LANGUAGE_NONE][0]['value']);
		$description = str_replace("\n", "<br>", $description); // add line breaks
		$description = preg_replace("/[-]{4,10}/", "", $description); // replace ----
		$description_linked = tm_base_linkify($description, ["http", "https", "mail", "twitter"], ["rel" => "nofollow"]); // add links
	}

	$html = str_replace("__PAGE_DESCRIPTION__", $description_linked, $html);
	
	// Country
	$chapter_is_group = false;
	if (isset($node->field_chapter_is_group[LANGUAGE_NONE][0]['value'])) {
		$chapter_is_group = $node->field_chapter_is_group[LANGUAGE_NONE][0]['value'];
	}
	if (!$chapter_is_group) {
		if (isset($node->field_country[LANGUAGE_NONE][0]['iso2'])) {
			$country = country_load($node->field_country[LANGUAGE_NONE][0]['iso2']);
			$country_name = $country->name;
		}
	}

	// extra
	$extra_info = '<section class="px3 mb4">
  <h2 class="mb2"></h2>
  <span class="ampstart-hint block mb3">Chapter details</span>
  <ul class="mb4">';

  	if ($country_name != "") {
  		$extra_info .= "<li>" . $country_name . "</li>";
  	}
  	
  	$num_members = _tm_chapters_get_num_members($node_id);
	if ($num_members > 0) {
		$extra_info .= '<li><a href="/node/' . $node_id . '/members">' . $num_members . ' Member' . tm_amp_pluralize($num_members) . '</a></li>';
	}

	// chapter leaders
	// Show chapter leader names
	$leader_uids = array();
	if (isset($node->field_chapter_leaders[LANGUAGE_NONE])) {
		foreach ($node->field_chapter_leaders[LANGUAGE_NONE] as $chapter_leader) {
			$leader_uids[] = $chapter_leader['target_id'];
		}
	}
	$chapter_leaders_names = array();
	foreach ($leader_uids as $leader_uid) {
		$chapter_leader = user_load($leader_uid);
		$chapter_leader_url = $conf["tm_email_server_http"] . "://" . $_SERVER["HTTP_HOST"] . "/" . drupal_get_path_alias("user/" . $leader_uid) . "?amp";
		$chapter_leaders_names[] = '<a href="' . $chapter_leader_url . '">' . strip_tags($chapter_leader->field_user_first_name[LANGUAGE_NONE][0]['value'] . " " . $chapter_leader->field_user_last_name[LANGUAGE_NONE][0]['value']) . '</a>'; 
	}
	if (sizeof($chapter_leaders_names) > 0) {
		$last = array_pop($chapter_leaders_names);
		$chapter_leader_links = (count($chapter_leaders_names) ? implode(", ", $chapter_leaders_names) . " and " . $last : $last);
		$extra_info .= "<li>Lead by " . $chapter_leader_links . "</li>";
	}

	// chapter stats
	$extra_info .= _tm_amp_chapter_stats($node_id);

	$extra_info .= '</ul>';
	$html = str_replace("__PAGE_EXTRA__", $extra_info, $html);

	// Insert meta data
	$meta_data = tm_amp_render_chapter_meta($node, $canonical_url, $description, $img_url);
	$html = str_replace("__META_DATA__", $meta_data, $html);

	// Youtube
	$youtube_js = "";
	$youtube_html = "";
	if (isset($node->field_chapter_cover_video[LANGUAGE_NONE][0]['value'])) {

		$cover_video_value = $node->field_chapter_cover_video[LANGUAGE_NONE][0]['value'];
		$youtube_id = tm_amp_get_youtube_id($cover_video_value);
	
		if ($youtube_id != "") {

			$youtube_js = '<script async custom-element="amp-youtube" src="https://cdn.ampproject.org/v0/amp-youtube-0.1.js"></script>';
			$youtube_html = '<p class="mb4 px3"><amp-youtube data-videoid="' . $youtube_id . '" layout="responsive" width="480" height="270"></amp-youtube></p>';
		}
	}

	$html = str_replace("__CUSTOM_SCRIPTS__", $youtube_js, $html);
	$html = str_replace("__YOUTUBE_HTML__", $youtube_html, $html);

    /*
    if (isset($node->field_chapter_cover_video[LANGUAGE_NONE][0]['value'])) {
    	$cover_video_value = $node->field_chapter_cover_video[LANGUAGE_NONE][0]['value'];
    }*/

	// return html
	return $html;
}

/**
 * Render chapter's highlight stats
 */
function _tm_amp_chapter_stats($node_id) {

	// rely on the permanent cache stored by _tm_reports_get_chapter_insights
	// run drush tm-chapters-update-display-stats-cache to update
	$chapter_ids_key = md5($node_id); 
	$cache = cache_get('tm-reports-get-chapter-insights-display-' . $chapter_ids_key, 'cache');
	if (empty($cache)) {
		return;
	}

	global $conf;

	// fetch report data from cache
	// tm-reports-get-chapter-insights-display-NID
	$report_data = $cache->data;

	$connections_text = "Connections";
	$new_members_text = "New Member";
	$upcoming_events_text = "New Event";
	$past_events_text = "Past Event";

	$num_connections = $report_data["data_values"]["total_chapter_member_connections"];
	$num_new_members = $report_data["data_values"]["members_30_days_total"];
	$num_events_upcoming = $report_data["data_values"]["num_events_upcoming"];

	$num_past_events = $report_data["data_values"]["num_events_past_total"];

	$html = "";

	// Make sure we have stats to display
	if (($num_new_members == 0) and ($num_events_upcoming == 0) and ($num_past_events == 0)) {
		return; 
	}

	if ($conf["tm_chapters_display_stats_connections"]) {
		if ($num_connections > 1) {
			$html .= "<li>" . tm_base_stats_format($num_connections) . " " . $connections_text . " </li>";
		}
	}

	/*if ($num_new_members > 0) {
		if ($num_new_members > 1) { $new_members_text = "New Members"; }
		$html .= "<li><a href='/node/" . $node_id . "/members'>" . $num_new_members . " " . $new_members_text . " </a></li>";
	}*/

	if ($num_events_upcoming > 0) {
		if ($num_events_upcoming > 1) { $upcoming_events_text = "New Events"; }
		$html .= "<li><a href='/" . drupal_get_path_alias("node/" . $node_id) . "'>" . $num_events_upcoming . " " . $upcoming_events_text . "</a></li>";
	} elseif ($num_past_events > 0) {
		if ($num_past_events > 1) { 
			$past_events_text = "Past Events";
		}
		$html .= "<li><a href='/node/" . $node_id . "/events'>" . $num_past_events . " " . $past_events_text . "</a></li>";
	}

	return $html;
}

/**
 * Render meta data
 */
function tm_amp_render_chapter_meta($node, $canonical_url, $description, $image_url) {

	$meta = '<script type="application/ld+json">
  {
    "@context": "http://schema.org",
    "@type": "Organization",
    "mainEntityOfPage": "__CANONICAL_URL__",
    "url": "__CANONICAL_URL__",
    "name": "__NAME__",
    "description": "__DESCRIPTION__",
    "logo": {
      "@type": "ImageObject",
      "url": "__IMAGE_URL__",
      "height": __IMAGE_HEIGHT__,
      "width": __IMAGE_WIDTH__
    }
  }
</script>';

	$meta = str_replace("__CANONICAL_URL__", $canonical_url, $meta);
	$meta = str_replace("__NAME__", tm_amp_escape_string(tm_chapters_formal_name($node)), $meta);
	$description = tm_amp_escape_string(str_replace("<br>", "\n", $description));
	$meta = str_replace("__DESCRIPTION__", tm_amp_escape_string($description), $meta);

	// image
	$meta = str_replace("__IMAGE_URL__", $image_url, $meta);
	$meta = str_replace("__IMAGE_HEIGHT__", 906, $meta);
	$meta = str_replace("__IMAGE_WIDTH__", 1610, $meta);

	return $meta;

}
