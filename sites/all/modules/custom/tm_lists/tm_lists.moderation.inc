<?php

// tm_lists.moderation.inc

/**
 * Render html options list for moderation
 * examples:
 * /lists/moderation-list-ajax/user/10507/add
 * /lists/moderation-list-ajax/node/10507/add
 */
function tm_lists_moderation_lists_ajax() {

	global $user;
	$account = user_load($user->uid);

	// entity type
	$entity_type = arg(2);

	// entity id
	$entity_id = arg(3);
	$entity_type_id = tm_lists_enumerate_entity_type($entity_type);

	// moderation_mode - add or remove
	$moderation_mode = arg(4);
	if (($moderation_mode != "add") and ($moderation_mode != "remove")) {
		return;
	}

	// calculate events to display
	$lists = tm_lists_get_lists();

	// no results
	if (count($lists) == 0) {
		$html = "No lists <script>jQuery('.jqidefaultbutton').hide();</script>";
		print $html;
		return;
	}

	// generate html
	$html = "<select id='moderation_list_ids'>";
	foreach ($lists as $item_list) {

		// check permission
		if ($moderation_mode == "add") {
			if (!tm_lists_user_can_add_to_list($account, $item_list["list_id"])) {
				continue;
			}
		}

		// check permission
		if ($moderation_mode == "remove") {
			if (!tm_lists_user_can_remove_from_list($account, $item_list["list_id"])) {
				continue;
			}
		}
		
		// show event title
		$num_items = tm_lists_get_num_items($item_list["list_id"], 1);
		$list_title = str_replace("##", $num_items, $item_list["title"]);
		$list_label = strip_tags($list_title);

		// check preview mode
		$label_extra = "";
		$preview_mode = tm_lists_is_list_preview_mode($item_list["list_id"]);
		if ($preview_mode) {
			$label_extra = " (preview)";
		}

		// listed
		$item_is_listed = false;
		if (tm_lists_item_exists($entity_type_id, $entity_id, $item_list["list_id"])) {
			$item_is_listed = true;
			$label_extra = " (listed)";
		}

		if (($moderation_mode == "add") and ($item_is_listed)) { 
			$label_extras[] = "listed";
			$html .= "<option value='" . $item_list["list_id"] . "' disabled>" . $list_label . $label_extra . "</option>\n";
		} else {
			$html .= "<option value='" . $item_list["list_id"] . "'>" . $list_label . $label_extra . "</option>\n";
		}
	}

	print $html;

}

/**
 * Add an item to a list callback
 * examples:
 * lists/add/user/10507/123?comment=mycomment
 * lists/add/node/10507/123?comment=mycomment
 */
function tm_lists_moderation_add_callback($entity_type, $entity_id, $list_id) {

	global $user;
	$moderator = user_load($user->uid);

	$comment = null;
	if (isset($_GET["comment"])) {
		$comment = $_GET["comment"];
		$comment = str_replace("__NEWLINE__", "\n", $comment); // replace new lines
	}

	tm_lists_moderation_add($entity_type, $entity_id, $list_id, $moderator, $comment);
}

/**
 * Add item to list
 */
function tm_lists_moderation_add($entity_type, $entity_id, $list_id, $moderator, $comment, $is_console = false) {

	// check permission
	if (!tm_lists_user_can_add_to_list($moderator, $list_id)) {
		if ($is_console) { return false; }
		drupal_set_message(t('You don\'t have permission to perform this action.'), 'error');
		drupal_goto("/");
	}

	// check entity type
	if (($entity_type != "user") and ($entity_type != "node")) {
		if ($is_console) { return false; }
		drupal_set_message(t('Oops, an error occurred performing the action (invalid item type).'), 'error');
		drupal_goto("/" . drupal_get_path_alias($entity_type . "/" . $entity_id));
	}

	// enumerate entity type
	$entity_type_id = tm_lists_enumerate_entity_type($entity_type);

	// check we can load entity
	$entity = null;
	if ($entity_type == "user") {
		$entity = user_load($entity_id);
	} else {
		$entity = node_load($entity_id);
	}
	if ($entity == null) {
		if ($is_console) { return false; }
		drupal_set_message(t('Oops, an error occurred performing the action (invalid item)'), 'error');
		drupal_goto("/" . drupal_get_path_alias($entity_type . "/" . $entity_id));
	}

	// check list
	$item_list = node_load($list_id);
	if ($item_list == null) {
		if ($is_console) { return false; }
		drupal_set_message(t('Oops, an error occurred performing the action (invalid list)'), 'error');
		drupal_goto("/" . drupal_get_path_alias($entity_type . "/" . $entity_id));
	}

	// check item is not already added to the list
	if (tm_lists_item_exists($entity_type_id, $entity_id, $list_id)) {
		if ($is_console) { return false; }
		drupal_set_message(t('Oops, an error occurred performing the action (item already exists)'), 'error');
		drupal_goto("/" . drupal_get_path_alias($entity_type . "/" . $entity_id));
	}

	// add to bottom of sort order
	$sort_order = tm_lists_get_max_sort_order($list_id) + 10;

	// add to list
	$insert_id = tm_lists_add_to_list($moderator->uid, $entity_type_id, $entity_id, $list_id, $comment, $sort_order);
	if ($insert_id == -1) {
		if ($is_console) { return false; }
		drupal_set_message(t('Oops, an error occurred performing the action (database error)'), 'error');
		drupal_goto("/" . drupal_get_path_alias($entity_type . "/" . $entity_id));
	}

	// tm_add_to_list_id
	// tm_add_to_list_moderator_first_name
	// tm_add_to_list_moderator_first_name

	// tm_add_to_list_id
	$tm_add_to_list_id = &drupal_static("tm_add_to_list_id");
	$tm_add_to_list_id = $list_id;

	// moderator first name
	$moderator_first_name = strip_tags($moderator->field_user_first_name[LANGUAGE_NONE][0]['value']);
	$static_add_member_moderator_first_name = &drupal_static("tm_add_to_list_moderator_first_name");
	$static_add_member_moderator_first_name = $moderator_first_name;

	// moderator last name
	$moderator_last_name = strip_tags($moderator->field_user_last_name[LANGUAGE_NONE][0]['value']);
	$static_add_member_moderator_last_name = &drupal_static("tm_add_to_list_moderator_last_name");
	$static_add_member_moderator_last_name = $moderator_last_name;

	// notify member, or owners of company, event, or chapter leaders
	$preview_mode = $item_list->field_list_preview_mode[LANGUAGE_NONE][0]['value'];
	if (!$preview_mode) {
		if (module_exists('tm_notifications_lists')) {
			tm_lists_update_timestamp_notified($insert_id); // set notification timestamp
			tm_notifications_added_to_list($entity, $item_list);
		}
	}

	// notify list creators
	if (!$preview_mode) {
		if (module_exists('tm_notifications_lists')) {
			tm_notifications_added_to_list_notify_creators($entity, $item_list);
		}
	}

    // console return
    if ($is_console) { return true; }
    
	// get entity label
	if ($entity_type == "node") {
		$entity_label = $entity->title;
	} else {
		$entity_label = tm_users_get_full_name($entity_id);
	}

	// redirect
	$message = strip_tags($entity_label) . ' has been added to <a href="/lists/' . $list_id . '">' . tm_lists_get_display_list_title($item_list) . '</a>.';
	if ($preview_mode) {
		$message .= " Preview mode is on, so notifications are disabled.";
	}
    drupal_set_message($message);
    drupal_goto("/" . drupal_get_path_alias($entity_type . "/" . $entity_id));
}

/**
 * Remove an item to a list callback
 * examples:
 * lists/remove/user/10507/123
 * lists/remove/node/10507/123
 */
function tm_lists_moderation_remove_callback($entity_type, $entity_id, $list_id) {

	global $user;
	$moderator = user_load($user->uid);

	tm_lists_moderation_remove($entity_type, $entity_id, $list_id, $moderator);
}

/**
 * Remove item from list
 */
function tm_lists_moderation_remove($entity_type, $entity_id, $list_id, $moderator, $is_console = false) {

	// check permission
	if (!tm_lists_user_can_remove_from_list($moderator, $list_id)) {
		if ($is_console) { return false; }
		drupal_set_message(t('You don\'t have permission to perform this action.'), 'error');
		drupal_goto("/");
	}

	// check entity type
	if (($entity_type != "user") and ($entity_type != "node")) {
		if ($is_console) { return false; }
		drupal_set_message(t('Oops, an error occurred performing the action (invalid item type).'), 'error');
		drupal_goto("/" . drupal_get_path_alias($entity_type . "/" . $entity_id));
	}

	// enumerate entity type
	$entity_type_id = tm_lists_enumerate_entity_type($entity_type);

	// check we can load entity
	$entity = null;
	if ($entity_type == "user") {
		$entity = user_load($entity_id);
	} else {
		$entity = node_load($entity_id);
	}
	if ($entity == null) {
		if ($is_console) { return false; }
		drupal_set_message(t('Oops, an error occurred performing the action (invalid item)'), 'error');
		drupal_goto("/" . drupal_get_path_alias($entity_type . "/" . $entity_id));
	}

	// check list
	$item_list = node_load($list_id);
	if ($item_list == null) {
		if ($is_console) { return false; }
		drupal_set_message(t('Oops, an error occurred performing the action (invalid list)'), 'error');
		drupal_goto("/" . drupal_get_path_alias($entity_type . "/" . $entity_id));
	}

	// check item exists
	if (!tm_lists_item_exists($entity_type_id, $entity_id, $list_id)) {
		if ($is_console) { return false; }
		drupal_set_message(t('Oops, an error occurred performing the action (item does not exist)'), 'error');
		drupal_goto("/" . drupal_get_path_alias($entity_type . "/" . $entity_id));
	}

	// remove from list
	$item = tm_lists_get_item($entity_type_id, $entity_id, $list_id);
	$result = tm_lists_remove_list_item($item->id);
	if ($result == -1) {
		if ($is_console) { return false; }
		drupal_set_message(t('Oops, an error occurred performing the action (database error)'), 'error');
		drupal_goto("/" . drupal_get_path_alias($entity_type . "/" . $entity_id));
	}

    // console return
    if ($is_console) { return true; }
    
	// get entity label
	if ($entity_type == "node") {
		$entity_label = $entity->title;
	} else {
		$entity_label = tm_users_get_full_name($entity_id);
	}

	// redirect to list
	if (isset($_GET["redirect_to_list"])) {
		drupal_set_message(strip_tags($entity_label) . ' has been removed from this list.');
		drupal_goto("/" . drupal_get_path_alias("node/" . $list_id));
	}
   
   	// redirect to item
   	drupal_set_message(strip_tags($entity_label) . ' has been removed from <a href="/lists/' . $list_id . '">' . tm_lists_get_display_list_title($item_list) . '</a>.');
    drupal_goto("/" . drupal_get_path_alias($entity_type . "/" . $entity_id));
}

/**
 * Display edit form by ajax
 */
function tm_lists_moderation_edit_form_ajax($entity_type, $entity_id, $list_id) {

	global $conf;
	global $user;

	// check permission
	if (!tm_lists_user_can_add_to_list($user, $list_id)) {
		echo('You don\'t have permission to perform this action.');
		return;
	}

	// enumerate entity type
	$entity_type_id = tm_lists_enumerate_entity_type($entity_type);

	// check item
	$list_item = tm_lists_get_item($entity_type_id, $entity_id, $list_id);
	if ($list_item == null) {
		echo('Unable to retrieve list item.');
		return;
	}

	// if can't remove from list, only allow editing if it's your own item
	if (!tm_lists_user_can_remove_from_list($user, $list_id)) {
		if ($list_item->uid != $user->uid) {
			$ret = array("result" => false, "error_message" => "You can only edit your own list items", "redirect" => null);
			drupal_json_output($ret);
			return;
		}
	}

	// generate edit form
	$edit_form = "<textarea id='edit_list_item_comment' onkeyup='countChar(this);' value='' placeholder='Add a short description to be displayed on the list...' rows='4' cols='50' placeholder=''>" . htmlentities($list_item->comment) . "</textarea><script>";

	echo $edit_form;
	return;
}

/**
 * Handle edit item
 */
function tm_lists_moderation_edit_item_ajax($entity_type, $entity_id, $list_id) {

	global $conf;
	global $user;

	// check can add to list
	if (!tm_lists_user_can_add_to_list($user, $list_id)) {
		$ret = array("result" => false, "error_message" => "You don't have permission to perform this action", "redirect" => null);
		drupal_json_output($ret);
		return;
	}

	// enumerate entity type
	$entity_type_id = tm_lists_enumerate_entity_type($entity_type);

	// check item
	$list_item = tm_lists_get_item($entity_type_id, $entity_id, $list_id);
	if ($list_item == null) {
		$ret = array("result" => false, "error_message" => "Unable to retrieve list item", "redirect" => null);
		drupal_json_output($ret);
		return;
	}

	// if can't remove from list, only allow editing if it's your own item
	if (!tm_lists_user_can_remove_from_list($user, $list_id)) {
		if ($list_item->uid != $user->uid) {
			$ret = array("result" => false, "error_message" => "You can only edit your own list items", "redirect" => null);
			drupal_json_output($ret);
			return;
		}
	}

	// get comment
	$comment = "";
	if (isset($_POST["comment"])) {
		$comment = trim(strip_tags($_POST["comment"]));
	}

	// save comment
	if (tm_lists_edit_comment($list_item->id, $comment)) {
		$redirect = "/node/" . $list_item->list_id; // redirect to list 
		$ret = array("result" => true, "error_message" => null, "redirect" => $redirect);
		drupal_json_output($ret);
		return;
	}

	// error saving
	$ret = array("result" => false, "error_message" => "Error saving list item", "redirect" => null);
	drupal_json_output($ret);
	return;
}

/**
 * Boost list item
 */
function tm_lists_moderation_position_callback($entity_type, $entity_id, $list_id, $position = 1) {

	global $conf;
	global $user;

	// check permission
	if (!tm_lists_user_can_add_to_list($user, $list_id)) {
		drupal_set_message(t('You don\'t have permission to perform this action.'), 'error');
		drupal_goto("/node/" . $list_id); // redirect to list 
	}

	// enumerate entity type
	$entity_type_id = tm_lists_enumerate_entity_type($entity_type);

	// check item
	$list_item = tm_lists_get_item($entity_type_id, $entity_id, $list_id);
	if ($list_item == null) {
		drupal_set_message(t('Unable to retrieve list item.'), 'error');
		drupal_goto("/node/" . $list_id); // redirect to list 
	}

	// check we can load entity
	$entity = null;
	if ($entity_type == "user") {
		$entity = user_load($entity_id);
	} else {
		$entity = node_load($entity_id);
	}
	if ($entity == null) {
		drupal_set_message(t('Oops, an error occurred performing the action (invalid item)'), 'error');
		drupal_goto("/node/" . $list_id); // redirect to list 
	}

	// get entity label
	if ($entity_type == "node") {
		$entity_label = $entity->title;
	} else {
		$entity_label = tm_users_get_full_name($entity_id);
	}

	// move to sort_order
	if (tm_lists_update_list_item_position($list_id, $list_item->id, $position)) {
		drupal_set_message(strip_tags($entity_label) . ' has been moved to position ' . $position . ' of the list.');
		drupal_goto("/node/" . $list_item->list_id); // redirect to list 
	}

	// error boosting
	drupal_set_message(t('Unable to update order of list item.'), 'error');
	drupal_goto("/node/" . $list_id); // redirect to list 
}

/**
 * Send notifications for items that were added oin preview mode
 * Restrict to list editors
 */
function tm_lists_moderation_send_unsent_notifications($list_id) {

	global $user;

	// check permission
	if (!tm_lists_user_can_edit_list($user, $list_id)) {
		drupal_set_message(t('You don\'t have permission to perform this action.'), 'error');
		drupal_goto("/node/" . $list_id); // redirect to list 
	}

	// check notifications enabled
	if (!module_exists('tm_notifications_lists')) {
		drupal_set_message(t('Oops, an error occurred performing the action (tm_notifications_lists not enabled)'), 'error');
		drupal_goto("/node/" . $list_id); // redirect to list 
	}

	// check list
	$item_list = node_load($list_id);
	if ($item_list == null) {
		drupal_set_message(t('Oops, an error occurred performing the action (invalid list)'), 'error');
		drupal_goto("/node/" . $list_id); // redirect to list 
	}

	// check not preview mode
	$preview_mode = $item_list->field_list_preview_mode[LANGUAGE_NONE][0]['value'];
	if ($preview_mode == 1) {
		drupal_set_message(t('Oops, an error occurred performing the action (list in preview mode)'), 'error');
		drupal_goto("/node/" . $list_id); // redirect to list 
	}

	// get items that have not had notifications sent
	$unsent_items = tm_lists_get_list_unsent_notification_items($list_id);

	// tm_add_to_list_id
	$tm_add_to_list_id = &drupal_static("tm_add_to_list_id");
	$tm_add_to_list_id = $list_id;

	// send notifications
	$sent_notifications = 0;
	foreach ($unsent_items as $item) {

		// load entity
		$entity = null;
		if ($item->entity_type == 1) {
			$entity = user_load($item->entity_id);
		} else {
			$entity = node_load($item->entity_id);
		}

		// load moderator who added item to list
		$moderator = user_load($item->uid);
		if ($moderator == null) {
			continue;
		}

		// moderator first name
		$moderator_first_name = strip_tags($moderator->field_user_first_name[LANGUAGE_NONE][0]['value']);
		$static_add_member_moderator_first_name = &drupal_static("tm_add_to_list_moderator_first_name");
		$static_add_member_moderator_first_name = $moderator_first_name;

		// moderator last name
		$moderator_last_name = strip_tags($moderator->field_user_last_name[LANGUAGE_NONE][0]['value']);
		$static_add_member_moderator_last_name = &drupal_static("tm_add_to_list_moderator_last_name");
		$static_add_member_moderator_last_name = $moderator_last_name;

		// sent notification
		if ($entity != null) {
			tm_lists_update_timestamp_notified($item->id); // set notification timestamp
			tm_notifications_added_to_list($entity, $item_list);
			$sent_notifications++;
		}
	}

	// sent
	drupal_set_message('Sent ' . $sent_notifications . ' notifications to item owners.');
	drupal_goto("/lists/" . $list_id); // redirect to list 
}
