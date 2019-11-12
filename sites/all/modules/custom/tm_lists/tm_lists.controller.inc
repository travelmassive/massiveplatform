<?php

// tm_lists.controller.inc - controller methods

/**
 * Get array of list ids and titles, ordered by when item was most recently added
 */
function tm_lists_get_lists() {

	// Fetch from tm_lists
	$query = "SELECT list_id, node.title AS title, max(timestamp) as timestamp FROM tm_lists
		LEFT JOIN node ON tm_lists.list_id = node.nid
		GROUP BY list_id ORDER BY timestamp DESC";
	$rows = db_query($query)->fetchAll();

	$nids = array();
	$lists = array();
	foreach ($rows as $row) {
		if ($row->title != "") {
			$nids[] = $row->list_id;
			$lists[] = array("list_id" => $row->list_id, "title" => $row->title, "updated" => $row->timestamp);
		}
	}

	// Fetch from nodes
	$query = "SELECT nid, title, changed FROM node WHERE type = 'list' AND nid NOT IN (" . implode(",", $nids) . ") ORDER BY changed DESC";

	if (sizeof($nids) == 0) {
		$query = "SELECT nid, title, changed FROM node WHERE type = 'list' ORDER BY changed DESC";
	}
	
	$rows = db_query($query)->fetchAll();
	foreach ($rows as $row) {
		$lists[] = array("list_id" => $row->nid, "title" => $row->title, "updated" => $row->changed);
	}

	return $lists;
}

/**
 * Get number of items in a list
 */
function tm_lists_get_num_items($list_id, $is_published = 1) {

	$query = "SELECT COUNT(*) AS total FROM tm_lists WHERE list_id = :list_id AND is_published = :is_published";
	$result = db_query($query, array(':list_id' => $list_id, ':is_published' => $is_published))->fetch();

	// return total
	return $result->total;
}

/**
 * Get maximum items
 */
function tm_lists_get_max_sort_order($list_id) {
	$query = "SELECT sort_order as sort_order FROM tm_lists WHERE list_id = :list_id ORDER BY sort_order DESC LIMIT 1";
	$result = db_query($query, array(':list_id' => $list_id))->fetch();

	// return total
	if (isset($result->sort_order)) {
		return $result->sort_order;
	}

	// no sort order
	return 0;
}

/**
 * Return any lists the entity is in
 */
function tm_lists_get_entity_lists($entity_type, $entity_id, $limit = 100) {

	$entity_type = intval($entity_type);
	$entity_id = intval($entity_id);
	$limit = intval($limit);

	$query = "SELECT DISTINCT(list_id) AS list_id, MAX(timestamp) as timestamp FROM tm_lists WHERE entity_type = " . $entity_type . " AND entity_id = " . $entity_id . " GROUP BY list_id ORDER BY timestamp DESC LIMIT " . $limit;
	$results = db_query($query)->fetchAll();

	return $results;
}

/**
 * Return any lists the user has added to
 */
function tm_lists_get_lists_user_added_to($user_id, $limit = 100) {

	$user_id = intval($user_id);
	$limit = intval($limit);

	$query = "SELECT DISTINCT(list_id) AS list_id, MAX(timestamp) as timestamp FROM tm_lists WHERE uid = " . $user_id . " GROUP BY list_id ORDER BY MAX(timestamp) DESC LIMIT " . $limit;
	$results = db_query($query)->fetchAll();

	return $results;
}

/**
 * Check if user has contributed to a list
 */
function tm_lists_has_user_added_to_list($user_id, $list_id) {

	$user_id = intval($user_id);
	$list_id = intval($list_id);

	$query = "SELECT COUNT(*) AS has_added FROM tm_lists WHERE uid = " . $user_id . " AND list_id = " . $list_id;
	$results = db_query($query)->fetchAll();
	if (sizeof($results) > 0) {
		return true;
	}

	// not an author
	return false;
}

/**
 * Check if a list item exists
 */
function tm_lists_item_exists($entity_type, $entity_id, $list_id) {

	$entity_type = intval($entity_type);
	$entity_id = intval($entity_id);
	$list_id = intval($list_id);

	$query = "SELECT COUNT(*) AS total FROM tm_lists WHERE entity_type = " . $entity_type . " AND entity_id = " . $entity_id . " AND list_id = " . $list_id;
	$result = db_query($query)->fetch();

	// return if item exists
	return ($result->total >= 1);
}

/**
 * Get a single item from a list
 */
function tm_lists_get_item($entity_type, $entity_id, $list_id) {

	$entity_type = intval($entity_type);
	$entity_id = intval($entity_id);
	$list_id = intval($list_id);

	$query = "SELECT * FROM tm_lists WHERE entity_type = " . $entity_type . " AND entity_id = " . $entity_id . " AND list_id = " . $list_id;
	$results = db_query($query)->fetchAll();

	// return if item exists
	if (isset($results[0])) {
		return $results[0];
	}

	return null;
}

/**
 * Get items in a list
 */
function tm_lists_get_items_by_id($list_id, $sort_method = "new_items_first", $limit_from = 0, $limit_to = 100) {

	// pagination
	$limit_from = intval($limit_from);
	$limit_to = intval($limit_to);

	// sort by new items first
	$results = array();
	if ($sort_method == "new_items_first") {
		$query = "SELECT * FROM tm_lists WHERE list_id = :list_id AND is_published = 1 ORDER BY timestamp DESC LIMIT " . $limit_from . ", " . $limit_to;
		$results = db_query($query, array(':list_id' => $list_id))->fetchAll();
	}

	// sort by new items last
	if ($sort_method == "new_items_last") {
		$query = "SELECT * FROM tm_lists WHERE list_id = :list_id AND is_published = 1 ORDER BY timestamp ASC LIMIT " . $limit_from . ", " . $limit_to;
		$results = db_query($query, array(':list_id' => $list_id))->fetchAll();
	}

	// sort by flag count
	if ($sort_method == "flag_count") {

		$order_flags = array();
		$order_flags[] = flag_get_flag('follow_members')->fid;
		$order_flags[] = flag_get_flag('follow_organizations')->fid;
		$order_flags[] = flag_get_flag('signup')->fid;
		$order_flags[] = flag_get_flag('event_register')->fid;

		
		// Do this in two steps
		// First we left join on flag_count and sort by highest flags 
		$query = "SELECT id, list_id, entity_id, entity_type_str, fid, MAX(count) AS max_count
		FROM (SELECT l2.id, l2.list_id, l2.entity_id, l2.entity_type_str, f.fid, f.count 
		FROM (SELECT l.id, l.entity_id, l.list_id, l.is_published, CASE WHEN l.entity_type = 1 THEN 'user' WHEN l.entity_type = 2 THEN 'node' END AS entity_type_str FROM tm_lists l) as l2
		LEFT JOIN flag_counts f
		ON f.entity_type = l2.entity_type_str AND f.entity_id = l2.entity_id 
		AND f.fid IN (" . implode(",", $order_flags) . ")
		WHERE l2.list_id = :list_id AND l2.is_published = 1) AS results
		GROUP by id, list_id, entity_id, entity_type_str, fid
		ORDER by max_count DESC, id DESC LIMIT " . $limit_from . ", " . $limit_to;	

		$sorted_results = db_query($query, array(':list_id' => $list_id))->fetchAll();
		$ids = array();
		foreach ($sorted_results as $result) {
			$ids[] = $result->id;
		}

		// Then we select from tm_lists with ids from first query
		// todo: optimise into single query
		if (sizeof($ids) == 0) {
			return array(); // no results
		}

		$query = "SELECT * FROM tm_lists WHERE id IN (" . implode(",", $ids) . ") ORDER BY FIELD(id, " . implode(",", $ids) . ")";
		$results = db_query($query)->fetchAll();

	}

	// sort by sort order
	if ($sort_method == "sort_order") {
		$query = "SELECT * FROM tm_lists WHERE list_id = :list_id AND is_published = 1 ORDER BY sort_order ASC, timestamp DESC LIMIT " . $limit_from . ", " . $limit_to;
		$results = db_query($query, array(':list_id' => $list_id))->fetchAll();
	}

	// sort randomly
	$random_seed = intval(time() / 120); // change random seed every two minutes 
	if ($sort_method == "random") {
		$query = "SELECT * FROM tm_lists WHERE list_id = :list_id AND is_published = 1 ORDER BY RAND(" . $random_seed . ") ASC LIMIT " . $limit_from . ", " . $limit_to;
		$results = db_query($query, array(':list_id' => $list_id))->fetchAll();
	}
	
	// no items
	if (sizeof($results) == 0) {
		return array();
	}

	// return items
	return $results;
}


/**
 * Add an item to a list
 */
function tm_lists_add_to_list($uid, $entity_type, $entity_id, $list_id, $comment, $sort_order = 1) {

	// prepare fields
	$update_fields = array();
	$update_fields['list_id'] = $list_id;
	$update_fields['entity_type'] = $entity_type;
	$update_fields['entity_id'] = $entity_id;
	$update_fields['uid'] = $uid;
	$update_fields['comment'] = trim(strip_tags($comment));
	$update_fields['timestamp'] = time();
	$update_fields['sort_order'] = $sort_order;
	$update_fields['is_published'] = 1;

	// check if list items already exists
	if (tm_lists_item_exists($entity_type, $entity_id, $list_id)) {
		return -1;
	}

	// Add item to db
	$insert_id = db_insert('tm_lists')->fields($update_fields)->execute();
	return $insert_id;

}

/**
 * Remove a list item
 */
function tm_lists_remove_list_item($item_id) {

	$item_id = intval($item_id);
	$query = "DELETE FROM tm_lists WHERE id = " . $item_id;
	$results = db_query($query);
	return 1; // todo: return status
}

/**
 * Remove a list
 */
function tm_lists_remove_list_items($list_id) {

	$item_id = intval($item_id);
	$query = "DELETE FROM tm_lists WHERE list_id = " . $list_id;
	$results = db_query($query);
	return 1; // todo: return status
}

/**
 * Edit list item comment
 */
function tm_lists_edit_comment($item_id, $comment) {

	try {
		$query = "UPDATE tm_lists SET 
			comment = :comment
			WHERE id = :item_id";
		db_query($query, 
		array(':comment' => $comment, 
			  ':item_id' => $item_id));
		return true;
	} catch (Exception $e) {
		return false;
	}
}

/**
 * Update list item sort order
 */
function tm_lists_update_sort_order($item_id, $sort_order) {

	$sort_order = intval($sort_order);

	try {
		$query = "UPDATE tm_lists SET 
			sort_order = :sort_order
			WHERE id = :item_id";
		db_query($query, 
		array(':sort_order' => $sort_order, 
			  ':item_id' => $item_id));
		return true;
	} catch (Exception $e) {
		return false;
	}
}

/**
 * Set item's sort order 1
 * Increment all other list item's sort order by 1
 */
function tm_lists_boost_item_sort_order($list_id, $item_id) {

	try {

		// set item's sort order to 1
		$query = "UPDATE tm_lists SET 
			sort_order = 1
			WHERE id = :item_id";
		db_query($query, 
		array(':item_id' => $item_id));

		// increment all other list items sort orders by 1
		$query = "UPDATE tm_lists SET 
			sort_order = sort_order + 1
			WHERE list_id = :list_id
			AND id != :item_id";
		db_query($query, 
		array(':list_id' => $list_id,
			':item_id' => $item_id));

		return true;

	} catch (Exception $e) {
		return false;
	}
}

/**
 * Update list item timestamp
 */
function tm_lists_update_timestamp($item_id) {

	try {
		$query = "UPDATE tm_lists SET 
			timestamp = :timestamp
			WHERE id = :item_id";
		db_query($query, 
		array(':timestamp' => time(), 
			  ':item_id' => $item_id));
		return true;
	} catch (Exception $e) {
		return false;
	}
}

/**
 * Update list item notification timestamp
 */
function tm_lists_update_timestamp_notified($item_id) {

	try {
		$query = "UPDATE tm_lists SET 
			timestamp_notified = :timestamp
			WHERE id = :item_id";
		db_query($query, 
		array(':timestamp' => time(), 
			  ':item_id' => $item_id));
		return true;
	} catch (Exception $e) {
		return false;
	}
}

/**
 * Get list contributors
 */
function tm_lists_get_list_contributors($list_id) {

	$list_id = intval($list_id);

	$query = "SELECT DISTINCT(uid) AS uid, MAX(timestamp) as timestamp FROM tm_lists WHERE list_id = " . $list_id . " GROUP BY uid ORDER BY timestamp DESC";
	$results = db_query($query)->fetchAll();

	// return items
	$contributor_uids = array();
	foreach ($results as $result) {
		$contributor_uids[] = $result->uid;
	}

	return $contributor_uids;
}

/**
 * Get when list last modified
 */
function tm_lists_get_latest_timestamp($list_id) {

	$list_id = intval($list_id);

	$query = "SELECT timestamp FROM tm_lists WHERE list_id = " . $list_id . " ORDER BY timestamp desc LIMIT 1";
	$results = db_query($query)->fetchAll();

	// return timestamp
	if (isset($results[0])) {
		return $results[0]->timestamp;
	}

	return null;
}

/**
 * Get list items that have not been notified
 */
function tm_lists_get_list_unsent_notification_items($list_id) {

	$query = "SELECT * FROM tm_lists WHERE list_id = :list_id AND timestamp_notified = 0";
	$results = db_query($query, array(':list_id' => $list_id))->fetchAll();

	// no items
	if (sizeof($results) == 0) {
		return array();
	}

	// return items
	return $results;
}

/**
 * Enumerate entity_type
 */
function tm_lists_enumerate_entity_type($entity_type) {

	// enumerate entity_type
	if ($entity_type == 'user') {
		return 1;
	}

	if ($entity_type == 'node') {
		return 2;
	}

	// invalid
	return 0;
}

