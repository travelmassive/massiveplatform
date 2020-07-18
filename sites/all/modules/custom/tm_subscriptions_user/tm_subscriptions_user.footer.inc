<?php

// tm_subscriptions_user.footer.inc

// render recent subscriptions in footer
function tm_subscriptions_user_render_footer($num_items = 5, $use_cache = true) {

	global $conf;

	// check cache
	 if ($use_cache) {
		$cache = cache_get('tm-subscriptions-footer', 'cache');
		if (!empty($cache)) {
			return $cache->data;
		}
	}

	// get recent subscribers
	$subscriber_uids = tm_subscriptions_user_get_new_subscribers($num_items);

	// load template
	$template = '<div class="tm-footer-lists-container subscriptions">
	<h2>__FOOTER_TITLE__</h2>
    __LIST_ITEMS__
</div>';

	// footer title
	$template = str_replace("__FOOTER_TITLE__", $conf["tm_subscriptions_user_subscribers_footer_title"], $template);

	// generate lists
	$list_html = "<ul class='tm-footer-lists subscriptions'>";
	foreach ($subscriber_uids as $uid) {

		// load account
		$account = user_load($uid);
		if ($account == null) {
			continue;
		}

		// get name
		$full_name = "User";
		if (isset($account->field_user_first_name[LANGUAGE_NONE][0]['value'])) {
			$full_name = strip_tags(trim($account->field_user_first_name[LANGUAGE_NONE][0]['value']));
		}

		if (isset($account->field_user_last_name[LANGUAGE_NONE][0]['value'])) {
			$full_name .= " " . strip_tags(trim($account->field_user_last_name[LANGUAGE_NONE][0]['value']));
		}

		// trim name
		if (isset($conf["tm_subscriptions_user_subscribers_footer_max_length"])) {
			$max_length = $conf["tm_subscriptions_user_subscribers_footer_max_length"];
			if (strlen($full_name) > $max_length) {
				$full_name = substr($full_name, 0, $max_length) . "...";
			}
		}

		// get member url
		$profile_url = drupal_get_path_alias("user/" . $account->uid);

		// render list
		$list_html .= "<li><span class='tm-footer-lists-label subscriptions'><a class='tm-footer-lists-label-link subscriptions' href='/" . $profile_url . "'>" . $full_name . "</a>";
		
		$list_html .= "</span></li>";
	}

	$list_html .= "</ul>";
	$html = str_replace("__LIST_ITEMS__", $list_html, $template);

	// add to cache
	cache_set('tm-subscriptions-footer', $html, 'cache', time() + 300); // 5 mins

	// return html
	return $html;

}
