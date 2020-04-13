<?php

// tm_match.connections.inc

function tm_match_connections_index() {

	global $conf;
	global $user;

	// connections per page
	$connections_per_page = $conf["tm_match_connections_per_page"];

	// clear help tip messages
	drupal_get_messages("help-tip");

	// not logged in
	if (!user_is_logged_in()) {
		drupal_goto("match");
	}

	// check disabled
	if ($conf["tm_match_offline"]) {
		drupal_goto("match");
	}

	// redirect if member is reported
	if (tm_users_is_member_reported($user->uid)) {
		drupal_goto("match");
	}

	// approved members
	if (!in_array("approved user", $user->roles)) {
		drupal_goto("match");
	}

	// get user id
	$user_id = $user->uid;
	if (in_array("moderator", $user->roles)) {
		if (isset($_GET["user_id"])) {
			$user_id = intval($_GET["user_id"]);
		}
	}

	// include css
	drupal_add_css(drupal_get_path('module', 'tm_match') . '/css/tm_match.css');

	// js
	drupal_add_js(drupal_get_path('module', 'tm_match') . '/js/noframework.waypoints.min.js');
	drupal_add_js(drupal_get_path('module', 'tm_match') . '/js/tm_match_connections.js');

	// load template
	$template = file_get_contents(drupal_get_path('module', 'tm_match') .'/templates/tm_match_connections.tpl.php');

	// get mutual connections
	$connections = tm_match_get_user_connections($user_id, 0, $connections_per_page);
	$results_html = tm_match_render_connections($connections);

	// pager
	$pager_template = "";
	if (sizeof($connections) >= $connections_per_page) {
		$pager_template = file_get_contents(drupal_get_path('module', 'tm_match') .'/templates/tm_match_connections_pager.tpl.php');
	}

	// fields
	$html = str_replace("__INTRO__", $conf["tm_match_connections_intro"], $template);
	$html = str_replace("__UID__", $user_id, $html);
	$html = str_replace("__RESULTS__", $results_html, $html);
	$html = str_replace("__PAGER__", $pager_template, $html);
	
	$mutual_connections = tm_match_get_count_user_connections($user_id);
	$num_connections = $mutual_connections . " members";
	if ($mutual_connections == 1) {
		$num_connections = "1 member";
	}
	$html = str_replace("__NUM_CONNECTIONS__", $num_connections, $html);

	// debug
	$debug_css = "display: none;";
	if (isset($_GET["debug"])) {
		$debug_css = "display: block;";
	}
	$html = str_replace("__DEBUG_CSS__", $debug_css, $html);

	// create form
	$form['html'] = array(
		'#type' => 'item',
		'#markup' => $html,
		);
	return $form;

}

function tm_match_render_connections($uids) {

	global $conf;

	// load template
	$template = file_get_contents(drupal_get_path('module', 'tm_match') .'/templates/tm_match_user.tpl.php');

	// rende users
	$html = "";
	foreach ($uids as $uid) {

		$account = user_load($uid);
		if ($account == null) {
			continue;
		}

		// get name
		$first_name = "";
		$last_name = "";
		if (isset($account->field_user_first_name[LANGUAGE_NONE][0]['value'])) {
			$first_name = strip_tags($account->field_user_first_name[LANGUAGE_NONE][0]['value']);
		}
		if (isset($account->field_user_last_name[LANGUAGE_NONE][0]['value'])) {
			$last_name = strip_tags($account->field_user_last_name[LANGUAGE_NONE][0]['value']);
		}
		$full_name = $first_name . " " . $last_name;

		// image url
		$avatar_url = image_style_url("avatar", _tm_users_get_avatar_uri($account));

		// cover image url
		if (isset($account->field_image[LANGUAGE_NONE][0]['uri'])) {
    		$cover_image_url = $account->field_image[LANGUAGE_NONE][0]['uri'];
  		} else {
  			$cover_image_url = _tm_users_get_default_image($conf["tm_images_default_field_image"], $account->uid);
  		}
  		$cover_image_url = image_style_url("banner_grid", $cover_image_url);

		// job role
		$job_role = "";
		if (isset($account->field_job_role[LANGUAGE_NONE][0]['value'])) {
			$job_role = '<span class="role">' . strip_tags($account->field_job_role[LANGUAGE_NONE][0]['value']) . '</span>';
		}

		// organization
		$organization_name = "";
		if (isset($account->field_job_organization[LANGUAGE_NONE][0]['value'])) {
			$organization_name = '<span class="organization">' . strip_tags($account->field_job_organization[LANGUAGE_NONE][0]['value']) . '</span>';
		}

		// home chapter
		$home_chapter_url = "/";
		$home_chapter_code = "";
		$home_chapter_color = "";
		if (isset($account->field_home_chapter[LANGUAGE_NONE][0]['target_id'])) {
			$home_chapter = node_load($account->field_home_chapter[LANGUAGE_NONE][0]['target_id']);
			$home_chapter_url = drupal_get_path_alias('node/' . $home_chapter->nid);
			$home_chapter_code = $home_chapter->field_chapter_shortcode[LANGUAGE_NONE][0]['value'];
			$home_chapter_color = $home_chapter->field_chapter_color[LANGUAGE_NONE][0]['rgb'];
		}

		$user_template = $template;
		$user_template = str_replace("__UID__", $account->uid, $user_template);
		$user_template = str_replace("__FULL_NAME__", $full_name, $user_template);
		$user_template = str_replace("__JOB_ROLE__", $job_role, $user_template);
		$user_template = str_replace("__ORGANIZATION_NAME__", $organization_name, $user_template);
		$user_template = str_replace("__AVATAR_URL__", $avatar_url, $user_template);
		$user_template = str_replace("__COVER_IMAGE_URL__", $cover_image_url, $user_template);
		$user_template = str_replace("__CHAPTER_URL__", $home_chapter_url, $user_template);
		$user_template = str_replace("__CHAPTER_CODE__", $home_chapter_code, $user_template);
		$user_template = str_replace("__CHAPTER_COLOR__", $home_chapter_color, $user_template);

		$html .= "<li>" . $user_template . "</li>";
	}


	return $html;
}

function tm_match_fetch_connections_ajax() {

	global $conf;
	global $user;

	$connections_per_page = $conf["tm_match_connections_per_page"];

	if (!user_is_logged_in()) {
		echo "Oops, you're not logged in.";
		drupal_exit();
	}

	// redirect if member is reported
	if (tm_users_is_member_reported($user->uid)) {
		echo "Oops, you can't fetch new members right now.";
		drupal_exit();
	}

	// get user id
	$user_id = $user->uid;
	if (in_array("moderator", $user->roles)) {
		if (isset($_GET["user_id"])) {
			$user_id = intval($_GET["user_id"]);
		}
	}

	// get page
	$page = 1;
	if (isset($_GET["page"])) {
		$page = intval($_GET["page"]);
	}
	if ($page < 1) {
		$page = 1;
	}

	$limit_from = ($page - 1) * $connections_per_page;
	$connections = tm_match_get_user_connections($user_id, $limit_from, $connections_per_page);
	$results_html = tm_match_render_connections($connections);

	// Return html
	echo $results_html;
	drupal_exit();
}
