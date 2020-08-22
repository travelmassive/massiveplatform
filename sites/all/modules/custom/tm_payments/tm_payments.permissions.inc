<?php

// tm_payments.permissions.inc - payment permission methods

/* Helper method to determine whether an event can have payments enabled 
 * Checks the following: 
 * - Moderator or administrator user
 * - If field_event_payment_type is not empty and not set to 'no_payments'
 * - If event has a chapter and chapter has reached threshold of members
 * - If enable_payments option is set for event chapter (todo)
 */
function _tm_payments_check_enable_payments($event_id = null, $chapter_id = null, $company_id = null) {

	global $conf;
	global $user;

	// By default, don't enable payment options
	$enable_payments = false;

  	// Check if moderator or administrator
	if ((in_array("moderator", $user->roles)) || (in_array("administrator", $user->roles))) {
		if (!(isset($_GET["hide_moderator_options"]))) {
			return true;
		}
	}

	// Check if field_event_payment_type is not empty and not set to 'no_payments'
	if ($event_id != null) {
		$event = node_load($event_id);
		if (isset($event->field_event_payment_type[LANGUAGE_NONE][0]['value'])) {
			if ($event->field_event_payment_type[LANGUAGE_NONE][0]['value'] != "no_payments") {
				return true;
			}
		}
	}

	// Check if event has chapter
	// Determine from field_chapter_payments_enabled
	// default - check if number of chapter members meets minimum
	// enabled - always enable payments
	// disabled - always disable payments

	if ($chapter_id != null) {

		$chapter = node_load($chapter_id);

		// Get chapter payment setting
		$chapter_payments_setting = "default";
		$enable_payments = false;
		if (isset($chapter->field_chapter_payments_enabled[LANGUAGE_NONE][0]['value'])) {
			$chapter_payments_setting = $chapter->field_chapter_payments_enabled[LANGUAGE_NONE][0]['value'];
		}

		// Case 1 - default - check if number of members meets minimum
		if ($chapter_payments_setting == 'default') {
			$num_members = _tm_chapters_get_num_members($chapter_id);
			$min_members = $conf["tm_payments_enable_chapter_min_members"];
			if ($min_members != 0) {
				$enable_payments = ($num_members >= $min_members);
			}
		}

		// Case 2 - enabled - always enable payments
		if ($chapter_payments_setting == 'enabled') {
			$enable_payments = true;
		}

		// Case 3 - disabled - always disable payments
		if ($chapter_payments_setting == 'disabled') {
			$enable_payments = false;
		}
		
	}

	// Check if event has company
	// Determine from field_company_payments_enabled
	// default - disabled
	// enabled - always enable payments
	// disabled - always disable payments

	if ($company_id != null) {

		$company = node_load($company_id);
		
		// Get chapter payment setting
		$company_payments_setting = "disabled";
		$enable_payments = false;
		if (isset($company->field_company_payments_enabled[LANGUAGE_NONE][0]['value'])) {
			$company_payments_setting = $company->field_company_payments_enabled[LANGUAGE_NONE][0]['value'];
		}
		// Case 2 - enabled - always enable payments
		if ($company_payments_setting == 'enabled') {
			$enable_payments = true;
		}

		// Case 3 - disabled - always disable payments
		if ($company_payments_setting == 'disabled') {
			$enable_payments = false;
		}
		
	}

	return $enable_payments;
}
