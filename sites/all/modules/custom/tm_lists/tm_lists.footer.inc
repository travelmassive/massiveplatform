<?php

// tm_lists.footer.inc

// render recent lists in footer
function tm_lists_render_footer($num_items = 5, $use_cache = true) {

	global $conf;

	// check cache
	 if ($use_cache) {
		$cache = cache_get('tm-lists-footer', 'cache');
		if (!empty($cache)) {
			return $cache->data;
		}
	}

	$lists = tm_lists_get_lists();

	// load template
	$template = file_get_contents(drupal_get_path('module', 'tm_lists') .'/templates/tm_lists_footer.tpl.php');

	// footer title
	$template = str_replace("__FOOTER_TITLE__", $conf["tm_lists_footer_title"], $template);

	// generate lists
	$count = 1;
	$list_html = "<ul class='tm-lists-footer'>";
	foreach ($lists as $list_item) {

		// limit number of items
		if ($count > $num_items) { 
			continue;
		}

		// list id
		$list_id = $list_item["list_id"];

		// load list node
		$list = node_load($list_id);
		if ($list == null) {
			continue;
		}

		// don't show preview mode
		$preview_mode = $list->field_list_preview_mode[LANGUAGE_NONE][0]['value'];
		if ($preview_mode == 1) {
			continue;
		}

		// get list title
		$list_title = tm_lists_get_display_list_title($list);

		// get list url
		$list_url = tm_lists_get_list_url($list);

		// render list
		$list_html .= "<li><span class='tm-lists-footer-label'><a class='tm-lists-footer-label-link' href='/" . $list_url . "'>" . $list_title . "</a>";
		
		// update time
		$updated_time = "";
		$updated_timestamp = tm_lists_get_latest_timestamp($list_id);
		if ($updated_timestamp != null) {
			$difference = time() - $updated_timestamp; // $list_item["updated"];
	        $update_time = format_interval($difference, 1);
	        $update_time = str_replace("sec", "seconds", $update_time);
	        $update_time = str_replace("min", "minutes", $update_time);
	        $updated_time = "Updated " . $update_time . " ago";
		}
		
		$list_html .= "</span><span class='tm-lists-footer-details'>" . $updated_time . "</span></li>";

		$count++;
	}

	$list_html .= "</ul>";
	$html = str_replace("__LISTS__", $list_html, $template);

	// add to cache
	cache_set('tm-lists-footer', $html, 'cache', time() + 300); // 5 mins

	// return html
	return $html;

}
