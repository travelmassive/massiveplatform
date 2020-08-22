<?php

// tm_payments.helpers.inc - helper methods for payments

/*
 * Check if an event has payments enabled
 * Requires event object
 */
function _tm_payments_is_payment_enabled($event) {

	// Check partner code for free ticket
	// Deprecated below as we use a checkout page now
	/*$partner_code = _tm_payments_get_partner_code($event->nid);
	if ($partner_code != "") {
		if ($partner_code["display_amount"] == 0) {
			return false;
		}
	}*/

	// check for no_payments
	if (isset($event->field_event_payment_type[LANGUAGE_NONE][0]['value'])) {
		return ($event->field_event_payment_type[LANGUAGE_NONE][0]['value'] != "no_payments");
	}

	// deprecated, but support backwards compatibility for old events
	if (isset($event->field_event_payment_enabled[LANGUAGE_NONE][0]['value'])) {
		return ($event->field_event_payment_enabled[LANGUAGE_NONE][0]['value'] == 1);
	}

	// not enabled
	return false;
}

/*
 * Get payment type
 * return types:
 * no_payments - No Payment
 * fixed_price - Fixed Payment with Early Bird special
 * pay_what_you_wish - Pay What You Wish / Donation
 */
function _tm_payments_get_payment_type($event) {

	if (isset($event->field_event_payment_type[LANGUAGE_NONE][0]['value'])) {
		return $event->field_event_payment_type[LANGUAGE_NONE][0]['value'];
	} 

	return "no_payment";
}

/*
 * Check if an event has early bird enabled
 * Requires event object
 */
function _tm_payments_is_early_bird_enabled($event) {

	if (isset($event->field_event_payment_early_enable[LANGUAGE_NONE][0]['value'])) {
		return ($event->field_event_payment_early_enable[LANGUAGE_NONE][0]['value'] == 1);
	}

	// not enabled
	return false;
}

/*
 * Return amount to use with stripe api
 */
function _tm_payments_get_api_amount($amount, $currency) {

	// zero-decimal currencies
    // https://support.stripe.com/questions/which-zero-decimal-currencies-does-stripe-support
    $zero_decimal_currencies = array("bif", "clp", "djf", "gnf", "jpy", "kmf", "krw", "mga", "pyg", "rwf", "vnd", "vuv", "xaf", "xof", "xpf");

    $stripe_amount = (int)$amount; // integer
    if (!in_array(strtolower($currency), $zero_decimal_currencies)) {
    	$stripe_amount = $stripe_amount * 100; // add cents
    }

    return $stripe_amount;
}

/**
 * Fast lookup if member is registered for event
 */
function _tm_payments_user_has_paid($event_id, $uid) {

  $paid_flag_id = flag_get_flag('event_paid')->fid;
  $query = "SELECT uid FROM flagging WHERE entity_id = :event_id AND fid = :fid AND uid = :uid";
  $row = db_query($query, array(':event_id' => $event_id, ':fid' => $paid_flag_id, ':uid' => $uid, ))->fetch();
  return ($row != null);
}
