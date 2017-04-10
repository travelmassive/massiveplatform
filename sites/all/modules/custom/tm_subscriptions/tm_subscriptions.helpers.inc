<?php

// tm_subscriptions.helpers.inc - helper methods for payments

/**
 * Check if we show a CTA to the user
 */
function tm_subscriptions_check_show_organization_cta($nid, $uid) {

	global $conf;

	// check if subscriptions are enabled
	if ($conf["tm_subscriptions_organization_enabled"] == false) {
		return false;
	}

	// check if user is logged in
	if (!user_is_logged_in()) {
		return false;
	}

	// check if organization is already subscribed
	if (tm_subscriptions_is_organization_subscription_enabled($nid)) {

		// check if in renewal period
		$organization = node_load($nid);
		if (!tm_subscriptions_check_organization_renewal_period($organization)) {
			return false;
		}
	}

	// check if user is a manager of the organization
	if (!tm_organizations_is_company_profile_manager($uid, $nid)) {
		return false;
	}

	// check that subscriptions are enabled in this country
	$organization = node_load($nid);
	$subscription = tm_subscriptions_get_organization_subscription($organization);
	if ($subscription == null) {
		return false;
	}

	// if subscription is private
	if ($subscription["private"] == true) {

		// check partner code
		$partner_code = strtoupper(tm_subscriptions_get_partner_code());
		if ($partner_code == "") {
			return false;
		}

		// check valid partner code
		if (!isset($subscription["partner_codes"][$partner_code])) {
			return false;
		}
	}
 
	return true;
}

/**
 * Helper method to get subscription type of an organization
 */
function tm_subscriptions_get_subscription_type($organization) {

	if (isset($organization->field_company_subscription_type[LANGUAGE_NONE][0]['value'])) {
		return $organization->field_company_subscription_type[LANGUAGE_NONE][0]['value'];
	}

	// not set
	return null;
}

/**
 * Helper method to get subscription date of an organization
 */
function tm_subscriptions_get_subscription_date($organization) {

	if (isset($organization->field_company_subscription_date[LANGUAGE_NONE][0]['value'])) {
		return $organization->field_company_subscription_date[LANGUAGE_NONE][0]['value'];
	}

	// not set
	return null;
}

/**
 * Helper method to see if subscription has expired
 */
function tm_subscriptions_has_subscription_expired($organization) {

	// check that organization has subscription date
	$subscription_date = tm_subscriptions_get_subscription_date($organization);
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
function tm_subscriptions_get_subscription_expires_text($organization) {

	// check that organization has subscription date
	$subscription_date = tm_subscriptions_get_subscription_date($organization);
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
 * Helper method to get chapter referer id from organization
 */
function tm_subscriptions_get_referer_id($organization) {

	$referer_code = "";
	$owner_uid = $organization->uid;
	$owner_account = user_load($organization->uid);
	if ($owner_account != null) {
		if (sizeof($owner_account->field_home_chapter) > 0) {
			$chapter_id = $owner_account->field_home_chapter[LANGUAGE_NONE][0]['target_id'];
			$home_chapter = node_load($chapter_id);
			if ($home_chapter != null) {
				$referer_code = "chapter-" . $home_chapter->field_chapter_shortcode[LANGUAGE_NONE][0]['value'];
			}
		}
	}

	return $referer_code;
}

/**
 * Check if organization subscription enabled
 */
function tm_subscriptions_is_organization_subscription_enabled($nid) {

	$organization_subscription_enable = flag_get_flag('organization_subscription_enable', NULL);
	return ($organization_subscription_enable->is_flagged($nid));
}

/**
 * Check if organization subscription is paid
 */
function tm_subscriptions_is_organization_subscription_paid($nid) {

	$organization_subscription_paid = flag_get_flag('organization_subscription_paid', NULL);
	return ($organization_subscription_paid->is_flagged($nid));
}

/**
 * Get organization subscription pricing for country from $conf
 * Return null if no pricing available
 */
function tm_subscriptions_get_organization_subscription($organization) {

	if ($organization == null) {
		return null;
	}

	global $conf;

	// get country code
	$country_code = $organization->field_country[LANGUAGE_NONE][0]['iso2'];

	// check subscription
	if (array_key_exists($country_code, $conf["tm_subscriptions_organization"])) {
		return $conf["tm_subscriptions_organization"][$country_code];
	}

	// default
	$default_country = $conf["tm_subscriptions_organization_default"];
	if (array_key_exists($default_country, $conf["tm_subscriptions_organization"])) {
		return $conf["tm_subscriptions_organization"][$default_country];
	}

	// no pricing
	return null;
}

/**
 * Helper method to get pricing variable
 */
function tm_subscriptions_get_organization_subscription_field($organization, $key) {

	$pricing = tm_subscriptions_get_organization_subscription($organization);
	if ($pricing != null) {
		return $pricing[$key];
	}

	return "";
}

/**
 * Get partner code
 * from ?partner= or from cookie subscription_partner
 */
function tm_subscriptions_get_partner_code() {

	global $conf;

	$partner_code = "";

	// check ?partner=
	if (isset($_GET["partner"])) {
		$partner_code = $_GET["partner"];
	} elseif (isset($_COOKIE['Drupal_visitor_subscription_partner'])) {
		// check cookie
		$partner_code = $_COOKIE['Drupal_visitor_subscription_partner'];
	}

	return trim($partner_code);
}

/**
 * Set partner code and company url cookie
 * Drupal.visitor.subscription_partner
 * Drupal.visitor.subscription_company_url
 */
function tm_subscriptions_set_partner_cookie($nid = null) {

	global $conf;
	$cookie_time = '+30 days';

	// get partner code
	$partner_code = tm_subscriptions_get_partner_code();

	// check ?partner=
	if ($partner_code != "") {
		setcookie("Drupal.visitor.subscription_partner", $partner_code, strtotime($cookie_time), "/");
	}

	// set subscription_company_url if nid set
	if ($nid != null) {
		$uri = $conf["tm_email_server_http"] . "://" . $conf["tm_email_server_url_domain"] . "/" . drupal_get_path_alias('node/' . $nid);
		setcookie("Drupal.visitor.subscription_company_url", $uri, strtotime($cookie_time), "/");

	}
}

/**
 * Determine how many days until subscription expiry
 * Return 0 if expired
 * Return -1 if no expiry date
 */
function tm_subscriptions_organization_days_until_expiry($organization) {

	// check that organization has subscription date
	$subscription_date = tm_subscriptions_get_subscription_date($organization);
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
 * Check if the organization is within the renewal period
 */
function tm_subscriptions_check_organization_renewal_period($organization) {

	global $conf;

	$renewal_period_days = $conf["tm_subscriptions_organization_cta_renewal_period"];

	// get expiry days
	$days_until_expiry = tm_subscriptions_organization_days_until_expiry($organization);

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

