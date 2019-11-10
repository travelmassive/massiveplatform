<?php

// tm_lists.sitemap.inc

/**
 * XML sitemap
 */
function tm_lists_display_sitemap_xml() {

	global $conf;

	// get lists
	$lists = tm_lists_get_lists();

	//create your XML document, using the namespaces
	$urlset = new SimpleXMLElement('<?xml version="1.0" encoding="UTF-8"?><urlset xmlns="http://www.sitemaps.org/schemas/sitemap/0.9"/>');

	// generate items
	foreach ($lists as $list_item) {

		// list id
		$list_id = $list_item["list_id"];

		// load list node
		$list = node_load($list_id);
		if ($list == null) {
			continue;
		}

		// check preview mode
		$preview_mode = $list->field_list_preview_mode[LANGUAGE_NONE][0]['value'];
		if ($preview_mode == 1) {
			continue;
		}

		// add the page URL to the XML urlset
		$item_url = tm_lists_get_list_url($list, true); // get absolute url
		$url = $urlset->addChild('url');
		$url->addChild('loc', $item_url );

		
		// updated
		if ($list_item["updated"] != null) {
			$lastmod = date(DATE_W3C, $list_item["updated"]);
			$url->addChild('lastmod', $lastmod);
		}

		$url->addChild('changefreq', 'daily');  // weekly etc.
		$url->addChild('priority', '1.0');		
	}

	//add whitespaces to xml output (optional, of course)
	$dom = new DomDocument();
	$dom->loadXML($urlset->asXML());
	$dom->formatOutput = true;
	
	// set header
	drupal_add_http_header('Content-Type', 'text/xml; charset=utf-8');

	// output RSS
	echo $dom->saveXML();
	drupal_exit();

}
