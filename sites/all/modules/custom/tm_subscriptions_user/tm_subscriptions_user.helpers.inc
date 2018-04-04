<?php

// tm_subscriptions_user.helpers.inc - helper methods for payments

/**
 * Check if we show a CTA to the user
 */
function tm_subscriptions_check_show_user_cta($uid, $signed_in_uid, $drush_command = false) {

	global $conf;

	// check if subscriptions are enabled
	if ($conf["tm_subscriptions_user_enabled"] == false) {
		return false;
	}

	// check if user is logged in
	if (!$drush_command) {
		if (!user_is_logged_in()) {
			return false;
		}
	}

	// load account
	if (!$drush_command) {
		$account = user_load($uid);
	} else {
		$account = user_load($uid, true); // don't load from cache when running from drush (performance)
	}

	// load signed in account
	if ($signed_in_uid == $uid) {
		$signed_in_account = $account;
	} else if (!$drush_command) {
		$signed_in_account = user_load($signed_in_uid);
	} else {
		$signed_in_account = user_load($signed_in_uid, true); // don't load from cache when running from drush (performance)
	}

	// user must be approved
	if ($conf["tm_subscriptions_user_cta_approved_account"]) {
		if (!in_array("approved user", $signed_in_account->roles)) {
			return false;
		}
	}
	
	// check if user is already subscribed
	if (tm_subscriptions_is_user_subscription_enabled($uid)) {

		// check if in renewal period
		if (!tm_subscriptions_check_user_renewal_period($account)) {
			return false;
		}
	}

	// TODO: check this logic
	// (note: allow moderators to upgrade other people's accounts)
	if ($uid != $signed_in_uid) {

		if (!in_array("moderator", $signed_in_account->roles)) {
			return false;
		}
	}

	// hide CTA to chapter leaders
	if (in_array("chapter leader", $signed_in_account->roles)) {
		return false;
	}

	// check that subscriptions are enabled in this country
	$subscription = tm_subscriptions_get_user_subscription($account);
	if ($subscription == null) {
		return false;
	}

	// allow showing CTA if member goes via /membership/invite link
	// retrive cookie set in from tm_subscriptions_user_invite_link
	// this bypasses check for chapter leader, company subscription, events, followers and account age
	if (isset($_COOKIE["tm_subscriptions_invite_user_cta"])) {
		if ($_COOKIE["tm_subscriptions_invite_user_cta"] == 1) {
			return true;
		}
	}

	// if subscription is private
	if ($subscription["private"] == true) {

		// check partner code
		$partner_code = strtoupper(tm_subscriptions_user_get_partner_code());
		if ($partner_code == "") {
			return false;
		}

		// check valid partner code
		if (!isset($subscription["partner_codes"][$partner_code])) {
			return false;
		}
	}

	// don't show if user has a subscribed company account
	if (module_exists("tm_subscriptions")) {
		$subscribed_companies = tm_subscriptions_get_users_subscribed_companies($uid);
		if (sizeof($subscribed_companies) > 0) {
			return false;
		}
	}

	// if attended minimum number of events
	$num_events = tm_users_get_num_registered_events($uid);
	if ($num_events >= $conf["tm_subscriptions_user_cta_min_events"]) {
		return true;
	}
	
	// if reached minimum account age
	$account_age_days = ((time() - $account->created) / (60 * 60 * 24));
	if ($account_age_days >= $conf["tm_subscriptions_user_cta_min_account_age"]) {
		return true;
	}

	// if reached minimum number of followers
	$num_following = tm_users_get_num_following($uid);
	if ($num_following >= $conf["tm_subscriptions_user_cta_min_following"]) {
		return true;
	}

	return false;
}

/**
 * Helper method to get subscription type of a user
 */
function tm_subscriptions_user_get_subscription_type($account) {

	if (isset($account->field_user_subscription_type[LANGUAGE_NONE][0]['value'])) {
		return $account->field_user_subscription_type[LANGUAGE_NONE][0]['value'];
	}

	// not set
	return null;
}

/**
 * Helper method to get subscription date of a user
 */
function tm_subscriptions_user_get_subscription_date($account) {

	if (isset($account->field_user_subscription_date[LANGUAGE_NONE][0]['value'])) {
		return $account->field_user_subscription_date[LANGUAGE_NONE][0]['value'];
	}

	// not set
	return null;
}

/**
 * Helper method to see if subscription has expired
 */
function tm_subscriptions_user_has_subscription_expired($account) {

	// check that user has subscription date
	$subscription_date = tm_subscriptions_user_get_subscription_date($account);
	if ($subscription_date == "") {
		return false;
	}

	$current_date = new DateTime('now', new DateTimeZone('UTC'));
	$subscription_datetime = new DateTime($subscription_date, new DateTimeZone("UTC"));
	return (($subscription_datetime->format('U') - $current_date->format('U')) < 0);
}

/**
 * Helper method to print visual dates when subscription expires
 */
function tm_subscriptions_user_get_subscription_expires_text($account) {

	// check that account has subscription date
	$subscription_date = tm_subscriptions_user_get_subscription_date($account);
	if ($subscription_date == "") {
		return "";
	}

	$current_date = new DateTime('now', new DateTimeZone('UTC'));
	$subscription_datetime = new DateTime($subscription_date, new DateTimeZone("UTC"));
	$time_until = _tm_event_time_elapsed($subscription_datetime->format('U') - $current_date->format('U'));
	if ($time_until == "0 seconds") {
	  return "Expired";
	}

	return $time_until;
}

/**
 * Check if user subscription enabled
 */
function tm_subscriptions_is_user_subscription_enabled($uid) {

	$user_subscription_enable = flag_get_flag('user_subscription_enable', NULL);
	return ($user_subscription_enable->is_flagged($uid));
}

/**
 * Check if user subscription is paid
 */
function tm_subscriptions_is_user_subscription_paid($uid) {

	$user_subscription_paid = flag_get_flag('user_subscription_paid', NULL);
	return ($user_subscription_paid->is_flagged($uid));
}

/**
 * Get user subscription pricing for country from $conf
 * Return null if no pricing available
 */
function tm_subscriptions_get_user_subscription($account) {

	if ($account == null) {
		return null;
	}

	global $conf;

	// get user's country
	$country_code = tm_subscriptions_user_get_country($account);

	// check subscription
	if (array_key_exists($country_code, $conf["tm_subscriptions_user"])) {
		return $conf["tm_subscriptions_user"][$country_code];
	}

	// default
	$default_country = $conf["tm_subscriptions_user_default"];
	if (array_key_exists($default_country, $conf["tm_subscriptions_user"])) {
		return $conf["tm_subscriptions_user"][$default_country];
	}

	// no pricing
	return null;
}

/**
 * Helper method to get pricing variable
 */
function tm_subscriptions_get_user_subscription_field($account, $key) {

	$pricing = tm_subscriptions_get_user_subscription($account);
	if ($pricing != null) {
		return $pricing[$key];
	}

	return "";
}

/**
 * Get partner code
 * from ?partner= or from cookie subscription_partner
 */
function tm_subscriptions_user_get_partner_code() {

	global $conf;

	$partner_code = "";

	// check ?partner=
	if (isset($_GET["partner"])) {
		$partner_code = $_GET["partner"];

		// If arriving at a ?partner= link then we show the CTA
		// set static variable
		$tm_force_show_cta = &drupal_static("tm_force_show_cta");
		$tm_force_show_cta = true;

		// hide for subsequent page loads
		setcookie("tm_subscriptions_hide_user_cta", 0, strtotime('+30 days'), "/", "." . $conf["tm_email_server_url_domain"]);


	} elseif (isset($_COOKIE['Drupal_visitor_subscription_partner'])) {
		// check cookie
		$partner_code = $_COOKIE['Drupal_visitor_subscription_partner'];
	}

	return trim($partner_code);
}

/**
 * Set partner code and company url cookie
 * Drupal.visitor.subscription_partner
 * Drupal.visitor.subscription_account_url
 * Drupal.visitor.subscription_account_first_name
 * Drupal.visitor.subscription_account_segment_l1
 * Drupal.visitor.subscription_account_segment_l2
 * Drupal.visitor.subscription_account_home_chapter
 */
function tm_subscriptions_user_set_partner_cookie($uid = null) {

	global $conf;
	$cookie_time = '+30 days';

	// get partner code
	$partner_code = tm_subscriptions_user_get_partner_code();

	// check ?partner=
	if ($partner_code != "") {
		setcookie("Drupal.visitor.subscription_partner", $partner_code, strtotime($cookie_time), "/");
	}

	// set subscription_account_url if uid set
	if ($uid != null) {

		// load user
		$account = user_load($uid);

		// account uri
		$uri = $conf["tm_email_server_http"] . "://" . $conf["tm_email_server_url_domain"] . "/" . drupal_get_path_alias('user/' . $uid);
		setcookie("Drupal.visitor.subscription_account_url", $uri, strtotime($cookie_time), "/");

		// first name
		$first_name = $account->field_user_first_name[LANGUAGE_NONE][0]['value'];
		setcookie("Drupal.visitor.subscription_first_name", $first_name, strtotime($cookie_time), "/");

		// home chapter name
		$home_chapter_name = "";
		if (sizeof($account->field_home_chapter) > 0) {
			$chapter_id = $account->field_home_chapter[LANGUAGE_NONE][0]['target_id'];
			$home_chapter = node_load($chapter_id);
			if ($home_chapter != null) {
				$home_chapter_name = $home_chapter->title;
			}
		}
		setcookie("Drupal.visitor.subscription_account_home_chapter", $home_chapter_name, strtotime($cookie_time), "/");

		// user segment
		$user_segment_l1 = "";
		$user_segment_l2 = "";
		$field_segment_tid = tm_users_get_taxonomy_tid($uid);
		if ($field_segment_tid != null){
			$term = taxonomy_term_load($field_segment_tid);
			// load parent term
			$parent_terms = taxonomy_get_parents($term->tid);
			if (sizeof($parent_terms) > 0) {
				$parent_term = current($parent_terms);
				$user_segment_l1 = $parent_term->name;
				$user_segment_l2 = $term->name;
			} else {
				$user_segment_l1 = $term->name;
				$user_segment_l2 = "";
			}
		}

		setcookie("Drupal.visitor.subscription_account_segment_l1", $user_segment_l1, strtotime($cookie_time), "/");
		setcookie("Drupal.visitor.subscription_account_segment_l2", $user_segment_l2, strtotime($cookie_time), "/");

		// set price_label from subscription
		// ie: Upgrade to Pro Membership for $99.
		$subscription = tm_subscriptions_get_user_subscription($account);
		$price_label = $subscription["price_label"];
		setcookie("Drupal.visitor.subscription_account_price_label", $price_label, strtotime($cookie_time), "/");

	}
}

/**
 * Determine how many days until subscription expiry
 * Return 0 if expired
 * Return -1 if no expiry date
 */
function tm_subscriptions_user_days_until_expiry($account) {

	// check that account has subscription date
	$subscription_date = tm_subscriptions_user_get_subscription_date($account);
	if ($subscription_date == "") {
		return -1;
	}

	// calcualte expiry
	$current_date = new DateTime('now', new DateTimeZone('UTC'));
	$subscription_datetime = new DateTime($subscription_date, new DateTimeZone("UTC"));
	$seconds_until_expiry = $subscription_datetime->format('U') - $current_date->format('U');
	$days_until_expiry = floor($seconds_until_expiry / (60 * 60 * 24));
	if ($days_until_expiry < 0) {
		return 0;
	}
	return $days_until_expiry;
}

/**
 * Check if the account is within the renewal period
 */
function tm_subscriptions_check_user_renewal_period($account) {

	global $conf;

	$renewal_period_days = $conf["tm_subscriptions_user_cta_renewal_period"];

	// get expiry days
	$days_until_expiry = tm_subscriptions_user_days_until_expiry($account);

	// check if has renewal date
	if ($days_until_expiry == -1) {
		return false;
	}

	// check if has expired
	if ($days_until_expiry == 0) {
		return false;
	}

	// check if in renewal period
	return ($days_until_expiry <= $renewal_period_days);
}

/**
 * Get country code of user
 */
function tm_subscriptions_user_get_country($account) {

	global $conf;

	// Step 1. User's country in account
	if (isset($account->field_user_country[LANGUAGE_NONE][0]['iso2'])) {
		$country_code = $account->field_user_country[LANGUAGE_NONE][0]['iso2'];
		return $country_code;
	}

	// Step 2. Users's home chapter country
	if (isset($account->field_home_chapter[LANGUAGE_NONE][0]['target_id'])) {
		$home_chapter_id = $account->field_home_chapter[LANGUAGE_NONE][0]['target_id'];
		$home_chapter = node_load($home_chapter_id);
		if (isset($home_chapter->field_country[LANGUAGE_NONE][0]['iso2'])) {
			$country_code = $home_chapter->field_country[LANGUAGE_NONE][0]['iso2'];
			return $country_code;
		}
	}

	// Step 3. Default country from $conf
	return $conf["tm_subscriptions_user_default_country"];

}

/**
 * Get subscription type for a user id
 * Quick database method
 */
function tm_subscriptions_user_get_type($uid) {

	$val = db_select('field_data_field_user_subscription_type', 'f')
		->fields('f', array('field_user_subscription_type_value'))
		->condition('entity_type', 'user')
		->condition('entity_id', $uid)
		->execute()
		->fetchField();
	
	return $val;
}

/**
 * Helper method to get chapter of subscriber
 * Returns home chapter code, or first joined chapter
 * Returns empty string if no chapter code
 */
function tm_subscriptions_user_get_chapter_code($account) {

	$chapter_code = "";

	// check for home chapter
	if (sizeof($account->field_home_chapter) > 0) {
		$chapter_id = $account->field_home_chapter[LANGUAGE_NONE][0]['target_id'];
		$home_chapter = node_load($chapter_id);
		if ($home_chapter != null) {
			$chapter_code = $home_chapter->field_chapter_shortcode[LANGUAGE_NONE][0]['value'];
		}
	} else {
		// use first chapter
		$chapter_ids = tm_users_get_chapters($account);
		if (sizeof($chapter_ids) > 0) {
			$first_chapter = node_load($chapter_ids[0]);
			if ($first_chapter != null) {
				$chapter_code = $first_chapter->field_chapter_shortcode[LANGUAGE_NONE][0]['value'];
			}
		}
	}

	return $chapter_code;
}

/**
 * Helper method to get country of subscriber
 * Returns home chapter country code, or first joined chapter
 * Returns empty string if no chapter code
 */
function tm_subscriptions_user_get_country_code($account) {

	$country_code = "";

	// get country code of user account
	if (isset($account->field_user_country[LANGUAGE_NONE][0]['iso2'])) {
  		$country_code = $account->field_user_country[LANGUAGE_NONE][0]['iso2'];
  	}

	// check for home chapter
	if (sizeof($account->field_home_chapter) > 0) {
		$chapter_id = $account->field_home_chapter[LANGUAGE_NONE][0]['target_id'];
		$home_chapter = node_load($chapter_id);
		if ($home_chapter != null) {
			if (isset($home_chapter->field_country[LANGUAGE_NONE][0]['iso2'])) {
  				$country_code = $home_chapter->field_country[LANGUAGE_NONE][0]['iso2'];
  			}
		}
	} else {
		// use first chapter
		$chapter_ids = tm_users_get_chapters($account);
		if (sizeof($chapter_ids) > 0) {
			$first_chapter = node_load($chapter_ids[0]);
			if ($first_chapter != null) {
				if (isset($first_chapter->field_country[LANGUAGE_NONE][0]['iso2'])) {
  					$country_code = $first_chapter->field_country[LANGUAGE_NONE][0]['iso2'];
  				}
			}
		}
	}

	return $country_code;
}
