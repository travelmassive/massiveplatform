<?php

// tm_amp.helpers.inc

/**
 * Insert common variables
 */
function tm_amp_insert_template_vars($html, $user_id = null) {

	global $conf;

	$html = str_replace("__FAVICON_URL__", $conf["tm_amp_favicon_url"], $html);
	$html = str_replace("__AMP_LOGO_URL__", $conf["tm_amp_logo_url"], $html);
	$html = str_replace("__SITE_NAME__", $conf["tm_site_name"], $html);
	$html = str_replace("__PAGE_FOOTER_TEXT__", $conf["tm_amp_footer_text"], $html);
	$html = str_replace("__PAGE_FOOTER_LINKS__", $conf["tm_amp_footer_links"], $html);
	$html = str_replace("__COPYRIGHT_NAME__", $conf["tm_amp_footer_copyright"], $html);
	$html = str_replace("__YEAR__", date('Y'), $html);
	$html = str_replace("__AMP_CLIENT_ID_API_TAG__", $conf["tm_amp_client_id_api_tag"], $html);
	$html = str_replace("__SIDEBAR_TOP_LINKS__", $conf["tm_amp_sidebar_top_links"], $html);
	$html = str_replace("__SIDEBAR_SECOND_LINKS__", $conf["tm_amp_sidebar_second_links"], $html);
	$html = str_replace("__TWITTER_URL__", $conf["tm_amp_sidebar_twitter_url"], $html);
	$html = str_replace("__FACEBOOK_URL__", $conf["tm_amp_sidebar_facebook_url"], $html);
	$html = str_replace("__INSTAGRAM_URL__", $conf["tm_amp_sidebar_instagram_url"], $html);
	$html = str_replace("__SITE_DOMAIN__", $_SERVER["HTTP_HOST"], $html);

	// Google Analytics
	$html = str_replace("__GA_TRACKING_ID__", $conf["tm_amp_google_analytics_id"], $html);

	// Protect pii on user pages
	$ga_extra_params = "";
	if ($user_id != null) {
		global $base_url;
		$ga_extra_params = ',
		"page_title": "User Profile Page ' . $user_id . '",
        "page_location": "' . $base_url . "/user/" . $user_id . '"';
	}
	$html = str_replace("__GA_TRACKING_EXTRA_PARAMS__", $ga_extra_params, $html);

	return $html;
}

/**
 * Pluralize helper
 */
function tm_amp_pluralize($num) {
	if ($num > 1) {
		return "s";
	}
	return "";
}

/**
 * Get youtube video ids from a URL
 */
function tm_amp_get_youtube_id($youtube_url) {

	// check if user has cover video
	$cover_video_id = null;

	// match youtube url
	// http://stackoverflow.com/questions/3392993/php-regex-to-get-youtube-video-id
	preg_match("/^(?:http(?:s)?:\/\/)?(?:www\.)?(?:m\.)?(?:youtu\.be\/|youtube\.com\/(?:(?:watch)?\?(?:.*&)?v(?:i)?=|(?:embed|v|vi|user)\/))([^\?&\"'>]+)/", $youtube_url, $matches);

	if (sizeof($matches) != 0 ) {
	  $cover_video_id = $matches[1];
	} else {
	  // check if its just id
	  if ((strlen($youtube_url < 20) && (strpos($youtube_url, ".") === false))) {
	    $cover_video_id = $youtube_url;
	  }
	}

  	return $cover_video_id;
}

/**
 * Escape a string for use in JSON
 */
function tm_amp_escape_string($str) {
	$json_encoded = json_encode($str);
	$json_encoded = trim($json_encoded,'"'); // remove the " at start and end
	$json_encoded = str_replace('\r\n', "\n", $json_encoded); // replace escaped \n with new line
	return $json_encoded;
}
