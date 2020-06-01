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
	$recent_follows = tm_organizations_get_followed_organizations($num_items, $num_days, $exlude_items);

	// load template
	$template = '<div class="lists-footer-container organizations">
	<h2>__FOOTER_TITLE__</h2>
    __LIST_ITEMS__
</div>';

	// footer title
	$template = str_replace("__FOOTER_TITLE__", $conf["tm_subscriptions_user_subscribers_footer_title"], $template);

	// generate lists
	$list_html = "<ul class='tm-lists-footer organizations'>";
	foreach ($recent_follows as $item) {

		// get details
		$nid = $item["entity_id"];
		$follower_count = $item["follower_count"];

		// load account
		$node = node_load($nid);
		if ($node == null) {
			continue;
		}

		// get title
		$title = strip_tags(trim($node->title));

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
		$list_html .= "<li><span class='tm-lists-footer-label organizations'><a class='tm-lists-footer-label-link organizations' href='/" . $profile_url . "'>" . $title . "</a>";
				
		$list_html .= "</span><span class='tm-lists-footer-details organizations'>" . $follower_count . " " . $followers_label . "</span></li>";
	}

	$list_html .= "</ul>";
	$html = str_replace("__LIST_ITEMS__", $list_html, $template);

	// add to cache
	cache_set('tm-organizations-footer', $html, 'cache', time() + 1800); // 30 mins

	// return html
	return $html;

}
