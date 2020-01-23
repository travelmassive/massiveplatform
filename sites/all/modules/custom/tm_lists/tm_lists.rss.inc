<?php

// tm_lists.rss.inc

/**
 * RSS view
 */
function tm_lists_display_list_rss($list_id) {

	global $conf;
	global $user;

	// cache for 5 minutes if logged out
	if (!user_is_logged_in()) {
		drupal_add_http_header('Cache-Control', 'public, max-age=300');
	}
	// load
	$list = node_load($list_id);

	// check valid list
	if ($list == null) {
		drupal_set_message(t('Sorry, the list you requested does not exist.'), 'help-tip');
		drupal_goto("/lists");
	}
	if ($list->type != "list") {
		drupal_set_message(t('Sorry, the list you requested does not exist.'), 'help-tip');
		drupal_goto("/lists");
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
	$list_description = strip_tags($list_description);
	
	// apply markdown, and remove
	$list_description = tm_base_parsedown($list_description, ""); // allow markdown
	$list_description = strip_tags($list_description); // remove markdown

	// apply counts
	$list_description = str_replace("[##]", $num_items, $list_description);
	$list_description = str_replace("##", $num_items, $list_description);

	// get items from list
	$items = tm_lists_get_items_by_id($list_id, $sort_method, 0, 100);

	// get list URL
	$list_url = tm_lists_get_list_url($list, true);

	// get list tiomestamp
	$list_timestamp = tm_lists_get_latest_timestamp($list_id);

	// base url
	$base_url = $conf["tm_email_server_http"] . "://" . $conf["tm_email_server_url_domain"];

	// create RSS
	$xml = new SimpleXMLElement('<rss version="2.0"></rss>');
	$xml->addChild('channel');
	$xml->channel->addChild('title', htmlentities(tm_lists_escape_xml($list_title)) . ' | ' . $conf["tm_site_name"]); 
	$xml->channel->addChild('link', $list_url);
	$xml->channel->addChild('description', htmlentities(tm_lists_escape_xml($list_description))); 
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
			$item_guid = "user-" . $entity->uid . '@' . $conf["tm_email_server_url_domain"];
			$item_link = $base_url . "/user/" . $entity->uid;
		}

		// node fields
		if ($item->entity_type == 2) {
			$item_guid = "node-" . $entity->nid . '@' . $conf["tm_email_server_url_domain"];
			$item_link = $base_url . "/node/" . $entity->nid;
			$item_title = $entity->title;
		}

		// comment
		$comment = strip_tags($item->comment);

		// apply markdown, and remove
		$comment = tm_base_parsedown($comment, ""); // allow markdown
		$comment = strip_tags($comment); // remove markdown

		// apply position
		$comment = str_replace("[##]", $num_items, $comment); // replace [##] with position
		$comment = str_replace("##", $num_items, $comment); // replace ## with position

		// create xml child
		$xml_item = $xml->channel->addChild('item'); 
		$xml_item->addChild('title', htmlentities(tm_lists_escape_xml($item_title))); 
		$xml_item->addChild('link', $item_link);
		$xml_item->addChild('description', htmlentities(tm_lists_escape_xml($comment))); 
		$guid = $xml_item->addChild('guid', $item_guid)->addAttribute('isPermaLink', "false"); 
		//$xml_item->addAttribute('isPermaLink', "false");
		$xml_item->addChild('pubDate', date(DATE_RSS, $item->timestamp));
	}

	$dom = new DOMDocument();
	$dom->preserveWhiteSpace = false;
	$dom->formatOutput = true;
	$dom->loadXML(html_entity_decode($xml->asXML()));
	
	// set header
	drupal_add_http_header('Content-Type', 'application/rss+xml;charset=utf-8');

	// output RSS
	echo $dom->saveXML();
	drupal_exit();

}

/*
* http://stackoverflow.com/questions/10276656/php-errors-parsing-xml-rss-feed
*/
function tm_lists_escape_xml($string) {

	// remove # for formatting
	$string = str_replace("###", "", $string);
	$string = str_replace("##", "", $string);
	$string = str_replace("# ", "", $string);

	return str_replace(array("&amp;", "&"), array("&", "&amp;"), $string);
}
