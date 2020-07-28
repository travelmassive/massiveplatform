<?php

// tm_organizations.footer.inc

// render recent organization follows / upvotes in footer
function tm_organizations_render_footer($num_items = 5, $use_cache = true) {

	global $conf;

	// check cache
	 if ($use_cache) {
		$cache = cache_get('tm-organizations-footer', 'cache');
		if (!empty($cache)) {
			return $cache->data;
		}
	}

	$num_days = $conf["tm_organizations_footer_num_days"];
	$exclude_items = $conf["tm_organizations_footer_exclude"];

	// get organizatins with recent follows, sorted by follower count
	$fetch_items = $num_items * 2;
	$recent_follows = tm_organizations_get_followed_organizations($fetch_items, $num_days, $exclude_items);

	// order list by title length
	if (isset($conf["tm_organizations_footer_order_length"])) {
		if ($conf["tm_organizations_footer_order_length"]) {

			// get node title lengths
			$sorted = array();
			foreach ($recent_follows as $item) {

				$node = node_load($item["entity_id"]);
				if ($node == null) {
					continue;
				}

				// add title to associative array
				$title = strip_tags(trim($node->title));
				$sorted[] = array("title" => $title, "entity_id" => $item["entity_id"], "follower_count" => $item["follower_count"]);
			}

			// sort by name length
			usort($sorted, function ($item1, $item2) {
				return (strlen($item1['title']) < strlen($item2['title']));
			});
			$recent_follows = $sorted;
		}
	}

	// load template
	$template = '<div class="tm-footer-lists-container organizations">
	<h2>__FOOTER_TITLE__</h2>
    __LIST_ITEMS__
</div>';

	// footer title
	$template = str_replace("__FOOTER_TITLE__", $conf["tm_organizations_footer_title"], $template);

	// generate lists
	$count = 1;
	$list_html = "<ul class='tm-footer-lists organizations'>";
	foreach ($recent_follows as $item) {

		if ($count > $num_items) {
			continue;
		}

		// get details
		$nid = $item["entity_id"];
		$follower_count = $item["follower_count"];
		$title = $item["title"];

		// skip all caps
		if ((strlen($title) > 5) and ctype_upper($title)) {
			continue;
		}

		// trim title
		if (isset($conf["tm_organizations_footer_max_length"])) {
			$max_length = $conf["tm_organizations_footer_max_length"];
			if (strlen($title) > $max_length) {
				$title = substr($title, 0, $max_length) . "...";
			}
		}
		
		// get company page url
		$profile_url = drupal_get_path_alias("node/" . $node->nid);

		// followers label
		$followers_label = "followers";
		if ($follower_count == 1) {
			$followers_label = "follower";
		}
		if (isset($conf["tm_organizations_upvote"])) {
			if ($conf["tm_organizations_upvote"]) {
				$followers_label = "upvotes";
				if ($follower_count == 1) {
					$followers_label = "upvote";
				}
			}
		}

		// render list
		$list_html .= "<li><span class='tm-footer-lists-label organizations'><a class='tm-footer-lists-label-link organizations' href='/" . $profile_url . "'>" . $title . "</a>";
				
		$list_html .= "</span><span class='tm-footer-lists-details organizations'>" . $follower_count . " " . $followers_label . "</span></li>";

		$count++;
	}

	$list_html .= "</ul>";
	$html = str_replace("__LIST_ITEMS__", $list_html, $template);

	// add to cache
	cache_set('tm-organizations-footer', $html, 'cache', time() + 1800); // 30 mins

	// return html
	return $html;

}

