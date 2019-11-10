<?php

// tm_lists.drush.inc - drush commands for list management

/**
 * Implements hook_drush_command().
 */
function tm_lists_drush_command() {

	$items['tm-lists-show-all'] = array(
		'description' => 'Show all lists.',
		'callback' => 'tm_lists_show_lists_drush'
	);

	$items['tm-lists-show-items'] = array(
		'description' => 'Show list items.',
		'callback' => 'tm_lists_show_items_drush',
		'options' => array(
			'list_id' => 'id of the list',
			'sort_method' => 'sort method',
		),
	);

	$items['tm-lists-add-to-list'] = array(
		'description' => 'Add an item to a list.',
		'callback' => 'tm_lists_add_to_list_drush',
		'options' => array(
			'uid' => 'uid who is adding the item',
			'list_id' => 'id of the list',
			'entity_type' => 'entity_type',
			'entity_id' => 'entity_id',
			'comment' => 'comment',
		),
	);

	$items['tm-lists-remove-item'] = array(
		'description' => 'Remove an item.',
		'callback' => 'tm_lists_remove_item_drush',
		'options' => array(
			'item_id' => 'id of the list item',
		),
	);

	$items['tm-lists-remove-list-items'] = array(
		'description' => 'Remove a list.',
		'callback' => 'tm_lists_remove_list_items_drush',
		'options' => array(
			'list_id' => 'id of the list',
		),
	);

	return $items;
}

function tm_lists_show_lists_drush() {

	$lists = tm_lists_get_lists();
	drush_print(sizeof($lists) . " lists:");
	foreach ($lists as $list) {
		$num_items = tm_lists_get_num_items($list['list_id']);
		drush_print($list['list_id'] . " (" . $num_items . " items) - " . $list['title']);
	}

	$account = user_load(10472);
	if (tm_lists_user_can_add_to_list($account, 13750)) {
		drush_print("permission allowed");
	}
}

function tm_lists_show_items_drush() {

	// get options
	$list_id = drush_get_option('list_id');
	$sort_method = drush_get_option('sort_method');

	// check options
	if (($list_id == "") or ($sort_method == ""))  {
		drush_print("Usage: drush tm-lists-show-items --list_id=100 --sort_method='new_items_first'\nSort methods: new_items_first, new_items_last, flag_count, random");
		return;
	}

	$items = tm_lists_get_items_by_id($list_id, $sort_method, 0, 1000);
	foreach ($items as $item) {
		print_r($item);
	}
}

function tm_lists_add_to_list_drush() {

	// get options
	$uid = drush_get_option('uid');
	$list_id = drush_get_option('list_id');
	$entity_type = drush_get_option('entity_type');
	$entity_id = drush_get_option('entity_id');
	$comment = drush_get_option('comment');

	// check options
	if (($uid == "") or ($list_id == "") or ($entity_type == "") or ($entity_id == "")) {
		drush_print("Usage: drush tm-lists-add-to-list --uid=123 --list_id=100 --entity_type=1 --entity_id=500 --comment='testing'");
		return;
	}
	
	// insert id
	$insert_id = tm_lists_add_to_list($uid, $entity_type, $entity_id, $list_id, $comment);
	if ($insert_id == -1) {
		drush_print("List item already exists.");
	} else {
		drush_print("List item inserted: " . $insert_id);
	}
}

function tm_lists_remove_item_drush() {

	// get options
	$item_id = drush_get_option('item_id');

	// check options
	if ($item_id == "")  {
		drush_print("Usage: drush tm-lists-remove-item --item_id=1");
		return;
	}

	// remove list item
	$item_deleted = tm_lists_remove_list_item($item_id);
	if ($item_deleted) {
		drush_print("List item " . $item_id . " removed.");
	} else {
		drush_print("List item " . $item_id . " does not exist.");
	}
}

function tm_lists_remove_list_items_drush() {

	// get options
	$list_id = drush_get_option('list_id');

	// check options
	if ($list_id == "")  {
		drush_print("Usage: drush tm-lists-remove-list-items --list_id=1");
		return;
	}

	// remove list
	$list_deleted = tm_lists_remove_list_items($list_id);
	if ($list_deleted) {
		drush_print("List " . $list_id . " items removed.");
	} else {
		drush_print("List " . $list_id . " does not exist.");
	}
}
