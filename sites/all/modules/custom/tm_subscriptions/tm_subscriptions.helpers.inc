<?php

// tm_subscriptions.helpers.inc - helper methods for payments

/**
 * Check if we show a CTA to the user
 */
function tm_subscriptions_check_show_organization_cta($nid, $uid, $drush_command = false) {

	global $conf;

	// check if subscriptions are enabled
	if ($conf["tm_subscriptions_organization_enabled"] == false) {
		return false;
	}

	// check if user is logged in
	if (!$drush_command) {
		if (!user_is_logged_in()) {
			return false;
		}
	}

	// check we are viewing organization
	if (!$drush_command) {
		$organization = node_load($nid);
	}
	else {
		$organization = node_load($nid, null, true); // don't load from cache when running from drush (performance)
	}

	if ($organization == null) {
		return false;
	}
	if ($organization->type != "organization") {
		return false;
	}	

	// check if organization is already subscribed
	if (tm_subscriptions_is_organization_subscription_enabled($nid)) {

		// check if in renewal period
		if (!tm_subscriptions_check_organization_renewal_period($organization)) {
			return false;
		}
	}

	// check if user is a manager of the organization
	if (!tm_organizations_is_company_profile_manager($uid, $nid)) {
		return false;
	}

	// check that subscriptions are enabled in this country
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

		// If arriving at a ?partner= link then we show the CTA
		// set static variable
		$tm_force_show_cta = &drupal_static("tm_force_show_cta");
		$tm_force_show_cta = true;

		// hide for subsequent page loads
		setcookie("tm_subscriptions_hide_organization_cta", 0, strtotime('+30 days'), "/", "." . $conf["tm_email_server_url_domain"]);

	} elseif (isset($_COOKIE['Drupal_visitor_subscription_partner'])) {
		// check cookie
		$partner_code = $_COOKIE['Drupal_visitor_subscription_partner'];
	}

	return trim($partner_code);
}

/**
 * Set partner code and company url cookie
 * Drupal.visitor.subscription_partner
 * Drupal.visitor.subscription_account_first_name
 * Drupal.visitor.subscription_company_url
 * Drupal.visitor.subscription_company_name
 * Drupal.visitor.subscription_company_segment_l1
 * Drupal.visitor.subscription_company_segment_l2
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

		// load company
		$company = node_load($nid);

		// first name
		global $user;
		$account = user_load($user->uid);
		$first_name = $account->field_user_first_name[LANGUAGE_NONE][0]['value'];
		setcookie("Drupal.visitor.subscription_first_name", $first_name, strtotime($cookie_time), "/");

		// uri
		$uri = $conf["tm_email_server_http"] . "://" . $conf["tm_email_server_url_domain"] . "/" . drupal_get_path_alias('node/' . $nid);
		setcookie("Drupal.visitor.subscription_company_url", $uri, strtotime($cookie_time), "/");

		// company name
		$company_name = $company->title;
		setcookie("Drupal.visitor.subscription_company_name", $company_name, strtotime($cookie_time), "/");

		// company segment
		$company_segment_l1 = "";
		$company_segment_l2 = "";
		$field_segment_tid = tm_organizations_get_taxonomy_tid($nid);
		if ($field_segment_tid != null){
			$term = taxonomy_term_load($field_segment_tid);
			// load parent term
			$parent_terms = taxonomy_get_parents($term->tid);
			if (sizeof($parent_terms) > 0) {
				$parent_term = current($parent_terms);
				$company_segment_l1 = $parent_term->name;
				$company_segment_l2 = $term->name;
			} else {
				$company_segment_l1 = $term->name;
				$company_segment_l2 = "";
			}
		}

		setcookie("Drupal.visitor.subscription_company_segment_l1", $company_segment_l1, strtotime($cookie_time), "/");
		setcookie("Drupal.visitor.subscription_company_segment_l2", $company_segment_l2, strtotime($cookie_time), "/");

		// set price_label from subscription
		// ie: Buy A Business Membership for $899.
		$pricing_label = tm_subscriptions_get_organization_pricing_label($company);
		if ($pricing_label != "") {
			setcookie("Drupal.visitor.subscription_company_price_label", $pricing_label, strtotime($cookie_time), "/");
		}
		
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

	// calculate expiry
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

/**
 * Check if user has a subscribed organization
 * Return array of subscribed companies
 */
function tm_subscriptions_get_users_subscribed_companies($uid) {

	// preload flag
	$organization_subscription_enable = flag_get_flag('organization_subscription_enable', NULL);

	$subscribed_companies = array();
	$nids = tm_organizations_get_users_companies($uid);
	foreach ($nids as $nid) {

		// use direct flag lookup for performance
		if ($organization_subscription_enable->is_flagged($nid)) {
			$subscribed_companies[] = $nid;
		}
	}

	return $subscribed_companies;
}

/**
 * Get subscription type for a organization id
 * Quick database method
 */
function tm_subscriptions_get_type($nid) {

	$val = db_select('field_data_field_company_subscription_type', 'f')
		->fields('f', array('field_company_subscription_type_value'))
		->condition('entity_type', 'node')
		->condition('entity_id', $nid)
		->execute()
		->fetchField();
	
	return $val;
}

/**
 * Helper method to get chapter of organization
 * Returns owner's home chapter code, or first joined chapter
 * Returns empty string if no chapter code
 */
function tm_subscriptions_get_chapter_code($organization) {

	$chapter_code = "";
	$owner_uid = $organization->uid;
	$account = user_load($organization->uid);
	
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
 * Helper method to get country code of organization
 * Returns owner's home chapter country code, or first joined chapter
 * Returns empty string if no country code
 */
function tm_subscriptions_get_country_code($organization) {

	$country_code = "";
	$owner_uid = $organization->uid;
	$account = user_load($organization->uid);
	
	// country code of organization
  	if (isset($organization->field_country[LANGUAGE_NONE][0]['iso2'])) {
  		$country_code = $organization->field_country[LANGUAGE_NONE][0]['iso2'];
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
