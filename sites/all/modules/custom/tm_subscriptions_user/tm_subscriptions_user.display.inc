<?php

// tm_user_subscriptions.display.inc - display methods

/**
 * Implements template_preprocess_page()
 */
function tm_subscriptions_user_preprocess_page(&$variables) {

	global $conf;
	global $user;

	// Show pro member CTA
	if (current_path() == "subscribers") {
		if (tm_subscriptions_is_user_subscription_enabled($user->uid)) {
			$message = $conf["tm_subscriptions_user_subscribers_page_message_subscribed"];
		} else {
			$message = $conf["tm_subscriptions_user_subscribers_page_message"];
		}
		drupal_set_message($message, "help-tip");
	}
}

/**
 * Implement views_post_execute
 */
function tm_subscriptions_user_views_post_execute(&$view) {

	global $conf;

	if ($view->name == 'subscribed_members') {
		if (isset($conf["tm_subscriptions_user_subscribers_page_title"])) {
			$view->build_info['title'] = $conf["tm_subscriptions_user_subscribers_page_title"];
		}
	}
}

/**
 * Implement hook form alter
 */
function tm_subscriptions_user_form_alter(&$form, $form_state, $form_id) {

  global $conf;

  // Alter subscribers view dropdowns
  if (isset($form_state['view'])) {

    if ($form_state['view']->name == 'subscribed_members') {

      // Limit countries dropdown to users countries
      $output = '';
      $output = cache_get('views-exposed-form-subscribers-countries', 'cache');
      if (empty($output)) {
        $approved_member_role = user_role_load_by_name("subscriber")->rid;
        $result = db_query("SELECT * FROM (SELECT DISTINCT iso2, name, entity_id from {field_data_field_user_country} f LEFT JOIN {countries_country} c ON f.field_user_country_iso2 = c.iso2 WHERE f.bundle = 'user' AND c.iso2 IS NOT NULL) as countries_with_users INNER JOIN {users_roles} u ON u.uid = countries_with_users.entity_id AND u.rid = " . intval($approved_member_role) . " ORDER BY name ASC");
        $defaultoptions = array('All'=>'- Any -');
        $output = $defaultoptions + $result->fetchAllKeyed();
        $form['name_list']['#options'] = $output;
      cache_set('views-exposed-form-subscribers-countries', $output, 'cache', time() + 3600);
      } else{
        $form['name_list']['#options'] = $output->data;
      }

      // Replace "All" option to more meaningful name
      $form['name_list']['#options']['All'] = "Country";

    }
  }
}


/**
 * Impement Hook node view
 * Expire pro account if it has expired and manager of account views it
 */
function tm_subscriptions_user_user_view($account, $view_mode, $langcode) {

	global $user;

	if (!user_is_logged_in()) {
		return;
	}

	if ($view_mode != "full") {
		return;
	}

	// check if user is viewing their own account
	if ($user->uid != $account->uid) {
		return false;
	}

	// remove subscription if it has expired
	// this will happen if a manager of the organization views their own profile
	if (tm_subscriptions_user_has_subscription_expired($account)) {

		global $conf;

		$flag = flag_get_flag('user_subscription_enable');

		if ($flag->is_flagged($account->uid)) {

			$flag->flag('unflag', $account->uid, null, true);

			// send expiry message
			tm_subscriptions_user_send_notification($account, "expired");

			drupal_set_message($conf["tm_subscriptions_user_expired_message"], "help-tip");
			drupal_goto("user/" . $user->uid);
		}
	}

}

/**
 * Render subscription CTA banner for nodes
 * Called from page--node.tpl.php
 */
function tm_subscriptions_user_cta_banner_node($user_id, $node_id) {

	global $conf;
	$node = node_load($node_id);

	if ($node == null) {
		return;
	}

	// show on chapter page if member is in the chapter
	if ($node->type == "chapter") {
		$account = user_load($user_id);
		if ($account != null) {
			$chapters = tm_users_get_chapters($account);
			if (in_array($node_id, $chapters)) {
				return tm_subscriptions_user_cta_banner($user_id, $node);
			}
		}
	}

	// show on event page if member is in the event
	if ($node->type == "event") {

		// don't display if livestream is on
		if (tm_event_livestream_is_on($node)) {
			return;
		}

		$account = user_load($user_id);
		if ($account != null) {
			if (isset($node->field_chapter[LANGUAGE_NONE][0]['target_id'])) {
				$chapter_id = $node->field_chapter[LANGUAGE_NONE][0]['target_id'];
				$chapters = tm_users_get_chapters($account);
				if (in_array($chapter_id, $chapters)) {
					return tm_subscriptions_user_cta_banner($user_id, $node);
				}
			}
		}
	}

}

/**
 * Render subscription CTA banner
 * Called from page--user.tpl.php
 */
function tm_subscriptions_user_cta_banner($user_id, $display_chapter = null) {

	global $conf;
	global $user;

	$account = user_load($user_id);
	if ($account == null) {
		return;
	}

	// don't show if /user path shown
	// example: /user/123/edit
	if (arg(2) != null) {
		return;
	}

	// check if we can show cta to user
	if (!tm_subscriptions_check_show_user_cta($account->uid, $user->uid)) {
		return;
	}

	// get country pricing and subscription
	$country_subscription = tm_subscriptions_get_user_subscription($account);

	// retrive static variable from tm_subscriptions_user_get_partner_code if ?partner code 
	$tm_force_show_cta = &drupal_static("tm_force_show_cta");
	
	// if ?preview_subscription_cta is set
	if (isset($_GET["preview_subscription_cta"])) {
		$tm_force_show_cta = true;
	}

	// get cookie
	$wrapper_style = "";
	$hide_cta_cookie_name = $conf["tm_subscriptions_user_hide_cta_cookie"];
	if ((!$tm_force_show_cta) and (isset($_COOKIE[$hide_cta_cookie_name]))) {
		if ($_COOKIE[$hide_cta_cookie_name] == 1) {
			$wrapper_style = "display: none";
		}
	}

	$wrapper = "<div id='tm-subscriptions-cta-banner' style='" . $wrapper_style . "' class='messages--page_notice messages page_notice member-subscription'>__CONTENT__</div>";

	// show subscription html
	$inner_html = $country_subscription["html"];

	// show renewal html if in renewal period
	if (tm_subscriptions_check_user_renewal_period($account)) {
		$inner_html = $country_subscription["html_renew"];
	}

	// if account can be renewed and ?preview_subscription_cta
	if ((tm_subscriptions_user_days_until_expiry($account) > 0) and (isset($_GET["preview_subscription_cta"]))) {
		$inner_html = $country_subscription["html_renew"];
	}

	// replace template fields
	$inner_html = tm_subscriptions_user_replace_template_fields($account, $inner_html, $display_chapter);

	// display pricing
	$payment_display = "none";
	
	// if ? partner link 
	if ($tm_force_show_cta) {
		$payment_display = "block";
	}

	// if show user payment
	if (isset($_COOKIE["tm_subscriptions_show_user_payment"])) {
		if ($_COOKIE["tm_subscriptions_show_user_payment"] == 1) { 
			$payment_display = "block";
		}       
	}

	// if via user membership/invite link
	if (isset($_COOKIE["tm_subscriptions_invite_user_cta"])) {
		if ($_COOKIE["tm_subscriptions_invite_user_cta"] == 1) { 
			$payment_display = "block";
		}       
	}

	// if preview cta 
	if (isset($_GET["preview_subscription_cta"])) {
		$payment_display = "none";
	}

	$inner_html = str_replace("__PAYMENT_DISPLAY__", $payment_display, $inner_html);

	// insert into inner html
	$html = str_replace("__CONTENT__", $inner_html, $wrapper);

	return $html;
}

/**
 * Render subscription CTA display suite
 */
function _tm_subscriptions_user_cta_ds($field, $title = NULL) {

	global $conf;
	global $user;

	$user_id = $field['entity']->uid;
	$account = $field['entity'];

	// check if we show cta
	$html = "";
	if (tm_subscriptions_check_show_user_cta($user_id, $user->uid)) {

		// set partner code
		tm_subscriptions_user_set_partner_cookie($user_id);

		// check if we are in subscription renewal period
		if (tm_subscriptions_check_user_renewal_period($account)) {
			return "";
		}

		// Show call to action
		$html = $conf["tm_subscriptions_user_cta_text"];

		// Add learn more
		$html .= " &middot; <a class='show-cta-banner-user' target='_blank' href='" . tm_subscriptions_get_user_subscription_field($account, "learn_more_url") . "'>Learn more</a>";
	}

	return $html;
}

/**
 * Render subscription CTA display suite
 */
function _tm_subscriptions_user_badge_ds($field, $title = NULL) {

	global $conf;
	global $user;

	$user_id = $field['entity']->uid;
	$account = $field['entity'];

	$html = "";
	if (tm_subscriptions_is_user_subscription_enabled($user_id)) {
		$html = $conf["tm_subscriptions_user_label_noun"];

		// show extra information to other users
		// check if own account
		if ($user->uid == $user_id) {

			// show renewal
			if (tm_subscriptions_check_user_renewal_period($account)) {
				$html .= " (Expires in " . tm_subscriptions_user_get_subscription_expires_text($account) . ")";
			}

			// show support link
			$html .= " &middot; <a class='show-cta-banner-user' target='_blank' href='" . tm_subscriptions_get_user_subscription_field($account, "support_url") . "'>Customer Support</a>";

		} else {

			// show learn more link
			$html .= " &middot; <a class='show-cta-banner-user' target='_blank' href='" . tm_subscriptions_get_user_subscription_field($account, "learn_more_url") . "'>Learn more</a>";

		}
	}

	return $html;
}

/**
 * Render new subscribers
 */
function tm_subscriptions_user_render_subscribers($limit = 5, $exclude_uid = null) {

	global $conf;

	$html = "";
	$new_subscriber_uids = tm_subscriptions_user_get_new_subscribers($limit, $exclude_uid);

	foreach ($new_subscriber_uids as $new_subscriber_uid) {

		// load account
		$account = user_load($new_subscriber_uid);
		if ($account == null) {
			continue;
		}

		// Set avatar
		$avatar_url = image_style_url("avatar_grid", _tm_users_get_avatar_uri($account));
		$img_url = str_replace("'", "%27", file_create_url($avatar_url)); // to fix
		$user_url = drupal_get_path_alias("user/" . $account->uid);
		$user_full_name = strip_tags($account->field_user_first_name[LANGUAGE_NONE][0]['value']) . " " . strip_tags($account->field_user_last_name[LANGUAGE_NONE][0]['value']);
		$user_full_name = str_replace("''", "", $user_full_name);

		$html .= "<a target='_blank' title='" . $user_full_name . "' href='/" . $user_url . "'><img class='tm-subscribers-user-avatar' src='" . $img_url . "'></a> ";
	}

	return $html;
}
