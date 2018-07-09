<?php

// tm_payments.pricing.inc - pricing methods

/* 
 * Return registration price and currency of event
 * Will return pricing even if payment is not enabled
 * Requires event object
 */
function _tm_payments_get_registration_price($event, $ticket_number = null, $timestamp = null) {

	global $conf;

	// STEP 1. VALIDATE
	// get currency
	if (!isset($event->field_event_payment_currency[LANGUAGE_NONE][0]['value'])) {
		return null;
	}

	// get amount
	if (!isset($event->field_event_payment_amount[LANGUAGE_NONE][0]['value'])) {
		return null;
	}

	// STEP 3. GET REGISTRATION CURRENCY AND PRICE
	$display_amount = $event->field_event_payment_amount[LANGUAGE_NONE][0]['value'];
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
	$early_bird_price = _tm_payments_get_early_bird_price($event, $ticket_number = null, $timestamp = null);
	$early_bird_enabled = $early_bird_price["enabled"];
	$early_bird_message = $early_bird_price["message"];
	$early_bird_label = $early_bird_price["label"];

	// Override price with early bird price
	if ($early_bird_enabled) {
		$display_amount = $early_bird_price["early_bird_price"];
		$event_label = $early_bird_label;
	}

	// STEP 4. PARTNER CODE
	$partner_code = _tm_payments_get_partner_code($event->nid);
	if ($partner_code != "") {
		$early_bird_enabled = true;
		$display_amount = $partner_code["display_amount"];
		$event_label = "Partner Ticket";
		$early_bird_label = $partner_code["partner_label"];
		$early_bird_message = $partner_code["partner_message"];
	}

	// STEP 5. CALCULATE SAVING MESSAGE
	$saving_message = "";
	if ($regular_amount > $display_amount) {
		$discount_amt = $regular_amount - $display_amount;
		$discount_pct = ($discount_amt / $regular_amount) * 100;
		$saving_message = "Save " . floor($discount_pct) . "%";
	}

	// STEP 6. CALCULATE STRIPE AMOUNT
	$stripe_amount = _tm_payments_get_api_amount($display_amount, $currency);
	
	// return pricing array
	return array("display_amount" => $display_amount, "stripe_currency" => $currency, "stripe_amount" => $stripe_amount, "currency_symbol" => $currency_symbol, "currency_prefix" => $currency_prefix, "regular_amount" => $regular_amount, "early_bird_enabled" => $early_bird_enabled, "early_bird_message" => $early_bird_message,  "early_bird_label" => $early_bird_label, "saving_message" => $saving_message, "event_label" => $event_label);
}


/* 
 * Return early bird price of event if enabled
 * Returns null if no early bird pricing
 * Requires event object
 */
function _tm_payments_get_early_bird_price($event, $ticket_number = null, $timestamp = null) {

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
	$early_bird_amount = $event->field_event_payment_early_amount[LANGUAGE_NONE][0]['value'];
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
	
	// Step 6. Check either either field_event_payment_early_seats or field_event_payment_early_date is set
	// Note: This check is optional
	//if (($early_bird_seats == 0) && ($early_bird_end_date == null)) {
		//return $ret;
	//}

	// Step 7. Check if seats are left
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

	// Step 8. Check if early bird end date is set
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

	// Step 9. Return early bird price and message
	return array("enabled" => true, "early_bird_price" => $early_bird_amount, "message" => $early_bird_message, "label" => $early_bird_label);

}

/**
 * Get partner code
 * from ?partner= or from cookie Drupal_visitor_event_partner_code
 */
function _tm_payments_get_partner_code($event_id, $set_message = false) {

	global $conf;
	$partner_code = "";

	// check ?partner=
	if (isset($_GET["partner"])) {

		// set partner code
		$display_partner_code = trim($_GET["partner"]);
		$partner_code = strtoupper(trim($_GET["partner"]));

		setcookie("Drupal.visitor.event_partner_code", $event_id . "_" . $partner_code, strtotime('+30 days'), "/", "." . $conf["tm_email_server_url_domain"]);

		// set message
		if ($set_message) {
			drupal_get_messages("help-tip");
			if (isset($conf["tm_payments_event_partner_codes"][$partner_code])) {
				drupal_set_message("Way to go! The partner code <em>" . strip_tags($display_partner_code) . "</em> has been applied to this event.", "help-tip");
			} else {
				drupal_set_message("Sorry, the partner code <em>" . strip_tags($display_partner_code) . "</em> is not valid or has expired.", "help-tip");
			}
		}

	} elseif (isset($_COOKIE['Drupal_visitor_event_partner_code'])) {

		// check cookie
		// must match event
		// ie: 1234_code
		$parts = explode("_", $_COOKIE['Drupal_visitor_event_partner_code'], 2);
		$partner_event_id = $parts[0];
		$partner_code = "";
		if (isset($parts[1])) {
			$partner_code = $parts[1];
		}
		
		// Check event_id
		if ($partner_event_id != $event_id) {
			return "";
		}
	}

	// get partner code from $conf['tm_payments_event_partner_codes']
	if (isset($conf["tm_payments_event_partner_codes"][$partner_code])) {
		return $conf["tm_payments_event_partner_codes"][$partner_code];
	}

	// partner code not found
	return "";
}
