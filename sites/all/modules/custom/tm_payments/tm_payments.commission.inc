<?php

// tm_payments.commission.inc - commission methods

/**
 * Get the commission % for an event
 */
function _tm_payments_get_event_commission($event) {

	// If commission set in event field, return it
	if (isset($event->field_event_payment_commission[LANGUAGE_NONE][0]['value'])) {
		return $event->field_event_payment_commission[LANGUAGE_NONE][0]['value'];
	}

	// If event has chapter, return chapter commission
	if (isset($event->field_chapter[LANGUAGE_NONE][0])) {
      $chapter_id = $event->field_chapter[LANGUAGE_NONE][0]['target_id'];
      return _tm_payments_get_chapter_commission($chapter_id);
    }

    // If event has company, return company commission
    if (isset($event->field_event_company[LANGUAGE_NONE][0])){
      $company_id = $event->field_event_company[LANGUAGE_NONE][0]['target_id'];
      return _tm_payments_get_company_commission($company_id);
    }

    // Return member commission of event owner
    $event_owner_uid = $event->uid;
    return _tm_payments_get_member_commission($event_owner_uid);
}

/**
 * Determine the default commission % for an event
 */
function _tm_payments_get_default_commission($typeof_event) {

	global $conf;

	if ($typeof_event == "chapter_event") {
		return $conf["tm_payments_commission_default_chapter"];
	}

	if ($typeof_event == "company_event") {
		return $conf["tm_payments_commission_default_company"];
	}

	if ($typeof_event == "member_event") {
		return $conf["tm_payments_commission_default_member"];
	}

	// shouldn't get here
	return null;
}

/**
 * Determine the commission % for an event
 */
function _tm_payments_get_commission($typeof_event, $typeof_id) {

	if ($typeof_event == "chapter_event") {
		return _tm_payments_get_chapter_commission($typeof_id);
	}

	if ($typeof_event == "company_event") {
		return _tm_payments_get_company_commission($typeof_id);
	}

	if ($typeof_event == "member_event") {
		return _tm_payments_get_member_commission($typeof_id);
	}

	// shouldn't get here
	return 0;
}

/**
 * Get the commission % for a chapter
 */
function _tm_payments_get_chapter_commission($chapter_id) {

	global $conf;

	// see if field_chapter_payment_commission is set
	$chapter = node_load($chapter_id);
	if (isset($chapter->field_chapter_payment_commission[LANGUAGE_NONE][0]['value'])) {
		return $chapter->field_chapter_payment_commission[LANGUAGE_NONE][0]['value'];
	}

	// otherwise, return default commission
	return _tm_payments_get_default_commission("chapter_event");
}

/**
 * Get the commission % for a company
 */
function _tm_payments_get_company_commission($company_id) {

	global $conf;

	// see if field_company_payment_commission is set
	$company = node_load($company_id);
	if (isset($company->field_company_payment_commission[LANGUAGE_NONE][0]['value'])) {
		return $company->field_company_payment_commission[LANGUAGE_NONE][0]['value'];
	}

	// otherwise, return default commission
	return _tm_payments_get_default_commission("company_event");
}

/**
 * Get the commission % for a member
 */
function _tm_payments_get_member_commission($chapter_id) {
	global $conf;
	return _tm_payments_get_default_commission("member_event");
}
