<?php

// tm_match.display.inc

function tm_match_display_index() {

	global $conf;
	global $user;

	$load_more = true;

	// include css
	drupal_add_css(drupal_get_path('module', 'tm_match') . '/css/tm_match.css');

	// include js
	drupal_add_js(drupal_get_path('module', 'tm_match') . '/js/tm_match_display.js');
	drupal_add_js(drupal_get_path('module', 'tm_match') . '/js/jquery.textfill.min.js');
	drupal_add_js(drupal_get_path('module', 'tm_match') . '/js/confetti.min.js');

	// load template
	$template = file_get_contents(drupal_get_path('module', 'tm_match') .'/templates/tm_match_index.tpl.php');
	// $js = file_get_contents(drupal_get_path('module', 'tm_match') .'/js/tm_match_display.js');
	// $template .= "<script>\n" . $js . "</script>";

	// hide tip js
	$hide_tip_js = "\n<script>
	jQuery(document).ready(function() {
		document.getElementsByClassName('tm_match__tip')[0].style.visibility = 'hidden';
	});</script>";

	// clear help tip messages
	drupal_get_messages("help-tip");

	$results_html = "";

	// demo mode
	if ($conf["tm_match_demo_mode"]) {
		drupal_set_message("We're in demo mode — notifications won't be sent when you swipe.", 'help-tip');
	}

	// check disabled
	if ($conf["tm_match_offline"]) {
		drupal_set_message("We're offline for a bit — come back later.", 'help-tip');
		$results_html = tm_match_render_card_message($conf["tm_match_no_cards_title"], $conf["tm_match_no_cards_message"]);
		$results_html .= $hide_tip_js;
	} elseif (tm_match_check_rate_limit($user->uid)) {
		$results_html = tm_match_render_cards($user->uid, $conf["tm_match_load_cards"]);
	} else {
		$results_html = tm_match_render_card_message($conf["tm_match_rate_limit_title"], $conf["tm_match_rate_limit_message"]);
		$results_html .= $hide_tip_js;
	}

	// redirect if member is reported
	if (tm_users_is_member_reported($user->uid)) {
		$results_html = tm_match_render_card_message($conf["tm_match_no_cards_title"], $conf["tm_match_no_cards_message"]);
		$results_html .= $hide_tip_js;
	}

	// not approved
	$is_approved = in_array("approved user", $user->roles);
	if (user_is_logged_in() and !$is_approved) {

		// generate approval link
		$approval_link = "javascript:jq_request_approval(" . $user->uid . ");";
		$who_flagged = flag_get_entity_flags("user", $user->uid, "approval_requested_by_user");
		if (sizeof($who_flagged) > 0) {
			$approval_link = "javascript:jq_approval_already_requested();";
		}

		// insert into help tip
		$message = $conf["tm_match_approved_member_message"];
		$message = str_replace("__UID__", $user->uid, $message);
		$message = str_replace("__APPROVAL_LINK__", $approval_link, $message);
		drupal_set_message($message, 'help-tip');

		// insert into card message
		$approved_member_card_message = $conf["tm_match_approved_member_card_message"];
		$approved_member_card_message = str_replace("__APPROVAL_LINK__", $approval_link, $approved_member_card_message);
		$results_html = tm_match_render_card_message($conf["tm_match_approved_member_card_title"], $approved_member_card_message);
		$results_html .= tm_match_render_cards($user->uid, 1, false);
		$load_more = false;
	}

	// not logged in
	if (!user_is_logged_in()) {
		drupal_set_message($conf["tm_match_sign_in_message"], 'help-tip');
		$results_html = tm_match_render_card_message($conf["tm_match_sign_in_card_title"], $conf["tm_match_sign_in_card_message"]);
		$results_html .= tm_match_render_cards($user->uid, 1, false);
		$load_more = false;
	}

	// html
	$html = $template;

	// settings
	$html = str_replace("__UID__", $user->uid, $html);		
	$html = str_replace("__LOAD_MORE__", intval($load_more), $html);

	// debug
	$debug_css = "display: none;";
	if (isset($_GET["debug"])) {
		$debug_css = "display: block;";
	}
	$html = str_replace("__DEBUG_CSS__", $debug_css, $html);

	// footer
	$footer = $conf["tm_match_footer_message"];
	$footer = str_replace("__NUM_MEMBERS__", tm_match_stats_total_users(), $footer);
	$footer = str_replace("__NUM_SWIPES__",  tm_match_stats_total_swipes(), $footer);
	$footer = str_replace("__NUM_MATCHES__", tm_match_stats_total_matches(), $footer);

	// fields
	$html = str_replace("__INTRO__", $conf["tm_match_index_intro"], $html);
	$html = str_replace("__RESULTS__", $results_html, $html);
	$html = str_replace("__MATCH_TIP__", "Swipe left or right ", $html);
	$html = str_replace("__MATCH_FOOTER__", $footer, $html);
	
	// create form
	$form['html'] = array(
		'#type' => 'item',
		'#markup' => $html,
		);
	return $form;

}

function tm_match_display_index_logged_out() {

	global $conf;
	global $user;

	// include css
	drupal_add_css(drupal_get_path('module', 'tm_match') . '/css/tm_match.css');

	// include list display js
	//drupal_add_js(drupal_get_path('module', 'tm_match') . '/js/tm_match_display.js');
	drupal_add_js(drupal_get_path('module', 'tm_match') . '/js/confetti.min.js');
	$js_file = file_get_contents(drupal_get_path('module', 'tm_match') . '/js/tm_match_display.js');

	// load template
	$template = file_get_contents(drupal_get_path('module', 'tm_match') .'/templates/tm_match_index.tpl.php');

	$results_html = tm_match_render_card_message($conf["tm_match_rate_limit_title"], $conf["tm_match_rate_limit_message"]);

	// html
	$html = str_replace("__INTRO__", $conf["tm_match_index_intro"], $template);
	$html = str_replace("__UID__", $user->uid, $html);		
	$html = str_replace("__RESULTS__", $results_html, $html);
	$html = str_replace("__MATCH_TIP__", "Swipe left or right ", $html);

	$debug_css = "display: none;";
	if (isset($_GET["debug"])) {
		$debug_css = "display: block;";
	}
	$html = str_replace("__DEBUG_CSS__", $debug_css, $html);

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

function tm_match_render_cards($uid, $num_cards = 10, $show_loading = true) {

	global $conf;

	// colors
	$css_colors = array("purple", "blue", "indigo", "cyan", "lime", "pink");

	// load template
	$template = file_get_contents(drupal_get_path('module', 'tm_match') .'/templates/tm_match_card.tpl.php');

	// loading template
	$template_loading = file_get_contents(drupal_get_path('module', 'tm_match') .'/templates/tm_match_card_loading.tpl.php');
	$loading_message = $conf["tm_match_loading_messages"][rand(0,sizeof($conf["tm_match_loading_messages"])-1)];
	$template_loading = str_replace("__LOADING_MESSAGE__", $loading_message, $template_loading);

	$html = "";
	if ($show_loading) {
		$html = $template_loading;
	}

	// get suggested uids
	if ($num_cards == 1) {
		// if only one card, use featured ids
		$featured_ids = $conf["tm_match_suggest_featured_uids"];
		shuffle($featured_ids);
		$uids = array_slice($featured_ids, 0, 1);
	} else {
		$uids = tm_match_suggest_uids($uid, $num_cards);
	}

	// no more
	if (sizeof($uids) == 0) {
		return tm_match_render_card_message($conf["tm_match_no_cards_title"], $conf["tm_match_no_cards_message"]);
	}

	// quick lookup, follow members flag id
	$follow_members_flag_id = flag_get_flag('follow_members')->fid;

	$count = 1;
	foreach ($uids as $card_uid) {
		
		// load account
		$account = user_load($card_uid);
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

		// if sub_title is empty, use first sentence of user description
		if (trim($sub_title) == "") {
			if (isset($account->field_about_me[LANGUAGE_NONE][0]['value'])) {
				$item_description = $account->field_about_me[LANGUAGE_NONE][0]['value'];
				$description_parts = explode(".", $item_description);
				$sub_title = strip_tags($description_parts[0]);
				if (sizeof($description_parts) > 1) {
					$sub_title .= ".";
				}
			}
		}

		// template fields
		$template_fields = array();
		$template_fields["__IMAGE_URL__"] = $image_url;
		$template_fields["__NAME__"] = $full_name;
		$template_fields["__TITLE__"] = $sub_title;
		$template_fields["__CSS_COLOR__"] = $css_colors[$count % sizeof($css_colors)];
		$template_fields["__MORE_LINK__"] = "<a target='_blank' href='/user/" . $card_uid . "'>View profile</a>";

		// follows you back
		$follows_you_back = tm_match_user_is_following($card_uid, $uid, $follow_members_flag_id);

		// render template
		$html .= tm_match_render_card($template, $card_uid, $first_name, $follows_you_back, $is_last_card, $template_fields);

		$count++;
	}
	
	return $html;
}

function tm_match_render_card($template, $uid, $first_name, $follows_you_back, $is_last_card, $template_fields) {

	$html = str_replace("__UID__", $uid, $template);
	$html = str_replace("__FIRST_NAME__", htmlentities($first_name), $html); // remove "

	$is_last_card_text = "false";
	if ($is_last_card) {
		$is_last_card_text = "true";
	}

	// follows you back
	$html = str_replace("__FOLLOWS_YOU_BACK__", intval($follows_you_back), $html);

	// is last card
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
