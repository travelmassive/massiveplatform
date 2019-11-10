<?php

// tm_lists.helpers.inc

/**
 * List marked up title
 * 1. Replace ## with number of items in list
 */
function tm_lists_get_display_list_title($list) {

	$list_title = trim(strip_tags($list->title));
	$num_items = tm_lists_get_num_items($list->nid, 1);
	$list_title = str_replace("##", $num_items, $list_title);
	return $list_title;
}

/**
 * List marked up description
 * 1. Linkify
 * 2. Replace ## with number of items in list
 */
function tm_lists_get_display_list_description($list) {

	$list_description = "";
	if (isset($list->field_list_description[LANGUAGE_NONE][0]['value'])) {
		$list_description = trim($list->field_list_description[LANGUAGE_NONE][0]['value']);
	}

	// format with paragraphs
	$list_description = str_replace("\n", "</p>\n<p>", $list_description);
	$list_description = "<p>" . $list_description . "</p>";

	// replace ---- with hr
	$list_description = preg_replace("/[-]{4,10}/", "<div class='tm_field_hr'></div>", $list_description); 

	// linkify
	$list_description = tm_base_linkify($list_description, ["http", "https", "mail", "twitter"], ["rel" => "nofollow noopener", "target" => "_blank"]); // add links

	// add number of items
	$num_items = tm_lists_get_num_items($list->nid, 1);
	$list_description = str_replace("##", $num_items, $list_description);
	
	return $list_description;
}

/**
 * Check if user can add to list
 */
function tm_lists_user_can_add_to_lists($account) {

	// is moderator
	if (in_array("moderator", $account->roles)) {
		return true;
	}

	// is chapter leader
	if (in_array("chapter leader", $account->roles)) {
		return true;
	}

	// get list creators
	$list_creators = tm_lists_get_list_creators();
	if ($list_creators != null) {
		if (array_search($account->uid, $list_creators)) {
			return true;
		}
	}

	// can't add to list
	return false;
}

/**
 * Check all list creator uids
 */
function tm_lists_get_list_creators() {

	$query = "SELECT field_list_creators_target_id as uid from field_data_field_list_creators";
	$list_creators = db_query($query)->fetchAssoc();
	return $list_creators;
}

/**
 * Check if user can edit list
 * Moderator, or list-editor
 */
function tm_lists_user_can_edit_list($account, $list_id) {
	$list = node_load($list_id);
	if ($list == null) {
		return false;
	}
	return (node_access("update", $list, $account) === TRUE);
}


/**
 * Check if user can add to list
 */
function tm_lists_user_can_add_to_list($account, $list_id) {
	return tm_lists_user_check_permission($account, $list_id, "add");
}

/**
 * Check if user can remove from list
 */
function tm_lists_user_can_remove_from_list($account, $list_id) {
	return tm_lists_user_check_permission($account, $list_id, "remove");
}

/**
 * Check if user can remove from at least one existing list
 */
function tm_lists_user_can_remove_from_existing_list($account, $entity_type, $entity_id) {

	// get any lists this entity is part of
	$lists = tm_lists_get_entity_lists($entity_type, $entity_id);

	// find if there are any removal permissions in any lists
	foreach ($lists as $list) {
		if (tm_lists_user_can_remove_from_list($account, $list->list_id)) {
			return true;
		}
	}

	// no permissions
	return false;
}

/**
 * Check if user can add or remove from list
 */
function tm_lists_user_check_permission($account, $list_id, $permission = "add") {

	// is moderator
	if (in_array("moderator", $account->roles)) {
		return true;
	}

	// is list-editor
	if (in_array("list-editor", $account->roles)) {
		return true;
	}

	// get list
	$list = node_load($list_id);
	if ($list == null) {
		return false;
	}

	// get list roles
	$list_roles = array();

	// add
	if ($permission == "add") {
		$list_roles = $list->field_list_roles_add[LANGUAGE_NONE];
	}

	// remove
	if ($permission == "remove") {
		$list_roles = $list->field_list_roles_remove[LANGUAGE_NONE];
	}

	// check roles
	foreach ($list_roles as $role) {

		// list creators
		if ($role['value'] == "list_creators") {
			if (isset($list->field_list_creators[LANGUAGE_NONE])) {
				$list_creators = $list->field_list_creators[LANGUAGE_NONE];
				foreach ($list_creators as $list_creator) {
					if ($account->uid == $list_creator['target_id']) {
						return true;
					}
				}
			}
		}

		// chapter leaders
		if ($role['value'] == "chapter_leaders") {
			if (in_array("chapter leader", $account->roles)) {
				return true;
			}
		}
	}
	
	// not allowed
	return false;
}

/**
 * Helper method to get list URL
 */
function tm_lists_get_list_url($node, $full_url = false) {

	global $conf;

	$list_slug = $node->field_list_slug_url[LANGUAGE_NONE][0]['value'];
	$list_count = tm_lists_get_num_items($node->nid, 1);
	$list_slug = str_replace("##", $list_count, $list_slug);
	$list_url = "lists/" . $node->nid . "/" . $list_slug;

	if ($full_url) {
		$list_url = $conf["tm_email_server_http"] . "://" . $conf["tm_email_server_url_domain"] . "/" . $list_url;
	}

	return $list_url;
}
