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
		return false;
	}

	// check if user is a manager of the organization
	if (!tm_organizations_is_company_profile_manager($uid, $nid)) {
		return false;
	}

	// check that subscriptions are enabled in this country
	$organization = node_load($nid);
	if (tm_subscriptions_get_organization_subscription($organization) == null) {
		return false;
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

	// check subsription
	if (array_key_exists($country_code, $conf["tm_subscriptions_organization"])) {
		return $conf["tm_subscriptions_organization"][$country_code];
	}

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
