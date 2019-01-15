<?php

// tm_amp.front_page.inc

/**
 * Render event
 */
function tm_amp_render_front_page() {

	global $conf;

	// Note: We use https://www.ampstart.com/templates#news-blog template
	$template_file = drupal_get_path('module', 'tm_amp') . '/templates/tm_amp-template.tpl.php';
	$template = file_get_contents($template_file);

	// Replace global template vars
	$html = tm_amp_insert_template_vars($template);

	// Canonical URL
	$canonical_url = $conf["tm_email_server_http"] . "://" . $_SERVER["HTTP_HOST"] . "/";
	$html = str_replace("__CANONICAL_URL__", $canonical_url, $html);

	// Title
	$title = $conf["tm_site_name"];
	$page_heading = $conf["tm_amp_front_page_heading"];
	$pre_heading = $conf["tm_amp_front_page_pre_heading"];
	$sub_heading = $conf["tm_amp_front_page_sub_heading"];

	// Replace variables
	$html = str_replace("__PAGE_TITLE__", $title, $html);
	$html = str_replace("__PAGE_PRE_HEADING__", $pre_heading, $html);
	$html = str_replace("__PAGE_HEADING__", $page_heading, $html);
	$html = str_replace("__PAGE_SUB_HEADING__", $sub_heading, $html);

	// image
	$page_image_url = $canonical_url;
	$img_url = $conf["tm_amp_front_page_image_url"];
	$html = str_replace("__PAGE_IMAGE_URL__", $page_image_url, $html); 
	$html = str_replace("__PAGE_IMAGE__", $img_url, $html);
	$html = str_replace("__PAGE_IMAGE_WIDTH__", $conf["tm_amp_front_page_image_width"], $html);
	$html = str_replace("__PAGE_IMAGE_HEIGHT__", $conf["tm_amp_front_page_image_height"], $html);
	$html = str_replace("__PAGE_IMAGE_LAYOUT__", 'layout="responsive"', $html);
	$html = str_replace("__PAGE_IMAGE_ALT__", $title, $html);

	// description
	$description = $conf["tm_amp_front_page_welcome_message"];
	$html = str_replace("__PAGE_DESCRIPTION__", $description, $html);

	// stats
	$community_stats = tm_api_stats_get_data();
	$extra_info = '<section class="px3 mb4">
  <h2 class="mb2"></h2>
  <span class="ampstart-hint block mb3">Community insights</span>
  <ul class="mb4">';

	$extra_info .= '<li><a href="/community">' . number_format($community_stats['num_members']) . ' members</a>' . '</li>';
	$extra_info .= '<li><a href="/chapters">' . number_format($community_stats['num_chapter_countries']) . ' countries</a>' . '</li>';
	$extra_info .= '<li>' . number_format($community_stats['num_connections']) . ' connections' . '</li>';
	$extra_info .= '<li><a href="/insights">Explore insights</a></li>';

	$extra_info .= '</ul>';
	$html = str_replace("__PAGE_EXTRA__", $extra_info, $html);

	// Insert meta data
	$meta_data = tm_amp_render_front_page_meta($canonical_url);
	$html = str_replace("__META_DATA__", $meta_data, $html);

	// Youtube
	$html = str_replace("__CUSTOM_SCRIPTS__", "", $html);
	$html = str_replace("__YOUTUBE_HTML__", "", $html);

	// return html
	return $html;
}

/**
 * Render meta data
 */
function tm_amp_render_front_page_meta($canonical_url) {

	global $conf;

	$meta = '<script type="application/ld+json">
  {
    "@context": "http://schema.org",
    "@type": "Organization",
    "mainEntityOfPage": "__CANONICAL_URL__",
    "name": "__NAME__",
    "description": "__DESCRIPTION__",
    "location": "__LOCATION__",
    "logo": {
      "@type": "ImageObject",
      "url": "__IMAGE_URL__",
      "height": __IMAGE_HEIGHT__,
      "width": __IMAGE_WIDTH__
    }
  }
</script>';

	$meta = str_replace("__CANONICAL_URL__", $canonical_url, $meta);
	$meta = str_replace("__NAME__", str_replace('"', "", $conf["tm_site_name"]), $meta);
	$meta = str_replace("__DESCRIPTION__", str_replace('"', "", $conf["tm_amp_front_page_meta_description"]), $meta);
	$meta = str_replace("__LOCATION__", str_replace('"', "", $conf["tm_amp_front_page_meta_location"]), $meta);


	// image
	$meta = str_replace("__IMAGE_URL__", $conf["tm_amp_front_page_meta_image_url"], $meta);
	$meta = str_replace("__IMAGE_HEIGHT__", $conf["tm_amp_front_page_meta_image_height"], $meta);
	$meta = str_replace("__IMAGE_WIDTH__", $conf["tm_amp_front_page_meta_image_width"], $meta);

	return $meta;

}
