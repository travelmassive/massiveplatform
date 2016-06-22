<?php

// tm_status_updates.post_form.inc - methods to render status update form

/**
 * Render the newsfeed update form
 */
function tm_status_updates_render_update_form($account) {

	global $conf;
	
	// prepare status update form
	if (isset($account->field_avatar[LANGUAGE_NONE][0]['uri'])) {
		$image_url = image_style_url("avatar", $account->field_avatar[LANGUAGE_NONE][0]['uri']);
	} else {
		$default_image_uri = _tm_users_get_default_avatar($conf["tm_images_default_field_avatar"], $account->uid);
		$image_url = image_style_url("avatar", $default_image_uri);
	}

	// status update form
	$tm_status_update_form = file_get_contents(drupal_get_path('module', 'tm_status_updates') .'/templates/tm_status_updates_post_form.tpl.php');
	$tm_status_update_form = str_replace("__MEMBER_IMAGE_SRC__", $image_url, $tm_status_update_form);
	
	// get post as options
	$post_as_options = tm_status_updates_get_user_options($account);
	$tm_status_update_form = str_replace("__POST_AS_OPTIONS__", $post_as_options, $tm_status_update_form);

	// get post as images
	$post_as_images = tm_status_updates_get_post_as_images($account);
	$tm_status_update_form = str_replace("__POST_AS_IMAGES__", json_encode($post_as_images), $tm_status_update_form);

	// placeholder text
	$rand_keys = array_rand($conf["tm_status_updates_placeholders"], 1);
	$placeholder_arr = $conf["tm_status_updates_placeholders"];
	$placeholder_text = $placeholder_arr[mt_rand(0, count($placeholder_arr) - 1)];
	$tm_status_update_form = str_replace("__PLACEHOLDER_TEXT__", $placeholder_text, $tm_status_update_form);

	return $tm_status_update_form;
}

/**
 * Helper method to generate list of post as options for the user
 */
function tm_status_updates_get_user_options($account) {


	// post as: user
	$posting_user_name = strip_tags($account->field_user_first_name[LANGUAGE_NONE][0]['value'] . " " . $account->field_user_last_name[LANGUAGE_NONE][0]['value']);
	$post_as_options = "<option value='uid-" . $account->uid . "'>Post as: " . $posting_user_name . "</option>";

	// post as: company
	$companies_nids = tm_organizations_get_users_companies($account->uid);
	foreach ($companies_nids as $company_nid) {
		$organization = node_load($company_nid);
		$post_as_options .= "\n<option value='nid-" . $company_nid . "'>Post as: " . strip_tags($organization->title) . "</option>";
	}

	// post as: chapter
	$chapters_nids = tm_users_get_chapters_leading($account);
	foreach ($chapters_nids as $chapter_nid) {
		$chapter = node_load($chapter_nid);
		$post_as_options .= "\n<option value='nid-" . $chapter_nid . "'>Post as: " . strip_tags(tm_chapters_formal_name($chapter)) . "</option>";
	}

	return $post_as_options;
}

/**
 * Helper method to generate array of images for account posting
 */
function tm_status_updates_get_post_as_images($account) {

	$images = array();

	// post as: user
	$image_uri = _tm_users_get_avatar_uri($account);
	$images['uid-' . $account->uid] = image_style_url("avatar", $image_uri);

	// post as: company
	$companies_nids = tm_organizations_get_users_companies($account->uid);
	foreach ($companies_nids as $company_nid) {
		$organization = node_load($company_nid);
		$image_uri = _tm_organizations_get_avatar_uri($organization);
		$images['nid-' . $organization->nid] = image_style_url("avatar", $image_uri);
	}

	// post as: chapter
	$chapters_nids = tm_users_get_chapters_leading($account);
	foreach ($chapters_nids as $chapter_nid) {
		$chapter = node_load($chapter_nid);
		$image_uri = _tm_chapters_get_image_uri($chapter);
		$images['nid-' . $chapter->nid] = image_style_url("banner", $image_uri);
	}

	return $images;
}


