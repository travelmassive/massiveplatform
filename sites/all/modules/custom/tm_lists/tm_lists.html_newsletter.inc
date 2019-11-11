<?php

// tm_lists.html_newsletter.inc

/**
 * Generate HTML preview for newsletters
 */
function tm_lists_display_list_html_newsletter($list_id) {

	global $conf;
	global $user;

	// load
	$list = node_load($list_id);

	// check permission
	if (!tm_lists_user_can_edit_list($user, $list_id)) {
		drupal_set_message(t('Sorry, you don\'t have permission to access this page.'), 'help-tip');
		drupal_goto("/lists");
	}

	// check valid list
	if ($list == null) {
		drupal_set_message(t('Sorry, the list you requested does not exist.'), 'help-tip');
		drupal_goto("/lists");
	}
	if ($list->type != "list") {
		drupal_set_message(t('Sorry, the list you requested does not exist.'), 'help-tip');
		drupal_goto("/lists");
	}

	// show items
	$show_items = 5; // default
	if (isset($_GET["items"])) {
		$show_items = intval($_GET["items"]);
	}
	if ($show_items <= 0) {
		$show_items = 5;
	}


	// hide cover image
	$hide_cover_image = false;
	if (isset($_GET["hide_cover_image"])) {
		$hide_cover_image = true;
	}

	// load templates
	$newsletter_template = file_get_contents(drupal_get_path('module', 'tm_lists') .'/templates/tm_list_html_newsletter.tpl.php');
	$newsletter_image_template = file_get_contents(drupal_get_path('module', 'tm_lists') .'/templates/tm_list_html_newsletter_image.tpl.php');
	$newsletter_item_template = file_get_contents(drupal_get_path('module', 'tm_lists') .'/templates/tm_list_html_newsletter_item.tpl.php');
	$newsletter_footer_template = @file_get_contents(drupal_get_path('module', 'tm_lists') .'/templates/tm_lists_html_newsletter_footer.tpl.php');
	if ($newsletter_footer_template == "") {
		$newsletter_footer_template = @file_get_contents(drupal_get_path('module', 'tm_lists') .'/templates/tm_lists_html_newsletter_footer.tpl.php.example');	
	}

	// get sort mode
	$sort_method = $list->field_list_sort_by[LANGUAGE_NONE][0]['value'];

	// get marked up list title
	$list_title = tm_lists_get_display_list_title($list);

	// get list description
	$list_description = "";
	if (isset($list->field_list_description[LANGUAGE_NONE][0]['value'])) {
		$list_description = trim($list->field_list_description[LANGUAGE_NONE][0]['value']);
	}
	$num_items = tm_lists_get_num_items($list->nid, 1);
	$list_description = str_replace("##", $num_items, $list_description);
	$list_description = strip_tags($list_description);
	$list_description = str_replace("\n", "</p>\n<p>", $list_description);
	$list_description = "<p>" . $list_description . "</p>";
	$list_description = tm_base_linkify($list_description, ["http", "https", "mail", "twitter"]);
	$list_description = $conf["tm_lists_newsletter_intro"] . $list_description;

	// get items from list
	$items = tm_lists_get_items_by_id($list_id, $sort_method, 0, $show_items);

	// get list URL
	$list_url = tm_lists_get_list_url($list, true);

	// get list timestamp
	$list_timestamp = tm_lists_get_latest_timestamp($list_id);

	// base url
	$base_url = $conf["tm_email_server_http"] . "://" . $conf["tm_email_server_url_domain"];

	// preview text (leave blank)
	$newsletter_template = str_replace("__LIST_PREVIEW_TEXT__", "", $newsletter_template);

	// headline
	$newsletter_template = str_replace("__LIST_HEADLINE__", $list_title, $newsletter_template);

	// description
	$newsletter_template = str_replace("__LIST_DESCRIPTION__", $list_description, $newsletter_template);

	// cover image and top padding
	$newsletter_image = "";
	$top_padding_px = "0px";
	if (!$hide_cover_image and isset($list->field_list_cover_image[LANGUAGE_NONE][0]['uri'])) {
		$image_style_url = image_style_url("list_newsletter", $list->field_list_cover_image[LANGUAGE_NONE][0]['uri']);
    	$image_url = file_create_url($image_style_url);
    	$newsletter_image = str_replace("__LIST_IMAGE_URL__", $image_url, $newsletter_image_template);
    	$top_padding_px = "20px";
    }
    $newsletter_template = str_replace("__TOP_PADDING_PX__", $top_padding_px, $newsletter_template);
    $newsletter_template = str_replace("__LIST_COVER_IMAGE__", $newsletter_image, $newsletter_template);

    // list url
    $newsletter_template = str_replace("__LIST_URL__", $list_url, $newsletter_template);

    // encoded list url
    $newsletter_template = str_replace("__ENCODED_LIST_URL__", urlencode($list_url), $newsletter_template);

    // site name
    $newsletter_template = str_replace("__SITE_NAME__", $conf["tm_site_name"], $newsletter_template);

    // hide item position
    $hide_item_position = $list->field_list_hide_rankings[LANGUAGE_NONE][0]['value'];

	// render items
	$count = 1;
	$items_html = array();
	foreach ($items as $item) { 

		$entity = null;
		
		// user
		if ($item->entity_type == 1) {
			$entity_type = "user";
			$entity = user_load($item->entity_id);
		}

		// node
		if ($item->entity_type == 2) {
			$entity_type = "node";
			$entity = node_load($item->entity_id);
		}

		// check valid item
		if ($entity == null) {
			continue;
		}

		// user fields
		if ($item->entity_type == 1) {

			$first_name = strip_tags($entity->field_user_first_name[LANGUAGE_NONE][0]['value']);
			$last_name = strip_tags($entity->field_user_last_name[LANGUAGE_NONE][0]['value']);
			$full_name = $first_name . " " . $last_name;
			$item_title = $full_name;
			$item_url = $base_url . "/user/" . $entity->uid;

			// User image
			$image_class = "avatar";
			$image_uri = _tm_users_get_avatar_uri($entity);
			$image_url = image_style_url("avatar", $image_uri);
		}

		// node fields
		if ($item->entity_type == 2) {

			$item_title = $entity->title;
			$item_url = $base_url . "/node/" . $entity->nid;

			// get image url
			if ($entity->type == "chapter") {
				$image_class = "cover";
				$image_uri = _tm_chapters_get_image_uri($entity);
				$image_url = image_style_url("banner", $image_uri);
			}
			if ($entity->type == "organization") {
				$image_class = "avatar";
				$image_uri = _tm_organizations_get_avatar_uri($entity);
				$image_url = image_style_url("avatar", $image_uri);
			}
			if ($entity->type == "event") {
				$image_class = "cover";
				$image_uri = $conf["tm_images_default_field_image"];
				if (isset($entity->field_image[LANGUAGE_NONE][0]['uri'])) {
					$image_uri = $entity->field_image[LANGUAGE_NONE][0]['uri'];
				}
				$image_url = _tm_events_get_default_image($img_url, $entity->nid);
			}
		}

		// show ranking
		if (!$hide_item_position) {
			$item_title = $count . ". " . $item_title;
		}

		// comment
		$item_comment = strip_tags($item->comment);
		$item_comment = str_replace("##", $num_items, $item_comment); // replace ## with position
		$item_comment = tm_base_linkify($item_comment, ["http", "https", "mail", "twitter"]);

		// replace fields
		$item_template = $newsletter_item_template;
		$item_template = str_replace("__ITEM_TITLE__", $item_title, $item_template);
		$item_template = str_replace("__ITEM_URL__", $item_url, $item_template);
		$item_template = str_replace("__ITEM_IMAGE_URL__", $image_url, $item_template);
		$item_template = str_replace("__ITEM_COMMENT__", $item_comment, $item_template);

		$items_html[] = $item_template;
		$count++;

	}

	// replace list items
	$list_items_html = implode("\n", $items_html);
	$newsletter_template = str_replace("__LIST_ITEMS__", $list_items_html, $newsletter_template);

	// more items
	$more_items = $num_items - $count;
	$newsletter_template = str_replace("__MORE_ITEMS__", $more_items, $newsletter_template);

	// contributors
	// get list contributors
	$contributor_uids = tm_lists_get_list_contributors($list_id);
	$show_contributor_uids = array_slice($contributor_uids, 0, 5); // show first five
	$contributor_names = array();
	foreach ($show_contributor_uids as $contributor_uid) {
		$contributor = user_load($contributor_uid);
		if ($contributor != null) {
			$first_name = strip_tags($contributor->field_user_first_name[LANGUAGE_NONE][0]['value']);
			$last_name = strip_tags($contributor->field_user_last_name[LANGUAGE_NONE][0]['value']);
			$full_name = $first_name . " " . $last_name;
			$full_name = str_replace("'", "%27", $full_name);
			$contributor_names[] = t($full_name);
		}
	}

	// outtro
	$list_outro = $conf["tm_lists_newsletter_outro"];
	$newsletter_template = str_replace("__LIST_OUTRO__", $list_outro, $newsletter_template);

	// list creators
	$list_creators = "Thanks to our list creator";
	if (sizeof($contributor_names) > 1) {
		$list_creators = "Thanks to our list creators";
	}
	//$list_creators = "This list was created by";
	$newsletter_template = str_replace("__LIST_CREATORS__", $list_creators, $newsletter_template);

	// created by
	$contributor_html = "";
	if (sizeof($contributor_names) > 0) {
		$contributor_html = implode(", ", $contributor_names);
	}
	if (sizeof($contributor_names) < sizeof($contributor_uids)) {
		$contributor_html .= " and " . (sizeof($contributor_uids) - (sizeof($contributor_links))) . " others";
	}
	$newsletter_template = str_replace("__CREATED_BY__", $contributor_html, $newsletter_template);

	// footer
	$newsletter_template = str_replace("__FOOTER_TEMPLATE__", $newsletter_footer_template, $newsletter_template);

	// facebook and twitter icon images
	$facebook_icon_url = $conf["tm_lists_newsletter_facebook_icon_url"];
	$twitter_icon_url = $conf["tm_lists_newsletter_twitter_icon_url"];
	$newsletter_template = str_replace("__FACEBOOK_ICON_URL__", $facebook_icon_url, $newsletter_template);
	$newsletter_template = str_replace("__TWITTER_ICON_URL__", $twitter_icon_url, $newsletter_template);

	// render
	echo "<style>a { color: #1595e7; }</style>";
	echo "<div id='header'><center>";
	if (trim($list->field_list_moderator_notes[LANGUAGE_NONE][0]['value']) != "") {
		$moderators_note = $list->field_list_moderator_notes[LANGUAGE_NONE][0]['value'];
		echo "<br><span style='display: inline-block; color: #888; font-family: Arial; padding-bottom: 0.5rem; border-bottom: 1px solid #ddd;'><i>Moderator notes:</i> " . str_replace("\n", "<br>", $moderators_note) . "</span><br>";
	}
	echo "</center></div>";
	echo $newsletter_template;
	echo "<div id='footer'><center><p><span style='font-family: Arial; font-size: smaller;'>HTML (copy/paste)</span>";
	echo "<br><textarea onClick='this.select();' rows='2'>" . $newsletter_template . "</textarea>";
	echo "</p>";
	echo "<a style='font-family: Arial; font-size: 9pt; color: #888;' href='?hide_cover_image'>Hide Cover Image</a>";
	echo " &middot; <a style='font-family: Arial; font-size: 9pt; color: #888;' href='?items=10'>10 Items</a>";
	echo " &middot; <a style='font-family: Arial; font-size: 9pt; color: #888;' href='#' onClick='document.getElementById(\"header\").style.display=\"none\"; document.getElementById(\"footer\").style.display=\"none\";'>Presentation Mode</a>";
	echo "</center></div><br><br>";

	drupal_exit();

}

