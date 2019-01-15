<?php

// tm_amp.event.inc

/**
 * Render event
 */
function tm_amp_render_event($node_id) {

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
	$html = str_replace("__CANONICAL_URL__", $conf["tm_email_server_http"] . "://" . $_SERVER["HTTP_HOST"] . "/" . drupal_get_path_alias("node/" . $node_id), $html);

	// Title
	$title = strip_tags($node->title);

	// Replace variables
	$pre_heading = "<a href='/user/login?destination=" . strip_tags(drupal_get_path_alias("node/" . $node_id)) . "'>Log in</a> or <a href='/user/register'>Sign Up</a>"; // "&raquo; <a href='/events'>Events</a>"
	$html = str_replace("__PAGE_TITLE__", $title . " | " . $conf["tm_site_name"], $html);
	$html = str_replace("__PAGE_PRE_HEADING__", $pre_heading, $html);
	$html = str_replace("__PAGE_HEADING__", $title, $html);

	// Event date
	$view = field_view_field('node',$node,'field_event_date',
	array('label'=>'hidden',));
	$event_date = render($view);

	$html = str_replace("__PAGE_SUB_HEADING__", $event_date, $html);

	// Show preview of event image
	if (isset($node->field_image[LANGUAGE_NONE][0]['uri'])) {
		$image_url = image_style_url("banner", $node->field_image[LANGUAGE_NONE][0]['uri']);
	} else {
		$default_image_uri = _tm_events_get_default_image($conf["tm_images_default_cover_event"], $node->nid);
		$image_url = image_style_url("banner", $default_image_uri);
	}

	$img_url = $image_url; //file_create_url($img_uri);

	// image
	$html = str_replace("__PAGE_IMAGE_URL__", "/" . drupal_get_path_alias("node/" . $node_id), $html); 
	$html = str_replace("__PAGE_IMAGE__", $img_url, $html);
	$html = str_replace("__PAGE_IMAGE_WIDTH__", 454, $html);
	$html = str_replace("__PAGE_IMAGE_HEIGHT__", 256, $html);
	$html = str_replace("__PAGE_IMAGE_LAYOUT", 'layout="responsive"', $html);
	$html = str_replace("__PAGE_IMAGE_ALT__", $title, $html);

	// description
	$description = "";
	if (isset($node->body[LANGUAGE_NONE][0]['value'])) {
		$description = strip_tags($node->body[LANGUAGE_NONE][0]['value']);
		$description = str_replace("\n", "<br>", $description); // add line breaks
		$description = preg_replace("/[-]{4,10}/", "<br>", $description); // replace ---- with hr
		$description = tm_base_linkify($description, ["http", "https", "mail", "twitter"], ["rel" => "nofollow", "target" => "_blank"]); // add links
	}

	$html = str_replace("__PAGE_DESCRIPTION__", $description, $html);

	// extra
	$num_members = _tm_chapters_get_num_members($node_id);

	$extra_info = '<section class="px3 mb4">
  <h2 class="mb2"></h2>
  <span class="ampstart-hint block mb3">Event details</span>
  <ul class="mb4">';

	if ($num_members > 0) {
		$extra_info .= '<li><a href="/node/' . $node_id . '/members">' . $num_members . ' member' . tm_amp_pluralize($num_members) . '</a></li>';
	}

	$extra_info .= "<li>" . t('When').': '.strip_tags($event_date) . "</li>";

	// location (can be hidden)
	$location_meta = "";
	$location_text = "";
	if (isset($node->field_event_hide_location[LANGUAGE_NONE][0]['value'])) {
		if ($node->field_event_hide_location[LANGUAGE_NONE][0]['value'] != 1) {

			if ($node->field_event_venue_name != null) {
				$extra_info .= "<li>" . t('Venue').': '.$node->field_event_venue_name[LANGUAGE_NONE][0]['value'];
				$location_text = strip_tags($node->field_event_venue_name[LANGUAGE_NONE][0]['value']);
				$location_meta = $location_text;
			}
			if ($node->field_location != null) {
				$location_text = $node->field_location[LANGUAGE_NONE][0]['value'];
				$location_text = str_replace("\r", ", ", $location_text);
				$extra_info .= "<li>" . t('Location').': '. $location_text . "</li>";
				
				if ($location_meta != "") {
					$location_meta .= ", ";
				}
				$location_meta .= $location_text;

			}
		} else {
			$extra_info .= "<li>Only attendees can view the location</li>";
		}
	}

	// List sponsors
	if (isset($node->field_event_sponsors[LANGUAGE_NONE][0])) {
		$sponsor_links = array();
		foreach ($event->field_event_sponsors[LANGUAGE_NONE] as $arr) {
			$sponsor = node_load($arr['target_id']);
			$sponsor_profile_url = drupal_get_path_alias("node/" . $arr['target_id']);
			$external_url = $conf["tm_email_server_http"] . "://" . $_SERVER["SERVER_NAME"] . "/" . $sponsor_profile_url;
			$sponsor_links[] = l(t($sponsor->title), $external_url);
		}
		$extra_info .= "<li>Sponsors: " . implode(", ", $sponsor_links) . "</li>";
	}


	// Register
	// time since start and end of event
	$event_date = field_get_items('node',$node,'field_event_date');
	$current_date = new DateTime('now', new DateTimeZone('UTC'));
	$event_timezone = _tm_get_event_timezone($node);
	$event_datetime_start = new DateTime($event_date[0]['value'], new DateTimeZone($event_timezone));
	$event_datetime_end = new DateTime($event_date[0]['value2'], new DateTimeZone($event_timezone));
	$seconds_since_event_start = $current_date->format('U') - $event_datetime_start->format('U');
	$seconds_since_event_end = $current_date->format('U') - $event_datetime_end->format('U');

	if ($seconds_since_event_end < 0) {
		$extra_info .= "<li><a href='/" . drupal_get_path_alias("node/" . $node_id) . "'>Register for event</li>";
	} else {
		$extra_info .= "<li>Past event</li>";
	}

	$extra_info .= '</ul>';
	$html = str_replace("__PAGE_EXTRA__", $extra_info, $html);

	// Insert meta data
	$meta_data = tm_amp_render_event_meta($node, $canonical_url, $description, $location_text, $img_url);
	$html = str_replace("__META_DATA__", $meta_data, $html);

	// Youtube
	$youtube_js = "";
	$youtube_html = "";
	if (isset($node->field_event_cover_video[LANGUAGE_NONE][0]['value'])) {

		$cover_video_value = $node->field_event_cover_video[LANGUAGE_NONE][0]['value'];
		$youtube_id = tm_amp_get_youtube_id($cover_video_value);
	
		if ($youtube_id != "") {

			$youtube_js = '<script async custom-element="amp-youtube" src="https://cdn.ampproject.org/v0/amp-youtube-0.1.js"></script>';
			$youtube_html = '<p class="mb4 px3"><amp-youtube data-videoid="' . $youtube_id . '" layout="responsive" width="480" height="270"></amp-youtube></p>';
		}
	}
	
	$html = str_replace("__CUSTOM_SCRIPTS__", $youtube_js, $html);
	$html = str_replace("__YOUTUBE_HTML__", $youtube_html, $html);

	// return html
	return $html;
}

/**
 * Render meta data
 */
function tm_amp_render_event_meta($node, $canonical_url, $description, $location_text, $image_url) {

	$meta = '<script type="application/ld+json">
  {
    "@context": "http://schema.org",
    "@type": "Event",
    "mainEntityOfPage": "__CANONICAL_URL__",
    "name": "__NAME__",
    "startDate": "__START_DATE__",__END_DATE__
    "description": "__DESCRIPTION__",
    "location": "__LOCATION__",
    "image": {
      "@type": "ImageObject",
      "url": "__IMAGE_URL__",
      "height": __IMAGE_HEIGHT__,
      "width": __IMAGE_WIDTH__
    }
  }
</script>';

	$meta = str_replace("__CANONICAL_URL__", $canonical_url, $meta);
	$meta = str_replace("__NAME__", str_replace('"', "", tm_chapters_formal_name($node)), $meta);
	$description = str_replace("<br>", "\n", $description);
	$meta = str_replace("__DESCRIPTION__", str_replace('"', "", $description), $meta);
	$location_text = str_replace("  ", " ", str_replace('"', "", str_replace("\n", " ", $location_text))); // quick clean up
	$meta = str_replace("__LOCATION__", $location_text, $meta);

	// start date
	$event_date = field_get_items('node',$node,'field_event_date');
	$current_date = new DateTime('now', new DateTimeZone('UTC'));
	$event_timezone = _tm_get_event_timezone($node);
	$event_datetime_start = new DateTime($event_date[0]['value'], new DateTimeZone($event_timezone));
	$start_date = $event_datetime_start->format(DateTime::ATOM); 
	$meta = str_replace("__START_DATE__", $start_date, $meta);

	// end date
	$end_date = "";
	$event_datetime_end = new DateTime($event_date[0]['value2'], new DateTimeZone($event_timezone));
	if ($event_datetime_end > $event_datetime_start) {
		$end_date_text = $event_datetime_end->format(DateTime::ATOM); 
		$end_date = "\n    " . '"endDate": "' . $end_date_text . '",';
	}
	$meta = str_replace("__END_DATE__", $end_date, $meta);

	// image
	$meta = str_replace("__IMAGE_URL__", $image_url, $meta);
	$meta = str_replace("__IMAGE_HEIGHT__", 906, $meta);
	$meta = str_replace("__IMAGE_WIDTH__", 1610, $meta);

	return $meta;

}

