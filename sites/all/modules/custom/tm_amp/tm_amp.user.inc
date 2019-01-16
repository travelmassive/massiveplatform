<?php

// tm_amp.user.inc

/**
 * Render user
 */
function tm_amp_render_user($user_id) {

	global $conf;

	// Load user
	$account = user_load($user_id);
	if ($account == null) {
		return "Oops, an error occured (invalid user id). <a href='/'>Return home</a>.";
	}

	// Note: We use https://www.ampstart.com/templates#news-blog template
	$template_file = drupal_get_path('module', 'tm_amp') . '/templates/tm_amp-template.tpl.php';
	$template = file_get_contents($template_file);

	// Replace global template vars
	$html = tm_amp_insert_template_vars($template, $user_id);

	// Canonical URL
	$canonical_url = $conf["tm_email_server_http"] . "://" . $_SERVER["HTTP_HOST"] . "/" . drupal_get_path_alias("user/" . $user_id);
	$html = str_replace("__CANONICAL_URL__", $canonical_url, $html);

	// Name
	$full_name = "User";
	$first_name = "";
	$last_name = "";
	if (isset($account->field_user_first_name[LANGUAGE_NONE][0]['value'])) {
		$first_name = strip_tags($account->field_user_first_name[LANGUAGE_NONE][0]['value']);
		$full_name = $first_name;
	}
	if (isset($account->field_user_last_name[LANGUAGE_NONE][0]['value'])) {
		$last_name = strip_tags($account->field_user_last_name[LANGUAGE_NONE][0]['value']);
		$full_name .= " " . $last_name;
	}

	// Replace variables
	$pre_heading = "<a href='/user/login?destination=" . strip_tags(drupal_get_path_alias("user/" . $user_id)) . "'>Log in</a> or <a href='/user/register'>Sign Up</a>"; // "&raquo; <a href='/community'>People</a>"
	$html = str_replace("__PAGE_TITLE__", $full_name . " | " . $conf["tm_site_name"], $html);
	$html = str_replace("__PAGE_PRE_HEADING__", $pre_heading, $html);
	$html = str_replace("__PAGE_HEADING__", $full_name, $html);

	// job role, and name
	$sub_title = "";
	if (isset($account->field_job_role[LANGUAGE_NONE][0]['value'])) {
		$sub_title = strip_tags($account->field_job_role[LANGUAGE_NONE][0]['value']);
	}
	if (isset($account->field_job_organization[LANGUAGE_NONE][0]['value'])) {
		if ($sub_title != "") {
			$sub_title .= ", ";
		}
		$sub_title .= strip_tags($account->field_job_organization[LANGUAGE_NONE][0]['value']);
	}

	// location
	$location = "";
	if (isset($account->field_location_city[LANGUAGE_NONE][0]['value'])) {
		$location = strip_tags($account->field_location_city[LANGUAGE_NONE][0]['value']);
	}
	if ($location != "") {
		if ($sub_title != "") {
			$sub_title .= "<br>";
		}
		$sub_title .= $location;
	}

	$html = str_replace("__PAGE_SUB_HEADING__", $sub_title, $html);

	// Set avatar
	$avatar_url = image_style_url("avatar", _tm_users_get_avatar_uri($account));
	$img_url = file_create_url($avatar_url);

	// image
	$html = str_replace("__PAGE_IMAGE_URL__", "/" . drupal_get_path_alias("user/" . $user_id), $html); 
	$html = str_replace("__PAGE_IMAGE__", $img_url, $html);
	$html = str_replace("__PAGE_IMAGE_WIDTH__", 128, $html);
	$html = str_replace("__PAGE_IMAGE_HEIGHT__", 128, $html);
	$html = str_replace("__PAGE_IMAGE_LAYOUT__", '', $html);
	$html = str_replace("__PAGE_IMAGE_ALT__", $full_name, $html);

	// about me
	$about_me = "";
	if (isset($account->field_about_me[LANGUAGE_NONE][0]['value'])) {
		$about_me = strip_tags($account->field_about_me[LANGUAGE_NONE][0]['value']);
		$about_me = str_replace("\n", "<br>", $about_me); // add line breaks
    	$about_me = preg_replace("/[-]{4,10}/", "<br>", $about_me); // replace ---- with hr
	}
	$html = str_replace("__PAGE_DESCRIPTION__", $about_me, $html);

	// extra
	$num_followers = tm_users_get_num_followers($user_id);
	$num_following = tm_users_get_num_following($user_id);
	$num_events = tm_users_get_num_registered_events($user_id);

	// user segment
	$user_segment_l1 = "";
	$user_segment_l2 = "";
	$field_segment_tid = tm_users_get_taxonomy_tid($user_id);
	if ($field_segment_tid != null){
		$term = taxonomy_term_load($field_segment_tid);
		// load parent term
		$parent_terms = taxonomy_get_parents($term->tid);
		if (sizeof($parent_terms) > 0) {
			$parent_term = current($parent_terms);
			$user_segment_l1 = $parent_term->name;
			$user_segment_l2 = $term->name;
		} else {
			$user_segment_l1 = $term->name;
			$user_segment_l2 = "";
		}
	}
	
	$industry_segment = $user_segment_l1;
	if ($user_segment_l2 != "") {
		$industry_segment .= " &gt; " . $user_segment_l1;
	}

	// home chapter
	$home_chapter_title = "";
	if (isset($account->field_home_chapter[LANGUAGE_NONE][0]['target_id'])) {
		$home_chapter = node_load($account->field_home_chapter[LANGUAGE_NONE][0]['target_id']);
		if ($home_chapter != null) {
			$home_chapter_title = strip_tags($home_chapter->title);
		}
	}

	$extra_info = '<section class="px3 mb4">
  <h2 class="mb2"></h2>
  <span class="ampstart-hint block mb3">Member details</span>
  <ul class="mb4">';

  	// check if chapter leader
	$approved_member_role = user_role_load_by_name("approved user")->rid;
	$chapter_leader_role = user_role_load_by_name("chapter leader")->rid;
	$is_chapter_leader = tm_users_userid_has_roleid($user_id, $chapter_leader_role);
	$approved_status = tm_users_userid_has_roleid($user_id, $approved_member_role);

	if ($industry_segment != "") {
		$extra_info .= '<li>' . $industry_segment . '</li>';
	}

	if ($home_chapter_title != "") {
	$extra_info .= '<li>Member of <a href="/' . drupal_get_path_alias('node/' . $home_chapter->nid) . '?amp">' . tm_chapters_formal_name($home_chapter) . '</a></li>';
	}

	if ($num_followers > 0) {
		$extra_info .= '<li><a href="/user/' . $user_id . '/followers">' . $num_followers . ' follower' . tm_amp_pluralize($num_followers) . '</a></li>';
	}

	if ($num_following > 0) {
		$extra_info .= '<li><a href="/user/' . $user_id . '/following">' . $num_following . ' following</a></li>';
	}

	if ($num_events > 0) {
		$extra_info .= '<li>' . $num_events . ' event' . tm_amp_pluralize($num_events) . '</li>';
	}

	// when joined
	$when_joined = format_date($account->created, 'custom', 'F Y');
	$extra_info .= '<li>Member since ' . $when_joined . '</li>';

	$extra_info .= '</ul>';
	$html = str_replace("__PAGE_EXTRA__", $extra_info, $html);
	
	// Insert meta data
	$meta_data = tm_amp_render_user_meta($account, $canonical_url, $first_name, $last_name, $about_me, $img_url);

	// Check if user has set to private
	$noindex = false;
	if (isset($account->field_user_disable_public_index[LANGUAGE_NONE][0]['value'])) {
		if ($account->field_user_disable_public_index[LANGUAGE_NONE][0]['value']) {
			$noindex = true;
		}
	}

	// Check if account is approved
	if (!in_array('approved user', $account->roles)) {
		$noindex = true;
	}

	// Set noindex
	if ($noindex) {
		$meta_data .= "
<meta name='robots' content='noindex'>";
	}

	$html = str_replace("__META_DATA__", $meta_data, $html);

	// Youtube
	$youtube_js = "";
	$youtube_html = "";
	if (isset($account->field_user_cover_video[LANGUAGE_NONE][0]['value'])) {
		
		$cover_video_value = $account->field_user_cover_video[LANGUAGE_NONE][0]['value'];
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
function tm_amp_render_user_meta($account, $canonical_url, $first_name, $last_name, $about_me, $image_url) {

	$meta = '<script type="application/ld+json">
  {
    "@context": "http://schema.org",
    "@type": "Person",
    "mainEntityOfPage": "__CANONICAL_URL__",
    "url": "__CANONICAL_URL__",
    "givenName": "__GIVEN_NAME__",
    "familyName": "__FAMILY_NAME__",
    "description": "__DESCRIPTION__",
    "nationality": {
    	"@type": "Country",
    	 "name": "__COUNTRY__"
    },
    "jobTitle": "__JOB_ROLE__",
    "memberOf": {
    	 "@type": "Organization",
    	 "name": "__JOB_ORGANIZATION__"
    },
    "image": {
      "@type": "ImageObject",
      "url": "__IMAGE_URL__",
      "height": __IMAGE_HEIGHT__,
      "width": __IMAGE_WIDTH__
    }
  }
</script>';

	$meta = str_replace("__CANONICAL_URL__", $canonical_url, $meta);
	$meta = str_replace("__GIVEN_NAME__", tm_amp_escape_string($first_name), $meta);
	$meta = str_replace("__FAMILY_NAME__", tm_amp_escape_string($last_name), $meta);
	$about_me = str_replace("<br>", "\n", $about_me);
	$meta = str_replace("__DESCRIPTION__", tm_amp_escape_string($about_me), $meta);

	// country
	$country_name = "";
	if (isset($account->field_user_country[LANGUAGE_NONE][0]['iso2'])) {
		$country = country_load($account->field_user_country[LANGUAGE_NONE][0]['iso2']);
		$country_name = $country->name;
	}
	$meta = str_replace("__COUNTRY__", $country_name, $meta);

	// job role
	$job_role = "";
	if (isset($account->field_job_role[LANGUAGE_NONE][0]['value'])) {
		$job_role = strip_tags($account->field_job_role[LANGUAGE_NONE][0]['value']);
	}
	$meta = str_replace("__JOB_ROLE__", $job_role, $meta);

	// job organization
	$job_organization = "";
	if (isset($account->field_job_organization[LANGUAGE_NONE][0]['value'])) {
		$job_organization = strip_tags($account->field_job_organization[LANGUAGE_NONE][0]['value']);
	}
	$meta = str_replace("__JOB_ORGANIZATION__", $job_organization, $meta);

	// image
	$meta = str_replace("__IMAGE_URL__", $image_url, $meta);
	$meta = str_replace("__IMAGE_HEIGHT__", 512, $meta);
	$meta = str_replace("__IMAGE_WIDTH__", 512, $meta);

	return $meta;

}
