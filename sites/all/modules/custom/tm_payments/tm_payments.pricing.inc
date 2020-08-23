<?php

// tm_payments.pricing.inc - pricing methods

/* 
 * Return registration price and currency of event
 * Will return pricing even if payment is not enabled
 * Requires event object
 */
function _tm_payments_get_registration_price($event, $account = null, $currency_fx = null, $partner_code = "") {

	global $conf;
	global $user;

	// STEP 1. VALIDATE
	// get currency
	if (!isset($event->field_event_payment_currency[LANGUAGE_NONE][0]['value'])) {
		return null;
	}

	// get amount
	if (!isset($event->field_event_payment_amount[LANGUAGE_NONE][0]['value'])) {
		return null;
	}

	// get account
	if ($account == null) {
		$account = user_load($user->uid);
	}

	// check we have valid fx
	if (($currency_fx != null) and (!isset($conf["tm_payments_currencies_fx"][$currency_fx]))) {
		return;
	}

	// STEP 3. GET REGISTRATION CURRENCY AND PRICE
	$display_amount = floor($event->field_event_payment_amount[LANGUAGE_NONE][0]['value']); // remove cents
	$regular_amount = $display_amount; // keep track of regular price in case we have early bird price
	$currency = $event->field_event_payment_currency[LANGUAGE_NONE][0]['value'];
	$currency_symbol = "$"; // default
	$currency_prefix = "$"; // default
	$event_label = "Event Ticket";
	if (array_key_exists($currency, $conf["tm_payments_currency_symbols"])) {
		$currency_symbol = $conf["tm_payments_currency_symbols"][$currency];
		$currency_prefix = $conf["tm_payments_currencies"][$currency];
	}	

	// STEP 3. GET EARLY BIRD PRICING
	$early_bird_price = _tm_payments_get_early_bird_price($event);
	$early_bird_enabled = $early_bird_price["enabled"];
	$early_bird_message = $early_bird_price["message"];
	$early_bird_label = $early_bird_price["label"];

	// Override price with early bird price
	if ($early_bird_enabled) {
		$display_amount = $early_bird_price["early_bird_price"];
		$event_label = $early_bird_label;
	}

	// STEP 4. CURRENCY FX
	if ($currency_fx != null) {
		
		$currency_symbol = $conf["tm_payments_currency_symbols"][$currency_fx];
		$currency_prefix = $conf["tm_payments_currencies"][$currency_fx];

		// calculate conversion
		$to_usd = floatval(1.0 / $conf["tm_payments_currencies_fx"][$currency]);
		$from_usd = floatval($conf["tm_payments_currencies_fx"][$currency_fx]);

		// convert fx and round
		$display_amount = ceil($display_amount * $to_usd * $from_usd);
		$regular_amount = ceil($regular_amount * $to_usd * $from_usd);

		// set new currency
		$currency = $currency_fx;
	}

	// STEP 5. APPLY PARTNER OR SUBSCRIBER DISCOUNT
	$applicable_discount = _tm_payments_get_event_subscriber_discount($event, $account);

	// partner code overrides
	if ($partner_code != "") {
		$applicable_discount = _tm_payments_get_event_partner_code_discount($event, $partner_code);
	}

	// apply discount
	if ($applicable_discount >= 0) {
		$display_amount = floor($display_amount - ($display_amount * ($applicable_discount / 100)));
	}

	// STEP 6. CALCULATE SAVING MESSAGE
	$saving_message = "";
	if ($regular_amount > $display_amount) {
		$discount_amt = $regular_amount - $display_amount;
		$discount_pct = ($discount_amt / $regular_amount) * 100;
		$saving_message = "Save " . floor($discount_pct) . "%";
	}

	// STEP 7. CALCULATE STRIPE AMOUNT
	$stripe_amount = _tm_payments_get_api_amount($display_amount, $currency);	

	// STEP 8. CHECKOUT ACTION AND TICKET LABEL
	$checkout_action = "Purchase";
	$ticket_label = "Ticket";
	if ($early_bird_enabled) {
		$ticket_label = $early_bird_label;
		$checkout_action = "Purchase a " . $early_bird_label;
		if (in_array(strtolower($early_bird_label){0}, ['a','e','i','o','u'])) {
			$checkout_action = "Purchase an " . $early_bird_label;
		}
	}
	
	// STEP 9. PRICING LABEL
	$pricing_label = $checkout_action . " for " . $currency_symbol . intval($display_amount);
	if ($saving_message != "") {
		$pricing_label .= " &mdash; " . $saving_message;
	}

	// STEP 10. STRIPE DESCRIPTION
	$stripe_description = $ticket_label . ": " . strip_tags($event->title);
	
	// return pricing array
	return array("display_amount" => $display_amount, "stripe_currency" => $currency, "stripe_amount" => $stripe_amount, "currency_symbol" => $currency_symbol, "stripe_description" => $stripe_description, "currency_prefix" => $currency_prefix, "regular_amount" => $regular_amount, "early_bird_enabled" => $early_bird_enabled, "early_bird_message" => $early_bird_message,  "early_bird_label" => $early_bird_label, "saving_message" => $saving_message, "event_label" => $event_label, "partner_code" => $partner_code, "pricing_label" => $pricing_label, "checkout_action" => $checkout_action, "ticket_label" => $ticket_label);
}


/* 
 * Return early bird price of event if enabled
 * Returns null if no early bird pricing
 * Requires event object
 */
function _tm_payments_get_early_bird_price($event) {

	global $conf;

	// default return
	$ret = array("enabled" => false, "early_bird_price" => null, "message" => null, "label" => null);
	$early_bird_message = "";

	// Step 1. Check if early bird is enabled
	$early_bird_enabled = _tm_payments_is_early_bird_enabled($event);
	if (!$early_bird_enabled) {
		return $ret;
	}

	// Step 2. Check early bird amount
	if (!isset($event->field_event_payment_early_amount[LANGUAGE_NONE][0]['value'])) {
		return $ret;
	}

	// Step 3. Get early bird amount and label
	$early_bird_amount = floor($event->field_event_payment_early_amount[LANGUAGE_NONE][0]['value']); // remove cents
	$early_bird_label = $conf["tm_payments_early_bird_label"];
	if (isset($event->field_event_payment_early_label[LANGUAGE_NONE][0]['safe_value'])) {
		$early_bird_label = trim($event->field_event_payment_early_label[LANGUAGE_NONE][0]['safe_value']);
	}
	
	// Step 4. Get early bird seats
	$early_bird_seats = 0;
	if (isset($event->field_event_payment_early_seats[LANGUAGE_NONE][0]['value'])) {
		$early_bird_seats = $event->field_event_payment_early_seats[LANGUAGE_NONE][0]['value'];
	}

	// Step 5. Step get early bird end date
	$early_bird_end_date = null;
	if (sizeof($event->field_event_payment_early_date) > 0) {
		$early_bird_end_date = $event->field_event_payment_early_date[LANGUAGE_NONE][0]['value'];
	}

	// Step 6. Check if seats are left
	if ($early_bird_seats > 0) {

		// Check free seats
		$num_seats_left = _tm_events_seats_left($event);
		if ($num_seats_left == 0) {
			return array("enabled" => false, "early_bird_price" => null, "message" => "Sold out", "label" => $early_bird_label);
		}

		// Get number of tickets sold for event
		$event_paid_flag = flag_get_flag('event_paid');
		$num_tickets_sold = $event_paid_flag->get_count($event->nid);
		$num_seats_left = _tm_events_seats_left($event);
		$num_early_bird_tickets_available = $early_bird_seats - $num_tickets_sold;
		if (($num_seats_left != -1) && ($num_early_bird_tickets_available > $num_seats_left)) {
			$num_early_bird_tickets_available = $num_seats_left;
		}

		if ($num_early_bird_tickets_available <= 0)  {
			return array("enabled" => false, "early_bird_price" => null, "message" => "Sold out", "label" => $early_bird_label);
		}

		// Set message to number of seats left
		$early_bird_message = $num_early_bird_tickets_available . " ticket left at this price!";
		if ($num_early_bird_tickets_available > 1) {
			$early_bird_message = $num_early_bird_tickets_available . " tickets left at this price";
		}
		
	}

	// Step 7. Check if early bird end date is set
	if ($early_bird_end_date != null) {
		
		// Get early bird end date and event timezone
		$event_timezone = _tm_get_event_timezone($event);
		$early_bird_date = new DateTime($early_bird_end_date, new DateTimeZone($event_timezone));

		// Calculate how many seconds from now until end of early bird
		$current_date = new DateTime('now', new DateTimeZone('UTC'));
    	$seconds_until_early_bird_expires = $early_bird_date->format('U') - $current_date->format('U');

    	// Check if early bird has expired
    	if ($seconds_until_early_bird_expires <= 0) {
    		return array("enabled" => false, "early_bird_price" => null, "message" => "Expired", "label" => $early_bird_label);
    	}

    	// Set message to when early bird expires
		$time_until = _tm_event_time_elapsed($seconds_until_early_bird_expires);
		if ($early_bird_message != "") {
			$early_bird_message .= ". Offer expires in " . $time_until;
			$early_bird_message = str_replace("!.", "!", $early_bird_message);
		} else {
			$early_bird_message = "Offer expires in " . $time_until;
		}
	}

	// Step 8. Return early bird price and message
	return array("enabled" => true, "early_bird_price" => $early_bird_amount, "message" => $early_bird_message, "label" => $early_bird_label);

}

/* 
 * Return price and currency for "pay what you wish" event
 * Requires event object
 */
function _tm_payments_get_pay_what_you_wish_price($event, $account, $currency, $price, $event_label = "Event Ticket", $partner_code = "") {

	global $conf;
	global $user;

	$display_amount = $price;
	$regular_amount = $price;
	$currency_symbol = $conf["tm_payments_currency_symbols"][$currency];
	$currency_prefix = $conf["tm_payments_currencies"][$currency];
	$saving_message = "";
	$stripe_amount = _tm_payments_get_api_amount($display_amount, $currency);	
	$checkout_action = "Buy a ticket";
	$ticket_label = "Ticket";
	// Set ticket label to "Donation" if member is already registered
	if (tm_events_user_is_registered($event->nid, $account->uid) and ($partner_code != "PREVIEW")) {
		$ticket_label = "Donation";
	}
	$pricing_label = $checkout_action . " for " . $currency_symbol . intval($display_amount);
	$stripe_description = $ticket_label . ": " . strip_tags($event->title);
	$early_bird_enabled = false;
	$early_bird_message = "";
	$early_bird_label = "";
	
	// return pricing array
	return array("display_amount" => $display_amount, "stripe_currency" => $currency, "stripe_amount" => $stripe_amount, "currency_symbol" => $currency_symbol, "stripe_description" => $stripe_description, "currency_prefix" => $currency_prefix, "regular_amount" => $regular_amount, "early_bird_enabled" => $early_bird_enabled, "early_bird_message" => $early_bird_message,  "early_bird_label" => $early_bird_label, "saving_message" => $saving_message, "event_label" => $event_label, "partner_code" => $partner_code, "pricing_label" => $pricing_label, "checkout_action" => $checkout_action, "ticket_label" => $ticket_label);
}

/* 
 * Helper method to get lower price for event ticket
 */
function _tm_payments_get_lowest_price($event, $account, $currency, $partner_code = "") {

	global $conf;

	// get payment type
	// fixed_price or pay_what_you_wish
	$payment_type = _tm_payments_get_payment_type($event);

	// get subscriber discount
	$subscriber_discount_pct = _tm_payments_get_event_subscriber_discount($event, $account);
	
	// get partner discount
	$partner_discount_pct = _tm_payments_get_event_partner_code_discount($event, $partner_code);

	// get biggest discount
	$partner_discount = 1;
	$biggest_discount_pct = max($subscriber_discount_pct, $partner_discount_pct);
	if ($biggest_discount_pct >= 0) {
		$partner_discount = (100 - $biggest_discount_pct) / 100;
	}

	// check PREVIEW
	if ($partner_code == "PREVIEW") {
		$partner_discount = 1;
	}

	// case for 100% discount
	if ($partner_discount_pct == 100) {
		return 0;
	}

	// fixed price
	if ($payment_type == "fixed_price") {
		$pricing = _tm_payments_get_registration_price($event, $account, $currency);
		return $pricing["display_amount"] * $partner_discount;
	}

	// check payment tier
	if (!isset($event->field_event_payment_tiers[LANGUAGE_NONE][0]['value'])) {
		return 0;
	}

	// check empty payment tiers
	if (trim(strip_tags($event->field_event_payment_tiers[LANGUAGE_NONE][0]['value'])) == "") {
		return 0;
	}

	// get event currency
	$event_currency = "usd";
	if ($event->field_event_payment_currency[LANGUAGE_NONE][0]['value'] != null) {
		$event_currency = $event->field_event_payment_currency[LANGUAGE_NONE][0]['value'];
	}

	// pay what you wish
	if ($payment_type == "pay_what_you_wish") {
	
		$parts = explode(",", $event->field_event_payment_tiers[LANGUAGE_NONE][0]['value']);
	
		$lowest_value = PHP_INT_MAX;
		foreach ($parts as $part) {

			// split label
			$more_parts = explode("|", $part);
			$part = $more_parts[0];

			$price = preg_replace("/[^0-9.]/", "", $part);

			// special case, FREE
			if (strtoupper($part) == "FREE") {
				return 0;
			}

			// ignore non value
			if ($price == "") {
				continue;
			}

			// FX
			if ($currency != $event_currency) {

				// calculate conversion
				$to_usd = floatval(1.0 / $conf["tm_payments_currencies_fx"][$event_currency]);
				$from_usd = floatval($conf["tm_payments_currencies_fx"][$currency]);

				// convert fx and round
				$price = floor($price * $to_usd * $from_usd);
			}

			if ($price < $lowest_value) {
				$lowest_value = $price;
			}
		}

		// no lowest value found
		if ($lowest_value == PHP_INT_MAX) {
			$lowest_value = 0;
		}

		return floor($lowest_value * $partner_discount);
	}

	// not found
	return 0;
}

/*
 * Get partner codes from event
 */
function _tm_payments_get_event_partner_codes($event) {

	// partner codes
	if (!isset($event->field_event_payment_partner_code[LANGUAGE_NONE][0]['value'])) {
		return array();
	}

	// get field
	$partner_codes_field = $event->field_event_payment_partner_code[LANGUAGE_NONE][0]['value'];
	$partner_codes_parts = explode(",", $partner_codes_field);

	$partner_codes = array();
	foreach ($partner_codes_parts as $part) {
		$part = trim($part);
		$code_discount_parts = explode("=", $part);
		if (sizeof($code_discount_parts) != 2) {
			continue;
		}
		$partner_code = trim($code_discount_parts[0]);
		$code_discount_parts_pct = str_replace("%", "", $code_discount_parts[1]);
		$discount_pct = trim(abs(intval($code_discount_parts_pct)));
		$partner_codes[$partner_code] = $discount_pct;
	}

	// return partner codes
	return $partner_codes;

}

/*
 * Get discount percentage from event partner code
 * Return -1 if not found
 */
function _tm_payments_get_event_partner_code_discount($event, $partner_code) {

	// get partner codes
	$partner_codes = _tm_payments_get_event_partner_codes($event);
	foreach ($partner_codes as $code => $discount_pct) {
		if (strtoupper($code) == strtoupper(trim($partner_code))) {
			return $discount_pct;
		}
	}

	// special case, PREVIEW code
	if ($partner_code == "PREVIEW") {
		return 0;
	}

	// not found
	return -1;
}

/*
 * Get discount percentage from event subscriber discount
 * Return 0 if no discount
 */
function _tm_payments_get_event_subscriber_discount($event, $account) {

	// check for user subscriber discount
	if (!module_exists("tm_subscriptions_user")) {
		return 0;
	}

	// check logged in
	if ($account->uid == 0) {
		return 0;
	}

	// check if user is subscriber or chapter leader
	if (!(tm_subscriptions_is_user_subscription_enabled($account->uid)) and (!in_array("chapter leader", $account->roles))) {
		return 0;
	}

	// get discount from field
	$subscriber_discount = 0;
	if (isset($event->field_event_payment_sub_discount[LANGUAGE_NONE][0]['value'])) {
		$subscriber_discount = abs(intval($event->field_event_payment_sub_discount[LANGUAGE_NONE][0]['value']));
		if ($subscriber_discount > 100) {
			$subscriber_discount = 0;
		}	
	}

	// return discount
	return $subscriber_discount;

}
