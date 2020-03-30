<?php

// tm_match.display.inc

function tm_match_display_index() {

	global $conf;
	global $user;

	if (!user_is_logged_in()) {
		drupal_get_messages();
		drupal_set_message($conf["tm_users_feedback_cta"] . "<a href='/user/login?destination=/match'>Please log in to share feedback</a>.");
		drupal_goto("/");
	}

	// redirect if member is reported
	if (tm_users_is_member_reported($user->uid)) {
		drupal_goto("/");
	}

	// include css
	drupal_add_css(drupal_get_path('module', 'tm_match') . '/css/tm_match.css');

	// include list display js
	//drupal_add_js(drupal_get_path('module', 'tm_match') . '/js/tm_match_display.js');
	drupal_add_js(drupal_get_path('module', 'tm_match') . '/js/confetti.min.js');
	$js_file = file_get_contents(drupal_get_path('module', 'tm_match') . '/js/tm_match_display.js');

	// load template
	$template = file_get_contents(drupal_get_path('module', 'tm_match') .'/templates/tm_match_index.tpl.php');

	// check rate limit
	if (tm_match_check_rate_limit($user->uid)) {
		$results_html = tm_match_render_cards($user->uid, $conf["tm_match_load_cards"]);
	} else {
		$results_html = tm_match_render_card_message($conf["tm_match_rate_limit_title"], $conf["tm_match_rate_limit_message"]);
	}

	// html
	$html = str_replace("__INTRO__", $conf["tm_match_index_intro"], $template);		
	$html = str_replace("__RESULTS__", $results_html, $html);
	$html = str_replace("__MATCH_TIP__", "Swipe left or right", $html);
	$html .= "<script>\n" . $js_file . "\n" . "</script>";

	// create form
	$form['html'] = array(
		'#type' => 'item',
		'#markup' => $html,
		);
	return $form;

}

function tm_match_render_card_message($title, $message) {

	$template = file_get_contents(drupal_get_path('module', 'tm_match') .'/templates/tm_match_card_message.tpl.php');
	$html = str_replace("__TITLE__", $title, $template);
	$html = str_replace("__MESSAGE__", $message, $html);
	return $html;
}

function tm_match_render_cards($uid, $num_cards = 10) {

	global $conf;

	// colors
	$css_colors = array("purple", "blue", "indigo", "cyan", "lime", "pink");

	// load template
	$template = file_get_contents(drupal_get_path('module', 'tm_match') .'/templates/tm_match_card.tpl.php');

	// loading template
	$template_loading = file_get_contents(drupal_get_path('module', 'tm_match') .'/templates/tm_match_card_loading.tpl.php');
	$loading_message = $conf["tm_match_loading_messages"][rand(0,sizeof($conf["tm_match_loading_messages"])-1)];
	$template_loading = str_replace("__LOADING_MESSAGE__", $loading_message, $template_loading);

	$html = $template_loading;
	$uids = tm_match_suggest_uids($uid, $num_cards);

	// no more
	if (sizeof($uids) == 0) {
		return tm_match_render_card_message($conf["tm_match_no_cards_title"], $conf["tm_match_no_cards_message"]);
	}

	$count = 1;
	foreach ($uids as $uid) {
		
		// load account
		$account = user_load($uid);
		if ($account == null) {
			continue;
		}
		
		// last card
		// (cards are shown in reverse order)
		$is_last_card = ($count == 1);

		// get name
		$first_name = strip_tags($account->field_user_first_name[LANGUAGE_NONE][0]['value']);
		$last_name = strip_tags($account->field_user_last_name[LANGUAGE_NONE][0]['value']);
		$full_name = $first_name . " " . $last_name;

		// image url
		$image_url = image_style_url("avatar", _tm_users_get_avatar_uri($account));

		// job role, and name
		$sub_title = "";
		if (isset($account->field_job_role[LANGUAGE_NONE][0]['value'])) {
			$sub_title = strip_tags($account->field_job_role[LANGUAGE_NONE][0]['value']);
		}
		if (isset($account->field_job_organization[LANGUAGE_NONE][0]['value'])) {
			if ($sub_title != "") {
				$sub_title .= ", ";
			}
			$sub_title .= strip_tags($account->field_job_organization[LANGUAGE_NONE][0]['value']);
		}

		// location
		$location = "";
		if (isset($account->field_location_city[LANGUAGE_NONE][0]['value'])) {
			$location = strip_tags($account->field_location_city[LANGUAGE_NONE][0]['value']);
		}
		if ($location != "") {
			if ($sub_title != "") {
				$sub_title .= "<br>";
			}
			$sub_title .= $location;
		}

		// template fields
		$template_fields = array();
		$template_fields["__IMAGE_URL__"] = $image_url;
		$template_fields["__NAME__"] = $full_name;
		$template_fields["__TITLE__"] = $sub_title;
		$template_fields["__CSS_COLOR__"] = $css_colors[$count % sizeof($css_colors)];

		// render template
		$html .= tm_match_render_card($template, $uid, $is_last_card, $template_fields);

		$count++;
	}
	
	return $html;
}

function tm_match_render_card($template, $uid, $is_last_card, $template_fields) {

	$html = str_replace("__UID__", $uid, $template);

	$is_last_card_text = "false";
	if ($is_last_card) {
		$is_last_card_text = "true";
	}

	$html = str_replace("__IS_LAST_CARD__", $is_last_card_text, $html);

	// replace template field values
	foreach($template_fields as $field_name => $field_value) {
		$html = str_replace($field_name, $field_value, $html);
	}

	return $html;
}

function tm_match_fetch_cards_ajax() {

	global $conf;
	global $user;

	if (!user_is_logged_in()) {
		echo "Oops, you're not logged in.";
		drupal_exit();
	}

	// redirect if member is reported
	if (tm_users_is_member_reported($user->uid)) {
		echo "Oops, you can't fetch new members right now.";
		drupal_exit();
	}

	// check rate limit
	if (!tm_match_check_rate_limit($user->uid)) {
		echo tm_match_render_card_message($conf["tm_match_rate_limit_title"], $conf["tm_match_rate_limit_message"]);
		drupal_exit();
	}

	// load template
	$results_html = tm_match_render_cards($user->uid);

	// Return html
	echo $results_html;
	drupal_exit();
}
