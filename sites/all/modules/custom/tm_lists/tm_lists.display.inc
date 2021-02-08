<?php

// tm_lists.display.inc

include_once 'includes/autoembed.php';

/* Lists index */

function tm_lists_display_index() {

	global $conf;
	global $user;

	// cache for 5 minutes if logged out
	if (!user_is_logged_in()) {
		drupal_add_http_header('Cache-Control', 'public, max-age=300');
	}

	$lists = tm_lists_get_lists();

	// include css
	drupal_add_css(drupal_get_path('module', 'tm_lists') . '/css/tm_lists.css');

	// load template
	$template = file_get_contents(drupal_get_path('module', 'tm_lists') .'/templates/tm_lists_index.tpl.php');

	// intro
	$template = str_replace("__INTRO__", $conf["tm_lists_homepage_intro"], $template);

	// generate lists
	$list_html = "<ul class='tm-lists-index'>";
	foreach ($lists as $list_item) {

		// list id
		$list_id = $list_item["list_id"];

		// load list node
		$list = node_load($list_id);
		if ($list == null) {
			continue;
		}

		// check preview mode
		$show_preview = false;
		$preview_mode = $list->field_list_preview_mode[LANGUAGE_NONE][0]['value'];
		if (($preview_mode == 1) and (tm_lists_user_can_add_to_list($user, $list_id))) {
			$show_preview = true;
		}
		if (($preview_mode == 1) and (!tm_lists_user_can_add_to_list($user, $list_id))) {
			continue;
		}

		// get list title
		$list_title = tm_lists_get_display_list_title($list);

		// get list url
		$list_url = tm_lists_get_list_url($list);

		// render list
		$list_html .= "<a href='" . $list_url . "'><li><span class='tm-lists-label'>" . $list_title;
		

		$updated_time = "";
		$preview_mode = "";
		if ($show_preview) {
			$preview_mode = "<b>Preview mode</b> &nbsp;&nbsp;";
		}
		$updated_timestamp = tm_lists_get_latest_timestamp($list_id);
		if ($updated_timestamp != null) {
			$difference = time() - $updated_timestamp; // $list_item["updated"];
	        $update_time = format_interval($difference, 1);
	        $update_time = str_replace("sec", "seconds", $update_time);
	        $update_time = str_replace("min", "minutes", $update_time);
	        $updated_time = $preview_mode . " Updated " . $update_time . " ago";
		}
		
		$list_html .= "</span><span class='tm-lists-details'>" . $updated_time . "</span></li></a>";
	}

	$list_html .= "</ul>";
	$template = str_replace("__LISTS__", $list_html, $template);

	// create list link
	$create_list_link = "";
	if ((in_array("moderator", $user->roles)) or (in_array("list-editor", $user->roles))) {
		$create_list_link = "<div style='float: right; margin-top: 1.5rem; margin-right: 0.5rem;'><a style='font-size: smaller; color: #888; text-decoration: underline;' href='/node/add/list'>Create list</a></a>";
	}
	$template = str_replace("__CREATE_LIST_LINK__", $create_list_link, $template);

	// create form
	$form['html'] = array(
		'#type' => 'item',
		'#markup' => $template,
		);
	return $form;

}

/* List view */
function tm_lists_display_list($list_id) {

	global $conf;
	global $user;

	// add rss link
	$rss_link = array(
		'#tag' => 'link',
		'#attributes' => array(
		'rel' => 'alternate',
		'href' => $conf["tm_email_server_http"] . "://" . $conf["tm_email_server_url_domain"] . '/lists/' . $list_id . "/rss",
		'type' => 'application/rss+xml',
		),
	);
	drupal_add_html_head($rss_link, true);

	// load
	$list = node_load($list_id);
	if ($list == null) {
		drupal_set_message(t('Sorry, the list you requested does not exist.'), 'help-tip');
		drupal_goto("/lists");
	}

	// redirect to slug
	if (isset($list->field_list_slug_url[LANGUAGE_NONE][0]['value'])) {

		// get list url
		$list_url = tm_lists_get_list_url($list);
		
		if (current_path() != $list_url) {
			drupal_goto($list_url);
		}
	}

	// get sort order
	$sort_method = $list->field_list_sort_by[LANGUAGE_NONE][0]['value'];

	// check if user can perform action on list
	$user_can_perform_list_action = false;
	if (tm_lists_user_can_add_to_list($user, $list_id)) {
		$user_can_perform_list_action = true;
	}
	if (tm_lists_user_can_remove_from_list($user, $list_id)) {
		$user_can_perform_list_action = true;
	}

	// custom css
	if (isset($list->field_list_custom_css[LANGUAGE_NONE][0]['value'])) {
		$custom_css = trim($list->field_list_custom_css[LANGUAGE_NONE][0]['value']);
		$custom_css = strip_tags($custom_css);
		if ($custom_css != "") {
			drupal_add_css($custom_css, array('type' => 'inline'));
		}
	}

	// include list display js
	drupal_add_js('var tm_global_list_id = ' . $list_id . '; var tm_global_list_sort_method = "' . $sort_method . '";', array('type' => 'inline', 'scope' => 'footer'));
	drupal_add_js(drupal_get_path('module', 'tm_lists') . '/js/tm_lists_display.js');

	// include waypoints js
	drupal_add_js(drupal_get_path('module', 'tm_lists') . '/js/noframework.waypoints.min.js');

	// include lazysizes and unload (plugin) js
	if ($conf["tm_lists_oembed_enabled"]) {

		drupal_add_js(drupal_get_path('module', 'tm_lists') . '/js/lazysizes.min.js');
	
		// include lazysizes config
		// see https://github.com/aFarkas/lazysizes js-options
		$lazysizes_js = "window.lazySizesConfig = window.lazySizesConfig || {}; lazySizesConfig.loadMode = 2;";
		drupal_add_js($lazysizes_js, array('type' => 'inline', 'scope' => 'header'));
	}

	// include tm_lists_actions.js if user can perform actions
	if ($user_can_perform_list_action) {
		drupal_add_js(drupal_get_path('module', 'tm_lists') . '/js/tm_lists_actions.js');
	}
	
	// include css
	drupal_add_css(drupal_get_path('module', 'tm_lists') . '/css/tm_lists.css');

	// add list meta tags
    tm_lists_add_list_meta_tags($list);

	// get preview mode
	$preview_mode = $list->field_list_preview_mode[LANGUAGE_NONE][0]['value'];
	if ($preview_mode) {
		drupal_set_message("This list is in Preview mode. Only people with the link can see this list.", "help-tip");
	}

	// disable analtyics if user is list editor
	if ($user_can_perform_list_action) {
		// disable analytics in tm_users_google_tag_insert_alter
		$tm_google_tag_disabled = &drupal_static("tm_google_tag_disabled");
		$tm_google_tag_disabled = true;
	}

	// cache for 5 minutes if logged out and not in preview mode
	if (!user_is_logged_in() and !$preview_mode) {
		drupal_add_http_header('Cache-Control', 'public, max-age=300');
	}

	// js settings
	$js_setting = array(
		'tm_track_views' => array(
			'path' => 'api/track_views',
			'entity_id' => $list_id,
			'entity_type' => 'node',
		),
	);
	
	// insert tracking js
	if (module_exists('tm_track_views')) {
		drupal_add_js($js_setting, 'setting');
		drupal_add_js(drupal_get_path('module', 'tm_track_views') . '/js/tm_track_views.js');
	}

	// allow 3rd party pageviews
	$pageviews = "";
	if (!$preview_mode) {
		if (isset($conf["tm_lists_pageviews_script"])) {
			$pageviews_incr = 100000 + $list_id; // so we don't collide with wp post ids
			$pageviews_script = $conf["tm_lists_pageviews_script"];
			$pageviews_script = str_replace("__INCR__", $pageviews_incr , $pageviews_script);
			drupal_add_js($pageviews_script, array('type' => 'inline', 'scope' => 'footer'));
			$pageviews = $conf["tm_lists_pageviews_placeholder"];
			$pageviews = str_replace("__INCR__", $pageviews_incr, $pageviews);
		}
	}
	
	// load templates
	$template = file_get_contents(drupal_get_path('module', 'tm_lists') .'/templates/tm_list_view.tpl.php');
	$pager_template = "";

	// get marked up list title
	$list_title = tm_lists_get_display_list_title($list);

	// get marked up list description
	$list_description = tm_lists_get_display_list_description($list);

	// if user can edit the list, add the moderator menu
	$moderator_menu = "";
	$user_can_edit_list = false;
	if (tm_lists_user_can_edit_list($user, $list_id)) {
		$user_can_edit_list = true;
		$moderator_menu = file_get_contents(drupal_get_path('module', 'tm_lists') .'/templates/tm_list_view_menu.tpl.php');

		// track views
		$who_viewed = "";
		if (module_exists("tm_track_views")) {
			$max_view_days = $conf["tm_track_views_store_days"];
			$who_viewed = "<li><a href='/lists/__LIST_ID__/viewers'>Viewers (__NUM_VIEWERS__)</a></li>";
			$num_views = tm_track_views_how_many_viewed($list_id, "node", $user->uid, $max_view_days);
        	$who_viewed = str_replace("__NUM_VIEWERS__", $num_views, $who_viewed);
		}
		$moderator_menu = str_replace("__WHO_VIEWED__", $who_viewed, $moderator_menu);
		
		// send notifications
		$send_notifications_menu = "";
		$unsent_items = tm_lists_get_list_unsent_notification_items($list_id);
		$unsent_notifications = sizeof($unsent_items);
		if ($unsent_notifications > 0) {
			$send_notifications_menu = "<li><a href='javascript:void();' onClick='tm_lists_send_unsent_notifications(" . $list_id . ", " . $unsent_notifications . ");'> Notifications (". $unsent_notifications . ")</a></li>";
		}
		
		$moderator_menu = str_replace("__SEND_NOTIFICATIONS__", $send_notifications_menu, $moderator_menu);

		// show google analytics link
		$google_analytics_report_menu = "";
		if (in_array("access-google-analytics-report", $user->roles)) {
			$list_path = tm_lists_get_list_url($list);
			$google_analytics_report_link = tm_fields_google_analytics_report_link($list_path);
			$google_analytics_report_menu = "<li><a target='_blank' href='" . $google_analytics_report_link . "'>View analytics</a></li>";
		}
		$moderator_menu = str_replace("__VIEW__ANALYTICS__", $google_analytics_report_menu, $moderator_menu);

		// clear cache
		$send_notifications_menu = "<li><a href='javascript:void();' onClick='tm_lists_send_unsent_notifications(" . $list_id . ", " . $unsent_notifications . ");'> Notifications (". $unsent_notifications . ")</a></li>";

	}

	// get list contributors
	$contributor_uids = tm_lists_get_list_contributors($list_id);
	$show_contributor_uids = array_slice($contributor_uids, 0, 5); // show first five
	$contributor_links = array();
	foreach ($show_contributor_uids as $contributor_uid) {
		$contributor = user_load($contributor_uid);
		if ($contributor != null) {
			$first_name = strip_tags($contributor->field_user_first_name[LANGUAGE_NONE][0]['value']);
			$last_name = strip_tags($contributor->field_user_last_name[LANGUAGE_NONE][0]['value']);
			$full_name = $first_name . " " . $last_name;
			$full_name = str_replace("'", "%27", $full_name);
			$contributor_url = drupal_get_path_alias("user/" . $contributor_uid);
			$avatar_url = image_style_url("avatar", _tm_users_get_avatar_uri($contributor));
			$img_url = str_replace("'", "%27", file_create_url($avatar_url));
			$contributor_links[] = "<a target='_blank' href='/" . $contributor_url . "'><img class='tm-list-contributor-avatar' alt='" . t($full_name) . "' title='" . t($full_name) . "' src='" . $img_url . "'></a>";
		}
	}

	// contributor html
	$contributor_html = "";
	if (sizeof($contributor_links) > 0) {
		$contributor_html = "Created by &nbsp;";
		$contributor_html .= implode(" ", $contributor_links);
	}
	if (sizeof($contributor_links) < sizeof($contributor_uids)) {
		$contributor_html .= " and " . (sizeof($contributor_uids) - (sizeof($contributor_links))) . " others";
	}

	// last updated
	$last_updated = "";
	$last_timestamp = tm_lists_get_latest_timestamp($list_id);
	if ($last_timestamp != null) {
		$difference = time() - $last_timestamp;
		$update_time = format_interval($difference, 1);
		$update_time = str_replace("sec", "seconds", $update_time);
		$update_time = str_replace("min", "minutes", $update_time);
		$last_updated = "Updated " . $update_time . " ago";
	}

	// replace fields
	$template = str_replace("__LIST_TITLE__", strip_tags($list_title), $template);
	$template = str_replace("__LIST_DESCRIPTION__", $list_description, $template);
	$template = str_replace("__PAGEVIEWS__", $pageviews, $template);
	$template = str_replace("__MODERATOR_MENU__", $moderator_menu, $template);
	$template = str_replace("__LIST_ID__", $list_id, $template);
	$template = str_replace("__LIST_CONTRIBUTORS__", $contributor_html, $template);
	$template = str_replace("__LAST_UPDATED__", $last_updated, $template);

	// total items
	$total_items = tm_lists_get_num_items($list_id);

	// get items from list
	$per_page = $conf["tm_lists_per_page"];
	$limit_to = $per_page;

	// if preview mode and list editor, show all items
	if ($preview_mode and $user_can_edit_list) {
		$limit_to = $total_items;
	}

	// fetch items
	$items = tm_lists_get_items_by_id($list_id, $sort_method, 0, $limit_to);

	// get list options
	$show_item_links = array();
	foreach ($list->field_list_display_links[LANGUAGE_NONE] as $field) {
		$show_item_links[] = $field['value'];
	}
	$item_link_nofollow = $list->field_list_nofollow_links[LANGUAGE_NONE][0]['value'];
	$hide_item_position = $list->field_list_hide_rankings[LANGUAGE_NONE][0]['value'];

	// description character limit
	$character_limit = 250; // default
	if (isset($list->field_list_item_description_char[LANGUAGE_NONE][0]['value'])) {
		$character_limit = $list->field_list_item_description_char[LANGUAGE_NONE][0]['value'];
	}

	// render list
	$items_html = tm_lists_render_list_items_html($list_id, $items, $total_items, $sort_method, 1, 1, $hide_item_position, $show_item_links, $item_link_nofollow, $character_limit);

	// show pager if more items to display
	if ($total_items > $per_page) {
		$pager_template = file_get_contents(drupal_get_path('module', 'tm_lists') .'/templates/tm_list_view_pager.tpl.php');
	}
	
	// no items
	if ($total_items == 0) {
		$items_html .= "<div class='tm-lists-no-items'><p>Looks like this is a new list!</p></div>";
	}

	// add footer if end of list
	if (($total_items <= $per_page) or ($preview_mode and $user_can_edit_list)) {
		$items_html .= $conf["tm_lists_footer_html"];
	}

	// if preview mode and list editor, hide pager
	if ($preview_mode and $user_can_edit_list) {
		$pager_template = "";
	}

	$template = str_replace("__LIST_PAGER__", $pager_template, $template);
	$template = str_replace("__LIST_ITEMS__", $items_html, $template);

	// set page title
	drupal_set_title($list_title);

	// create form
	$form['html'] = array(
    	'#type' => 'item',
    	'#markup' => $template,
  	);
  	return $form;

}

/* Render html results */
function tm_lists_render_list_items_html($list_id, $items, $total_items, $sort_method, $count, $page, $hide_item_position = false, $show_item_links = array("profile", "website"), $item_link_nofollow = true, $character_limit = 250) {

	global $conf;
	global $user;

	// check if user can perform action on list
	$user_can_perform_list_action = false;
	if (tm_lists_user_can_add_to_list($user, $list_id)) {
		$user_can_perform_list_action = true;
	}
	if (tm_lists_user_can_remove_from_list($user, $list_id)) {
		$user_can_perform_list_action = true;
	}

	// include tm_lists_actions.js if user can perform actions
	if ($user_can_perform_list_action) {
		drupal_add_js(drupal_get_path('module', 'tm_lists') . '/js/tm_lists_actions.js');
	}

	// preload sort flags
	if ($sort_method == "flag_count") {
		$order_flags = array();
		$order_flags['follow_members'] = flag_get_flag('follow_members');
		$order_flags['follow_organizations'] = flag_get_flag('follow_organizations');
		$order_flags['signup'] = flag_get_flag('signup');
		$order_flags['event_register'] = flag_get_flag('event_register');
	}

	// get status of preview
	$preview_mode = tm_lists_is_list_preview_mode($list_id);

	// load item template
	$item_template = file_get_contents(drupal_get_path('module', 'tm_lists') .'/templates/tm_list_view_item.tpl.php');

	// list url
	$list_node = node_load($list_id);
	$list_url = tm_lists_get_list_url($list_node);

	// render list items
	$items_html = "";
	$entity_type = "node";
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

			// User image
			$image_class = "avatar";
			$image_uri = _tm_users_get_avatar_uri($entity);
			$image_url = image_style_url("avatar", $image_uri);

			// Get user name
			$first_name = strip_tags($entity->field_user_first_name[LANGUAGE_NONE][0]['value']);
			$last_name = strip_tags($entity->field_user_last_name[LANGUAGE_NONE][0]['value']);
			$full_name = $first_name . " " . $last_name;
			$item_title = $full_name;

			// About me
			$item_description = "";
			if (isset($entity->field_about_me[LANGUAGE_NONE][0]['value'])) {
				$item_description = $entity->field_about_me[LANGUAGE_NONE][0]['value'];
			}

		}

		// node fields
		if ($item->entity_type == 2) {

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
				$image_uri = _tm_events_get_default_image($image_uri, $entity->nid);
				if (isset($entity->field_image[LANGUAGE_NONE][0]['uri'])) {
					$image_uri = $entity->field_image[LANGUAGE_NONE][0]['uri'];
				}
				$image_url = image_style_url("banner", $image_uri);
			}

			$item_title = $entity->title;
			$item_description = $entity->body[LANGUAGE_NONE][0]['value'];
		}

		// item position
		$item_position = "";
		if (!$hide_item_position) {
			$item_position = "<span class='tm-list-view-item-position'>" . $count . "</span>";
		}

		// anchor tag
		// $item_anchor_tag = preg_replace('/[^a-zA-Z0-9]/', '', $item_title);
		$item_anchor_tag = "feature-" . $item->entity_type . $item->entity_id;

		// actions menu
		$item_menu = "";
		if ($user_can_perform_list_action) {

			$item_menu = "<div class='tm-list-item-actions-container'>";
			$item_menu .= "<ul><li class='tm-list-item-actions-dropdown' data-dropd-wrapper>";
			$item_menu .= "<a class='tm-list-item-actions' href='javascript:void(0);' data-dropd-toggle=''></a>";

			// more-actions
			$item_menu .= "<div class='inner dropd dropd-s dropd-right' data-dropd='' style='top: 24px; min-width: 8rem;'>";

			// dropd-menu
			$item_menu .= "<ul class='tm-list-item-actions-dropdown-menu dropd-menu'>";
			
			// who added
			$who_added = user_load($item->uid);
			if ($who_added != null) {
				$who_added_first_name = strip_tags($who_added->field_user_first_name[LANGUAGE_NONE][0]['value']);
				$who_added_last_name = strip_tags($who_added->field_user_last_name[LANGUAGE_NONE][0]['value']);
				$who_added_full_name = $who_added_first_name . " " . $who_added_last_name;
				$who_added_url = drupal_get_path_alias("user/" . $item->uid);
				$item_menu .= "<li style='white-space: nowrap;'><a href='/" . $who_added_url . "'>Added by " . $who_added_full_name . "</a></li>";
			}

			// boost item
			if ($sort_method == "sort_order") {
				if (tm_lists_user_can_add_to_list($user, $list_id)) {
					$item_menu .= "<li><a style='white-space: nowrap;' href='javascript:void(0);' onClick='tm_lists_position_item(\"" . $entity_type . "\", " . $item->entity_id . "," . $list_id . ", \"" . strip_tags(htmlentities($item_title, ENT_QUOTES)) . "\", " . $total_items . "," . $count . ");'>Change position</a></li>";
				}
			}

			// edit items
			$show_edit_item = false;
			if (tm_lists_user_can_remove_from_list($user, $list_id)) {
				$show_edit_item = true;
			} elseif (tm_lists_user_can_add_to_list($user, $list_id) and ($user->uid == $item->uid)) {
				$show_edit_item = true; // can edit own item
			}

			// edit item
			if ($show_edit_item) {
				$item_menu .= "<li><a href='javascript:void(0);' onClick='tm_lists_edit_form(\"" . $entity_type . "\", " . $item->entity_id . "," . $list_id . "," . $character_limit . ");'>Edit</a></li>";
			}

			// remove
			if (tm_lists_user_can_remove_from_list($user, $list_id)) {

				$item_menu .= "<li><a href='javascript:void(0);' onClick='tm_remove_item_from_list(\"" . $entity_type . "\", " . $item->entity_id . "," . $list_id . ", \"" . strip_tags(htmlentities($item_title, ENT_QUOTES)) . "\");'>Remove</a></li>";
			}

			// item link
			$item_menu .= "<li><a href='/" . $list_url . "/" . $item_anchor_tag . "'>Link to item</a></li>";

			$item_menu .= "</ul>";
			$item_menu .= "</div>"; // more-actions
			$item_menu .= "</li></ul>"; // data-dropd-wrapper
			$item_menu .= "</div>";
		}

		// everyone else who is logged in
		if (!$user_can_perform_list_action and (user_is_logged_in())) {
			
			$item_menu = "<div class='tm-list-item-actions-container'>";
			$item_menu .= "<ul><li class='tm-list-item-actions-dropdown' data-dropd-wrapper>";
			$item_menu .= "<a class='tm-list-item-actions' href='javascript:void(0);' data-dropd-toggle=''></a>";

			// more-actions
			$item_menu .= "<div class='inner dropd dropd-s dropd-right' data-dropd='' style='top: 24px; min-width: 8rem;'>";

			// dropd-menu
			$item_menu .= "<ul class='tm-list-item-actions-dropdown-menu dropd-menu'>";
			
			// who added
			$who_added = user_load($item->uid);
			if ($who_added != null) {
				$who_added_first_name = strip_tags($who_added->field_user_first_name[LANGUAGE_NONE][0]['value']);
				$who_added_last_name = strip_tags($who_added->field_user_last_name[LANGUAGE_NONE][0]['value']);
				$who_added_full_name = $who_added_first_name . " " . $who_added_last_name;
				$who_added_url = drupal_get_path_alias("user/" . $item->uid);
				$item_menu .= "<li style='white-space: nowrap;'><a href='/" . $who_added_url . "'>Added by " . $who_added_full_name . "</a></li>";
			}

			// item link
			$item_menu .= "<li><a href='/" . $list_url . "/" . $item_anchor_tag . "'>Link to item</a></li>";

			$item_menu .= "</ul>";
			$item_menu .= "</div>"; // more-actions
			$item_menu .= "</li></ul>"; // data-dropd-wrapper
			$item_menu .= "</div>";
		}

		// link target and rel
		$rel = "nofollow noopener"; // default
		$link_target = $conf["tm_lists_link_target"];
		if (!$item_link_nofollow) {
			$rel = "noopener";
		}

		// get comment
		$comment = $item->comment;

		// if comment is empty, use first sentence of entity description
		if ($item->comment == "") {
			$description_parts = explode(".", $item_description);
			$comment = $description_parts[0];
			if (sizeof($description_parts) > 1) {
				$comment .= ".";
			}
		}

		// format item comment
		$comment = strip_tags($comment);

		// autoembed youtube, vimeo, soundcloud
		if ($conf["tm_lists_oembed_enabled"]) {

			$autoembed = new AutoEmbed();
			$autoembed->setListId($list_id); // set list id for cache
	        $comment = $autoembed->parse($comment);

	        // protect autoembed tags from parsedown
	        $comment = str_replace("https://", "AUTOEMBED_HTTPS", $comment);
	        $comment = str_replace("http://", "AUTOEMBED_HTTP", $comment);
	        $comment = str_replace("<iframe", "AUTOEMBED_OPENIFRAME", $comment);
	        $comment = str_replace("</iframe>", "AUTOEMBED_CLOSEIFRAME", $comment);
		}
		
        // parse markdown
		$comment = tm_base_parsedown($comment, $link_target, true);

		// protect autoembed tags from parsedown
		if ($conf["tm_lists_oembed_enabled"]) {
			$comment = str_replace("AUTOEMBED_HTTPS", "https://", $comment);
			$comment = str_replace("AUTOEMBED_HTTP", "http://", $comment);
			$comment = str_replace("AUTOEMBED_OPENIFRAME", "<iframe", $comment);
	        $comment = str_replace("AUTOEMBED_CLOSEIFRAME", "</iframe>", $comment);
	    }

		$comment = tm_base_linkify($comment, ["http", "https", "mail", "twitter"], ["rel" => $rel, "target" => $link_target]); // linkify
		$comment = str_replace("[##]", $count, $comment); // in case you want to put ## at start of line
		$comment = str_replace("##", $count, $comment); // replace ## with position

		// profile url
		$item_url = "/" . drupal_get_path_alias($entity_type . "/" . $item->entity_id);

		// add links
		$item_links_html = "";

		// get entity links
		$links = tm_lists_get_entity_link_fields($entity, $show_item_links);

		// always show profile
		$link_url = "/" . drupal_get_path_alias($entity_type . "/" . $item->entity_id);
		$link_label = "View Profile";
		if (isset($entity->type)) {
			if ($entity->type == "event") {
			$link_label = "View Event";
			}
			if ($entity->type == "chapter") {
				$link_label = "View Chapter";
			}
		}

		$item_links_html = "<span class='tm-list-view-item-link view-profile'><a class='tm-list-item-link-profile' " . $link_target . " rel='" . $rel . "' href='" . $link_url . "'>" . $link_label . "</a></span>";

		// show other links
		// can be: 'website', 'twitter', 'facebook', 'linkedin', 'instagram', 'youtube', 'vimeo', 'snapchat', 'tiktok'
		$link_labels = array('website' => "Website", 'twitter' => "Twitter", 'facebook' => "Facebook", 'linkedin' => "LinkedIn", 'instagram' => "Instagram", 'youtube' => "YouTube", 'vimeo' => "Vimeo", 'snapchat' => "Snapchat", 'tiktok' => "TikTok");

		foreach ($links as $field => $link) {

			// create html
			$link_url = $link;
			$link_label = $link_labels[$field];
			$link_url = str_replace("'", "%27", $link_url);
			$rel = "nofollow noopener";

			// only add nofollow for website field
			if ((!$item_link_nofollow) and ($field == "website")) {
				$rel = "noopener";
			}

			// link text
			$link_text = "";
			if ($field == "website") {
				$link_label = $link_url;
				$link_label = str_ireplace("https://", "", $link_label);
				$link_label = str_ireplace("http://", "", $link_label);
				$parts = explode("/", $link_label);
				$link_label = $parts[0];
				$parts = explode("?", $link_label);
				$link_label = $parts[0];
				$parts = explode("&", $link_label);
				$link_label = $parts[0];
				$link_label = str_ireplace("www.", "", $link_label);
			}

			$item_links_html .= "<span class='tm-list-view-item-link'><a class='tm-list-item-link-" . $field . "' " . $link_target . " rel='" . $rel . "' href='" . $link_url . "'>" . $link_label . "</a></span>";
		}

		// item flag count
		$link_url = "/" . drupal_get_path_alias($entity_type . "/" . $item->entity_id);
		$item_score_html = "";
		if ($sort_method == "flag_count") {
			$score = tm_lists_get_entity_score($item->entity_type, $entity, $order_flags);
			if ($score != "") {
				$item_score_html = "<a " . $link_target . " rel='" . $rel . "' href='" . $link_url . "'><span class='tm-list-view-item-score tm_stats tm_stats_followers'>" . $score . "</span></a>";
			}
		}

		// replace fields
		$item_html = $item_template;
		$item_html = str_replace("__ITEM_POSITION__", $item_position, $item_html);
		$item_html = str_replace("__ITEM_IMAGE_CLASS__", $image_class, $item_html);
		$item_html = str_replace("__ITEM_IMAGE_URL__", $image_url, $item_html);
		$item_html = str_replace("__ITEM_TITLE__", $item_title, $item_html);
		$item_html = str_replace("__ITEM_COMMENT__", $comment, $item_html);
		$item_html = str_replace("__ITEM_MENU__", $item_menu, $item_html);
		$item_html = str_replace("__PROFILE_URL__", $item_url, $item_html);
		$item_html = str_replace("__ITEM_LINKS__", $item_links_html, $item_html);
		$item_html = str_replace("__ITEM_SCORE__", $item_score_html, $item_html);
		$items_html .= "<li class='list-item list-page-" . $page . "'>" . $item_html . "</li>";

		$count++;
	}

	return $items_html;
}

/* Return html search results for search page */
function tm_lists_display_list_ajax() {

	global $user;
	global $conf;

	// cache for 5 minutes if logged out
	if (!user_is_logged_in()) {
		drupal_add_http_header('Cache-Control', 'public, max-age=300');
	}

	// Step 1. Check input
	if (!isset($_GET["list_id"])) {
		return;
	}
	$list_id = intval($_GET["list_id"]);
	if ($list_id <= 0) {
		return;
	}

	// Sort method
	if (!isset($_GET["sort_method"])) {
		return;
	}
	$sort_method = $_GET["sort_method"];

	$page = 1;
	if (isset($_GET["page"])) {
		$page = intval($_GET["page"]);
	}
	if ($page == 0) { 
		return;
	}

	// load list
	$list = node_load($list_id);
	if ($list == null) {
		print "Invalid list id";
		drupal_exit();
	}

	// calculate pagination
	$per_page = $conf["tm_lists_per_page"];
	$limit_from = ($page - 1) * $per_page;
	$limit_to = $per_page; // offset
	$items = tm_lists_get_items_by_id($list_id, $sort_method, $limit_from, $limit_to);

	// get list options
	$show_item_links = array();
	foreach ($list->field_list_display_links[LANGUAGE_NONE] as $field) {
		$show_item_links[] = $field['value'];
	}
	$item_link_nofollow = $list->field_list_nofollow_links[LANGUAGE_NONE][0]['value'];
	$hide_item_position = $list->field_list_hide_rankings[LANGUAGE_NONE][0]['value'];

	// total items
	$total_items = tm_lists_get_num_items($list_id);

	// description character limit
	$character_limit = 250; // default
	if (isset($list->field_list_item_description_char[LANGUAGE_NONE][0]['value'])) {
		$character_limit = $list->field_list_item_description_char[LANGUAGE_NONE][0]['value'];
	}
	
	// Render results
	$html = tm_lists_render_list_items_html($list_id, $items, $total_items, $sort_method, $limit_from + 1, $page, $hide_item_position, $show_item_links, $item_link_nofollow, $character_limit);

	// If end of list:
	// Hide load more
	// Show footer
	if ((sizeof($items) < $per_page) or ($total_items <= ($per_page * $page))) {
		$html .= "<script> document.getElementById('item-list-load-more').remove(); </script>";
		$html .= $conf["tm_lists_footer_html"];
	}	

	// Return html
	echo $html;
	drupal_exit();
}

/**
 * Get entity link fields
 */
function tm_lists_get_entity_link_fields($entity, $link_fields) {

	global $conf;

	// get entity fields
	$links = array();
	foreach ($link_fields as $field_name) {
		if ($field_name == "profile") {
			continue; // ignore profile url
		}
		$obj_field_name = "field_link_" . $field_name;
		if (isset($entity->$obj_field_name[LANGUAGE_NONE][0]['url'])) {
			$raw_link = strip_tags($entity->$obj_field_name[LANGUAGE_NONE][0]['url']);
			$validated_link = tm_lists_validate_link_url($raw_link, $field_name);
			if ($validated_link != null) {
				$links[$field_name] = $validated_link;
			}
		}
	}

	return $links;
}

/**
 * Return validated link url, if possible
 * Otherwise, return false
 */
function tm_lists_validate_link_url($url, $url_type) {

	// fix user typos
	$url = trim($url);
	$url = str_replace(" ", "", $url); // strip spaces
	$url = str_replace(",", ".", $url); // replace , with .
	$url = str_ireplace("http//", "http://", $url); // replace http// with http:// in url
	$url = str_ireplace("https//", "http://", $url); // replace https// with https:// in url

	// @twitter
	if ($url_type == "twitter") {
		if ((strpos($url, "@") == 0) && (strpos($url, "@") !== FALSE)) {
			$url = "https://twitter.com/" . str_replace("@", "", $url);
		}
		if (((strpos($url, ".")) === FALSE) and ((strpos($url, "/") === FALSE))) {
			$url = "https://twitter.com/" . $url;
		}
	}

	// @instagram
	if ($url_type == "instagram") {
		if ((strpos($url, "@") == 0) && (strpos($url, "@") !== FALSE)) {
			$url = "https://instagram.com/" . str_replace("@", "", $url);
		}
		if (((strpos($url, ".")) === FALSE) and ((strpos($url, "/") === FALSE))) {
			$url = "https://instagram.com/" . $url;
		}
	}

	// vimeo
	if ($url_type == "vimeo") {
		if (((strpos($url, ".")) === FALSE) and ((strpos($url, "/") === FALSE))) {
			$url = "https://vimeo.com/" . $url;
		}
	}	

	// snapchat
	if ($url_type == "snapchat") {
		if (((strpos($url, ".")) === FALSE) and ((strpos($url, "/") === FALSE))) {
			$url = "https://www.snapchat.com/add/" . $url;
		}
	}	

	// tiktok
	if ($url_type == "tiktok") {
		if (((strpos($url, ".")) === FALSE) and ((strpos($url, "/") === FALSE))) {
			$url = str_replace("@", "", $url);
			$url = "https://tiktok.com/@" . $url;
		}
	}	

	// encode url and validate
	// filter_var() only support ascii, so convert
	$path = parse_url($url, PHP_URL_PATH);
    $encoded_path = array_map('urlencode', explode('/', $path));
    $encoded_url = str_replace($path, implode('/', $encoded_path), $url);

    // check encoded url
    if (filter_var($encoded_url, FILTER_VALIDATE_URL)) {
    	return $url;
    }

    // not found
	return null;
}

/* Display entity score */
function tm_lists_get_entity_score($entity_type, $entity, $flags) {

	global $conf;
	$count = 0;

	// user
	if ($entity_type == 1) {
		$count = $flags["follow_members"]->get_count($entity->uid);
		$label_single = "Follower";
		$label_plural = "Followers";
	}

	// node
	if ($entity_type == 2) {

		// chapter
		if ($entity->type == "chapter") {	
			$count = $flags["signup"]->get_count($entity->nid);
			$label_single = "Member";
			$label_plural = "Members";
		}

		// event
		if ($entity->type == "event") {
			$count = $flags["event_register"]->get_count($entity->nid);
			$label_single = "Attendee";
			$label_plural = "Attendees";
		}

		// organization
		if ($entity->type == "organization") {
			$count = $flags["follow_organizations"]->get_count($entity->nid);
			$label_single = "Attendee";
			$label_plural = "Attendees";

			// upvotes
			$display_upvotes = $conf["tm_organizations_upvote"];
			if ($display_upvotes) {
				$label_single = "Upvote";
				$label_plural = "Upvotes";
			}
		}
	}

	// create text
	$score = "";
	if ($count > 0) {
		if ($count == 1) {
			$score = $count . '<span class="tm_stats_followers_divider">|</span> ' . $label_single;
		} else {
			$score = $count . '<span class="tm_stats_followers_divider">|</span> ' . $label_plural;
		}	
	}

	// return score
	return $score;
}

/* Return html for a single list item */
function tm_lists_display_list_item_ajax($list_id, $entity_type, $entity_id) {

	// cache for 5 minutes if logged out
	if (!user_is_logged_in()) {
		drupal_add_http_header('Cache-Control', 'public, max-age=300');
	}

	// load list
	$list = node_load($list_id);
	if ($list == null) {
		print "<script>jQuery.prompt.close();</script>";
		drupal_exit();
	}

	// load template
	$template = file_get_contents(drupal_get_path('module', 'tm_lists') .'/templates/tm_list_feature_item.tpl.php');

	// get list options
	$show_item_links = array();
	foreach ($list->field_list_display_links[LANGUAGE_NONE] as $field) {
		$show_item_links[] = $field['value'];
	}
	$sort_method = $list->field_list_sort_by[LANGUAGE_NONE][0]['value'];
	$item_link_nofollow = $list->field_list_nofollow_links[LANGUAGE_NONE][0]['value'];
	$hide_item_position = $list->field_list_hide_rankings[LANGUAGE_NONE][0]['value'];

	$all_items = tm_lists_get_items_by_id($list_id, $sort_method, 0, 1000);
	$items = array();
	$count = 1;
	foreach ($all_items as $item) {
		if (($item->entity_id == $entity_id) and ($item->entity_type == $entity_type)) {
			$items[] = $item;
			break;
		}
		$count++;
	}

	// check we have an item
	if (sizeof($items) == 0) {
		print "<script>jQuery.prompt.close();</script>";
		drupal_exit();
	}

	// description character limit
	$character_limit = 250; // default
	if (isset($list->field_list_item_description_char[LANGUAGE_NONE][0]['value'])) {
		$character_limit = $list->field_list_item_description_char[LANGUAGE_NONE][0]['value'];
	}
	
	// Render results
	$item_html = tm_lists_render_list_items_html($list_id, $items, 1, $sort_method, $count, 0, $hide_item_position, $show_item_links, $item_link_nofollow, $character_limit);

	// Insert into template
	$html = str_replace("__LIST_ITEM__", $item_html, $template);

	// Return html
	echo $html;
	drupal_exit();
}

/**
 Featured item permalink redirect
 ie: /lists/13741/test-list/feature-213737 
 redirect to: /lists/13741/test-list#feature-213737
*/
function tm_lists_display_list_feature($list_id, $list_slug, $hash) {
	drupal_goto("/lists/" . $list_id . "/" . $list_slug, array("fragment" => $hash));
}
