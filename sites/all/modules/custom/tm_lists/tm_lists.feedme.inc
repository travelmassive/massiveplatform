<?php

// tm_lists.feedme.inc

/**
 * Generate feedme display
 */
function tm_lists_feedme_display() {

	global $user;

	// validate input
	if (!isset($_GET["entity_type"])) {
		echo "";
		drupal_exit();
	}
	if (!isset($_GET["entity_id"])) {
		echo "";
		drupal_exit();
	}
	if (!isset($_GET["num_items"])) {
		echo "";
		drupal_exit();
	}

	// get input
	$entity_type = $_GET["entity_type"];
	$entity_id = intval($_GET["entity_id"]);
	$num_items = intval($_GET["num_items"]);

	// max items
	if ($num_items > 10) {
		$num_items = 5;
	}

	// get related lists
	$entity_type_int = tm_lists_enumerate_entity_type($entity_type);
	$list_ids = tm_lists_get_entity_lists($entity_type_int, $entity_id, $num_items);

	// include any lists the member has authored
	$author_list_ids = array();
	if ($entity_type == 'user') {
		$author_list_ids = tm_lists_get_lists_user_added_to($entity_id, $num_items);
	}

	// make array of unique list ids
	$list_id_arr = array();
	foreach ($list_ids as $list_id) {
		$list_id_arr[] = $list_id->list_id;
	}
	foreach ($author_list_ids as $list_id) {
		if (!in_array($list_id->list_id, $list_id_arr)) {
			$list_id_arr[] = $list_id->list_id;
		}
	}

	// show contributor status on user profile
	$show_contributor_uid = 0;
	if ($entity_type == "user") {
		$show_contributor_uid = $entity_id;
	}

	// render related lists
	$html = tm_lists_feedme_render($list_id_arr, "Lists", "/lists", "All lists", $entity_type, $entity_id, $show_contributor_uid);
	echo $html;
	drupal_exit();
}

/**
 * Render list of related lists
 */
function tm_lists_feedme_render($list_ids, $title, $more_link, $more_text, $entity_type, $entity_id, $show_contributor_uid = 0) {

	global $user;
	global $conf;

	// load templates
	$template = file_get_contents(drupal_get_path('module', 'tm_lists') .'/templates/tm_lists_feedme.tpl.php');
	$list_template = file_get_contents(drupal_get_path('module', 'tm_lists') .'/templates/tm_lists_feedme_list.tpl.php');
	$more_template = '<div class="more-link" style="font-size: 14px;"><a href="__MORE_URL__" class="feedme more">__MORE_TEXT__</a></div>';

	$lists_html = "";

	// render each item
	$render_count = 0;
	foreach ($list_ids as $list_id) {

		$list = node_load($list_id);
		if ($list == null) {
			continue;
		}

		$list_meta = "";

		// check preview mode
		$preview_mode = $list->field_list_preview_mode[LANGUAGE_NONE][0]['value'];
		if (($preview_mode == 1) and (tm_lists_user_can_add_to_list($user, $list_id))) {
			$list_meta = "(Preview mode) ";
		}
		if (($preview_mode == 1) and (!tm_lists_user_can_add_to_list($user, $list_id))) {
			continue;
		}

		// get list title and url
		// link to featured item
		// ie: /lists/123/list-name#feature-125355
		$list_title = tm_lists_get_display_list_title($list);
		$list_url = "/lists/" . $list_id . "#feature-" . tm_lists_enumerate_entity_type($entity_type) . $entity_id;

		// list image
		// try cover first, then sharing image
		$list_image_url = $conf["tm_lists_default_list_thumbnail_url"];
  		if (isset($list->field_list_cover_image[LANGUAGE_NONE][0]['uri'])) {
  			$list_image_url = image_style_url("medium", $list->field_list_cover_image[LANGUAGE_NONE][0]['uri']);
    	} elseif(isset($list->field_list_social_image[LANGUAGE_NONE][0]['uri'])) {
			$list_image_url = image_style_url("medium", $list->field_list_social_image[LANGUAGE_NONE][0]['uri']);
		}

    	// check if author
    	if ($show_contributor_uid > 0) {
			if (tm_lists_has_user_added_to_list($show_contributor_uid, $list_id)) {
				$list_meta .= "List curator";
			}
    	}

		//$list_meta .= "";
		$list_html = $list_template;
		$list_html = str_replace("__LIST_TITLE__", $list_title, $list_html);
		$list_html = str_replace("__LIST_URL__", $list_url, $list_html);
		$list_html = str_replace("__LIST_IMAGE__", $list_image_url, $list_html);
		$list_html = str_replace("__LIST_META__", $list_meta, $list_html);
		$lists_html .= $list_html;

		$render_count++;
	}
	
	// make sure we've rendered something
	if ($render_count == 0) {
		return "";
	}

	$more_template = str_replace("__MORE_URL__", $more_link, $more_template);
	$more_template = str_replace("__MORE_TEXT__", $more_text, $more_template);

	$template = str_replace("__MORE_LINK__", $more_template, $template);

	$template = str_replace("__TITLE__", $title, $template);
	$template = str_replace("__LIST_ITEMS__", $lists_html, $template);

	// return html
	return $template;
}
